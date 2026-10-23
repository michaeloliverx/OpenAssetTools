
#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace xmodel
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
