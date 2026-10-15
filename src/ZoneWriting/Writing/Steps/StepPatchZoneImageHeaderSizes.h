#pragma once

#include "Game/IGame.h"
#include "Writing/IWritingStep.h"
#include "Writing/Steps/StepWriteZoneImageHeaders.h"

#include <memory>

class StepPatchZoneImageHeaderSizes final : public IWritingStep
{
public:
    StepPatchZoneImageHeaderSizes(std::shared_ptr<XFileSizePatch> patch, GameEndianness endianness);

    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override;

private:
    std::shared_ptr<XFileSizePatch> m_patch;
    GameEndianness m_endianness;
};
