#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace sound_curve
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetSoundCurve>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace sound_curve
