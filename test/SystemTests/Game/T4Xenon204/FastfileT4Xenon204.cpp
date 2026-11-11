#include "Game/GameTarget.h"
#include "Game/T4Xenon204/AssetEndianSwapT4Xenon204.h"
#include "Game/T4Xenon204/ZoneLoaderFactoryT4Xenon204.h"
#include "Loading/ILoadingStep.h"
#include "Loading/ZoneDataPeeking.h"
#include "Pool/XAssetInfo.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <zlib.h>

namespace
{
    void AppendU32(std::string& bytes, const uint32_t value)
    {
        for (auto shift = 24; shift >= 0; shift -= 8)
            bytes.push_back(static_cast<char>(value >> shift));
    }

    void PutU32(std::string& bytes, const size_t offset, const uint32_t value)
    {
        std::string encoded;
        AppendU32(encoded, value);
        bytes.replace(offset, encoded.size(), encoded);
    }

    TEST_CASE("T4 Xenon 204 identifies its version and asset numbering", "[t4xenon204][system]")
    {
        const T4Xenon204::ZoneLoaderFactory factory;
        for (const auto version : {204u, 269u, 302u, 358u})
        {
            CAPTURE(version);
            std::string header;
            AppendU32(header, version);
            std::istringstream input(header, std::ios::binary);
            ZoneDataPeeking peek(input);
            const auto inspected = factory.InspectZoneHeader(peek);
            REQUIRE(inspected.has_value() == (version == 204));
            if (inspected)
            {
                CHECK(inspected->m_game_id == GameId::T4Xenon204);
                CHECK(inspected->m_platform == GamePlatform::XBOX);
                CHECK(inspected->m_endianness == GameEndianness::BE);
                CHECK(inspected->m_word_size == GameWordSize::ARCH_32);
            }
        }
        CHECK(game_target::Resolve(GameId::T4Xenon204, GamePlatform::XBOX) == GameId::T4Xenon204);
        CHECK_FALSE(game_target::Resolve(GameId::T4Xenon204, GamePlatform::PC));
        const auto* game = IGame::GetGameById(GameId::T4Xenon204);
        CHECK(game->GetShortName() == "T4Xenon204");
        CHECK(game->FindAssetTypeByName("clipmap") == 7);
        CHECK(game->FindAssetTypeByName("gfxworld") == 9);
        CHECK(game->FindAssetTypeByName("weapon") == 16);
        CHECK(game->FindAssetTypeByName("rawfile") == 24);
        CHECK_FALSE(game->FindAssetTypeByName("pixelshader"));
    }

#ifdef ARCH_x86
    class CheckEndOfPayload final : public ILoadingStep
    {
        void PerformStep(ZoneLoader&, ILoadingStream& stream) override
        {
            char trailingByte;
            REQUIRE(stream.Load(&trailingByte, 1) == 0);
        }
    };

    std::unique_ptr<Zone> LoadChecked(std::istream& input)
    {
        ZoneDataPeeking peek(input);
        const T4Xenon204::ZoneLoaderFactory factory;
        auto loader = factory.CreateLoaderForHeader(peek, "descent", std::nullopt);
        REQUIRE(loader);
        loader->AddLoadingStep(std::make_unique<CheckEndOfPayload>());
        input.seekg(0);
        auto zone = loader->LoadZone(input);
        REQUIRE(zone);
        return zone;
    }

    std::string MakeFastfile(const std::string& payload, const uint32_t tempSize)
    {
        std::string compressed(compressBound(static_cast<uLong>(payload.size())), '\0');
        auto compressedSize = static_cast<uLongf>(compressed.size());
        REQUIRE(compress2(reinterpret_cast<Bytef*>(compressed.data()),
                          &compressedSize,
                          reinterpret_cast<const Bytef*>(payload.data()),
                          static_cast<uLong>(payload.size()),
                          Z_BEST_SPEED)
                == Z_OK);
        compressed.resize(compressedSize);
        std::string fastfile;
        for (const auto value : {204u, static_cast<uint32_t>(payload.size()), tempSize, 0u, 4096u, 0u, 0u})
            AppendU32(fastfile, value);
        return fastfile + compressed;
    }

