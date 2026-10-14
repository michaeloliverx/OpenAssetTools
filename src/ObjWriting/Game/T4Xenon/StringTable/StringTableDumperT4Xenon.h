#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace string_table
{
    class DumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::StringTable>& asset) override;
    };
} // namespace string_table
