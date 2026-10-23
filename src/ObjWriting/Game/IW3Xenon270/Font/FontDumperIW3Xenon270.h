#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace font
{
    class JsonDumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetFont>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetFont::Type>& asset) override;
    };
} // namespace font