    TEST_CASE("T4 Xenon 204 retains the layouts recovered from Descent", "[t4xenon204][system]")
    {
        using namespace T4Xenon204;
        CHECK(sizeof(XModel) == 256);
        CHECK(sizeof(XModelLodInfo) == 40);
        CHECK(offsetof(XModel, materials) == 244);
        CHECK(sizeof(XSurface) == 104);
        CHECK(offsetof(XSurface, vertCount) == 4);
        CHECK(offsetof(XSurface, triCount) == 6);
        CHECK(sizeof(Material) == 96);
        CHECK(offsetof(Material, textureCount) == 70);
        CHECK(sizeof(MaterialTechniqueSet) == 148);
        CHECK(offsetof(MaterialTechniqueSet, techniques) == 8);
        CHECK(sizeof(MaterialPass) == 20);
        CHECK(sizeof(MaterialVertexDeclaration) == 32);
        CHECK(sizeof(snd_alias_t) == 76);
        CHECK(sizeof(clipMap_t) == 292);
        CHECK(offsetof(clipMap_t, path) == 236);
        CHECK(sizeof(GfxWorld) == 560);
        CHECK(offsetof(GfxWorld, lightGrid) == 292);
        CHECK(sizeof(GfxSurface) == 68);
        CHECK(offsetof(GfxSurface, material) == 36);
        CHECK(sizeof(GfxStaticModelDrawInst) == 84);
        CHECK(sizeof(GfxLightmapArray) == 12);
        CHECK(sizeof(GfxLightGridEntry) == 8);
        CHECK(sizeof(GfxLightGridColors) == 24);
        CHECK(sizeof(FxElemDef) == 232);
        CHECK(sizeof(WeaponDef) == 1652);
        CHECK(offsetof(WeaponDef, aiVsAiAccuracyGraphName) == 0x56c);
    }

    TEST_CASE("T4 Xenon 204 resolves inserted two-bit aliases and preserves unknown material bytes", "[t4xenon204][system]")
    {
        using namespace T4Xenon204;
        constexpr uint32_t FOLLOWING = 0xffffffff;
        std::string payload;
        for (const auto value : {2u, FOLLOWING, 4u, FOLLOWING, 0u, FOLLOWING})
            AppendU32(payload, value);
        payload.append("tag", 4);
        for (const auto value : {24u, 0xfffffffeu, 15u, FOLLOWING, 3u, FOLLOWING, 2u, FOLLOWING})
            AppendU32(payload, value);
        for (const auto value : {FOLLOWING, 5u, FOLLOWING})
            AppendU32(payload, value);
        // Virtual offset 48: script strings (12), asset table (32), insertion (4).
        payload.append("test_shared", 12);
        payload.append("hello", 6);
        AppendU32(payload, FOLLOWING);
        AppendU32(payload, 0x80000031);
        payload.append("translation", 12);
        AppendU32(payload, FOLLOWING);
        payload.append(144, '\0');
        payload.append("tech", 5);
        std::string material(96, '\0');
        PutU32(material, 0, FOLLOWING);
        material[73] = static_cast<char>(0xa5);
        material[92] = static_cast<char>(0x5a);
        payload += material;
        payload.append("mat", 4);
        const auto bytes = MakeFastfile(payload, 512);
        std::istringstream input(bytes, std::ios::binary);
        const auto zone = LoadChecked(input);
        CHECK(zone->m_script_strings.Count() == 2);
        CHECK(zone->m_pools.GetTotalAssetCount() == 4);
        const auto* raw = zone->m_pools.GetAsset<AssetRawFile>("test_shared");
        const auto* localize = zone->m_pools.GetAsset<AssetLocalize>("test_shared");
        REQUIRE(raw);
        REQUIRE(localize);
        CHECK(raw->Asset()->len == 5);
        CHECK(std::string(raw->Asset()->buffer, 6) == std::string("hello", 6));
        CHECK(localize->Asset()->name == raw->Asset()->name);
        CHECK(std::string(localize->Asset()->value) == "translation");
        REQUIRE(zone->m_pools.GetAsset<AssetTechniqueSet>("tech"));
        const auto* mat = zone->m_pools.GetAsset<AssetMaterial>("mat");
        REQUIRE(mat);
        CHECK(mat->Asset()->unknown_0x41[0] == 0xa5);
        CHECK(mat->Asset()->unknown_0x54[0] == 0x5a);
    }

