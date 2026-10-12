#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace ima_adpcm
{
    bool Decode(std::span<const uint8_t> data, unsigned channelCount, unsigned blockSize, size_t frameCount, std::vector<int16_t>& samples);
} // namespace ima_adpcm
