#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace addon_map_ents
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetAddonMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetAddonMapEnts::Type>& asset) override;
    };
} // namespace addon_map_ents
