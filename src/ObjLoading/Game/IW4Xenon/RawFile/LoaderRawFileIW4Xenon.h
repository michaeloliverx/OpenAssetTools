#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace raw_file
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetRawFile>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace raw_file
