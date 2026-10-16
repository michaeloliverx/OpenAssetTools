#pragma once

#include "IW4Xenon.h"
#include "Utils/Djb2.h"

#include <cctype>

namespace IW4Xenon
{
    class Common
    {
    public:
        static constexpr uint32_t R_HashString(const char* str, const uint32_t hash = 0u)
        {
            return djb2_xor_nocase(str, hash);
        }

        static constexpr unsigned GetPcMaterialTechniqueIndex(const unsigned techniqueIndex)
        {
            return techniqueIndex < TECHNIQUE_LIGHT_SPOT ? techniqueIndex : techniqueIndex + 14u;
        }

        static constexpr int StringTable_HashString(const char* str)
        {
            if (!str)
                return 0;

            // Lets do djb2 with 31 instead of 33 because why not
            // and leave out the starting value while we are at it
            uint32_t hash = 0;
            for (char c = *str; c; c = *++str)
                hash = hash * 31 + std::tolower(c);

            return static_cast<int>(hash);
        }
    };
} // namespace IW4Xenon
