#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefinitionStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <sstream>

TEST_CASE("IW3Xenon253 zone definitions default to Xbox 360", "[iw3xenon253][zonedefinition]")
{
    const auto header = GENERATE(">game,IW3Xenon253\n", ">game,IW3Xenon253\n>platform,xbox360\n", ">platform,xbox360\n>game,IW3Xenon253\n");
    std::istringstream data(std::string(header) + "rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::IW3Xenon253);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
    REQUIRE(definition->m_assets.front().m_asset_type == IW3Xenon253::ASSET_TYPE_RAWFILE);
}

TEST_CASE("IW3Xenon253 zone definitions reject PC targets", "[iw3xenon253][zonedefinition]")
{
    const auto header = GENERATE(">game,IW3Xenon253\n>platform,pc\n", ">platform,pc\n>game,IW3Xenon253\n");
    std::istringstream data(header);
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    REQUIRE_FALSE(input.ReadDefinition());
}
