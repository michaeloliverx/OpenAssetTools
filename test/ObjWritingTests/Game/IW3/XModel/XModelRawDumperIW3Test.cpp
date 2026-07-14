#include "Game/IW3/XModel/XModelRawDumperIW3.h"

#include "Game/IW3/CommonIW3.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

using namespace IW3;

namespace
{
    class Reader
    {
    public:
        explicit Reader(const std::vector<uint8_t>& data)
            : m_data(data)
        {
        }

        template<typename T> T Read()
        {
            REQUIRE(m_offset + sizeof(T) <= m_data.size());
            T result;
            std::memcpy(&result, &m_data[m_offset], sizeof(T));
            m_offset += sizeof(T);
            return result;
        }

        std::string ReadString()
        {
            const auto start = m_offset;
            while (m_offset < m_data.size() && m_data[m_offset])
                m_offset++;
            REQUIRE(m_offset < m_data.size());
            const std::string result(reinterpret_cast<const char*>(&m_data[start]), m_offset - start);
            m_offset++;
            return result;
        }

        [[nodiscard]] size_t Offset() const
        {
            return m_offset;
        }

    private:
        const std::vector<uint8_t>& m_data;
        size_t m_offset = 0u;
    };

    GfxPackedVertex MakeVertex(const float x, const float y, const float z)
    {
        GfxPackedVertex result{};
        result.xyz = {x, y, z};
        result.binormalSign = 1.0f;
        result.color.packed = 0xFFFFFFFFu;
        const float uv[2]{0.25f, 0.75f};
        const float normal[3]{0.0f, 0.0f, 1.0f};
        const float tangent[3]{1.0f, 0.0f, 0.25f};
        result.texCoord = Common::Vec2PackTexCoords(uv);
        result.normal = Common::Vec3PackUnitVec(normal);
        result.tangent = Common::Vec3PackUnitVec(tangent);
        return result;
    }
} // namespace

TEST_CASE("IW3 raw xmodel writer emits the linker version-25 bundle", "[iw3][xmodel]")
{
    ZoneScriptStrings scriptStrings;
    const ScriptString boneName = scriptStrings.AddOrGetScriptString("tag_origin");

    ScriptString boneNames[]{boneName};
    uint8_t classifications[]{0u};
    XBoneInfo boneInfo[]{{.bounds = {{-1.0f, -2.0f, -3.0f}, {1.0f, 2.0f, 3.0f}}}};

    std::array<GfxPackedVertex, 3> vertices{
        MakeVertex(0.0f, 0.0f, 0.0f),
        MakeVertex(1.0f, 0.0f, 0.0f),
        MakeVertex(0.0f, 1.0f, 0.0f),
    };
    XSurfaceTri16 triangles[1]{};
    triangles[0].i[0] = 0u;
    triangles[0].i[1] = 1u;
    triangles[0].i[2] = 2u;
    XRigidVertList rigidList{.boneOffset = 0u, .vertCount = 3u, .triOffset = 0u, .triCount = 1u};
    XSurface surface{
        .tileMode = 1,
        .deformed = false,
        .vertCount = 3u,
        .triCount = 1u,
        .triIndices = triangles,
        .verts0 = vertices.data(),
        .vertListCount = 1u,
        .vertList = &rigidList,
    };

    Material material{};
    material.info.name = "mc/test_material";
    Material* materials[]{&material};

    XModel model{};
    model.name = "test_model";
    model.numBones = 1u;
    model.numRootBones = 1u;
    model.numsurfs = 1u;
    model.boneNames = boneNames;
    model.partClassification = classifications;
    model.surfs = &surface;
    model.materialHandles = materials;
    // The linker compiles the source format's terminal zero distance to this sentinel.
    model.lodInfo[0].dist = 1000000.0f;
    model.lodInfo[0].numsurfs = 1u;
    model.lodInfo[0].surfIndex = 0u;
    model.boneInfo = boneInfo;
    model.radius = 5.0f;
    model.numLods = 1u;
    model.collLod = -1;
    model.flags = 1u;

    xmodel::RawXModelFilesIW3 files;
    std::string error;
    REQUIRE(xmodel::BuildRawXModelFilesIW3(model, model.name, scriptStrings, files, error));
    CHECK(error.empty());
    CHECK(files.partsName == "test_model_lod0");
    CHECK(files.surfaceNames[0] == "test_model_lod0");

    Reader parts(files.parts);
    CHECK(parts.Read<uint16_t>() == 25u);
    CHECK(parts.Read<uint16_t>() == 0u);
    CHECK(parts.Read<uint16_t>() == 1u);
    CHECK(parts.ReadString() == "tag_origin");
    CHECK(parts.Read<uint8_t>() == 0u);
    CHECK(parts.Read<uint8_t>() == 1u);
    CHECK(parts.Offset() == files.parts.size());

    Reader surfaces(files.surfaces[0]);
    CHECK(surfaces.Read<uint16_t>() == 25u);
    CHECK(surfaces.Read<uint16_t>() == 1u);
    CHECK(surfaces.Read<uint8_t>() == 1u);
    CHECK(surfaces.Read<uint16_t>() == 0u);
    CHECK(surfaces.Read<uint16_t>() == 3u);
    CHECK(surfaces.Read<uint16_t>() == 1u);
    CHECK(surfaces.Read<uint16_t>() == 3u);
    CHECK(surfaces.Read<uint16_t>() == 0u);
    CHECK(surfaces.Read<uint16_t>() == 0u);
    const std::array normal{surfaces.Read<float>(), surfaces.Read<float>(), surfaces.Read<float>()};
    (void)surfaces.Read<uint32_t>();
    (void)surfaces.Read<float>();
    (void)surfaces.Read<float>();
    const std::array binormal{surfaces.Read<float>(), surfaces.Read<float>(), surfaces.Read<float>()};
    const std::array tangent{surfaces.Read<float>(), surfaces.Read<float>(), surfaces.Read<float>()};
    const auto Dot = [](const auto& a, const auto& b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    };
    CHECK(std::abs(Dot(normal, tangent)) < 0.002f);
    CHECK(std::abs(Dot(normal, binormal)) < 0.002f);
    CHECK(std::abs(Dot(tangent, binormal)) < 0.002f);
    CHECK(files.surfaces[0].size() == 203u);

    Reader main(files.model);
    CHECK(main.Read<uint16_t>() == 25u);
    CHECK(main.Read<uint8_t>() == 1u);
    for (auto axis = 0u; axis < 3u; axis++)
        CHECK(main.Read<float>() == -5.0f);
    for (auto axis = 0u; axis < 3u; axis++)
        CHECK(main.Read<float>() == 5.0f);
    CHECK(main.ReadString().empty());
    CHECK(main.Read<float>() == 0.0f);
    CHECK(main.ReadString() == "test_model_lod0");
    for (auto lod = 1u; lod < 4u; lod++)
    {
        CHECK(main.Read<float>() == 0.0f);
        CHECK(main.ReadString().empty());
    }
    CHECK(main.Read<int32_t>() == -1);
    CHECK(main.Read<int32_t>() == 0);
    CHECK(main.Read<uint16_t>() == 1u);
    CHECK(main.ReadString() == "test_material");
}
