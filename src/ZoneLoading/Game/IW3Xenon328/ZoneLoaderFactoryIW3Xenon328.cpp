#include "ZoneLoaderFactoryIW3Xenon328.h"

#include "ContentLoaderIW3Xenon328.h"
#include "Game/GameLanguage.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "Game/IW3Xenon328/ZoneConstantsIW3Xenon328.h"
#include "Loading/Exception/InvalidVersionException.h"
#include "Loading/Processor/ProcessorAuthedBlocks.h"
#include "Loading/Processor/ProcessorCaptureData.h"
#include "Loading/Processor/ProcessorInflate.h"
#include "Loading/Steps/StepAddProcessor.h"
#include "Loading/Steps/StepAllocXBlocks.h"
#include "Loading/Steps/StepLoadHash.h"
#include "Loading/Steps/StepLoadSignature.h"
#include "Loading/Steps/StepLoadZoneContent.h"
#include "Loading/Steps/StepRemoveProcessor.h"
#include "Loading/Steps/StepSkipBytes.h"
#include "Loading/Steps/StepVerifyFileName.h"
#include "Loading/Steps/StepVerifyHash.h"
#include "Loading/Steps/StepVerifyMagic.h"
#include "Loading/Steps/StepVerifySignature.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

#include <cassert>
#include <cstring>
#include <type_traits>

namespace IW3Xenon328
{
    namespace
    {
        class StepVerifyVersion final : public ILoadingStep
        {
        public:
            void PerformStep(ZoneLoader& zoneLoader, ILoadingStream& stream) override
            {
                uint32_t version;
                stream.Load(&version, sizeof(version));
                version = endianness::FromBigEndian(version);
                if (version != ZoneConstants::ZONE_VERSION)
                    throw InvalidVersionException(ZoneConstants::ZONE_VERSION, version);
            }
        };

        void SetupBlock(ZoneLoader& zoneLoader)
        {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

            zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon328::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
            zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon328::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
            zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon328::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
            zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon328::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
            zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon328::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
        }

        std::unique_ptr<cryptography::IPublicKeyAlgorithm> SetupRsa(const bool isOfficial)
        {
            if (isOfficial)
            {
                auto rsa = cryptography::CreateRsa(cryptography::HashingAlgorithm::RSA_HASH_SHA256, cryptography::RsaPaddingMode::RSA_PADDING_PSS);

                if (!rsa->SetKey(ZoneConstants::RSA_PUBLIC_KEY_INFINITY_WARD, sizeof(ZoneConstants::RSA_PUBLIC_KEY_INFINITY_WARD)))
                {
                    con::error("Invalid public key for signature checking");
                    return nullptr;
                }

                return rsa;
            }

            assert(false);

            return nullptr;
        }

        void AddAuthHeaderSteps(const ZoneLoaderInspectionResult& inspectResult, ZoneLoader& zoneLoader, const std::string& fileName)
        {
            if (!inspectResult.m_is_signed)
                return;

            auto rsa = SetupRsa(inspectResult.m_is_official);

            zoneLoader.AddLoadingStep(step::CreateStepVerifyMagic(ZoneConstants::MAGIC_AUTH_HEADER));
            zoneLoader.AddLoadingStep(step::CreateStepSkipBytes(4)); // Skip reserved

            auto subHeaderHash = step::CreateStepLoadHash(sizeof(DB_AuthHash::bytes), 1);
            auto* subHeaderHashPtr = subHeaderHash.get();
            zoneLoader.AddLoadingStep(std::move(subHeaderHash));

            auto subHeaderHashSignature = step::CreateStepLoadSignature(sizeof(DB_AuthSignature::bytes));
            auto* subHeaderHashSignaturePtr = subHeaderHashSignature.get();
            zoneLoader.AddLoadingStep(std::move(subHeaderHashSignature));

            zoneLoader.AddLoadingStep(step::CreateStepVerifySignature(std::move(rsa), subHeaderHashSignaturePtr, subHeaderHashPtr));

            auto subHeaderCapture = processor::CreateProcessorCaptureData(sizeof(DB_AuthSubHeader));
            auto* subHeaderCapturePtr = subHeaderCapture.get();
            zoneLoader.AddLoadingStep(step::CreateStepAddProcessor(std::move(subHeaderCapture)));

            zoneLoader.AddLoadingStep(step::CreateStepVerifyFileName(fileName, sizeof(DB_AuthSubHeader::fastfileName)));
            zoneLoader.AddLoadingStep(step::CreateStepSkipBytes(4)); // Skip reserved

            auto masterBlockHashes =
                step::CreateStepLoadHash(sizeof(DB_AuthHash::bytes), static_cast<unsigned>(std::extent_v<decltype(DB_AuthSubHeader::masterBlockHashes)>));
            auto* masterBlockHashesPtr = masterBlockHashes.get();
            zoneLoader.AddLoadingStep(std::move(masterBlockHashes));

            zoneLoader.AddLoadingStep(step::CreateStepVerifyHash(cryptography::CreateSha256(), 0, subHeaderHashPtr, subHeaderCapturePtr));
            zoneLoader.AddLoadingStep(step::CreateStepRemoveProcessor(subHeaderCapturePtr));

            zoneLoader.AddLoadingStep(step::CreateStepSkipBytes(ZoneConstants::AUTHED_CHUNK_SIZE - sizeof(DB_AuthHeader)));

            zoneLoader.AddLoadingStep(step::CreateStepAddProcessor(
                processor::CreateProcessorAuthedBlocks(ZoneConstants::AUTHED_CHUNK_COUNT_PER_GROUP,
                                                       ZoneConstants::AUTHED_CHUNK_SIZE,
                                                       static_cast<unsigned>(std::extent_v<decltype(DB_AuthSubHeader::masterBlockHashes)>),
                                                       cryptography::CreateSha256(),
                                                       masterBlockHashesPtr)));
        }
    } // namespace

