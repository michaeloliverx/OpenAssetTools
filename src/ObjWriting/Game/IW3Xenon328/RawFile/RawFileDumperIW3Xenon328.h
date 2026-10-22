#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace raw_file
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::RawFile>& asset) override;
    };
} // namespace raw_file
