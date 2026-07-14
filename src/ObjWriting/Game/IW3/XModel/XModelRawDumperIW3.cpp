#include "XModelRawDumperIW3.h"

#include "Dumping/AssetDumpingContext.h"
#include "Game/IW3/CommonIW3.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

using namespace IW3;

namespace
{
    constexpr uint16_t XMODEL_RAW_VERSION = 25;
    constexpr auto MAX_LODS = 4u;
    constexpr auto MAX_RAW_COMPANION_NAME = 51u;
    constexpr float COLLISION_BOUNDS_EPSILON = 0.001f;

    class OutputBuffer
    {
    public:
        template<typename T> void Write(const T& value)
        {
            static_assert(std::is_trivially_copyable_v<T>);
            const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
            m_data.insert(m_data.end(), bytes, bytes + sizeof(T));
        }

        void WriteBytes(const void* data, const size_t size)
        {
            if (!size)
                return;
            const auto* bytes = static_cast<const uint8_t*>(data);
            m_data.insert(m_data.end(), bytes, bytes + size);
        }

        void WriteString(const std::string_view value)
        {
            WriteBytes(value.data(), value.size());
            Write(uint8_t{0});
        }

        [[nodiscard]] std::vector<uint8_t> Take()
        {
            return std::move(m_data);
        }

    private:
        std::vector<uint8_t> m_data;
    };

    std::string_view StripReferencePrefix(std::string_view value)
    {
        if (!value.empty() && value.front() == ',')
            value.remove_prefix(1u);
        return value;
    }

    uint64_t Fnv1a64(const std::string_view value)
    {
        uint64_t result = 14695981039346656037ull;
        for (const auto c : value)
        {
            result ^= static_cast<uint8_t>(c);
            result *= 1099511628211ull;
        }
        return result;
    }

    std::string NormalizeAssetName(std::string_view value)
    {
        value = StripReferencePrefix(value);
        std::string result(value);
        std::ranges::replace(result, '\\', '/');
        return result;
    }

    std::string GetCompanionName(const std::string& assetName, const unsigned lod)
    {
        auto result = std::format("{}_lod{}", assetName, lod);
        if (result.size() <= MAX_RAW_COMPANION_NAME)
            return result;

        return std::format("oat_{:016x}_lod{}", Fnv1a64(assetName), lod);
    }

    bool GetRawMaterialName(const Material* material, std::string& result)
    {
        if (!material || !material->info.name)
            return false;

        auto name = StripReferencePrefix(material->info.name);
        if (name.starts_with("mc/"))
            name.remove_prefix(3u);

        if (name == "$default3d")
            name = "$default";

        result.assign(name);
        return !result.empty();
    }

