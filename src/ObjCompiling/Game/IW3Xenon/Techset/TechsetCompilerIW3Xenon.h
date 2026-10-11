#pragma once

#include "Asset/IAssetCreator.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace techset
{
    std::unique_ptr<IAssetCreator> CreateTechsetCompilerIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
    std::unique_ptr<ISubAssetCreator> CreateTechniqueCompilerIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace techset
