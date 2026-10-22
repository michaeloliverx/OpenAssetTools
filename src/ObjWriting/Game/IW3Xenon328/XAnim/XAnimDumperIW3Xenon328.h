#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace xanim
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
