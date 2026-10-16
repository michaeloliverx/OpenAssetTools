#include "Game/AutoSearchPaths.h"
#include "Game/T5Xenon/Image/ImageToCommonConverterT5Xenon.h"
#include "Game/T5Xenon/Image/LoaderImageT5Xenon.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/ImageCommon.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter13.h"
#include "Image/XenonTextureDecoder.h"
#include "Image/XenonTextureEncoder.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "Pool/XAssetInfo.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "SearchPath/SearchPathFilesystem.h"
#include "SearchPath/SearchPaths.h"
#include "Utils/Endianness.h"
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
        REQUIRE(static_cast<unsigned char>(header[10]) == 1);
        REQUIRE(static_cast<unsigned char>(header[11]) == 0xd9);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::T5Xenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("T5 Xenon disk images survive native fastfile linking with all faces and mips", "[t5xenon][system][image][loading]")
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
            image::iwi13::IwiWriter().DumpImage(output, source.get());
        else
            image::DdsWriter().DumpImage(output, source.get());
        auto fileData = output.str();

        const std::string assetName = function ? "*testimage" : "testimage";
        MockSearchPath searchPath;
        searchPath.AddFileData(std::string(function ? "images/_testimage" : "images/testimage") + (iwi ? ".iwi" : ".dds"), std::move(fileData));
        Zone zone("disk_images", 0, GameId::T5Xenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::T5Xenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        REQUIRE(creators.CreateAsset(T5Xenon::ASSET_TYPE_IMAGE, assetName, context).HasBeenSuccessful());

        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<T5Xenon::AssetImage>(assetName);
        REQUIRE(asset);
        const auto* image = asset->Asset();
        REQUIRE(image->mapType == (cube ? T5Xenon::MAPTYPE_CUBE : T5Xenon::MAPTYPE_2D));
        REQUIRE(image->delayLoadPixels == !function);
        REQUIRE_FALSE(image->streaming);
        REQUIRE(image->levelCount == source->GetMipMapCount());
        image::xenon::XenonTextureHeader header;
        REQUIRE(image::xenon::DecodeTextureHeader(image->texture, sizeof(image->texture), header));
        REQUIRE(header.fetch.tiled == 1u);
        REQUIRE(header.fetch.depth == static_cast<unsigned>(source->GetFaceCount()));
        REQUIRE(header.fetch.maxMipLevel == static_cast<unsigned>(source->GetMipMapCount() - 1));

        const auto decoded = image::ToCommonConverterT5Xenon().Convert(*asset, searchPath);
        REQUIRE(decoded);
        REQUIRE(decoded->GetFormat()->GetId() == formatId);
        REQUIRE(decoded->HasMipMaps());
        for (auto face = 0; face < source->GetFaceCount(); ++face)
            for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
                REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip, face), source->GetBufferForMipLevel(mip, face), source->GetSizeOfMipLevel(mip)) == 0);
    }

    void AppendBigEndian(std::string& output, const uint32_t value)
    {
        const auto bigEndian = endianness::ToBigEndian(value);
        output.append(reinterpret_cast<const char*>(&bigEndian), sizeof(bigEndian));
    }

    std::string MakeImagePak(const unsigned entry, const std::string& pixels, const bool chunked, const bool overlap)
    {
        std::string data;
        if (chunked)
        {
            AppendBigEndian(data, overlap ? 0x80000000u : 0u);
            const auto tailSize = overlap ? std::min(1024uz, pixels.size()) : 0uz;
            if (tailSize)
            {
                AppendBigEndian(data, static_cast<uint32_t>(tailSize));
                AppendBigEndian(data, static_cast<uint32_t>(tailSize));
                data.append(pixels.end() - tailSize, pixels.end());
            }
            if (pixels.size() > tailSize)
            {
                const auto size = static_cast<uint32_t>(pixels.size() - tailSize);
                AppendBigEndian(data, size);
                AppendBigEndian(data, size);
                data.append(pixels.data(), size);
            }
        }
        else
            data = pixels;

        const auto size = static_cast<uint16_t>((data.size() + 2047u) / 2048u);
        data.resize(size * 2048u);
        std::string pak = "pak2";
        AppendBigEndian(pak, 0u);
        AppendBigEndian(pak, 41u);
        AppendBigEndian(pak, 2u);
        AppendBigEndian(pak, 2048u);
        AppendBigEndian(pak, 2048u);
        AppendBigEndian(pak, 1u);
        AppendBigEndian(pak, entry == 0u ? 1u : 0xFFFFFFFFu);
        AppendBigEndian(pak, entry == 1u ? 1u : 0xFFFFFFFFu);
        for (auto index = 0u; index < 2u; ++index)
        {
            const auto value = endianness::ToBigEndian(static_cast<uint16_t>(index == entry ? (chunked ? 0x8000u | size : 0u) : 0xFFFFu));
            pak.append(reinterpret_cast<const char*>(&value), sizeof(value));
        }
        pak.resize(2048u);
        pak += data;
        return pak;
    }

    TEST_CASE("T5 Xenon streamed images combine both image packs with embedded mips", "[t5xenon][system][image][streaming]")
    {
        const auto embedded = GENERATE(false, true);
        const auto chunked = GENERATE(false, true);
        const auto overlap = GENERATE(false, true);
        auto source = image::Texture::CreateForType(image::TextureType::T_2D, &image::format::BC1, 64u, 64u, 1u, true);
        source->Allocate();
        for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
            for (auto offset = 0uz; offset < source->GetSizeOfMipLevel(mip); ++offset)
                source->GetBufferForMipLevel(mip)[offset] = static_cast<uint8_t>(offset * 17u + mip * 31u);
        image::xenon::XenonTextureEncodeResult encoded;
        REQUIRE(image::xenon::EncodeTexture(*source, encoded));

        Zone zone("streamed_images", 0, GameId::T5Xenon, GamePlatform::XBOX);
        auto* image = zone.Memory().Alloc<T5Xenon::GfxImage>();
        image->name = zone.Memory().Dup("streamed");
        image->mapType = T5Xenon::MAPTYPE_2D;
        image->streaming = true;
        image->depth = 1;
        image->format = encoded.rawFormat;
        image->streamedPartCount = 2;
        std::memcpy(image->texture, encoded.textureHeader.data(), encoded.textureHeader.size());
        const auto halfSize = encoded.pixels.size() / 2u;
        const auto embeddedSize = embedded ? halfSize / 2u : 0uz;
        image->cardMemory.platform[0] = static_cast<int>(embeddedSize);
        if (embeddedSize)
        {
            image->pixels = zone.Memory().Alloc<unsigned char>(embeddedSize);
            std::memcpy(image->pixels, encoded.pixels.data() + encoded.pixels.size() - embeddedSize, embeddedSize);
        }
        image->streams[0] = {static_cast<unsigned>(halfSize << 4u) | 6u, 32u, 32u, 0u};
        image->streams[1] = {static_cast<unsigned>(encoded.pixels.size() << 4u) | 7u, 64u, 64u, 1u};
        zone.m_pools.AddAsset(T5Xenon::ASSET_TYPE_IMAGE, "streamed", image, {}, {}, {});
        const auto loaded = RoundTripZone(zone);
        const auto* asset = loaded->m_pools.GetAsset<T5Xenon::AssetImage>("streamed");
        REQUIRE(asset);
        REQUIRE(asset->Asset()->streams[1].pixelSize == image->streams[1].pixelSize);

        MockSearchPath searchPath;
        searchPath.AddFileData("images.pak", MakeImagePak(1u, std::string(encoded.pixels.begin(), encoded.pixels.begin() + halfSize), chunked, overlap));
        auto lowPak = MakeImagePak(0u, std::string(encoded.pixels.begin() + halfSize, encoded.pixels.end() - embeddedSize), chunked, overlap);
        searchPath.AddFileData("images_low.pak", lowPak);
        const auto decoded = image::ToCommonConverterT5Xenon().Convert(*asset, searchPath);
        REQUIRE(decoded);
        for (auto mip = 0; mip < source->GetMipMapCount(); ++mip)
            REQUIRE(std::memcmp(decoded->GetBufferForMipLevel(mip), source->GetBufferForMipLevel(mip), source->GetSizeOfMipLevel(mip)) == 0);

        lowPak.resize(2049u);
        MockSearchPath truncatedPath;
        truncatedPath.AddFileData("images_low.pak", std::move(lowPak));
        REQUIRE_FALSE(image::ToCommonConverterT5Xenon().Convert(*asset, truncatedPath));
    }

} // namespace

#endif
