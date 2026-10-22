#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace sound_curve
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
