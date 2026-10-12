#include "ImaAdpcmDecoder.h"

#include <algorithm>
#include <array>

namespace
{
    constexpr int STEP_TABLE[]{
        7,    8,    9,    10,   11,   12,   13,   14,    16,    17,    19,    21,    23,    25,    28,    31,    34,    37,    41,    45,    50,   55,   60,
        66,   73,   80,   88,   97,   107,  118,  130,   143,   157,   173,   190,   209,   230,   253,   279,   307,   337,   371,   408,   449,  494,  544,
        598,  658,  724,  796,  876,  963,  1060, 1166,  1282,  1411,  1552,  1707,  1878,  2066,  2272,  2499,  2749,  3024,  3327,  3660,  4026, 4428, 4871,
        5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
    };
    constexpr int INDEX_TABLE[]{-1, -1, -1, -1, 2, 4, 6, 8};
    constexpr auto CHANNEL_HEADER_SIZE = 4uz;
    constexpr auto CHANNEL_GROUP_SIZE = 4uz;
    constexpr auto FRAMES_PER_GROUP = 8uz;

    struct ChannelState
    {
        int predictor;
        int index;
    };

    int16_t DecodeSample(ChannelState& state, const unsigned nibble)
    {
        const auto step = STEP_TABLE[state.index];
        auto delta = step >> 3;
        if (nibble & 4u)
            delta += step;
        if (nibble & 2u)
            delta += step >> 1;
        if (nibble & 1u)
            delta += step >> 2;

        state.predictor = std::clamp(state.predictor + ((nibble & 8u) ? -delta : delta), -32768, 32767);
        state.index = std::clamp(state.index + INDEX_TABLE[nibble & 7u], 0, 88);
        return static_cast<int16_t>(state.predictor);
    }
} // namespace

namespace ima_adpcm
{
    bool Decode(
        const std::span<const uint8_t> data, const unsigned channelCount, const unsigned blockSize, const size_t frameCount, std::vector<int16_t>& samples)
    {
        samples.clear();
        if (channelCount != 1 && channelCount != 2)
            return false;

        const auto headerSize = CHANNEL_HEADER_SIZE * channelCount;
        const auto groupSize = CHANNEL_GROUP_SIZE * channelCount;
        if (blockSize < headerSize || (blockSize - headerSize) % groupSize != 0 || data.size() % blockSize != 0)
            return false;

        const auto framesPerBlock = 1uz + (blockSize - headerSize) / groupSize * FRAMES_PER_GROUP;
        const auto availableFrames = static_cast<uint64_t>(data.size() / blockSize) * framesPerBlock;
        if (frameCount > availableFrames || frameCount > samples.max_size() / channelCount)
            return false;

        samples.reserve(frameCount * channelCount);
        std::array<ChannelState, 2> states{};
        auto remainingFrames = frameCount;
        for (auto blockOffset = 0uz; remainingFrames > 0; blockOffset += blockSize)
        {
            const auto block = data.subspan(blockOffset, blockSize);
            for (auto channel = 0u; channel < channelCount; channel++)
            {
                const auto offset = channel * CHANNEL_HEADER_SIZE;
                const auto predictor = static_cast<int16_t>(block[offset] | (block[offset + 1] << 8));
                const auto index = block[offset + 2];
                if (index >= std::size(STEP_TABLE))
                {
                    samples.clear();
                    return false;
                }

                states[channel] = {.predictor = predictor, .index = index};
                samples.push_back(predictor);
            }
            remainingFrames--;

            // Each channel stores eight samples in four bytes, with the low nibble first.
            for (auto groupOffset = headerSize; groupOffset < blockSize && remainingFrames > 0; groupOffset += groupSize)
            {
                const auto groupFrames = std::min(remainingFrames, FRAMES_PER_GROUP);
                for (auto frame = 0uz; frame < groupFrames; frame++)
                {
                    for (auto channel = 0u; channel < channelCount; channel++)
                    {
                        const auto byte = block[groupOffset + channel * CHANNEL_GROUP_SIZE + frame / 2];
                        const auto nibble = (byte >> ((frame & 1u) * 4u)) & 0xfu;
                        samples.push_back(DecodeSample(states[channel], nibble));
                    }
                }
                remainingFrames -= groupFrames;
            }
        }

        return true;
    }
} // namespace ima_adpcm
