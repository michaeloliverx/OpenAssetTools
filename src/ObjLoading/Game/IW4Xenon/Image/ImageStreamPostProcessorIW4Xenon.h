#pragma once

#include "Asset/IAssetPostProcessor.h"

namespace image
{
    std::unique_ptr<IAssetPostProcessor> CreateStreamPostProcessorIW4Xenon(Zone& zone, ISearchPath& searchPath);
} // namespace image
