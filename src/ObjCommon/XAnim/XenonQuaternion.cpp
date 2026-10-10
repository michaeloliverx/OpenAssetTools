#include "XenonQuaternion.h"

#include <algorithm>
#include <cmath>

using namespace xanim;

namespace
{
    int16_t QuantizeComponent(const float value)
    {
        return static_cast<int16_t>(std::lround(std::clamp(value, -1.0f, 1.0f) * 32767.0f));
    }

    float UnpackRatio(const uint64_t packed, const unsigned bits)
    {
        const auto signBit = 1u << (bits - 1u);
        const auto mask = (1u << bits) - 1u;
        const auto signedValue = static_cast<int>((static_cast<unsigned>(packed) & mask) ^ signBit) - static_cast<int>(signBit);
        return std::clamp(static_cast<float>(signedValue) / static_cast<float>(signBit - 1u), -1.0f, 1.0f);
    }

    CommonXQuat ReconstructQuat(const std::array<float, 3>& ratios, const unsigned rotation, const bool negative)
    {
        // Xenon stores ratios to the largest component, plus its index and sign.
        const auto droppedIndex = (3u - rotation) & 3u;
        const auto dropped = (negative ? -1.0f : 1.0f) / std::sqrt(1.0f + ratios[0] * ratios[0] + ratios[1] * ratios[1] + ratios[2] * ratios[2]);
        CommonXQuat result;
        result.value[droppedIndex] = QuantizeComponent(dropped);
        for (auto i = 0u; i < ratios.size(); ++i)
            result.value[(droppedIndex + i + 1u) & 3u] = QuantizeComponent(ratios[i] * dropped);
        return result;
    }

    uint64_t PackRatio(const float value)
    {
        const auto quantized = std::clamp(static_cast<int>(std::lround(std::clamp(value, -1.0f, 1.0f) * 16383.0f)), -16384, 16383);
        return static_cast<uint16_t>(quantized) & 0x7fffu;
    }
} // namespace

namespace xanim::xenon
{
    CommonXQuat2 UnpackQuat16(const uint16_t packed)
    {
        const auto ratio = UnpackRatio(packed, 14u);
        const auto dropped = (packed & 0x8000u ? -1.0f : 1.0f) / std::sqrt(1.0f + ratio * ratio);
        const auto other = QuantizeComponent(ratio * dropped);
        const auto largest = QuantizeComponent(dropped);
        return packed & 0x4000u ? CommonXQuat2(largest, other) : CommonXQuat2(other, largest);
    }

    CommonXQuat UnpackQuat32(const uint32_t packed)
    {
        return ReconstructQuat(
            {UnpackRatio(packed, 9u), UnpackRatio(packed >> 9u, 10u), UnpackRatio(packed >> 19u, 10u)}, (packed >> 29u) & 3u, (packed & 0x80000000u) != 0u);
    }

    CommonXQuat UnpackQuat48(const int16_t* words)
    {
        const auto packed = (static_cast<uint64_t>(static_cast<uint16_t>(words[0])) << 32u) | (static_cast<uint64_t>(static_cast<uint16_t>(words[1])) << 16u)
                            | static_cast<uint16_t>(words[2]);
        return ReconstructQuat({UnpackRatio(packed, 15u), UnpackRatio(packed >> 15u, 15u), UnpackRatio(packed >> 30u, 15u)},
                               static_cast<unsigned>((packed >> 45u) & 3u),
                               (packed & (1ull << 47u)) != 0u);
    }

    std::array<int16_t, 3> PackQuat48(const CommonXQuat& quat)
    {
        auto droppedIndex = 0u;
        for (auto i = 1u; i < 4u; ++i)
            if (std::abs(static_cast<int>(quat.value[i])) > std::abs(static_cast<int>(quat.value[droppedIndex])))
                droppedIndex = i;
        const auto dropped = static_cast<float>(quat.value[droppedIndex]);
        if (dropped == 0.0f)
            return {};

        const auto packed = PackRatio(static_cast<float>(quat.value[(droppedIndex + 1u) & 3u]) / dropped)
                            | (PackRatio(static_cast<float>(quat.value[(droppedIndex + 2u) & 3u]) / dropped) << 15u)
                            | (PackRatio(static_cast<float>(quat.value[(droppedIndex + 3u) & 3u]) / dropped) << 30u)
                            | (static_cast<uint64_t>((3u - droppedIndex) & 3u) << 45u) | (static_cast<uint64_t>(dropped < 0.0f) << 47u);
        return {static_cast<int16_t>(packed >> 32u), static_cast<int16_t>(packed >> 16u), static_cast<int16_t>(packed)};
    }
} // namespace xanim::xenon
