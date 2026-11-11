#include "Game/T4Xenon302/Maps/D3DBspWriterT4Xenon302.h"

#include "Game/T4Xenon302/AssetEndianSwapT4Xenon302.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <map>
#include <sstream>

using namespace T4Xenon302;

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
        ComWorld lights{};
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
            lights.name = "maps/test.d3dbsp";
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
            world.lightGrid.rowDataStart = &emptyRow;
            surfaces[0].material = &material;
            surfaces[0].lightmapIndex = 31;
            surfaces[0].tris.vertexCount = 3;
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
            const auto result = d3dbsp::Write(output, world, clip, lights, search, recovery);
            INFO((result ? "success" : result.error()));
            REQUIRE(result);
            return ReadLumps(output.str());
        }
    };

} // namespace

TEST_CASE("T4 302 BSP retains index counts and Xenon packed vertices", "[t4xenon302][d3dbsp]")
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

TEST_CASE("T4 302 BSP follows nested cell tree pointers", "[t4xenon302][d3dbsp]")
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

TEST_CASE("T4 302 BSP rejects invalid pointer cell trees", "[t4xenon302][d3dbsp]")
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
    CHECK_FALSE(d3dbsp::Write(stream, f.world, f.clip, f.lights, f.search, f.recovery));
    CHECK(stream.str().empty());
}

TEST_CASE("T4 302 BSP retains the primary light exponent without inventing regions", "[t4xenon302][d3dbsp]")
{
    Fixture f;
    ComPrimaryLight lights[2]{};
    lights[1].type = 1;
    lights[1].exponent = 173;
    f.lights.primaryLightCount = f.world.primaryLightCount = 2;
    f.lights.primaryLights = lights;
    const auto lumps = f.Dump();
    CHECK_FALSE(lumps.contains(52));
    CHECK_FALSE(lumps.contains(53));
    CHECK_FALSE(lumps.contains(54));
    CHECK(Read<unsigned>(lumps.at(43), 128 + 52) == 173);
    CHECK(Read<unsigned char>(lumps.at(43), 128) == 1);
}

TEST_CASE("T4 302 BSP recovers non-colliding model placement and embedded paths", "[t4xenon302][d3dbsp]")
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
    instance.groundLighting.packed = 0x44332211;
    instance.primaryLightIndex = 7;
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
    CHECK(lumps.at(39).find("3322114407") != std::string::npos);
    CHECK(lumps.contains(40));
}

TEST_CASE("T4 302 BSP recovers graphics lights when common-world lights are absent", "[t4xenon302][d3dbsp]")
{
    Fixture f;
    GfxLight lights[2]{};
    lights[1].type = 1;
    lights[1].color[2] = .75f;
    lights[1].dir[2] = 1;
    lights[1].exponent = 19;
    f.world.primaryLightCount = 2;
    f.world.primaryLights = lights;
    const auto lumps = f.Dump();
    CHECK(lumps.at(43).size() == 2 * 128);
    CHECK(Read<unsigned char>(lumps.at(43), 128) == 1);
    CHECK(Read<float>(lumps.at(43), 128 + 12) == Catch::Approx(.75f));
    CHECK(Read<unsigned>(lumps.at(43), 128 + 52) == 19);
    CHECK(f.recovery.source.contains("primaryLightFallback"));
}

TEST_CASE("T4 302 model and light fields decode at executable-derived byte offsets", "[t4xenon302][d3dbsp]")
{
    unsigned char bytes[76]{};
    const auto word = [&](size_t offset, uint32_t value)
    {
        for (unsigned i = 0; i < 4; ++i)
            bytes[offset + i] = static_cast<unsigned char>(value >> (24 - i * 8));
    };
    word(0, 0x42c80000);  // cull distance 100
    word(4, 0x42f60000);  // origin x 123
    word(16, 511);        // packed axis x
    word(28, 0x40000000); // scale 2
    word(36, 0xc1200000); // bounds min x -10
    word(60, 0x44332211); // packed ground lighting
    bytes[72] = 0x01;
    bytes[73] = 0x23;
    bytes[74] = 7;
    bytes[75] = 0xa5;
    GfxStaticModelDrawInst instance;
    std::memcpy(&instance, bytes, sizeof(instance));
    EndianSwap(instance);
    CHECK(instance.cullDist == 100);
    CHECK(instance.placement.origin[0] == 123);
    CHECK(instance.placement.axis[0].packed == 511);
    CHECK(instance.placement.scale == 2);
    CHECK(instance.mins[0] == -10);
    CHECK(instance.groundLighting.packed == 0x44332211);
    CHECK(instance.reflectionProbeIndex == 0x123);
    CHECK(instance.primaryLightIndex == 7);
    CHECK(instance.unknown_0x4b == 0xa5);
    std::memset(bytes, 0, sizeof(bytes));
    word(0, 1); // type is a big-endian word, not a byte followed by flags
    word(4, 0x3f400000);
    word(52, 19);
    ComPrimaryLight common;
    std::memcpy(&common, bytes, sizeof(common));
    EndianSwap(common);
    CHECK(common.type == 1);
    CHECK(common.color[0] == .75f);
    CHECK(common.exponent == 19);
    GfxLight graphics;
    std::memcpy(&graphics, bytes, sizeof(graphics));
    EndianSwap(graphics);
    CHECK(graphics.type == 1);
    CHECK(graphics.color[0] == .75f);
    CHECK(graphics.exponent == 19);
}
