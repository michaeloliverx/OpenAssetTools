#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace sound_curve
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
