#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace font
{
    class JsonDumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetFont>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetFont::Type>& asset) override;
    };
} // namespace font
