#include "Game/IW3Xenon/Techset/TechsetCompilerIW3Xenon.h"
#include "Game/IW3Xenon/Techset/TechsetConstantsIW3Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Shader/XenonShader.h"
#include "Utils/Endianness.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <format>
#include <fstream>

#ifdef ARCH_x86

using namespace IW3Xenon;

namespace
{
    struct RestoreWritingConfiguration
    {
        ObjWriting::Configuration_t original{ObjWriting::Configuration};

        ~RestoreWritingConfiguration()
        {
            ObjWriting::Configuration = original;
        }
    };

    std::string Microcode(const shader::xenon::ShaderStage stage)
    {
        std::string bytes(64, '\x5a');
        const std::array header{
            endianness::ToBigEndian(0x102a1100u | static_cast<unsigned>(stage)), endianness::ToBigEndian(32u), endianness::ToBigEndian(32u)};
        std::memcpy(bytes.data(), header.data(), sizeof(header));
        return bytes;
    }

    void AddShader(MockSearchPath& searchPath, const std::string& name, const shader::xenon::ShaderStage stage)
    {
        const bool pixel = stage == shader::xenon::ShaderStage::PIXEL;
        const auto path = pixel ? shader::xenon::GetFileNameForPixelShaderAssetName(name) : shader::xenon::GetFileNameForVertexShaderAssetName(name);
        searchPath.AddFileData(path, Microcode(stage));
    }

    std::string ReflectedMicrocode(const shader::xenon::ShaderStage stage)
    {
        std::string bytes(224, '\0');
        const auto word = [&bytes](const size_t offset, const uint32_t value)
        {
            const auto bigEndian = endianness::ToBigEndian(value);
            std::memcpy(bytes.data() + offset, &bigEndian, sizeof(bigEndian));
        };
        const auto half = [&bytes](const size_t offset, const uint16_t value)
        {
            const auto bigEndian = endianness::ToBigEndian(value);
            std::memcpy(bytes.data() + offset, &bigEndian, sizeof(bigEndian));
        };
        word(0, 0x102a1100u | static_cast<unsigned>(stage));
        word(4, 192);
        word(8, 32);
        word(16, 36); // Size-prefixed constant table.
        word(36, 152);
        constexpr size_t table = 40;
        word(table, 28);
        word(table + 4, 128);
        word(table + 8, stage == shader::xenon::ShaderStage::VERTEX ? 0xfffe0300u : 0xffff0300u);
        word(table + 12, 2);
        word(table + 16, 28);
        word(table + 24, 136);
        word(table + 28, 100);
        half(table + 32, static_cast<uint16_t>(d3d9::RegisterSet::FLOAT_4));
        half(table + 34, 4);
        half(table + 36, 8);
        word(table + 40, 68);
        word(table + 48, 112);
        half(table + 52, static_cast<uint16_t>(d3d9::RegisterSet::SAMPLER));
        half(table + 54, 3);
        half(table + 56, 1);
        word(table + 60, 84);
        half(table + 68, static_cast<uint16_t>(d3d9::ParameterClass::MATRIX_COLUMNS));
        half(table + 70, static_cast<uint16_t>(d3d9::ParameterType::FLOAT));
        half(table + 72, 4);
        half(table + 74, 4);
        half(table + 76, 2);
        half(table + 84, static_cast<uint16_t>(d3d9::ParameterClass::OBJECT));
        half(table + 86, static_cast<uint16_t>(d3d9::ParameterType::SAMPLER_2D));
        half(table + 88, 1);
        half(table + 90, 1);
        half(table + 92, 1);
        std::memcpy(bytes.data() + table + 100, "worldMatrix", 12);
        std::memcpy(bytes.data() + table + 112, "colorMapSampler", 16);
        std::memcpy(bytes.data() + table + 128, "test", 5);
        std::memcpy(bytes.data() + table + 136, stage == shader::xenon::ShaderStage::VERTEX ? "vs_3_0" : "ps_3_0", 7);
        return bytes;
    }

