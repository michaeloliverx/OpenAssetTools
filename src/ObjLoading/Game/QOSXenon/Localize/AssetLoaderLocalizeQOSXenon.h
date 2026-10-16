#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<QOSXenon::AssetLocalize>> CreateLoaderQOSXenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