    TEST_CASE("T4 Xenon 204 handles nested weapon scratch and a two-LOD material table", "[t4xenon204][system]")
    {
        using namespace T4Xenon204;
        constexpr uint32_t FOLLOWING = 0xffffffff;
        std::string payload;
        for (const auto value : {0u, 0u, 1u, FOLLOWING, 16u, FOLLOWING})
            AppendU32(payload, value);
        std::string weapon(1652, '\0');
        PutU32(weapon, 0, FOLLOWING);
        PutU32(weapon, 12, FOLLOWING);
        payload += weapon;
        payload.append("weapon", 7);
        std::string model(256, '\0');
        PutU32(model, 0, FOLLOWING);
        model[41] = model[81] = 1; // One surface per LOD.
        model[237] = 2;            // Two LODs, big-endian ushort.
        PutU32(model, 244, FOLLOWING);
        payload += model;
        payload.append("gun", 4);
        AppendU32(payload, FOLLOWING);
        AppendU32(payload, FOLLOWING);
        for (const auto* name : {"lod0", "lod1"})
        {
            AppendU32(payload, FOLLOWING);
            std::string material(96, '\0');
            PutU32(material, 0, FOLLOWING);
            payload += material;
            payload.append(name, 5);
        }
        const auto bytes = MakeFastfile(payload, 1652);
        std::istringstream input(bytes, std::ios::binary);
        const auto zone = LoadChecked(input);
        CHECK(zone->m_pools.GetTotalAssetCount() == 4);
        const auto* asset = zone->m_pools.GetAsset<AssetWeapon>("weapon");
        REQUIRE(asset);
        const auto* gun = asset->Asset()->gunXModel;
        REQUIRE(gun);
        CHECK(std::string(gun->name) == "gun");
        REQUIRE(gun->materials);
        CHECK(std::string(gun->materials->count2[0].lod0[0]->info.name) == "lod0");
        CHECK(std::string(gun->materials->count2[0].lod1[0]->info.name) == "lod1");
    }

