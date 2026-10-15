#pragma once

#include "Asset/IAssetPostProcessor.h"

namespace image
{
    std::unique_ptr<IAssetPostProcessor> CreateStreamPostProcessorNX1(Zone& zone, ISearchPath& searchPath);
} // namespace image
