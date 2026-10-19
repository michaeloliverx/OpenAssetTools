#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace image
{
    std::unique_ptr<AssetCreator<IW2Xenon::AssetImage>> CreateLoaderIW2Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace image
