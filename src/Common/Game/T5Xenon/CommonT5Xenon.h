#pragma once
#include "T5Xenon.h"
#include "Utils/Djb2.h"

namespace T5Xenon
{
    class Common
    {
    public:
        static constexpr int Com_HashString(const char* str)
        {
            return str ? static_cast<int>(djb2_lower(str)) : 0;
        }

        static constexpr uint32_t R_HashString(const char* str)
        {
            return djb2_xor_nocase(str, 0u);
        }
    };
} // namespace T5Xenon
