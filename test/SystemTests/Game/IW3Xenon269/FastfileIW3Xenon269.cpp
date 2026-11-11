#include "Game/GameTarget.h"
#include "Game/IW3Xenon269/AssetEndianSwapIW3Xenon269.h"
#include "Game/IW3Xenon269/ZoneLoaderFactoryIW3Xenon269.h"
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

    TEST_CASE("IW3 Xenon 269 identifies its version and asset numbering", "[iw3xenon269][system]")
    {
        const IW3Xenon269::ZoneLoaderFactory factory;
        for (const auto version : {204u, 269u, 302u, 358u})
        {
            CAPTURE(version);
            std::string header;
            AppendU32(header, version);
            std::istringstream input(header, std::ios::binary);
            ZoneDataPeeking peek(input);
            const auto inspected = factory.InspectZoneHeader(peek);
            REQUIRE(inspected.has_value() == (version == 269));
            if (inspected)
            {
                CHECK(inspected->m_game_id == GameId::IW3Xenon269);
                CHECK(inspected->m_platform == GamePlatform::XBOX);
                CHECK(inspected->m_endianness == GameEndianness::BE);
                CHECK(inspected->m_word_size == GameWordSize::ARCH_32);
            }
        }
        CHECK(game_target::Resolve(GameId::IW3Xenon269, GamePlatform::XBOX) == GameId::IW3Xenon269);
        CHECK_FALSE(game_target::Resolve(GameId::IW3Xenon269, GamePlatform::PC));
        const auto* game = IGame::GetGameById(GameId::IW3Xenon269);
        CHECK(game->GetShortName() == "IW3Xenon269");
        CHECK(game->FindAssetTypeByName("clipmap") == 9);
        CHECK(game->FindAssetTypeByName("gfxworld") == 11);
        CHECK(game->FindAssetTypeByName("weapon") == 18);
        CHECK(game->FindAssetTypeByName("rawfile") == 26);
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
        const IW3Xenon269::ZoneLoaderFactory factory;
        auto loader = factory.CreateLoaderForHeader(peek, "embassy", std::nullopt);
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
        for (const auto value : {269u, static_cast<uint32_t>(payload.size()), tempSize, 0u, 4096u, 0u, 0u})
            AppendU32(fastfile, value);
        return fastfile + compressed;
    }

    TEST_CASE("IW3 Xenon 269 retains the layouts recovered from Embassy", "[iw3xenon269][system]")
    {
        using namespace IW3Xenon269;
        CHECK(sizeof(PhysPreset) == 40);
        CHECK(sizeof(XModel) == 264);
        CHECK(sizeof(XModelLodInfo) == 40);
        CHECK(offsetof(XModel, materials) == 244);
        CHECK(offsetof(XModelLodInfo, materialNames) == 12);
        CHECK(offsetof(XModelLodInfo, surfs) == 16);
        CHECK(sizeof(XSurface) == 104);
        CHECK(sizeof(Material) == 88);
        CHECK(offsetof(Material, textureCount) == 62);
        CHECK(sizeof(MaterialTechniqueSet) == 152);
        CHECK(sizeof(clipMap_t) == 300);
        CHECK(offsetof(clipMap_t, path) == 236);
        CHECK(sizeof(GfxWorld) == 620);
        CHECK(offsetof(GfxWorld, nodes) == 12);
        CHECK(offsetof(GfxWorld, lightGrid) == 364);
        CHECK(sizeof(GfxBspNode) == 24);
        CHECK(sizeof(GfxCell) == 48);
        CHECK(sizeof(GfxBrushModel) == 32);
        CHECK(sizeof(GfxLightmapArray) == 12);
        CHECK(sizeof(GfxLightGridEntry) == 8);
        CHECK(sizeof(GfxLight) == 56);
        CHECK(sizeof(WeaponDef) == 1856);
        CHECK(offsetof(WeaponDef, unknownMaterial_0x328) == 0x328);
        CHECK(offsetof(WeaponDef, aiVsAiAccuracyGraphName) == 0x628);
    }

    TEST_CASE("IW3 Xenon 269 resolves inserted two-bit aliases and preserves unknown material bytes", "[iw3xenon269][system]")
    {
        using namespace IW3Xenon269;
        constexpr uint32_t FOLLOWING = 0xffffffff;
        std::string payload;
        for (const auto value : {2u, FOLLOWING, 4u, FOLLOWING, 0u, FOLLOWING})
            AppendU32(payload, value);
        payload.append("tag", 4);
        for (const auto value : {26u, 0xfffffffeu, 17u, FOLLOWING, 5u, FOLLOWING, 4u, FOLLOWING})
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
        payload.append(148, '\0');
        payload.append("tech", 5);
        std::string material(88, '\0');
        PutU32(material, 0, FOLLOWING);
        material[65] = static_cast<char>(0xa5);
        material[84] = static_cast<char>(0x5a);
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

    TEST_CASE("IW3 Xenon 269 handles nested weapon scratch and a two-LOD material table", "[iw3xenon269][system]")
    {
        using namespace IW3Xenon269;
        constexpr uint32_t FOLLOWING = 0xffffffff;
        std::string payload;
        for (const auto value : {0u, 0u, 1u, FOLLOWING, 18u, FOLLOWING})
            AppendU32(payload, value);
        std::string weapon(1856, '\0');
        PutU32(weapon, 0, FOLLOWING);
        PutU32(weapon, 12, FOLLOWING);
        payload += weapon;
        payload.append("weapon", 7);
        std::string model(264, '\0');
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
            std::string material(88, '\0');
            PutU32(material, 0, FOLLOWING);
            payload += material;
            payload.append(name, 5);
        }
        const auto bytes = MakeFastfile(payload, 1856);
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

    TEST_CASE("IW3 Xenon 269 fully parses Embassy and retains export geometry", "[.][iw3xenon269][stock]")
    {
        using namespace IW3Xenon269;
        const auto* path = std::getenv("OAT_IW3_XENON_269_FF");
        if (!path)
            SKIP("Set OAT_IW3_XENON_269_FF to Embassy's version-269 fastfile");
        std::ifstream input(path, std::ios::binary);
        REQUIRE(input);
        const auto zone = LoadChecked(input);
        CHECK(zone->m_game_id == GameId::IW3Xenon269);
        CHECK(zone->m_script_strings.Count() == 628);
        std::map<asset_type_t, size_t> counts;
        for (const auto* asset : zone->m_pools)
            ++counts[asset->m_type];
        CHECK(counts
              == std::map<asset_type_t, size_t>{
                  {1,  1   },
                  {2,  208 },
                  {3,  73  },
                  {4,  122 },
                  {5,  32  },
                  {6,  207 },
                  {7,  4796},
                  {8,  6   },
                  {9,  1   },
                  {11, 1   },
                  {17, 46  },
                  {18, 15  },
                  {20, 59  },
                  {26, 40  }
        });
        CHECK(zone->m_pools.GetTotalAssetCount() == 5607);
        const auto* clipAsset = zone->m_pools.GetAsset<AssetClipMapPvs>("maps/embassy.d3dbsp");
        const auto* worldAsset = zone->m_pools.GetAsset<AssetGfxWorld>("maps/embassy.d3dbsp");
        REQUIRE(clipAsset);
        REQUIRE(worldAsset);
        const auto& clip = *clipAsset->Asset();
        const auto& world = *worldAsset->Asset();
        CHECK(clip.numBrushes == 5815);
        CHECK(clip.numBrushSides == 1513);
        CHECK(clip.triCount == 22036);
        CHECK(clip.vertCount == 13063);
        CHECK(clip.numSubModels == 217);
        CHECK(clip.numStaticModels == 188);
        REQUIRE(clip.entityString);
        CHECK(std::string_view(clip.entityString, clip.numEntityChars).find("\"classname\" \"worldspawn\"") != std::string_view::npos);
        CHECK(world.nodeCount == 9310);
        CHECK(world.surfaceCount == 1979);
        CHECK(world.vertexCount == 163175);
        CHECK(world.indexCount == 282651);
        CHECK(world.modelCount == 217);
        CHECK(world.smodelCount == 188);
        CHECK(world.cellCount == 3);
        CHECK(world.lightGrid.entryCount == 40723);
        CHECK(world.lightGrid.colorCount == 21183);
        REQUIRE(world.models);
        CHECK(world.models[0].surfaceCount == 1922);
        REQUIRE(world.lightmaps);
        REQUIRE(world.lightmaps[0].primary);
        REQUIRE(world.lightmaps[0].secondary);
        CHECK(std::string(world.lightmaps[0].primary->name) == "*lightmap0_indirect");
        CHECK(std::string(world.lightmaps[0].secondary->name) == "*lightmap0_sunvis");
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
            REQUIRE(static_cast<unsigned>(tris.firstVertex) + tris.vertexCount <= world.vertexCount);
            REQUIRE(tris.baseIndex >= 0);
            REQUIRE(tris.baseIndex + tris.indexCount <= world.indexCount);
            for (unsigned j = 0; j < tris.indexCount; ++j)
                REQUIRE(world.indices[tris.baseIndex + j] < tris.vertexCount);
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
