#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3/IW3.h"

namespace material
{
    class RawDumperIW3 final : public AbstractAssetDumper<IW3::AssetMaterial>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        bool ShouldDump(const XAssetInfo<IW3::AssetMaterial::Type>& asset) override;
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3::AssetMaterial::Type>& asset) override;
    };
} // namespace material
