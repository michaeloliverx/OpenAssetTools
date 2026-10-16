#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace image
{
    std::unique_ptr<AssetCreator<QOSXenon::AssetImage>> CreateLoaderQOSXenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace image
