#include "TechsetDumperIW3Xenon.h"

#include "Dumping/AbstractTextDumper.h"
#include "Game/IW3Xenon/Material/MaterialConstantZoneStateIW3Xenon.h"
#include "Game/IW3Xenon/Techset/TechsetConstantsIW3Xenon.h"
#include "Pool/GlobalAssetPool.h"
#include "Shader/XenonShader.h"
#include "Techset/ShaderDumpingZoneState.h"
#include "Techset/TechniqueDumpingZoneState.h"
#include "Utils/Logging/Log.h"
#include "Utils/StringUtils.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <stdexcept>
#include <tuple>

using namespace IW3Xenon;

namespace
{
    const char* NonReferenceAssetName(const char* name)
    {
        return name && name[0] == ',' ? name + 1 : name;
    }

    std::string TechniqueName(const std::string_view name)
    {
        const bool identifier = !name.empty() && (std::isalpha(static_cast<unsigned char>(name.front())) || name.front() == '_')
                                && std::ranges::all_of(name,
                                                       [](const unsigned char character)
                                                       {
                                                           return std::isalnum(character) || character == '_';
                                                       });
        return identifier ? std::string(name) : std::format("\"{}\"", name);
    }

    const MaterialPixelShader* ResolvePixelShader(const MaterialPixelShader* shader)
    {
        if (shader && shader->name[0] == ',')
        {
            auto* globalAsset = GameGlobalAssetPools::GetGlobalPoolsForGame(GameId::IW3Xenon)->GetAsset<AssetPixelShader>(shader->name + 1);
            return globalAsset ? globalAsset->Asset() : nullptr;
        }
        return shader;
    }

    template<typename LoadDef>
    void DumpShader(const AssetDumpingContext& context,
                    const std::string& fileName,
                    const LoadDef& loadDef,
                    const shader::xenon::ShaderStage stage,
                    const size_t runtimeHeaderSize)
    {
        if (!loadDef.cachedPart || loadDef.cachedPartSize < runtimeHeaderSize || (loadDef.physicalPartSize && !loadDef.physicalPart))
            throw std::runtime_error(std::format("invalid native shader \"{}\"", fileName));
        const auto cachedSize = loadDef.cachedPartSize - runtimeHeaderSize;
        std::vector<uint8_t> microcode(loadDef.cachedPart + runtimeHeaderSize, loadDef.cachedPart + loadDef.cachedPartSize);
        if (loadDef.physicalPartSize)
            microcode.insert(microcode.end(), loadDef.physicalPart, loadDef.physicalPart + loadDef.physicalPartSize);
        const auto parts = shader::xenon::GetShaderParts(microcode, stage);
        if (!parts || parts->cached.size() != cachedSize || parts->physical.size() != loadDef.physicalPartSize)
            throw std::runtime_error(std::format("invalid SDK microcode object \"{}\"", fileName));

        const auto file = context.OpenAssetFile(fileName);
        if (file)
            file->write(reinterpret_cast<const char*>(microcode.data()), static_cast<std::streamsize>(microcode.size()));
    }

    void DumpPixelShader(AssetDumpingContext& context, const MaterialPixelShader* shader)
    {
        shader = ResolvePixelShader(shader);
        if (!shader || !context.GetZoneAssetDumperState<techset::ShaderDumpingZoneState>()->ShouldDumpPixelShader(shader))
            return;
        if (shader->prog.loadDef.cachedPartSize == 0 && shader->prog.loadDef.physicalPartSize == 0 && std::string_view(shader->name) == "null.hlsl")
            return;
        DumpShader(context,
                   shader::xenon::GetFileNameForPixelShaderAssetName(shader->name),
                   shader->prog.loadDef,
                   shader::xenon::ShaderStage::PIXEL,
                   PIXEL_SHADER_RUNTIME_HEADER_SIZE);
    }

    void DumpVertexShader(AssetDumpingContext& context, const MaterialVertexShader* shader)
    {
        if (!shader || !context.GetZoneAssetDumperState<techset::ShaderDumpingZoneState>()->ShouldDumpVertexShader(shader))
            return;
        DumpShader(context,
                   shader::xenon::GetFileNameForVertexShaderAssetName(shader->name),
                   shader->prog.loadDef,
                   shader::xenon::ShaderStage::VERTEX,
                   VERTEX_SHADER_RUNTIME_HEADER_SIZE);
    }

