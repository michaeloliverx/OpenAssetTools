#include "GamePlatform.h"

#include "Utils/StringUtils.h"

#include <cassert>

namespace game_platform
{
    std::optional<GamePlatform> FromName(const std::string& name)
    {
        auto lowerName = name;
        utils::MakeStringLowerCase(lowerName);

        if (lowerName == "pc")
            return GamePlatform::PC;
        if (lowerName == "xbox360")
            return GamePlatform::XBOX;
        if (lowerName == "ps3")
            return GamePlatform::PS3;
        if (lowerName == "wiiu")
            return GamePlatform::WIIU;

        return std::nullopt;
    }

    const char* ToName(const GamePlatform platform)
    {
        switch (platform)
        {
        case GamePlatform::PC:
            return "pc";
        case GamePlatform::XBOX:
            return "xbox360";
        case GamePlatform::PS3:
            return "ps3";
        case GamePlatform::WIIU:
            return "wiiu";
        }

        assert(false);
        return "unknown";
    }
} // namespace game_platform
