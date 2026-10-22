#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace raw_file
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::RawFile>& asset) override;
    };
} // namespace raw_file
