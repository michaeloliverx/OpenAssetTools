#include "Game/IW3Xenon253/AssetEndianSwapIW3Xenon253.h"
#include "Game/IW3Xenon253/ZoneConstantsIW3Xenon253.h"

#include <catch2/catch_test_macros.hpp>

#ifdef ARCH_x86

TEST_CASE("IW3 253 native layouts match the executable loaders", "[iw3xenon253][layout]")
{
    using namespace IW3Xenon253;
    REQUIRE(sizeof(PhysPreset) == 0x2c);
    REQUIRE(sizeof(XModel) == 0xcc);
    REQUIRE(TECHNIQUE_LIT == 6);
    REQUIRE(TECHNIQUE_COUNT == 24);
    REQUIRE(NUM_OPERATORS == 43);
    REQUIRE(sizeof(ComPrimaryLight) == 68);
    REQUIRE(offsetof(ComPrimaryLight, exponent) == 52);
    REQUIRE(sizeof(Material) == 0x48);
    REQUIRE(sizeof(GfxImage) == 0x28);
    REQUIRE(sizeof(GfxImageLoadDef) == 0x10);
    REQUIRE(sizeof(D3DTexture) == 52);
    REQUIRE(sizeof(SndCurve) == 0x48);
    REQUIRE(sizeof(MapEnts) == 0xc);
    REQUIRE(sizeof(GfxWorld) == 0x318);
    REQUIRE(sizeof(WeaponDef) == 0x798);
    REQUIRE(sizeof(ComWorld) == 0xc);
    REQUIRE(sizeof(MaterialTechniqueSet) == 0x6c);
    REQUIRE(sizeof(FxEffectDef) == 0x1c);
    REQUIRE(sizeof(XAssetList) == 0x10);
    REQUIRE(ASSET_TYPE_PIXELSHADER == 5);
    REQUIRE(ASSET_TYPE_TECHNIQUE_SET == 6);
    REQUIRE(ASSET_TYPE_COUNT == 32);
    REQUIRE(MAX_XFILE_COUNT == 5);
    REQUIRE(ZoneConstants::ZONE_VERSION == 358);
}

TEST_CASE("IW3 253 shader union swaps preserve code constant rows", "[iw3xenon253][endian]")
{
    using namespace IW3Xenon253;
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
