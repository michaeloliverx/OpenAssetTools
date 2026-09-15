#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5/T5.h"

namespace menu
{
    class MenuDumperT5 final : public AbstractAssetDumper<T5::AssetMenu>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5::AssetMenu::Type>& asset) override;
    };
} // namespace menu
