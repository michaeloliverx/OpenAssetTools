#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace string_table
{
    std::unique_ptr<AssetCreator<IW3Xenon270::AssetStringTable>> CreateLoaderIW3Xenon270(MemoryManager& memory, ISearchPath& searchPath);
} // namespace string_table
