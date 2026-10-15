#include "Game/NX1/NX1.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefinitionStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <sstream>

TEST_CASE("NX1 zone definitions default to Xbox 360", "[nx1][zonedefinition]")
{
    const auto header = GENERATE(">game,NX1\n", ">game,NX1\n>platform,xbox360\n", ">platform,xbox360\n>game,NX1\n");
    std::istringstream data(std::string(header) + "rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::NX1);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
    REQUIRE(definition->m_assets.front().m_asset_type == NX1::ASSET_TYPE_RAWFILE);
}

TEST_CASE("NX1 zone definitions reject PC targets", "[nx1][zonedefinition]")
{
    const auto header = GENERATE(">game,NX1\n>platform,pc\n", ">platform,pc\n>game,NX1\n");
    std::istringstream data(header);
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    REQUIRE_FALSE(input.ReadDefinition());
}
