#pragma once

#include "Game/T5Xenon/T5Xenon.h"
#include "Zone/ZoneTypes.h"

#include <cstdint>
#include <string>

namespace T5Xenon
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr const char* MAGIC_SIGNED = "IWff0100";
        static constexpr const char* MAGIC_UNSIGNED = "IWffu100";
        static constexpr const char* MAGIC_AUTH_HEADER = "PHEEBs71";
        static constexpr int ZONE_VERSION = 473;

        static constexpr unsigned STREAM_COUNT = 4;
        // 200 hash entries shared across four streams.
        static constexpr unsigned SALSA20_HASHES_PER_STREAM = 50;
        static constexpr unsigned XCHUNK_SIZE = 0xC000;
        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 3u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;

        inline static const uint8_t SALSA20_KEY_TREYARCH_XENON[]{
            0x1A, 0xC1, 0xD1, 0x2D, 0x52, 0x7C, 0x59, 0xB4, 0x0E, 0xCA, 0x61, 0x91, 0x20, 0xFF, 0x82, 0x17,
            0xCC, 0xFF, 0x09, 0xCD, 0x16, 0x89, 0x6F, 0x81, 0xB8, 0x29, 0xC7, 0xF5, 0x27, 0x93, 0x40, 0x5D,
        };

        static_assert(std::char_traits<char>::length(MAGIC_SIGNED) == sizeof(ZoneHeader::m_magic));
        static_assert(std::char_traits<char>::length(MAGIC_UNSIGNED) == sizeof(ZoneHeader::m_magic));
    };
} // namespace T5Xenon
