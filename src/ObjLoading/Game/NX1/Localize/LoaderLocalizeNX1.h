#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<NX1::AssetLocalize>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
