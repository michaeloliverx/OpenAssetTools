#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<IW3Xenon328::AssetMapEnts>> CreateLoaderIW3Xenon328(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
