#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<QOSXenon::AssetStringTable>> CreateLoaderQOSXenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
