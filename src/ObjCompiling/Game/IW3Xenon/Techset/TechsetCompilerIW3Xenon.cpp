#include "TechsetCompilerIW3Xenon.h"

#include "Game/IW3/CommonIW3.h"
#include "Game/IW3Xenon/Techset/TechsetConstantsIW3Xenon.h"
#include "Parsing/Impl/CommentRemovingStreamProxy.h"
#include "Parsing/Impl/ParserSingleInputStream.h"
#include "Parsing/ParsingException.h"
#include "Parsing/Simple/SimpleLexer.h"
#include "Shader/XenonShader.h"
#include "Techset/CommonShaderArgCreator.h"
#include "Utils/Logging/Log.h"
#include "Utils/StringUtils.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <limits>
#include <map>

using namespace IW3Xenon;

namespace
{
    class TechniqueShaderLoader final : public techset::ITechniqueShaderLoader
    {
    public:
        explicit TechniqueShaderLoader(const MaterialPass& pass)
            : m_pass(pass)
        {
        }

        std::optional<techset::CommonTechniqueShaderBin> LoadVertexShader(const std::string& name) override
        {
            auto* shader = m_pass.vertexShader;
            if (!shader)
                for (auto* variant : m_pass.vertexShaderArray)
                    if (variant)
                    {
                        shader = variant;
                        break;
                    }
            return shader ? ShaderBin(shader->prog.loadDef, VERTEX_SHADER_RUNTIME_HEADER_SIZE) : techset::CommonTechniqueShaderBin{};
        }

        std::optional<techset::CommonTechniqueShaderBin> LoadPixelShader(const std::string& name) override
        {
            return m_pass.pixelShader ? ShaderBin(m_pass.pixelShader->prog.loadDef, PIXEL_SHADER_RUNTIME_HEADER_SIZE) : techset::CommonTechniqueShaderBin{};
        }

        std::unique_ptr<d3d9::ShaderInfo> GetShaderInfoDx9(const techset::CommonTechniqueShaderBin& shader) const override
        {
            if (!shader.m_shader_bin_size)
                return std::make_unique<d3d9::ShaderInfo>();
            return shader::xenon::GetShaderInfo({static_cast<const uint8_t*>(shader.m_shader_bin), shader.m_shader_bin_size});
        }

    private:
        template<typename LoadDef> static std::optional<techset::CommonTechniqueShaderBin> ShaderBin(const LoadDef& data, const size_t runtimeHeaderSize)
        {
            if (!data.cachedPartSize && !data.physicalPartSize)
                return techset::CommonTechniqueShaderBin{};
            if (!data.cachedPart || data.cachedPartSize < runtimeHeaderSize)
                return std::nullopt;
            return techset::CommonTechniqueShaderBin{data.cachedPart + runtimeHeaderSize, data.cachedPartSize - runtimeHeaderSize};
        }

        const MaterialPass& m_pass;
    };

    class TextReader
    {
    public:
        TextReader(std::istream& stream, const std::string& fileName)
            : m_input(stream, fileName),
              m_comments(&m_input),
              m_lexer(&m_comments)
        {
        }

        bool End()
        {
            return m_lexer.GetToken(0).IsEof();
        }

        [[noreturn]] void Error(const std::string& message)
        {
            throw ParsingException(m_lexer.GetToken(0).GetPos(), message);
        }

        bool Accept(const char character)
        {
            const auto& token = m_lexer.GetToken(0);
            if (token.m_type != SimpleParserValueType::CHARACTER || token.CharacterValue() != character)
                return false;
            m_lexer.PopTokens(1);
            return true;
        }

        void Expect(const char character)
        {
            if (!Accept(character))
                Error(std::format("expected '{}'", character));
        }

        std::string Name()
        {
            const auto& token = m_lexer.GetToken(0);
            std::string result;
            if (token.m_type == SimpleParserValueType::STRING)
                result = token.StringValue();
            else if (token.m_type == SimpleParserValueType::IDENTIFIER)
                result = token.IdentifierValue();
            else
                Error("expected a name");
            m_lexer.PopTokens(1);
            return result;
        }

