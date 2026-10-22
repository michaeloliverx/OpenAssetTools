#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace xanim
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
