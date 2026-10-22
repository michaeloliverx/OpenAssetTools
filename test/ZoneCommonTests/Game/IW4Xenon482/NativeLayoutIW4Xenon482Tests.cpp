#include "Game/IW4Xenon482/AssetEndianSwapIW4Xenon482.h"
#include "Game/IW4Xenon482/ZoneConstantsIW4Xenon482.h"

#include <catch2/catch_test_macros.hpp>

#ifdef ARCH_x86

TEST_CASE("IW4 482 native layouts match the executable loaders", "[iw4xenon482][layout]")
{
    using namespace IW4Xenon482;
    REQUIRE(sizeof(XSurface) == 0x78);
    REQUIRE(sizeof(XModelSurfs) == 0x20);
    REQUIRE(sizeof(XModelLodInfo) == 0x24);
    REQUIRE(sizeof(XModel) == 0x10C);
    REQUIRE(sizeof(XAnimParts) == 0x58);
    REQUIRE(sizeof(MaterialTechniqueSet) == 0x90);
    REQUIRE(sizeof(GfxImage) == 0x70);
    REQUIRE(sizeof(itemDef_s) == 0x1F4);
    REQUIRE(sizeof(menuDef_t) == 0x1A0);
    REQUIRE(sizeof(WeaponDef) == 0x680);
    REQUIRE(ZoneConstants::ZONE_VERSION == 253);
}

TEST_CASE("IW4 482 item expression pointers survive endian conversion", "[iw4xenon482][endian]")
{
    using namespace IW4Xenon482;
    itemDef_s item{};
    item.rectXExp = reinterpret_cast<Statement_s*>(0x12345678u);
    item.forecolorAExp = reinterpret_cast<Statement_s*>(0x87654321u);
    item.glowColorBExp = reinterpret_cast<Statement_s*>(0x11223344u);
    EndianSwap(item, EndianOperation::Encode);
    REQUIRE(reinterpret_cast<uintptr_t>(item.rectXExp) == 0x78563412u);
    REQUIRE(reinterpret_cast<uintptr_t>(item.forecolorAExp) == 0x21436587u);
    REQUIRE(reinterpret_cast<uintptr_t>(item.glowColorBExp) == 0x44332211u);
    EndianSwap(item, EndianOperation::Decode);
    REQUIRE(reinterpret_cast<uintptr_t>(item.rectXExp) == 0x12345678u);
    REQUIRE(reinterpret_cast<uintptr_t>(item.forecolorAExp) == 0x87654321u);
    REQUIRE(reinterpret_cast<uintptr_t>(item.glowColorBExp) == 0x11223344u);
}

#endif