        unsigned Integer(const unsigned maximum = std::numeric_limits<unsigned>::max())
        {
            const auto& token = m_lexer.GetToken(0);
            if (token.m_type != SimpleParserValueType::INTEGER || (token.m_has_sign_prefix && token.IntegerValue() < 0))
                Error("expected an unsigned integer");
            const auto result = static_cast<unsigned>(token.IntegerValue());
            if (result > maximum)
                Error(std::format("value exceeds {}", maximum));
            m_lexer.PopTokens(1);
            return result;
        }

        double Number()
        {
            const auto& token = m_lexer.GetToken(0);
            double result;
            if (token.m_type == SimpleParserValueType::FLOATING_POINT)
                result = token.FloatingPointValue();
            else if (token.m_type == SimpleParserValueType::INTEGER)
                result = token.IntegerValue();
            else
                Error("expected a number");
            m_lexer.PopTokens(1);
            return result;
        }

        std::string Accessor()
        {
            auto result = Name();
            while (Accept('.'))
                result += '.' + Name();
            return result;
        }

        std::optional<unsigned> Index()
        {
            if (!Accept('['))
                return std::nullopt;
            const auto index = Integer();
            Expect(']');
            return index;
        }

        std::string IndexedAccessor()
        {
            auto result = Accessor();
            if (const auto index = Index())
                result += std::format("[{}]", *index);
            return result;
        }

    private:
        ParserSingleInputStream m_input;
        CommentRemovingStreamProxy m_comments;
        SimpleLexer m_lexer;
    };

    class TechniqueReader final : public TextReader
    {
    public:
        TechniqueReader(
            std::istream& stream, const std::string& fileName, const std::string& techniqueName, MemoryManager& memory, AssetCreationContext& context)
            : TextReader(stream, fileName),
              m_technique_name(techniqueName),
              m_memory(memory),
              m_context(context)
        {
        }

        std::vector<MaterialPass> Read()
        {
            std::vector<MaterialPass> passes;
            while (!End())
            {
                if (Accept('{'))
                    passes.push_back(ReadPass());
                else
                {
                    if (Name() != "xenonTechniqueFlags")
                        Error("expected a technique pass or xenonTechniqueFlags");
                    m_explicit_flags = static_cast<uint16_t>(Integer(UINT16_MAX));
                    Expect(';');
                }
            }
            if (passes.size() > UINT16_MAX)
                Error("too many technique passes");
            if (m_explicit_flags)
                flags = *m_explicit_flags;
            else
            {
                auto name = m_technique_name;
                utils::MakeStringLowerCase(name);
                if (name == "zprepass")
                    flags |= IW3::MTL_TECHFLAG_ZPREPASS;
                for (const auto& pass : passes)
                    if (pass.vertexDecl && pass.vertexDecl->hasOptionalSource)
                        flags |= IW3::MTL_TECHFLAG_DECL_HAS_OPTIONAL_SOURCE;
            }
            return passes;
        }

        uint16_t flags{};
        std::vector<XAssetInfoGeneric*> dependencies;

    private:
        MaterialVertexShader* VertexShader(const std::string& name)
        {
            if (name.empty())
                return nullptr;
            auto* shader = m_context.LoadSubAsset<SubAssetVertexShader>(name);
            if (!shader)
                Error(std::format("missing vertex shader \"{}\"", name));
            dependencies.push_back(shader);
            return shader->Asset();
        }

        MaterialPixelShader* PixelShader(const std::string& name)
        {
            if (name.empty())
                return nullptr;
            auto* shader = m_context.LoadDependency<AssetPixelShader>(name);
            if (!shader)
                Error(std::format("missing pixel shader \"{}\"", name));
            dependencies.push_back(shader);
            return shader->Asset();
        }

