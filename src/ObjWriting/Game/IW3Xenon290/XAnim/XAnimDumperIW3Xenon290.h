#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace xanim
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
