#pragma once

#include "Game/IGame.h"
#include "Loading/ILoadingStep.h"

#include <memory>

namespace step
{
    class IStepLoadZoneSizes : public ILoadingStep
    {
    public:
        [[nodiscard]] virtual uint32_t GetSize() const = 0;
        [[nodiscard]] virtual uint32_t GetExternalSize() const = 0;
    };

    std::unique_ptr<IStepLoadZoneSizes> CreateStepLoadZoneSizes(GameEndianness endianness = GameEndianness::LE);
} // namespace step
