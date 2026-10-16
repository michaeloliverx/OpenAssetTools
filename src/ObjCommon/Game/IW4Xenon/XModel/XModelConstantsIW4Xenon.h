#pragma once

#include "Game/IW4Xenon/IW4Xenon.h"

namespace IW4Xenon
{
    inline const char* HITLOC_NAMES[]{
        "none",
        "helmet",
        "head",
        "neck",
        "torso_upper",
        "torso_lower",
        "right_arm_upper",
        "left_arm_upper",
        "right_arm_lower",
        "left_arm_lower",
        "right_hand",
        "left_hand",
        "right_leg_upper",
        "left_leg_upper",
        "right_leg_lower",
        "left_leg_lower",
        "right_foot",
        "left_foot",
        "gun",
        "shield",
    };
    static_assert(std::extent_v<decltype(HITLOC_NAMES)> == HITLOC_COUNT);
} // namespace IW4Xenon
