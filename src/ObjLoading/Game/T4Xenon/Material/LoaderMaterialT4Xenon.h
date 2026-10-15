#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "Gdt/IGdtQueryable.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

namespace material
{
    std::unique_ptr<AssetCreator<T4Xenon::AssetMaterial>> CreateLoaderT4Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace material
