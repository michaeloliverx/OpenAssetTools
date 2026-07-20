#include "Game/T4/Menu/MenuDumperT4.h"

#include "Game/T4/Menu/MenuListDumperT4.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <iterator>
#include <memory>

using namespace T4;

namespace
{
    expressionEntry Operator(const expOperationEnum operation)
    {
        expressionEntry entry{};
        entry.type = EET_OPERATOR;
        entry.data.op = operation;
        return entry;
    }

    template<std::size_t EntryCount> std::array<expressionEntry*, EntryCount> EntryPointers(std::array<expressionEntry, EntryCount>& entries)
    {
        std::array<expressionEntry*, EntryCount> result{};
        for (auto entryIndex = 0u; entryIndex < EntryCount; entryIndex++)
            result[entryIndex] = &entries[entryIndex];
        return result;
    }

    TEST_CASE("MenuDumperT4: Can dump menu with T4 expressions and listbox properties", "[t4][menu][assetdumper]")
    {
        auto visibleValues = std::array{Operator(OP_PARTYISMISSINGMAPPACK), Operator(OP_RIGHTPAREN)};
        auto visibleEntries = EntryPointers(visibleValues);

        Material itemBackground{};
        itemBackground.info.name = "item_background";

        Material selectIcon{};
        selectIcon.info.name = "listbox_select_icon";

        Material listboxBackground{};
        listboxBackground.info.name = "listbox_background";

        Material highlightTexture{};
        highlightTexture.info.name = "listbox_highlight";

        listBoxDef_s listBox{};
        listBox.elementWidth = 200.0f;
        listBox.elementHeight = 18.0f;
        listBox.elementStyle = 1;
        listBox.numColumns = 2;
        listBox.columnInfo[0] = {4, 120, 32, 0};
        listBox.columnInfo[1] = {128, 64, 8, 2};
        listBox.doubleClick = "play mouse_click; open server_info;";
        listBox.selectBorder[0] = 0.8f;
        listBox.selectBorder[1] = 0.95f;
        listBox.selectBorder[2] = 1.0f;
        listBox.disableColor[0] = 0.3f;
        listBox.disableColor[1] = 0.3f;
        listBox.disableColor[2] = 0.3f;
        listBox.disableColor[3] = 1.0f;
        listBox.focusColor[0] = 1.0f;
        listBox.focusColor[1] = 0.8f;
        listBox.focusColor[2] = 0.2f;
        listBox.focusColor[3] = 1.0f;
        listBox.selectIcon = &selectIcon;
        listBox.backgroundItemListbox = &listboxBackground;
        listBox.highlightTexture = &highlightTexture;

        itemDef_s serverList{};
        serverList.window.name = "server_list";
        serverList.window.rectClient = {20.0f, 80.0f, 400.0f, 180.0f, 1, 2};
        serverList.window.style = 1;
        serverList.window.dynamicFlags[0] = WINDOW_FLAG_VISIBLE;
        serverList.window.foreColor[0] = 1.0f;
        serverList.window.foreColor[1] = 1.0f;
        serverList.window.foreColor[2] = 1.0f;
        serverList.window.foreColor[3] = 1.0f;
        serverList.window.background = &itemBackground;
        serverList.type = ITEM_TYPE_LISTBOX;
        serverList.text = "";
        serverList.special = 2.0f;
        serverList.onFocus = "play mouse_over;";
        serverList.onListboxSelectionChange = "play mouse_over; setdvar ui_server_selected 1;";
        serverList.typeData.listBox = &listBox;

        itemDef_s* items[]{&serverList};

        menuDef_t menu{};
        menu.window.name = "server_browser";
        menu.window.rect = {0.0f, 0.0f, 640.0f, 480.0f, 0, 0};
        menu.window.style = 1;
        menu.window.foreColor[0] = 1.0f;
        menu.window.foreColor[1] = 1.0f;
        menu.window.foreColor[2] = 1.0f;
        menu.window.foreColor[3] = 1.0f;
        menu.fullScreen = 1;
        menu.onOpen = "setdvar ui_server_selected 0;";
        menu.onFocus = "setdvar ui_hint server_browser;";
        menu.onClose = "setdvar ui_hint clear;";
        menu.visibleExp = {static_cast<int>(visibleEntries.size()), visibleEntries.data()};
        menu.itemCount = static_cast<int>(std::size(items));
        menu.items = items;

        Zone zone("MockZone", 0, GameId::T4, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuDumperT4 dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("ui_mp/server_browser.menu");
        REQUIRE(file);

        constexpr auto expectedOutput = R"({
    menuDef
    {
        name                        "server_browser"
        fullscreen                  1
        rect                        0 0 640 480 0 0
        style                       1
        visible                     when(partyismissingmappack());
        onOpen
        {
            setdvar ui_server_selected 0;
        }
        onFocus
        {
            setdvar ui_hint server_browser;
        }
        onClose
        {
            setdvar ui_hint clear;
        }
        itemDef
        {
            name                        "server_list"
            text                        ""
            rect                        20 80 400 180 1 2
            style                       1
            type                        6
            visible                     1
            background                  "item_background"
            onFocus
            {
                play mouse_over;
            }
            elementwidth                200
            elementheight               18
            feeder                      2
            elementtype                 1
            columns                     2 4 120 32 0 128 64 8 2
            doubleclick
            {
                play mouse_click;
                open server_info;
            }
            onListboxSelectionChange
            {
                play mouse_over;
                setdvar ui_server_selected 1;
            }
            selectBorder                0.8 0.95 1 0
            disableColor                0.3 0.3 0.3 1
            focusColor                  1 0.8 0.2 1
            selectIcon                  "listbox_select_icon"
            backgroundItemListbox       "listbox_background"
            highlightTexture            "listbox_highlight"
        }
    }
}
)";
        REQUIRE(file->AsString() == expectedOutput);
    }

    TEST_CASE("MenuDumperT4: Prefers parent menu list path over ui_mp fallback", "[t4][menu][assetdumper]")
    {
        menuDef_t menu{};
        menu.window.name = "test_menu";

        menuDef_t* menus[]{&menu};

        MenuList menuList{};
        menuList.name = "ui/menus.txt";
        menuList.menuCount = static_cast<int>(std::size(menus));
        menuList.menus = menus;

        Zone zone("MockZone", 0, GameId::T4, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<MenuList>>(ASSET_TYPE_MENULIST, menuList.name, &menuList));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuListDumperT4 menuListDumper;
        menuListDumper.Dump(context);

        menu::MenuDumperT4 menuDumper;
        menuDumper.Dump(context);

        REQUIRE(mockOutput.GetMockedFile("ui/test_menu.menu"));
        REQUIRE_FALSE(mockOutput.GetMockedFile("ui_mp/test_menu.menu"));
    }
} // namespace
