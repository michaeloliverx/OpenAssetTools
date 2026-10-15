#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "Menu/MenuDumpingZoneState.h"

namespace menu
{
    void CreateDumpingStateForMenuListT4Xenon(MenuDumpingZoneState* zoneState, const T4Xenon::MenuList* menuList);

    class MenuListDumperT4Xenon final : public AbstractAssetDumper<T4Xenon::AssetMenuList>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T4Xenon::AssetMenuList::Type>& asset) override;
    };
} // namespace menu
