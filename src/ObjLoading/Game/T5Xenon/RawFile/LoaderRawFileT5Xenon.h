#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<T5Xenon::AssetRawFile>> CreateLoaderT5Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
