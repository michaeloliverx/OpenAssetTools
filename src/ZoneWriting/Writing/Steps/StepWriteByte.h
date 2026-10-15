#pragma once
#include "Writing/IWritingStep.h"

#include <cstdint>

class StepWriteByte final : public IWritingStep
{
public:
    explicit StepWriteByte(const std::uint8_t value)
        : m_value(value)
    {
    }

    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override
    {
        stream->Write(&m_value, sizeof(m_value));
    }

private:
    std::uint8_t m_value;
};
