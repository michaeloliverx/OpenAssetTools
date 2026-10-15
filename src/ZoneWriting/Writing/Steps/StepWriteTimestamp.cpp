#include "StepWriteTimestamp.h"

#include "Utils/Endianness.h"

#include <cstdint>
#include <ctime>

StepWriteTimestamp::StepWriteTimestamp(const GameEndianness endianness)
    : m_endianness(endianness)
{
}

void StepWriteTimestamp::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    // time_t is not guaranteed to be 64bit so cast is necessary
    auto time = static_cast<uint64_t>(std::time(nullptr));
    if (m_endianness == GameEndianness::BE)
        time = endianness::ToBigEndian(time);

    stream->Write(&time, sizeof(time));
}
