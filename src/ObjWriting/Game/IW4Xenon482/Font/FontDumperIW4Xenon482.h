#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace font
{
    class JsonDumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetFont>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetFont::Type>& asset) override;
    };
} // namespace font
