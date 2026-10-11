#include "PixelShaderLoaderIW3Xenon.h"

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Game/IW3Xenon/ObjConstantsIW3Xenon.h"
#include "Shader/XenonShader.h"
#include "Utils/Logging/Log.h"

#include <cstring>
#include <limits>
#include <vector>

using namespace IW3Xenon;

namespace
{
    bool LoadPixelShader(const SearchPathOpenFile& file, GfxPixelShaderLoadDef& loadDef, MemoryManager& memory)
    {
        constexpr auto MAX_PART_SIZE = std::numeric_limits<uint16_t>::max();
        if (file.m_length < 0 || file.m_length > 2u * MAX_PART_SIZE - PIXEL_SHADER_RUNTIME_HEADER_SIZE)
            return false;

        std::vector<uint8_t> data(static_cast<size_t>(file.m_length));
        file.m_stream->read(reinterpret_cast<char*>(data.data()), file.m_length);
        if (file.m_stream->gcount() != file.m_length)
            return false;

        const auto parts = shader::xenon::GetShaderParts(data, shader::xenon::ShaderStage::PIXEL);
        if (!parts || parts->cached.size() + PIXEL_SHADER_RUNTIME_HEADER_SIZE > MAX_PART_SIZE || parts->physical.size() > MAX_PART_SIZE)
            return false;

        loadDef.cachedPartSize = static_cast<uint16_t>(PIXEL_SHADER_RUNTIME_HEADER_SIZE + parts->cached.size());
        // The game initializes this zero-filled D3D header when loading the shader.
        loadDef.cachedPart = memory.Alloc<uint8_t>(loadDef.cachedPartSize);
        std::memcpy(loadDef.cachedPart + PIXEL_SHADER_RUNTIME_HEADER_SIZE, parts->cached.data(), parts->cached.size());
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
            if (file.IsOpen() && !LoadPixelShader(file, pixelShader->prog.loadDef, m_memory))
            {
                con::error("Cannot load IW3 Xenon pixel shader \"{}\": invalid shader object", assetName);
                return AssetCreationResult::Failure();
            }

            return AssetCreationResult::Success(context.AddAsset<AssetPixelShader>(assetName, pixelShader));
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
} // namespace techset
