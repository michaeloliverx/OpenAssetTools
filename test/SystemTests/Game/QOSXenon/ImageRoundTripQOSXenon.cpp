#include "Game/AutoSearchPaths.h"
#include "Game/QOSXenon/Image/ImageToCommonConverterQOSXenon.h"
#include "Game/QOSXenon/Image/LoaderImageQOSXenon.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/ImageCommon.h"
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
#include <format>
#include <fstream>
#include <sstream>
#include <string>

#ifdef ARCH_x86

namespace
{
    std::unique_ptr<Zone> RoundTripZone(const Zone& zone)
    {
        const auto path = oat::paths::GetTempDirectory("QOSXenonRoundTrip") / (zone.m_name + ".ff");
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, zone));
        }

        std::ifstream input(path, std::ios::binary);
        char header[4]{};
        input.read(header, sizeof(header));
        REQUIRE(header[0] == 0);
        REQUIRE(header[1] == 0);
        REQUIRE(static_cast<unsigned char>(header[2]) == 1);
        REQUIRE(static_cast<unsigned char>(header[3]) == 0xd6);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::QOSXenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("QoS Xenon disk images survive native fastfile linking with all faces and mips", "[qosxenon][system][image][loading]")
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
        Zone zone("disk_images", 0, GameId::QOSXenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::QOSXenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        REQUIRE(creators.CreateAsset(QOSXenon::ASSET_TYPE_IMAGE, assetName, context).HasBeenSuccessful());

        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<QOSXenon::AssetImage>(assetName);
        REQUIRE(asset);
        const auto* image = asset->Asset();
        REQUIRE(image->mapType == (cube ? QOSXenon::MAPTYPE_CUBE : QOSXenon::MAPTYPE_2D));
        REQUIRE_FALSE(image->streaming);
        REQUIRE(image->streamSlot == 0xFFFFu);
        REQUIRE(image->texture.loadDef->levelCount == source->GetMipMapCount());
        REQUIRE((image->texture.loadDef->flags & QOSXenon::IMG_FLAG_STREAMING) == 0);
        if (iwi)
        {
            REQUIRE(image->texture.loadDef->flags & QOSXenon::IMG_FLAG_NOPICMIP);
            REQUIRE(image->texture.loadDef->flags & QOSXenon::IMG_FLAG_CLAMP_U);
            REQUIRE(image->texture.loadDef->flags & QOSXenon::IMG_FLAG_CLAMP_V);
        }
        image::xenon::XenonTextureHeader header;
        REQUIRE(image::xenon::DecodeTextureHeader(image->texture.loadDef->texture.map, sizeof(QOSXenon::D3DTexture), header));
        REQUIRE(header.fetch.tiled == 1u);
        REQUIRE(header.fetch.depth == static_cast<unsigned>(source->GetFaceCount()));
        REQUIRE(header.fetch.maxMipLevel == static_cast<unsigned>(source->GetMipMapCount() - 1));

        const auto decoded = image::ToCommonConverterQOSXenon().Convert(*asset, searchPath);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == formatId);
        REQUIRE(decoded->HasMipMaps());
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);
    }

    TEST_CASE("QoS Xenon image loading rejects unsupported volumes and broken DDS overrides", "[qosxenon][system][image][loading]")
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

        Zone zone("invalid_images", 0, GameId::QOSXenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        const auto loader = image::CreateLoaderQOSXenon(zone.Memory(), searchPath);
        REQUIRE(loader->CreateAsset("volume", context).HasFailed());
        REQUIRE(loader->CreateAsset("invalid", context).HasFailed());
        REQUIRE(zone.m_pools.GetTotalAssetCount() == 0);
    }

    TEST_CASE("QoS Xenon highmips extend the resident mip chain and fall back when unavailable", "[qosxenon][system][image][highmip]")
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
        const auto prefix = GENERATE('a', 'b', 'c', 'd');
        constexpr auto levelCount = 4u;
        const auto faceCount = isCube ? 6u : 1u;
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

        Zone zone("high_mip_images", 0, GameId::QOSXenon, GamePlatform::XBOX);
        QOSXenon::D3DTexture header{};
        header.Format[0] = 2u | (1u << 22u);
        header.Format[1] = image::xenon::GPUTEXTUREFORMAT_8 | (image::xenon::GPUENDIAN_8IN32 << 6u);
        header.Format[2] = 7u | (7u << 13u) | ((faceCount - 1u) << 26u);
        header.Format[4] = 3u << 6u;
        header.Format[5] = ((isCube ? image::xenon::GPUDIMENSION_CUBEMAP : image::xenon::GPUDIMENSION_2D) << 9u) | (1u << 11u);
        QOSXenon::GfxImageLoadDef loadDef{};
        loadDef.levelCount = static_cast<uint8_t>(levelCount);
        loadDef.flags = isCube ? QOSXenon::IMG_FLAG_CUBEMAP : 0;
        loadDef.dimensions[0] = 8;
        loadDef.dimensions[1] = 8;
        loadDef.dimensions[2] = 1;
        loadDef.format = static_cast<int>(header.Format[1]);
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

        const auto assetName = std::string(1, prefix) + "streamed";
        QOSXenon::GfxImage image{};
        image.name = assetName.c_str();
        image.mapType = isCube ? QOSXenon::MAPTYPE_CUBE : QOSXenon::MAPTYPE_2D;
        image.texture.loadDef = &loadDef;
        image.width = 8;
        image.height = 8;
        image.depth = 1;
        image.cardMemory.platform[0] = static_cast<int>(pixels.size());
        image.pixels = pixels.data();
        image.baseSize = 12345; // Load_Texture replaces this serialized value before the game streams.
        image.streaming = state != HighMipState::NOT_STREAMED;
        zone.m_pools.AddAsset(QOSXenon::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});

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
            const auto highMipPath = std::format("highmip/{}/{}.hi", static_cast<unsigned char>(prefix) & 3u, image.name);
            searchPath.AddFileData(highMipPath, std::move(highMip));
        }

        const auto loaded = RoundTripZone(zone);
        MockOutputPath outputPath;
        const std::string basePath;
        AssetDumpingContext context(*loaded, basePath, outputPath, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::QOSXenon)->DumpZone(context));
        const auto* file = outputPath.GetMockedFile(image::GetFileNameForAsset(image.name, outputFormat == ImageOutputFormat_e::DDS ? ".dds" : ".iwi"));
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
        const auto* loadedImage = loaded->m_pools.GetAsset<QOSXenon::AssetImage>(image.name)->Asset();
        REQUIRE(loadedImage->width == 8u);
        REQUIRE(loadedImage->height == 8u);
        REQUIRE(loadedImage->texture.loadDef->texture.map->Format[0] == header.Format[0]);
    }

} // namespace

#endif
