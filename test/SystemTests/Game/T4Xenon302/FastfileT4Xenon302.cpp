#include "Game/GameTarget.h"
#include "Game/T4Xenon302/AssetEndianSwapT4Xenon302.h"
#include "Game/T4Xenon302/ZoneLoaderFactoryT4Xenon302.h"
#include "Loading/ILoadingStep.h"
#include "Loading/ZoneDataPeeking.h"
#include "Pool/XAssetInfo.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include <zlib.h>

namespace
{
    void AppendU32(std::string& bytes, const uint32_t value)
    {
        for (auto shift = 24; shift >= 0; shift -= 8)
            bytes.push_back(static_cast<char>(value >> shift));
    }

    TEST_CASE("T4 Xenon 302 identifies only its own fastfile version", "[t4xenon302][system]")
    {
        const T4Xenon302::ZoneLoaderFactory factory;
        for (const auto version : {204u, 302u, 358u})
        {
            CAPTURE(version);
            std::string header;
            AppendU32(header, version);
            std::istringstream input(header, std::ios::binary);
            ZoneDataPeeking peek(input);
            const auto inspected = factory.InspectZoneHeader(peek);
            REQUIRE(inspected.has_value() == (version == 302));
            if (inspected)
            {
                CHECK(inspected->m_game_id == GameId::T4Xenon302);
                CHECK(inspected->m_platform == GamePlatform::XBOX);
                CHECK(inspected->m_endianness == GameEndianness::BE);
                CHECK(inspected->m_word_size == GameWordSize::ARCH_32);
            }
        }
        CHECK(game_target::Resolve(GameId::T4Xenon302, GamePlatform::XBOX) == GameId::T4Xenon302);
        CHECK_FALSE(game_target::Resolve(GameId::T4Xenon302, GamePlatform::PC));
        const auto* game = IGame::GetGameById(GameId::T4Xenon302);
        CHECK(game->FindAssetTypeByName("techniqueset") == 5);
        CHECK(game->FindAssetTypeByName("rawfile") == 27);
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
        const T4Xenon302::ZoneLoaderFactory factory;
        auto loader = factory.CreateLoaderForHeader(peek, "test", std::nullopt);
        REQUIRE(loader);
        loader->AddLoadingStep(std::make_unique<CheckEndOfPayload>());
        input.seekg(0);
        auto zone = loader->LoadZone(input);
        REQUIRE(zone);
        return zone;
    }

    TEST_CASE("T4 Xenon 302 preserves its executable's structure layouts", "[t4xenon302][system]")
    {
        using namespace T4Xenon302;
        CHECK(sizeof(XModel) == 260);
        CHECK(sizeof(XModelLodInfo) == 40);
        CHECK(sizeof(XSurface) == 104);
        CHECK(sizeof(PhysGeomInfo) == 104);
        CHECK(sizeof(Material) == 88);
        CHECK(sizeof(MaterialPass) == 88);
        CHECK(sizeof(MaterialVertexDeclaration) == 72);
        CHECK(alignof(D3DVertexDeclaration) == 4);
        CHECK(sizeof(MaterialTechniqueSet) == 164);
        CHECK(sizeof(snd_alias_t) == 80);
        CHECK(sizeof(clipMap_t) == 332);
        CHECK(sizeof(pathnode_t) == 124);
        CHECK(sizeof(DynEntityClient) == 36);
        CHECK(alignof(DynEntityClient) == 4);
        CHECK(sizeof(GfxWorld) == 752);
        CHECK(sizeof(GfxStaticModelDrawInst) == 76);
        CHECK(sizeof(GfxLight) == 60);
        CHECK(sizeof(listBoxDef_s) == 256);
        CHECK(sizeof(FxTrailVertex) == 12);
        CHECK(sizeof(WeaponDef) == 1888);
        CHECK(offsetof(XModel, lodInfo) == 32);
        CHECK(offsetof(XModelLodInfo, materialHandles) == 36);
        CHECK(offsetof(Material, techniqueSet) == 68);
        CHECK(offsetof(clipMap_t, path) == 244);
        CHECK(offsetof(GfxWorld, lightGrid) == 384);
        CHECK(offsetof(listBoxDef_s, doubleClick) == 244);
    }

