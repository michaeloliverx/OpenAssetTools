#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<IW3Xenon253::AssetMapEnts>> CreateLoaderIW3Xenon253(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
