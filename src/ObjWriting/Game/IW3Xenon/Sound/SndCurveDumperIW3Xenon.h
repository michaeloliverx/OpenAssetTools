#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace sound_curve
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
