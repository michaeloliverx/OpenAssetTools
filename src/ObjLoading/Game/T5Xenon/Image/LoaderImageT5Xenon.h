#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace image
{
    std::unique_ptr<AssetCreator<T5Xenon::AssetImage>> CreateLoaderT5Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace image
