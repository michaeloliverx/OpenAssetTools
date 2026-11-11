#include "Game/T4Xenon204/Maps/D3DBspWriterT4Xenon204.h"

#include "Game/T4Xenon204/AssetEndianSwapT4Xenon204.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <map>
#include <sstream>

using namespace T4Xenon204;

namespace
{
    template<typename T> T Read(const std::string& bytes, size_t offset)
    {
        REQUIRE(offset + sizeof(T) <= bytes.size());
        T value;
        std::memcpy(&value, bytes.data() + offset, sizeof(value));
        return value;
    }

    std::map<unsigned, std::string> ReadLumps(const std::string& bytes)
    {
        REQUIRE(bytes.substr(0, 4) == "IBSP");
        REQUIRE(Read<uint32_t>(bytes, 4) == 22);
        const auto count = Read<uint32_t>(bytes, 8);
        size_t offset = 12 + count * 8;
        std::map<unsigned, std::string> lumps;
        for (unsigned i = 0; i < count; ++i)
        {
            const auto id = Read<uint32_t>(bytes, 12 + i * 8);
            const auto size = Read<uint32_t>(bytes, 16 + i * 8);
            REQUIRE(offset + size <= bytes.size());
            REQUIRE(lumps.emplace(id, bytes.substr(offset, size)).second);
            offset += (size + 3u) & ~3u;
        }
        REQUIRE(offset == bytes.size());
        return lumps;
    }

    struct Fixture
    {
        GfxWorld world{};
        clipMap_t clip{};
        char entityText[48] = "{\n\"classname\" \"worldspawn\"\n}\n";
        Material material{};
        dmaterial_t materials[3]{
            {"stone",  0, 1},
            {"dirt",   0, 1},
            {"*0n_1n", 0, 1}
        };
        GfxWorldVertex vertices[3]{};
        uint16_t indices[3]{0, 1, 2};
        GfxSurface surfaces[2]{};
        GfxAabbTree tree{};
        d3dbsp::Recovery recovery;
        GfxCell cell{};
        GfxBrushModel model{};
        uint16_t emptyRow = 0xffff;
        MockSearchPath search;

        Fixture()
        {
            clip.entityString = entityText;
            clip.numEntityChars = static_cast<int>(std::strlen(entityText) + 1);
            material.info.name = "wc/stone";
            world.name = "maps/test.d3dbsp";
            clip.name = "maps/test.d3dbsp";
            clip.materials = materials;
            clip.numMaterials = 3;
            world.surfaceCount = 1;
            world.surfaces = surfaces;
            world.vertexCount = 3;
            world.vd.vertices = vertices;
            world.indexCount = 3;
            world.indices = indices;
            world.cellCount = 1;
            world.cells = &cell;
            cell.aabbTree = &tree;
            tree.surfaceCount = 1;
            world.modelCount = 1;
            world.models = &model;
            model.surfaceCount = 1;
            surfaces[0].material = &material;
            surfaces[0].lightmapIndex = 31;
            surfaces[0].tris.indices = indices;
            surfaces[0].tris.indexCount = 3;
            for (auto& vertex : vertices)
            {
                // Signed 10-bit +1 is 511; use disk bits independently of the packer.
                vertex.normal.packed = 511u << 20;
                vertex.tangent.packed = 511u;
                vertex.binormalSign = -1;
                vertex.color.packed = 0x482d80e1;
            }
            vertices[1].xyz[0] = 64;
            vertices[2].xyz[1] = 64;
        }

        std::map<unsigned, std::string> Dump()
        {
            std::ostringstream output(std::ios::binary);
            const auto result = d3dbsp::Write(output, world, clip, search, recovery);
            INFO((result ? "success" : result.error()));
            REQUIRE(result);
            return ReadLumps(output.str());
        }
    };

} // namespace

TEST_CASE("T4 FF204 BSP retains index counts and Xenon packed vertices", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    const auto lumps = f.Dump();
    CHECK(Read<uint16_t>(lumps.at(9), 18) == 3);
    CHECK(lumps.at(10).size() == 3 * 68);
    CHECK(Read<float>(lumps.at(10), 20) == Catch::Approx(1));
    CHECK(Read<uint32_t>(lumps.at(10), 24) == 0x482d80e1);
    CHECK(Read<float>(lumps.at(10), 44) == Catch::Approx(1));
    CHECK(Read<float>(lumps.at(10), 60) == Catch::Approx(-1));
    CHECK_FALSE(lumps.contains(52));
}