        void ReadVertexShaderFamily(MaterialPass& pass, const std::string& sourceName, const std::array<bool, VERTDECL_COUNT>& declarations)
        {
            if (sourceName.empty() || shader::xenon::GetVertexShaderSourceName(sourceName) != sourceName)
                Error("vertex shader families require a source name without a declaration suffix");
            std::unique_ptr<d3d9::ShaderInfo> firstInfo;
            for (auto i = 0u; i < VERTDECL_COUNT; ++i)
            {
                if (!declarations[i])
                    continue;
                auto* shader = VertexShader(shader::xenon::GetVertexShaderVariantName(sourceName, vertexDeclNames[i]));
                pass.vertexShaderArray[i] = shader;
                const auto& data = shader->prog.loadDef;
                auto info = data.cachedPart && data.cachedPartSize >= VERTEX_SHADER_RUNTIME_HEADER_SIZE
                                ? shader::xenon::GetShaderInfo(
                                      {data.cachedPart + VERTEX_SHADER_RUNTIME_HEADER_SIZE, data.cachedPartSize - VERTEX_SHADER_RUNTIME_HEADER_SIZE})
                                : nullptr;
                if (!info || (firstInfo && !shader::xenon::HaveMatchingConstantTables(*firstInfo, *info)))
                    Error("vertex shader family constant tables must match; use explicit shader names for different interfaces");
                if (!firstInfo)
                    firstInfo = std::move(info);
            }
        }

        std::unique_ptr<d3d9::ShaderInfo> ShaderInfo(const MaterialPass& pass, const bool vertex)
        {
            if (vertex)
            {
                auto* shader = pass.vertexShader;
                if (!shader)
                    for (auto* variant : pass.vertexShaderArray)
                        if (variant)
                        {
                            shader = variant;
                            break;
                        }
                if (!shader || !shader->prog.loadDef.cachedPart || shader->prog.loadDef.cachedPartSize < VERTEX_SHADER_RUNTIME_HEADER_SIZE)
                    return nullptr;
                const auto& data = shader->prog.loadDef;
                return shader::xenon::GetShaderInfo(
                    {data.cachedPart + VERTEX_SHADER_RUNTIME_HEADER_SIZE, data.cachedPartSize - VERTEX_SHADER_RUNTIME_HEADER_SIZE});
            }
            if (!pass.pixelShader || !pass.pixelShader->prog.loadDef.cachedPart
                || pass.pixelShader->prog.loadDef.cachedPartSize < PIXEL_SHADER_RUNTIME_HEADER_SIZE)
                return nullptr;
            const auto& data = pass.pixelShader->prog.loadDef;
            return shader::xenon::GetShaderInfo({data.cachedPart + PIXEL_SHADER_RUNTIME_HEADER_SIZE, data.cachedPartSize - PIXEL_SHADER_RUNTIME_HEADER_SIZE});
        }

