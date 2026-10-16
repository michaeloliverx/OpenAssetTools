#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace phys_preset
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetPhysPreset>> CreateRawLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace phys_preset
