#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<T5Xenon::AssetLocalize>> CreateLoaderT5Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