    std::string TechniqueDocument()
    {
        const std::string pass = R"({
    stateMap "passthrough";
    xenonArgumentCounts 3, 2, 3;
    xenonCustomSamplerFlags 7;
    xenonPrecompiledIndex 42;
    xenonOptionalSource 1;
    xenonVertexShaders
    {
        generic "vertex*generic";
        packed "vertex*packed";
        world "vertex*generic";
    }
    vertexShader 3.0 "vertex*generic"
    {
        xenonArg 0, 1, 0, 0;
        register[1] = material.#0x12345678;
        xenonArg 1, 2, 0, 0;
        register[2] = float4(1.25, -2.5, 3.75, 4);
        xenonArg 3, 4, 1, 4;
        register[4] = constant.#0x39;
    }
    pixelShader 3.0 "pixel"
    {
        xenonArg 2, 3, 0, 0;
        samplerRegister[3] = material.#0x87654321;
        xenonArg 4, 5, 0, 0;
        samplerRegister[5] = sampler.dynamicShadowSampler;
        xenonArg 5, 6, 0, 1;
        register[6] = constant.#0x22;
        xenonArg 6, 7, 0, 0;
        register[7] = material.#0xfeedcafe;
        xenonArg 7, 8, 0, 0;
        register[8] = float4(5, 6.25, -7.5, 8.75);
    }
    vertex.position = code.position;
    vertex.texcoord[0] = code.texcoord[1];
}
)";
        return "xenonTechniqueFlags 4660;\n" + pass + pass;
    }

    struct ShaderParameter
    {
        const char* name;
        unsigned destination;
        unsigned registers = 1;
        unsigned elements = 1;
        d3d9::ParameterClass parameterClass = d3d9::ParameterClass::VECTOR;
    };

    std::string TechniqueMicrocode(const shader::xenon::ShaderStage stage, const std::vector<ShaderParameter>& parameters)
    {
        constexpr size_t table = 40;
        const auto typeOffset = 28 + 20 * parameters.size();
        const auto stringOffset = typeOffset + 16 * parameters.size();
        std::string strings("test\0", 5);
        const auto targetOffset = stringOffset + strings.size();
        strings += std::string(stage == shader::xenon::ShaderStage::VERTEX ? "vs_3_0\0" : "ps_3_0\0", 7);
        std::vector<size_t> nameOffsets;
        for (const auto& parameter : parameters)
        {
            nameOffsets.push_back(stringOffset + strings.size());
            strings += parameter.name;
            strings += '\0';
        }
        const auto cachedSize = table + stringOffset + strings.size();
        std::string bytes(cachedSize + 32, '\0');
        const auto word = [&bytes](const size_t offset, const uint32_t value)
        {
            const auto bigEndian = endianness::ToBigEndian(value);
            std::memcpy(bytes.data() + offset, &bigEndian, sizeof(bigEndian));
        };
        const auto half = [&bytes](const size_t offset, const uint16_t value)
        {
            const auto bigEndian = endianness::ToBigEndian(value);
            std::memcpy(bytes.data() + offset, &bigEndian, sizeof(bigEndian));
        };
        word(0, 0x102a1100u | static_cast<unsigned>(stage));
        word(4, static_cast<uint32_t>(cachedSize));
        word(8, 32);
        word(16, 36);
        word(36, static_cast<uint32_t>(cachedSize - table));
        word(table, 28);
        word(table + 4, static_cast<uint32_t>(stringOffset));
        word(table + 8, stage == shader::xenon::ShaderStage::VERTEX ? 0xfffe0300u : 0xffff0300u);
        word(table + 12, static_cast<uint32_t>(parameters.size()));
        word(table + 16, 28);
        word(table + 24, static_cast<uint32_t>(targetOffset));
        for (auto i = 0uz; i < parameters.size(); ++i)
        {
            const auto& parameter = parameters[i];
            const bool sampler = parameter.parameterClass == d3d9::ParameterClass::OBJECT;
            const auto info = table + 28 + i * 20;
            const auto type = table + typeOffset + i * 16;
            word(info, static_cast<uint32_t>(nameOffsets[i]));
            half(info + 4, static_cast<uint16_t>(sampler ? d3d9::RegisterSet::SAMPLER : d3d9::RegisterSet::FLOAT_4));
            half(info + 6, static_cast<uint16_t>(parameter.destination));
            half(info + 8, static_cast<uint16_t>(parameter.registers));
            word(info + 12, static_cast<uint32_t>(typeOffset + i * 16));
            half(type, static_cast<uint16_t>(parameter.parameterClass));
            half(type + 2, static_cast<uint16_t>(sampler ? d3d9::ParameterType::SAMPLER_2D : d3d9::ParameterType::FLOAT));
            half(type + 4,
                 static_cast<uint16_t>(parameter.parameterClass == d3d9::ParameterClass::MATRIX_COLUMNS ? 4 : parameter.registers / parameter.elements));
            half(type + 6, 4);
            half(type + 8, static_cast<uint16_t>(parameter.elements));
        }
        std::memcpy(bytes.data() + table + stringOffset, strings.data(), strings.size());
        return bytes;
    }

    std::string TechsetDocument()
    {
        auto techset = std::format("xenonWorldVertexFormat {};\nxenonUnused 17, 23;\n", static_cast<unsigned>(MTL_WORLDVERT_TEX_2_NRM_1));
        for (const auto* slot : techniqueTypeNames)
            techset += std::format("\"{}\":\n", slot);
        techset += "test_technique;\n";
        return techset;
    }

    MockSearchPath SourceFiles(MockSearchPath searchPath = {})
    {
        searchPath.AddFileData("techsets/test_techset.techset", TechsetDocument());
        searchPath.AddFileData("techniques/test_technique.tech", TechniqueDocument());
        AddShader(searchPath, "pixel", shader::xenon::ShaderStage::PIXEL);
        AddShader(searchPath, "vertex*generic", shader::xenon::ShaderStage::VERTEX);
        AddShader(searchPath, "vertex*packed", shader::xenon::ShaderStage::VERTEX);
        return searchPath;
    }

    bool LoadTechset(Zone& zone, MockSearchPath& searchPath)
    {
        GdtLookup gdt;
        AssetCreatorCollection creators(zone);
        IObjLoader::GetObjLoaderForGame(zone.m_game_id)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        creators.AddAssetCreator(techset::CreateTechsetCompilerIW3Xenon(zone.Memory(), searchPath));
        creators.AddSubAssetCreator(techset::CreateTechniqueCompilerIW3Xenon(zone.Memory(), searchPath));
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        return creators.CreateAsset(ASSET_TYPE_TECHNIQUE_SET, "test_techset", context).HasBeenSuccessful();
    }

    MockOutputPath DumpTechset(const Zone& zone)
    {
        MockOutputPath output;
        MockSearchPath searchPath;
        AssetDumpingContext context(zone, "", output, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(zone.m_game_id)->DumpZone(context));
        return output;
    }

    void CheckTechset(const Zone& zone)
    {
        const auto* asset = zone.m_pools.GetAsset<AssetTechniqueSet>("test_techset");
        REQUIRE(asset);
        const auto& techset = *asset->Asset();
        CHECK(techset.worldVertFormat == MTL_WORLDVERT_TEX_2_NRM_1);
        CHECK(techset.unused[0] == 17);
        CHECK(techset.unused[1] == 23);
        const auto* technique = techset.techniques[0];
        REQUIRE(technique);
        for (const auto* slot : techset.techniques)
            CHECK(slot == technique);
        CHECK(technique->flags == 0x1234);
        REQUIRE(technique->passCount == 2);
        for (const auto& pass : std::span(technique->passArray, technique->passCount))
        {
            REQUIRE(pass.vertexDecl);
            CHECK(pass.vertexDecl->hasOptionalSource);
            CHECK(pass.vertexDecl->routing.data[1].source == STREAM_SRC_TEXCOORD_1);
            CHECK(pass.precompiledIndex == 42);
            CHECK(pass.customSamplerFlags == 7);
            CHECK(pass.perPrimArgCount == 3);
            CHECK(pass.perObjArgCount == 2);
            CHECK(pass.stableArgCount == 3);
            for (auto i = 0u; i < 8u; ++i)
            {
                CHECK(pass.args[i].type == i);
                CHECK(pass.args[i].dest == i + 1);
            }
            CHECK(pass.vertexShaderArray[VERTDECL_GENERIC] == pass.vertexShader);
            CHECK(pass.vertexShaderArray[VERTDECL_WORLD] == pass.vertexShader);
            REQUIRE(pass.vertexShaderArray[VERTDECL_PACKED]);
            CHECK(pass.vertexShaderArray[VERTDECL_PACKED] != pass.vertexShader);
            REQUIRE(pass.pixelShader);
            const auto& loadDef = pass.vertexShader->prog.loadDef;
            REQUIRE(loadDef.cachedPartSize == VERTEX_SHADER_RUNTIME_HEADER_SIZE + 32u);
            CHECK(std::ranges::all_of(std::span(loadDef.cachedPart, VERTEX_SHADER_RUNTIME_HEADER_SIZE),
                                      [](uint8_t byte)
                                      {
                                          return byte == 0;
                                      }));
            const auto& pixel = pass.pixelShader->prog.loadDef;
            REQUIRE(pixel.cachedPartSize == PIXEL_SHADER_RUNTIME_HEADER_SIZE + 32u);
            CHECK(std::ranges::all_of(std::span(pixel.cachedPart, PIXEL_SHADER_RUNTIME_HEADER_SIZE),
                                      [](uint8_t byte)
                                      {
                                          return byte == 0;
                                      }));
            CHECK(pass.args[3].u.codeConst.index == 57);
            CHECK(pass.args[3].u.codeConst.firstRow == 1);
            CHECK(pass.args[3].u.codeConst.rowCount == 4);
            CHECK(pass.args[4].u.codeSampler == TEXTURE_SRC_CODE_DYNAMIC_SHADOWS);
            CHECK(pass.args[6].u.nameHash == 0xfeedcafeu);
            CHECK((*pass.args[7].u.literalConst)[2] == -7.5f);
        }
    }
} // namespace

