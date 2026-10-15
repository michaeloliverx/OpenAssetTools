#include "Game/IW4Xenon/IW4Xenon.h"
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
#include <fstream>
#include <sstream>
#include <string>

#ifdef ARCH_x86

namespace
{
    std::unique_ptr<Zone> RoundTripZone(const Zone& zone)
    {
        const auto path = oat::paths::GetTempDirectory("IW4XenonRoundTrip") / (zone.m_name + ".ff");
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
        REQUIRE(static_cast<unsigned char>(header[11]) == 0x0d);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::IW4Xenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("IW4 Xenon zone round trip preserves assets and emits Xenon asset types", "[iw4xenon][system][zone]")
    {
        Zone zone("assets", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        IW4Xenon::RawFile rawFile{};
        rawFile.name = "test.txt";
        rawFile.data.buffer = "zone round trip";
        rawFile.len = 15;
        zone.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_RAWFILE, rawFile.name, &rawFile, {}, {}, {});

        IW4Xenon::PhysPreset physPreset{};
        physPreset.name = "test_phys";
        physPreset.type = 0x12345678;
        physPreset.mass = 12.5f;
        physPreset.bounce = 0.25f;
        zone.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_PHYSPRESET, physPreset.name, &physPreset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 2);
        const auto* loadedRawFile = loaded->m_pools.GetAsset<IW4Xenon::AssetRawFile>(rawFile.name);
        REQUIRE(loadedRawFile);
        REQUIRE(loadedRawFile->Asset()->len == rawFile.len);
        REQUIRE(std::string(loadedRawFile->Asset()->data.buffer) == rawFile.data.buffer);
        const auto* loadedPhysPreset = loaded->m_pools.GetAsset<IW4Xenon::AssetPhysPreset>(physPreset.name);
        REQUIRE(loadedPhysPreset);
        REQUIRE(loadedPhysPreset->Asset()->type == physPreset.type);
        REQUIRE(loadedPhysPreset->Asset()->mass == physPreset.mass);
        REQUIRE(loadedPhysPreset->Asset()->bounce == physPreset.bounce);
        REQUIRE(physPreset.type == 0x12345678);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(loaded->m_game_id)->WriteZoneDef(definition, *loaded, false, false);
        REQUIRE(definition.str().find(">game,IW4\n>platform,xbox360\n") != std::string::npos);
        REQUIRE(definition.str().find("rawfile,test.txt\n") != std::string::npos);
        REQUIRE(definition.str().find("physpreset,test_phys\n") != std::string::npos);
    }

    TEST_CASE("IW4 Xenon zone round trip preserves animation arrays and script strings", "[iw4xenon][system][zone]")
    {
        const auto numFrames = GENERATE(30, 300);
        Zone zone("animation", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        IW4Xenon::ScriptString names[]{zone.m_script_strings.AddOrGetScriptString("tag_origin")};
        IW4Xenon::XAnimNotifyInfo notify{};
        notify.name = zone.m_script_strings.AddOrGetScriptString("fire");
        notify.time = 0.5f;
        int16_t data[]{0x1234, -1234};
        uint8_t indices8[]{1, 20};
        uint16_t indices16[]{1, 299};

        IW4Xenon::XAnimParts animation{};
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
        zone.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_XANIMPARTS, animation.name, &animation, {}, {names[0], notify.name}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* loadedAsset = loaded->m_pools.GetAsset<IW4Xenon::AssetXAnim>(animation.name);
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

    TEST_CASE("IW4 Xenon zone round trip preserves image data and external stream records", "[iw4xenon][system][zone]")
    {
        Zone zone("images", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        uint8_t pixels[]{1, 2, 3, 4};
        IW4Xenon::GfxImage images[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& image = images[i];
            image.name = i == 0 ? "immediate" : "delayed";
            image.mapType = IW4Xenon::MAPTYPE_2D;
            image.width = 1;
            image.height = 1;
            image.depth = 1;
            image.cardMemory.platform[0] = sizeof(pixels);
            image.pixels = pixels;
            image.cached = i != 0;
            zone.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
            if (image.cached)
                zone.m_xfile_image_stream_files_by_name[image.name].push_back({
                    XAssetStreamFile{2, 4096, 8192},
                    {},
                    {},
                    {}
                });
        }

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_xfile_image_stream_files.size() == 4);
        REQUIRE(loaded->m_xfile_image_stream_files[0].fileIndex == 2);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offset == 4096);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offsetEnd == 8192);
        for (const auto& image : images)
        {
            const auto* loadedAsset = loaded->m_pools.GetAsset<IW4Xenon::AssetImage>(image.name);
            REQUIRE(loadedAsset);
            const auto& result = *loadedAsset->Asset();
            REQUIRE(result.cached == image.cached);
            REQUIRE(result.cardMemory.platform[0] == sizeof(pixels));
            REQUIRE(result.width == image.width);
            for (auto i = 0u; i < sizeof(pixels); i++)
                REQUIRE(result.pixels[i] == pixels[i]);
        }
    }

