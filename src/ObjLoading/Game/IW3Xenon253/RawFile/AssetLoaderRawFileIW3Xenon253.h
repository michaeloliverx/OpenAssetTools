#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<IW3Xenon253::AssetRawFile>> CreateLoaderIW3Xenon253(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
