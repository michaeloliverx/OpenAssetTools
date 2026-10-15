#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace map_ents
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
