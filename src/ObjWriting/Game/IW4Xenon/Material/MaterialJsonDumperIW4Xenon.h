#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace material
{
    class JsonDumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetMaterial>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetMaterial::Type>& asset) override;
    };
} // namespace material