TEST_CASE("T4 FF204 BSP follows nested cell tree pointers", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    GfxAabbTree children[2]{}, grandchild{};
    f.tree.childCount = 2;
    f.tree.children = children;
    children[0].surfaceCount = grandchild.surfaceCount = 1;
    children[0].childCount = 1;
    children[0].children = &grandchild;
    const auto lumps = f.Dump();
    CHECK(lumps.at(24).size() == 4 * 12);
    CHECK(Read<unsigned>(lumps.at(24), 8) == 2);
    CHECK(Read<unsigned>(lumps.at(24), 20) == 1);
    CHECK(Read<unsigned>(lumps.at(24), 40) == 1);
}

TEST_CASE("T4 FF204 BSP rejects invalid pointer cell trees", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    SECTION("Cycle")
    {
        f.tree.childCount = 1;
        f.tree.children = &f.tree;
    }
    SECTION("Missing children")
    {
        f.tree.childCount = 1;
    }
    SECTION("Negative children")
    {
        f.tree.childCount = -1;
    }
    std::ostringstream stream;
    CHECK_FALSE(d3dbsp::Write(stream, f.world, f.clip, f.search, f.recovery));
    CHECK(stream.str().empty());
}

TEST_CASE("T4 FF204 BSP recovers non-colliding model placement and embedded paths", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    XModel model{};
    model.name = "test_model";
    GfxStaticModelDrawInst instance{};
    instance.model = &model;
    instance.placement.origin[0] = 123;
    instance.placement.scale = 2;
    instance.placement.axis[0].packed = 511;
    instance.placement.axis[1].packed = 511u << 10;
    instance.placement.axis[2].packed = 511u << 20;
    instance.unknown_0x3c[0] = .5f;
    instance.primaryLightIndex = 0;
    f.world.smodelDrawInsts = &instance;
    f.world.smodelCount = 1;
    pathnode_t nodes[2]{};
    pathlink_s link{};
    link.nodeNum = 1;
    link.fDist = 64;
    nodes[0].constant.totalLinkCount = 1;
    nodes[0].constant.Links = &link;
    f.clip.path.nodes = nodes;
    f.clip.path.nodeCount = 2;
    const auto lumps = f.Dump();
    CHECK(lumps.at(39).find("test_model") != std::string::npos);
    CHECK(lumps.at(39).find("123 0 0") != std::string::npos);
    CHECK(f.recovery.source["staticModelUnknownLightingCoordinates"][0][0].get<float>() == Catch::Approx(.5f));
    CHECK(lumps.contains(40));
}

TEST_CASE("T4 FF204 BSP gathers referenced planes and retains inverse pairs", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    cplane_s plane{};
    plane.normal[0] = 1;
    plane.dist = 32;
    cbrushside_t side{};
    side.plane = &plane;
    f.clip.numBrushSides = 1;
    f.clip.brushsides = &side;
    const auto lumps = f.Dump();
    REQUIRE(lumps.at(4).size() == 32);
    CHECK(Read<float>(lumps.at(4), 0) == 1);
    CHECK(Read<float>(lumps.at(4), 12) == 32);
    CHECK(Read<float>(lumps.at(4), 16) == -1);
    CHECK(Read<float>(lumps.at(4), 28) == -32);
}

TEST_CASE("T4 FF204 BSP reconstructs the pointer partition and leaf cells", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    cplane_s plane{};
    plane.normal[0] = 1;
    cNode_t clipNode{};
    clipNode.plane = &plane;
    clipNode.children[0] = -1;
    clipNode.children[1] = -2;
    cLeaf_t leaves[2]{};
    leaves[0].leafBrushNode = leaves[1].leafBrushNode = -1;
    f.clip.numNodes = 1;
    f.clip.nodes = &clipNode;
    f.clip.numLeafs = 2;
    f.clip.leafs = leaves;
    GfxBspNode nodes[3]{};
    nodes[0].unknownNodeType = -1;
    nodes[0].data.branch.plane = &plane;
    nodes[0].data.branch.children[0] = nodes + 1;
    nodes[0].data.branch.children[1] = nodes + 2;
    nodes[1].cellIndex = 0;
    nodes[2].cellIndex = -1;
    f.world.nodes = nodes;
    f.world.nodeCount = 3;
    SECTION("valid partition")
    {
        const auto lumps = f.Dump();
        CHECK(Read<int>(lumps.at(28), 20) == 0);
        CHECK(Read<int>(lumps.at(28), 44) == -1);
    }
    SECTION("cycle rejected")
    {
        nodes[0].data.branch.children[0] = nodes;
        std::ostringstream output;
        CHECK_FALSE(d3dbsp::Write(output, f.world, f.clip, f.search, f.recovery));
        CHECK(output.str().empty());
    }
}

