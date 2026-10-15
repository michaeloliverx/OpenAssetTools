#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace sound_curve
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetSoundCurve>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace sound_curve
