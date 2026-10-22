#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"

namespace string_table
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetStringTable>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::StringTable>& asset) override;
    };
} // namespace string_table