    std::array<float, 3> Cross(const float (&a)[3], const float (&b)[3])
    {
        return {
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0],
        };
    }

    void Normalize(std::array<float, 3>& value)
    {
        const auto lengthSquared = value[0] * value[0] + value[1] * value[1] + value[2] * value[2];
        if (lengthSquared <= std::numeric_limits<float>::epsilon())
        {
            value = {0.0f, 1.0f, 0.0f};
            return;
        }

        const auto inverseLength = 1.0f / std::sqrt(lengthSquared);
        value[0] *= inverseLength;
        value[1] *= inverseLength;
        value[2] *= inverseLength;
    }

    std::array<float, 3> InverseRotate(const vec4_t& rotation, const std::array<float, 3>& value)
    {
        auto x = -rotation.x;
        auto y = -rotation.y;
        auto z = -rotation.z;
        auto w = rotation.w;
        const auto lengthSquared = x * x + y * y + z * z + w * w;
        if (lengthSquared <= std::numeric_limits<float>::epsilon())
            return value;

        const auto inverseLength = 1.0f / std::sqrt(lengthSquared);
        x *= inverseLength;
        y *= inverseLength;
        z *= inverseLength;
        w *= inverseLength;

        const std::array<float, 3> q{x, y, z};
        const float input[3]{value[0], value[1], value[2]};
        const float quat[3]{q[0], q[1], q[2]};
        const auto cross = Cross(quat, input);
        const float doubledCross[3]{2.0f * cross[0], 2.0f * cross[1], 2.0f * cross[2]};
        const auto secondCross = Cross(quat, doubledCross);
        return {
            value[0] + w * doubledCross[0] + secondCross[0],
            value[1] + w * doubledCross[1] + secondCross[1],
            value[2] + w * doubledCross[2] + secondCross[2],
        };
    }

    bool ShouldReconstructTranslations(const XModel& model)
    {
        const auto childBoneCount = static_cast<unsigned>(model.numBones - model.numRootBones);
        if (!childBoneCount || !model.baseMat)
            return false;
        if (!model.trans)
            return true;

        for (auto index = 0u; index < childBoneCount * 3u; index++)
        {
            if (model.trans[index] != 0.0f)
                return false;
        }

        for (auto boneIndex = static_cast<unsigned>(model.numRootBones); boneIndex < model.numBones; boneIndex++)
        {
            const auto& trans = model.baseMat[boneIndex].trans;
            if (trans.x != 0.0f || trans.y != 0.0f || trans.z != 0.0f)
                return true;
        }

        return false;
    }

    std::array<float, 3> GetChildTranslation(const XModel& model, const unsigned boneIndex, const bool reconstruct)
    {
        const auto childIndex = boneIndex - model.numRootBones;
        if (!reconstruct && model.trans)
            return {model.trans[childIndex * 3u], model.trans[childIndex * 3u + 1u], model.trans[childIndex * 3u + 2u]};

        if (!model.baseMat || !model.parentList)
            return {};

        const auto parentIndex = boneIndex - model.parentList[childIndex];
        const auto& child = model.baseMat[boneIndex];
        const auto& parent = model.baseMat[parentIndex];
        const std::array<float, 3> delta{
            child.trans.x - parent.trans.x,
            child.trans.y - parent.trans.y,
            child.trans.z - parent.trans.z,
        };
        return InverseRotate(parent.quat, delta);
    }

    bool BuildParts(const XModel& model, const ZoneScriptStrings& scriptStrings, std::vector<uint8_t>& result, std::string& error)
    {
        if (model.numRootBones > model.numBones || model.numBones > 127u)
        {
            error = std::format("invalid bone counts: roots={}, total={}", model.numRootBones, model.numBones);
            return false;
        }
        if (model.numBones && (!model.boneNames || !model.partClassification))
        {
            error = "missing bone names or part classifications";
            return false;
        }

        const auto childBoneCount = static_cast<uint16_t>(model.numBones - model.numRootBones);
        OutputBuffer output;
        output.Write(XMODEL_RAW_VERSION);
        output.Write(childBoneCount);
        output.Write(static_cast<uint16_t>(model.numRootBones));

        const auto reconstructTranslations = ShouldReconstructTranslations(model);
        for (auto childIndex = 0u; childIndex < childBoneCount; childIndex++)
        {
            const auto boneIndex = static_cast<unsigned>(model.numRootBones) + childIndex;
            if (!model.parentList || model.parentList[childIndex] == 0u || model.parentList[childIndex] > boneIndex)
            {
                error = std::format("bone {} has an invalid parent delta", boneIndex);
                return false;
            }

            const auto parentIndex = boneIndex - model.parentList[childIndex];
            output.Write(static_cast<uint8_t>(parentIndex));

            const auto translation = GetChildTranslation(model, boneIndex, reconstructTranslations);
            output.Write(translation[0]);
            output.Write(translation[1]);
            output.Write(translation[2]);

            std::array<int16_t, 3> quaternion{};
            if (model.quats)
            {
                const auto& source = model.quats[childIndex];
                const auto negate = source.v[3] < 0;
                for (auto component = 0u; component < quaternion.size(); component++)
                {
                    const auto value = static_cast<int32_t>(source.v[component]);
                    quaternion[component] = static_cast<int16_t>(negate ? std::clamp(-value, -32767, 32767) : value);
                }
            }
            output.Write(quaternion[0]);
            output.Write(quaternion[1]);
            output.Write(quaternion[2]);
        }

        for (auto boneIndex = 0u; boneIndex < model.numBones; boneIndex++)
        {
            const auto scriptString = static_cast<size_t>(model.boneNames[boneIndex]);
            if (scriptString >= scriptStrings.Count())
            {
                error = std::format("bone {} references invalid script string {}", boneIndex, scriptString);
                return false;
            }
            output.WriteString(scriptStrings[scriptString]);
        }

        output.WriteBytes(model.partClassification, model.numBones);
        output.Write(uint8_t{1});
        result = output.Take();
        return true;
    }

    bool WriteVertexBase(OutputBuffer& output, const GfxPackedVertex& vertex)
    {
        float unpackedNormal[3]{};
        float unpackedTangent[3]{};
        float uv[2]{};
        Common::Vec3UnpackUnitVec(vertex.normal, unpackedNormal);
        Common::Vec3UnpackUnitVec(vertex.tangent, unpackedTangent);
        Common::Vec2UnpackTexCoords(vertex.texCoord, uv);

        std::array normal{unpackedNormal[0], unpackedNormal[1], unpackedNormal[2]};
        std::array tangent{unpackedTangent[0], unpackedTangent[1], unpackedTangent[2]};
        Normalize(normal);

        // Packed runtime vectors are quantized independently. Re-orthogonalize the
        // source basis because the stock raw loader asserts N.T, N.B and T.B are
        // within 0.002 of zero before packing them again.
        const auto normalDotTangent = normal[0] * tangent[0] + normal[1] * tangent[1] + normal[2] * tangent[2];
        for (auto axis = 0u; axis < 3u; axis++)
            tangent[axis] -= normalDotTangent * normal[axis];
        const auto tangentLengthSquared = tangent[0] * tangent[0] + tangent[1] * tangent[1] + tangent[2] * tangent[2];
        if (tangentLengthSquared <= std::numeric_limits<float>::epsilon())
        {
            const std::array reference = std::abs(normal[2]) < 0.9f ? std::array{0.0f, 0.0f, 1.0f} : std::array{0.0f, 1.0f, 0.0f};
            tangent = {
                reference[1] * normal[2] - reference[2] * normal[1],
                reference[2] * normal[0] - reference[0] * normal[2],
                reference[0] * normal[1] - reference[1] * normal[0],
            };
        }
        Normalize(tangent);

        output.Write(normal[0]);
        output.Write(normal[1]);
        output.Write(normal[2]);
        output.WriteBytes(vertex.color.array, sizeof(vertex.color.array));
        output.Write(uv[0]);
        output.Write(uv[1]);

        float normalArray[3]{normal[0], normal[1], normal[2]};
        float tangentArray[3]{tangent[0], tangent[1], tangent[2]};
        auto binormal = Cross(normalArray, tangentArray);
        Normalize(binormal);
        if (vertex.binormalSign < 0.0f)
        {
            binormal[0] = -binormal[0];
            binormal[1] = -binormal[1];
            binormal[2] = -binormal[2];
        }
        output.Write(binormal[0]);
        output.Write(binormal[1]);
        output.Write(binormal[2]);
        output.Write(tangent[0]);
        output.Write(tangent[1]);
        output.Write(tangent[2]);
        return true;
    }

    bool BuildSurface(const XSurface& surface, OutputBuffer& output, std::string& error)
    {
        if (!surface.verts0 || !surface.triIndices || !surface.vertCount || !surface.triCount)
        {
            error = "surface has no vertices or triangles";
            return false;
        }

        output.Write(static_cast<uint8_t>(surface.tileMode));
        output.Write(uint16_t{0}); // Read and ignored by the stock version-25 loader.
        output.Write(surface.vertCount);
        output.Write(surface.triCount);

        uint32_t rigidVertexCount = 0u;
        if (!surface.deformed && surface.vertList && surface.vertListCount)
        {
            for (auto listIndex = 0u; listIndex < surface.vertListCount; listIndex++)
            {
                const auto& list = surface.vertList[listIndex];
                if (list.boneOffset % sizeof(DObjSkelMat) != 0u)
                {
                    error = std::format("rigid list {} has unaligned bone offset {}", listIndex, list.boneOffset);
                    return false;
                }
                output.Write(list.vertCount);
                output.Write(static_cast<uint16_t>(list.boneOffset / sizeof(DObjSkelMat)));
                rigidVertexCount += list.vertCount;
            }
        }

        if (!surface.deformed && rigidVertexCount != surface.vertCount)
        {
            error = std::format("rigid lists cover {} vertices but surface contains {}", rigidVertexCount, surface.vertCount);
            return false;
        }
        output.Write(uint16_t{0});

        const auto singleRigidList = !surface.deformed && surface.vertListCount == 1u;
        uint32_t blendCount = 0u;
        if (!singleRigidList)
        {
            if (surface.deformed)
            {
                uint32_t groupedVertexCount = 0u;
                for (auto group = 0u; group < 4u; group++)
                {
                    if (surface.vertInfo.vertCount[group] < 0)
                    {
                        error = std::format("negative vertex blend group {}", group);
                        return false;
                    }
                    groupedVertexCount += static_cast<uint16_t>(surface.vertInfo.vertCount[group]);
                    blendCount += group * static_cast<uint16_t>(surface.vertInfo.vertCount[group]);
                }
                if (!surface.vertInfo.vertsBlend || groupedVertexCount != surface.vertCount || blendCount > std::numeric_limits<uint16_t>::max())
                {
                    error =
                        std::format("invalid deformed vertex groups: grouped={}, vertices={}, blends={}", groupedVertexCount, surface.vertCount, blendCount);
                    return false;
                }
            }
            output.Write(static_cast<uint16_t>(blendCount));
        }

        auto rigidListIndex = 0u;
        auto rigidListEnd = surface.vertListCount ? static_cast<uint32_t>(surface.vertList[0].vertCount) : 0u;
        auto blendOffset = 0u;
        auto blendGroup = 0u;
        auto blendGroupEnd = surface.deformed ? static_cast<uint32_t>(surface.vertInfo.vertCount[0]) : 0u;

        for (auto vertexIndex = 0u; vertexIndex < surface.vertCount; vertexIndex++)
        {
            const auto& vertex = surface.verts0[vertexIndex];
            WriteVertexBase(output, vertex);

            if (singleRigidList)
            {
                output.Write(vertex.xyz.x);
                output.Write(vertex.xyz.y);
                output.Write(vertex.xyz.z);
                continue;
            }

            uint8_t additionalWeightCount = 0u;
            uint16_t primaryBoneIndex = 0u;
            if (surface.deformed)
            {
                while (blendGroup < 3u && vertexIndex >= blendGroupEnd)
                {
                    blendGroup++;
                    blendGroupEnd += static_cast<uint16_t>(surface.vertInfo.vertCount[blendGroup]);
                }
                additionalWeightCount = static_cast<uint8_t>(blendGroup);
                const auto primaryOffset = surface.vertInfo.vertsBlend[blendOffset++];
                if (primaryOffset % sizeof(DObjSkelMat) != 0u)
                {
                    error = std::format("vertex {} has unaligned primary bone offset {}", vertexIndex, primaryOffset);
                    return false;
                }
                primaryBoneIndex = static_cast<uint16_t>(primaryOffset / sizeof(DObjSkelMat));
            }
            else
            {
                while (rigidListIndex + 1u < surface.vertListCount && vertexIndex >= rigidListEnd)
                {
                    rigidListIndex++;
                    rigidListEnd += surface.vertList[rigidListIndex].vertCount;
                }
                primaryBoneIndex = static_cast<uint16_t>(surface.vertList[rigidListIndex].boneOffset / sizeof(DObjSkelMat));
            }

            output.Write(additionalWeightCount);
            output.Write(primaryBoneIndex);
            output.Write(vertex.xyz.x);
            output.Write(vertex.xyz.y);
            output.Write(vertex.xyz.z);

            for (auto weight = 0u; weight < additionalWeightCount; weight++)
            {
                const auto boneOffset = surface.vertInfo.vertsBlend[blendOffset++];
                const auto boneWeight = surface.vertInfo.vertsBlend[blendOffset++];
                if (boneOffset % sizeof(DObjSkelMat) != 0u)
                {
                    error = std::format("vertex {} blend {} has unaligned bone offset {}", vertexIndex, weight, boneOffset);
                    return false;
                }
                output.Write(static_cast<uint16_t>(boneOffset / sizeof(DObjSkelMat)));
                output.Write(boneWeight);
            }
        }

        for (auto triangleIndex = 0u; triangleIndex < surface.triCount; triangleIndex++)
        {
            output.Write(surface.triIndices[triangleIndex].i[0]);
            output.Write(surface.triIndices[triangleIndex].i[1]);
            output.Write(surface.triIndices[triangleIndex].i[2]);
        }
        return true;
    }

    bool BuildSurfaces(const XModel& model, const unsigned lod, std::vector<uint8_t>& result, std::string& error)
    {
        if (lod >= model.numLods)
        {
            error = std::format("LOD {} is outside model LOD count {}", lod, model.numLods);
            return false;
        }

        const auto& lodInfo = model.lodInfo[lod];
        if (!model.surfs || !lodInfo.numsurfs || static_cast<unsigned>(lodInfo.surfIndex) + lodInfo.numsurfs > model.numsurfs)
        {
            error = std::format("LOD {} has an invalid surface range", lod);
            return false;
        }

        OutputBuffer output;
        output.Write(XMODEL_RAW_VERSION);
        output.Write(lodInfo.numsurfs);
        for (auto surfaceIndex = 0u; surfaceIndex < lodInfo.numsurfs; surfaceIndex++)
        {
            if (!BuildSurface(model.surfs[lodInfo.surfIndex + surfaceIndex], output, error))
            {
                error = std::format("LOD {} surface {}: {}", lod, surfaceIndex, error);
                return false;
            }
        }

        result = output.Take();
        return true;
    }

    bool BuildModel(const XModel& model, const std::array<std::string, MAX_LODS>& surfaceNames, std::vector<uint8_t>& result, std::string& error)
    {
        if (!model.numLods || model.numLods > MAX_LODS)
        {
            error = std::format("invalid LOD count {}", model.numLods);
            return false;
        }
        if (model.numBones && !model.boneInfo)
        {
            error = "missing bone bounds";
            return false;
        }

        OutputBuffer output;
        output.Write(XMODEL_RAW_VERSION);
        output.Write(model.flags);
        for (auto axis = 0u; axis < 3u; axis++)
            output.Write(-model.radius);
        for (auto axis = 0u; axis < 3u; axis++)
            output.Write(model.radius);

        output.WriteString(model.physPreset && model.physPreset->name ? StripReferencePrefix(model.physPreset->name) : std::string_view{});
        for (auto lod = 0u; lod < MAX_LODS; lod++)
        {
            const auto compiledDistance = lod < model.numLods ? model.lodInfo[lod].dist : 0.0f;
            output.Write(compiledDistance >= 1000000.0f ? 0.0f : compiledDistance);
            output.WriteString(lod < model.numLods ? std::string_view(surfaceNames[lod]) : std::string_view{});
        }
        output.Write(static_cast<int32_t>(model.collLod));

        if (model.numCollSurfs < 0 || (model.numCollSurfs && !model.collSurfs))
        {
            error = "invalid collision surface table";
            return false;
        }
        output.Write(static_cast<int32_t>(model.numCollSurfs));
        for (auto surfaceIndex = 0; surfaceIndex < model.numCollSurfs; surfaceIndex++)
        {
            const auto& surface = model.collSurfs[surfaceIndex];
            if (surface.numCollTris <= 0 || !surface.collTris)
            {
                error = std::format("collision surface {} has no triangles", surfaceIndex);
                return false;
            }
            output.Write(static_cast<int32_t>(surface.numCollTris));
            for (auto triangleIndex = 0; triangleIndex < surface.numCollTris; triangleIndex++)
            {
                const auto& triangle = surface.collTris[triangleIndex];
                output.WriteBytes(triangle.plane, sizeof(triangle.plane));
                output.WriteBytes(triangle.svec, sizeof(triangle.svec));
                output.WriteBytes(triangle.tvec, sizeof(triangle.tvec));
            }
            for (auto axis = 0u; axis < 3u; axis++)
                output.Write(surface.mins[axis] + COLLISION_BOUNDS_EPSILON);
            for (auto axis = 0u; axis < 3u; axis++)
                output.Write(surface.maxs[axis] - COLLISION_BOUNDS_EPSILON);
            output.Write(static_cast<int32_t>(surface.boneIdx));
            output.Write(static_cast<int32_t>(surface.contents));
            output.Write(static_cast<int32_t>(surface.surfFlags));
        }

        if (!model.materialHandles && model.numsurfs)
        {
            error = "missing material handle table";
            return false;
        }
        for (auto lod = 0u; lod < model.numLods; lod++)
        {
            const auto& lodInfo = model.lodInfo[lod];
            if (static_cast<unsigned>(lodInfo.surfIndex) + lodInfo.numsurfs > model.numsurfs)
            {
                error = std::format("LOD {} has an invalid material range", lod);
                return false;
            }
            output.Write(lodInfo.numsurfs);
            for (auto surfaceIndex = 0u; surfaceIndex < lodInfo.numsurfs; surfaceIndex++)
            {
                std::string materialName;
                if (!GetRawMaterialName(model.materialHandles[lodInfo.surfIndex + surfaceIndex], materialName))
                {
                    error = std::format("LOD {} surface {} has no material name", lod, surfaceIndex);
                    return false;
                }
                output.WriteString(materialName);
            }
        }

        for (auto boneIndex = 0u; boneIndex < model.numBones; boneIndex++)
        {
            output.Write(model.boneInfo[boneIndex].bounds[0].x);
            output.Write(model.boneInfo[boneIndex].bounds[0].y);
            output.Write(model.boneInfo[boneIndex].bounds[0].z);
            output.Write(model.boneInfo[boneIndex].bounds[1].x);
            output.Write(model.boneInfo[boneIndex].bounds[1].y);
            output.Write(model.boneInfo[boneIndex].bounds[1].z);
        }

        result = output.Take();
        return true;
    }

    bool WriteFile(AssetDumpingContext& context, const std::string& name, const std::vector<uint8_t>& data)
    {
        const auto file = context.OpenAssetFile(name);
        if (!file)
            return false;
        file->write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        return file->good();
    }
} // namespace

