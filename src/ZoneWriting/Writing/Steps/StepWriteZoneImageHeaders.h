#pragma once

#include "Game/IGame.h"
#include "Writing/IWritingStep.h"
#include "Zone/XAssetStreamFile.h"

#include <functional>
#include <memory>
#include <vector>

struct XFileSizePatch
{
    int64_t m_offset = -1;
};

class StepWriteZoneImageHeaders final : public IWritingStep
{
public:
    using ImageStreamFileProvider = std::function<std::vector<XAssetStreamFile>()>;

    StepWriteZoneImageHeaders(GameEndianness endianness, ImageStreamFileProvider provider, std::shared_ptr<XFileSizePatch> sizePatch);
    void PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream) override;

private:
    GameEndianness m_endianness;
    ImageStreamFileProvider m_provider;
    std::shared_ptr<XFileSizePatch> m_size_patch;
};
