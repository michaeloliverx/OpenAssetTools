#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace image
{
    std::unique_ptr<AssetCreator<IW3Xenon290::AssetImage>> CreateLoaderIW3Xenon290(MemoryManager& memory, ISearchPath& searchPath);
} // namespace image
