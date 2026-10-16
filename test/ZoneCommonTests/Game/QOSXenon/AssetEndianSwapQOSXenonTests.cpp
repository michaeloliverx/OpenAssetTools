#include "Game/QOSXenon/AssetEndianSwapQOSXenon.h"

#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstddef>
#include <cstdint>

namespace test::zone::game::qosxenon::asset_endian_swap
{
    using QOSXenon::EndianOperation;

    TEST_CASE("QOSXenon endian swap: Material techniques swap only their static prefix", "[qosxenon][endian]")
    {
        QOSXenon::MaterialTechnique technique{};
        technique.flags = 0x1234;
        technique.passCount = 0x5678;
        technique.passArray[0].perPrimArgCount = 3;

        EndianSwapPartial(technique, offsetof(QOSXenon::MaterialTechnique, passArray), EndianOperation::Encode);

        REQUIRE(technique.flags == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(technique.passCount == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
        REQUIRE(technique.passArray[0].perPrimArgCount == 3);

        EndianSwapPartial(technique, offsetof(QOSXenon::MaterialTechnique, passArray), EndianOperation::Decode);

        REQUIRE(technique.flags == 0x1234);
        REQUIRE(technique.passCount == 0x5678);
        REQUIRE(technique.passArray[0].perPrimArgCount == 3);
    }

    TEST_CASE("QOSXenon endian swap: Material code constants preserve byte fields", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_CODE_VERTEX_CONST, QOSXenon::MTL_ARG_CODE_PIXEL_CONST);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        argument.dest = 0x1234;
        argument.u.codeConst.index = 0x5678;
        argument.u.codeConst.firstRow = 3;
        argument.u.codeConst.rowCount = 4;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        REQUIRE(argument.dest == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(argument.u.codeConst.index == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        REQUIRE(argument.dest == 0x1234);
        REQUIRE(argument.u.codeConst.index == 0x5678);
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);
    }

    TEST_CASE("QOSXenon endian swap: Material hashes and samplers use their full word", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_MATERIAL_VERTEX_CONST,
                                   QOSXenon::MTL_ARG_MATERIAL_PIXEL_CONST,
                                   QOSXenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER,
                                   QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            argument.u.codeSampler = 0x12345678u;
        else
            argument.u.nameHash = 0x12345678u;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            REQUIRE(argument.u.codeSampler == endianness::ToBigEndian(0x12345678u));
        else
            REQUIRE(argument.u.nameHash == endianness::ToBigEndian(0x12345678u));

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            REQUIRE(argument.u.codeSampler == 0x12345678u);
        else
            REQUIRE(argument.u.nameHash == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: Unknown shader argument types are not inferred from swapped tags", "[qosxenon][endian]")
    {
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = 0x0300;
        argument.dest = 0x1234;
        argument.u.nameHash = 0x12345678u;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(0x0300)));
        REQUIRE(argument.dest == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(argument.u.nameHash == 0x12345678u);

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == 0x0300);
        REQUIRE(argument.dest == 0x1234);
        REQUIRE(argument.u.nameHash == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: Integer operands use the requested operation", "[qosxenon][endian]")
    {
        QOSXenon::Operand operand{};
        operand.dataType = QOSXenon::VAL_INT;
        operand.internals.intVal = 0x12345678;

        EndianSwap(operand, EndianOperation::Encode);

        REQUIRE(operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_INT))));
        REQUIRE(operand.internals.intVal == endianness::ToBigEndian(0x12345678));

        EndianSwap(operand, EndianOperation::Decode);

        REQUIRE(operand.dataType == QOSXenon::VAL_INT);
        REQUIRE(operand.internals.intVal == 0x12345678);
    }

    TEST_CASE("QOSXenon endian swap: Expressions forward the operation to their operands", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERAND;
        entry.data.operand.dataType = QOSXenon::VAL_FLOAT;
        entry.data.operand.internals.floatVal = 123.5f;

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERAND)));
        REQUIRE(entry.data.operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_FLOAT))));
        REQUIRE(std::bit_cast<uint32_t>(entry.data.operand.internals.floatVal) == endianness::ToBigEndian(std::bit_cast<uint32_t>(123.5f)));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERAND);
        REQUIRE(entry.data.operand.dataType == QOSXenon::VAL_FLOAT);
        REQUIRE(entry.data.operand.internals.floatVal == 123.5f);
    }

    TEST_CASE("QOSXenon endian swap: Operator expressions swap their enum", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERATOR;
        entry.data.op = QOSXenon::OP_ADD;

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERATOR)));
        REQUIRE(entry.data.op == static_cast<QOSXenon::operationEnum>(endianness::ToBigEndian(static_cast<int>(QOSXenon::OP_ADD))));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERATOR);
        REQUIRE(entry.data.op == QOSXenon::OP_ADD);
    }

#ifdef ARCH_x86
    TEST_CASE("QOSXenon endian swap: Literal arguments swap their pointer", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_LITERAL_VERTEX_CONST, QOSXenon::MTL_ARG_LITERAL_PIXEL_CONST);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        argument.u.literalConst = reinterpret_cast<float (*)[4]>(0x12345678u);

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        REQUIRE(reinterpret_cast<uintptr_t>(argument.u.literalConst) == endianness::ToBigEndian(0x12345678u));

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        REQUIRE(reinterpret_cast<uintptr_t>(argument.u.literalConst) == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: String expressions forward the operation to their pointer", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERAND;
        entry.data.operand.dataType = QOSXenon::VAL_STRING;
        entry.data.operand.internals.string = reinterpret_cast<const char*>(0x12345678u);

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERAND)));
        REQUIRE(entry.data.operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_STRING))));
        REQUIRE(reinterpret_cast<uintptr_t>(entry.data.operand.internals.string) == endianness::ToBigEndian(0x12345678u));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERAND);
        REQUIRE(entry.data.operand.dataType == QOSXenon::VAL_STRING);
        REQUIRE(reinterpret_cast<uintptr_t>(entry.data.operand.internals.string) == 0x12345678u);
    }
#endif
} // namespace test::zone::game::qosxenon::asset_endian_swap
