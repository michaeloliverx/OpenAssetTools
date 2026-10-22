#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"
#include "Zone/Zone.h"

#include <memory>

namespace localize
{
    std::unique_ptr<AssetCreator<IW3Xenon270::AssetLocalize>> CreateLoaderIW3Xenon270(MemoryManager& memory, ISearchPath& searchPath, Zone& zone);
} // namespace localize
