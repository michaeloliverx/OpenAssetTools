#pragma once
#include "Game/IW3Xenon253/IW3Xenon253.h"

namespace IW3Xenon253
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr uint32_t ZONE_VERSION = 358u;
        static constexpr size_t FILE_HEADER_SIZE = 28u;
        static constexpr size_t STREAM_BUFFER_SIZE = 0x20000u;
        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 3u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;
    };
} // namespace IW3Xenon253
