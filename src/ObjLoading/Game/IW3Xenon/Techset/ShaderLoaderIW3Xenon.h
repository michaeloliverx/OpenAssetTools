#pragma once

#include "Asset/IAssetCreator.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace techset
{
    std::unique_ptr<IAssetCreator> CreatePixelShaderLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
    std::unique_ptr<ISubAssetCreator> CreateVertexShaderLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace techset
