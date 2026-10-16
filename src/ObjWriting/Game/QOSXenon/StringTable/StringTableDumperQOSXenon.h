#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"

namespace string_table
{
    class DumperQOSXenon final : public AbstractAssetDumper<QOSXenon::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<QOSXenon::StringTable>& asset) override;
    };
} // namespace string_table
