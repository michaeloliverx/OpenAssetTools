#pragma once

#include "Game/T4/Weapon/WeaponFields.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace T4Xenon
{
    static_assert(sizeof(WeaponDef) == sizeof(T4::WeaponDef));
    static_assert(alignof(WeaponDef) == alignof(T4::WeaponDef));

    using T4::weapon_fields;
} // namespace T4Xenon
