#include "MenuDumperIW3Xenon.h"

#include "MenuListDumperIW3Xenon.h"
#include "MenuWriterIW3Xenon.h"
#include "ObjWriting.h"

#include <format>
#include <string>

using namespace IW3Xenon;

namespace
{
    std::string GetPathForMenu(menu::MenuDumpingZoneState* zoneState, const XAssetInfo<menuDef_t>& asset)
    {
        const auto menuDumpingState = zoneState->m_menu_dumping_state_map.find(asset.Asset());

        if (menuDumpingState == zoneState->m_menu_dumping_state_map.end())
            return std::format("ui_mp/{}.menu", asset.Asset()->window.name);

        return menuDumpingState->second.m_path;
    }
} // namespace

namespace menu
{
    void MenuDumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMenu::Type>& asset)
    {
        const auto* menu = asset.Asset();
        auto* zoneState = context.GetZoneAssetDumperState<MenuDumpingZoneState>();

        if (!ObjWriting::ShouldHandleAssetType(ASSET_TYPE_MENULIST))
        {
            // Make sure menu paths based on menu lists are created
            const auto menuListAssets = context.m_zone.m_pools.PoolAssets<AssetMenuList>();
            for (auto* menuListAsset : menuListAssets)
                CreateDumpingStateForMenuListIW3Xenon(zoneState, menuListAsset->Asset());
        }

        const auto menuFilePath = GetPathForMenu(zoneState, asset);
        const auto assetFile = context.OpenAssetFile(menuFilePath);

        if (!assetFile)
            return;

        const auto menuWriter = CreateMenuWriterIW3Xenon(*assetFile);

        menuWriter->Start();
        menuWriter->WriteMenu(*menu);
        menuWriter->End();
    }
} // namespace menu