namespace xmodel
{
    bool BuildRawXModelFilesIW3(
        const XModel& model, const std::string_view assetName, const ZoneScriptStrings& scriptStrings, RawXModelFilesIW3& output, std::string& error)
    {
        output = {};
        const auto normalizedName = NormalizeAssetName(assetName);
        if (normalizedName.empty())
        {
            error = "model has no asset name";
            return false;
        }
        if (!model.numLods || model.numLods > MAX_LODS)
        {
            error = std::format("invalid LOD count {}", model.numLods);
            return false;
        }

        for (auto lod = 0u; lod < model.numLods; lod++)
            output.surfaceNames[lod] = GetCompanionName(normalizedName, lod);
        output.partsName = output.surfaceNames[0];

        if (!BuildParts(model, scriptStrings, output.parts, error))
            return false;
        for (auto lod = 0u; lod < model.numLods; lod++)
        {
            if (!BuildSurfaces(model, lod, output.surfaces[lod], error))
                return false;
        }
        return BuildModel(model, output.surfaceNames, output.model, error);
    }

    void DumpRawXModelIW3(AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        RawXModelFilesIW3 files;
        std::string error;
        if (!BuildRawXModelFilesIW3(*asset.Asset(), asset.m_name, context.m_zone.m_script_strings, files, error))
        {
            con::warn("Cannot dump IW3 raw xmodel \"{}\": {}", asset.m_name, error);
            return;
        }

        const auto normalizedName = NormalizeAssetName(asset.m_name);
        if (!WriteFile(context, std::format("xmodel/{}", normalizedName), files.model)
            || !WriteFile(context, std::format("xmodelparts/{}", files.partsName), files.parts))
        {
            con::warn("Could not write IW3 raw xmodel bundle for \"{}\"", asset.m_name);
            return;
        }
        for (auto lod = 0u; lod < asset.Asset()->numLods; lod++)
        {
            if (!WriteFile(context, std::format("xmodelsurfs/{}", files.surfaceNames[lod]), files.surfaces[lod]))
            {
                con::warn("Could not write IW3 raw xmodelsurfs LOD {} for \"{}\"", lod, asset.m_name);
                return;
            }
        }

        if (asset.Asset()->physGeoms)
            con::warn("IW3 raw xmodel \"{}\" has physics geometry; phys_collmaps output is not implemented yet", asset.m_name);
    }
} // namespace xmodel