    std::optional<ZoneLoaderInspectionResult> ZoneLoaderFactory::InspectZoneHeader(ZoneDataPeeking& filePeek) const
    {
        const auto& header = filePeek.PeekStruct<ZoneHeader>();
        const auto* headerBytes = reinterpret_cast<const uint8_t*>(&header.m_version);

        if (!memcmp(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, std::char_traits<char>::length(ZoneConstants::MAGIC_UNSIGNED)))
        {
            // The compressed stream follows the magic without a version field.
            if (headerBytes[0] != 0x78u)
                return std::nullopt;

            return ZoneLoaderInspectionResult{
                .m_game_id = GameId::IW3Xenon328,
                .m_endianness = GameEndianness::BE,
                .m_word_size = GameWordSize::ARCH_32,
                .m_platform = GamePlatform::XBOX,
                .m_is_official = true,
                .m_is_signed = false,
                .m_is_encrypted = false,
            };
        }

        if (!memcmp(header.m_magic, ZoneConstants::MAGIC_SIGNED, std::char_traits<char>::length(ZoneConstants::MAGIC_SIGNED)))
        {
            // The auth header follows the magic without a version field.
            if (memcmp(headerBytes, ZoneConstants::MAGIC_AUTH_HEADER, sizeof(header.m_version)) != 0)
                return std::nullopt;

            return ZoneLoaderInspectionResult{
                .m_game_id = GameId::IW3Xenon328,
                .m_endianness = GameEndianness::BE,
                .m_word_size = GameWordSize::ARCH_32,
                .m_platform = GamePlatform::XBOX,
                .m_is_official = true,
                .m_is_signed = true,
                .m_is_encrypted = false,
            };
        }

        return std::nullopt;
    }

    std::unique_ptr<ZoneLoader> ZoneLoaderFactory::CreateLoaderForHeader(ZoneDataPeeking& filePeek,
                                                                         const std::string& fileName,
                                                                         std::optional<std::unique_ptr<ProgressCallback>> progressCallback) const
    {
        if constexpr (sizeof(void*) != 4u)
        {
            con::error("IW3 Xenon 328 zone loading requires the x86 tools.");
            return nullptr;
        }

        const auto inspectResult = InspectZoneHeader(filePeek);
        if (!inspectResult)
            return nullptr;

        auto zone = std::make_unique<Zone>(fileName, 0, GameId::IW3Xenon328, inspectResult->m_platform);
        auto* zonePtr = zone.get();
        zone->m_language = GameLanguage::LANGUAGE_NONE;

        auto zoneLoader = std::make_unique<ZoneLoader>(std::move(zone));

        SetupBlock(*zoneLoader);

        zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(std::char_traits<char>::length(ZoneConstants::MAGIC_UNSIGNED)));
        AddAuthHeaderSteps(*inspectResult, *zoneLoader, fileName);

        zoneLoader->AddLoadingStep(step::CreateStepAddProcessor(processor::CreateProcessorInflate(ZoneConstants::AUTHED_CHUNK_SIZE)));

        zoneLoader->AddLoadingStep(std::make_unique<StepVerifyVersion>());
        zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(sizeof(uint32_t)));
        zoneLoader->AddLoadingStep(step::CreateStepAllocXBlocks(GameEndianness::BE));

        zoneLoader->AddLoadingStep(step::CreateStepLoadZoneContent(
            [zonePtr](ZoneInputStream& stream)
            {
                return std::make_unique<ContentLoader>(*zonePtr, stream);
            },
            32u,
            ZoneConstants::OFFSET_BLOCK_BIT_COUNT,
            ZoneConstants::INSERT_BLOCK,
            zonePtr->Memory(),
            std::move(progressCallback)));

        return zoneLoader;
    }
} // namespace IW3Xenon328
