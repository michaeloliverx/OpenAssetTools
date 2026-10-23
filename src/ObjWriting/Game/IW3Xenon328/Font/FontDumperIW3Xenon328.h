#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace font
{
    class JsonDumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetFont>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetFont::Type>& asset) override;
    };
} // namespace font
