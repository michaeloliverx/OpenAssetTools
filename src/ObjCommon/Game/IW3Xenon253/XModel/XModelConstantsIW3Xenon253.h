#pragma once

#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace IW3Xenon253
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
    };
    // Blend indices address 64-byte bone matrices.
    inline constexpr unsigned BONE_MATRIX_SIZE = 64u;
} // namespace IW3Xenon253
