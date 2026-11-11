#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon204/T4Xenon204.h"

namespace map_d3dbsp
{
    class DumperT4Xenon204 final : public AbstractAssetDumper<T4Xenon204::AssetGfxWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon204::GfxWorld>& asset) override;
    };
} // namespace map_d3dbsp
