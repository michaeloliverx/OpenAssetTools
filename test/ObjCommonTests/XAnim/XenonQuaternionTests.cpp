#include "XAnim/XenonQuaternion.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cmath>
#include <utility>

namespace xanim::xenon
{
    TEST_CASE("Xenon quaternion selectors preserve the omitted axis and its sign", "[xanim][xenon][packing]")
    {
        const auto axis = GENERATE(0u, 1u, 2u, 3u);
        const auto negative = GENERATE(false, true);
        const auto value = static_cast<int16_t>(negative ? -32767 : 32767);
        const auto rotation = (3u - axis) & 3u;
        const auto packed32 = (rotation << 29u) | (static_cast<uint32_t>(negative) << 31u);
        const auto packed48 = (static_cast<uint64_t>(rotation) << 45u) | (static_cast<uint64_t>(negative) << 47u);
        const std::array<int16_t, 3> words{static_cast<int16_t>(packed48 >> 32u), 0, 0};
        const auto decoded32 = UnpackQuat32(packed32);
        const auto decoded48 = UnpackQuat48(words.data());
        CommonXQuat source;
        source.value[axis] = value;
        CHECK(PackQuat48(source) == words);
        CHECK(PackQuat32(source) == packed32);
        for (auto component = 0u; component < 4u; ++component)
        {
            CHECK(decoded32.value[component] == (component == axis ? value : 0));
            CHECK(decoded48.value[component] == (component == axis ? value : 0));
        }
        if (axis >= 2u)
        {
            const auto packed16 = static_cast<uint16_t>((axis == 2u ? 0x4000u : 0u) | (negative ? 0x8000u : 0u));
            const auto decoded16 = UnpackQuat16(packed16);
            CHECK(PackQuat16(CommonXQuat2(source.value[2], source.value[3])) == packed16);
            CHECK(decoded16.value[0] == (axis == 2u ? value : 0));
            CHECK(decoded16.value[1] == (axis == 3u ? value : 0));
        }
    }

    TEST_CASE("Compact Xenon quaternions retain rotations with nonzero ratios", "[xanim][xenon][packing]")
    {
        const auto axis = GENERATE(0u, 1u, 2u, 3u);
        const auto sign = GENERATE(-1, 1);
        CommonXQuat source(8192, -16384, 24576, 20000);
        std::swap(source.value[axis], source.value[2]);
        for (auto& value : source.value)
            value = static_cast<int16_t>(value * sign);
        const auto decoded = UnpackQuat32(PackQuat32(source));
        double dot = 0, sourceLength = 0, decodedLength = 0;
        for (auto i = 0u; i < 4u; ++i)
        {
            dot += static_cast<double>(source.value[i]) * decoded.value[i];
            sourceLength += static_cast<double>(source.value[i]) * source.value[i];
            decodedLength += static_cast<double>(decoded.value[i]) * decoded.value[i];
        }
        CHECK(std::abs(dot) / std::sqrt(sourceLength * decodedLength) > 0.9999904807);

        const CommonXQuat2 source2(static_cast<int16_t>(16384 * sign), 28377);
        const auto decoded2 = UnpackQuat16(PackQuat16(source2));
        CHECK(std::abs(decoded2.value[0] - source2.value[0]) < 3);
        CHECK(std::abs(decoded2.value[1] - source2.value[1]) < 3);
    }
} // namespace xanim::xenon
