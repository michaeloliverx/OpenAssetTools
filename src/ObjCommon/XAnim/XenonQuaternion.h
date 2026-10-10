#pragma once

#include "XAnimCommon.h"

#include <array>
#include <cstdint>

namespace xanim::xenon
{
    [[nodiscard]] CommonXQuat2 UnpackQuat16(uint16_t packed);
    [[nodiscard]] CommonXQuat UnpackQuat32(uint32_t packed);
    [[nodiscard]] CommonXQuat UnpackQuat48(const int16_t* words);
    [[nodiscard]] std::array<int16_t, 3> PackQuat48(const CommonXQuat& quat);
} // namespace xanim::xenon
