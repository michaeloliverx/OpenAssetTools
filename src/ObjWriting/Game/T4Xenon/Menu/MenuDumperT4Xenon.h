#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace menu
{
    class MenuDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetMenu>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetMenu::Type>& asset) override;
    };
} // namespace menu