TEST_CASE("IW3 Xenon techniques and SDK shader objects round trip through native fastfiles", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_PIXELSHADER] = true;
    auto source = SourceFiles();
    Zone zone("xenon_techset", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    CheckTechset(zone);
    const auto path = oat::paths::GetTempDirectory("IW3XenonTechsetRoundTrip") / "techset.ff";
    {
        std::ofstream fastfile(path, std::ios::binary);
        REQUIRE(ZoneWriting::WriteZone(fastfile, zone));
    }
    const auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
    REQUIRE(loaded);
    CheckTechset(**loaded);
    const auto dumped = DumpTechset(**loaded);
    const auto* techset = dumped.GetMockedFile("techsets/test_techset.techset");
    REQUIRE(techset);
    CHECK(techset->AsString().find("\"unlit\":") != std::string::npos);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("xenonPrecompiledIndex 42;") != std::string::npos);
    const auto* vertex = dumped.GetMockedFile("shader_bin/vs_vertex~2Ageneric.xvu");
    REQUIRE(vertex);
    CHECK(vertex->AsString() == Microcode(shader::xenon::ShaderStage::VERTEX));
    const auto* pixel = dumped.GetMockedFile("shader_bin/ps_pixel.xpu");
    REQUIRE(pixel);
    CHECK(pixel->AsString() == Microcode(shader::xenon::ShaderStage::PIXEL));
    CHECK(dumped.GetMockedFileList().size() == 5u);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("xenon_reloaded_techset", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    CheckTechset(reloaded);
    const auto redumped = DumpTechset(reloaded);
    for (const auto& file : dumped.GetMockedFileList())
    {
        const auto* actual = redumped.GetMockedFile(file.m_name);
        REQUIRE(actual);
        CHECK(actual->m_data == file.m_data);
    }
}

