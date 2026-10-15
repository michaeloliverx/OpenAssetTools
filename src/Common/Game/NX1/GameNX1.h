#pragma once

#include "Game/IGame.h"

namespace NX1
{
    class Game final : public AbstractGame
    {
    public:
        Game();

        [[nodiscard]] GameId GetId() const override;
        [[nodiscard]] const std::string& GetFullName() const override;
        [[nodiscard]] const std::string& GetShortName() const override;
    };
} // namespace NX1
