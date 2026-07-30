#include "Game/IW4/Maps/MapEntsDumperIW4.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

using namespace IW4;

namespace
{
    TEST_CASE("MapEnts dumper writes entity string and trigger map (IW4)", "[iw4][mapents][system]")
    {
        constexpr auto MAP_ENTS_NAME = "maps/mp/test_map.d3dbsp";
        char entityString[] = R"({
"classname" "worldspawn"
}
)";

        TriggerSlab triggerSlab{};
        triggerSlab.dir[0] = 1.0f;
        triggerSlab.midPoint = 10.0f;
        triggerSlab.halfSize = 0.5f;

        TriggerHull triggerHull{};
        triggerHull.bounds.midPoint.x = 10.0f;
        triggerHull.bounds.midPoint.y = 20.0f;
        triggerHull.bounds.midPoint.z = 30.0f;
        triggerHull.bounds.halfSize.x = 1.0f;
        triggerHull.bounds.halfSize.y = 2.0f;
        triggerHull.bounds.halfSize.z = 3.0f;
        triggerHull.slabCount = 1;

        TriggerModel triggerModel{};
        triggerModel.hullCount = 1;

        Stage stage{};
        MapEnts mapEnts{};
        mapEnts.name = MAP_ENTS_NAME;
        mapEnts.entityString = entityString;
        mapEnts.numEntityChars = sizeof(entityString);
        mapEnts.trigger.count = 1;
        mapEnts.trigger.models = &triggerModel;
        mapEnts.trigger.hullCount = 1;
        mapEnts.trigger.hulls = &triggerHull;
        mapEnts.trigger.slabCount = 1;
        mapEnts.trigger.slabs = &triggerSlab;
        mapEnts.stages = &stage;
        mapEnts.stageCount = 1;

        Zone zone("MockZone", 0, GameId::IW4, GamePlatform::PC);
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<MapEnts>>(ASSET_TYPE_MAP_ENTS, mapEnts.name, &mapEnts));

        MockSearchPath mockObjPath;
        MockOutputPath mockOutput;
        AssetDumpingContext dumpingContext(zone, "", mockOutput, mockObjPath, std::nullopt);
        map_ents::DumperIW4 dumper;
        dumper.Dump(dumpingContext);

        const auto* dumpedFile = mockOutput.GetMockedFile("maps/mp/test_map.d3dbsp.ents");
        REQUIRE(dumpedFile != nullptr);
        REQUIRE(dumpedFile->m_data.size() == sizeof(entityString) - 1);
        REQUIRE(dumpedFile->AsString() == entityString);

        const auto* dumpedMapFile = mockOutput.GetMockedFile("maps/mp/test_map.d3dbsp.map");
        REQUIRE(dumpedMapFile != nullptr);

        constexpr auto expectedMap = R"(iwmap 4
"000_Global" flags active
"The Map" flags
// entity 0
{
  "classname" "worldspawn"
  // trigger model 0
  // brush 0
  {
    ( 9 0 0 ) ( 9 64 0 ) ( 9 0 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 11 0 0 ) ( 11 -64 0 ) ( 11 0 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 0 18 0 ) ( -64 18 0 ) ( 0 18 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 0 22 0 ) ( 64 22 0 ) ( 0 22 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 0 0 27 ) ( 64 0 27 ) ( 0 64 27 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 0 0 33 ) ( -64 0 33 ) ( 0 64 33 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 9.500000 0 0 ) ( 9.500000 64 0 ) ( 9.500000 0 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
    ( 10.500000 0 0 ) ( 10.500000 -64 0 ) ( 10.500000 0 64 ) trigger 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0
  }
}
)";
        REQUIRE(dumpedMapFile->AsString() == expectedMap);
    }
} // namespace