        void ReadCompiledArguments(const MaterialPass& pass, const bool vertex, std::map<unsigned, MaterialShaderArgument>& args)
        {
            const auto info = ShaderInfo(pass, vertex);
            while (!Accept('}'))
            {
                MaterialShaderArgument arg{};
                auto name = Name();
                const bool explicitLayout = name == "xenonArg";
                unsigned order = static_cast<unsigned>(args.size());
                uint8_t firstRow = 0, rowCount = 1;
                if (explicitLayout)
                {
                    order = Integer(3u * UINT8_MAX - 1u);
                    Expect(',');
                    arg.dest = static_cast<uint16_t>(Integer(UINT16_MAX));
                    Expect(',');
                    firstRow = static_cast<uint8_t>(Integer(UINT8_MAX));
                    Expect(',');
                    rowCount = static_cast<uint8_t>(Integer(UINT8_MAX));
                    Expect(';');
                    name = Name();
                }
                const auto element = Index();
                const bool registerBinding = name == "register" || name == "samplerRegister";
                if (registerBinding)
                {
                    if (!element || *element > UINT16_MAX || (explicitLayout && *element != arg.dest))
                        Error("invalid destination register");
                    arg.dest = static_cast<uint16_t>(*element);
                }
                const d3d9::ShaderConstant* constant = nullptr;
                if (info && !registerBinding)
                {
                    const auto found = std::ranges::find(info->m_constants, name, &d3d9::ShaderConstant::m_name);
                    if (found != info->m_constants.end())
                        constant = &*found;
                }
                const bool sampler = name == "samplerRegister" || (constant && constant->m_register_set == d3d9::RegisterSet::SAMPLER);
                if (vertex && sampler)
                    Error("vertex sampler arguments are not supported");
                if (!explicitLayout && !registerBinding)
                {
                    if (!constant)
                        Error(std::format("shader has no reflection for \"{}\"; use a register binding", name));
                    const auto elements = std::max(constant->m_type_elements, 1u);
                    const auto registers = constant->m_register_count / elements;
                    if (element.value_or(0) >= elements || !registers || registers > UINT8_MAX
                        || constant->m_register_index + element.value_or(0) * registers > UINT16_MAX)
                        Error("invalid shader parameter index");
                    arg.dest = static_cast<uint16_t>(constant->m_register_index + element.value_or(0) * registers);
                    rowCount = static_cast<uint8_t>(registers);
                }
                if (explicitLayout && !registerBinding && !constant)
                    Error(std::format("unknown shader parameter \"{}\"", name));
                Expect('=');
                const auto source = Name();
                if (source == "float4" || source == "null")
                {
                    if (sampler)
                        Error("samplers cannot use a literal value");
                    arg.type = vertex ? MTL_ARG_LITERAL_VERTEX_CONST : MTL_ARG_LITERAL_PIXEL_CONST;
                    if (source == "float4")
                    {
                        Expect('(');
                        arg.u.literalConst = m_memory.Alloc<float[4]>();
                        for (auto i = 0u; i < 4u; ++i)
                        {
                            if (i)
                                Expect(',');
                            (*arg.u.literalConst)[i] = static_cast<float>(Number());
                        }
                        Expect(')');
                    }
                }
                else
                {
                    Expect('.');
                    const bool raw = Accept('#');
                    unsigned value{};
                    std::string accessor;
                    if (raw)
                        value = Integer();
                    else
                        accessor = Accessor();
                    if (source == "material")
                    {
                        arg.type = vertex ? MTL_ARG_MATERIAL_VERTEX_CONST : sampler ? MTL_ARG_MATERIAL_PIXEL_SAMPLER : MTL_ARG_MATERIAL_PIXEL_CONST;
                        arg.u.nameHash = raw ? value : IW3::Common::R_HashString(accessor.c_str());
                    }
                    else if (source == "constant")
                    {
                        if (sampler)
                            Error("samplers cannot use a code constant");
                        if (!raw)
                        {
                            const auto index = commonCodeSourceInfos.GetCodeConstSourceForAccessor(accessor);
                            if (!index)
                                Error(std::format("unknown code constant \"{}\"", accessor));
                            value = *index;
                            const auto sourceInfo = commonCodeSourceInfos.GetInfoForCodeConstSource(*index);
                            const auto arrayIndex = Index().value_or(0);
                            if (!sourceInfo || arrayIndex >= std::max<unsigned>(sourceInfo->arrayCount, 1u))
                                Error("code constant array index out of range");
                            value += arrayIndex;
                        }
                        if (value > UINT16_MAX)
                            Error("code constant index exceeds 65535");
                        arg.type = vertex ? MTL_ARG_CODE_VERTEX_CONST : MTL_ARG_CODE_PIXEL_CONST;
                        arg.u.codeConst = {static_cast<uint16_t>(value), firstRow, rowCount};
                    }
                    else if (source == "sampler")
                    {
                        if (vertex || !sampler)
                            Error("code samplers require a pixel sampler destination");
                        if (!raw)
                        {
                            const auto index = commonCodeSourceInfos.GetCodeSamplerSourceForAccessor(accessor);
                            if (!index)
                                Error(std::format("unknown code sampler \"{}\"", accessor));
                            value = *index;
                        }
                        arg.type = MTL_ARG_CODE_PIXEL_SAMPLER;
                        arg.u.codeSampler = value;
                    }
                    else
                        Error(std::format("unknown binding source \"{}\"", source));
                }
                Expect(';');
                if (!args.emplace(order, arg).second)
                    Error("duplicate argument index");
            }
        }

