#include "Game/IW4/IW4.h"
#include "SystemTestsPaths.h"
#include "ZoneLoading.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string_view>

namespace
{
    TEST_CASE("Shared retail Xenon loader(IW4)", "[iw4][xenon][system]")
    {
        const auto zonePath = oat::paths::GetSystemTestsDirectory() / "Game/IW4/RetailXenonSharedLoader/iw4_xenon_shared_smoke.ff";
        auto maybeZone = ZoneLoading::LoadZone(zonePath.string(), std::nullopt);
        REQUIRE(maybeZone);

        auto zone = std::move(*maybeZone);

        REQUIRE(zone->m_game_id == GameId::IW4);
        REQUIRE(zone->m_platform == GamePlatform::XBOX);
        REQUIRE(zone->m_name == "iw4_xenon_shared_smoke");
        REQUIRE(zone->m_pools.GetTotalAssetCount() == 11);

        REQUIRE(zone->m_pools.GetAsset<IW4::AssetPhysPreset>(",iw4_xenon_test_phys"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetPhysCollMap>(",iw4_xenon_test_collmap"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetSoundCurve>(",iw4_xenon_test_curve"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetMapEnts>(",iw4_xenon_test_mapents"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetLightDef>(",iw4_xenon_test_light"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetLocalize>(",IW4_XENON_TEST_LOCALIZE"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetLeaderboard>(",iw4_xenon_test_leaderboard"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetTracer>(",iw4_xenon_test_tracer"));
        REQUIRE(zone->m_pools.GetAsset<IW4::AssetAddonMapEnts>(",iw4_xenon_test_addon_mapents"));

        const auto* rawFile = zone->m_pools.GetAsset<IW4::AssetRawFile>("iw4_xenon_test.txt");
        REQUIRE(rawFile);
        REQUIRE(rawFile->Asset()->compressedLen > 0);
        REQUIRE(rawFile->Asset()->len == 118);
        REQUIRE(rawFile->Asset()->data.compressedBuffer);

        const auto* stringTable = zone->m_pools.GetAsset<IW4::AssetStringTable>("iw4_xenon_test.csv");
        REQUIRE(stringTable);
        REQUIRE(stringTable->Asset()->columnCount == 2);
        REQUIRE(stringTable->Asset()->rowCount == 3);
        REQUIRE(std::string_view(stringTable->Asset()->values[2].string) == "endianness");
        REQUIRE(std::string_view(stringTable->Asset()->values[3].string) == "big");
        REQUIRE(std::string_view(stringTable->Asset()->values[5].string) == "xbox360");
    }
} // namespace
