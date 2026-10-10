#pragma once

#include "Texture.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace image::xenon
{
    struct XenonTextureEncodeResult
    {
        std::array<std::uint8_t, 52> textureHeader{};
        std::vector<std::uint8_t> pixels;
        std::uint32_t rawFormat = 0u;
        std::uint32_t levelCount = 0u;
        std::uint32_t baseSize = 0u;
    };

    bool EncodeTexture(const Texture& texture, XenonTextureEncodeResult& result, std::string* error = nullptr);
} // namespace image::xenon
