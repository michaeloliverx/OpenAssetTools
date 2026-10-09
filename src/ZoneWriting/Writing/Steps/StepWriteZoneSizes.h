#pragma once
#include "Game/IGame.h"
#include "StepWriteZoneContentToMemory.h"
#include "Writing/IWritingStep.h"

class StepWriteZoneSizes final : public IWritingStep
{
    StepWriteZoneContentToMemory* m_memory;
    GameEndianness m_endianness;

public:
    explicit StepWriteZoneSizes(StepWriteZoneContentToMemory* memory, GameEndianness endianness = GameEndianness::LE);

    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override;
};