    template<typename LoadDef> std::unique_ptr<d3d9::ShaderInfo> ShaderInfo(const LoadDef& data, const size_t runtimeHeaderSize)
    {
        if (!data.cachedPart || data.cachedPartSize < runtimeHeaderSize)
            return nullptr;
        return shader::xenon::GetShaderInfo({data.cachedPart + runtimeHeaderSize, data.cachedPartSize - runtimeHeaderSize});
    }

    std::optional<std::string_view> VertexShaderFamily(const MaterialPass& pass)
    {
        if (pass.vertexShader)
            return std::nullopt;
        std::optional<std::string_view> source;
        std::unique_ptr<d3d9::ShaderInfo> firstInfo;
        for (auto i = 0u; i < VERTDECL_COUNT; ++i)
        {
            const auto* shader = pass.vertexShaderArray[i];
            if (!shader)
                continue;
            if (!shader->name)
                return std::nullopt;
            const auto name = shader::xenon::GetVertexShaderSourceName(shader->name);
            if (name.empty() || (source && *source != name) || shader->name != shader::xenon::GetVertexShaderVariantName(name, vertexDeclNames[i]))
                return std::nullopt;
            auto info = ShaderInfo(shader->prog.loadDef, VERTEX_SHADER_RUNTIME_HEADER_SIZE);
            if (!info || (firstInfo && !shader::xenon::HaveMatchingConstantTables(*firstInfo, *info)))
                return std::nullopt;
            source = name;
            if (!firstInfo)
                firstInfo = std::move(info);
        }
        return source;
    }

    bool IsVertexArgument(const MaterialShaderArgument& arg)
    {
        return arg.type == MTL_ARG_MATERIAL_VERTEX_CONST || arg.type == MTL_ARG_LITERAL_VERTEX_CONST || arg.type == MTL_ARG_CODE_VERTEX_CONST;
    }

    struct PassAnalysis
    {
        std::unique_ptr<d3d9::ShaderInfo> vertexShader;
        std::unique_ptr<d3d9::ShaderInfo> pixelShader;
        bool deriveLayout = true;
        uint16_t techniqueFlags{};
        uint8_t customSamplerFlags{};
    };

    const d3d9::ShaderConstant* FindParameter(const MaterialShaderArgument& arg, const d3d9::ShaderInfo* info)
    {
        const bool sampler = arg.type == MTL_ARG_CODE_PIXEL_SAMPLER || arg.type == MTL_ARG_MATERIAL_PIXEL_SAMPLER;
        if (info)
            for (const auto& parameter : info->m_constants)
                if (parameter.m_register_set == (sampler ? d3d9::RegisterSet::SAMPLER : d3d9::RegisterSet::FLOAT_4))
                {
                    const auto elements = std::max(parameter.m_type_elements, 1u);
                    const auto registers = parameter.m_register_count / elements;
                    if (registers && parameter.m_register_count % elements == 0 && arg.dest >= parameter.m_register_index
                        && arg.dest < parameter.m_register_index + parameter.m_register_count && (arg.dest - parameter.m_register_index) % registers == 0)
                        return &parameter;
                }
        return nullptr;
    }

