#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"

namespace sound_curve
{
    class DumperQOSXenon final : public AbstractAssetDumper<QOSXenon::AssetSoundCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<QOSXenon::AssetSoundCurve::Type>& asset) override;
    };
} // namespace sound_curve