TEST_CASE("IW3 Xenon techniques reject invalid native metadata and shader files", "[iw3xenon][system][techset]")
{
    MockSearchPath source;
    auto technique = TechniqueDocument();
    const auto replace = [&technique](const std::string& from, const std::string& to)
    {
        const auto offset = technique.find(from);
        REQUIRE(offset != std::string::npos);
        technique.replace(offset, from.size(), to);
    };
    SECTION("argument group overflow")
    {
        replace("xenonArgumentCounts 3, 2, 3", "xenonArgumentCounts 256, 2, 3");
    }
    SECTION("argument group mismatch")
    {
        replace("xenonArgumentCounts 3, 2, 3", "xenonArgumentCounts 3, 2, 2");
    }
    SECTION("unknown vertex shader variant")
    {
        replace("packed \"vertex*packed\"", "unknown \"vertex*packed\"");
    }
    SECTION("invalid stream routing")
    {
        replace("code.position", "code.unknown");
    }
    SECTION("code constant index overflow")
    {
        replace("constant.#0x39", "constant.#0x10000");
    }
    SECTION("wrong shader stage")
    {
        source.AddFileData("shader_bin/ps_pixel.xpu", Microcode(shader::xenon::ShaderStage::VERTEX));
    }
    SECTION("truncated shader")
    {
        source.AddFileData("shader_bin/ps_pixel.xpu", Microcode(shader::xenon::ShaderStage::PIXEL).substr(0, 20));
    }
    SECTION("unsupported state map")
    {
        replace("stateMap \"passthrough\"", "stateMap \"default\"");
    }
    source.AddFileData("techniques/test_technique.tech", technique);
    source = SourceFiles(std::move(source));
    Zone zone("invalid_techset", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    CHECK_FALSE(LoadTechset(zone, source));
}

TEST_CASE("Xbox shader reflection reads big-endian constant tables", "[iw3xenon][techset][shader]")
{
    auto bytes = ReflectedMicrocode(shader::xenon::ShaderStage::VERTEX);
    const auto info = shader::xenon::GetShaderInfo({reinterpret_cast<const uint8_t*>(bytes.data()), 192});
    REQUIRE(info);
    CHECK(info->m_type == d3d9::ShaderType::VERTEX_SHADER);
    CHECK(info->m_version_major == 3);
    CHECK(info->m_version_minor == 0);
    CHECK(info->m_creator == "test");
    CHECK(info->m_target == "vs_3_0");
    REQUIRE(info->m_constants.size() == 2);
    const auto& matrix = info->m_constants[0];
    CHECK(matrix.m_name == "worldMatrix");
    CHECK(matrix.m_register_set == d3d9::RegisterSet::FLOAT_4);
    CHECK(matrix.m_register_index == 4);
    CHECK(matrix.m_register_count == 8);
    CHECK(matrix.m_class == d3d9::ParameterClass::MATRIX_COLUMNS);
    CHECK(matrix.m_type == d3d9::ParameterType::FLOAT);
    CHECK(matrix.m_type_rows == 4);
    CHECK(matrix.m_type_columns == 4);
    CHECK(matrix.m_type_elements == 2);
    CHECK(info->m_constants[1].m_name == "colorMapSampler");
    CHECK(info->m_constants[1].m_register_set == d3d9::RegisterSet::SAMPLER);
    CHECK(info->m_constants[1].m_type == d3d9::ParameterType::SAMPLER_2D);
    SECTION("truncated constant table")
    {
        bytes.resize(191);
    }
    SECTION("out-of-bounds string")
    {
        const auto value = endianness::ToBigEndian(0xffffffffu);
        std::memcpy(bytes.data() + 68, &value, sizeof(value));
    }
    SECTION("enum values must not truncate")
    {
        const auto value = endianness::ToBigEndian<uint16_t>(0x102);
        std::memcpy(bytes.data() + 72, &value, sizeof(value));
    }
    CHECK_FALSE(shader::xenon::GetShaderInfo({reinterpret_cast<const uint8_t*>(bytes.data()), std::min(bytes.size(), 192uz)}));
}

TEST_CASE("IW3 Xenon text uses reflected sampler names and matrix array elements", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    source.AddFileData("techniques/test_technique.tech", R"({
        stateMap "passthrough";
        xenonArgumentCounts 1, 0, 1;
        xenonVertexShaders {}
        vertexShader 3.0 "vertex"
        {
            worldMatrix[1] = constant.transposeWorldMatrix;
        }
        pixelShader 3.0 "pixel"
        {
            colorMapSampler = material.colorMap;
        }
    })");
    source.AddFileData("shader_bin/vs_vertex.xvu", ReflectedMicrocode(shader::xenon::ShaderStage::VERTEX));
    source.AddFileData("shader_bin/ps_pixel.xpu", ReflectedMicrocode(shader::xenon::ShaderStage::PIXEL));
    Zone zone("named_bindings", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    const auto& pass = zone.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
    CHECK(pass.args[0].type == MTL_ARG_CODE_VERTEX_CONST);
    CHECK(pass.args[0].dest == 8);
    CHECK(pass.args[0].u.codeConst.index == CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX);
    CHECK(pass.args[0].u.codeConst.rowCount == 4);
    CHECK(pass.args[1].type == MTL_ARG_MATERIAL_PIXEL_SAMPLER);
    CHECK(pass.args[1].dest == 3);
    CHECK(pass.vertexShaderArray[VERTDECL_GENERIC] == nullptr);
    const auto dumped = DumpTechset(zone);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("worldMatrix[1] = constant.transposeWorldMatrix;") != std::string::npos);
    CHECK(technique->AsString().find("colorMapSampler = material.colorMap;") != std::string::npos);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("named_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    const auto redumped = DumpTechset(reloaded);
    REQUIRE(redumped.GetMockedFile("techniques/test_technique.tech"));
    CHECK(redumped.GetMockedFile("techniques/test_technique.tech")->m_data == technique->m_data);
}

TEST_CASE("IW3 Xenon text preserves null and empty declarations and shader blocks", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    const bool emptyDeclaration = GENERATE(false, true);
    const auto declaration = emptyDeclaration ? "xenonOptionalSource 0;" : "";
    source.AddFileData("techniques/test_technique.tech",
                       std::format(R"({{
        {}
        xenonArgumentCounts 0, 0, 1;
        xenonVertexShaders {{}}
        pixelShader 3.0 ""
        {{
            xenonArg 0, 4, 0, 0;
            register[4] = null;
        }}
    }})",
                                   declaration));
    Zone zone("empty_bindings", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    const auto& pass = zone.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
    CHECK((pass.vertexDecl != nullptr) == emptyDeclaration);
    CHECK(pass.pixelShader == nullptr);
    CHECK(pass.vertexShader == nullptr);
    CHECK(pass.args[0].u.literalConst == nullptr);
    const auto dumped = DumpTechset(zone);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("empty_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    const auto redumped = DumpTechset(reloaded);
    CHECK(redumped.GetMockedFile("techniques/test_technique.tech")->AsString() == dumped.GetMockedFile("techniques/test_technique.tech")->AsString());
}

TEST_CASE("IW3 Xenon vertex shader families retain sparse layouts and a null direct shader", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    source.AddFileData("techniques/test_technique.tech", R"({
        xenonVertexShaders { packed; world_t2n1; }
        vertexShader 3.0 "lit.hlsl"
        {
            tint = material.tint;
        }
        vertex.position = code.position;
    })");
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName("lit.hlsl*packed"),
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"tint",     0},
                                              {"fogColor", 1}
    }));
    // Reflection order is immaterial; names, registers and types must match.
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName("lit.hlsl*world_t2n1"),
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"fogColor", 1},
                                              {"tint",     0}
    }));
    Zone zone("shader_family", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    const auto checkPass = [](const Zone& loaded)
    {
        const auto& pass = loaded.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
        CHECK(pass.vertexShader == nullptr);
        for (auto i = 0u; i < VERTDECL_COUNT; ++i)
            CHECK((pass.vertexShaderArray[i] != nullptr) == (i == VERTDECL_PACKED || i == VERTDECL_WORLD_T2N1));
        CHECK(std::string_view(pass.vertexShaderArray[VERTDECL_PACKED]->name) == "lit.hlsl*packed");
        CHECK(std::string_view(pass.vertexShaderArray[VERTDECL_WORLD_T2N1]->name) == "lit.hlsl*world_t2n1");
        REQUIRE(pass.stableArgCount == 2);
        CHECK(pass.args[0].type == MTL_ARG_MATERIAL_VERTEX_CONST);
        CHECK(pass.args[0].dest == 0);
        CHECK(pass.args[1].u.codeConst.index == CONST_SRC_CODE_FOG_COLOR);
    };
    checkPass(zone);
    const auto dumped = DumpTechset(zone);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("packed;") != std::string::npos);
    CHECK(technique->AsString().find("world_t2n1;") != std::string::npos);
    CHECK(technique->AsString().find("vertexShader 3.0 \"lit.hlsl\"") != std::string::npos);
    CHECK(technique->AsString().find("*packed") == std::string::npos);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("shader_family_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    checkPass(reloaded);
    CHECK(DumpTechset(reloaded).GetMockedFile("techniques/test_technique.tech")->AsString() == technique->AsString());
}

