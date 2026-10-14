#include "Game/GameTarget.h"
#include "Game/T4/T4.h"
#include "Game/T4Xenon/T4Xenon.h"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <utility>

namespace test::game::game_target
{
    TEST_CASE("Game target display names hide internal game variants", "[game][gametarget]")
    {
        REQUIRE(::game_target::GetDisplayName(GameId::IW3Xenon, GamePlatform::XBOX) == "IW3, xbox360");
        REQUIRE(::game_target::GetDisplayName(GameId::IW3, GamePlatform::PC) == "IW3");
        REQUIRE(::game_target::GetDisplayName(GameId::T4Xenon, GamePlatform::XBOX) == "T4, xbox360");
    }

    TEST_CASE("T4 Xbox targets preserve public PC asset names", "[game][gametarget][t4xenon]")
    {
        REQUIRE(::game_target::Resolve(GameId::T4, GamePlatform::PC) == GameId::T4);
        REQUIRE(::game_target::Resolve(GameId::T4, GamePlatform::XBOX) == GameId::T4Xenon);
        REQUIRE(::game_target::GetPublicGameId(GameId::T4Xenon) == GameId::T4);
        REQUIRE_FALSE(::game_target::Resolve(GameId::T4Xenon, GamePlatform::PC));

        const auto* pc = IGame::GetGameById(GameId::T4);
        const auto* xenon = IGame::GetGameById(GameId::T4Xenon);
        for (asset_type_t type = 0; type < pc->GetAssetTypeCount(); ++type)
        {
            const auto pcName = pc->GetAssetTypeName(type);
            REQUIRE(pcName);
            INFO(*pcName);
            const auto xenonType = xenon->FindAssetTypeByName(*pcName);
            REQUIRE(xenonType);
            const auto xenonName = xenon->GetAssetTypeName(*xenonType);
            REQUIRE(xenonName);
            REQUIRE(std::string_view(*pcName) == *xenonName);
        }
    }

    TEST_CASE("T4 Xbox subassets preserve PC names with pixel shaders registered as assets", "[game][gametarget][t4xenon]")
    {
        constexpr std::pair<asset_type_t, asset_type_t> subAssetTypes[]{
            {T4::SubAssetTechnique::EnumEntry,     T4Xenon::SubAssetTechnique::EnumEntry    },
            {T4::SubAssetVertexDecl::EnumEntry,    T4Xenon::SubAssetVertexDecl::EnumEntry   },
            {T4::SubAssetVertexShader::EnumEntry,  T4Xenon::SubAssetVertexShader::EnumEntry },
            {T4::SubAssetAccuracyGraph::EnumEntry, T4Xenon::SubAssetAccuracyGraph::EnumEntry},
            {T4::SubAssetFlameTable::EnumEntry,    T4Xenon::SubAssetFlameTable::EnumEntry   },
        };

        const auto* pc = IGame::GetGameById(GameId::T4);
        const auto* xenon = IGame::GetGameById(GameId::T4Xenon);
        REQUIRE(xenon->GetSubAssetTypeCount() == pc->GetSubAssetTypeCount() - 1);
        REQUIRE(xenon->GetSubAssetTypeCount() == std::extent_v<decltype(subAssetTypes)>);
        REQUIRE_FALSE(xenon->GetSubAssetTypeName(xenon->GetSubAssetTypeCount()));
        REQUIRE(xenon->FindAssetTypeByName("pixelshader") == T4Xenon::AssetPixelShader::EnumEntry);

        for (const auto [pcType, xenonType] : subAssetTypes)
        {
            const auto pcName = pc->GetSubAssetTypeName(pcType);
            REQUIRE(pcName);
            INFO(*pcName);
            const auto xenonName = xenon->GetSubAssetTypeName(xenonType);
            REQUIRE(xenonName);
            REQUIRE(std::string_view(*pcName) == *xenonName);
        }
    }
} // namespace test::game::game_target
