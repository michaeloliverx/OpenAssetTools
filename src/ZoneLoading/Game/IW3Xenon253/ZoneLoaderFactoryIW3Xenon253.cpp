#include "ZoneLoaderFactoryIW3Xenon253.h"

#include "ContentLoaderIW3Xenon253.h"
#include "Game/GameLanguage.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Game/IW3Xenon253/ZoneConstantsIW3Xenon253.h"
#include "Loading/Processor/ProcessorInflate.h"
#include "Loading/Steps/StepAddProcessor.h"
#include "Loading/Steps/StepAllocXBlocks.h"
#include "Loading/Steps/StepLoadZoneContent.h"
#include "Loading/Steps/StepSkipBytes.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

using namespace IW3Xenon253;

namespace
{
    constexpr size_t REMAINING_FILE_HEADER_SIZE = ZoneConstants::FILE_HEADER_SIZE - sizeof(ZoneHeader253);

    static_assert(REMAINING_FILE_HEADER_SIZE == sizeof(uint32_t) * 6u);

    void SetupBlock(ZoneLoader& zoneLoader)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        zoneLoader.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }
} // namespace

std::optional<ZoneLoaderInspectionResult> ZoneLoaderFactory::InspectZoneHeader(ZoneDataPeeking& filePeek) const
{
    const auto& header = filePeek.PeekStruct<ZoneHeader253>();
    if (endianness::FromBigEndian(header.version) == ZoneConstants::ZONE_VERSION)
    {
        return ZoneLoaderInspectionResult{
            .m_game_id = GameId::IW3Xenon253,
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
        con::error("IW3 Xenon 253 zone loading requires the x86 tools.");
        return nullptr;
    }

    const auto inspectResult = InspectZoneHeader(filePeek);
    if (!inspectResult)
        return nullptr;

    auto zone = std::make_unique<Zone>(fileName, 0, GameId::IW3Xenon253, inspectResult->m_platform);
    auto* zonePtr = zone.get();
    zone->m_language = GameLanguage::LANGUAGE_NONE;

    auto zoneLoader = std::make_unique<ZoneLoader>(std::move(zone));
    SetupBlock(*zoneLoader);

    zoneLoader->AddLoadingStep(step::CreateStepSkipBytes(sizeof(ZoneHeader253)));

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
