#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace phys_preset
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetPhysPreset>> CreateRawLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace phys_preset
