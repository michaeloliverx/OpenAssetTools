#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW2Xenon/IW2Xenon.h"

namespace sound_curve
{
    class DumperIW2Xenon final : public AbstractAssetDumper<IW2Xenon::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW2Xenon::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
