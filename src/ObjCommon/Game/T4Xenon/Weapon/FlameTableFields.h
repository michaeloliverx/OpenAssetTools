#pragma once

#include "Game/T4/Weapon/FlameTableFields.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace T4Xenon
{
    static_assert(sizeof(FlameTable) == sizeof(T4::FlameTable));
    static_assert(alignof(FlameTable) == alignof(T4::FlameTable));

    using T4::flameTableFields;
} // namespace T4Xenon
