#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace sound_curve
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
