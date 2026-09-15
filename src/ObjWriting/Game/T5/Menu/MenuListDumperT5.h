#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5/T5.h"
#include "Menu/MenuDumpingZoneState.h"

namespace menu
{
    void CreateDumpingStateForMenuListT5(MenuDumpingZoneState* zoneState, const T5::MenuList* menuList);

    class MenuListDumperT5 final : public AbstractAssetDumper<T5::AssetMenuList>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5::AssetMenuList::Type>& asset) override;
    };
} // namespace menu
