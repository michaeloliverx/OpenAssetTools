#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace sound_curve
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetSndVfCurve>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetSndVfCurve::Type>& asset) override;
    };
} // namespace sound_curve
