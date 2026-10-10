#include "XAnim/XenonQuaternion.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

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
        for (auto component = 0u; component < 4u; ++component)
        {
            CHECK(decoded32.value[component] == (component == axis ? value : 0));
            CHECK(decoded48.value[component] == (component == axis ? value : 0));
        }
        if (axis >= 2u)
        {
            const auto packed16 = static_cast<uint16_t>((axis == 2u ? 0x4000u : 0u) | (negative ? 0x8000u : 0u));
            const auto decoded16 = UnpackQuat16(packed16);
            CHECK(decoded16.value[0] == (axis == 2u ? value : 0));
            CHECK(decoded16.value[1] == (axis == 3u ? value : 0));
        }
    }
} // namespace xanim::xenon
