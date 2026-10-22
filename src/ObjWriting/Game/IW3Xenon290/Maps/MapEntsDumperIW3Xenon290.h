#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace map_ents
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
