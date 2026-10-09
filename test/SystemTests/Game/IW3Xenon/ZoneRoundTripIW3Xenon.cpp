#include "Game/IW3Xenon/IW3Xenon.h"
#include "OatTestPaths.h"
#include "Pool/XAssetInfo.h"
#include "Zone/Definition/ZoneDefWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <fstream>
#include <sstream>
#include <string>

#ifdef ARCH_x86

namespace
{
    std::unique_ptr<Zone> RoundTripZone(const Zone& zone)
    {
        const auto path = oat::paths::GetTempDirectory("IW3XenonRoundTrip") / (zone.m_name + ".ff");
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
        REQUIRE(header[10] == 0);
        REQUIRE(header[11] == 1);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::IW3Xenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("IW3 Xenon zone round trip preserves assets and emits Xenon asset types", "[iw3xenon][system][zone]")
    {
        Zone zone("assets", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        IW3Xenon::RawFile rawFile{};
        rawFile.name = "test.txt";
        rawFile.buffer = "zone round trip";
        rawFile.len = 15;
        zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_RAWFILE, rawFile.name, &rawFile, {}, {}, {});

        IW3Xenon::PhysPreset physPreset{};
        physPreset.name = "test_phys";
        physPreset.type = 0x12345678;
        physPreset.mass = 12.5f;
        physPreset.bounce = 0.25f;
        zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_PHYSPRESET, physPreset.name, &physPreset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 2);
        const auto* loadedRawFile = loaded->m_pools.GetAsset<IW3Xenon::AssetRawFile>(rawFile.name);
        REQUIRE(loadedRawFile);
        REQUIRE(loadedRawFile->Asset()->len == rawFile.len);
        REQUIRE(std::string(loadedRawFile->Asset()->buffer) == rawFile.buffer);
        const auto* loadedPhysPreset = loaded->m_pools.GetAsset<IW3Xenon::AssetPhysPreset>(physPreset.name);
        REQUIRE(loadedPhysPreset);
        REQUIRE(loadedPhysPreset->Asset()->type == physPreset.type);
        REQUIRE(loadedPhysPreset->Asset()->mass == physPreset.mass);
        REQUIRE(loadedPhysPreset->Asset()->bounce == physPreset.bounce);
        REQUIRE(physPreset.type == 0x12345678);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(loaded->m_game_id)->WriteZoneDef(definition, *loaded, false, false);
        REQUIRE(definition.str().find(">game,IW3\n>platform,xbox360\n") != std::string::npos);
        REQUIRE(definition.str().find("rawfile,test.txt\n") != std::string::npos);
        REQUIRE(definition.str().find("physpreset,test_phys\n") != std::string::npos);
    }

    TEST_CASE("IW3 Xenon zone round trip preserves animation arrays and script strings", "[iw3xenon][system][zone]")
    {
        const auto numFrames = GENERATE(30, 300);
        Zone zone("animation", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        IW3Xenon::ScriptString names[]{zone.m_script_strings.AddOrGetScriptString("tag_origin")};
        IW3Xenon::XAnimNotifyInfo notify{};
        notify.name = zone.m_script_strings.AddOrGetScriptString("fire");
        notify.time = 0.5f;
        int16_t data[]{0x1234, -1234};
        uint8_t indices8[]{1, 20};
        uint16_t indices16[]{1, 299};

        IW3Xenon::XAnimParts animation{};
        animation.name = "test_anim";
        animation.numframes = static_cast<uint16_t>(numFrames);
        animation.framerate = 30.0f;
        animation.boneCount[11] = 1;
        animation.names = names;
        animation.notifyCount = 1;
        animation.notify = &notify;
        animation.dataShortCount = 2;
        animation.dataShort = data;
        animation.indexCount = 2;
        if (numFrames < 256)
            animation.indices._1 = indices8;
        else
            animation.indices._2 = indices16;
        zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_XANIMPARTS, animation.name, &animation, {}, {names[0], notify.name}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* loadedAsset = loaded->m_pools.GetAsset<IW3Xenon::AssetXAnim>(animation.name);
        REQUIRE(loadedAsset);
        const auto& result = *loadedAsset->Asset();
        REQUIRE(result.numframes == numFrames);
        REQUIRE(result.framerate == animation.framerate);
        REQUIRE(result.dataShortCount == 2);
        REQUIRE(result.dataShort[0] == data[0]);
        REQUIRE(result.dataShort[1] == data[1]);
        REQUIRE(std::string(loaded->m_script_strings.CValue(result.names[0])) == "tag_origin");
        REQUIRE(std::string(loaded->m_script_strings.CValue(result.notify[0].name)) == "fire");
        REQUIRE(result.notify[0].time == notify.time);
        REQUIRE(result.indexCount == 2);
        if (numFrames < 256)
            REQUIRE(result.indices._1[1] == indices8[1]);
        else
            REQUIRE(result.indices._2[1] == indices16[1]);
        REQUIRE(animation.numframes == numFrames);
        REQUIRE(data[0] == 0x1234);
    }

    TEST_CASE("IW3 Xenon zone round trip preserves immediate and delayed image data", "[iw3xenon][system][zone]")
    {
        Zone zone("images", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        uint8_t pixels[]{1, 2, 3, 4};
        IW3Xenon::GfxImage images[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& image = images[i];
            image.name = i == 0 ? "immediate" : "delayed";
            image.mapType = IW3Xenon::MAPTYPE_2D;
            image.width = 1;
            image.height = 1;
            image.depth = 1;
            image.cardMemory.platform[0] = sizeof(pixels);
            image.pixels = pixels;
            image.delayLoadPixels = i != 0;
            zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        }

        const auto loaded = RoundTripZone(zone);
        for (const auto& image : images)
        {
            const auto* loadedAsset = loaded->m_pools.GetAsset<IW3Xenon::AssetImage>(image.name);
            REQUIRE(loadedAsset);
            const auto& result = *loadedAsset->Asset();
            REQUIRE(result.delayLoadPixels == image.delayLoadPixels);
            REQUIRE(result.cardMemory.platform[0] == sizeof(pixels));
            REQUIRE(result.width == image.width);
            for (auto i = 0u; i < sizeof(pixels); i++)
                REQUIRE(result.pixels[i] == pixels[i]);
        }
    }

    TEST_CASE("IW3 Xenon zone round trip preserves reusable material data", "[iw3xenon][system][zone]")
    {
        Zone zone("materials", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        IW3Xenon::MaterialConstantDef constant{};
        constant.nameHash = 0x12345678;
        constant.literal[0] = 12.5f;
        IW3Xenon::Material materials[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& material = materials[i];
            material.info.name = i == 0 ? "first" : "second";
            material.constantCount = 1;
            material.constantTable = &constant;
            zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});
        }

        const auto loaded = RoundTripZone(zone);
        const auto* first = loaded->m_pools.GetAsset<IW3Xenon::AssetMaterial>("first");
        const auto* second = loaded->m_pools.GetAsset<IW3Xenon::AssetMaterial>("second");
        REQUIRE(first);
        REQUIRE(second);
        REQUIRE(first->Asset()->constantTable == second->Asset()->constantTable);
        REQUIRE(first->Asset()->constantTable->nameHash == constant.nameHash);
        REQUIRE(first->Asset()->constantTable->literal[0] == constant.literal[0]);
    }
} // namespace

#endif
