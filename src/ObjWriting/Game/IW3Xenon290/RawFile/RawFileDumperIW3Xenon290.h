#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace raw_file
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::RawFile>& asset) override;
    };
} // namespace raw_file
