#include "Game/IW2Xenon/IW2Xenon.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefinitionStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <sstream>

TEST_CASE("IW2Xenon zone definitions default to Xbox 360", "[iw2xenon][zonedefinition]")
{
    const auto header = GENERATE(">game,IW2\n", ">game,IW2\n>platform,xbox360\n", ">platform,xbox360\n>game,IW2\n");
    std::istringstream data(std::string(header) + "rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::IW2Xenon);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
    REQUIRE(definition->m_assets.front().m_asset_type == IW2Xenon::ASSET_TYPE_RAWFILE);
}

TEST_CASE("IW2Xenon zone definitions reject PC targets", "[iw2xenon][zonedefinition]")
{
    const auto header = GENERATE(">game,IW2\n>platform,pc\n", ">platform,pc\n>game,IW2\n");
    std::istringstream data(header);
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    REQUIRE_FALSE(input.ReadDefinition());
}
