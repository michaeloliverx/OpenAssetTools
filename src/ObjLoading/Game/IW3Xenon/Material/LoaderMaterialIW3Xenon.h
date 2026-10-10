#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Gdt/IGdtQueryable.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

namespace material
{
    std::unique_ptr<AssetCreator<IW3Xenon::AssetMaterial>> CreateLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath);
} // namespace material
