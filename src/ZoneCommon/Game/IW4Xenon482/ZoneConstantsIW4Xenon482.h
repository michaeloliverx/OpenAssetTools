#pragma once
#include "Game/IW4/ZoneConstantsIW4.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Zone/ZoneTypes.h"

#include <cstdint>
#include <string>

namespace IW4Xenon482
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr const char* MAGIC_SIGNED = "IWff0100";
        static constexpr const char* MAGIC_UNSIGNED = "IWffu100";
        static constexpr int ZONE_VERSION = 253;

        static_assert(std::char_traits<char>::length(MAGIC_UNSIGNED) == sizeof(ZoneHeader::m_magic));

        static constexpr const char* MAGIC_AUTH_HEADER = "IWffs100";
        static constexpr const auto& RSA_PUBLIC_KEY_IW4_XENON = IW4::ZoneConstants::RSA_PUBLIC_KEY_INFINITY_WARD_XENON;

        static constexpr size_t AUTHED_CHUNK_SIZE = 0x2000;
        static constexpr unsigned AUTHED_CHUNK_COUNT_PER_GROUP = 256;

        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 4u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;
    };
} // namespace IW4Xenon482
