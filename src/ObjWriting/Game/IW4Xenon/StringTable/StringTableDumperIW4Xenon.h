#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace string_table
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetStringTable::Type>& asset) override;
    };
} // namespace string_table
