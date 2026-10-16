#include "Game/T5Xenon/T5Xenon.h"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>

#ifdef ARCH_x86

TEST_CASE("T5 Xenon material tables retain the native loader layout", "[t5xenon][layout][material]")
{
    REQUIRE(sizeof(T5Xenon::Material) == 128);
    REQUIRE(offsetof(T5Xenon::Material, constantCount) == 104);
    REQUIRE(offsetof(T5Xenon::Material, stateBitsCount) == 105);
    REQUIRE(offsetof(T5Xenon::Material, constantTable) == 120);
    REQUIRE(offsetof(T5Xenon::Material, stateBitsTable) == 124);
    REQUIRE(sizeof(T5Xenon::MaterialConstantDef) == 32);
    REQUIRE(alignof(T5Xenon::MaterialConstantDef) == 16);
    REQUIRE(offsetof(T5Xenon::MaterialConstantDef, literal) == 16);
    REQUIRE(sizeof(T5Xenon::GfxStateBits) == 8);
    REQUIRE(alignof(T5Xenon::GfxStateBits) == 4);
}

TEST_CASE("T5 Xenon vertex routing overlays rebuilt runtime declarations", "[t5xenon][layout][material]")
{
    REQUIRE(sizeof(T5Xenon::MaterialVertexDeclaration) == 72);
    REQUIRE(offsetof(T5Xenon::MaterialVertexDeclaration, routing) == 4);
    REQUIRE(offsetof(T5Xenon::MaterialVertexStreamRouting, data) == 0);
    REQUIRE(offsetof(T5Xenon::MaterialVertexStreamRouting, decl) == 0);
    REQUIRE(sizeof(T5Xenon::MaterialVertexStreamRouting::decl) == 68);
}

TEST_CASE("T5 Xenon static-model draw metadata matches native rendering", "[t5xenon][layout][model]")
{
    REQUIRE(sizeof(T5Xenon::GfxStaticModelDrawInst) == 44);
    REQUIRE(offsetof(T5Xenon::GfxStaticModelDrawInst, flags) == 36);
    REQUIRE(offsetof(T5Xenon::GfxStaticModelDrawInst, lightingHandle) == 40);
    REQUIRE(offsetof(T5Xenon::GfxStaticModelDrawInst, reflectionProbeIndex) == 42);
    REQUIRE(offsetof(T5Xenon::GfxStaticModelDrawInst, primaryLightIndex) == 43);
}

#endif
