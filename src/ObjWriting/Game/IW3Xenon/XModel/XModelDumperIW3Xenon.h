// This file was templated.
// See XModelDumper.h.template.
// Do not modify, changes will be lost.

#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"


namespace xmodel
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetXModel>
    {
    public:
        explicit DumperIW3Xenon (const AssetPool<IW3Xenon::AssetXModel::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
