#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace menu
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetMenuList>> CreateMenuListLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
}
