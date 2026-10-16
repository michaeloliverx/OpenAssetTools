#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5Xenon/T5Xenon.h"

namespace map_ents
{
    class DumperT5Xenon final : public AbstractAssetDumper<T5Xenon::AssetMapEnts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5Xenon::AssetMapEnts::Type>& asset) override;
    };
} // namespace map_ents
