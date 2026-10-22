#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "SearchPath/IOutputPath.h"

namespace image
{
    std::unique_ptr<AssetCreator<IW4Xenon482::AssetImage>>
        CreateLoaderIW4Xenon482(MemoryManager& memory, ISearchPath& searchPath, Zone& zone, IOutputPath& outDir, unsigned imageFileIndex);
} // namespace image
