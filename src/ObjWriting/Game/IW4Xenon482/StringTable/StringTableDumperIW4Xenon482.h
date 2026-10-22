#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"

namespace string_table
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetStringTable::Type>& asset) override;
    };
} // namespace string_table
