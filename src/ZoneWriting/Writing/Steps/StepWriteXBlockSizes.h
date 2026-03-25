#pragma once
#include "Game/IGame.h"
#include "Writing/IWritingStep.h"
#include "Zone/Zone.h"

class StepWriteXBlockSizes final : public IWritingStep
{
public:
    explicit StepWriteXBlockSizes(const Zone& zone, GameEndianness endianness = GameEndianness::LE);

    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override;

private:
    const Zone& m_zone;
    GameEndianness m_endianness;
};
