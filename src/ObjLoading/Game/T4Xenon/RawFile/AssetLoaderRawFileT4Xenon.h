#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetRawFile>> CreateLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
