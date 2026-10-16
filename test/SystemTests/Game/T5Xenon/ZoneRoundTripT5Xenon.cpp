#include "Game/T5Xenon/T5Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "Pool/XAssetInfo.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#ifdef ARCH_x86

namespace
{
    std::unique_ptr<Zone> RoundTripZone(const Zone& zone)
    {
        const auto path = oat::paths::GetTempDirectory("T5XenonRoundTrip") / (zone.m_name + ".ff");
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, zone));
        }

        std::ifstream input(path, std::ios::binary);
        char header[12]{};
        input.read(header, sizeof(header));
        REQUIRE(std::string(header, 8) == "IWffu100");
        REQUIRE(header[8] == 0);
        REQUIRE(header[9] == 0);
        REQUIRE(header[10] == 1);
        REQUIRE(static_cast<unsigned char>(header[11]) == 0xd9);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::T5Xenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("T5Xenon zone round trip preserves assets and emits Xenon asset types", "[t5xenon][system][zone]")
    {
        Zone zone("assets", 0, GameId::T5Xenon, GamePlatform::XBOX);
        T5Xenon::RawFile rawFile{};
        rawFile.name = "test.txt";
        rawFile.buffer = "zone round trip";
        rawFile.len = 15;
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_RAWFILE, rawFile.name, &rawFile, {}, {}, {});

        T5Xenon::PhysPreset physPreset{};
        physPreset.name = "test_phys";
        physPreset.flags = 0x12345678;
        physPreset.mass = 12.5f;
        physPreset.bounce = 0.25f;
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_PHYSPRESET, physPreset.name, &physPreset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 2);
        const auto* loadedRawFile = loaded->m_pools.GetAsset<T5Xenon::AssetRawFile>(rawFile.name);
        REQUIRE(loadedRawFile);
        REQUIRE(loadedRawFile->Asset()->len == rawFile.len);
        REQUIRE(std::string(loadedRawFile->Asset()->buffer) == rawFile.buffer);
        const auto* loadedPhysPreset = loaded->m_pools.GetAsset<T5Xenon::AssetPhysPreset>(physPreset.name);
        REQUIRE(loadedPhysPreset);
        REQUIRE(loadedPhysPreset->Asset()->flags == physPreset.flags);
        REQUIRE(loadedPhysPreset->Asset()->mass == physPreset.mass);
        REQUIRE(loadedPhysPreset->Asset()->bounce == physPreset.bounce);
        REQUIRE(physPreset.flags == 0x12345678);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(loaded->m_game_id)->WriteZoneDef(definition, *loaded, false, false);
        REQUIRE(definition.str().find(">game,T5\n>platform,xbox360\n") != std::string::npos);
        REQUIRE(definition.str().find("rawfile,test.txt\n") != std::string::npos);
        REQUIRE(definition.str().find("physpreset,test_phys\n") != std::string::npos);
    }

    TEST_CASE("T5 Xenon technique passes resolve shared pixel shaders through asset aliases", "[t5xenon][system][zone][pixelshader]")
    {
        Zone zone("shader_aliases", 0, GameId::T5Xenon, GamePlatform::XBOX);
        T5Xenon::MaterialPixelShader shader{};
        shader.name = "shared_pixel_shader";
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_PIXEL_SHADER, shader.name, &shader, {}, {}, {});
        T5Xenon::MaterialTechnique first{};
        first.name = "first";
        first.passCount = 1;
        first.passArray[0].pixelShader = &shader;
        auto second = first;
        second.name = "second";
        T5Xenon::MaterialTechniqueSet techset{};
        techset.name = "shared_shaders";
        techset.techniques[0] = &first;
        techset.techniques[1] = &second;
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_TECHNIQUE_SET, techset.name, &techset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* loadedSet = loaded->m_pools.GetAsset<T5Xenon::AssetTechniqueSet>(techset.name);
        const auto* loadedShader = loaded->m_pools.GetAsset<T5Xenon::AssetPixelShader>(shader.name);
        REQUIRE(loadedSet);
        REQUIRE(loadedShader);
        REQUIRE(loadedSet->Asset()->techniques[0]->passArray[0].pixelShader == loadedShader->Asset());
        REQUIRE(loadedSet->Asset()->techniques[1]->passArray[0].pixelShader == loadedShader->Asset());
        REQUIRE(std::string(loadedShader->Asset()->name) == shader.name);
    }

    TEST_CASE("T5 Xenon materials preserve named constants and render-state tables", "[t5xenon][system][zone][material]")
    {
        Zone zone("material_tables", 0, GameId::T5Xenon, GamePlatform::XBOX);
        T5Xenon::MaterialConstantDef constants[2]{};
        constants[0].nameHash = 0xb60c3b3a;
        std::memcpy(constants[0].name, "colorTint", 10);
        constants[0].literal[0] = 1.0f;
        constants[0].literal[1] = 0.5f;
        constants[1].nameHash = 0x3d9994dc;
        std::memcpy(constants[1].name, "envMapParms", 12);
        constants[1].literal[3] = -1.25f;
        T5Xenon::GfxStateBits states[3]{};
        states[0].loadBits[0] = 0x12345678;
        states[2].loadBits[1] = 0x90abcdef;
        T5Xenon::Material material{};
        material.info.name = "named_material";
        material.info.gameFlags = 0x1234;
        material.info.drawSurf[0] = 0x01234567;
        material.info.drawSurf[1] = 0x89abcdef;
        material.constantCount = 2;
        material.constantTable = constants;
        material.stateBitsCount = 3;
        material.stateBitsTable = states;
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<T5Xenon::AssetMaterial>(material.info.name);
        REQUIRE(asset);
        const auto* result = asset->Asset();
        REQUIRE(result->info.gameFlags == 0x1234);
        REQUIRE(result->info.drawSurf[0] == 0x01234567);
        REQUIRE(result->info.drawSurf[1] == 0x89abcdef);
        REQUIRE(result->constantCount == 2);
        REQUIRE(result->constantTable[0].nameHash == 0xb60c3b3a);
        REQUIRE(std::string(result->constantTable[0].name) == "colorTint");
        REQUIRE(result->constantTable[0].literal[0] == 1.0f);
        REQUIRE(result->constantTable[0].literal[1] == 0.5f);
        REQUIRE(result->constantTable[1].literal[3] == -1.25f);
        REQUIRE(result->stateBitsCount == 3);
        REQUIRE(result->stateBitsTable[0].loadBits[0] == 0x12345678);
        REQUIRE(result->stateBitsTable[2].loadBits[1] == 0x90abcdef);
        REQUIRE(constants[0].literal[0] == 1.0f);
        REQUIRE(states[2].loadBits[1] == 0x90abcdef);
    }

} // namespace

#endif
