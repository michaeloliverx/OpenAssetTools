#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace techset
{
    class PixelShaderDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetPixelShader>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::MaterialPixelShader>& asset) override;
    };
} // namespace techset
