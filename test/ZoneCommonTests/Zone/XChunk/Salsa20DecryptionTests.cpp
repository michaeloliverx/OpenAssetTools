#include "Zone/XChunk/XChunkProcessorSalsa20Decryption.h"

#include <array>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Salsa20 chunk hash count preserves the default and supports shorter native rings", "[xchunk][salsa20]")
{
    std::array<uint8_t, 32> key{};
    for (auto i = 0u; i < key.size(); ++i)
        key[i] = static_cast<uint8_t>(i);

    XChunkProcessorSalsa20Decryption defaults(4u, "test", key.data(), key.size());
    const uint8_t* captured;
    size_t capturedSize;
    defaults.GetCapturedData(&captured, &capturedSize);
    REQUIRE(capturedSize == 4u * 200u * 20u);

    XChunkProcessorSalsa20Decryption shortRing(4u, "test", key.data(), key.size(), 1u);
    shortRing.GetCapturedData(&captured, &capturedSize);
    REQUIRE(capturedSize == 4u * 20u);

    // Independent Salsa20 vectors: the second chunk reuses the first hash entry after XORing its plaintext SHA1.
    constexpr std::array<uint8_t, 16> first{0x96, 0x1a, 0xae, 0x78, 0x7f, 0x00, 0xf4, 0x05, 0x7f, 0xdc, 0xe3, 0x01, 0x7b, 0x0c, 0x06, 0x77};
    constexpr std::array<uint8_t, 16> second{0x6c, 0x2c, 0x54, 0x5c, 0x71, 0xb3, 0x2e, 0x4b, 0x72, 0xf9, 0x8e, 0x91, 0x79, 0xa9, 0x87, 0xec};
    std::array<uint8_t, 16> expected;
    expected.fill(0x56);
    std::array<uint8_t, 16> output{};
    REQUIRE(shortRing.Process(0u, first.data(), first.size(), output.data(), output.size()) == output.size());
    REQUIRE(output == expected);
    REQUIRE(shortRing.Process(0u, second.data(), second.size(), output.data(), output.size()) == output.size());
    REQUIRE(output == expected);
}
