#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<IW4Xenon482::AssetStringTable>> CreateLoaderIW4Xenon482(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
