#pragma once

#include "Loading/ILoadingStep.h"
#include "Zone/XAssetStreamFile.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace step
{
    using ZoneImageHeaderCallback = std::function<void(uint32_t languageFlags,
                                                       uint32_t languageCount,
                                                       uint32_t imageCount,
                                                       const std::vector<XAssetStreamFile>& imageStreamFiles,
                                                       uint32_t xfileSize,
                                                       uint32_t xfileLoadSize)>;

    std::unique_ptr<ILoadingStep> CreateStepSkipZoneImageHeaders(ZoneImageHeaderCallback callback = {});
} // namespace step
