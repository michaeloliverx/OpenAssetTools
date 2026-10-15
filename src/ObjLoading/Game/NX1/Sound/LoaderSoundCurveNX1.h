#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace sound_curve
{
    std::unique_ptr<AssetCreator<NX1::AssetSndVfCurve>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath);
} // namespace sound_curve
