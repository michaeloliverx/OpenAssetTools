#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace map_ents
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
