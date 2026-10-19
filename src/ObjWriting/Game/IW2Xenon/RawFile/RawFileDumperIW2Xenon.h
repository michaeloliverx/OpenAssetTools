#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW2Xenon/IW2Xenon.h"

namespace raw_file
{
    class DumperIW2Xenon final : public AbstractAssetDumper<IW2Xenon::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW2Xenon::RawFile>& asset) override;
    };
} // namespace raw_file
