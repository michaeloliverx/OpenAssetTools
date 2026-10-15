#include "Game/GameTarget.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "Game/NX1/NX1.h"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("NX1 is an Xbox 360-only target with standard asset names", "[game][gametarget][nx1]")
{
    REQUIRE(game_target::Resolve(GameId::NX1, GamePlatform::XBOX) == GameId::NX1);
    REQUIRE_FALSE(game_target::Resolve(GameId::NX1, GamePlatform::PC));
    REQUIRE(game_target::GetPublicGameId(GameId::NX1) == GameId::NX1);
    REQUIRE(game_target::GetDisplayName(GameId::NX1, GamePlatform::XBOX) == "NX1, xbox360");
    const auto* nx1 = IGame::GetGameById(GameId::NX1);
    const auto* iw4 = IGame::GetGameById(GameId::IW4Xenon);
    for (asset_type_t type = 0; type < iw4->GetAssetTypeCount(); ++type)
    {
        const auto name = iw4->GetAssetTypeName(type);
        REQUIRE(name);
        INFO(*name);
        const auto nx1Type = nx1->FindAssetTypeByName(*name);
        REQUIRE(nx1Type);
        REQUIRE(std::string_view(*nx1->GetAssetTypeName(*nx1Type)) == *name);
    }
    REQUIRE(nx1->FindAssetTypeByName("soundlpfcurve") == NX1::ASSET_TYPE_SND_LPF_CURVE);
    REQUIRE(nx1->GetSubAssetTypeCount() == 3);
}
