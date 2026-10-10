#pragma once

#include "Game/IW3/Weapon/WeaponFields.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace IW3Xenon
{
    static_assert(sizeof(WeaponDef) == sizeof(IW3::WeaponDef));
    static_assert(alignof(WeaponDef) == alignof(IW3::WeaponDef));

    using IW3::weapon_fields;
} // namespace IW3Xenon
