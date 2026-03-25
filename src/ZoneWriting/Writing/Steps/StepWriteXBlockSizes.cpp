#include "StepWriteXBlockSizes.h"

#include "Utils/Endianness.h"

StepWriteXBlockSizes::StepWriteXBlockSizes(const Zone& zone, const GameEndianness endianness)
    : m_zone(zone),
      m_endianness(endianness)
{
}

void StepWriteXBlockSizes::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    for (const auto& block : zoneWriter->m_blocks)
    {
        auto blockSize = static_cast<xblock_size_t>(block->m_buffer_size);

        if (m_endianness == GameEndianness::BE)
            blockSize = endianness::ToBigEndian(blockSize);

        stream->Write(&blockSize, sizeof(blockSize));
    }
}
