#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5Xenon/T5Xenon.h"

namespace raw_file
{
    class DumperT5Xenon final : public AbstractAssetDumper<T5Xenon::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5Xenon::AssetRawFile::Type>& asset) override;
    };
} // namespace raw_file
