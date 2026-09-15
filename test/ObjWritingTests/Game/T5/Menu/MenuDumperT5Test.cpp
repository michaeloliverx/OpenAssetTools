#include "Game/T5/Menu/MenuDumperT5.h"

#include "Game/T5/Menu/MenuListDumperT5.h"
#include "Game/T5/MenuConstantsT5.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>
#include <memory>

using namespace T5;

namespace
{
    expressionRpn IntOperand(const int value)
    {
        expressionRpn entry{};
        entry.type = RPN_CONSTANT;
        entry.data.constant.dataType = VAL_INT;
        entry.data.constant.internals.intVal = value;
        return entry;
    }

    expressionRpn StringOperand(const char* value)
    {
        expressionRpn entry{};
        entry.type = RPN_CONSTANT;
        entry.data.constant.dataType = VAL_STRING;
        entry.data.constant.internals.stringVal = value;
        return entry;
    }

    expressionRpn Command(const int commandIndex)
    {
        expressionRpn entry{};
        entry.type = RPN_CMD_IDX;
        entry.data.cmdIdx = commandIndex;
        return entry;
    }

    expressionRpn EndExpression()
    {
        expressionRpn entry{};
        entry.type = RPN_END;
        return entry;
    }

    TEST_CASE("MenuDumperT5: Can dump T5 expressions, event scripts, and nested item data", "[t5][menu][assetdumper]")
    {
        constexpr auto DVAR_BOOL_FUNCTION = NUM_EXPRESSION_OPERATORS + 7;
        constexpr auto MILLISECONDS_FUNCTION = NUM_EXPRESSION_OPERATORS + 4;
        constexpr auto SIN_FUNCTION = NUM_EXPRESSION_OPERATORS;
        constexpr auto MIN_FUNCTION = NUM_EXPRESSION_OPERATORS + 2;

        std::array visibleExpression{
            StringOperand("ui_test"),
            Command(DVAR_BOOL_FUNCTION),
            Command(MILLISECONDS_FUNCTION),
            IntOperand(10),
            Command(SIN_FUNCTION),
            IntOperand(20),
            Command(OP_COMMA),
            Command(MIN_FUNCTION),
            Command(OP_GREATERTHAN),
            Command(OP_AND),
            EndExpression(),
        };

        GenericEventScript actionScript{
            .type = EVENT_UNCONDITIONAL,
            .action = "play mouse_click; setcolor bordercolor 0.98 .83 .25 1; open options;",
        };
        GenericEventHandler actionHandler{
            .name = "action",
            .eventScript = &actionScript,
        };
        GenericEventScript acceptScript{
            .type = EVENT_UNCONDITIONAL,
            .action = "accept name_field;",
        };
        GenericEventHandler acceptHandler{
            .name = "onAccept",
            .eventScript = &acceptScript,
            .next = &actionHandler,
        };

        ItemKeyHandler keyHandler{
            .key = 'a',
            .keyScript = &actionScript,
        };

        editFieldDef_s editField{
            .minVal = -1.0f,
            .maxVal = -1.0f,
            .defVal = -1.0f,
            .maxChars = 16,
            .maxPaintChars = 16,
        };
        focusItemDef_s focusDef{
            .mouseEnter = "play mouse_over;",
            .onKey = &keyHandler,
        };
        focusDef.focusTypeData.editField = &editField;

        textDef_s textDef{};
        textDef.text = "@MENU_NAME";
        textDef.textAlignMode = 1;
        textDef.textalignx = -28.0f;
        textDef.textaligny = 14.0f;
        textDef.textscale = 0.3f;
        textDef.fontEnum = 1;
        textDef.textTypeData.focusItemDef = &focusDef;

        itemDef_s item{};
        item.window.name = "name_field";
        item.window.group = "fields";
        item.window.rectClient = {.x = 10.0f, .y = 56.0f, .w = 180.0f, .h = 24.0f, .horzAlign = 1, .vertAlign = 2};
        item.window.style = 1;
        item.window.borderSize = 1.0f;
        item.window.dynamicFlags[0] = WINDOW_FLAG_VISIBLE;
        item.window.foreColor[0] = 1.0f;
        item.window.foreColor[1] = 1.0f;
        item.window.foreColor[2] = 1.0f;
        item.window.foreColor[3] = 1.0f;
        item.type = ITEM_TYPE_EDITFIELD;
        item.dvar = "player_name";
        item.typeData.textDef = &textDef;
        item.hideBits = std::numeric_limits<uint64_t>::max();
        item.onEvent = &acceptHandler;
        item.ui3dWindowId = -1;

        itemDef_s* items[]{&item};

        GenericEventScript openScript{
            .type = EVENT_UNCONDITIONAL,
            .action = "play mouse_open; setfocus name_field;",
        };
        GenericEventHandler openHandler{
            .name = "onOpen",
            .eventScript = &openScript,
        };

        menuDef_t menu{};
        menu.window.name = "test_menu";
        menu.window.rect = {.x = 0.0f, .y = 0.0f, .w = 640.0f, .h = 480.0f, .horzAlign = 0, .vertAlign = 0};
        menu.window.borderSize = 1.0f;
        menu.window.foreColor[0] = 1.0f;
        menu.window.foreColor[1] = 1.0f;
        menu.window.foreColor[2] = 1.0f;
        menu.window.foreColor[3] = 1.0f;
        menu.fullScreen = 1;
        menu.ui3dWindowId = -1;
        menu.priority = 2;
        menu.openSlideSpeed = -1;
        menu.closeSlideSpeed = -1;
        menu.openSlideDirection = -1;
        menu.closeSlideDirection = -1;
        menu.openFadingTime = -1;
        menu.closeFadingTime = -1;
        menu.onEvent = &openHandler;
        menu.visibleExp = {.numRpn = static_cast<int>(visibleExpression.size()), .rpn = visibleExpression.data()};
        menu.showBits = uint64_t{1} << 2;
        menu.hideBits = std::numeric_limits<uint64_t>::max();
        menu.hideBits &= ~(uint64_t{1} << 12);
        menu.itemCount = static_cast<int>(std::size(items));
        menu.items = items;

        Zone zone("MockZone", 0, GameId::T5, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuDumperT5 dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("ui_mp/test_menu.menu");
        REQUIRE(file);

        constexpr auto expectedOutput = R"({
    menuDef
    {
        name                        "test_menu"
        fullscreen                  1
        rect                        0 0 640 480 0 0
        visible                     when((dvarbool("ui_test") && (milliseconds() > min(sin(10), 20))));
        visibilityBits              bits( 2 !12 )
        onOpen
        {
            play mouse_open;
            setfocus name_field;
        }
        itemDef
        {
            name                        "name_field"
            group                       "fields"
            rect                        10 56 180 24 1 2
            style                       1
            type                        5
            text                        "@MENU_NAME"
            visible                     1
            textalign                   1
            textalignx                  -28
            textaligny                  14
            textscale                   0.3
            textfont                    1
            action
            {
                play mouse_click;
                setcolor bordercolor 0.98 0.83 0.25 1;
                open options;
            }
            accept
            {
                accept name_field;
            }
            mouseEnter
            {
                play mouse_over;
            }
            execKey "a"
            {
                play mouse_click;
                setcolor bordercolor 0.98 0.83 0.25 1;
                open options;
            }
            dvar                        "player_name"
            maxChars                    16
            maxPaintChars               16
        }
    }
}
)";
        REQUIRE(file->AsString() == expectedOutput);
    }

    TEST_CASE("MenuDumperT5: Prefers parent menu list path over ui_mp fallback", "[t5][menu][assetdumper]")
    {
        menuDef_t menu{};
        menu.window.name = "test_menu";

        menuDef_t* menus[]{&menu};

        MenuList menuList{};
        menuList.name = "ui/menus.txt";
        menuList.menuCount = static_cast<int>(std::size(menus));
        menuList.menus = menus;

        Zone zone("MockZone", 0, GameId::T5, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<MenuList>>(ASSET_TYPE_MENULIST, menuList.name, &menuList));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuListDumperT5 menuListDumper;
        menuListDumper.Dump(context);

        menu::MenuDumperT5 menuDumper;
        menuDumper.Dump(context);

        REQUIRE(mockOutput.GetMockedFile("ui/test_menu.menu"));
        REQUIRE_FALSE(mockOutput.GetMockedFile("ui_mp/test_menu.menu"));
    }

    TEST_CASE("MenuDumperT5: Dumps root comma expressions and dynamic listbox columns", "[t5][menu][assetdumper]")
    {
        constexpr auto SIN_FUNCTION = NUM_EXPRESSION_OPERATORS;
        std::array ownerDataExpression{
            IntOperand(1),
            Command(SIN_FUNCTION),
            IntOperand(2),
            Command(OP_COMMA),
            EndExpression(),
        };

        ownerDrawDef_s ownerDrawDef{
            .dataExp = {.numRpn = static_cast<int>(ownerDataExpression.size()), .rpn = ownerDataExpression.data()},
        };
        itemDef_s ownerDrawItem{};
        ownerDrawItem.type = ITEM_TYPE_OWNERDRAW;
        ownerDrawItem.typeData.ownerDrawDef = &ownerDrawDef;
        ownerDrawItem.hideBits = std::numeric_limits<uint64_t>::max();
        ownerDrawItem.ui3dWindowId = -1;

        MenuCell cells[2]{};
        cells[0].type = 2;
        cells[1].type = 5;
        MenuRow rows[1]{};
        rows[0].cells = cells;

        listBoxDef_s listBox{};
        listBox.numColumns = 2;
        listBox.columnInfo[0].elementStyle = 0;
        listBox.columnInfo[1].elementStyle = 0;
        listBox.rows = rows;
        listBox.maxRows = 1;
        listBox.rowCount = 0;

        focusItemDef_s focusDef{};
        focusDef.focusTypeData.listBox = &listBox;
        textDef_s textDef{};
        textDef.textTypeData.focusItemDef = &focusDef;

        itemDef_s listBoxItem{};
        listBoxItem.type = ITEM_TYPE_LISTBOX;
        listBoxItem.typeData.textDef = &textDef;
        listBoxItem.hideBits = std::numeric_limits<uint64_t>::max();
        listBoxItem.ui3dWindowId = -1;

        itemDef_s* items[]{&ownerDrawItem, &listBoxItem};

        menuDef_t menu{};
        menu.window.name = "listbox_menu";
        menu.window.borderSize = 1.0f;
        menu.window.foreColor[0] = 1.0f;
        menu.window.foreColor[1] = 1.0f;
        menu.window.foreColor[2] = 1.0f;
        menu.window.foreColor[3] = 1.0f;
        menu.ui3dWindowId = -1;
        menu.priority = 2;
        menu.openSlideSpeed = -1;
        menu.closeSlideSpeed = -1;
        menu.openSlideDirection = -1;
        menu.closeSlideDirection = -1;
        menu.openFadingTime = -1;
        menu.closeFadingTime = -1;
        menu.hideBits = std::numeric_limits<uint64_t>::max();
        menu.itemCount = static_cast<int>(std::size(items));
        menu.items = items;

        Zone zone("MockZone", 0, GameId::T5, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuDumperT5 dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("ui_mp/listbox_menu.menu");
        REQUIRE(file);

        const auto output = file->AsString();
        REQUIRE(output.find("exp ownerdata               (sin(1), 2);") != std::string::npos);
        REQUIRE(output.find("elementtype                 0\n") != std::string::npos);
        REQUIRE(output.find("type                        2") != std::string::npos);
        REQUIRE(output.find("type                        5") != std::string::npos);
    }

    TEST_CASE("MenuDumperT5: Preserves conditional custom event behavior", "[t5][menu][assetdumper]")
    {
        std::array conditionExpression{
            IntOperand(1),
            EndExpression(),
        };

        ScriptCondition elsePrerequisite{
            .fireOnTrue = false,
            .constructID = 4,
            .blockID = 2,
        };
        GenericEventScript elseScript{
            .prerequisites = &elsePrerequisite,
            .type = EVENT_ELSE,
            .action = "setdvar ui_choice no;",
            .blockID = 3,
            .constructID = 4,
        };
        GenericEventScript ifScript{
            .condition = {.numRpn = static_cast<int>(conditionExpression.size()), .rpn = conditionExpression.data()},
            .type = EVENT_IF,
            .fireOnTrue = true,
            .action = "setdvar ui_choice yes;",
            .blockID = 2,
            .constructID = 4,
            .next = &elseScript,
        };
        GenericEventHandler customHandler{
            .name = "Choice",
            .eventScript = &ifScript,
        };

        itemDef_s item{};
        item.window.name = "conditional_item";
        item.window.borderSize = 1.0f;
        item.window.foreColor[0] = 1.0f;
        item.window.foreColor[1] = 1.0f;
        item.window.foreColor[2] = 1.0f;
        item.window.foreColor[3] = 1.0f;
        item.hideBits = std::numeric_limits<uint64_t>::max();
        item.ui3dWindowId = -1;
        item.onEvent = &customHandler;

        itemDef_s* items[]{&item};

        menuDef_t menu{};
        menu.window.name = "conditional_menu";
        menu.window.borderSize = 1.0f;
        menu.window.foreColor[0] = 1.0f;
        menu.window.foreColor[1] = 1.0f;
        menu.window.foreColor[2] = 1.0f;
        menu.window.foreColor[3] = 1.0f;
        menu.ui3dWindowId = -1;
        menu.priority = 2;
        menu.openSlideSpeed = -1;
        menu.closeSlideSpeed = -1;
        menu.openSlideDirection = -1;
        menu.closeSlideDirection = -1;
        menu.openFadingTime = -1;
        menu.closeFadingTime = -1;
        menu.hideBits = std::numeric_limits<uint64_t>::max();
        menu.itemCount = static_cast<int>(std::size(items));
        menu.items = items;

        Zone zone("MockZone", 0, GameId::T5, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<menuDef_t>>(ASSET_TYPE_MENU, menu.window.name, &menu));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext context(zone, "", mockOutput, mockObjPath, std::nullopt);

        menu::MenuDumperT5 dumper;
        dumper.Dump(context);

        const auto* file = mockOutput.GetMockedFile("ui_mp/conditional_menu.menu");
        REQUIRE(file);

        const auto output = file->AsString();
        REQUIRE(output.find("onEvent                     Choice") != std::string::npos);
        REQUIRE(output.find("if (1)") != std::string::npos);
        REQUIRE(output.find("if (!(1))") != std::string::npos);
        REQUIRE(output.find("setdvar ui_choice yes;") < output.find("setdvar ui_choice no;"));
    }
} // namespace
