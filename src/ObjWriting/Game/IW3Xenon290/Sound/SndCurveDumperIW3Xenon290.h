#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace sound_curve
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
