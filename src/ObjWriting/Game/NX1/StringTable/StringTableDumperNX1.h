#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"

namespace string_table
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetStringTable::Type>& asset) override;
    };
} // namespace string_table