TEST_CASE("IW3 Xenon dumps retain explicit names for irregular vertex shader families", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    const bool differentInterface = GENERATE(false, true);
    const auto worldName = differentInterface ? "lit.hlsl*world" : "other.hlsl*world";
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    source.AddFileData("techniques/test_technique.tech",
                       std::format(R"({{
        xenonArgumentCounts 0, 0, 1;
        xenonVertexShaders {{ packed "lit.hlsl*packed"; world "{}"; }}
        vertexShader 3.0 "" {{ fogColor = constant.fogColor; }}
    }})",
                                   worldName));
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName("lit.hlsl*packed"),
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"fogColor", 1}
    }));
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName(worldName),
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"fogColor", differentInterface ? 2u : 1u}
    }));
    Zone zone("explicit_shader_family", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    const auto dumped = DumpTechset(zone);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("packed \"lit.hlsl*packed\";") != std::string::npos);
    CHECK(technique->AsString().find(std::format("world \"{}\";", worldName)) != std::string::npos);
    CHECK(technique->AsString().find("vertexShader 3.0 \"\"") != std::string::npos);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("explicit_family_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    CHECK(DumpTechset(reloaded).GetMockedFile("techniques/test_technique.tech")->AsString() == technique->AsString());
}

TEST_CASE("IW3 Xenon vertex shader families reject ambiguous or incompatible variants", "[iw3xenon][system][techset]")
{
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    auto document = std::string(R"({ xenonVertexShaders { packed; world; } vertexShader 3.0 "lit.hlsl" {} })");
    auto worldShader = TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"fogColor", 1}
    });
    SECTION("different registers")
    {
        worldShader = TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                         {
                                             {"fogColor", 2}
        });
    }
    SECTION("different parameter types")
    {
        worldShader = TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                         {
                                             {"fogColor", 1, 1, 1, d3d9::ParameterClass::SCALAR}
        });
    }
    SECTION("mixed family and explicit names")
    {
        document = R"({ xenonVertexShaders { packed; world "lit.hlsl*world"; } vertexShader 3.0 "lit.hlsl" {} })";
    }
    SECTION("missing source name")
    {
        document = R"({ xenonVertexShaders { packed; } vertexShader 3.0 "" {} })";
    }
    SECTION("missing vertex block")
    {
        document = R"({ xenonVertexShaders { packed; } })";
    }
    SECTION("suffixed source name")
    {
        document = R"({ xenonVertexShaders { packed; } vertexShader 3.0 "lit.hlsl*packed" {} })";
    }
    SECTION("missing variant")
    {
        document = R"({ xenonVertexShaders { generic; } vertexShader 3.0 "lit.hlsl" {} })";
    }
    source.AddFileData("techniques/test_technique.tech", document);
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName("lit.hlsl*packed"),
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"fogColor", 1}
    }));
    source.AddFileData(shader::xenon::GetFileNameForVertexShaderAssetName("lit.hlsl*world"), worldShader);
    Zone zone("invalid_shader_family", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    CHECK_FALSE(LoadTechset(zone, source));
}

