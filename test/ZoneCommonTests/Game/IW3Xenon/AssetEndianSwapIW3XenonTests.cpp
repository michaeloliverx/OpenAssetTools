#include "Game/IW3Xenon/AssetEndianSwapIW3Xenon.h"

#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace test::zone::game::iw3xenon::asset_endian_swap
{
    TEST_CASE("IW3Xenon D3D texture metadata matches the XDK base-texture layout", "[iw3xenon][endian]")
    {
        IW3Xenon::D3DTexture texture{};
        texture.Format.fields.Type = 2u;
        texture.Format.fields.Pitch = 8u;
        texture.Format.fields.Tiled = 1u;

        REQUIRE(sizeof(IW3Xenon::D3DTexture) == 52u);
        REQUIRE(offsetof(IW3Xenon::D3DBaseTexture, Format) == 28u);
        REQUIRE(texture.Format.dword[0] == 0x82000002u);

        const auto original = texture;
        EndianSwap(texture, EndianOperation::Encode);
        REQUIRE(std::memcmp(&texture, &original, sizeof(texture)) == 0);

        EndianSwap(texture, EndianOperation::Decode);
        REQUIRE(std::memcmp(&texture, &original, sizeof(texture)) == 0);
    }

    TEST_CASE("IW3Xenon endian swap: Material technique swaps its static prefix", "[iw3xenon][endian]")
    {
        IW3Xenon::MaterialTechnique technique{};
        technique.flags = 0x1234;
        technique.passCount = 0x5678;

        EndianSwapPartial(technique, offsetof(IW3Xenon::MaterialTechnique, passArray), EndianOperation::Encode);

        REQUIRE(technique.flags == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(technique.passCount == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));

        EndianSwapPartial(technique, offsetof(IW3Xenon::MaterialTechnique, passArray), EndianOperation::Decode);

        REQUIRE(technique.flags == 0x1234);
        REQUIRE(technique.passCount == 0x5678);
    }

    TEST_CASE("IW3Xenon endian swap: Material shader argument uses the requested operation", "[iw3xenon][endian]")
    {
        IW3Xenon::MaterialShaderArgument argument{};
        argument.type = IW3Xenon::MTL_ARG_CODE_VERTEX_CONST;
        argument.dest = 0x1234;
        argument.u.codeConst.index = 0x5678;
        argument.u.codeConst.firstRow = 3;
        argument.u.codeConst.rowCount = 4;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(IW3Xenon::MTL_ARG_CODE_VERTEX_CONST)));
        REQUIRE(argument.dest == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(argument.u.codeConst.index == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == IW3Xenon::MTL_ARG_CODE_VERTEX_CONST);
        REQUIRE(argument.dest == 0x1234);
        REQUIRE(argument.u.codeConst.index == 0x5678);
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);
    }

    TEST_CASE("IW3Xenon endian swap: Operand uses the requested operation", "[iw3xenon][endian]")
    {
        IW3Xenon::Operand operand{};
        operand.dataType = IW3Xenon::VAL_INT;
        operand.internals.intVal = 0x12345678;

        EndianSwap(operand, EndianOperation::Encode);

        REQUIRE(operand.dataType == static_cast<IW3Xenon::expDataType>(endianness::ToBigEndian(static_cast<int>(IW3Xenon::VAL_INT))));
        REQUIRE(operand.internals.intVal == endianness::ToBigEndian(0x12345678));

        EndianSwap(operand, EndianOperation::Decode);

        REQUIRE(operand.dataType == IW3Xenon::VAL_INT);
        REQUIRE(operand.internals.intVal == 0x12345678);
    }

    TEST_CASE("IW3Xenon endian swap: Expression entry forwards the operation to its operand", "[iw3xenon][endian]")
    {
        IW3Xenon::expressionEntry entry{};
        entry.type = IW3Xenon::EET_OPERAND;
        entry.data.operand.dataType = IW3Xenon::VAL_FLOAT;
        entry.data.operand.internals.floatVal = 123.5f;

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(IW3Xenon::EET_OPERAND)));
        REQUIRE(entry.data.operand.dataType == static_cast<IW3Xenon::expDataType>(endianness::ToBigEndian(static_cast<int>(IW3Xenon::VAL_FLOAT))));
        REQUIRE(std::bit_cast<uint32_t>(entry.data.operand.internals.floatVal) == endianness::ToBigEndian(std::bit_cast<uint32_t>(123.5f)));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == IW3Xenon::EET_OPERAND);
        REQUIRE(entry.data.operand.dataType == IW3Xenon::VAL_FLOAT);
        REQUIRE(entry.data.operand.internals.floatVal == 123.5f);
    }
} // namespace test::zone::game::iw3xenon::asset_endian_swap
