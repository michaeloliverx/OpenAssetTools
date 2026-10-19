#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace sound_curve
{
    std::unique_ptr<AssetCreator<IW2Xenon::AssetSoundCurve>> CreateLoaderIW2Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace sound_curve
