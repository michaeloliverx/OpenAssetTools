#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace map_ents
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
