#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace xanim
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
