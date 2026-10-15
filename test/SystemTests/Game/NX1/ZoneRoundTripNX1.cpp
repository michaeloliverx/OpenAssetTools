#include "Game/NX1/NX1.h"
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
        const auto path = oat::paths::GetTempDirectory("NX1RoundTrip") / (zone.m_name + ".ff");
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, zone));
        }

        std::ifstream input(path, std::ios::binary);
        char header[12]{};
        input.read(header, sizeof(header));
        REQUIRE(std::string(header, 8) == "NXffu100");
        REQUIRE(header[8] == 0);
        REQUIRE(header[9] == 0);
        REQUIRE(header[10] == 1);
        REQUIRE(static_cast<unsigned char>(header[11]) == 0x65);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::NX1);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("NX1 zone round trip preserves assets and emits Xenon asset types", "[nx1][system][zone]")
    {
        Zone zone("assets", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::RawFile rawFile{};
        rawFile.name = "test.txt";
        rawFile.buffer = "zone round trip";
        rawFile.len = 15;
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_RAWFILE, rawFile.name, &rawFile, {}, {}, {});

        NX1::PhysPreset physPreset{};
        physPreset.name = "test_phys";
        physPreset.type = 0x12345678;
        physPreset.mass = 12.5f;
        physPreset.bounce = 0.25f;
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_PHYSPRESET, physPreset.name, &physPreset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 2);
        const auto* loadedRawFile = loaded->m_pools.GetAsset<NX1::AssetRawFile>(rawFile.name);
        REQUIRE(loadedRawFile);
        REQUIRE(loadedRawFile->Asset()->len == rawFile.len);
        REQUIRE(std::string(loadedRawFile->Asset()->buffer) == rawFile.buffer);
        const auto* loadedPhysPreset = loaded->m_pools.GetAsset<NX1::AssetPhysPreset>(physPreset.name);
        REQUIRE(loadedPhysPreset);
        REQUIRE(loadedPhysPreset->Asset()->type == physPreset.type);
        REQUIRE(loadedPhysPreset->Asset()->mass == physPreset.mass);
        REQUIRE(loadedPhysPreset->Asset()->bounce == physPreset.bounce);
        REQUIRE(physPreset.type == 0x12345678);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(loaded->m_game_id)->WriteZoneDef(definition, *loaded, false, false);
        REQUIRE(definition.str().find(">game,NX1\n>platform,xbox360\n") != std::string::npos);
        REQUIRE(definition.str().find("rawfile,test.txt\n") != std::string::npos);
        REQUIRE(definition.str().find("physpreset,test_phys\n") != std::string::npos);
    }

    TEST_CASE("NX1 zone round trip preserves animation arrays and script strings", "[nx1][system][zone]")
    {
        const auto numFrames = GENERATE(30, 300);
        Zone zone("animation", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::ScriptString names[]{zone.m_script_strings.AddOrGetScriptString("tag_origin")};
        NX1::XAnimNotifyInfo notify{};
        notify.name = zone.m_script_strings.AddOrGetScriptString("fire");
        notify.time = 0.5f;
        int16_t data[]{0x1234, -1234};
        uint8_t indices8[]{1, 20};
        uint16_t indices16[]{1, 299};

        NX1::XAnimParts animation{};
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
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_XANIMPARTS, animation.name, &animation, {}, {names[0], notify.name}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* loadedAsset = loaded->m_pools.GetAsset<NX1::AssetXAnim>(animation.name);
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

    TEST_CASE("NX1 zone round trip preserves image data and external stream records", "[nx1][system][zone]")
    {
        Zone zone("images", 0, GameId::NX1, GamePlatform::XBOX);
        uint8_t pixels[]{1, 2, 3, 4};
        NX1::GfxImage images[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& image = images[i];
            image.name = i == 0 ? "immediate" : "delayed";
            image.mapType = NX1::MAPTYPE_2D;
            image.width = 1;
            image.height = 1;
            image.depth = 1;
            image.cardMemory.platform[0] = sizeof(pixels);
            image.pixels = pixels;
            image.cached = i != 0;
            zone.m_pools.AddAsset(NX1::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
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
            const auto* loadedAsset = loaded->m_pools.GetAsset<NX1::AssetImage>(image.name);
            REQUIRE(loadedAsset);
            const auto& result = *loadedAsset->Asset();
            REQUIRE(result.cached == image.cached);
            REQUIRE(result.cardMemory.platform[0] == sizeof(pixels));
            REQUIRE(result.width == image.width);
            for (auto i = 0u; i < sizeof(pixels); i++)
                REQUIRE(result.pixels[i] == pixels[i]);
        }
    }

    TEST_CASE("NX1 zone round trip preserves aligned collision bounds", "[nx1][system][zone]")
    {
        Zone zone("collision", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::BoundsArray bounds[2]{};
        bounds[0].midPoint.v[0] = 12.5f;
        bounds[1].halfSize.v[2] = 123.25f;
        NX1::clipMap_t collision{};
        collision.name = "maps/mp/test.d3dbsp";
        collision.numBrushes = 2;
        collision.brushBounds = bounds;
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_CLIPMAP_MP, collision.name, &collision, {}, {}, {});
        const auto loaded = RoundTripZone(zone);
        const auto* result = loaded->m_pools.GetAsset<NX1::AssetClipMapMp>(collision.name);
        REQUIRE(result);
        REQUIRE(result->Asset()->numBrushes == 2);
        REQUIRE(result->Asset()->brushBounds[0].midPoint.v[0] == 12.5f);
        REQUIRE(result->Asset()->brushBounds[1].halfSize.v[2] == 123.25f);
    }

    TEST_CASE("NX1 model surfaces stay nested and use the LOD surface count", "[nx1][system][zone]")
    {
        Zone zone("models", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::XSurface surfaces[2]{};
        surfaces[0].m_flags = 0x12345678;
        surfaces[1].m_flags = 0x87654321;
        NX1::XSurface otherSurface{};
        otherSurface.m_flags = 0xabcdef01;
        NX1::XModelSurfs modelSurfs{};
        modelSurfs.name = "shared_surfaces";
        modelSurfs.surfs = surfaces;
        NX1::XModelSurfs otherModelSurfs{};
        otherModelSurfs.name = "other_surfaces";
        otherModelSurfs.surfs = &otherSurface;
        NX1::XModel models[3]{};
        for (size_t index = 0; index < 3; ++index)
        {
            auto& model = models[index];
            model.name = index == 0 ? "first" : index == 1 ? "second" : "third";
            model.numLods = 1;
            model.numsurfs = index == 2 ? 1 : 2;
            model.lodInfo[0].numsurfs = model.numsurfs;
            model.lodInfo[0].modelSurfs = index == 2 ? &otherModelSurfs : &modelSurfs;
            zone.m_pools.AddAsset(NX1::ASSET_TYPE_XMODEL, model.name, &model, {}, {}, {});
        }
        models[0].numLods = 2;
        models[0].lodInfo[1].numsurfs = 1;
        models[0].lodInfo[1].modelSurfs = &otherModelSurfs;

        const auto loaded = RoundTripZone(zone);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 3);
        REQUIRE_FALSE(loaded->m_pools.GetAsset<NX1::AssetXModelSurfs>(modelSurfs.name));
        REQUIRE_FALSE(loaded->m_pools.GetAsset<NX1::AssetXModelSurfs>(otherModelSurfs.name));
        for (size_t index = 0; index < 3; ++index)
        {
            const auto* result = loaded->m_pools.GetAsset<NX1::AssetXModel>(models[index].name);
            REQUIRE(result);
            const auto& lod = result->Asset()->lodInfo[0];
            REQUIRE(lod.modelSurfs);
            REQUIRE(std::string(lod.modelSurfs->name) == (index == 2 ? otherModelSurfs.name : modelSurfs.name));
            REQUIRE(lod.numsurfs == models[index].lodInfo[0].numsurfs);
            REQUIRE(lod.modelSurfs->surfs);
            REQUIRE(lod.modelSurfs->surfs[0].m_flags == (index == 2 ? otherSurface.m_flags : surfaces[0].m_flags));
            if (index != 2)
                REQUIRE(lod.modelSurfs->surfs[1].m_flags == surfaces[1].m_flags);
        }
        const auto* first = loaded->m_pools.GetAsset<NX1::AssetXModel>(models[0].name)->Asset();
        REQUIRE(std::string(first->lodInfo[1].modelSurfs->name) == otherModelSurfs.name);
        REQUIRE(first->lodInfo[1].modelSurfs->surfs[0].m_flags == otherSurface.m_flags);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(loaded->m_game_id)->WriteZoneDef(definition, *loaded, false, false);
        REQUIRE(definition.str().find("xmodelsurfs,") == std::string::npos);
    }

    TEST_CASE("NX1 rejects standalone model surfaces", "[nx1][system][zone]")
    {
        Zone zone("standalone_surfaces", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::XModelSurfs surfaces{};
        surfaces.name = "test_surfaces";
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_XMODEL_SURFS, surfaces.name, &surfaces, {}, {}, {});

        std::ostringstream output(std::ios::binary);
        REQUIRE_FALSE(ZoneWriting::WriteZone(output, zone));
    }

    TEST_CASE("NX1 shader asset aliases resolve through the asset pool", "[nx1][system][zone]")
    {
        Zone zone("shader_aliases", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::MaterialPixelShader shader{};
        shader.name = "shared_shader";
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_PIXELSHADER, shader.name, &shader, {}, {}, {});
        NX1::MaterialTechnique techniques[2]{};
        NX1::MaterialTechniqueSet sets[2]{};
        for (size_t index = 0; index < 2; ++index)
        {
            techniques[index].name = "test";
            techniques[index].passCount = 1;
            techniques[index].passArray[0].pixelShader = &shader;
            sets[index].name = index == 0 ? "first" : "second";
            sets[index].techniques[0] = &techniques[index];
            zone.m_pools.AddAsset(NX1::ASSET_TYPE_TECHNIQUE_SET, sets[index].name, &sets[index], {}, {}, {});
        }
        const auto loaded = RoundTripZone(zone);
        const auto* loadedShader = loaded->m_pools.GetAsset<NX1::AssetPixelShader>(shader.name);
        REQUIRE(loadedShader);
        for (const auto& set : sets)
        {
            const auto* result = loaded->m_pools.GetAsset<NX1::AssetTechniqueSet>(set.name);
            REQUIRE(result);
            REQUIRE(result->Asset()->techniques[0]->passArray[0].pixelShader == loadedShader->Asset());
        }
    }

    TEST_CASE("NX1 sound aliases retain shared VF and nested LPF curve assets", "[nx1][system][zone]")
    {
        Zone zone("sound_curves", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::SndVfCurve vf{};
        vf.filename = "vf";
        vf.sndCurve.knotCount = 2;
        vf.sndCurve.knots[1][0] = 1.0f;
        NX1::SndLpfCurve lpf{};
        lpf.filename = "lpf";
        lpf.sndCurve.knotCount = 2;
        lpf.sndCurve.knots[1][1] = 0.5f;
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_SND_VOLUME_FALLOFF_CURVE, vf.filename, &vf, {}, {}, {});
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_SND_LPF_CURVE, lpf.filename, &lpf, {}, {}, {});
        NX1::snd_alias_t aliases[2]{};
        for (auto& alias : aliases)
        {
            alias.aliasName = "sound";
            alias.volumeFalloffCurve = &vf;
            alias.lpfCurve = &lpf;
        }
        NX1::snd_alias_list_t sound{};
        sound.aliasName = "sound";
        sound.head = aliases;
        sound.count = 2;
        zone.m_pools.AddAsset(NX1::ASSET_TYPE_SOUND, sound.aliasName, &sound, {}, {}, {});
        const auto loaded = RoundTripZone(zone);
        const auto* loadedVf = loaded->m_pools.GetAsset<NX1::AssetSndVfCurve>(vf.filename);
        const auto* loadedLpf = loaded->m_pools.GetAsset<NX1::AssetSndLpfCurve>(lpf.filename);
        const auto* loadedSound = loaded->m_pools.GetAsset<NX1::AssetSound>(sound.aliasName);
        REQUIRE(loadedVf);
        REQUIRE(loadedLpf);
        REQUIRE(loadedSound);
        REQUIRE(loadedLpf->Asset()->sndCurve.knots[1][1] == 0.5f);
        for (size_t index = 0; index < 2; ++index)
        {
            REQUIRE(loadedSound->Asset()->head[index].volumeFalloffCurve == loadedVf->Asset());
            REQUIRE(loadedSound->Asset()->head[index].lpfCurve == loadedLpf->Asset());
        }
    }

    TEST_CASE("NX1 zone round trip preserves reusable material data", "[nx1][system][zone]")
    {
        Zone zone("materials", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::MaterialConstantDef constant{};
        constant.nameHash = 0x12345678;
        constant.literal[0] = 12.5f;
        NX1::Material materials[2]{};
        for (auto i = 0u; i < 2; i++)
        {
            auto& material = materials[i];
            material.info.name = i == 0 ? "first" : "second";
            material.constantCount = 1;
            material.constantTable = &constant;
            zone.m_pools.AddAsset(NX1::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});
        }

        const auto loaded = RoundTripZone(zone);
        const auto* first = loaded->m_pools.GetAsset<NX1::AssetMaterial>("first");
        const auto* second = loaded->m_pools.GetAsset<NX1::AssetMaterial>("second");
        REQUIRE(first);
        REQUIRE(second);
        REQUIRE(first->Asset()->constantTable == second->Asset()->constantTable);
        REQUIRE(first->Asset()->constantTable->nameHash == constant.nameHash);
        REQUIRE(first->Asset()->constantTable->literal[0] == constant.literal[0]);
    }

    TEST_CASE("NX1 imports in-memory assets with nested image streams", "[nx1][system][zone]")
    {
        Zone source("source", 0, GameId::NX1, GamePlatform::XBOX);
        NX1::GfxImage image{};
        image.name = "nested_image";
        image.cached = true;
        NX1::MaterialTextureDef texture{};
        texture.semantic = NX1::TS_COLOR_MAP;
        texture.u.image = &image;
        NX1::Material material{};
        material.info.name = "nested_material";
        material.textureCount = 1;
        material.textureTable = &texture;
        source.m_pools.AddAsset(NX1::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        source.m_pools.AddAsset(NX1::ASSET_TYPE_MATERIAL, material.info.name, &material, {}, {}, {});
        source.m_xfile_image_stream_files_by_name[image.name].push_back({
            XAssetStreamFile{3, 4096, 8192},
            {},
            {},
            {}
        });
        source.Register();

        Zone target("imported", 0, GameId::NX1, GamePlatform::XBOX);
        MockSearchPath searchPath;
        GdtLookup gdt;
        AssetCreatorCollection creators(target);
        IObjLoader::GetObjLoaderForGame(GameId::NX1)->ConfigureCreatorCollection(creators, target, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        REQUIRE(context.LoadDependency<NX1::AssetMaterial>(material.info.name));
        REQUIRE(target.m_pools.GetAsset<NX1::AssetImage>(image.name));

        const auto loaded = RoundTripZone(target);
        REQUIRE(loaded->m_xfile_image_stream_files.size() == 4);
        REQUIRE(loaded->m_xfile_image_stream_files[0].fileIndex == 3);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offset == 4096);
        REQUIRE(loaded->m_xfile_image_stream_files[0].offsetEnd == 8192);
        const auto* result = loaded->m_pools.GetAsset<NX1::AssetMaterial>(material.info.name);
        REQUIRE(result);
        REQUIRE(std::string(result->Asset()->textureTable[0].u.image->name) == image.name);
    }
} // namespace

#endif
