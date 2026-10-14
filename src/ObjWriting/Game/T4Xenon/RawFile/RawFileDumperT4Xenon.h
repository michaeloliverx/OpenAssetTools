#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace raw_file
{
    class DumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::RawFile>& asset) override;
    };
} // namespace raw_file
