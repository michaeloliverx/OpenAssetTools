#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace map_ents
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetMapEnts>
    {
    public:
        explicit DumperIW3Xenon(const AssetPool<IW3Xenon::AssetMapEnts::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::MapEnts>& asset) override;
    };
} // namespace map_ents
