#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace map_ents
{
    std::unique_ptr<AssetCreator<QOSXenon::AssetMapEnts>> CreateLoaderQOSXenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace map_ents
