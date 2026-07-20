#include "MenuListDumperT4.h"

#include "MenuWriterT4.h"

#include <filesystem>

namespace fs = std::filesystem;

using namespace T4;

namespace
{
    std::string PathForMenu(const std::string& menuListParentPath, const menuDef_t* menu)
    {
        auto* menuName = menu->window.name;
        if (!menuName)
            return {};

        if (menuName[0] == ',')
            menuName++;

        return menuListParentPath + menuName + ".menu";
    }

    void DumpMenus(menu::IWriterT4& writer, menu::MenuDumpingZoneState* zoneState, const MenuList* menuList)
    {
        if (!menuList->menus)
            return;

        for (auto menuIndex = 0; menuIndex < menuList->menuCount; menuIndex++)
        {
            const auto* menu = menuList->menus[menuIndex];
            if (!menu)
                continue;

            const auto menuDumpingState = zoneState->m_menu_dumping_state_map.find(menu);
            if (menuDumpingState == zoneState->m_menu_dumping_state_map.end())
                continue;

            if (menuDumpingState->second.m_alias_menu_list == menuList)
                writer.WriteMenu(*menu);
            else
                writer.IncludeMenu(menuDumpingState->second.m_path);
        }
    }
} // namespace

namespace menu
{
    void CreateDumpingStateForMenuListT4(MenuDumpingZoneState* zoneState, const MenuList* menuList)
    {
        if (!menuList || menuList->menuCount <= 0 || !menuList->menus || !menuList->name)
            return;

        const fs::path menuListPath(menuList->name);
        std::string parentPath;
        if (menuListPath.has_parent_path())
            parentPath = menuListPath.parent_path().generic_string() + "/";

        for (auto menuIndex = 0; menuIndex < menuList->menuCount; menuIndex++)
        {
            auto* menu = menuList->menus[menuIndex];
            if (!menu)
                continue;

            const auto menuPath = PathForMenu(parentPath, menu);
            if (menuPath.empty())
                continue;

            const auto isEmbeddedMenu = menuPath == menuList->name;
            const auto existingState = zoneState->m_menu_dumping_state_map.find(menu);
            if (existingState == zoneState->m_menu_dumping_state_map.end())
            {
                zoneState->CreateMenuDumpingState(menu, menuPath, isEmbeddedMenu ? menuList : nullptr);
            }
            else if (isEmbeddedMenu && !existingState->second.m_alias_menu_list)
            {
                existingState->second.m_alias_menu_list = menuList;
                existingState->second.m_path = menuPath;
            }
        }
    }

    void MenuListDumperT4::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMenuList::Type>& asset)
    {
        const auto assetFile = context.OpenAssetFile(asset.m_name);
        if (!assetFile)
            return;

        auto* zoneState = context.GetZoneAssetDumperState<MenuDumpingZoneState>();
        const auto writer = CreateMenuWriterT4(*assetFile);

        writer->Start();
        DumpMenus(*writer, zoneState, asset.Asset());
        writer->End();
    }

    void MenuListDumperT4::Dump(AssetDumpingContext& context)
    {
        auto* zoneState = context.GetZoneAssetDumperState<MenuDumpingZoneState>();
        const auto menuListAssets = context.m_zone.m_pools.PoolAssets<AssetMenuList>();
        for (const auto* asset : menuListAssets)
            CreateDumpingStateForMenuListT4(zoneState, asset->Asset());

        AbstractAssetDumper::Dump(context);
    }
} // namespace menu
