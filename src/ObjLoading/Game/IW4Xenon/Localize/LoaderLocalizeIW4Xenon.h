#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetLocalize>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
