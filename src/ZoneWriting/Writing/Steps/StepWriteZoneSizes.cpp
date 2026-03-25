#include "StepWriteZoneSizes.h"

#include "Utils/Endianness.h"

StepWriteZoneSizes::StepWriteZoneSizes(StepWriteZoneContentToMemory* memory, const GameEndianness endianness)
    : m_memory(memory),
      m_endianness(endianness)
{
}

void StepWriteZoneSizes::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    auto totalSize = static_cast<size_t>(m_memory->GetData()->m_total_size);
    size_t externalSize = 0;

    if (m_endianness == GameEndianness::BE)
    {
        auto totalSizeBE = endianness::ToBigEndian(static_cast<uint32_t>(totalSize));
        auto externalSizeBE = endianness::ToBigEndian(static_cast<uint32_t>(externalSize));
        stream->Write(&totalSizeBE, sizeof(totalSizeBE));
        stream->Write(&externalSizeBE, sizeof(externalSizeBE));
    }
    else
    {
        stream->Write(&totalSize, sizeof(totalSize));
        stream->Write(&externalSize, sizeof(externalSize));
    }
}
