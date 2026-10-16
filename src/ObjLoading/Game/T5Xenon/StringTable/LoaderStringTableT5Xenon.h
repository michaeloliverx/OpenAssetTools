#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<T5Xenon::AssetStringTable>> CreateLoaderT5Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
