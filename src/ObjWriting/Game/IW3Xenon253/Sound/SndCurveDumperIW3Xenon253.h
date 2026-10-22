#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace sound_curve
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
