#include "GameTarget.h"

#include <format>

namespace game_target
{
    std::optional<GameId> Resolve(const GameId game, const GamePlatform platform)
    {
        if (game == GameId::COUNT || GetPublicGameId(game) != game)
            return std::nullopt;

        if (game == GameId::NX1 || game == GameId::IW3Xenon253 || game == GameId::IW3Xenon270)
            return platform == GamePlatform::XBOX ? std::make_optional(game) : std::nullopt;

        if (platform == GamePlatform::PC)
            return game;

        if (game == GameId::IW3 && platform == GamePlatform::XBOX)
            return GameId::IW3Xenon;

        if (game == GameId::IW4 && platform == GamePlatform::XBOX)
            return GameId::IW4Xenon;

        if (game == GameId::T4 && platform == GamePlatform::XBOX)
            return GameId::T4Xenon;

        if (game == GameId::QOS && platform == GamePlatform::XBOX)
            return GameId::QOSXenon;

        if (game == GameId::T5 && platform == GamePlatform::XBOX)
            return GameId::T5Xenon;

        return std::nullopt;
    }

    GameId GetPublicGameId(const GameId game)
    {
        if (game == GameId::IW3Xenon)
            return GameId::IW3;

        if (game == GameId::IW4Xenon)
            return GameId::IW4;

        if (game == GameId::T4Xenon)
            return GameId::T4;

        if (game == GameId::QOSXenon)
            return GameId::QOS;

        if (game == GameId::T5Xenon)
            return GameId::T5;

        return game;
    }

    std::string GetDisplayName(const GameId game, const GamePlatform platform)
    {
        const auto& gameName = IGame::GetGameById(GetPublicGameId(game))->GetShortName();
        if (platform == GamePlatform::PC)
            return gameName;

        return std::format("{}, {}", gameName, game_platform::ToName(platform));
    }
} // namespace game_target
