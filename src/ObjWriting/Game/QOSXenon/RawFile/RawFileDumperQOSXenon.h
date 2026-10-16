#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"

namespace raw_file
{
    class DumperQOSXenon final : public AbstractAssetDumper<QOSXenon::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<QOSXenon::RawFile>& asset) override;
    };
} // namespace raw_file
