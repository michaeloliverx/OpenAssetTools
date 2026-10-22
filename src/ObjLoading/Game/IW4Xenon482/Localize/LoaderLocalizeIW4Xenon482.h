#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW4Xenon482::AssetLocalize>> CreateLoaderIW4Xenon482(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
