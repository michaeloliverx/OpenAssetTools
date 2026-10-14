#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetMapEnts>> CreateLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
