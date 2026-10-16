#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5Xenon/T5Xenon.h"

namespace string_table
{
    class DumperT5Xenon final : public AbstractAssetDumper<T5Xenon::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5Xenon::AssetStringTable::Type>& asset) override;
    };
} // namespace string_table