        void ReadArguments(techset::CommonShaderArgCreator& creator, const bool vertex, const std::string& shaderName)
        {
            Check(creator.EnterShader(vertex ? techset::CommonTechniqueShaderType::VERTEX : techset::CommonTechniqueShaderType::PIXEL, shaderName));
            while (!Accept('}'))
            {
                techset::CommonShaderArgCreatorDestination destination(Name());
                destination.m_argument_index = Index();
                Expect('=');
                const auto source = Name();
                if (source == "float4")
                {
                    Expect('(');
                    std::array<float, 4> value{};
                    for (auto i = 0u; i < value.size(); ++i)
                    {
                        if (i)
                            Expect(',');
                        value[i] = static_cast<float>(Number());
                    }
                    Expect(')');
                    Check(creator.AcceptShaderLiteralArgument(destination, value));
                }
                else
                {
                    Expect('.');
                    const bool raw = Accept('#');
                    const auto value = raw ? Integer() : 0u;
                    const auto accessor = raw ? std::string() : Accessor();
                    if (source == "material")
                    {
                        Check(raw ? creator.AcceptShaderMaterialArgument(destination, value) : creator.AcceptShaderMaterialArgument(destination, accessor));
                    }
                    else if (source == "constant")
                    {
                        const auto index = raw && value <= UINT8_MAX ? std::optional(static_cast<uint8_t>(value))
                                                                     : commonCodeSourceInfos.GetCodeConstSourceForAccessor(accessor);
                        if (!index)
                            Error(std::format("unknown code constant \"{}\"", accessor));
                        Check(creator.AcceptShaderConstantArgument(destination, *index, Index().value_or(0)));
                    }
                    else if (source == "sampler")
                    {
                        const auto index = raw && value <= UINT8_MAX ? std::optional(static_cast<uint8_t>(value))
                                                                     : commonCodeSourceInfos.GetCodeSamplerSourceForAccessor(accessor);
                        if (!index)
                            Error(std::format("unknown code sampler \"{}\"", accessor));
                        Check(creator.AcceptShaderSamplerArgument(destination, *index));
                    }
                    else
                        Error(std::format("unknown binding source \"{}\"", source));
                }
                Expect(';');
            }
            Check(creator.LeaveShader());
        }

        void Check(std::expected<void, std::string> result)
        {
            if (!result)
                Error(result.error());
        }

        void ConvertArguments(MaterialPass& pass, const techset::CommonPass& commonPass)
        {
            const auto counts = commonPass.GetFrequencyCounts(commonCodeSourceInfos);
            for (auto i = 0u; i < 3u; ++i)
                if (counts[i] > UINT8_MAX)
                    Error("argument group exceeds 255 entries");
            pass.perPrimArgCount = static_cast<uint8_t>(counts[0]);
            pass.perObjArgCount = static_cast<uint8_t>(counts[1]);
            pass.stableArgCount = static_cast<uint8_t>(counts[2]);
            const auto count = counts[0] + counts[1] + counts[2];
            if (!count)
                return;
            pass.args = m_memory.Alloc<MaterialShaderArgument>(count);
            for (auto i = 0uz; i < count; ++i)
            {
                auto& arg = pass.args[i];
                const auto& commonArg = commonPass.m_args[i];
                const auto type = commonCodeSourceInfos.GetArgumentTypeNumericValue(commonArg.m_type);
                if (!type || commonArg.m_destination.dx9.m_destination_register > UINT16_MAX)
                    Error("invalid compiled shader argument");
                arg.type = static_cast<uint16_t>(*type);
                arg.dest = static_cast<uint16_t>(commonArg.m_destination.dx9.m_destination_register);
                switch (commonArg.m_type.m_value_type)
                {
                case techset::CommonShaderValueType::LITERAL_CONST:
                    arg.u.literalConst = m_memory.Alloc<float[4]>();
                    std::ranges::copy(commonArg.m_value.literal_value, *arg.u.literalConst);
                    break;
                case techset::CommonShaderValueType::CODE_CONST:
                {
                    const auto& value = commonArg.m_value.code_const_source;
                    if (value.m_first_row > UINT8_MAX || value.m_row_count > UINT8_MAX)
                        Error("invalid code constant row range");
                    arg.u.codeConst = {value.m_index, static_cast<uint8_t>(value.m_first_row), static_cast<uint8_t>(value.m_row_count)};
                    break;
                }
                case techset::CommonShaderValueType::CODE_SAMPLER:
                    arg.u.codeSampler = commonArg.m_value.code_sampler_source;
                    break;
                case techset::CommonShaderValueType::MATERIAL_CONST:
                case techset::CommonShaderValueType::MATERIAL_SAMPLER:
                    arg.u.nameHash = commonArg.m_value.name_hash;
                    break;
                default:
                    Error("unsupported compiled shader argument");
                }
            }
        }

