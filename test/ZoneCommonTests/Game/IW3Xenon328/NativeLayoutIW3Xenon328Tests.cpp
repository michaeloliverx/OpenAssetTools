#include "Game/IW3Xenon328/AssetEndianSwapIW3Xenon328.h"
#include "Game/IW3Xenon328/ZoneConstantsIW3Xenon328.h"

#include <catch2/catch_test_macros.hpp>

#ifdef ARCH_x86

TEST_CASE("IW3 328 native layouts match the executable loaders", "[iw3xenon328][layout]")
{
    using namespace IW3Xenon328;
    REQUIRE(sizeof(PhysPreset) == 0x2C);
    REQUIRE(sizeof(XAnimParts) == 0x5C);
    REQUIRE(sizeof(XModel) == 0xCC);
    REQUIRE(sizeof(Material) == 0x48);
    REQUIRE(sizeof(GfxImage) == 0x28);
    REQUIRE(sizeof(SndCurve) == 0x48);
    REQUIRE(sizeof(MapEnts) == 0xC);
    REQUIRE(sizeof(GfxWorld) == 0x334);
    REQUIRE(sizeof(WeaponDef) == 0x874);
    REQUIRE(sizeof(StringTable) == 0x10);
    REQUIRE(sizeof(GfxImageLoadDef) == 0x10);
    REQUIRE(sizeof(D3DTexture) == 52);
    REQUIRE(sizeof(XAssetList) == 0x10);
    REQUIRE(ASSET_TYPE_PIXELSHADER == 5);
    REQUIRE(ASSET_TYPE_TECHNIQUE_SET == 6);
    REQUIRE(ASSET_TYPE_STRINGTABLE == 32);
    REQUIRE(ASSET_TYPE_COUNT == 33);
    REQUIRE(MAX_XFILE_COUNT == 5);
    REQUIRE(ZoneConstants::ZONE_VERSION == 429);
}

TEST_CASE("IW3 328 shader union swaps preserve code constant rows", "[iw3xenon328][endian]")
{
    using namespace IW3Xenon328;
    MaterialShaderArgument argument{};
    argument.type = MTL_ARG_CODE_VERTEX_CONST;
    argument.dest = 0x1357;
    argument.u.codeConst = {0x1234, 3, 4};
    EndianSwap(argument, EndianOperation::Encode);
    REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(MTL_ARG_CODE_VERTEX_CONST)));
    REQUIRE(argument.u.codeConst.index == 0x3412);
    REQUIRE(argument.u.codeConst.firstRow == 3);
    REQUIRE(argument.u.codeConst.rowCount == 4);
    EndianSwap(argument, EndianOperation::Decode);
    REQUIRE(argument.type == MTL_ARG_CODE_VERTEX_CONST);
    REQUIRE(argument.dest == 0x1357);
    REQUIRE(argument.u.codeConst.index == 0x1234);
    REQUIRE(argument.u.codeConst.firstRow == 3);
    REQUIRE(argument.u.codeConst.rowCount == 4);
}

TEST_CASE("IW3 328 path leaf swaps preserve node counts", "[iw3xenon328][endian]")
{
    using namespace IW3Xenon328;
    pathnode_tree_t tree{};
    tree.axis = -1;
    tree.u.s.nodeCount = 0x1234;
    tree.u.s.nodes = reinterpret_cast<decltype(tree.u.s.nodes)>(0x12345678u);
    EndianSwap(tree, EndianOperation::Encode);
    REQUIRE(tree.u.s.nodeCount == 0x34120000);
    REQUIRE(reinterpret_cast<uintptr_t>(tree.u.s.nodes) == 0x78563412u);
    EndianSwap(tree, EndianOperation::Decode);
    REQUIRE(tree.axis == -1);
    REQUIRE(tree.u.s.nodeCount == 0x1234);
    REQUIRE(reinterpret_cast<uintptr_t>(tree.u.s.nodes) == 0x12345678u);
}

#endif
