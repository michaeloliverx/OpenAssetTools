#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<IW2Xenon::AssetRawFile>> CreateLoaderIW2Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
