#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"

namespace string_table
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::StringTable>& asset) override;
    };
} // namespace string_table
