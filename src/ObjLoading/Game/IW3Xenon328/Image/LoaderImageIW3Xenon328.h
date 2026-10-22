#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace image
{
    std::unique_ptr<AssetCreator<IW3Xenon328::AssetImage>> CreateLoaderIW3Xenon328(MemoryManager& memory, ISearchPath& searchPath);
} // namespace image