    PassAnalysis AnalysePass(const MaterialPass& pass)
    {
        PassAnalysis result;
        auto* vertex = pass.vertexShader;
        if (!vertex)
            for (auto* variant : pass.vertexShaderArray)
                if (variant)
                {
                    vertex = variant;
                    break;
                }
        if (vertex)
            result.vertexShader = ShaderInfo(vertex->prog.loadDef, VERTEX_SHADER_RUNTIME_HEADER_SIZE);
        const auto* pixel = ResolvePixelShader(pass.pixelShader);
        if (pixel)
            result.pixelShader = ShaderInfo(pixel->prog.loadDef, PIXEL_SHADER_RUNTIME_HEADER_SIZE);
        if ((vertex && !result.vertexShader) || (pixel && pixel->prog.loadDef.cachedPartSize && !result.pixelShader))
            result.deriveLayout = false;

        std::vector<techset::CommonCodeSourceUpdateFrequency> frequencies;
        std::set<std::tuple<bool, bool, unsigned>> assignedBindings;
        const auto count = pass.perPrimArgCount + pass.perObjArgCount + pass.stableArgCount;
        if (count && !pass.args)
            throw std::runtime_error("missing shader arguments");
        for (auto i = 0; i < count; ++i)
        {
            const auto& arg = pass.args[i];
            const bool sampler = arg.type == MTL_ARG_CODE_PIXEL_SAMPLER || arg.type == MTL_ARG_MATERIAL_PIXEL_SAMPLER;
            const auto* parameter = FindParameter(arg, IsVertexArgument(arg) ? result.vertexShader.get() : result.pixelShader.get());
            auto frequency = techset::CommonCodeSourceUpdateFrequency::RARELY;
            bool valid = parameter != nullptr;
            valid &= assignedBindings.emplace(IsVertexArgument(arg), sampler, arg.dest).second;
            switch (arg.type)
            {
            case MTL_ARG_CODE_VERTEX_CONST:
            case MTL_ARG_CODE_PIXEL_CONST:
            {
                const auto source = arg.u.codeConst.index <= UINT8_MAX
                                        ? commonCodeSourceInfos.GetInfoForCodeConstSource(static_cast<uint8_t>(arg.u.codeConst.index))
                                        : std::nullopt;
                if (!source)
                {
                    valid = false;
                    break;
                }
                frequency = source->updateFrequency;
                if (source->techFlags
                    && (!source->techFlagShaderType
                        || *source->techFlagShaderType
                               == (IsVertexArgument(arg) ? techset::CommonTechniqueShaderType::VERTEX : techset::CommonTechniqueShaderType::PIXEL)))
                    result.techniqueFlags |= static_cast<uint16_t>(*source->techFlags);
                if (source->transposedMatrix)
                {
                    const auto rows = parameter ? parameter->m_register_count / std::max(parameter->m_type_elements, 1u) : 0;
                    valid &= rows == arg.u.codeConst.rowCount && rows && arg.u.codeConst.firstRow + rows <= 4;
                }
                else
                    valid &= arg.u.codeConst.firstRow == 0 && arg.u.codeConst.rowCount == 1;
                break;
            }
            case MTL_ARG_CODE_PIXEL_SAMPLER:
            {
                const auto source =
                    arg.u.codeSampler <= UINT8_MAX ? commonCodeSourceInfos.GetInfoForCodeSamplerSource(static_cast<uint8_t>(arg.u.codeSampler)) : std::nullopt;
                if (!source)
                {
                    valid = false;
                    break;
                }
                frequency = source->updateFrequency;
                if (source->techFlags)
                    result.techniqueFlags |= static_cast<uint16_t>(*source->techFlags);
                if (source->customSamplerIndex)
                    result.customSamplerFlags |= static_cast<uint8_t>(1u << *source->customSamplerIndex);
                break;
            }
            case MTL_ARG_LITERAL_VERTEX_CONST:
            case MTL_ARG_LITERAL_PIXEL_CONST:
                valid &= arg.u.literalConst != nullptr;
                break;
            case MTL_ARG_MATERIAL_VERTEX_CONST:
            case MTL_ARG_MATERIAL_PIXEL_CONST:
            case MTL_ARG_MATERIAL_PIXEL_SAMPLER:
                break;
            default:
                valid = false;
                break;
            }
            const auto expectedFrequency = i < pass.perPrimArgCount                         ? techset::CommonCodeSourceUpdateFrequency::PER_PRIM
                                           : i < pass.perPrimArgCount + pass.perObjArgCount ? techset::CommonCodeSourceUpdateFrequency::PER_OBJECT
                                                                                            : techset::CommonCodeSourceUpdateFrequency::RARELY;
            result.deriveLayout &= valid && frequency == expectedFrequency;
            frequencies.push_back(frequency);
            if (i && frequencies[i - 1] == frequency)
            {
                const auto& previous = pass.args[i - 1];
                if (previous.type != arg.type)
                    result.deriveLayout &= previous.type < arg.type;
                else if (arg.type == MTL_ARG_MATERIAL_VERTEX_CONST || arg.type == MTL_ARG_MATERIAL_PIXEL_CONST || arg.type == MTL_ARG_MATERIAL_PIXEL_SAMPLER)
                    result.deriveLayout &= previous.u.nameHash <= arg.u.nameHash;
                else
                    result.deriveLayout &= previous.dest <= arg.dest;
            }
        }

        // Unassigned engine samplers can be represented by flags rather than entries in the argument array.
        for (const bool isVertex : {true, false})
        {
            const auto* info = isVertex ? result.vertexShader.get() : result.pixelShader.get();
            if (!info)
                continue;
            for (const auto& parameter : info->m_constants)
                for (auto element = 0u; element < std::max(parameter.m_type_elements, 1u); ++element)
                {
                    const auto dest = parameter.m_register_index + element * parameter.m_register_count / std::max(parameter.m_type_elements, 1u);
                    const bool sampler = parameter.m_register_set == d3d9::RegisterSet::SAMPLER;
                    if (assignedBindings.contains({isVertex, sampler, dest}))
                        continue;
                    const auto sourceIndex = sampler ? commonCodeSourceInfos.GetCodeSamplerSourceForAccessor(parameter.m_name) : std::nullopt;
                    const auto source = sourceIndex ? commonCodeSourceInfos.GetInfoForCodeSamplerSource(*sourceIndex) : std::nullopt;
                    if (isVertex || !source || source->updateFrequency != techset::CommonCodeSourceUpdateFrequency::CUSTOM || element
                        || parameter.m_register_count != 1)
                        result.deriveLayout = false;
                    else
                    {
                        if (source->customSamplerIndex)
                            result.customSamplerFlags |= static_cast<uint8_t>(1u << *source->customSamplerIndex);
                        if (source->techFlags)
                            result.techniqueFlags |= static_cast<uint16_t>(*source->techFlags);
                    }
                }
        }
        if (!result.deriveLayout)
        {
            result.techniqueFlags = 0;
            result.customSamplerFlags = 0;
        }
        return result;
    }

