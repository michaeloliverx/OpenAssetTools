#include "ZoneLoaderFactoryT5Xenon.h"

#include "ContentLoaderT5Xenon.h"
#include "Game/GameLanguage.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "Game/T5Xenon/ZoneConstantsT5Xenon.h"
#include "Loading/Processor/ProcessorXChunks.h"
#include "Loading/Steps/StepAddProcessor.h"
#include "Loading/Steps/StepAllocXBlocks.h"
#include "Loading/Steps/StepLoadZoneContent.h"
#include "Loading/Steps/StepLoadZoneSizes.h"
#include "Loading/Steps/StepSkipBytes.h"
#include "Loading/Steps/StepVerifyFileName.h"
#include "Loading/Steps/StepVerifyMagic.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Zone/XChunk/XChunkProcessorInflate.h"
#include "Zone/XChunk/XChunkProcessorSalsa20Decryption.h"

#include <cstring>
#include <memory>

namespace T5Xenon
{
    namespace
    {
        void SetupBlocks(ZoneLoader& zoneLoader)
        {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_LARGE_RUNTIME, XBlockType::BLOCK_TYPE_DELAY));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_PHYSICAL_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
            zoneLoader.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
        }

        void AddAuthHeaderSteps(const ZoneLoaderInspectionResult& inspectResult, ZoneLoader& zoneLoader, const std::string& fileName)
        {
            if (!inspectResult.m_is_signed)
                return;

            zoneLoader.AddLoadingStep(step::CreateStepVerifyMagic(ZoneConstants::MAGIC_AUTH_HEADER));
            zoneLoader.AddLoadingStep(step::CreateStepSkipBytes(4));
            zoneLoader.AddLoadingStep(step::CreateStepVerifyFileName(fileName, 32));
            zoneLoader.AddLoadingStep(step::CreateStepSkipBytes(256));
        }

        void AddXChunkProcessor(const ZoneLoaderInspectionResult& inspectResult, ZoneLoader& zoneLoader, const std::string& fileName)
        {
            auto xChunkProcessor = processor::CreateProcessorXChunks(ZoneConstants::STREAM_COUNT, ZoneConstants::XCHUNK_SIZE, inspectResult.m_endianness);

            if (inspectResult.m_is_encrypted)
            {
                xChunkProcessor->AddChunkProcessor(std::make_unique<XChunkProcessorSalsa20Decryption>(ZoneConstants::STREAM_COUNT,
                                                                                                      fileName,
                                                                                                      ZoneConstants::SALSA20_KEY_TREYARCH_XENON,
                                                                                                      sizeof(ZoneConstants::SALSA20_KEY_TREYARCH_XENON),
                                                                                                      ZoneConstants::SALSA20_HASHES_PER_STREAM));
            }

            xChunkProcessor->AddChunkProcessor(std::make_unique<XChunkProcessorInflate>());
            zoneLoader.AddLoadingStep(step::CreateStepAddProcessor(std::move(xChunkProcessor)));
        }
    } // namespace

    std::optional<ZoneLoaderInspectionResult> ZoneLoaderFactory::InspectZoneHeader(ZoneDataPeeking& filePeek) const
    {
        const auto& header = filePeek.PeekStruct<ZoneHeader>();
        if (endianness::FromBigEndian(header.m_version) != ZoneConstants::ZONE_VERSION)
            return std::nullopt;

        if (!std::memcmp(header.m_magic, ZoneConstants::MAGIC_SIGNED, sizeof(header.m_magic)))
        {
            return ZoneLoaderInspectionResult{
                .m_game_id = GameId::T5Xenon,
                .m_endianness = GameEndianness::BE,
                .m_word_size = GameWordSize::ARCH_32,
                .m_platform = GamePlatform::XBOX,
                .m_is_official = true,
                .m_is_signed = true,
                .m_is_encrypted = true,

            };
        }

        if (!std::memcmp(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, sizeof(header.m_magic)))
        {
            return ZoneLoaderInspectionResult{
                .m_game_id = GameId::T5Xenon,
                .m_endianness = GameEndianness::BE,
                .m_word_size = GameWordSize::ARCH_32,
                .m_platform = GamePlatform::XBOX,
                .m_is_official = false,
                .m_is_signed = false,
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
            return nullptr;
        const auto inspectResult = InspectZoneHeader(filePeek);
        if (!inspectResult)
            return nullptr;

        auto zone = std::make_unique<Zone>(fileName, 0, GameId::T5Xenon, inspectResult->m_platform);
        auto* zonePtr = zone.get();
        zone->m_language = GameLanguage::LANGUAGE_NONE;

        auto zoneLoader = std::make_unique<ZoneLoader>(std::move(zone));

        SetupBlocks(*zoneLoader);
        zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(sizeof(ZoneHeader) + sizeof(uint32_t)));
        AddAuthHeaderSteps(*inspectResult, *zoneLoader, fileName);
        AddXChunkProcessor(*inspectResult, *zoneLoader, fileName);

        zoneLoader->AddLoadingStep(step::CreateStepLoadZoneSizes(GameEndianness::BE));
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
} // namespace T5Xenon
