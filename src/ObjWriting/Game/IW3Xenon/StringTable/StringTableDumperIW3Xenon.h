#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace string_table
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::StringTable>& asset) override;
    };
} // namespace string_table
