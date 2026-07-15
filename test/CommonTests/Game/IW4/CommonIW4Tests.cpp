#include "Game/IW4/CommonIW4.h"
#include "Game/IW4/IW4.h"
#include "Utils/Endianness.h"

#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE("IW4: Check checksums", "[iw4]")
{
    SECTION("for StringTable_HashString")
    {
        const auto [str, expectedHash] = GENERATE(Catch::Generators::table<const char*, int>({
            {"hello world",                      0x6aefe2c4},
            {"universe2",                        0xe796fe8d},
            {"lit_r0c0n0x0_b1c1n1s1v1_b2c2n2x2", 0x16c4d3f1},
            {"AngularVelocityScale",             0x573e9feb},
            {"BakedLightingIntensity",           0x15af785c},
            {"Layer1OffsetBobbleDelay",          0xfb83324a},
            {"MaxDepth",                         0x1892e3df},
            {"MomentumColor",                    0x5e7abdcb},
            {"SparkleScale",                     0x9e4409f4},
            {"TickMarkColorAndHarshness",        0x5dfe4c47},
            {"worldViewProjectionMatrix",        0xa2501a07},
        }));

        CAPTURE(str);
        const auto hash = IW4::Common::StringTable_HashString(str);
        REQUIRE(hash == expectedHash);
    }

    SECTION("for R_HashString")
    {
        const auto [str, expectedHash] = GENERATE(Catch::Generators::table<const char*, uint32_t>({
            {"hello world",                      0xe7d74060},
            {"universe2",                        0x113fdcd7},
            {"lit_r0c0n0x0_b1c1n1s1v1_b2c2n2x2", 0xdd0153c5},
            {"AngularVelocityScale",             0x18f2cb6d},
            {"BakedLightingIntensity",           0xd627f218},
            {"Layer1OffsetBobbleDelay",          0xcd91b6ae},
            {"MaxDepth",                         0x61ed5959},
            {"MomentumColor",                    0xc80f3595},
            {"SparkleScale",                     0x5488816a},
            {"TickMarkColorAndHarshness",        0xd6c718bd},
            {"worldViewProjectionMatrix",        0x7f661409},
        }));

        CAPTURE(str);
        const auto hash = IW4::Common::R_HashString(str);
        REQUIRE(hash == expectedHash);
    }
}

TEST_CASE("IW4: Convert shared retail Xenon assets from big endian", "[iw4][endianness]")
{
    IW4::PhysPreset physPreset{};
    physPreset.type = 0x02000000;
    physPreset.mass = std::bit_cast<float>(0x0000803fu);
    physPreset.bounce = std::bit_cast<float>(0x0000003fu);

    endianness::FromBigEndianInPlace(physPreset);

    REQUIRE(physPreset.type == 2);
    REQUIRE(physPreset.mass == 1.0f);
    REQUIRE(physPreset.bounce == 0.5f);

    IW4::StringTableCell cells[1]{};
    cells[0].hash = 0x78563412;

    IW4::StringTable stringTable{};
    stringTable.columnCount = 0x02000000;
    stringTable.rowCount = 0x03000000;

    endianness::FromBigEndianInPlace(cells);
    endianness::FromBigEndianInPlace(stringTable);

    REQUIRE(cells[0].hash == 0x12345678);
    REQUIRE(stringTable.columnCount == 2);
    REQUIRE(stringTable.rowCount == 3);

    IW4::RawFile rawFile{};
    rawFile.compressedLen = 0x0c000000;
    rawFile.len = 0x2a000000;

    endianness::FromBigEndianInPlace(rawFile);

    REQUIRE(rawFile.compressedLen == 12);
    REQUIRE(rawFile.len == 42);

    IW4::SndCurve soundCurve{};
    soundCurve.knotCount = 0x0300;
    soundCurve.knots[0][0] = std::bit_cast<float>(0x0000803fu);
    soundCurve.knots[0][1] = std::bit_cast<float>(0x00000040u);

    endianness::FromBigEndianInPlace(soundCurve);

    REQUIRE(soundCurve.knotCount == 3);
    REQUIRE(soundCurve.knots[0][0] == 1.0f);
    REQUIRE(soundCurve.knots[0][1] == 2.0f);

    IW4::PhysCollmap physCollmap{};
    physCollmap.count = 0x02000000;
    physCollmap.mass.centerOfMass[0] = std::bit_cast<float>(0x0000803fu);
    physCollmap.bounds.halfSize.v[2] = std::bit_cast<float>(0x00000040u);

    endianness::FromBigEndianInPlace(physCollmap);

    REQUIRE(physCollmap.count == 2);
    REQUIRE(physCollmap.mass.centerOfMass[0] == 1.0f);
    REQUIRE(physCollmap.bounds.halfSize.v[2] == 2.0f);

    IW4::MapEnts mapEnts{};
    mapEnts.numEntityChars = 0x2a000000;
    mapEnts.trigger.count = 0x02000000;
    mapEnts.trigger.hullCount = 0x03000000;

    endianness::FromBigEndianInPlace(mapEnts);

    REQUIRE(mapEnts.numEntityChars == 42);
    REQUIRE(mapEnts.trigger.count == 2);
    REQUIRE(mapEnts.trigger.hullCount == 3);

    IW4::LeaderboardDef leaderboard{};
    leaderboard.id = 0x07000000;
    leaderboard.columnCount = 0x04000000;

    endianness::FromBigEndianInPlace(leaderboard);

    REQUIRE(leaderboard.id == 7);
    REQUIRE(leaderboard.columnCount == 4);

    IW4::TracerDef tracer{};
    tracer.drawInterval = 0x08000000;
    tracer.speed = std::bit_cast<float>(0x0000803fu);
    tracer.colors[4][3] = std::bit_cast<float>(0x00000040u);

    endianness::FromBigEndianInPlace(tracer);

    REQUIRE(tracer.drawInterval == 8);
    REQUIRE(tracer.speed == 1.0f);
    REQUIRE(tracer.colors[4][3] == 2.0f);
}
