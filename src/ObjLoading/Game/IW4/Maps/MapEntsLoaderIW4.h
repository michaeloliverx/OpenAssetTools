#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4/IW4.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<IW4::AssetMapEnts>> CreateLoaderIW4(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
