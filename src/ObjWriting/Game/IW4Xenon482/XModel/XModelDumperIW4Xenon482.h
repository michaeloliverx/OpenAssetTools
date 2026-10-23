#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace xmodel
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
