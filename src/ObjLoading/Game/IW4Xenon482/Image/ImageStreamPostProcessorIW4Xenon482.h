#pragma once

#include "Asset/IAssetPostProcessor.h"

namespace image
{
    std::unique_ptr<IAssetPostProcessor> CreateStreamPostProcessorIW4Xenon482(Zone& zone, ISearchPath& searchPath);
} // namespace image
