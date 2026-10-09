#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetLocalize>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
