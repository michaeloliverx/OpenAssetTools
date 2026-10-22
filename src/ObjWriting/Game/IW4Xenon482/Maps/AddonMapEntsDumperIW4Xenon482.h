#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace addon_map_ents
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetAddonMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetAddonMapEnts::Type>& asset) override;
    };
} // namespace addon_map_ents
