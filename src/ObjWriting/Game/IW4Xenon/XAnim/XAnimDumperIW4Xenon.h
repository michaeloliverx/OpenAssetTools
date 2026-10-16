#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace xanim
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
