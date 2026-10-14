#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetLocalize>> CreateLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
