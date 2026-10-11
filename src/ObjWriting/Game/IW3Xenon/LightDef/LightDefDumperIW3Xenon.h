#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace light_def
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetLightDef>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetLightDef::Type>& asset) override;
    };
} // namespace light_def
