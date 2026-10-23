
#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace xmodel
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
