#include "Game/CommonAsset.h"
#include "Game/GameTarget.h"
#include "Game/T5Xenon/T5Xenon.h"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("T5 Xbox resolves to the Xenon target with standard OAT names", "[t5xenon][gametarget]")
{
    REQUIRE(game_target::Resolve(GameId::T5, GamePlatform::XBOX) == GameId::T5Xenon);
    REQUIRE(game_target::Resolve(GameId::T5, GamePlatform::PC) == GameId::T5);
    REQUIRE(game_target::GetPublicGameId(GameId::T5Xenon) == GameId::T5);
    const auto* game = IGame::GetGameById(GameId::T5Xenon);
    REQUIRE(std::string_view(*game->GetAssetTypeName(T5Xenon::ASSET_TYPE_MAP_ENTS)) == "mapents");
    REQUIRE(std::string_view(*game->GetAssetTypeName(T5Xenon::ASSET_TYPE_SOUND)) == "soundbank");
    REQUIRE(std::string_view(*game->GetAssetTypeName(T5Xenon::ASSET_TYPE_PIXEL_SHADER)) == "pixelshader");
    REQUIRE(game->GetSubAssetTypeCount() == 3);
    REQUIRE(std::string_view(*game->GetSubAssetTypeName(2)) == "vertexshader");
    REQUIRE(ICommonAssetTypeMapper::GetCommonAssetMapperByGame(GameId::T5Xenon)->GameToCommonAssetType(T5Xenon::ASSET_TYPE_PIXEL_SHADER)
            == CommonAssetType::PIXEL_SHADER);
}
