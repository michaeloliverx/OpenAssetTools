#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW3Xenon328::AssetLocalize>> CreateLoaderIW3Xenon328(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
