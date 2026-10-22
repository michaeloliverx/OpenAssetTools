#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace map_ents
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
