#pragma once
#include "Game/IGame.h"
#include "Writing/IWritingStep.h"

class StepWriteTimestamp final : public IWritingStep
{
public:
    explicit StepWriteTimestamp(GameEndianness endianness = GameEndianness::LE);

    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override;

private:
    GameEndianness m_endianness;
};
