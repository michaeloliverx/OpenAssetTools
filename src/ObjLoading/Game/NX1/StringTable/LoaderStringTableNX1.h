#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<NX1::AssetStringTable>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
