#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace xmodel
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
