#include "Game/GameTarget.h"

#include <catch2/catch_test_macros.hpp>

namespace test::game::game_target
{
    TEST_CASE("Game target display names hide internal game variants", "[game][gametarget]")
    {
        REQUIRE(::game_target::GetDisplayName(GameId::IW3Xenon, GamePlatform::XBOX) == "IW3, xbox360");
        REQUIRE(::game_target::GetDisplayName(GameId::IW3, GamePlatform::PC) == "IW3");
    }
} // namespace test::game::game_target
