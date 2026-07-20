#include "MenuDumperT4.h"

#include "MenuListDumperT4.h"
#include "MenuWriterT4.h"
#include "ObjWriting.h"

using namespace T4;

namespace
{
    std::string GetPathForMenu(menu::MenuDumpingZoneState* zoneState, const XAssetInfo<menuDef_t>& asset)
    {
        const auto menuDumpingState = zoneState->m_menu_dumping_state_map.find(asset.Asset());

        if (menuDumpingState != zoneState->m_menu_dumping_state_map.end())
            return menuDumpingState->second.m_path;

        const auto* menuName = asset.Asset()->window.name;
        const std::string fallbackName = menuName ? menuName : asset.m_name;
        return "ui_mp/" + fallbackName + ".menu";
    }
} // namespace

namespace menu
{
    void MenuDumperT4::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMenu::Type>& asset)
    {
        auto* zoneState = context.GetZoneAssetDumperState<MenuDumpingZoneState>();

        if (!ObjWriting::ShouldHandleAssetType(ASSET_TYPE_MENULIST))
        {
            const auto menuListAssets = context.m_zone.m_pools.PoolAssets<AssetMenuList>();
            for (const auto* menuListAsset : menuListAssets)
                CreateDumpingStateForMenuListT4(zoneState, menuListAsset->Asset());
        }

        const auto assetFile = context.OpenAssetFile(GetPathForMenu(zoneState, asset));
        if (!assetFile)
            return;

        const auto menuWriter = CreateMenuWriterT4(*assetFile);
        menuWriter->Start();
        menuWriter->WriteMenu(*asset.Asset());
        menuWriter->End();
    }
} // namespace menu
