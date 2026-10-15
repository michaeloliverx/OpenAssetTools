#include "Game/GameTarget.h"
#include "Game/IW4/IW4.h"
#include "Game/IW4Xenon/IW4Xenon.h"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <utility>

TEST_CASE("IW4 Xbox targets use public game and asset names", "[game][gametarget][iw4xenon]")
{
    REQUIRE(game_target::Resolve(GameId::IW4, GamePlatform::XBOX) == GameId::IW4Xenon);
    REQUIRE(game_target::GetPublicGameId(GameId::IW4Xenon) == GameId::IW4);
    REQUIRE(game_target::GetDisplayName(GameId::IW4Xenon, GamePlatform::XBOX) == "IW4, xbox360");
    REQUIRE_FALSE(game_target::Resolve(GameId::IW4Xenon, GamePlatform::PC));

    const auto* pc = IGame::GetGameById(GameId::IW4);
    const auto* xenon = IGame::GetGameById(GameId::IW4Xenon);
    for (asset_type_t type = 0; type < pc->GetAssetTypeCount(); ++type)
    {
        if (type == IW4::ASSET_TYPE_VERTEXSHADER || type == IW4::ASSET_TYPE_VERTEXDECL)
            continue;
        const auto name = pc->GetAssetTypeName(type);
        REQUIRE(name);
        INFO(*name);
        const auto xenonType = xenon->FindAssetTypeByName(*name);
        REQUIRE(xenonType);
        REQUIRE(std::string_view(*xenon->GetAssetTypeName(*xenonType)) == *name);
    }
    REQUIRE(xenon->FindAssetTypeByName("phys_collmap") == IW4Xenon::ASSET_TYPE_PHYSCOLLMAP);
    REQUIRE(xenon->FindAssetTypeByName("techset") == IW4Xenon::ASSET_TYPE_TECHNIQUE_SET);
}

TEST_CASE("IW4 Xbox classifies nested techset and weapon data as subassets", "[game][gametarget][iw4xenon]")
{
    const auto* pc = IGame::GetGameById(GameId::IW4);
    const auto* xenon = IGame::GetGameById(GameId::IW4Xenon);
    const std::pair<asset_type_t, const char*> subAssetNames[]{
        {IW4Xenon::SubAssetTechnique::EnumEntry,     *pc->GetSubAssetTypeName(IW4::SubAssetTechnique::EnumEntry)    },
        {IW4Xenon::SubAssetVertexDecl::EnumEntry,    *pc->GetAssetTypeName(IW4::AssetVertexDecl::EnumEntry)         },
        {IW4Xenon::SubAssetVertexShader::EnumEntry,  *pc->GetAssetTypeName(IW4::AssetVertexShader::EnumEntry)       },
        {IW4Xenon::SubAssetAccuracyGraph::EnumEntry, *pc->GetSubAssetTypeName(IW4::SubAssetAccuracyGraph::EnumEntry)},
    };
    REQUIRE(xenon->GetSubAssetTypeCount() == std::extent_v<decltype(subAssetNames)>);
    REQUIRE_FALSE(xenon->GetSubAssetTypeName(xenon->GetSubAssetTypeCount()));

    for (const auto [type, expectedName] : subAssetNames)
    {
        const auto name = xenon->GetSubAssetTypeName(type);
        REQUIRE(name);
        REQUIRE(std::string_view(*name) == expectedName);
        REQUIRE_FALSE(xenon->FindAssetTypeByName(expectedName));
    }
    REQUIRE(xenon->FindAssetTypeByName("pixelshader") == IW4Xenon::AssetPixelShader::EnumEntry);
}
