#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace font
{
    class JsonDumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetFont>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::AssetFont::Type>& asset) override;
    };
} // namespace font