    TEST_CASE("IW4 Xenon zone round trip preserves reusable material data", "[iw4xenon][system][zone]")
    {
        Zone zone("materials", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        IW4Xenon::MaterialConstantDef constant{};
        constant.nameHash = 0x12345678;
        constant.literal[0] = 12.5f;
        IW4Xenon::Material materials[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& material = materials[i];
            material.info.name = i == 0 ? "first" : "second";
            material.constantCount = 1;
            material.constantTable = &constant;
            zone.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});
        }

        const auto loaded = RoundTripZone(zone);
        const auto* first = loaded->m_pools.GetAsset<IW4Xenon::AssetMaterial>("first");
        const auto* second = loaded->m_pools.GetAsset<IW4Xenon::AssetMaterial>("second");
        REQUIRE(first);
        REQUIRE(second);
        REQUIRE(first->Asset()->constantTable == second->Asset()->constantTable);
        REQUIRE(first->Asset()->constantTable->nameHash == constant.nameHash);
        REQUIRE(first->Asset()->constantTable->literal[0] == constant.literal[0]);
    }

    TEST_CASE("IW4 Xenon imports in-memory assets with nested image streams", "[iw4xenon][system][zone]")
    {
        Zone source("source", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        IW4Xenon::GfxImage image{};
        image.name = "nested_image";
        image.cached = true;
        IW4Xenon::MaterialTextureDef texture{};
        texture.semantic = IW4Xenon::TS_COLOR_MAP;
        texture.u.image = &image;
        IW4Xenon::Material material{};
        material.info.name = "nested_material";
        material.textureCount = 1;
        material.textureTable = &texture;
        source.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        source.m_pools.AddAsset(IW4Xenon::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});
        source.m_xfile_image_stream_files_by_name[image.name].push_back({
            XAssetStreamFile{3, 4096, 8192},
            {},
            {},
            {}
        });
        source.Register();

        Zone target("imported", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        MockSearchPath searchPath;
        GdtLookup gdt;
        AssetCreatorCollection creators(target);
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon)->ConfigureCreatorCollection(creators, target, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        REQUIRE(context.LoadDependency<IW4Xenon::AssetMaterial>(material.info.name));
        REQUIRE(target.m_pools.GetAsset<IW4Xenon::AssetImage>(image.name));

        const auto loaded = RoundTripZone(target);
        REQUIRE(loaded->m_xfile_image_stream_files.size() == 4);
        REQUIRE(loaded->m_xfile_image_stream_files[0].fileIndex == 3);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offset == 4096);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offsetEnd == 8192);
        const auto* result = loaded->m_pools.GetAsset<IW4Xenon::AssetMaterial>(material.info.name);
        REQUIRE(result);
        REQUIRE(std::string(result->Asset()->textureTable[0].u.image->name) == image.name);
    }
} // namespace

#endif
