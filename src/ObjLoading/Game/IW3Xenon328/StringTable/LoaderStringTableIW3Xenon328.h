#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<IW3Xenon328::AssetStringTable>> CreateLoaderIW3Xenon328(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
