#include "Game/IW3Xenon/ObjConstantsIW3Xenon.h"
#include "Game/IW3Xenon/Techset/PixelShaderDumperIW3Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Shader/XenonShader.h"
#include "Utils/Endianness.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <fstream>
#include <span>

#ifdef ARCH_x86

using namespace IW3Xenon;

namespace
{
    std::string Microcode(const uint32_t cachedSize, const uint32_t physicalSize, const bool vertex = false)
    {
        std::string bytes(static_cast<size_t>(cachedSize) + physicalSize, '\x5a');
        const std::array header{
            endianness::ToBigEndian(0x102a1100u | static_cast<unsigned>(vertex)), endianness::ToBigEndian(cachedSize), endianness::ToBigEndian(physicalSize)};
        std::memcpy(bytes.data(), header.data(), sizeof(header));
        return bytes;
    }

    struct ShaderContext
    {
        Zone zone{"pixel_shader", 0, GameId::IW3Xenon, GamePlatform::XBOX};
        MockSearchPath searchPath;
        GdtLookup gdt;
        AssetCreatorCollection creators{zone};
        IgnoredAssetLookup ignored;
        AssetCreationContext context{zone, &creators, &ignored};

        ShaderContext()
        {
            IObjLoader::GetObjLoaderForGame(GameId::IW3Xenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        }
    };

    void CheckShader(const Zone& zone, const std::string& name, const std::string& data, const uint32_t physicalSize)
    {
        const auto* asset = zone.m_pools.GetAsset<AssetPixelShader>(name);
        REQUIRE(asset);
        const auto& loadDef = asset->Asset()->prog.loadDef;
        const auto cachedSize = data.size() - physicalSize;
        REQUIRE(loadDef.cachedPart);
        REQUIRE(loadDef.cachedPartSize == PIXEL_SHADER_RUNTIME_HEADER_SIZE + cachedSize);
        CHECK(std::ranges::all_of(std::span(loadDef.cachedPart, PIXEL_SHADER_RUNTIME_HEADER_SIZE),
                                  [](uint8_t byte)
                                  {
                                      return byte == 0;
                                  }));
        CHECK(std::memcmp(loadDef.cachedPart + PIXEL_SHADER_RUNTIME_HEADER_SIZE, data.data(), cachedSize) == 0);
        REQUIRE(loadDef.physicalPartSize == physicalSize);
        if (physicalSize)
        {
            REQUIRE(loadDef.physicalPart);
            CHECK(std::memcmp(loadDef.physicalPart, data.data() + cachedSize, physicalSize) == 0);
        }
        else
            CHECK(loadDef.physicalPart == nullptr);
    }

    TEST_CASE("IW3 Xenon pixel shaders round-trip native microcode through fastfiles", "[iw3xenon][system][shader]")
    {
        const auto physicalSize = GENERATE(0u, 32u);
        const std::string name = "effects/test*variant.hlsl";
        const std::string fileName = "shader_bin/ps_effects~2Ftest~2Avariant.hlsl.xpu";
        const auto data = Microcode(32, physicalSize);
        ShaderContext source;
        source.searchPath.AddFileData(fileName, data);
        REQUIRE(source.creators.CreateAsset(ASSET_TYPE_PIXELSHADER, name, source.context).HasBeenSuccessful());
        CheckShader(source.zone, name, data, physicalSize);

        const auto path = oat::paths::GetTempDirectory("IW3XenonPixelShaderRoundTrip") / "shader.ff";
        {
            std::ofstream fastfile(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(fastfile, source.zone));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        CheckShader(**loaded, name, data, physicalSize);

        MockOutputPath output;
        const std::string basePath;
        AssetDumpingContext context(**loaded, basePath, output, source.searchPath, std::nullopt);
        techset::PixelShaderDumperIW3Xenon().Dump(context);
        const auto* shaderFile = output.GetMockedFile(fileName);
        REQUIRE(shaderFile);
        CHECK(shaderFile->AsString() == data);
    }

    TEST_CASE("IW3 Xenon pixel shader loading rejects malformed microcode", "[iw3xenon][system][shader]")
    {
        auto data = Microcode(32, 32);
        SECTION("wrong shader stage")
        {
            data = Microcode(32, 32, true);
        }
        SECTION("truncated header")
        {
            data.resize(8);
        }
        SECTION("truncated physical data")
        {
            data.pop_back();
        }
        SECTION("cached part exceeds its native size field")
        {
            data = Microcode(65500, 0);
        }
        SECTION("physical part exceeds its native size field")
        {
            data = Microcode(32, 65536);
        }
        SECTION("empty file")
        {
            data.clear();
        }
        ShaderContext source;
        source.searchPath.AddFileData("shader_bin/ps_invalid.xpu", data);
        CHECK(source.creators.CreateAsset(ASSET_TYPE_PIXELSHADER, "invalid", source.context).HasFailed());
    }

    TEST_CASE("IW3 Xenon supports the empty null pixel shader without an xpu file", "[iw3xenon][system][shader]")
    {
        ShaderContext source;
        CHECK_FALSE(source.creators.CreateAsset(ASSET_TYPE_PIXELSHADER, "missing", source.context).HasTakenAction());
        REQUIRE(source.creators.CreateAsset(ASSET_TYPE_PIXELSHADER, "null.hlsl", source.context).HasBeenSuccessful());
        const auto* shader = source.zone.m_pools.GetAsset<AssetPixelShader>("null.hlsl");
        REQUIRE(shader);
        CHECK(shader->Asset()->prog.loadDef.cachedPartSize == 0);
        CHECK(shader->Asset()->prog.loadDef.physicalPartSize == 0);

        MockOutputPath output;
        const std::string basePath;
        AssetDumpingContext context(source.zone, basePath, output, source.searchPath, std::nullopt);
        techset::PixelShaderDumperIW3Xenon().Dump(context);
        CHECK(output.GetMockedFile("shader_bin/ps_null.hlsl.xpu") == nullptr);
    }
} // namespace

#endif
