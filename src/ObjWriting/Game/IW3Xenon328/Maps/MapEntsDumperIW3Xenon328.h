#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace map_ents
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
