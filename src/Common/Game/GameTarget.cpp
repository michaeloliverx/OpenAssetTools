#include "GameTarget.h"

#include <format>

namespace game_target
{
    std::optional<GameId> Resolve(const GameId game, const GamePlatform platform)
    {
        if (game == GameId::COUNT || GetPublicGameId(game) != game)
            return std::nullopt;

        if (platform == GamePlatform::PC)
            return game;

        if (game == GameId::IW3 && platform == GamePlatform::XBOX)
            return GameId::IW3Xenon;

        if (game == GameId::T4 && platform == GamePlatform::XBOX)
            return GameId::T4Xenon;

        return std::nullopt;
    }

    GameId GetPublicGameId(const GameId game)
    {
        if (game == GameId::IW3Xenon)
            return GameId::IW3;

        if (game == GameId::T4Xenon)
            return GameId::T4;

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