TEST_CASE("T4 FF204 surface lightmap follows the reflection byte", "[t4xenon204][d3dbsp]")
{
    CHECK(offsetof(GfxSurface, reflectionProbeIndex) == 40);
    CHECK(offsetof(GfxSurface, lightmapIndex) == 41);
}

TEST_CASE("T4 FF204 BSP supplies Radiant's reserved null and sun light records", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    SECTION("source sun absent")
    {
        const auto lumps = f.Dump();
        REQUIRE(lumps.at(43).size() == 256);
        CHECK(lumps.at(43).substr(0, 128) == std::string(128, '\0'));
        CHECK(Read<unsigned char>(lumps.at(43), 128) == 1);
        CHECK(Read<float>(lumps.at(43), 128 + 24) == 1);
        CHECK(Read<float>(lumps.at(43), 128 + 4) == 0);
    }
    SECTION("retained big-endian source sun")
    {
        GfxLight source{};
        const auto putFloat = [&](size_t offset, float value)
        {
            uint32_t bits;
            std::memcpy(&bits, &value, 4);
            bits = endianness::ToBigEndian(bits);
            std::memcpy(source.unknown + offset, &bits, 4);
        };
        putFloat(4, -.8137976527f);
        putFloat(8, .2961981893f);
        putFloat(12, .5f);
        putFloat(20, .216f);
        putFloat(24, .216f);
        putFloat(28, .24f);
        f.world.sunLight = &source;
        const auto lumps = f.Dump();
        REQUIRE(lumps.at(43).size() == 256);
        CHECK(Read<unsigned char>(lumps.at(43), 0) == 0);
        CHECK(Read<unsigned char>(lumps.at(43), 128) == 1);
        CHECK(Read<float>(lumps.at(43), 128 + 16) == Catch::Approx(-.8137976527f));
        CHECK(Read<float>(lumps.at(43), 128 + 20) == Catch::Approx(.2961981893f));
        CHECK(Read<float>(lumps.at(43), 128 + 24) == Catch::Approx(.5f));
        CHECK(Read<float>(lumps.at(43), 128 + 4) == Catch::Approx(.216f));
        CHECK(Read<float>(lumps.at(43), 128 + 12) == Catch::Approx(.24f));
    }
}

TEST_CASE("T4 FF204 BSP rejects malformed triangle pointers and ranges", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    SECTION("pointer outside index array")
    {
        f.surfaces[0].tris.indices = f.indices + 3;
    }
    SECTION("range outside index array")
    {
        f.surfaces[0].tris.indexCount = 6;
    }
    SECTION("incomplete triangle")
    {
        f.surfaces[0].tris.indexCount = 2;
    }
    SECTION("negative vertex base")
    {
        f.surfaces[0].tris.firstVertex = -1;
    }
    SECTION("vertex outside vertex array")
    {
        f.indices[2] = 3;
    }
    std::ostringstream output;
    CHECK_FALSE(d3dbsp::Write(output, f.world, f.clip, f.search, f.recovery));
    CHECK(output.str().empty());
}

