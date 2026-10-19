#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW2Xenon::AssetLocalize>> CreateLoaderIW2Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
