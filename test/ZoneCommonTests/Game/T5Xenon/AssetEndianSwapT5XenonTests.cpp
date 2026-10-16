#include "Game/T5Xenon/AssetEndianSwapT5Xenon.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <string>

TEST_CASE("T5 Xenon expression unions round trip with their host-order discriminator", "[t5xenon][endian]")
{
    T5Xenon::Operand operand{};
    operand.dataType = GENERATE(T5Xenon::VAL_INT, T5Xenon::VAL_FLOAT, T5Xenon::VAL_STRING);
    operand.internals.intVal = 0x12345678;
    const auto original = operand;
    T5Xenon::EndianSwap(operand, T5Xenon::EndianOperation::Encode);
    T5Xenon::EndianSwap(operand, T5Xenon::EndianOperation::Decode);
    REQUIRE(operand.dataType == original.dataType);
    REQUIRE(operand.internals.intVal == original.internals.intVal);
}

TEST_CASE("T5 Xenon packed image layout matches the native loader", "[t5xenon][endian]")
{
#ifdef ARCH_x86
    REQUIRE(sizeof(T5Xenon::GfxImage) == 156);
    REQUIRE(offsetof(T5Xenon::GfxImage, delayLoadPixels) == 55);
    REQUIRE(offsetof(T5Xenon::GfxImage, cardMemory) == 56);
    REQUIRE(offsetof(T5Xenon::GfxImage, pixels) == 72);
    REQUIRE(offsetof(T5Xenon::GfxImage, name) == 148);
#endif
}

TEST_CASE("T5 Xenon named constants decode retail material data", "[t5xenon][endian][material]")
{
    // colorTint = (1, 1, 1, 1) from code_post_gfx.ff.
    constexpr std::array<unsigned char, 32> bytes{0xb6, 0x0c, 0x3b, 0x3a, 'c',  'o',  'l', 'o', 'r',  'T',  'i', 'n', 't',  0,    0, 0,
                                                  0x3f, 0x80, 0,    0,    0x3f, 0x80, 0,   0,   0x3f, 0x80, 0,   0,   0x3f, 0x80, 0, 0};
    T5Xenon::MaterialConstantDef constant;
    REQUIRE(sizeof(constant) == bytes.size());
    std::memcpy(&constant, bytes.data(), bytes.size());
    T5Xenon::EndianSwap(constant, T5Xenon::EndianOperation::Decode);
    REQUIRE(constant.nameHash == 0xb60c3b3au);
    REQUIRE(std::string(constant.name) == "colorTint");
    for (const auto value : constant.literal)
        REQUIRE(value == 1.0f);
    T5Xenon::EndianSwap(constant, T5Xenon::EndianOperation::Encode);
    REQUIRE(std::memcmp(&constant, bytes.data(), bytes.size()) == 0);
}

TEST_CASE("T5 Xenon image streaming retains the native temporary-image state", "[t5xenon][endian][image]")
{
    T5Xenon::GfxImage image{};
    image.streaming = 3;
    image.cardMemory.platform[0] = 0x1000;
    T5Xenon::EndianSwap(image, T5Xenon::EndianOperation::Encode);
    T5Xenon::EndianSwap(image, T5Xenon::EndianOperation::Decode);
    REQUIRE(image.streaming == 3);
    REQUIRE(image.cardMemory.platform[0] == 0x1000);
}
