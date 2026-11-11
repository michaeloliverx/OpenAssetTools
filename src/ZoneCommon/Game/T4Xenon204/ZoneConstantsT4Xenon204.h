#pragma once
#include "Game/T4Xenon204/T4Xenon204.h"

namespace T4Xenon204
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr uint32_t ZONE_VERSION = 204u;
        static constexpr size_t FILE_HEADER_SIZE = 28u;
        static constexpr size_t STREAM_BUFFER_SIZE = 0x20000u;
        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 2u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;
    };
} // namespace T4Xenon204
