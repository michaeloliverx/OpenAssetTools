#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<NX1::AssetRawFile>> CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