        MaterialPass ReadPass()
        {
            MaterialPass pass{};
            TechniqueShaderLoader shaderLoader(pass);
            const auto creator = techset::CommonShaderArgCreator::CreateDx9(shaderLoader, m_context, commonCodeSourceInfos);
            std::vector<MaterialStreamRouting> routing;
            std::map<unsigned, MaterialShaderArgument> args;
            std::optional<bool> optionalSource;
            std::array<bool, VERTDECL_COUNT> familyDeclarations{};
            bool explicitVariantNames = false;
            bool explicitCounts = false, explicitVariants = false, explicitSamplers = false, vertexSeen = false, pixelSeen = false;
            while (!Accept('}'))
            {
                const auto command = Name();
                if (command == "stateMap")
                {
                    if (Name() != "passthrough")
                        Error("native techniques require passthrough; compiled state bits are stored in material files");
                }
                else if (command == "xenonArgumentCounts")
                {
                    if (explicitCounts || vertexSeen || pixelSeen)
                        Error("argument counts must appear once, before the shader blocks");
                    explicitCounts = true;
                    pass.perPrimArgCount = static_cast<uint8_t>(Integer(UINT8_MAX));
                    Expect(',');
                    pass.perObjArgCount = static_cast<uint8_t>(Integer(UINT8_MAX));
                    Expect(',');
                    pass.stableArgCount = static_cast<uint8_t>(Integer(UINT8_MAX));
                }
                else if (command == "xenonCustomSamplerFlags")
                {
                    explicitSamplers = true;
                    pass.customSamplerFlags = static_cast<uint8_t>(Integer(UINT8_MAX));
                }
                else if (command == "xenonPrecompiledIndex")
                    pass.precompiledIndex = static_cast<uint8_t>(Integer(UINT8_MAX));
                else if (command == "xenonOptionalSource")
                    optionalSource = Integer(1) != 0;
                else if (command == "xenonVertexShaders")
                {
                    if (explicitVariants || vertexSeen)
                        Error("vertex shader variants must appear once, before the vertex shader block");
                    explicitVariants = true;
                    Expect('{');
                    std::array<bool, VERTDECL_COUNT> assigned{};
                    while (!Accept('}'))
                    {
                        const auto declaration = Name();
                        const auto found = std::ranges::find(vertexDeclNames, declaration);
                        if (found == std::end(vertexDeclNames))
                            Error(std::format("unknown vertex declaration \"{}\"", declaration));
                        const auto index = found - std::begin(vertexDeclNames);
                        if (assigned[index])
                            Error("duplicate vertex shader variant");
                        assigned[index] = true;
                        if (Accept(';'))
                            familyDeclarations[index] = true;
                        else
                        {
                            explicitVariantNames = true;
                            pass.vertexShaderArray[index] = VertexShader(Name());
                            Expect(';');
                        }
                    }
                    continue;
                }
                else if (command == "vertexShader" || command == "pixelShader")
                {
                    const bool vertex = command == "vertexShader";
                    if (vertex ? vertexSeen : pixelSeen)
                        Error("duplicate shader block");
                    (vertex ? vertexSeen : pixelSeen) = true;
                    Number(); // Profile is descriptive; the shader object supplies the actual shader version.
                    const auto shaderName = Name();
                    if (vertex
                        && std::ranges::any_of(familyDeclarations,
                                               [](const bool selected)
                                               {
                                                   return selected;
                                               }))
                    {
                        if (explicitVariantNames)
                            Error("vertex shader families cannot mix declarations with explicit shader names");
                        ReadVertexShaderFamily(pass, shaderName, familyDeclarations);
                    }
                    else if (vertex)
                        pass.vertexShader = VertexShader(shaderName);
                    else
                        pass.pixelShader = PixelShader(shaderName);
                    Expect('{');
                    if (explicitCounts)
                        ReadCompiledArguments(pass, vertex, args);
                    else
                        ReadArguments(*creator, vertex, shaderName);
                    continue;
                }
                else if (command == "vertex")
                {
                    Expect('.');
                    const auto destination = commonRoutingInfos.GetDestinationByName(IndexedAccessor());
                    Expect('=');
                    if (Name() != "code")
                        Error("expected a code stream source");
                    Expect('.');
                    const auto source = commonRoutingInfos.GetSourceByName(IndexedAccessor());
                    if (!source || !destination || routing.size() >= std::size(MaterialVertexStreamRouting{}.data))
                        Error("invalid vertex stream routing");
                    routing.push_back({*source, *destination});
                }
                else
                    Error(std::format("unknown pass field \"{}\"", command));
                Expect(';');
            }
            if (!vertexSeen
                && std::ranges::any_of(familyDeclarations,
                                       [](const bool selected)
                                       {
                                           return selected;
                                       }))
                Error("vertex shader family declarations require a vertex shader block");
            if (!routing.empty() || optionalSource)
            {
                pass.vertexDecl = m_memory.Alloc<MaterialVertexDeclaration>();
                pass.vertexDecl->streamCount = static_cast<uint8_t>(routing.size());
                pass.vertexDecl->hasOptionalSource = optionalSource.value_or(std::ranges::any_of(routing,
                                                                                                 [](const auto& route)
                                                                                                 {
                                                                                                     return commonRoutingInfos.IsSourceOptional(route.source);
                                                                                                 }));
                std::ranges::copy(routing, pass.vertexDecl->routing.data);
            }
            if (!explicitVariants && pass.vertexShader)
                pass.vertexShaderArray[VERTDECL_GENERIC] = pass.vertexShader;
            if (!explicitCounts)
            {
                techset::CommonTechnique technique;
                technique.m_flags = 0;
                techset::CommonPass commonPass;
                commonPass.m_sampler_flags = 0;
                Check(creator->FinalizePass(technique, commonPass));
                ConvertArguments(pass, commonPass);
                flags |= static_cast<uint16_t>(technique.m_flags);
                if (!explicitSamplers)
                    pass.customSamplerFlags = static_cast<uint8_t>(commonPass.m_sampler_flags);
                return pass;
            }
            std::vector<MaterialShaderArgument> ordered;
            for (const auto& [index, arg] : args)
            {
                if (index != ordered.size())
                    Error("argument indexes must be contiguous");
                ordered.push_back(arg);
            }
            if (ordered.size() != pass.perPrimArgCount + pass.perObjArgCount + pass.stableArgCount)
                Error("argument counts do not match the bindings");
            if (!ordered.empty())
            {
                pass.args = m_memory.Alloc<MaterialShaderArgument>(ordered.size());
                for (auto i = 0uz; i < ordered.size(); ++i)
                    pass.args[i] = ordered[i];
            }
            return pass;
        }

