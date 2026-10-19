#pragma once
#include "Game/IW2Xenon/IW2Xenon.h"

namespace IW2Xenon
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr uint32_t ZONE_VERSION = 104u;
        static constexpr size_t FILE_HEADER_SIZE = 20u;
        static constexpr size_t STREAM_BUFFER_SIZE = 0x20000u;
        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 2u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;
    };
} // namespace IW2Xenon
