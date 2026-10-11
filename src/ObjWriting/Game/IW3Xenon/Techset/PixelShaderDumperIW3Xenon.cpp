#include "PixelShaderDumperIW3Xenon.h"

#include "Game/IW3Xenon/ObjConstantsIW3Xenon.h"
#include "Shader/XenonShader.h"
#include "Utils/Logging/Log.h"

#include <string_view>
#include <vector>

using namespace IW3Xenon;

namespace techset
{
    void PixelShaderDumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<MaterialPixelShader>& asset)
    {
        const auto& shader = *asset.Asset();
        const auto& loadDef = shader.prog.loadDef;
        if (loadDef.cachedPartSize == 0 && loadDef.physicalPartSize == 0 && std::string_view(shader.name) == "null.hlsl")
            return;

        if (!loadDef.cachedPart || loadDef.cachedPartSize < PIXEL_SHADER_RUNTIME_HEADER_SIZE || (loadDef.physicalPartSize && !loadDef.physicalPart))
        {
            con::error("Cannot dump IW3 Xenon pixel shader \"{}\": invalid shader data", asset.m_name);
            return;
        }

        std::vector<uint8_t> microcode(loadDef.cachedPart + PIXEL_SHADER_RUNTIME_HEADER_SIZE, loadDef.cachedPart + loadDef.cachedPartSize);
        if (loadDef.physicalPartSize)
            microcode.insert(microcode.end(), loadDef.physicalPart, loadDef.physicalPart + loadDef.physicalPartSize);

        const auto parts = shader::xenon::GetShaderParts(microcode, shader::xenon::ShaderStage::PIXEL);
        if (!parts || parts->cached.size() + PIXEL_SHADER_RUNTIME_HEADER_SIZE != loadDef.cachedPartSize || parts->physical.size() != loadDef.physicalPartSize)
        {
            con::error("Cannot dump IW3 Xenon pixel shader \"{}\": invalid shader object", asset.m_name);
            return;
        }

        const auto shaderFile = context.OpenAssetFile(shader::xenon::GetFileNameForPixelShaderAssetName(shader.name));
        if (shaderFile)
            shaderFile->write(reinterpret_cast<const char*>(microcode.data()), static_cast<std::streamsize>(microcode.size()));
    }
} // namespace techset
