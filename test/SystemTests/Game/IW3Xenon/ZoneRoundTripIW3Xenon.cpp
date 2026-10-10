#include "Game/AutoSearchPaths.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Game/IW3Xenon/Image/ImageToCommonConverterIW3Xenon.h"
#include "Game/IW3Xenon/Image/LoaderImageIW3Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter6.h"
#include "Image/XenonTextureDecoder.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "Pool/XAssetInfo.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "SearchPath/SearchPathFilesystem.h"
#include "SearchPath/SearchPaths.h"
#include "Zone/Definition/ZoneDefWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <filesystem>
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

    TEST_CASE("IW3 Xenon disk images survive native fastfile linking with all faces and mips", "[iw3xenon][system][image][loading]")
    {
        const auto iwi = GENERATE(false, true);
        const auto cube = GENERATE(false, true);
        const auto function = GENERATE(false, true);
        const auto formatId = GENERATE(image::ImageFormatId::BC1, image::ImageFormatId::B8_G8_R8_A8, image::ImageFormatId::R8_A8, image::ImageFormatId::A8);
        CAPTURE(iwi, cube, function, formatId);
        auto source = image::Texture::CreateForType(
            cube ? image::TextureType::T_CUBE : image::TextureType::T_2D, image::ImageFormat::GetImageFormatById(formatId), 64u, 64u, 1u, true);
        source->Allocate();
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                for (auto offset = 0uz; offset < source->GetSizeOfMipLevel(mip); ++offset)
                    source->GetBufferForMipLevel(mip, face)[offset] = static_cast<uint8_t>(offset * 17u + mip * 31u + face * 43u);

        std::ostringstream output(std::ios::binary);
        if (iwi)
            image::iwi6::IwiWriter().DumpImage(output, source.get());
        else
            image::DdsWriter().DumpImage(output, source.get());
        auto fileData = output.str();
        if (iwi)
            fileData[sizeof(image::IwiVersionHeader) + offsetof(image::iwi6::IwiHeader, flags)] |=
                image::iwi6::IMG_FLAG_NOPICMIP | image::iwi6::IMG_FLAG_CLAMP_U | image::iwi6::IMG_FLAG_CLAMP_V | image::iwi6::IMG_FLAG_STREAMING;

        const std::string assetName = function ? "*testimage" : "testimage";
        MockSearchPath searchPath;
        searchPath.AddFileData(std::string(function ? "images/_testimage" : "images/testimage") + (iwi ? ".iwi" : ".dds"), std::move(fileData));
        Zone zone("disk_images", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW3Xenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        REQUIRE(creators.CreateAsset(IW3Xenon::ASSET_TYPE_IMAGE, assetName, context).HasBeenSuccessful());

        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<IW3Xenon::AssetImage>(assetName);
        REQUIRE(asset);
        const auto* image = asset->Asset();
        REQUIRE(image->mapType == (cube ? IW3Xenon::MAPTYPE_CUBE : IW3Xenon::MAPTYPE_2D));
        REQUIRE(image->delayLoadPixels == !function);
        REQUIRE_FALSE(image->streaming);
        REQUIRE(image->streamSlot == 0xFFFFu);
        REQUIRE(image->texture.loadDef->levelCount == source->GetMipMapCount());
        REQUIRE((image->texture.loadDef->flags & IW3Xenon::IMG_FLAG_STREAMING) == 0);
        if (iwi)
        {
            REQUIRE(image->texture.loadDef->flags & IW3Xenon::IMG_FLAG_NOPICMIP);
            REQUIRE(image->texture.loadDef->flags & IW3Xenon::IMG_FLAG_CLAMP_U);
            REQUIRE(image->texture.loadDef->flags & IW3Xenon::IMG_FLAG_CLAMP_V);
        }
        image::xenon::XenonTextureHeader header;
        REQUIRE(image::xenon::DecodeTextureHeader(image->texture.loadDef->texture.map, sizeof(IW3Xenon::D3DTexture), header));
        REQUIRE(header.fetch.tiled == 1u);
        REQUIRE(header.fetch.depth == static_cast<unsigned>(source->GetFaceCount()));
        REQUIRE(header.fetch.maxMipLevel == static_cast<unsigned>(source->GetMipMapCount() - 1));

        const auto decoded = image::ToCommonConverterIW3Xenon().Convert(*asset, searchPath);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == formatId);
        REQUIRE(decoded->HasMipMaps());
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);
    }

    TEST_CASE("IW3 Xenon image loading rejects unsupported volumes and broken DDS overrides", "[iw3xenon][system][image][loading]")
    {
        MockSearchPath searchPath;
        image::Texture3D source(&image::format::B8_G8_R8_A8, 4u, 4u, 4u);
        source.Allocate();
        std::ostringstream output(std::ios::binary);
        image::DdsWriter().DumpImage(output, &source);
        searchPath.AddFileData("images/volume.dds", output.str());
        searchPath.AddFileData("images/invalid.dds", "broken DDS");
        image::Texture2D fallback(&image::format::BC1, 4u, 4u);
        fallback.Allocate();
        std::ostringstream iwiOutput(std::ios::binary);
        image::iwi6::IwiWriter().DumpImage(iwiOutput, &fallback);
        searchPath.AddFileData("images/invalid.iwi", iwiOutput.str());

        Zone zone("invalid_images", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        const auto loader = image::CreateLoaderIW3Xenon(zone.Memory(), searchPath);
        REQUIRE(loader->CreateAsset("volume", context).HasFailed());
        REQUIRE(loader->CreateAsset("invalid", context).HasFailed());
        REQUIRE(zone.m_pools.GetTotalAssetCount() == 0);
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

    TEST_CASE("IW3 Xenon images dump decoded pixels from immediate and delayed fastfile data", "[iw3xenon][system][image]")
    {
        const auto outputFormat = GENERATE(ImageOutputFormat_e::DDS, ImageOutputFormat_e::IWI);

        struct RestoreImageOutputFormat
        {
            ImageOutputFormat_e original = ObjWriting::Configuration.ImageOutputFormat;

            ~RestoreImageOutputFormat()
            {
                ObjWriting::Configuration.ImageOutputFormat = original;
            }
        } restoreImageOutputFormat;

        ObjWriting::Configuration.ImageOutputFormat = outputFormat;

        Zone zone("decoded_images", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        IW3Xenon::D3DTexture header{};
        header.Format.dword[0] = 2u | (1u << 22u);
        header.Format.dword[1] = IW3Xenon::GPUTEXTUREFORMAT_8_8_8_8 | (IW3Xenon::GPUENDIAN_8IN32 << 6u);
        header.Format.dword[2] = 1u | (1u << 13u);
        header.Format.dword[5] = image::xenon::GPUDIMENSION_2D << 9u;
        IW3Xenon::GfxImageLoadDef loadDef{};
        loadDef.levelCount = 1;
        loadDef.dimensions[0] = 2;
        loadDef.dimensions[1] = 2;
        loadDef.dimensions[2] = 1;
        loadDef.format = static_cast<int>(header.Format.dword[1]);
        loadDef.texture.map = &header;

        constexpr std::array<std::uint8_t, 16> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        std::array<std::uint8_t, 4096> pixels{};
        for (auto row = 0u; row < 2u; ++row)
            for (auto byte = 0u; byte < 8u; ++byte)
                pixels[row * 128u + (byte ^ 3u)] = expected[row * 8u + byte];

        IW3Xenon::GfxImage images[2]{};
        for (auto i = 0u; i < 2; ++i)
        {
            auto& image = images[i];
            image.name = i == 0 ? "immediate" : "delayed";
            image.mapType = IW3Xenon::MAPTYPE_2D;
            image.texture.loadDef = &loadDef;
            image.width = 2;
            image.height = 2;
            image.depth = 1;
            image.cardMemory.platform[0] = static_cast<int>(pixels.size());
            image.pixels = pixels.data();
            image.delayLoadPixels = i != 0;
            zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        }

        const auto loaded = RoundTripZone(zone);
        MockSearchPath searchPath;
        MockOutputPath outputPath;
        const std::string basePath;
        AssetDumpingContext context(*loaded, basePath, outputPath, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW3Xenon)->DumpZone(context));
        REQUIRE(outputPath.GetMockedFileList().size() == 2u);
        for (const auto& image : images)
        {
            const auto* file = outputPath.GetMockedFile(std::string("images/") + image.name + (outputFormat == ImageOutputFormat_e::DDS ? ".dds" : ".iwi"));
            REQUIRE(file);
            std::istringstream stream(file->AsString());
            std::unique_ptr<image::Texture> texture;
            if (outputFormat == ImageOutputFormat_e::DDS)
                texture = image::LoadDds(stream);
            else
            {
                auto result = image::LoadIwi(stream);
                REQUIRE(result);
                REQUIRE(result->m_version == image::IwiVersion::IWI_6);
                texture = std::move(result->m_texture);
            }
            REQUIRE(texture);
            REQUIRE(texture->GetWidth() == 2u);
            REQUIRE(texture->GetHeight() == 2u);
            REQUIRE(texture->GetFormat()->GetId() == image::ImageFormatId::B8_G8_R8_A8);
            REQUIRE(std::memcmp(texture->GetBufferForMipLevel(0), expected.data(), expected.size()) == 0);
        }

        const auto* asset = loaded->m_pools.GetAsset<IW3Xenon::AssetImage>("immediate");
        REQUIRE(asset);
        auto brokenAsset = *asset;
        auto* brokenImage = loaded->Memory().Alloc<IW3Xenon::GfxImage>();
        *brokenImage = *asset->Asset();
        brokenAsset.m_ptr = brokenImage;
        brokenImage->texture.loadDef = nullptr;
        REQUIRE_FALSE(image::ToCommonConverterIW3Xenon().Convert(brokenAsset, searchPath));
    }

    TEST_CASE("IW3 Xenon highmips extend the resident mip chain and fall back when unavailable", "[iw3xenon][system][image][highmip]")
    {
        enum class HighMipState
        {
            AVAILABLE,
            MISSING,
            TRUNCATED,
            OVERSIZED,
            NOT_STREAMED
        };
        const auto state =
            GENERATE(HighMipState::AVAILABLE, HighMipState::MISSING, HighMipState::TRUNCATED, HighMipState::OVERSIZED, HighMipState::NOT_STREAMED);
        const auto isCube = GENERATE(false, true);
        const auto faceCount = isCube ? 6u : 1u;
        const auto delayLoadPixels = GENERATE(false, true);
        const auto outputFormat = GENERATE(ImageOutputFormat_e::DDS, ImageOutputFormat_e::IWI);

        struct RestoreImageOutputFormat
        {
            ImageOutputFormat_e original = ObjWriting::Configuration.ImageOutputFormat;

            ~RestoreImageOutputFormat()
            {
                ObjWriting::Configuration.ImageOutputFormat = original;
            }
        } restoreImageOutputFormat;

        ObjWriting::Configuration.ImageOutputFormat = outputFormat;

        Zone zone("high_mip_images", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        IW3Xenon::D3DTexture header{};
        header.Format.dword[0] = 2u | (1u << 22u);
        header.Format.dword[1] = IW3Xenon::GPUTEXTUREFORMAT_8 | (IW3Xenon::GPUENDIAN_8IN32 << 6u);
        header.Format.dword[2] = 7u | (7u << 13u) | ((faceCount - 1u) << 26u);
        header.Format.dword[4] = 3u << 6u;
        header.Format.dword[5] = ((isCube ? image::xenon::GPUDIMENSION_CUBEMAP : image::xenon::GPUDIMENSION_2D) << 9u) | (1u << 11u);
        IW3Xenon::GfxImageLoadDef loadDef{};
        loadDef.levelCount = 4;
        loadDef.flags = isCube ? IW3Xenon::IMG_FLAG_CUBEMAP : 0;
        loadDef.dimensions[0] = 8;
        loadDef.dimensions[1] = 8;
        loadDef.dimensions[2] = 1;
        loadDef.format = static_cast<int>(header.Format.dword[1]);
        loadDef.texture.map = &header;

        constexpr std::array<std::array<unsigned, 2>, 4> origins{
            {{16, 0}, {8, 0}, {4, 0}, {0, 4}}
        };
        std::vector<std::uint8_t> pixels(faceCount * 4096u);
        for (auto face = 0u; face < faceCount; ++face)
            for (auto mip = 0u; mip < origins.size(); ++mip)
                for (auto y = 0u; y < (8u >> mip); ++y)
                    for (auto x = 0u; x < (8u >> mip); ++x)
                        pixels[face * 4096u + (((origins[mip][1] + y) * 32u + origins[mip][0] + x) ^ 3u)] = static_cast<std::uint8_t>(0x20u + mip + face * 16u);

        IW3Xenon::GfxImage image{};
        image.name = "streamed";
        image.mapType = isCube ? IW3Xenon::MAPTYPE_CUBE : IW3Xenon::MAPTYPE_2D;
        image.texture.loadDef = &loadDef;
        image.width = 8;
        image.height = 8;
        image.depth = 1;
        image.cardMemory.platform[0] = static_cast<int>(pixels.size());
        image.pixels = pixels.data();
        image.baseSize = 12345; // Load_Texture replaces this serialized value before the game streams.
        image.streaming = state != HighMipState::NOT_STREAMED;
        image.delayLoadPixels = delayLoadPixels;
        zone.m_pools.AddAsset(IW3Xenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});

        MockSearchPath searchPath;
        if (state != HighMipState::MISSING)
        {
            std::string highMip(faceCount * 16384u, '\0');
            for (auto face = 0u; face < faceCount; ++face)
                for (auto y = 0u; y < 16u; ++y)
                    for (auto x = 0u; x < 16u; ++x)
                        highMip[face * 4096u + y * 64u + (x ^ 3u)] = static_cast<char>(face * 31u + y * 16u + x);
            if (state == HighMipState::TRUNCATED)
                highMip.pop_back();
            else if (state == HighMipState::OVERSIZED)
                highMip.push_back('\0');
            searchPath.AddFileData("highmip/streamed.hi", std::move(highMip));
        }

        const auto loaded = RoundTripZone(zone);
        MockOutputPath outputPath;
        const std::string basePath;
        AssetDumpingContext context(*loaded, basePath, outputPath, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW3Xenon)->DumpZone(context));
        const auto* file = outputPath.GetMockedFile(outputFormat == ImageOutputFormat_e::DDS ? "images/streamed.dds" : "images/streamed.iwi");
        REQUIRE(file);
        std::istringstream stream(file->AsString());
        std::unique_ptr<image::Texture> texture;
        if (outputFormat == ImageOutputFormat_e::DDS)
            texture = image::LoadDds(stream);
        else
        {
            auto result = image::LoadIwi(stream);
            REQUIRE(result);
            texture = std::move(result->m_texture);
        }
        REQUIRE(texture);
        const auto hasHighMip = state == HighMipState::AVAILABLE;
        REQUIRE(texture->GetWidth() == (hasHighMip ? 16u : 8u));
        REQUIRE(texture->GetHeight() == (hasHighMip ? 16u : 8u));
        REQUIRE(texture->GetMipMapCount() == (hasHighMip ? 5 : 4));
        REQUIRE(texture->GetFaceCount() == faceCount);
        if (hasHighMip)
            for (auto face = 0u; face < faceCount; ++face)
                for (auto pixel = 0u; pixel < 256u; ++pixel)
                    REQUIRE(texture->GetBufferForMipLevel(0, static_cast<int>(face))[pixel] == static_cast<std::uint8_t>(face * 31u + pixel));
        for (auto face = 0u; face < faceCount; ++face)
            for (auto mip = 0; mip < 4; ++mip)
            {
                const auto outputMip = mip + (hasHighMip ? 1 : 0);
                const auto* data = texture->GetBufferForMipLevel(outputMip, static_cast<int>(face));
                REQUIRE(std::all_of(data,
                                    data + texture->GetSizeOfMipLevel(outputMip),
                                    [mip, face](auto value)
                                    {
                                        return value == 0x20u + mip + face * 16u;
                                    }));
            }
        const auto* loadedImage = loaded->m_pools.GetAsset<IW3Xenon::AssetImage>(image.name)->Asset();
        REQUIRE(loadedImage->width == 8u);
        REQUIRE(loadedImage->height == 8u);
        REQUIRE(loadedImage->texture.loadDef->texture.map->Format.dword[0] == header.Format.dword[0]);
    }

    TEST_CASE("IW3 Xenon automatic search paths find a highmip folder beside the fastfile", "[iw3xenon][system][image][highmip]")
    {
        const auto root = oat::paths::GetTempDirectory("IW3XenonHighMipSearch");
        std::filesystem::create_directories(root / "highmip");
        {
            std::ofstream output(root / "highmip" / "test.hi", std::ios::binary);
            output << "high mip data";
        }
        const auto zonePath = GENERATE_COPY((root / "test.ff").generic_string(), root.generic_string() + "/");
        SearchPaths searchPath;
        for (const auto& path : AutoSearchPaths::GetForGame(GameId::IW3Xenon)->GetSearchPathsForZonePath(zonePath))
            searchPath.CommitSearchPath(std::make_unique<SearchPathFilesystem>(path));
        const auto file = searchPath.Open("highmip/test.hi");
        REQUIRE(file.IsOpen());
        REQUIRE(file.m_length == 13);
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
