#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetMapEnts>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
