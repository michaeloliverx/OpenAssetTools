#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace addon_map_ents
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetAddonMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetAddonMapEnts::Type>& asset) override;
    };
} // namespace addon_map_ents
