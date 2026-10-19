#include "ZoneLoaderFactoryIW2Xenon.h"

#include "ContentLoaderIW2Xenon.h"
#include "Game/GameLanguage.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "Game/IW2Xenon/ZoneConstantsIW2Xenon.h"
#include "Loading/Processor/ProcessorInflate.h"
#include "Loading/Steps/StepAddProcessor.h"
#include "Loading/Steps/StepAllocXBlocks.h"
#include "Loading/Steps/StepLoadZoneContent.h"
#include "Loading/Steps/StepSkipBytes.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

using namespace IW2Xenon;

namespace
{
    constexpr size_t REMAINING_FILE_HEADER_SIZE = ZoneConstants::FILE_HEADER_SIZE - sizeof(ZoneHeaderIW2);

    static_assert(REMAINING_FILE_HEADER_SIZE == sizeof(uint32_t) * 4u);

    void SetupBlock(ZoneLoader& zoneLoader)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        // IW2 retains the temp block until loading finishes.

        zoneLoader.AddXBlock(XBLOCK_DEF(IW2Xenon::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_NORMAL));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW2Xenon::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW2Xenon::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }
} // namespace

std::optional<ZoneLoaderInspectionResult> ZoneLoaderFactory::InspectZoneHeader(ZoneDataPeeking& filePeek) const
{
    const auto& header = filePeek.PeekStruct<ZoneHeaderIW2>();
    if (endianness::FromBigEndian(header.version) == ZoneConstants::ZONE_VERSION)
    {
        return ZoneLoaderInspectionResult{
            .m_game_id = GameId::IW2Xenon,
            .m_endianness = GameEndianness::BE,
            .m_word_size = GameWordSize::ARCH_32,
            .m_platform = GamePlatform::XBOX,
            // There is no way to know whether unsigned zones are official.
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
    {
        con::error("IW2 Xenon zone loading requires the x86 tools.");
        return nullptr;
    }

    const auto inspectResult = InspectZoneHeader(filePeek);
    if (!inspectResult)
        return nullptr;

    auto zone = std::make_unique<Zone>(fileName, 0, GameId::IW2Xenon, inspectResult->m_platform);
    auto* zonePtr = zone.get();
    zone->m_language = GameLanguage::LANGUAGE_NONE;

    auto zoneLoader = std::make_unique<ZoneLoader>(std::move(zone));
    SetupBlock(*zoneLoader);

    // Skip the initial header that we peeked at before
    zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(sizeof(ZoneHeaderIW2)));

    // Uncompressed payload size.
    zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(4));

    zoneLoader->AddLoadingStep(step::CreateStepAllocXBlocks(GameEndianness::BE));
    zoneLoader->AddLoadingStep(step::CreateStepAddProcessor(processor::CreateProcessorInflate(ZoneConstants::STREAM_BUFFER_SIZE)));
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
