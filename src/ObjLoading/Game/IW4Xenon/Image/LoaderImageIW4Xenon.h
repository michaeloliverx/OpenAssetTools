#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "SearchPath/IOutputPath.h"

namespace image
{
    std::unique_ptr<AssetCreator<IW4Xenon::AssetImage>>
        CreateLoaderIW4Xenon(MemoryManager& memory, ISearchPath& searchPath, Zone& zone, IOutputPath& outDir, unsigned imageFileIndex);
} // namespace image
