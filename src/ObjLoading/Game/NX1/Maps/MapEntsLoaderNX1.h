#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<NX1::AssetMapEnts>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
