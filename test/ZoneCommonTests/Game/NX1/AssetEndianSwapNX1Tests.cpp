#include "Game/NX1/AssetEndianSwapNX1.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace NX1;

TEST_CASE("NX1 shader arguments preserve the discriminated union", "[nx1][endian]")
{
    MaterialShaderArgument argument{};
    argument.type = GENERATE(MTL_ARG_LITERAL_VERTEX_CONST, MTL_ARG_CODE_VERTEX_CONST, MTL_ARG_CODE_PIXEL_CONST);
    const auto type = argument.type;
    argument.dest = 0x1234;
    if (type == MTL_ARG_LITERAL_VERTEX_CONST)
        argument.u.literalConst = reinterpret_cast<float (*)[4]>(0x12345678u);
    else
    {
        argument.u.codeConst.index = 0x5678;
        argument.u.codeConst.firstRow = 3;
        argument.u.codeConst.rowCount = 4;
    }
    EndianSwap(argument, EndianOperation::Encode);
    REQUIRE(argument.type == endianness::ToBigEndian(type));
    EndianSwap(argument, EndianOperation::Decode);
    REQUIRE(argument.type == type);
    REQUIRE(argument.dest == 0x1234);
    if (type == MTL_ARG_LITERAL_VERTEX_CONST)
        REQUIRE(argument.u.literalConst == reinterpret_cast<float (*)[4]>(0x12345678u));
    else
    {
        REQUIRE(argument.u.codeConst.index == 0x5678);
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);
    }
}

TEST_CASE("NX1 path tree unions use the host-order axis", "[nx1][endian]")
{
    pathnode_tree_t tree{};
    tree.axis = -1;
    tree.u.s.nodeCount = 0x1234;
    EndianSwap(tree, EndianOperation::Encode);
    EndianSwap(tree, EndianOperation::Decode);
    REQUIRE(tree.axis == -1);
    REQUIRE(tree.u.s.nodeCount == 0x1234);
}

TEST_CASE("NX1 expression unions support a zero discriminator", "[nx1][endian]")
{
    expressionEntry entry{};
    entry.type = EET_OPERATOR;
    entry.data.op = 0x12345678;
    EndianSwap(entry, EndianOperation::Encode);
    REQUIRE(entry.type == 0);
    REQUIRE(entry.data.op == endianness::ToBigEndian(0x12345678));
    EndianSwap(entry, EndianOperation::Decode);
    REQUIRE(entry.type == EET_OPERATOR);
    REQUIRE(entry.data.op == 0x12345678);
}
