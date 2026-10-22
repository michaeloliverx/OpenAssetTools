#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace map_ents
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
