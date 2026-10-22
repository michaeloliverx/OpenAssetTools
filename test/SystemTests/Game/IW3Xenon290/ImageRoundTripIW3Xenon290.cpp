#include "Game/IW3Xenon290/IW3Xenon290.h"
#include "Game/IW3Xenon290/Image/ImageToCommonConverterIW3Xenon290.h"
#include "Game/IW3Xenon290/Image/LoaderImageIW3Xenon290.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter6.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "Pool/XAssetInfo.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

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
        const auto path = oat::paths::GetTempDirectory("IW3Xenon290RoundTrip") / (zone.m_name + ".ff");
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
        REQUIRE(static_cast<unsigned char>(header[3]) == 148);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::IW3Xenon290);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("IW3 Xenon 290 images survive disk and fastfile round trips with all faces and mips", "[iw3xenon290][system][image][loading]")
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
        const auto fileName = std::string(function ? "images/_testimage" : "images/testimage") + (iwi ? ".iwi" : ".dds");
        searchPath.AddFileData(fileName, std::move(fileData));
        Zone zone("disk_images", 0, GameId::IW3Xenon290, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW3Xenon290)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        REQUIRE(creators.CreateAsset(IW3Xenon290::ASSET_TYPE_IMAGE, assetName, context).HasBeenSuccessful());

        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<IW3Xenon290::AssetImage>(assetName);
        REQUIRE(asset);
        const auto* image = asset->Asset();
        REQUIRE(image->mapType == (cube ? IW3Xenon290::MAPTYPE_CUBE : IW3Xenon290::MAPTYPE_2D));
        REQUIRE(image->texture.loadDef->levelCount == source->GetMipMapCount());
        if (iwi)
        {
            REQUIRE(image->texture.loadDef->flags & IW3Xenon290::IMG_FLAG_NOPICMIP);
            REQUIRE(image->texture.loadDef->flags & IW3Xenon290::IMG_FLAG_CLAMP_U);
            REQUIRE(image->texture.loadDef->flags & IW3Xenon290::IMG_FLAG_CLAMP_V);
        }
        REQUIRE(image->cardMemory.platform[0] > 0);

        const auto decoded = image::ToCommonConverterIW3Xenon290().Convert(*asset, searchPath);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == formatId);
        REQUIRE(decoded->HasMipMaps());
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);

        struct RestoreConfiguration
        {
            ObjWriting::Configuration_t original{ObjWriting::Configuration};

            ~RestoreConfiguration()
            {
                ObjWriting::Configuration = original;
            }
        } restore;

        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(IW3Xenon290::ASSET_TYPE_COUNT, true);
        ObjWriting::Configuration.ImageOutputFormat = iwi ? ImageOutputFormat_e::IWI : ImageOutputFormat_e::DDS;
        MockOutputPath dumped;
        AssetDumpingContext dumpingContext(*loaded, "", dumped, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(loaded->m_game_id)->DumpZone(dumpingContext));
        REQUIRE(dumped.GetMockedFileList().size() == 1);
        const auto* dumpedFile = dumped.GetMockedFile(fileName);
        REQUIRE(dumpedFile);
        std::istringstream dumpedStream(dumpedFile->AsString(), std::ios::binary);
        std::unique_ptr<image::Texture> dumpedTexture;
        if (iwi)
        {
            auto result = image::LoadIwi(dumpedStream);
            REQUIRE(result);
            REQUIRE(result->m_version == image::IwiVersion::IWI_6);
            dumpedTexture = std::move(result->m_texture);
        }
        else
            dumpedTexture = image::LoadDds(dumpedStream);

        REQUIRE(dumpedTexture);
        REQUIRE(dumpedTexture->GetFormat()->GetId() == formatId);
        REQUIRE(dumpedTexture->GetFaceCount() == source->GetFaceCount());
        REQUIRE(dumpedTexture->GetMipMapCount() == source->GetMipMapCount());
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                REQUIRE(std::memcmp(dumpedTexture->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);
    }

    TEST_CASE("IW3 Xenon 290 image loading rejects unsupported volumes and broken DDS overrides", "[iw3xenon290][system][image][loading]")
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

        Zone zone("invalid_images", 0, GameId::IW3Xenon290, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        const auto loader = image::CreateLoaderIW3Xenon290(zone.Memory(), searchPath);
        REQUIRE(loader->CreateAsset("volume", context).HasFailed());
        REQUIRE(loader->CreateAsset("invalid", context).HasFailed());
        REQUIRE(zone.m_pools.GetTotalAssetCount() == 0);
    }

} // namespace
#endif
