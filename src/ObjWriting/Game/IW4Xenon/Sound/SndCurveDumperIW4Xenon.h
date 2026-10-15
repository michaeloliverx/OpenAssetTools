#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace sound_curve
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
