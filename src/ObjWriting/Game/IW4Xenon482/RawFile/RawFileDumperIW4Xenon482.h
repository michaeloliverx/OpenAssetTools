#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace raw_file
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetRawFile::Type>& asset) override;
    };
} // namespace raw_file
