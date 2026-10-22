#include "Game/IW3Xenon270/AssetEndianSwapIW3Xenon270.h"
#include "Game/IW3Xenon270/ZoneConstantsIW3Xenon270.h"

#include <catch2/catch_test_macros.hpp>

#ifdef ARCH_x86

TEST_CASE("IW3 270 native layouts match the executable loaders", "[iw3xenon270][layout]")
{
    using namespace IW3Xenon270;
    REQUIRE(sizeof(PhysPreset) == 0x2c);
    REQUIRE(sizeof(XModel) == 0xcc);
    REQUIRE(sizeof(Material) == 0x48);
    REQUIRE(sizeof(GfxImage) == 0x28);
    REQUIRE(sizeof(GfxImageLoadDef) == 0x10);
    REQUIRE(sizeof(D3DTexture) == 52);
    REQUIRE(sizeof(SndCurve) == 0x48);
    REQUIRE(sizeof(MapEnts) == 0xc);
    REQUIRE(sizeof(GfxWorld) == 0x318);
    REQUIRE(sizeof(WeaponDef) == 0x83c);
    REQUIRE(sizeof(StringTable) == 0x10);
    REQUIRE(sizeof(XAssetList) == 0x10);
    REQUIRE(ASSET_TYPE_PIXELSHADER == 5);
    REQUIRE(ASSET_TYPE_TECHNIQUE_SET == 6);
    REQUIRE(ASSET_TYPE_STRINGTABLE == 32);
    REQUIRE(ASSET_TYPE_COUNT == 33);
    REQUIRE(MAX_XFILE_COUNT == 5);
    REQUIRE(ZoneConstants::ZONE_VERSION == 382);
}

TEST_CASE("IW3 270 shader union swaps preserve code constant rows", "[iw3xenon270][endian]")
{
    using namespace IW3Xenon270;
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

#endif
