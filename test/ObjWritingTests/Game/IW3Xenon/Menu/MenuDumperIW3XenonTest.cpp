#include "Game/IW3Xenon/Menu/MenuWriterIW3Xenon.h"
#include "Parsing/Menu/MenuFileReader.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

using namespace IW3Xenon;

namespace
{
    TEST_CASE("MenuDumperIW3Xenon: Omits retail expressions containing only parentheses", "[iw3xenon][menu][assetdumper]")
    {
        expressionEntry opening{};
        opening.type = EET_OPERATOR;
        opening.data.op = OP_LEFTPAREN;
        expressionEntry closing{};
        closing.type = EET_OPERATOR;
        closing.data.op = OP_RIGHTPAREN;
        expressionEntry* entries[]{&opening, &opening, &closing, &closing};
        const statement_s emptyExpression{.numEntries = static_cast<int>(std::size(entries)), .entries = entries};

        itemDef_s item{};
        item.window.name = "empty_expression";
        item.visibleExp = emptyExpression;
        item.textExp = emptyExpression;
        item.rectXExp = emptyExpression;
        itemDef_s* items[]{&item};

        menuDef_t menu{};
        menu.window.name = "test";
        menu.visibleExp = emptyExpression;
        menu.itemCount = static_cast<int>(std::size(items));
        menu.items = items;

        std::ostringstream output;
        const auto writer = menu::CreateMenuWriterIW3Xenon(output);
        writer->Start();
        writer->WriteMenu(menu);
        writer->End();

        const auto source = output.str();
        REQUIRE(source.find("when(") == std::string::npos);
        REQUIRE(source.find("exp text") == std::string::npos);
        REQUIRE(source.find("exp rect") == std::string::npos);

        MockSearchPath searchPath;
        menu::MenuAssetZoneState zoneState;
        std::istringstream input(source);
        menu::MenuFileReader reader(input, "ui_mp/test.menu", menu::FeatureLevel::IW3, searchPath);
        reader.IncludeZoneState(zoneState);
        const auto parsed = reader.ReadMenuFile();
        REQUIRE(parsed);
        REQUIRE(parsed->m_menus.size() == 1);
        REQUIRE(parsed->m_menus[0]->m_items.size() == 1);
    }
} // namespace
