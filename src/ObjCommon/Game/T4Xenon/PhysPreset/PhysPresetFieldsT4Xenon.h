#pragma once

#include "Game/T4/T4.h"
#include "Game/T4Xenon/T4Xenon.h"

namespace T4Xenon
{
    inline T4::cspField_t phys_preset_fields[]{
        {"mass",                  offsetof(PhysPresetInfo, mass),                  T4::CSPFT_FLOAT },
        {"bounce",                offsetof(PhysPresetInfo, bounce),                T4::CSPFT_FLOAT },
        {"friction",              offsetof(PhysPresetInfo, friction),              T4::CSPFT_FLOAT },
        {"isFrictionInfinity",    offsetof(PhysPresetInfo, isFrictionInfinity),    T4::CSPFT_BOOL  },
        {"bulletForceScale",      offsetof(PhysPresetInfo, bulletForceScale),      T4::CSPFT_FLOAT },
        {"explosiveForceScale",   offsetof(PhysPresetInfo, explosiveForceScale),   T4::CSPFT_FLOAT },
        {"sndAliasPrefix",        offsetof(PhysPresetInfo, sndAliasPrefix),        T4::CSPFT_STRING},
        {"piecesSpreadFraction",  offsetof(PhysPresetInfo, piecesSpreadFraction),  T4::CSPFT_FLOAT },
        {"piecesUpwardVelocity",  offsetof(PhysPresetInfo, piecesUpwardVelocity),  T4::CSPFT_FLOAT },
        {"canFloat",              offsetof(PhysPresetInfo, canFloat),              T4::CSPFT_BOOL  },
        {"tempDefaultToCylinder", offsetof(PhysPresetInfo, tempDefaultToCylinder), T4::CSPFT_BOOL  },
    };
}
