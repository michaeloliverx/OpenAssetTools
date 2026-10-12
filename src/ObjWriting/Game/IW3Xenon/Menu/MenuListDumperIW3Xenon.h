#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Menu/MenuDumpingZoneState.h"

namespace menu
{
    void CreateDumpingStateForMenuListIW3Xenon(MenuDumpingZoneState* zoneState, const IW3Xenon::MenuList* menuList);

    class MenuListDumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetMenuList>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetMenuList::Type>& asset) override;
    };
} // namespace menu
