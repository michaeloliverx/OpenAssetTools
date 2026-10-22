#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace raw_file
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetRawFile>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::RawFile>& asset) override;
    };
} // namespace raw_file
