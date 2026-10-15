#include "Game/IW4/Maps/MapEntsLoaderIW4.h"
#include "Game/IW4Xenon/Maps/MapEntsLoaderIW4Xenon.h"
#include "OatTestPaths.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_template_test_macros.hpp>
#include <cstring>
#include <fstream>
#include <string>
#include <type_traits>

namespace
{
    TEMPLATE_TEST_CASE("IW4 entity text overrides preserve loaded triggers and stages",
                       "[iw4][iw4xenon][textassets][mapents][system]",
                       IW4::AssetMapEnts,
                       IW4Xenon::AssetMapEnts)
    {
        constexpr auto GAME_ID = std::is_same_v<TestType, IW4::AssetMapEnts> ? GameId::IW4 : GameId::IW4Xenon;
        constexpr auto PLATFORM = GAME_ID == GameId::IW4 ? GamePlatform::PC : GamePlatform::XBOX;
        constexpr auto MAP_NAME = "maps/mp/override.d3dbsp";
        constexpr auto ORIGINAL_ENTITIES = "original entities";
        std::string entities;
        SECTION("Modified entity text")
        {
            entities = "{\n\"classname\" \"worldspawn\"\n}\n";
        }
        SECTION("Empty entity text") {}

        Zone source("original", 1, GAME_ID, PLATFORM);
        auto* original = source.Memory().template Alloc<typename TestType::Type>();
        original->name = MAP_NAME;
        original->entityString = source.Memory().Dup(ORIGINAL_ENTITIES);
        original->numEntityChars = static_cast<int>(std::strlen(ORIGINAL_ENTITIES) + 1);
        original->trigger.count = original->trigger.hullCount = original->trigger.slabCount = 1;
        original->trigger.models = source.Memory().template Alloc<std::remove_pointer_t<decltype(original->trigger.models)>>();
        original->trigger.hulls = source.Memory().template Alloc<std::remove_pointer_t<decltype(original->trigger.hulls)>>();
        original->trigger.slabs = source.Memory().template Alloc<std::remove_pointer_t<decltype(original->trigger.slabs)>>();
        original->trigger.models[0] = {0x12345, 1, 0};
        original->trigger.hulls[0].bounds.halfSize.x = 32.0f;
        original->trigger.hulls[0].slabCount = 1;
        original->trigger.slabs[0].dir[2] = 1.0f;
        original->trigger.slabs[0].halfSize = 16.0f;
        original->stageCount = 1;
        original->stages = source.Memory().template Alloc<std::remove_pointer_t<decltype(original->stages)>>();
        original->stages[0] = {
            "stage_one", {1.0f, 2.0f, 3.0f},
             0, 2
        };
        source.m_pools.AddAsset(TestType::EnumEntry, MAP_NAME, original, {}, {}, {});

        MockSearchPath searchPath;
        searchPath.AddFileData("maps/mp/override.d3dbsp.ents", entities);
        Zone target("override", 0, GAME_ID, PLATFORM);
        AssetCreatorCollection creators(target);
        if constexpr (GAME_ID == GameId::IW4)
            creators.AddAssetCreator(map_ents::CreateLoaderIW4(target.Memory(), searchPath));
        else
            creators.AddAssetCreator(map_ents::CreateLoaderIW4Xenon(target.Memory(), searchPath));
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        const auto* info = context.template LoadDependency<TestType>(MAP_NAME);
        REQUIRE(info);
        CHECK(info->m_zone == &target);
        CHECK(info->Asset() != original);
        CHECK(std::string(original->entityString) == ORIGINAL_ENTITIES);

        const auto checkMapEnts = [&](const Zone& zone)
        {
            const auto* mapInfo = zone.m_pools.template GetAsset<TestType>(MAP_NAME);
            REQUIRE(mapInfo);
            const auto* map = mapInfo->Asset();
            CHECK(map->numEntityChars == entities.size() + 1);
            CHECK(std::string(map->entityString, map->numEntityChars) == entities + '\0');
            REQUIRE(map->trigger.count == 1);
            REQUIRE(map->trigger.models);
            CHECK(map->trigger.models[0].contents == 0x12345);
            CHECK(map->trigger.models[0].hullCount == 1);
            REQUIRE(map->trigger.hullCount == 1);
            REQUIRE(map->trigger.hulls);
            CHECK(map->trigger.hulls[0].bounds.halfSize.x == 32.0f);
            CHECK(map->trigger.hulls[0].slabCount == 1);
            REQUIRE(map->trigger.slabCount == 1);
            REQUIRE(map->trigger.slabs);
            CHECK(map->trigger.slabs[0].dir[2] == 1.0f);
            CHECK(map->trigger.slabs[0].halfSize == 16.0f);
            REQUIRE(map->stageCount == 1);
            REQUIRE(map->stages);
            CHECK(std::string(map->stages[0].name) == "stage_one");
            CHECK(map->stages[0].origin[2] == 3.0f);
            CHECK(map->stages[0].sunPrimaryLightIndex == 2);
        };
        checkMapEnts(target);

#ifdef ARCH_x86
        const auto path = oat::paths::GetTempDirectory("IW4MapEntsOverride") / "override.ff";
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, target));
        }
        const auto reloaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(reloaded);
        checkMapEnts(**reloaded);
#endif
    }
} // namespace
