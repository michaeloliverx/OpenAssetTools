#pragma once

#include "Game/IW3/Material/MaterialConstantZoneStateIW3.h"

namespace IW3Xenon
{
    // IW3 shares constant names and hashes across platforms. Unknown names remain
    // represented by their hash and name fragment in material JSON.
    using IW3::MaterialConstantZoneState;
} // namespace IW3Xenon
