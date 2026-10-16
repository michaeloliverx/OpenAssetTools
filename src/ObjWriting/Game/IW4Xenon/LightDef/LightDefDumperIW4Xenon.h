#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace light_def
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetLightDef>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetLightDef::Type>& asset) override;
    };
} // namespace light_def
