#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon269/IW3Xenon269.h"

namespace map_d3dbsp
{
    class DumperIW3Xenon269 final : public AbstractAssetDumper<IW3Xenon269::AssetGfxWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon269::GfxWorld>& asset) override;
    };
} // namespace map_d3dbsp
