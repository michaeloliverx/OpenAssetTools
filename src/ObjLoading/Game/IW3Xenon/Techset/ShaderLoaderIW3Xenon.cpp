#include "ShaderLoaderIW3Xenon.h"

#include "Game/IW3Xenon/Techset/TechsetConstantsIW3Xenon.h"
#include "Shader/XenonShader.h"
#include "Utils/Logging/Log.h"

#include <cstring>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace IW3Xenon;

namespace
{
    template<typename LoadDef>
    bool LoadShader(
        const SearchPathOpenFile& file, const shader::xenon::ShaderStage stage, const size_t runtimeHeaderSize, LoadDef& loadDef, MemoryManager& memory)
    {
        constexpr auto MAX_PART_SIZE = std::numeric_limits<uint16_t>::max();
        if (file.m_length < 0 || file.m_length > 2u * MAX_PART_SIZE - runtimeHeaderSize)
            return false;

        std::vector<uint8_t> data(static_cast<size_t>(file.m_length));
        file.m_stream->read(reinterpret_cast<char*>(data.data()), file.m_length);
        if (file.m_stream->gcount() != file.m_length)
            return false;

        const auto parts = shader::xenon::GetShaderParts(data, stage);
        if (!parts || parts->cached.size() + runtimeHeaderSize > MAX_PART_SIZE || parts->physical.size() > MAX_PART_SIZE)
            return false;

        loadDef.cachedPartSize = static_cast<uint16_t>(runtimeHeaderSize + parts->cached.size());
        // IW3 fastfiles reserve zero-filled space for the D3D header, which the game initializes when loading.
        loadDef.cachedPart = memory.Alloc<uint8_t>(loadDef.cachedPartSize);
        std::memcpy(loadDef.cachedPart + runtimeHeaderSize, parts->cached.data(), parts->cached.size());
        loadDef.physicalPartSize = static_cast<uint16_t>(parts->physical.size());
        if (!parts->physical.empty())
        {
            loadDef.physicalPart = memory.Alloc<uint8_t>(parts->physical.size());
            std::memcpy(loadDef.physicalPart, parts->physical.data(), parts->physical.size());
        }
        return true;
    }

    class PixelShaderLoader final : public AssetCreator<AssetPixelShader>
    {
    public:
        PixelShaderLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto fileName = shader::xenon::GetFileNameForPixelShaderAssetName(assetName);
            const auto file = m_search_path.Open(fileName);
            if (!file.IsOpen() && assetName != "null.hlsl")
                return AssetCreationResult::NoAction();

            auto* pixelShader = m_memory.Alloc<MaterialPixelShader>();
            pixelShader->name = m_memory.Dup(assetName.c_str());
            pixelShader->prog.loadDef = {};
            try
            {
                if (file.IsOpen()
                    && !LoadShader(file, shader::xenon::ShaderStage::PIXEL, PIXEL_SHADER_RUNTIME_HEADER_SIZE, pixelShader->prog.loadDef, m_memory))
                    throw std::runtime_error("invalid shader object");
            }
            catch (const std::exception& error)
            {
                con::error("Cannot load IW3 Xenon pixel shader \"{}\": {}", assetName, error.what());
                return AssetCreationResult::Failure();
            }
            return AssetCreationResult::Success(context.AddAsset<AssetPixelShader>(assetName, pixelShader));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };

    class VertexShaderLoader final : public SubAssetCreator<SubAssetVertexShader>
    {
    public:
        VertexShaderLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateSubAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto fileName = shader::xenon::GetFileNameForVertexShaderAssetName(assetName);
            const auto file = m_search_path.Open(fileName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            auto* vertexShader = m_memory.Alloc<MaterialVertexShader>();
            vertexShader->name = m_memory.Dup(assetName.c_str());
            vertexShader->prog.loadDef = {};
            try
            {
                if (!LoadShader(file, shader::xenon::ShaderStage::VERTEX, VERTEX_SHADER_RUNTIME_HEADER_SIZE, vertexShader->prog.loadDef, m_memory))
                    throw std::runtime_error("invalid shader object");
            }
            catch (const std::exception& error)
            {
                con::error("Cannot load IW3 Xenon vertex shader \"{}\": {}", assetName, error.what());
                return AssetCreationResult::Failure();
            }
            return AssetCreationResult::Success(context.AddSubAsset<SubAssetVertexShader>(assetName, vertexShader));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace techset
{
    std::unique_ptr<IAssetCreator> CreatePixelShaderLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<PixelShaderLoader>(memory, searchPath);
    }

    std::unique_ptr<ISubAssetCreator> CreateVertexShaderLoaderIW3Xenon(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<VertexShaderLoader>(memory, searchPath);
    }
} // namespace techset
