#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace techset
{
    class PixelShaderDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetPixelShader>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::MaterialPixelShader>& asset) override;
    };
} // namespace techset