TEST_CASE("IW3 Xenon named bindings infer registers, engine arguments and custom samplers", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", R"(
        "unlit": test_technique;
        "lit instanced": pc_only_technique;
    )");
    source.AddFileData("techniques/test_technique.tech", R"({
        stateMap "passthrough";
        vertexShader 3.0 "vertex" {}
        pixelShader 3.0 "pixel"
        {
            scale = float4(1, 2, 3, 4);
            specularMapSampler = material.specularMap;
            colorMapSampler = material.colorMap;
        }
        vertex.position = code.position;
    })");
    source.AddFileData("shader_bin/vs_vertex.xvu",
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"worldMatrix",          4, 4, 1, d3d9::ParameterClass::MATRIX_COLUMNS},
                                              {"viewProjectionMatrix", 0, 4, 1, d3d9::ParameterClass::MATRIX_COLUMNS}
    }));
    source.AddFileData("shader_bin/ps_pixel.xpu",
                       TechniqueMicrocode(shader::xenon::ShaderStage::PIXEL,
                                          {
                                              {"colorMapSampler", 3, 1, 1, d3d9::ParameterClass::OBJECT},
                                              {"specularMapSampler", 8, 1, 1, d3d9::ParameterClass::OBJECT},
                                              {"reflectionProbeSampler", 2, 1, 1, d3d9::ParameterClass::OBJECT},
                                              {"fogColor", 1},
                                              {"scale", 9}
    }));
    Zone zone("inferred_bindings", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    auto& pass = zone.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
    CHECK(pass.perPrimArgCount == 1);
    CHECK(pass.perObjArgCount == 1);
    REQUIRE(pass.stableArgCount == 4);
    CHECK(pass.customSamplerFlags == 1);
    CHECK(pass.args[0].dest == 4);
    CHECK(pass.args[0].u.codeConst.index == CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX);
    CHECK(pass.args[0].u.codeConst.rowCount == 4);
    CHECK(pass.args[1].dest == 0);
    CHECK(pass.args[1].u.codeConst.index == CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX);
    CHECK(pass.args[2].type == MTL_ARG_MATERIAL_PIXEL_SAMPLER);
    CHECK(pass.args[3].type == MTL_ARG_MATERIAL_PIXEL_SAMPLER);
    CHECK(pass.args[2].u.nameHash < pass.args[3].u.nameHash);
    CHECK(pass.args[4].type == MTL_ARG_CODE_PIXEL_CONST);
    CHECK(pass.args[4].u.codeConst.index == CONST_SRC_CODE_FOG_COLOR);
    CHECK(pass.args[5].type == MTL_ARG_LITERAL_PIXEL_CONST);
    CHECK(pass.args[5].dest == 9);
    const auto dumped = DumpTechset(zone);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("xenonArg") == std::string::npos);
    CHECK(technique->AsString().find("xenonCustomSamplerFlags") == std::string::npos);
    CHECK(technique->AsString().find("worldMatrix = constant.worldMatrix;") != std::string::npos);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("inferred_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    const auto redumped = DumpTechset(reloaded);
    CHECK(redumped.GetMockedFile("techniques/test_technique.tech")->AsString() == technique->AsString());

    // A native pass can retain a different order even when every parameter has reflection.
    std::swap(pass.args[2], pass.args[3]);
    const auto explicitDump = DumpTechset(zone);
    REQUIRE(explicitDump.GetMockedFile("techniques/test_technique.tech"));
    CHECK(explicitDump.GetMockedFile("techniques/test_technique.tech")->AsString().find("xenonArgumentCounts 1, 1, 4;") != std::string::npos);
    MockSearchPath explicitSource;
    for (const auto& file : explicitDump.GetMockedFileList())
        explicitSource.AddFileData(file.m_name, file.AsString());
    Zone explicitReloaded("explicit_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(explicitReloaded, explicitSource));
    const auto& explicitPass = explicitReloaded.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
    CHECK(explicitPass.args[2].u.nameHash == pass.args[2].u.nameHash);
    CHECK(explicitPass.args[3].u.nameHash == pass.args[3].u.nameHash);
    CHECK(explicitPass.customSamplerFlags == pass.customSamplerFlags);
}

TEST_CASE("IW3 Xenon named array elements retain unassigned engine elements and matrix rows", "[iw3xenon][system][techset]")
{
    const RestoreWritingConfiguration restoreConfiguration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
    ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_TECHNIQUE_SET] = true;
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    source.AddFileData("techniques/test_technique.tech", R"({
        vertexShader 3.0 "vertex"
        {
            worldRows = constant.worldMatrix[1];
            codeMeshArg[1] = material.secondMesh;
        }
    })");
    source.AddFileData("shader_bin/vs_vertex.xvu",
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"worldRows", 4, 3, 1, d3d9::ParameterClass::MATRIX_ROWS},
                                              {"codeMeshArg", 8, 2, 2}
    }));
    Zone zone("array_bindings", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(zone, source));
    const auto& pass = zone.m_pools.GetAsset<AssetTechniqueSet>("test_techset")->Asset()->techniques[TECHNIQUE_UNLIT]->passArray[0];
    CHECK(pass.perPrimArgCount == 1);
    CHECK(pass.perObjArgCount == 1);
    CHECK(pass.stableArgCount == 1);
    CHECK(pass.args[0].dest == 4);
    CHECK(pass.args[0].u.codeConst.index == CONST_SRC_CODE_WORLD_MATRIX);
    CHECK(pass.args[0].u.codeConst.firstRow == 1);
    CHECK(pass.args[0].u.codeConst.rowCount == 3);
    CHECK(pass.args[1].dest == 8);
    CHECK(pass.args[1].u.codeConst.index == CONST_SRC_CODE_CODE_MESH_ARG_0);
    CHECK(pass.args[2].dest == 9);
    CHECK(pass.args[2].type == MTL_ARG_MATERIAL_VERTEX_CONST);
    const auto dumped = DumpTechset(zone);
    const auto* technique = dumped.GetMockedFile("techniques/test_technique.tech");
    REQUIRE(technique);
    CHECK(technique->AsString().find("xenonArg") == std::string::npos);
    CHECK(technique->AsString().find("worldRows = constant.worldMatrix[1];") != std::string::npos);
    CHECK(technique->AsString().find("codeMeshArg[1] = material.") != std::string::npos);
    MockSearchPath reloadedSource;
    for (const auto& file : dumped.GetMockedFileList())
        reloadedSource.AddFileData(file.m_name, file.AsString());
    Zone reloaded("array_reloaded", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    REQUIRE(LoadTechset(reloaded, reloadedSource));
    const auto redumped = DumpTechset(reloaded);
    CHECK(redumped.GetMockedFile("techniques/test_technique.tech")->AsString() == technique->AsString());
}