    class TechniqueFileWriter final : public AbstractTextDumper
    {
    public:
        TechniqueFileWriter(std::ostream& stream, const AbstractMaterialConstantZoneState& constantZoneState)
            : AbstractTextDumper(stream),
              m_constant_zone_state(constantZoneState)
        {
        }

        void DumpTechnique(const MaterialTechnique& technique)
        {
            std::vector<PassAnalysis> passes;
            uint16_t derivedFlags{};
            auto name = std::string(technique.name);
            utils::MakeStringLowerCase(name);
            if (name == "zprepass")
                derivedFlags |= IW3::MTL_TECHFLAG_ZPREPASS;
            for (auto i = 0u; i < technique.passCount; ++i)
            {
                const auto& pass = technique.passArray[i];
                passes.push_back(AnalysePass(pass));
                derivedFlags |= passes.back().techniqueFlags;
                if (pass.vertexDecl && pass.vertexDecl->hasOptionalSource)
                    derivedFlags |= IW3::MTL_TECHFLAG_DECL_HAS_OPTIONAL_SOURCE;
            }
            if (technique.flags != derivedFlags)
                m_stream << std::format("xenonTechniqueFlags {};\n\n", technique.flags);
            for (auto i = 0u; i < technique.passCount; ++i)
                DumpPass(technique.passArray[i], passes[i]);
        }

    private:
        void Line(const std::string& text) const
        {
            Indent();
            m_stream << text << '\n';
        }

