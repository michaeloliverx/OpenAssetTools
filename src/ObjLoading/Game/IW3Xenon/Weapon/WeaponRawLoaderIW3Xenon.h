#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace weapon
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetWeapon>> CreateRawLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace weapon
