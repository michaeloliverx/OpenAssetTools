#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetStringTable>> CreateLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