        void DumpArgument(const MaterialShaderArgument& arg, const unsigned order, const d3d9::ShaderInfo* info, const bool deriveLayout) const
        {
            const bool sampler = arg.type == MTL_ARG_CODE_PIXEL_SAMPLER || arg.type == MTL_ARG_MATERIAL_PIXEL_SAMPLER;
            const auto* constant = FindParameter(arg, info);
            std::string destination = std::format("{}[{}]", sampler ? "samplerRegister" : "register", arg.dest);
            if (constant)
            {
                destination = constant->m_name;
                if (constant->m_type_elements > 1)
                {
                    const auto registers = std::max(constant->m_register_count / constant->m_type_elements, 1u);
                    destination += std::format("[{}]", (arg.dest - constant->m_register_index) / registers);
                }
            }
            const bool codeConst = arg.type == MTL_ARG_CODE_VERTEX_CONST || arg.type == MTL_ARG_CODE_PIXEL_CONST;
            if (!deriveLayout)
                Line(std::format(
                    "xenonArg {}, {}, {}, {};", order, arg.dest, codeConst ? arg.u.codeConst.firstRow : 0, codeConst ? arg.u.codeConst.rowCount : 0));
            std::string value;
            switch (arg.type)
            {
            case MTL_ARG_CODE_VERTEX_CONST:
            case MTL_ARG_CODE_PIXEL_CONST:
            {
                const auto index = arg.u.codeConst.index;
                auto source = index <= UINT8_MAX ? commonCodeSourceInfos.GetInfoForCodeConstSource(static_cast<uint8_t>(index)) : std::nullopt;
                if (deriveLayout && constant && constant->m_class == d3d9::ParameterClass::MATRIX_COLUMNS && source && source->transposedMatrix)
                    source = commonCodeSourceInfos.GetInfoForCodeConstSource(*source->transposedMatrix);
                if (source)
                {
                    value = std::format("constant.{}", source->accessor);
                    if (deriveLayout && source->transposedMatrix && arg.u.codeConst.firstRow)
                        value += std::format("[{}]", arg.u.codeConst.firstRow);
                    else if (source->arrayCount > 1)
                        value += std::format("[{}]", index - source->value);
                }
                else
                    value = std::format("constant.#0x{:x}", index);
                break;
            }
            case MTL_ARG_CODE_PIXEL_SAMPLER:
            {
                const auto index = arg.u.codeSampler;
                const auto source = index <= UINT8_MAX ? commonCodeSourceInfos.GetInfoForCodeSamplerSource(static_cast<uint8_t>(index)) : std::nullopt;
                value = source ? std::format("sampler.{}", source->accessor) : std::format("sampler.#0x{:x}", index);
                break;
            }
            case MTL_ARG_LITERAL_VERTEX_CONST:
            case MTL_ARG_LITERAL_PIXEL_CONST:
                if (arg.u.literalConst)
                {
                    const auto& literal = *arg.u.literalConst;
                    value = std::format("float4({}, {}, {}, {})", literal[0], literal[1], literal[2], literal[3]);
                }
                else
                    value = "null";
                break;
            case MTL_ARG_MATERIAL_VERTEX_CONST:
            case MTL_ARG_MATERIAL_PIXEL_CONST:
            case MTL_ARG_MATERIAL_PIXEL_SAMPLER:
            {
                std::string name;
                if (constant && m_constant_zone_state.HashString(constant->m_name) == arg.u.nameHash)
                    name = constant->m_name;
                else if (!m_constant_zone_state.GetConstantName(arg.u.nameHash, name) && !m_constant_zone_state.GetTextureDefName(arg.u.nameHash, name))
                    name = std::format("#0x{:x}", arg.u.nameHash);
                value = "material." + name;
                break;
            }
            default:
                throw std::runtime_error(std::format("unknown shader argument type {}", arg.type));
            }
            Line(std::format("{} = {};", destination, value));
        }

        void DumpShaderBlock(const MaterialPass& pass,
                             const bool vertex,
                             const PassAnalysis& analysis,
                             const std::optional<std::string_view> vertexShaderFamily = std::nullopt)
        {
            const auto* info = vertex ? analysis.vertexShader.get() : analysis.pixelShader.get();
            std::string_view name;
            if (vertex)
            {
                if (vertexShaderFamily)
                    name = *vertexShaderFamily;
                else if (pass.vertexShader)
                    name = pass.vertexShader->name;
            }
            else if (pass.pixelShader)
            {
                name = NonReferenceAssetName(pass.pixelShader->name);
            }
            m_stream << '\n';
            Line(std::format(
                "{} {}.{} \"{}\"", vertex ? "vertexShader" : "pixelShader", info ? info->m_version_major : 3, info ? info->m_version_minor : 0, name));
            Line("{");
            IncIndent();
            const auto count = pass.perPrimArgCount + pass.perObjArgCount + pass.stableArgCount;
            for (auto i = 0; i < count; ++i)
                if (IsVertexArgument(pass.args[i]) == vertex)
                    DumpArgument(pass.args[i], i, info, analysis.deriveLayout);
            DecIndent();
            Line("}");
        }

