#pragma once

#include <array>
#include <cstdint>

struct XAssetStreamFile
{
    std::uint32_t fileIndex = 0u;
    std::uint32_t offset = 0u;
    std::uint32_t offsetEnd = 0u;
};

static_assert(sizeof(XAssetStreamFile) == 0xCu);

using XAssetStreamFilesForImage = std::array<XAssetStreamFile, 4>;
