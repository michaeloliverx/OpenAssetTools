#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Game/IW4Xenon482/Image/ImageToCommonConverterIW4Xenon482.h"
#include "Gdt/GdtLookup.h"
#include "IObjCompiler.h"
#include "IObjLoader.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter8.h"
#include "Image/XenonTextureEncoder.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Utils/Endianness.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <zlib.h>

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

    void FillTexture(image::Texture& texture)
    {
        texture.Allocate();
        for (auto face = 0; face < texture.GetFaceCount(); ++face)
            for (auto mip = 0; mip < (texture.HasMipMaps() ? texture.GetMipMapCount() : 1); ++mip)
                for (auto byte = 0uz; byte < texture.GetSizeOfMipLevel(mip); ++byte)
                    texture.GetBufferForMipLevel(mip, face)[byte] = static_cast<uint8_t>(byte * 17u + mip * 31u + face * 43u);
    }

    void CheckTexture(const image::Texture& actual, const image::Texture& expected)
    {
        REQUIRE(actual.GetTextureType() == expected.GetTextureType());
        REQUIRE(actual.GetFormat()->GetId() == expected.GetFormat()->GetId());
        REQUIRE(actual.GetWidth() == expected.GetWidth());
        REQUIRE(actual.GetHeight() == expected.GetHeight());
        REQUIRE(actual.GetFaceCount() == expected.GetFaceCount());
        REQUIRE(actual.GetMipMapCount() == expected.GetMipMapCount());
        CHECK(actual.HasMipMaps() == expected.HasMipMaps());
        for (auto face = 0; face < expected.GetFaceCount(); ++face)
            for (auto mip = 0; mip < (expected.HasMipMaps() ? expected.GetMipMapCount() : 1); ++mip)
                CHECK(std::memcmp(actual.GetBufferForMipLevel(mip, face), expected.GetBufferForMipLevel(mip, face), expected.GetSizeOfMipLevel(mip)) == 0);
    }

