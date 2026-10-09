#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetStringTable>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