        const std::string& m_technique_name;
        MemoryManager& m_memory;
        AssetCreationContext& m_context;
        std::optional<uint16_t> m_explicit_flags;
    };

    class TechniqueCompiler final : public SubAssetCreator<SubAssetTechnique>
    {
    public:
        TechniqueCompiler(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateSubAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto fileName = std::format("techniques/{}.tech", shader::xenon::EncodeFileName(assetName));
            const auto file = m_search_path.Open(fileName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();
            try
            {
                TechniqueReader reader(*file.m_stream, fileName, assetName, m_memory, context);
                const auto passes = reader.Read();
                const auto additionalPassCount = std::max(passes.size(), 1uz) - 1uz;
                auto* technique = static_cast<MaterialTechnique*>(m_memory.AllocRaw(sizeof(MaterialTechnique) + additionalPassCount * sizeof(MaterialPass)));
                technique->name = m_memory.Dup(assetName.c_str());
                technique->flags = reader.flags;
                technique->passCount = static_cast<uint16_t>(passes.size());
                std::ranges::copy(passes, technique->passArray);
                AssetRegistration<SubAssetTechnique> registration(assetName, technique);
                for (auto* dependency : reader.dependencies)
                    registration.AddDependency(dependency);
                return AssetCreationResult::Success(context.AddSubAsset(std::move(registration)));
            }
            catch (const std::exception& error)
            {
                con::error("Cannot load IW3 Xenon technique \"{}\": {}", assetName, error.what());
                return AssetCreationResult::Failure();
            }
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };

    class TechsetCompiler final : public AssetCreator<AssetTechniqueSet>
    {
    public:
        TechsetCompiler(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto fileName = std::format("techsets/{}.techset", shader::xenon::EncodeFileName(assetName));
            const auto file = m_search_path.Open(fileName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();
            try
            {
                TextReader reader(*file.m_stream, fileName);
                auto* techset = m_memory.Alloc<MaterialTechniqueSet>();
                techset->name = m_memory.Dup(assetName.c_str());
                AssetRegistration<AssetTechniqueSet> registration(assetName, techset);
                std::vector<size_t> slots;
                std::array<bool, TECHNIQUE_COUNT> assigned{};
                std::array<bool, IW3::TECHNIQUE_COUNT> ignored{};
                bool pendingSlots = false;
                while (!reader.End())
                {
                    const auto name = reader.Name();
                    if (name == "xenonWorldVertexFormat")
                        techset->worldVertFormat = static_cast<uint8_t>(reader.Integer(UINT8_MAX));
                    else if (name == "xenonUnused")
                    {
                        techset->unused[0] = static_cast<uint8_t>(reader.Integer(UINT8_MAX));
                        reader.Expect(',');
                        techset->unused[1] = static_cast<uint8_t>(reader.Integer(UINT8_MAX));
                    }
                    else if (reader.Accept(':'))
                    {
                        const auto slot = commonTechniqueTypeNames.GetTechniqueTypeByName(name);
                        if (slot)
                        {
                            if (assigned[*slot])
                                reader.Error("duplicate technique slot");
                            assigned[*slot] = true;
                            slots.push_back(*slot);
                        }
                        else
                        {
                            // Shared PC source can include instanced slots that do not exist on Xbox 360.
                            const auto pcSlot = IW3::commonTechniqueTypeNames.GetTechniqueTypeByName(name);
                            if (!pcSlot || ignored[*pcSlot])
                                reader.Error("unknown or duplicate technique slot");
                            ignored[*pcSlot] = true;
                        }
                        pendingSlots = true;
                        continue;
                    }
                    else
                    {
                        if (!pendingSlots || name.empty())
                            reader.Error("expected a technique slot");
                        if (!slots.empty())
                        {
                            auto* technique = context.LoadSubAsset<SubAssetTechnique>(name);
                            if (!technique)
                                return AssetCreationResult::Failure();
                            for (const auto slot : slots)
                                techset->techniques[slot] = technique->Asset();
                            registration.AddDependency(technique);
                        }
                        slots.clear();
                        pendingSlots = false;
                    }
                    reader.Expect(';');
                }
                if (pendingSlots)
                    reader.Error("technique slot has no technique name");
                return AssetCreationResult::Success(context.AddAsset(std::move(registration)));
            }
            catch (const std::exception& error)
            {
                con::error("Cannot load IW3 Xenon techniqueset \"{}\": {}", assetName, error.what());
                return AssetCreationResult::Failure();
            }
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace techset
{
    std::unique_ptr<IAssetCreator> CreateTechsetCompilerIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<TechsetCompiler>(memory, searchPath);
    }

    std::unique_ptr<ISubAssetCreator> CreateTechniqueCompilerIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<TechniqueCompiler>(memory, searchPath);
    }
} // namespace techset
