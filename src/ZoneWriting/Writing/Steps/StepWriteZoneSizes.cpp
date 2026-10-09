#include "StepWriteZoneSizes.h"

#include "Utils/Endianness.h"

#include <cstdint>

StepWriteZoneSizes::StepWriteZoneSizes(StepWriteZoneContentToMemory* memory, const GameEndianness endianness)
    : m_memory(memory),
      m_endianness(endianness)
{
}

void StepWriteZoneSizes::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    auto totalSize = static_cast<uint32_t>(m_memory->GetData()->m_total_size);
    uint32_t externalSize = 0;

    if (m_endianness == GameEndianness::BE)
    {
        totalSize = endianness::ToBigEndian(totalSize);
        externalSize = endianness::ToBigEndian(externalSize);
    }

    stream->Write(&totalSize, sizeof(totalSize));
    stream->Write(&externalSize, sizeof(externalSize));
}
