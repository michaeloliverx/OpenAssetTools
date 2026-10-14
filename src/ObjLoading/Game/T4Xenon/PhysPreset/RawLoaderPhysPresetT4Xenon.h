#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace phys_preset
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetPhysPreset>> CreateRawLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace phys_preset
