#include "StepWriteZoneContentToFile.h"

StepWriteZoneContentToFile::StepWriteZoneContentToFile(StepWriteZoneContentToMemory* zoneMemory)
    : m_memory(zoneMemory)
{
}

void StepWriteZoneContentToFile::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    for (const auto& dataBuffer : m_memory->GetData()->m_buffers)
    {
        stream->Write(dataBuffer.m_data.get(), dataBuffer.m_size);
    }

    // Write delay stream data after main content
    for (const auto& dataBuffer : m_memory->GetDelayData()->m_buffers)
    {
        stream->Write(dataBuffer.m_data.get(), dataBuffer.m_size);
    }
}
