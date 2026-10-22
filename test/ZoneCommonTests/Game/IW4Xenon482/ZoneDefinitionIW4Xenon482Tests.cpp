#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefinitionStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <sstream>

TEST_CASE("IW4Xenon482 zone definitions default to Xbox 360", "[iw4xenon482][zonedefinition]")
{
    const auto header = GENERATE(">game,IW4Xenon482\n", ">game,IW4Xenon482\n>platform,xbox360\n", ">platform,xbox360\n>game,IW4Xenon482\n");
    std::istringstream data(std::string(header) + "rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::IW4Xenon482);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
    REQUIRE(definition->m_assets.front().m_asset_type == IW4Xenon482::ASSET_TYPE_RAWFILE);
}

TEST_CASE("IW4Xenon482 zone definitions reject PC targets", "[iw4xenon482][zonedefinition]")
{
    const auto header = GENERATE(">game,IW4Xenon482\n>platform,pc\n", ">platform,pc\n>game,IW4Xenon482\n");
    std::istringstream data(header);
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    REQUIRE_FALSE(input.ReadDefinition());
}
