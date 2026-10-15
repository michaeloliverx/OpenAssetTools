#pragma once
#include "Game/NX1/NX1.h"
#include "Zone/ZoneTypes.h"

#include <cstdint>
#include <string>

namespace NX1
{
    class ZoneConstants final
    {
        ZoneConstants() = default;

    public:
        static constexpr const char* MAGIC_SIGNED = "NXff0100";
        static constexpr const char* MAGIC_UNSIGNED = "NXffu100";
        static constexpr int ZONE_VERSION = 357;

        static_assert(std::char_traits<char>::length(MAGIC_UNSIGNED) == sizeof(ZoneHeader::m_magic));

        static constexpr const char* MAGIC_AUTH_HEADER = "NXffs100";
        inline static const std::uint8_t RSA_PUBLIC_KEY_NX1_XENON[270]{
            48u,  130u, 1u,   10u,  2u,   130u, 1u,   1u,   0u,   165u, 134u, 204u, 24u,  169u, 18u,  23u,  79u,  58u,  201u, 12u,  210u, 56u,  93u,
            219u, 103u, 98u,  164u, 227u, 212u, 66u,  5u,   138u, 87u,  12u,  49u,  78u,  25u,  228u, 186u, 137u, 115u, 19u,  219u, 114u, 37u,  99u,
            177u, 47u,  215u, 241u, 8u,   72u,  52u,  6u,   215u, 132u, 95u,  200u, 207u, 47u,  182u, 163u, 90u,  143u, 126u, 170u, 157u, 81u,  231u,
            15u,  185u, 7u,   183u, 48u,  145u, 4u,   57u,  156u, 220u, 28u,  241u, 22u,  203u, 150u, 16u,  235u, 56u,  177u, 59u,  186u, 66u,  227u,
            226u, 120u, 189u, 119u, 130u, 26u,  43u,  84u,  39u,  10u,  247u, 102u, 6u,   173u, 70u,  57u,  192u, 235u, 181u, 194u, 39u,  221u, 44u,
            8u,   98u,  44u,  11u,  196u, 0u,   13u,  203u, 173u, 34u,  103u, 1u,   166u, 146u, 156u, 0u,   175u, 157u, 85u,  204u, 104u, 236u, 57u,
            73u,  133u, 126u, 44u,  152u, 207u, 76u,  18u,  141u, 237u, 199u, 29u,  33u,  84u,  156u, 47u,  201u, 84u,  54u,  8u,   169u, 103u, 238u,
            145u, 230u, 217u, 177u, 250u, 169u, 43u,  136u, 173u, 42u,  208u, 170u, 40u,  249u, 71u,  166u, 15u,  207u, 85u,  76u,  155u, 38u,  65u,
            137u, 118u, 17u,  253u, 27u,  131u, 228u, 232u, 142u, 126u, 180u, 3u,   163u, 41u,  221u, 79u,  172u, 153u, 190u, 124u, 211u, 253u, 20u,
            40u,  28u,  89u,  105u, 224u, 121u, 95u,  75u,  218u, 107u, 171u, 72u,  78u,  40u,  57u,  132u, 198u, 43u,  198u, 32u,  5u,   219u, 5u,
            33u,  195u, 225u, 212u, 32u,  40u,  221u, 58u,  77u,  81u,  231u, 73u,  138u, 73u,  239u, 245u, 218u, 218u, 125u, 93u,  168u, 11u,  161u,
            119u, 205u, 98u,  125u, 157u, 64u,  38u,  68u,  75u,  59u,  10u,  137u, 2u,   3u,   1u,   0u,   1u,
        };

        static constexpr size_t AUTHED_CHUNK_SIZE = 0x2000;
        static constexpr unsigned AUTHED_CHUNK_COUNT_PER_GROUP = 256;

        static constexpr unsigned OFFSET_BLOCK_BIT_COUNT = 4u;
        static constexpr block_t INSERT_BLOCK = XFILE_BLOCK_VIRTUAL;
    };
} // namespace NX1
