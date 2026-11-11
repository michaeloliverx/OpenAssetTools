#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon302/T4Xenon302.h"

namespace map_d3dbsp
{
    class DumperT4Xenon302 final : public AbstractAssetDumper<T4Xenon302::AssetGfxWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon302::GfxWorld>& asset) override;
    };
} // namespace map_d3dbsp