        void DumpPass(const MaterialPass& pass, const PassAnalysis& analysis)
        {
            m_stream << "{\n";
            IncIndent();
            // Like PC, compiled fastfiles retain state bits in materials, not the original state-map rules.
            Line("stateMap \"passthrough\"; // TODO: original state-map rules");
            if (!analysis.deriveLayout)
                Line(std::format("xenonArgumentCounts {}, {}, {};", pass.perPrimArgCount, pass.perObjArgCount, pass.stableArgCount));
            if (pass.customSamplerFlags != analysis.customSamplerFlags)
                Line(std::format("xenonCustomSamplerFlags {};", pass.customSamplerFlags));
            if (pass.precompiledIndex)
                Line(std::format("xenonPrecompiledIndex {};", pass.precompiledIndex));
            if (pass.vertexDecl)
                Line(std::format("xenonOptionalSource {};", pass.vertexDecl->hasOptionalSource ? 1 : 0));
            Line("xenonVertexShaders");
            Line("{");
            IncIndent();
            const auto family = VertexShaderFamily(pass);
            for (auto i = 0u; i < VERTDECL_COUNT; ++i)
                if (pass.vertexShaderArray[i])
                    Line(family ? std::format("{};", vertexDeclNames[i]) : std::format("{} \"{}\";", vertexDeclNames[i], pass.vertexShaderArray[i]->name));
            DecIndent();
            Line("}");
            DumpShaderBlock(pass, true, analysis, family);
            DumpShaderBlock(pass, false, analysis);
            if (pass.vertexDecl)
            {
                if (pass.vertexDecl->streamCount > std::size(pass.vertexDecl->routing.data))
                    throw std::runtime_error("too many vertex stream routes");
                m_stream << '\n';
                for (auto i = 0u; i < pass.vertexDecl->streamCount; ++i)
                {
                    const auto& route = pass.vertexDecl->routing.data[i];
                    if (route.source >= STREAM_SRC_COUNT || route.dest >= STREAM_DST_COUNT)
                        throw std::runtime_error("invalid vertex stream routing");
                    Line(
                        std::format("vertex.{} = code.{};", commonRoutingInfos.GetDestinationName(route.dest), commonRoutingInfos.GetSourceName(route.source)));
                }
            }
            DecIndent();
            m_stream << "}\n";
        }

        const AbstractMaterialConstantZoneState& m_constant_zone_state;
    };

    void DumpTechnique(AssetDumpingContext& context, const MaterialTechnique& technique)
    {
        if (!context.GetZoneAssetDumperState<techset::TechniqueDumpingZoneState>()->ShouldDumpTechnique(&technique))
            return;
        for (auto i = 0u; i < technique.passCount; ++i)
        {
            const auto& pass = technique.passArray[i];
            for (const auto* shader : pass.vertexShaderArray)
                DumpVertexShader(context, shader);
            DumpVertexShader(context, pass.vertexShader);
            DumpPixelShader(context, pass.pixelShader);
        }
        const auto file = context.OpenAssetFile(std::format("techniques/{}.tech", shader::xenon::EncodeFileName(technique.name)));
        if (file)
        {
            auto* constants = context.GetZoneAssetDumperState<MaterialConstantZoneState>();
            constants->EnsureInitialized();
            TechniqueFileWriter writer(*file, *constants);
            writer.DumpTechnique(technique);
        }
    }
} // namespace

namespace techset
{
    void DumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<MaterialTechniqueSet>& asset)
    {
        try
        {
            const auto& techset = *asset.Asset();
            const auto file = context.OpenAssetFile(std::format("techsets/{}.techset", shader::xenon::EncodeFileName(asset.m_name)));
            if (!file)
                return;
            if (techset.worldVertFormat)
                *file << std::format("xenonWorldVertexFormat {};\n\n", techset.worldVertFormat);
            if (techset.unused[0] || techset.unused[1])
                *file << std::format("xenonUnused {}, {};\n\n", techset.unused[0], techset.unused[1]);
            std::array<bool, TECHNIQUE_COUNT> dumped{};
            bool first = true;
            for (auto i = 0u; i < TECHNIQUE_COUNT; ++i)
            {
                const auto* technique = techset.techniques[i];
                if (!technique || dumped[i])
                    continue;
                if (!first)
                    *file << '\n';
                first = false;
                for (auto j = i; j < TECHNIQUE_COUNT; ++j)
                    if (techset.techniques[j] && std::string_view(techset.techniques[j]->name) == technique->name)
                    {
                        *file << std::format("\"{}\":\n", techniqueTypeNames[j]);
                        dumped[j] = true;
                    }
                *file << std::format("  {};\n", TechniqueName(technique->name));
                DumpTechnique(context, *technique);
            }
        }
        catch (const std::exception& error)
        {
            con::error("Cannot dump IW3 Xenon techniqueset \"{}\": {}", asset.m_name, error.what());
        }
    }

    void PixelShaderDumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<MaterialPixelShader>& asset)
    {
        try
        {
            DumpPixelShader(context, asset.Asset());
        }
        catch (const std::exception& error)
        {
            con::error("Cannot dump IW3 Xenon pixel shader \"{}\": {}", asset.m_name, error.what());
        }
    }
} // namespace techset
