#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"

namespace string_table
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::StringTable>& asset) override;
    };
} // namespace string_table