TEST_CASE("IW3 Xenon named bindings reject incomplete or invalid assignments", "[iw3xenon][system][techset]")
{
    MockSearchPath source;
    source.AddFileData("techsets/test_techset.techset", "\"unlit\": test_technique;");
    auto binding = std::string("codeMeshArg[1] = material.secondMesh;");
    SECTION("array overflow")
    {
        binding = "codeMeshArg[2] = material.secondMesh;";
    }
    SECTION("duplicate assignment")
    {
        binding += binding;
    }
    SECTION("unknown parameter")
    {
        binding = "unknown = material.secondMesh;";
    }
    SECTION("unassigned material parameter")
    {
        binding.clear();
        source.AddFileData("shader_bin/vs_vertex.xvu",
                           TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                              {
                                                  {"materialOnly", 0}
        }));
    }
    SECTION("matrix row overflow")
    {
        binding = "codeMeshArg[1] = constant.worldMatrix[1];";
        source.AddFileData("shader_bin/vs_vertex.xvu",
                           TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                              {
                                                  {"codeMeshArg", 4, 8, 2, d3d9::ParameterClass::MATRIX_ROWS}
        }));
    }
    source.AddFileData("techniques/test_technique.tech", std::format("{{ vertexShader 3.0 \"vertex\" {{ {} }} }}", binding));
    source.AddFileData("shader_bin/vs_vertex.xvu",
                       TechniqueMicrocode(shader::xenon::ShaderStage::VERTEX,
                                          {
                                              {"codeMeshArg", 8, 2, 2}
    }));
    Zone zone("invalid_named_bindings", 0, GameId::IW3Xenon, GamePlatform::XBOX);
    CHECK_FALSE(LoadTechset(zone, source));
}

#endif