    TEST_CASE("T4 Xenon 204 fully parses Descent and retains export geometry", "[.][t4xenon204][stock]")
    {
        using namespace T4Xenon204;
        const auto* path = std::getenv("OAT_T4_XENON_204_FF");
        if (!path)
            SKIP("Set OAT_T4_XENON_204_FF to Descent's version-204 fastfile");
        std::ifstream input(path, std::ios::binary);
        REQUIRE(input);
        const auto zone = LoadChecked(input);
        CHECK(zone->m_game_id == GameId::T4Xenon204);
        CHECK(zone->m_script_strings.Count() == 629);
        std::map<asset_type_t, size_t> counts;
        for (const auto* asset : zone->m_pools)
            ++counts[asset->m_type];
        CHECK(counts
              == std::map<asset_type_t, size_t>{
                  {0,  54  },
                  {1,  96  },
                  {2,  113 },
                  {3,  59  },
                  {4,  193 },
                  {5,  7164},
                  {6,  6   },
                  {7,  1   },
                  {9,  1   },
                  {15, 18  },
                  {16, 2   },
                  {18, 17  },
                  {24, 15  }
        });
        CHECK(zone->m_pools.GetTotalAssetCount() == 7739);
        const auto* clipAsset = zone->m_pools.GetAsset<AssetClipMapPvs>("maps/descent.d3dbsp");
        const auto* worldAsset = zone->m_pools.GetAsset<AssetGfxWorld>("maps/descent.d3dbsp");
        REQUIRE(clipAsset);
        REQUIRE(worldAsset);
        const auto& clip = *clipAsset->Asset();
        const auto& world = *worldAsset->Asset();
        CHECK(clip.numBrushes == 17587);
        CHECK(clip.numBrushSides == 52640);
        CHECK(clip.triCount == 47014);
        CHECK(clip.vertCount == 37358);
        CHECK(clip.numSubModels == 984);
        CHECK(clip.numStaticModels == 1745);
        REQUIRE(clip.entityString);
        CHECK(std::string_view(clip.entityString, clip.numEntityChars).find("\"classname\" \"worldspawn\"") != std::string_view::npos);
        CHECK(world.nodeCount == 6479);
        CHECK(world.surfaceCount == 5940);
        CHECK(world.vertexCount == 300767);
        CHECK(world.indexCount == 560334);
        CHECK(world.modelCount == 984);
        CHECK(world.smodelCount == 1751);
        CHECK(world.cellCount == 63);
        CHECK(world.lightGrid.entryCount == 57694);
        CHECK(world.lightGrid.colorCount == 29741);
        REQUIRE(world.models);
        CHECK(world.models[0].surfaceCount == 3573);
        REQUIRE(world.lightmaps);
        REQUIRE(world.lightmaps[0].primary);
        REQUIRE(world.lightmaps[0].secondary);
        CHECK(std::string(world.lightmaps[0].primary->name) == "*lightmap0_0");
        CHECK(std::string(world.lightmaps[0].secondary->name) == "*lightmap0_1");
        REQUIRE(world.lightmaps[0].unknownImage);
        CHECK(std::string(world.lightmaps[0].unknownImage->name) == "*lightmap0_2");
        CHECK(world.lightmaps[0].unknownImage->width == 2048);
        CHECK(world.lightmaps[0].unknownImage->height == 2048);
        REQUIRE(world.lightGrid.unknownImage);
        CHECK(world.lightGrid.unknownImage->depth == 2);
        REQUIRE(world.vd.vertices);
        REQUIRE(world.indices);
        REQUIRE(world.surfaces);
        REQUIRE(world.nodes);
        for (int i = 0; i < world.surfaceCount; ++i)
        {
            CAPTURE(i);
            const auto& surface = world.surfaces[i];
            REQUIRE(surface.material);
            const auto& tris = surface.tris;
            REQUIRE(tris.firstVertex >= 0);
            const auto base = reinterpret_cast<uintptr_t>(world.indices);
            const auto pointer = reinterpret_cast<uintptr_t>(tris.indices);
            REQUIRE(pointer >= base);
            REQUIRE((pointer - base) % 2 == 0);
            REQUIRE((pointer - base) / 2 + tris.indexCount <= static_cast<unsigned>(world.indexCount));
            REQUIRE(tris.indexCount % 3 == 0);
            for (unsigned j = 0; j < tris.indexCount; ++j)
                REQUIRE(static_cast<unsigned>(tris.firstVertex) + tris.indices[j] < world.vertexCount);
        }
        CHECK(std::all_of(world.vd.vertices,
                          world.vd.vertices + world.vertexCount,
                          [](const GfxWorldVertex& vertex)
                          {
                              return std::all_of(std::begin(vertex.xyz),
                                                 std::end(vertex.xyz),
                                                 [](const float coordinate)
                                                 {
                                                     return std::isfinite(coordinate);
                                                 });
                          }));
        const auto validNode = [&world](const GfxBspNode* node)
        {
            const auto begin = reinterpret_cast<uintptr_t>(world.nodes);
            const auto pointer = reinterpret_cast<uintptr_t>(node);
            return pointer >= begin && pointer < begin + sizeof(GfxBspNode) * world.nodeCount && (pointer - begin) % sizeof(GfxBspNode) == 0;
        };
        size_t branchCount = 0;
        for (int i = 0; i < world.nodeCount; ++i)
        {
            CAPTURE(i);
            const auto& node = world.nodes[i];
            CHECK((node.parent == nullptr || validNode(node.parent)));
            if (node.unknownNodeType == -1)
            {
                ++branchCount;
                REQUIRE(node.data.branch.plane);
                REQUIRE(validNode(node.data.branch.children[0]));
                REQUIRE(validNode(node.data.branch.children[1]));
            }
        }
        CHECK(branchCount == clip.numNodes);
    }
#endif
} // namespace
