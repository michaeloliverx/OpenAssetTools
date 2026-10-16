#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"

namespace map_ents
{
    class DumperQOSXenon final : public AbstractAssetDumper<QOSXenon::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<QOSXenon::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
