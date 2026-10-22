#include "ZoneWriterFactoryIW3Xenon253.h"

#include "ContentWriterIW3Xenon253.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Game/IW3Xenon253/ZoneConstantsIW3Xenon253.h"
#include "Utils/ClassUtils.h"
#include "Utils/Endianness.h"
#include "Writing/IWritingStep.h"
#include "Writing/Processor/OutputProcessorDeflate.h"
#include "Writing/Steps/StepAddOutputProcessor.h"
#include "Writing/Steps/StepWriteXBlockSizes.h"
#include "Writing/Steps/StepWriteZoneContentToFile.h"
#include "Writing/Steps/StepWriteZoneContentToMemory.h"

#include <cstdint>

using namespace IW3Xenon253;

namespace
{
    void SetupBlocks(ZoneWriter& writer)
    {
#define XBLOCK_DEF(name, type) std::make_unique<XBlock>(STR(name), name, type)

        writer.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_TEMP, XBlockType::BLOCK_TYPE_TEMP));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_RUNTIME, XBlockType::BLOCK_TYPE_RUNTIME));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_VIRTUAL, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_LARGE, XBlockType::BLOCK_TYPE_NORMAL));
        writer.AddXBlock(XBLOCK_DEF(IW3Xenon253::XFILE_BLOCK_PHYSICAL, XBlockType::BLOCK_TYPE_NORMAL));

#undef XBLOCK_DEF
    }

    class StepWriteHeader final : public IWritingStep
    {
    public:
        explicit StepWriteHeader(StepWriteZoneContentToMemory* content)
            : m_content(content)
        {
        }

        void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override
        {
            const ZoneHeader253 header{
                .version = endianness::ToBigEndian(ZoneConstants::ZONE_VERSION),
            };

            stream->Write(&header, sizeof(header));
            const auto size = endianness::ToBigEndian(static_cast<uint32_t>(m_content->GetData()->m_total_size));
            stream->Write(&size, sizeof(size));
        }

    private:
        StepWriteZoneContentToMemory* m_content;
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

    writer->AddWritingStep(std::make_unique<StepWriteHeader>(contentInMemoryPtr));

    writer->AddWritingStep(std::make_unique<StepWriteXBlockSizes>(zone, GameEndianness::BE));
    writer->AddWritingStep(std::make_unique<StepAddOutputProcessor>(std::make_unique<OutputProcessorDeflate>(ZoneConstants::STREAM_BUFFER_SIZE)));
    writer->AddWritingStep(std::make_unique<StepWriteZoneContentToFile>(contentInMemoryPtr));

    return writer;
}
