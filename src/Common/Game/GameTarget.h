#pragma once

#include "GamePlatform.h"
#include "IGame.h"

#include <optional>
#include <string>

namespace game_target
{
    [[nodiscard]] std::optional<GameId> Resolve(GameId game, GamePlatform platform);
    [[nodiscard]] GameId GetPublicGameId(GameId game);
    [[nodiscard]] std::string GetDisplayName(GameId game, GamePlatform platform);
} // namespace game_target