#ifdef ARCH_x86
    void CheckAppendedStreams(const Zone& zone)
    {
        std::ifstream input(zone.m_source_path, std::ios::binary | std::ios::ate);
        REQUIRE(input.is_open());
        const auto fileEnd = static_cast<int64_t>(input.tellg());
        const auto& records = zone.m_xfile_image_stream_files;
        // Header, flags, timestamp, language mask and stream count precede the records.
        input.seekg(29u + records.size() * sizeof(XAssetStreamFile));
        uint32_t sizes[2]{};
        input.read(reinterpret_cast<char*>(sizes), sizeof(sizes));
        REQUIRE(input.gcount() == static_cast<std::streamsize>(sizeof(sizes)));
        const auto zoneEnd = endianness::FromBigEndian(sizes[0]);
        const auto loadEnd = endianness::FromBigEndian(sizes[1]);
        REQUIRE(zoneEnd < fileEnd);
        REQUIRE(loadEnd >= zoneEnd);
        std::vector<std::pair<uint32_t, uint32_t>> ranges;
        for (size_t index = 0; index < records.size(); ++index)
        {
            const auto& record = records[index];
            if (record.fileIndex != 0u || record.offsetEnd == 0u)
                continue;
            REQUIRE(record.offsetEnd > record.offset);
            if (index % XAssetStreamFilesForImage{}.size() == 0u)
                CHECK(record.offsetEnd <= loadEnd);
            else
                CHECK(record.offset >= loadEnd);
            ranges.emplace_back(record.offset, record.offsetEnd);
        }
        std::ranges::sort(ranges);
        auto offset = zoneEnd;
        for (const auto& [start, end] : ranges)
        {
            CHECK(start == offset);
            offset = end;
        }
        CHECK(offset == fileEnd);
    }

    std::unique_ptr<Zone> RoundTripZone(const Zone& zone, MockSearchPath* searchPath = nullptr)
    {
        const auto path = oat::paths::GetTempDirectory("IW4Xenon482Images") / (zone.m_name + ".ff");
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, zone));
        }
        if (searchPath)
        {
            std::ifstream input(path, std::ios::binary);
            searchPath->AddFileData(zone.m_name + ".ff", std::string(std::istreambuf_iterator<char>(input), {}));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        return std::move(*loaded);
    }

    TEST_CASE("IW4 Xenon 482 imagefile images retain all faces and mips through native fastfiles", "[iw4xenon482][image][imagefile][system]")
    {
        RestoreObjWritingConfiguration restore;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(IW4Xenon482::ASSET_TYPE_COUNT, true);
        ObjWriting::Configuration.ImageOutputFormat = ImageOutputFormat_e::DDS;
        const auto index = GENERATE(0u, 5u, 20u);
        const auto iwi = GENERATE(false, true);
        const auto cube = GENERATE(false, true);
        const auto mipMaps = GENERATE(false, true);
        const auto format = GENERATE(image::ImageFormatId::BC1, image::ImageFormatId::BC3, image::ImageFormatId::B8_G8_R8_A8);
        CAPTURE(index, iwi, cube, mipMaps, format);
        auto source = image::Texture::CreateForType(
            cube ? image::TextureType::T_CUBE : image::TextureType::T_2D, image::ImageFormat::GetImageFormatById(format), 64u, 64u, 1u, mipMaps);
        FillTexture(*source);
        std::ostringstream file(std::ios::binary);
        if (iwi)
            image::iwi8::IwiWriter().DumpImage(file, source.get());
        else
            image::DdsWriter().DumpImage(file, source.get());

        MockSearchPath searchPath;
        for (const auto* name : {"first", "second"})
            searchPath.AddFileData(std::string("images/") + name + (iwi ? ".iwi" : ".dds"), file.str());
        Zone zone("imagefile_images", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        ZoneDefinition definition;
        definition.m_properties.AddProperty("imagefile", std::to_string(index));
        ZoneDefinitionContext zoneDefinition(definition);
        AssetCreatorCollection creators(zone);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(zone, &creators, &ignored);
        MockOutputPath output;
        GdtLookup gdt;
        IObjCompiler::GetObjCompilerForGame(GameId::IW4Xenon482)
            ->ConfigureCreatorCollection(creators, zone, zoneDefinition, searchPath, gdt, context, output, output);
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        for (const auto* name : {"first", "second"})
        {
            const auto* info = context.LoadDependency<IW4Xenon482::AssetImage>(name);
            REQUIRE(info);
            CHECK(info->m_zone == &zone);
            CHECK(info->Asset()->cached == 1u);
            CHECK(info->Asset()->pixels == nullptr);
        }
        creators.FinalizeZone(context);
        const auto& first = zone.m_xfile_image_stream_files_by_name.at("first").front()[0];
        const auto& second = zone.m_xfile_image_stream_files_by_name.at("second").front()[0];
        CHECK(first.fileIndex == index);
        CHECK(first.offset == 0u);
        CHECK(second.offset == first.offsetEnd);
        if (index == 0u)
        {
            CHECK(output.GetMockedFileList().empty());
            CHECK(second.offsetEnd == zone.m_xfile_image_stream_data.size());
        }
        else
        {
            REQUIRE(output.GetMockedFileList().size() == 1u);
            const auto fileName = "imagefile" + std::to_string(index) + ".pakm";
            const auto* pak = output.GetMockedFile(fileName);
            REQUIRE(pak);
            CHECK(second.offsetEnd == pak->m_data.size());
            searchPath.AddFileData(fileName, pak->AsString());
        }
        const auto loaded = RoundTripZone(zone, &searchPath);
        REQUIRE(loaded->m_pools.GetTotalAssetCount() == 2u);
        if (index == 0u)
            CheckAppendedStreams(*loaded);
        for (const auto* name : {"first", "second"})
        {
            const auto* info = loaded->m_pools.GetAsset<IW4Xenon482::AssetImage>(name);
            REQUIRE(info);
            const auto decoded = image::ToCommonConverterIW4Xenon482().Convert(*info, searchPath);
            REQUIRE(decoded);
            CheckTexture(*decoded, *source);
        }
        MockOutputPath dumped;
        AssetDumpingContext dumping(*loaded, "", dumped, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4Xenon482)->DumpZone(dumping));
        REQUIRE(dumped.GetMockedFileList().size() == 2u);
        for (const auto& dumpedFile : dumped.GetMockedFileList())
        {
            std::istringstream input(dumpedFile.AsString());
            const auto texture = image::LoadDds(input);
            REQUIRE(texture);
            CheckTexture(*texture, *source);
        }
    }

    TEST_CASE("IW4 Xenon 482 appended images relink from the loaded fastfile path", "[iw4xenon482][image][imagefile][system]")
    {
        image::Texture2D texture(&image::format::BC1, 64u, 64u, true);
        FillTexture(texture);
        image::xenon::XenonTextureEncodeResult encoded;
        REQUIRE(image::xenon::EncodeTexture(texture, encoded));
        auto compressedSize = compressBound(static_cast<uLong>(encoded.pixels.size()));
        std::vector<unsigned char> compressed(compressedSize);
        REQUIRE(compress2(compressed.data(), &compressedSize, encoded.pixels.data(), static_cast<uLong>(encoded.pixels.size()), Z_DEFAULT_COMPRESSION) == Z_OK);
        compressed.resize(compressedSize);
        std::unique_ptr<Zone> loaded;
        {
            Zone source("appended_source", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
            IW4Xenon482::GfxImage image{};
            image.name = "preloaded_image";
            image.format = static_cast<int>(encoded.rawFormat);
            image.mapType = IW4Xenon482::MAPTYPE_2D;
            image.width = image.height = image.depth = image.levelCount = image.cached = 1u;
            image.streams[0] = {64u, 64u, static_cast<unsigned>(encoded.pixels.size()) | (encoded.levelCount << 26u)};
            source.m_xfile_image_stream_data = compressed;
            XAssetStreamFilesForImage records{};
            records[0] = {0u, 0u, static_cast<unsigned>(compressed.size())};
            source.m_xfile_image_stream_files_by_name[image.name].emplace_back(records);
            source.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
            loaded = RoundTripZone(source);
        }
        loaded->m_name = "renamed_source";
        MockSearchPath searchPath;
        const auto* sourceImage = loaded->m_pools.GetAsset<IW4Xenon482::AssetImage>("preloaded_image");
        REQUIRE(sourceImage);
        const auto sourceTexture = image::ToCommonConverterIW4Xenon482().Convert(*sourceImage, searchPath);
        REQUIRE(sourceTexture);
        CheckTexture(*sourceTexture, texture);

        Zone target("appended_target", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        AssetCreatorCollection creators(target);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, target, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        const auto* imported = context.LoadDependency<IW4Xenon482::AssetImage>("preloaded_image");
        REQUIRE(imported);
        REQUIRE(imported->m_zone == loaded.get());
        CHECK(target.m_xfile_image_stream_data == compressed);
        const auto relinked = RoundTripZone(target);
        CheckAppendedStreams(*relinked);
        const auto* relinkedImage = relinked->m_pools.GetAsset<IW4Xenon482::AssetImage>("preloaded_image");
        REQUIRE(relinkedImage);
        const auto decoded = image::ToCommonConverterIW4Xenon482().Convert(*relinkedImage, searchPath);
        REQUIRE(decoded);
        CheckTexture(*decoded, texture);
    }

    TEST_CASE("IW4 Xenon 482 embedded images dump to DDS and IWI8", "[iw4xenon482][image][embedded][system]")
    {
        RestoreObjWritingConfiguration restore;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(IW4Xenon482::ASSET_TYPE_COUNT, true);
        ObjWriting::Configuration.ImageOutputFormat = GENERATE(ImageOutputFormat_e::DDS, ImageOutputFormat_e::IWI);
        const auto cube = GENERATE(false, true);
        auto source =
            image::Texture::CreateForType(cube ? image::TextureType::T_CUBE : image::TextureType::T_2D, &image::format::B8_G8_R8_A8, 64u, 64u, 1u, true);
        FillTexture(*source);
        image::xenon::XenonTextureEncodeResult encoded;
        REQUIRE(image::xenon::EncodeTexture(*source, encoded));
        IW4Xenon482::GfxImage image{};
        std::memcpy(image.texture.data, encoded.textureHeader.data(), encoded.textureHeader.size());
        image.name = "embedded";
        image.format = static_cast<int>(encoded.rawFormat);
        image.mapType = cube ? IW4Xenon482::MAPTYPE_CUBE : IW4Xenon482::MAPTYPE_2D;
        image.width = image.height = 64u;
        image.depth = 1u;
        image.levelCount = static_cast<unsigned char>(encoded.levelCount);
        image.cardMemory.platform[0] = static_cast<int>(encoded.pixels.size());
        image.pixels = encoded.pixels.data();
        Zone zone("embedded_image", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        zone.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        const auto loaded = RoundTripZone(zone);
        MockSearchPath searchPath;
        MockOutputPath output;
        AssetDumpingContext dumping(*loaded, "", output, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4Xenon482)->DumpZone(dumping));
        REQUIRE(output.GetMockedFileList().size() == 1u);
        std::istringstream input(output.GetMockedFileList().front().AsString());
        std::unique_ptr<image::Texture> decoded;
        if (ObjWriting::Configuration.ImageOutputFormat == ImageOutputFormat_e::DDS)
            decoded = image::LoadDds(input);
        else
        {
            auto iwi = image::LoadIwi(input);
            REQUIRE(iwi);
            CHECK(iwi->m_version == image::IwiVersion::IWI_8);
            decoded = std::move(iwi->m_texture);
        }
        REQUIRE(decoded);
        CheckTexture(*decoded, *source);
    }
#endif

    TEST_CASE("IW4 Xenon 482 disk images require one valid imagefile directive", "[iw4xenon482][image][imagefile][system]")
    {
        const auto directive = GENERATE("missing", "", "21", "5garbage", "-1", "duplicate");
        Zone zone("invalid_imagefile", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        ZoneDefinition definition;
        if (std::string(directive) != "missing")
            definition.m_properties.AddProperty("imagefile", directive);
        if (std::string(directive) == "duplicate")
            definition.m_properties.AddProperty("imagefile", "5");
        ZoneDefinitionContext zoneDefinition(definition);
        MockSearchPath searchPath;
        image::Texture2D texture(&image::format::BC1, 64u, 64u, true);
        FillTexture(texture);
        std::ostringstream file(std::ios::binary);
        image::DdsWriter().DumpImage(file, &texture);
        searchPath.AddFileData("images/test.dds", file.str());
        AssetCreatorCollection creators(zone);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(zone, &creators, &ignored);
        MockOutputPath output;
        GdtLookup gdt;
        IObjCompiler::GetObjCompilerForGame(GameId::IW4Xenon482)
            ->ConfigureCreatorCollection(creators, zone, zoneDefinition, searchPath, gdt, context, output, output);
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        CHECK_FALSE(creators.CreateAsset(IW4Xenon482::ASSET_TYPE_IMAGE, "test", context).HasTakenAction());
        creators.FinalizeZone(context);
        CHECK(output.GetMockedFileList().empty());
    }

    TEST_CASE("IW4 Xenon 482 native streams retain their source zone and complete mip chain", "[iw4xenon482][image][streamed][system]")
    {
        const auto state = GENERATE("valid", "appended", "missing", "truncated", "corrupt", "invalid_range");
        const auto appended = std::string(state) == "appended";
        image::Texture2D source(&image::format::BC1, 512u, 512u, true);
        FillTexture(source);
        image::xenon::XenonTextureEncodeResult encoded;
        REQUIRE(image::xenon::EncodeTexture(source, encoded));
        Zone sourceZone("native_streams", 1, GameId::IW4Xenon482, GamePlatform::XBOX);
        IW4Xenon482::GfxImage image{};
        image.name = "streamed_image";
        image.format = static_cast<int>(encoded.rawFormat);
        image.mapType = IW4Xenon482::MAPTYPE_2D;
        image.width = image.height = image.depth = 1u;
        image.levelCount = 1u;
        image.cached = 1u;
        XAssetStreamFilesForImage records{};
        MockSearchPath searchPath;
        std::string fastfile(16u, '\0');
        size_t previousSize = 0u;
        for (auto slot = 0u; slot < 4u; ++slot)
        {
            const auto width = 64u << slot;
            image::Texture2D lower(&image::format::BC1, width, width, true);
            FillTexture(lower);
            image::xenon::XenonTextureEncodeResult lowerEncoded;
            REQUIRE(image::xenon::EncodeTexture(lower, lowerEncoded));
            const auto size = lowerEncoded.pixels.size();
            image.streams[slot] = {static_cast<uint16_t>(width), static_cast<uint16_t>(width), static_cast<unsigned>(size) | (lowerEncoded.levelCount << 26u)};
            auto compressedSize = compressBound(static_cast<uLong>(size - previousSize));
            std::string compressed(compressedSize, '\0');
            REQUIRE(compress2(reinterpret_cast<Bytef*>(compressed.data()),
                              &compressedSize,
                              encoded.pixels.data() + encoded.pixels.size() - size,
                              static_cast<uLong>(size - previousSize),
                              Z_DEFAULT_COMPRESSION)
                    == Z_OK);
            compressed.resize(compressedSize);
            const auto fileIndex = appended ? 0u : slot;
            const auto offset = fileIndex == 0u ? static_cast<unsigned>(fastfile.size()) : 16u;
            records[slot] = {fileIndex, offset, static_cast<unsigned>(compressedSize) + offset};
            if (slot == 3u)
            {
                if (std::string(state) == "missing")
                    continue;
                if (std::string(state) == "truncated")
                    compressed.pop_back();
                if (std::string(state) == "corrupt")
                    compressed[0] ^= 0xFF;
                if (std::string(state) == "invalid_range")
                    records[slot].offsetEnd = records[slot].offset - 1u;
            }
            if (fileIndex == 0u)
                fastfile += compressed;
            else
                searchPath.AddFileData("imagefile" + std::to_string(fileIndex) + ".pakm", std::string(16u, '\0') + compressed);
            previousSize = size;
        }
        searchPath.AddFileData("native_streams.ff", fastfile);
        sourceZone.m_xfile_image_stream_files_by_name[image.name].emplace_back(records);
        sourceZone.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_IMAGE, image.name, &image, {}, {}, {});
        IW4Xenon482::GfxImage second = image;
        if (appended)
        {
            second.name = "second_streamed_image";
            sourceZone.m_xfile_image_stream_files_by_name[second.name].emplace_back(records);
            sourceZone.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_IMAGE, second.name, &second, {}, {}, {});
        }

        Zone target("other_zone", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        AssetCreatorCollection creators(target);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, target, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        const auto* info = context.LoadDependency<IW4Xenon482::AssetImage>(image.name);
        REQUIRE(info);
        REQUIRE(info->m_zone == &sourceZone);
        if (appended)
            REQUIRE(context.LoadDependency<IW4Xenon482::AssetImage>(second.name));
        const auto texture = image::ToCommonConverterIW4Xenon482().Convert(*info, searchPath);
        if (std::string(state) == "valid" || appended)
        {
            REQUIRE(texture);
            CheckTexture(*texture, source);
#ifdef ARCH_x86
            const auto relinked = RoundTripZone(target, &searchPath);
            const auto* relinkedImage = relinked->m_pools.GetAsset<IW4Xenon482::AssetImage>(image.name);
            REQUIRE(relinkedImage);
            const auto decoded = image::ToCommonConverterIW4Xenon482().Convert(*relinkedImage, searchPath);
            REQUIRE(decoded);
            CheckTexture(*decoded, source);
            CheckAppendedStreams(*relinked);
            if (appended)
            {
                const auto* secondImage = relinked->m_pools.GetAsset<IW4Xenon482::AssetImage>(second.name);
                REQUIRE(secondImage);
                const auto secondDecoded = image::ToCommonConverterIW4Xenon482().Convert(*secondImage, searchPath);
                REQUIRE(secondDecoded);
                CheckTexture(*secondDecoded, source);
            }
#endif
        }
        else
            CHECK_FALSE(texture);
    }
} // namespace
