#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum class GamePlatform : std::uint8_t
{
    PC,
    XBOX,
    PS3,
    WIIU
};

namespace game_platform
{
    [[nodiscard]] std::optional<GamePlatform> FromName(const std::string& name);
    [[nodiscard]] const char* ToName(GamePlatform platform);
} // namespace game_platform
