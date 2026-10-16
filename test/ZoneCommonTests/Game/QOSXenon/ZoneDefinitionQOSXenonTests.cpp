#include "Game/QOS/QOS.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefinitionStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <sstream>

TEST_CASE("QoS Xenon zone definitions resolve Xbox 360 targets", "[qosxenon][zonedefinition]")
{
    const auto header = GENERATE(">game,QOS\n>platform,xbox360\n", ">platform,xbox360\n>game,QOS\n");
    std::istringstream data(std::string(header) + "rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::QOSXenon);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
    REQUIRE(definition->m_assets.front().m_asset_type == QOSXenon::ASSET_TYPE_RAWFILE);
}

TEST_CASE("QoS Xenon asset names match PC QoS", "[qosxenon][assets]")
{
    const auto* pc = IGame::GetGameById(GameId::QOS);
    const auto* xenon = IGame::GetGameById(GameId::QOSXenon);
    for (auto type = 0; type < QOS::ASSET_TYPE_COUNT; ++type)
    {
        const auto xenonType = type >= QOS::ASSET_TYPE_TECHNIQUE_SET ? type + 1 : type;
        REQUIRE(std::string(*pc->GetAssetTypeName(type)) == *xenon->GetAssetTypeName(xenonType));
    }
    REQUIRE(std::string(*xenon->GetAssetTypeName(QOSXenon::ASSET_TYPE_PIXELSHADER)) == "pixelshader");
    REQUIRE(std::string(*xenon->GetSubAssetTypeName(QOSXenon::SUB_ASSET_TYPE_TECHNIQUE)) == "technique");
    REQUIRE(std::string(*xenon->GetSubAssetTypeName(QOSXenon::SUB_ASSET_TYPE_VERTEX_DECL)) == "vertexdecl");
    REQUIRE(std::string(*xenon->GetSubAssetTypeName(QOSXenon::SUB_ASSET_TYPE_VERTEX_SHADER)) == "vertexshader");
}

TEST_CASE("QoS Xenon includes inherit the Xbox 360 target", "[qosxenon][zonedefinition]")
{
    std::istringstream data("rawfile,test.txt\n");
    MockSearchPath searchPath;
    ZoneDefinitionInputStream input(data, "test", "test.zone", searchPath);
    input.SetPreviouslySetGame(GameId::QOSXenon);
    const auto definition = input.ReadDefinition();
    REQUIRE(definition);
    REQUIRE(definition->GetResolvedGameId() == GameId::QOSXenon);
    REQUIRE(definition->m_platform == GamePlatform::XBOX);
}
