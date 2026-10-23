
#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace xmodel
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
