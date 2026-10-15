#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace raw_file
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetRawFile::Type>& asset) override;
    };
} // namespace raw_file