    TEST_CASE("T4 Xenon 302 loads two-bit offsets, inserted pointers and unknown bytes", "[t4xenon302][system]")
    {
        using namespace T4Xenon302;
        constexpr uint32_t FOLLOWING = 0xffffffff;
        std::string payload;
        // Script-string list, followed by a four-entry asset list.
        for (const auto value : {2u, FOLLOWING, 4u, FOLLOWING, 0u, FOLLOWING})
            AppendU32(payload, value);
        payload.append("tag", 4);
        for (const auto value : {27u, 0xfffffffeu, 18u, FOLLOWING, 5u, FOLLOWING, 4u, FOLLOWING})
            AppendU32(payload, value);
        for (const auto value : {FOLLOWING, 5u, FOLLOWING})
            AppendU32(payload, value);
        // Virtual offset 48: 8 string-pointer bytes + 4 string bytes + 32 asset
        // bytes + 4 bytes reserved by the inserted rawfile pointer.
        payload.append("test_shared", 12);
        payload.append("hello", 6);
        AppendU32(payload, FOLLOWING);
        AppendU32(payload, 0x80000031);
        payload.append("translation", 12);
        AppendU32(payload, FOLLOWING);
        payload.append(160, '\0');
        payload.append("tech", 5);
        std::array<char, 88> material{};
        material[0] = material[1] = material[2] = material[3] = static_cast<char>(0xff);
        material[62] = static_cast<char>(0xa5);
        material[84] = static_cast<char>(0x5a);
        payload.append(material.data(), material.size());
        payload.append("mat", 4);

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
        for (const auto value : {302u, static_cast<uint32_t>(payload.size()), 512u, 0u, 512u, 0u, 0u})
            AppendU32(fastfile, value);
        fastfile += compressed;
        std::istringstream input(fastfile, std::ios::binary);
        const auto zone = LoadChecked(input);
        CHECK(zone->m_script_strings.Count() == 2);
        REQUIRE(zone->m_pools.GetTotalAssetCount() == 4);
        const auto* raw = zone->m_pools.GetAsset<AssetRawFile>("test_shared");
        REQUIRE(raw);
        CHECK(raw->Asset()->len == 5);
        CHECK(std::string(raw->Asset()->buffer, 6) == std::string("hello", 6));
        const auto* localize = zone->m_pools.GetAsset<AssetLocalize>("test_shared");
        REQUIRE(localize);
        CHECK(localize->Asset()->name == raw->Asset()->name);
        CHECK(std::string(localize->Asset()->value) == "translation");
        REQUIRE(zone->m_pools.GetAsset<AssetTechniqueSet>("tech"));
        const auto* loadedMaterial = zone->m_pools.GetAsset<AssetMaterial>("mat");
        REQUIRE(loadedMaterial);
        CHECK(loadedMaterial->Asset()->unknown_0x3e[0] == 0xa5);
        CHECK(loadedMaterial->Asset()->unknown_0x54[0] == 0x5a);
    }

    TEST_CASE("T4 Xenon 302 decodes its shader argument discriminators", "[t4xenon302][system]")
    {
        using namespace T4Xenon302;
        for (const auto type : {0u, 1u})
        {
            MaterialShaderArgument argument{};
            argument.type = static_cast<unsigned short>(type);
            argument.dest = 0x1234;
            argument.u.codeConst.index = 0x5678;
            argument.u.codeConst.firstRow = 2;
            argument.u.codeConst.rowCount = 3;
            const auto original = argument;
            EndianSwap(argument, EndianOperation::Encode);
            CHECK(argument.u.codeConst.index == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
            CHECK(argument.u.codeConst.firstRow == 2);
            CHECK(argument.u.codeConst.rowCount == 3);
            EndianSwap(argument, EndianOperation::Decode);
            CHECK(std::memcmp(&argument, &original, sizeof(argument)) == 0);
        }
        for (const auto type : {6u, 7u})
        {
            MaterialShaderArgument argument{};
            argument.type = static_cast<unsigned short>(type);
            argument.u.literalConst = reinterpret_cast<float (*)[4]>(0x80000031u);
            EndianSwap(argument, EndianOperation::Encode);
            CHECK(reinterpret_cast<uintptr_t>(argument.u.literalConst) == 0x31000080u);
            EndianSwap(argument, EndianOperation::Decode);
            CHECK(argument.type == type);
            CHECK(reinterpret_cast<uintptr_t>(argument.u.literalConst) == 0x80000031u);
        }
    }

    TEST_CASE("T4 Xenon 302 fully consumes the stock corpus", "[.][t4xenon302][stock]")
    {
        const auto* directory = std::getenv("OAT_T4_XENON_302_DIR");
        if (!directory)
            SKIP("Set OAT_T4_XENON_302_DIR to the March 27, 2007 fastfile directory");
        size_t loadedFiles = 0;
        std::set<asset_type_t> seenTypes;
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.path().extension() != ".ff")
                continue;
            CAPTURE(entry.path().filename().string());
            std::ifstream input(entry.path(), std::ios::binary);
            ZoneDataPeeking peek(input);
            if (!T4Xenon302::ZoneLoaderFactory().InspectZoneHeader(peek))
                continue;
            input.seekg(0);
            const auto zone = LoadChecked(input);
            CHECK(zone->m_game_id == GameId::T4Xenon302);
            for (const auto* asset : zone->m_pools)
                seenTypes.insert(asset->m_type);
            ++loadedFiles;
        }
        CHECK(loadedFiles > 0);
        // The two serialized clipmap IDs share the clipmap pool. Menus also
        // appear as dependencies of menu lists.
        CHECK(seenTypes == std::set<asset_type_t>{1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 27});
    }
#endif
} // namespace
