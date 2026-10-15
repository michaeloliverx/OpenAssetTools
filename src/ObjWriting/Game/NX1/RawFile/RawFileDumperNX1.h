#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace raw_file
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetRawFile::Type>& asset) override;
    };
} // namespace raw_file
