#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/NX1/NX1.h"
#include "SearchPath/IOutputPath.h"

namespace image
{
    std::unique_ptr<AssetCreator<NX1::AssetImage>>
        CreateLoaderNX1(MemoryManager& memory, ISearchPath& searchPath, Zone& zone, IOutputPath& outDir, unsigned imageFileIndex);
} // namespace image
