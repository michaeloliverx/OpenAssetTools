#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW2Xenon/IW2Xenon.h"

namespace map_ents
{
    class DumperIW2Xenon final : public AbstractAssetDumper<IW2Xenon::AssetClipMapMp>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW2Xenon::AssetClipMapMp::Type>& asset) override;
    };
} // namespace map_ents
