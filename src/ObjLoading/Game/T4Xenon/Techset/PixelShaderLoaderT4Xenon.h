#pragma once

#include "Asset/IAssetCreator.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace techset
{
    std::unique_ptr<IAssetCreator> CreatePixelShaderLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace techset
