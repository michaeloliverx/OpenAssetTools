#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace menu
{
    class MenuDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetMenu>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetMenu::Type>& asset) override;
    };
} // namespace menu