TEST_CASE("T4 FF204 BSP retains authored model entities without duplicating placements", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    XModel model{};
    model.name = "native_model";
    GfxStaticModelDrawInst instance{};
    instance.model = &model;
    instance.placement.scale = 1;
    f.world.smodelDrawInsts = &instance;
    f.world.smodelCount = 1;
    const std::string entities = R"({
"classname" "worldspawn"
}
{
"classname" "misc_model"
"model" "native_model"
"origin" "1 2 3"
"gndLt" "12345678"
"authored_key" "retained value"
}
)";
    f.clip.entityString = const_cast<char*>(entities.c_str());
    f.clip.numEntityChars = static_cast<int>(entities.size() + 1);
    SECTION("complete authored list retained verbatim")
    {
        const auto lumps = f.Dump();
        CHECK(lumps.at(39) == entities + '\0');
        CHECK(f.recovery.source["modelEntities"]["retained"] == 1);
        CHECK(f.recovery.source["modelEntities"]["reconstructed"] == 0);
    }
    SECTION("partial authored list rejected")
    {
        f.world.smodelCount = 2;
        GfxStaticModelDrawInst instances[2]{instance, instance};
        f.world.smodelDrawInsts = instances;
        std::ostringstream output;
        const auto result = d3dbsp::Write(output, f.world, f.clip, f.search, f.recovery);
        REQUIRE_FALSE(result);
        CHECK(result.error().find("retained model-entity count") != std::string::npos);
        CHECK(output.str().empty());
    }
}

TEST_CASE("T4 FF204 BSP converts packed legacy grid samples and channel-major colors", "[t4xenon204][d3dbsp]")
{
    Fixture f;
    // Grid (4096,4096,2048), sunlight visible, old trace corner 1, color zero.
    GfxLightGridEntry entries[2]{{{0x80100200, 0x01020000}}, {{0x80100200, 0x09000000}}};
    GfxLightGridColors color{};
    auto* bytes = reinterpret_cast<unsigned char*>(&color);
    std::memset(bytes, 64, 8);
    std::memset(bytes + 8, 96, 8);
    std::memset(bytes + 16, 128, 8);
    f.world.lightGrid.entries = entries;
    f.world.lightGrid.entryCount = 1;
    f.world.lightGrid.colors = &color;
    f.world.lightGrid.colorCount = 1;
    SECTION("single point and trace swizzle")
    {
        const auto lumps = f.Dump();
        REQUIRE(lumps.at(2).size() == 4);
        CHECK(Read<uint16_t>(lumps.at(2), 0) == 0);
        CHECK(Read<uint8_t>(lumps.at(2), 2) == 1);
        CHECK(Read<uint8_t>(lumps.at(2), 3) == 16);
        CHECK(Read<uint16_t>(lumps.at(44), 0) == 4096);
        CHECK(Read<uint16_t>(lumps.at(44), 4) == 2048);
        CHECK(Read<uint16_t>(lumps.at(45), 0) == 4096);
        CHECK(Read<uint16_t>(lumps.at(45), 4) == 2048);
        CHECK(Read<uint8_t>(lumps.at(45), 12) == 1);
        CHECK(Read<uint8_t>(lumps.at(45), 13) == 1);
        REQUIRE(lumps.at(3).size() == 168);
        for (unsigned i = 0; i < 56; ++i)
            for (unsigned channel = 0; channel < 3; ++channel)
            {
                const auto value = Read<uint8_t>(lumps.at(3), i * 3 + channel);
                CHECK(value >= 63 + 32 * channel);
                CHECK(value <= 65 + 32 * channel);
            }
    }
    SECTION("vertical hole uses the stock default entry")
    {
        f.world.lightGrid.entryCount = 2;
        const auto lumps = f.Dump();
        REQUIRE(lumps.at(2).size() == 12);
        CHECK(Read<uint16_t>(lumps.at(45), 6) == 3);
        CHECK(Read<uint8_t>(lumps.at(45), 13) == 3);
        CHECK(Read<uint16_t>(lumps.at(2), 4) == 0);
        CHECK(Read<uint8_t>(lumps.at(2), 6) == 1);
        CHECK(Read<uint8_t>(lumps.at(2), 7) == 0);
    }
    SECTION("invalid native color reference rejected")
    {
        entries[0].unknown[1] |= 1;
        std::ostringstream output;
        CHECK_FALSE(d3dbsp::Write(output, f.world, f.clip, f.search, f.recovery));
        CHECK(output.str().empty());
    }
    SECTION("duplicate native coordinate retained once")
    {
        entries[1] = entries[0];
        f.world.lightGrid.entryCount = 2;
        const auto lumps = f.Dump();
        CHECK(lumps.at(2).size() == 4);
        CHECK(f.recovery.source["lightGrid"]["decodedPoints"] == 1);
    }
}
