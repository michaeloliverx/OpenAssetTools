#include "Base64.h"
#include "Game/IW3/IW3.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

#ifdef ARCH_x86

using nlohmann::json;

namespace
{
    struct RestoreObjWritingConfiguration
    {
        ObjWriting::Configuration_t original{ObjWriting::Configuration};

        ~RestoreObjWritingConfiguration()
        {
            ObjWriting::Configuration = original;
        }
    };

    json MaterialDocument(const int waterSize)
    {
        auto document = json::parse(R"JSON({
            "_game": "iw3", "_type": "material", "_version": 1,
            "gameFlags": ["2", "CASTS_SHADOW"], "sortKey": 4,
            "textureAtlas": {"rows": 1, "columns": 2}, "surfaceTypeBits": 32768,
            "stateFlags": 57, "cameraRegion": "lit", "techniqueSet": "test_techset",
            "textures": [
                {"name": "colorMap", "image": "test_image", "semantic": "colorMap",
                 "samplerState": {"filter": "aniso4x", "mipMap": "linear", "clampU": true, "clampV": false, "clampW": true}},
                {"nameHash": 305419896, "nameStart": "u", "nameEnd": "r", "image": "test_image", "semantic": "waterMap",
                 "samplerState": {"filter": "linear", "mipMap": "nearest", "clampU": false, "clampV": true, "clampW": false}}
            ],
            "constants": [
                {"name": "envMapParms", "literal": [1.25, 2.5, -3.75, 4.0]},
                {"nameHash": 324508639, "nameFragment": "abcdefghijkl", "literal": [-5.0, 6.25, 7.5, 8.75]}
            ],
            "stateBits": [
                {"srcBlendRgb": "srcalpha", "dstBlendRgb": "invsrcalpha", "blendOpRgb": "add", "alphaTest": "lt128",
                 "cullFace": "back", "srcBlendAlpha": "one", "dstBlendAlpha": "zero", "blendOpAlpha": "max",
                 "colorWriteRgb": true, "colorWriteAlpha": false, "polymodeLine": true, "depthWrite": true,
                 "depthTest": "less_equal", "polygonOffset": "offsetShadowmap",
                 "stencilFront": {"pass": "replace", "fail": "zero", "zfail": "incrsat", "func": "equal"},
                 "stencilBack": {"pass": "decrsat", "fail": "invert", "zfail": "incr", "func": "always"}},
                {"srcBlendRgb": "one", "dstBlendRgb": "zero", "blendOpRgb": "disabled", "alphaTest": "disabled",
                 "cullFace": "front", "srcBlendAlpha": "one", "dstBlendAlpha": "zero", "blendOpAlpha": "disabled",
                 "colorWriteRgb": false, "colorWriteAlpha": true, "polymodeLine": false, "depthWrite": false,
                 "depthTest": "disabled", "polygonOffset": "offset0"}
            ]
        })JSON");
        std::vector<int8_t> entries(34);
        for (auto i = 0u; i < entries.size(); ++i)
            entries[i] = static_cast<int8_t>(i % 2);
        document["stateBitsEntry"] = entries;

