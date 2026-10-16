#include "ZoneWriterFactoryQOSXenon.h"

#include "ContentWriterQOSXenon.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "Game/QOSXenon/ZoneConstantsQOSXenon.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Writing/IWritingStep.h"
#include "Writing/Processor/OutputProcessorDeflate.h"
#include "Writing/Steps/StepAddOutputProcessor.h"
#include "Writing/Steps/StepWriteXBlockSizes.h"
#include "Writing/Steps/StepWriteZero.h"
#include "Writing/Steps/StepWriteZoneContentToFile.h"
#include "Writing/Steps/StepWriteZoneContentToMemory.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

using namespace QOSXenon;

namespace
{
    void SetupBlocks(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(QOSXenon::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(QOSXenon::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(QOSXenon::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(QOSXenon::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(QOSXenon::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    class StepWriteQosHeader final : public IWritingStep
    {
    public:
        void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override
        {
            const ZoneHeaderQos header{
                .version = endianness::ToBigEndian(ZoneConstants::ZONE_VERSION),
            };

            stream->Write(&header, sizeof(header));
        }
    };
} // namespace

std::unique_ptr<ZoneWriter> ZoneWriterFactory::CreateWriter(const Zone& zone) const
{
    auto writer = std::make_unique<ZoneWriter>();

    SetupBlocks(*writer);

    auto contentInMemory = std::make_unique<StepWriteZoneContentToMemory>(
        std::make_unique<ContentWriter>(zone), zone, 32u, ZoneConstants::OFFSET_BLOCK_BIT_COUNT, ZoneConstants::INSERT_BLOCK, GameEndianness::BE);
    auto* contentInMemoryPtr = contentInMemory.get();
    writer->AddWritingStep(std::move(contentInMemory));

    writer->AddWritingStep(std::make_unique<StepWriteQosHeader>());

    // External data size.
    writer->AddWritingStep(std::make_unique<StepWriteZero>(4));

    writer->AddWritingStep(std::make_unique<StepWriteXBlockSizes>(zone, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepAddOutputProcessor>(std::make_unique<OutputProcessorDeflate>(ZoneConstants::AUTHED_CHUNK_SIZE)));
    writer->AddWritingStep(std::make_unique<StepWriteZoneContentToFile>(contentInMemoryPtr));

    return writer;
}
