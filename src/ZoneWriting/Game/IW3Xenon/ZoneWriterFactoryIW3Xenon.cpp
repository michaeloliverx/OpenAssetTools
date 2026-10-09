#include "ZoneWriterFactoryIW3Xenon.h"

#include "ContentWriterIW3Xenon.h"
#include "Game/IW3Xenon/ZoneConstantsIW3Xenon.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"
#include "Writing/Processor/OutputProcessorDeflate.h"
#include "Writing/Steps/StepAddOutputProcessor.h"
#include "Writing/Steps/StepWriteXBlockSizes.h"
#include "Writing/Steps/StepWriteZoneContentToFile.h"
#include "Writing/Steps/StepWriteZoneContentToMemory.h"
#include "Writing/Steps/StepWriteZoneHeader.h"
#include "Writing/Steps/StepWriteZoneSizes.h"

#include <cstring>

using namespace IW3Xenon;

namespace
{
    void SetupBlocks(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_LARGE_RUNTIME, XBlockType::BLOCK_TYPE_DELAY));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_PHYSICAL_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    ZoneHeader CreateHeader()
    {
        ZoneHeader header{};
        header.m_version = endianness::ToBigEndian(ZoneConstants::ZONE_VERSION);
        std::memcpy(header.m_magic, ZoneConstants::MAGIC_UNSIGNED, sizeof(header.m_magic));
        return header;
    }
} // namespace

std::unique_ptr<ZoneWriter> ZoneWriterFactory::CreateWriter(const Zone& zone) const
{
    if constexpr (sizeof(void*) != 4u)
    {
        con::error("IW3 Xenon zone writing requires the x86 tools.");
        return nullptr;
    }

    auto writer = std::make_unique<ZoneWriter>();
    SetupBlocks(*writer);

    auto contentInMemory = std::make_unique<StepWriteZoneContentToMemory>(
        std::make_unique<ContentWriter>(zone), zone, 32u, ZoneConstants::OFFSET_BLOCK_BIT_COUNT, ZoneConstants::INSERT_BLOCK, GameEndianness::BE);
    auto* contentInMemoryPtr = contentInMemory.get();
    writer->AddWritingStep(std::move(contentInMemory));
    writer->AddWritingStep(std::make_unique<StepWriteZoneHeader>(CreateHeader()));
    writer->AddWritingStep(std::make_unique<StepAddOutputProcessor>(std::make_unique<OutputProcessorDeflate>()));
    writer->AddWritingStep(std::make_unique<StepWriteZoneSizes>(contentInMemoryPtr, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepWriteXBlockSizes>(zone, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepWriteZoneContentToFile>(contentInMemoryPtr));

    return writer;
}