        std::vector<IW3::complex_s> h0(waterSize * 2);
        std::vector<float> wTerm(h0.size());
        for (auto i = 0uz; i < h0.size(); ++i)
        {
            h0[i] = {static_cast<float>(i) + 0.25f, -static_cast<float>(i) - 0.5f};
            wTerm[i] = static_cast<float>(i) + 1.5f;
        }
        document["textures"][1]["water"] = {
            {"floatTime", 1.25f},
            {"m", waterSize},
            {"n", 2},
            {"h0", h0.empty() ? std::string() : base64::EncodeBase64(h0.data(), h0.size() * sizeof(IW3::complex_s))},
            {"wTerm", wTerm.empty() ? std::string() : base64::EncodeBase64(wTerm.data(), wTerm.size() * sizeof(float))},
            {"lx", 64.0f},
            {"lz", 32.0f},
            {"gravity", 9.0f},
            {"windvel", 3.0f},
            {"winddir", {0.5f, 0.75f}},
            {"amplitude", 2.0f},
            {"codeConstant", {1.0f, 2.0f, 3.0f, 4.0f}},
        };
        return document;
    }

    bool LoadMaterialDocument(Zone& zone, const json& document)
    {
        MockSearchPath searchPath;
        searchPath.AddFileData("materials/test_material.json", document.dump());
        GdtLookup gdt;
        AssetCreatorCollection creators(zone);
        IObjLoader::GetObjLoaderForGame(zone.m_game_id)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        const auto xenon = zone.m_game_id == GameId::IW3Xenon;
        ignoredAssets.m_ignored_asset_lookup.emplace("test_techset",
                                                     xenon ? static_cast<asset_type_t>(IW3Xenon::ASSET_TYPE_TECHNIQUE_SET) : IW3::ASSET_TYPE_TECHNIQUE_SET);
        ignoredAssets.m_ignored_asset_lookup.emplace("test_image", xenon ? static_cast<asset_type_t>(IW3Xenon::ASSET_TYPE_IMAGE) : IW3::ASSET_TYPE_IMAGE);
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        const auto materialType = xenon ? static_cast<asset_type_t>(IW3Xenon::ASSET_TYPE_MATERIAL) : IW3::ASSET_TYPE_MATERIAL;
        return creators.CreateAsset(materialType, "test_material", context).HasBeenSuccessful();
    }

    json DumpMaterialDocument(Zone& zone)
    {
        MockSearchPath searchPath;
        MockOutputPath output;
        const std::string basePath;
        AssetDumpingContext context(zone, basePath, output, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(zone.m_game_id)->DumpZone(context));
        const auto* file = output.GetMockedFile("materials/test_material.json");
        REQUIRE(file);
        return json::parse(file->AsString());
    }

    void CheckMaterial(const Zone& zone, const IW3::Material& pc)
    {
        const auto* asset = zone.m_pools.GetAsset<IW3Xenon::AssetMaterial>("test_material");
        REQUIRE(asset);
        const auto& material = *asset->Asset();
        CHECK(material.info.gameFlags == pc.info.gameFlags);
        CHECK(material.info.sortKey == pc.info.sortKey);
        CHECK(material.info.textureAtlasRowCount == pc.info.textureAtlasRowCount);
        CHECK(material.info.textureAtlasColumnCount == pc.info.textureAtlasColumnCount);
        CHECK(material.info.surfaceTypeBits == pc.info.surfaceTypeBits);
        CHECK(material.stateFlags == pc.stateFlags);
        CHECK(material.cameraRegion == pc.cameraRegion);
        REQUIRE(material.textureCount == pc.textureCount);
        REQUIRE(material.constantCount == pc.constantCount);
        REQUIRE(material.stateBitsCount == pc.stateBitsCount);
        for (auto i = 0u; i < 26u; ++i)
            CHECK(material.stateBitsEntry[i] == pc.stateBitsEntry[i < 14u ? i : i + 7u]);
        for (auto i = 0u; i < material.textureCount; ++i)
        {
            CHECK(material.textureTable[i].nameHash == pc.textureTable[i].nameHash);
            CHECK(material.textureTable[i].nameStart == pc.textureTable[i].nameStart);
            CHECK(material.textureTable[i].nameEnd == pc.textureTable[i].nameEnd);
            CHECK(material.textureTable[i].semantic == pc.textureTable[i].semantic);
            CHECK(std::memcmp(&material.textureTable[i].samplerState, &pc.textureTable[i].samplerState, 1u) == 0);
        }
        for (auto i = 0u; i < material.constantCount; ++i)
        {
            CHECK(material.constantTable[i].nameHash == pc.constantTable[i].nameHash);
            CHECK(std::memcmp(material.constantTable[i].name, pc.constantTable[i].name, 12u) == 0);
            for (auto component = 0u; component < 4u; ++component)
                CHECK(material.constantTable[i].literal.v[component] == pc.constantTable[i].literal.v[component]);
        }
        for (auto i = 0u; i < material.stateBitsCount; ++i)
            for (auto word = 0u; word < 2u; ++word)
                CHECK(material.stateBitsTable[i].loadBits.raw[word] == pc.stateBitsTable[i].loadBits.raw[word]);

        const auto* water = material.textureTable[1].u.water;
        const auto* pcWater = pc.textureTable[1].u.water;
        REQUIRE(water);
        REQUIRE(pcWater);
        CHECK(water->writable.floatTime == pcWater->writable.floatTime);
        CHECK(water->M == pcWater->M);
        CHECK(water->N == pcWater->N);
        CHECK(water->codeConstant[3] == pcWater->codeConstant[3]);
        for (auto i = 0; i < water->M * water->N; ++i)
        {
            REQUIRE(water->H0X);
            REQUIRE(water->H0Y);
            REQUIRE(water->wTerm);
            CHECK(water->H0X[i] == pcWater->H0[i].real);
            CHECK(water->H0Y[i] == pcWater->H0[i].imag);
            CHECK(water->wTerm[i] == pcWater->wTerm[i]);
        }
    }

    TEST_CASE("IW3 Xenon materials share PC JSON and preserve water and state bits through native fastfiles", "[iw3xenon][system][material]")
    {
        const auto waterSize = GENERATE(0, 2);
        const RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(std::max<unsigned>(IW3::ASSET_TYPE_COUNT, IW3Xenon::ASSET_TYPE_COUNT), false);
        ObjWriting::Configuration.AssetTypesToHandleBitfield[IW3::ASSET_TYPE_MATERIAL] = true;
        const auto source = MaterialDocument(waterSize);
        Zone pcZone("pc_material", 0, GameId::IW3, GamePlatform::PC);
        REQUIRE(LoadMaterialDocument(pcZone, source));
        const auto* pcAsset = pcZone.m_pools.GetAsset<IW3::AssetMaterial>("test_material");
        REQUIRE(pcAsset);
        Zone zone("xenon_material", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        REQUIRE(LoadMaterialDocument(zone, source));
        CheckMaterial(zone, *pcAsset->Asset());

        const auto path = oat::paths::GetTempDirectory("IW3XenonMaterialRoundTrip") / "material.ff";
        {
            std::ofstream fastfile(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(fastfile, zone));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        CheckMaterial(**loaded, *pcAsset->Asset());
        const auto dumped = DumpMaterialDocument(**loaded);
        CHECK(dumped["_game"] == "iw3");
        REQUIRE(dumped["stateBitsEntry"].size() == 34u);
        for (auto i = 14u; i <= 20u; ++i)
            CHECK(dumped["stateBitsEntry"][i] == -1);
        CHECK(dumped["stateBitsEntry"][33] == -1);
        CHECK(dumped["textures"][1]["nameHash"] == source["textures"][1]["nameHash"]);
        CHECK(dumped["constants"][1]["nameHash"] == source["constants"][1]["nameHash"]);
        CHECK(dumped["textures"][1]["water"] == source["textures"][1]["water"]);

        Zone pcReloaded("pc_reloaded_material", 0, GameId::IW3, GamePlatform::PC);
        REQUIRE(LoadMaterialDocument(pcReloaded, dumped));
        CHECK(DumpMaterialDocument(pcReloaded) == dumped);
        Zone relinked("xenon_relinked_material", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        REQUIRE(LoadMaterialDocument(relinked, dumped));
        CheckMaterial(relinked, *pcAsset->Asset());
    }

    TEST_CASE("IW3 Xenon material loading rejects invalid water data and technique counts", "[iw3xenon][system][material]")
    {
        auto document = MaterialDocument(2);
        SECTION("wrong technique count")
        {
            document["stateBitsEntry"] = std::vector<int8_t>(26, -1);
        }
        SECTION("negative water dimensions")
        {
            document["textures"][1]["water"]["m"] = -1;
        }
        SECTION("water dimensions overflow")
        {
            document["textures"][1]["water"]["m"] = 2147483647;
        }
        SECTION("missing complex water samples")
        {
            document["textures"][1]["water"]["h0"] = "";
        }
        Zone zone("invalid_material", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        CHECK_FALSE(LoadMaterialDocument(zone, document));
    }
} // namespace

#endif
