#pragma once

#include "Game/IGame.h"
#include "Loading/ILoadingStep.h"

#include <memory>

namespace step
{
    std::unique_ptr<ILoadingStep> CreateStepAllocXBlocks(GameEndianness endianness = GameEndianness::LE);
}
