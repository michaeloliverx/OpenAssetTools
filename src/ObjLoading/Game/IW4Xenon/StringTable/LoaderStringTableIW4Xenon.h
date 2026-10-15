#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetStringTable>> CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
