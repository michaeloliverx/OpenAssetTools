#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<T5Xenon::AssetMapEnts>> CreateLoaderT5Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
