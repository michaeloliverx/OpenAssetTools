#include "ZoneWriterFactoryIW3.h"

#include "ContentWriterIW3.h"
#include "Game/IW3/GameIW3.h"
#include "Game/IW3/IW3.h"
#include "Game/IW3/ZoneConstantsIW3.h"
#include "Game/IW3Xenon/ContentWriterIW3Xenon.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Utils/Endianness.h"
#include "Writing/Processor/OutputProcessorDeflate.h"
#include "Writing/Steps/StepAddOutputProcessor.h"
#include "Writing/Steps/StepWriteXBlockSizes.h"
#include "Writing/Steps/StepWriteZoneContentToFile.h"
#include "Writing/Steps/StepWriteZoneContentToMemory.h"
#include "Writing/Steps/StepWriteZoneHeader.h"
#include "Writing/Steps/StepWriteZoneSizes.h"

#include <cstring>

using namespace IW3;

namespace
{
    void SetupBlocksPC(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_LARGE_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_PHYSICAL_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_VERTEX, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3::XFILE_BLOCK_INDEX, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    void SetupBlocksXenon(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_LARGE_RUNTIME, XBlockType::BLOCK_TYPE_DELAY));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_PHYSICAL_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    ZoneHeader CreateHeaderPC()
    {
        ZoneHeader header{};
        header.m_version = ZoneConstants::ZONE_VERSION_PC;
        memcpy(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, sizeof(ZoneHeader::m_magic));

        return header;
    }

    ZoneHeader CreateHeaderXenon()
    {
        ZoneHeader header{};
        header.m_version = endianness::ToBigEndian(ZoneConstants::ZONE_VERSION_XENON);
        memcpy(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, sizeof(ZoneHeader::m_magic));

        return header;
    }
} // namespace

std::unique_ptr<ZoneWriter> ZoneWriterFactory::CreateWriter(const Zone& zone) const
{
    auto writer = std::make_unique<ZoneWriter>();

    const auto isXenon = zone.m_platform == GamePlatform::XBOX;

    if (isXenon)
        SetupBlocksXenon(*writer);
    else
        SetupBlocksPC(*writer);

    const auto offsetBlockBitCount = isXenon ? ZoneConstants::OFFSET_BLOCK_BIT_COUNT_XENON : ZoneConstants::OFFSET_BLOCK_BIT_COUNT;
    const auto insertBlock = isXenon ? ZoneConstants::INSERT_BLOCK_XENON : ZoneConstants::INSERT_BLOCK;

    std::unique_ptr<StepWriteZoneContentToMemory> contentInMemory;
    if (isXenon)
    {
        contentInMemory =
            std::make_unique<StepWriteZoneContentToMemory>(std::make_unique<IW3Xenon::ContentWriter>(zone), zone, offsetBlockBitCount, insertBlock);
    }
    else
    {
        contentInMemory = std::make_unique<StepWriteZoneContentToMemory>(std::make_unique<ContentWriter>(zone), zone, offsetBlockBitCount, insertBlock);
    }
    auto* contentInMemoryPtr = contentInMemory.get();
    writer->AddWritingStep(std::move(contentInMemory));

    // Write zone header
    writer->AddWritingStep(std::make_unique<StepWriteZoneHeader>(isXenon ? CreateHeaderXenon() : CreateHeaderPC()));

    writer->AddWritingStep(std::make_unique<StepAddOutputProcessor>(std::make_unique<OutputProcessorDeflate>()));

    const auto endianness = isXenon ? GameEndianness::BE : GameEndianness::LE;

    // Start of the XFile struct
    writer->AddWritingStep(std::make_unique<StepWriteZoneSizes>(contentInMemoryPtr, endianness));
    writer->AddWritingStep(std::make_unique<StepWriteXBlockSizes>(zone, endianness));

    // Start of the zone content
    writer->AddWritingStep(std::make_unique<StepWriteZoneContentToFile>(contentInMemoryPtr));

    return std::move(writer);
}
