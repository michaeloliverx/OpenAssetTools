
#include "XModelToCommonConverterIW3Xenon253.h"

#include "Game/IW3Xenon253/XModel/XModelConstantsIW3Xenon253.h"
#include "ObjWriting.h"
#include "Utils/DistinctMapper.h"
#include "Utils/Logging/Log.h"
#include "Utils/Pack.h"
#include "Utils/QuatInt16.h"
#include "XModel/XModelWriter.h"

#include <cassert>
#include <format>
#include <string_view>
#include <utility>

using namespace IW3Xenon253;

namespace
{
    struct XSurfaceTri
    {
        uint16_t i[3];
    };

    XSurfaceTri* GetTriangles(const XSurface& surface)
    {
        return reinterpret_cast<XSurfaceTri*>(surface.triIndices);
    }

    GfxImage* GetImageFromTextureDef(const MaterialTextureDef& textureDef)
    {
        return textureDef.u.image;
    }

    GfxImage* GetMaterialColorMap(const Material* material)
    {
        std::vector<MaterialTextureDef*> potentialTextureDefs;

        for (auto textureIndex = 0u; textureIndex < material->textureCount; textureIndex++)
        {
            MaterialTextureDef* def = &material->textureTable[textureIndex];

            if (def->semantic == TS_COLOR_MAP)
                potentialTextureDefs.push_back(def);
        }

        if (potentialTextureDefs.empty())
            return nullptr;
        if (potentialTextureDefs.size() == 1)
            return GetImageFromTextureDef(*potentialTextureDefs[0]);

        for (const auto* def : potentialTextureDefs)
        {
            if (tolower(def->nameStart) == 'c' && tolower(def->nameEnd) == 'p')
                return GetImageFromTextureDef(*def);
        }

        for (const auto* def : potentialTextureDefs)
        {
            if (tolower(def->nameStart) == 'r' && tolower(def->nameEnd) == 'k')
                return GetImageFromTextureDef(*def);
        }

        for (const auto* def : potentialTextureDefs)
        {
            if (tolower(def->nameStart) == 'd' && tolower(def->nameEnd) == 'p')
                return GetImageFromTextureDef(*def);
        }

        return GetImageFromTextureDef(*potentialTextureDefs[0]);
    }

    GfxImage* GetMaterialNormalMap(const Material* material)
    {
        std::vector<MaterialTextureDef*> potentialTextureDefs;

        for (auto textureIndex = 0u; textureIndex < material->textureCount; textureIndex++)
        {
            MaterialTextureDef* def = &material->textureTable[textureIndex];

            if (def->semantic == TS_NORMAL_MAP)
                potentialTextureDefs.push_back(def);
        }

        if (potentialTextureDefs.empty())
            return nullptr;
        if (potentialTextureDefs.size() == 1)
            return GetImageFromTextureDef(*potentialTextureDefs[0]);

        for (const auto* def : potentialTextureDefs)
        {
            if (def->nameStart == 'n' && def->nameEnd == 'p')
                return GetImageFromTextureDef(*def);
        }

        return GetImageFromTextureDef(*potentialTextureDefs[0]);
    }

    GfxImage* GetMaterialSpecularMap(const Material* material)
    {
        std::vector<MaterialTextureDef*> potentialTextureDefs;

        for (auto textureIndex = 0u; textureIndex < material->textureCount; textureIndex++)
        {
            MaterialTextureDef* def = &material->textureTable[textureIndex];

            if (def->semantic == TS_SPECULAR_MAP)
                potentialTextureDefs.push_back(def);
        }

        if (potentialTextureDefs.empty())
            return nullptr;
        if (potentialTextureDefs.size() == 1)
            return GetImageFromTextureDef(*potentialTextureDefs[0]);

        for (const auto* def : potentialTextureDefs)
        {
            if (def->nameStart == 's' && def->nameEnd == 'p')
                return GetImageFromTextureDef(*def);
        }

        return GetImageFromTextureDef(*potentialTextureDefs[0]);
    }

    bool GetSurfaces(const XModel& model, const unsigned lod, XSurface*& surfs, unsigned& surfCount)
    {
        if (!model.surfs)
            return false;

        surfs = &model.surfs[model.lodInfo[lod].surfIndex];
        surfCount = model.lodInfo[lod].numsurfs;

        return true;
    }

    bool HasDefaultArmatureForLod(const XModel& model, const unsigned lod)
    {
        if (model.numRootBones != 1 || model.numBones != 1)
            return false;

        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return true;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            const auto& surface = surfs[surfIndex];

            if (surface.vertListCount != 1 || surface.vertInfo.vertsBlend)
                return false;

            const auto& vertList = surface.vertList[0];
            if (vertList.boneOffset != 0 || vertList.triOffset != 0 || vertList.vertCount != surface.vertCount)
                return false;
        }

        return true;
    }

    bool HasDefaultArmatureForAllLods(const XModel& model)
    {
        for (auto lod = 0u; lod < model.numLods; lod++)
        {
            if (!HasDefaultArmatureForLod(model, lod))
                return false;
        }

        return true;
    }

    void OmitDefaultArmature(XModelCommon& common)
    {
        common.m_bones.clear();
        common.m_bone_weight_data.weights.clear();
        common.m_vertex_bone_weights.resize(common.m_vertices.size());
        for (auto& vertexWeights : common.m_vertex_bone_weights)
        {
            vertexWeights.weightOffset = 0u;
            vertexWeights.weightCount = 0u;
        }
    }

    void AddXModelBones(XModelCommon& out, const ZoneScriptStrings& scriptStrings, const XModel& model)
    {
        for (auto boneNum = 0u; boneNum < model.numBones; boneNum++)
        {
            XModelBone bone;
            if (model.boneNames[boneNum] < scriptStrings.Count())
                bone.name = scriptStrings[model.boneNames[boneNum]];
            else
                bone.name = "INVALID_BONE_NAME";

            if (boneNum >= model.numRootBones)
            {
                const auto parentIndex = static_cast<int>(boneNum) - static_cast<int>(model.parentList[boneNum - model.numRootBones]);
                if (parentIndex >= 0 && std::cmp_less(parentIndex, model.numBones))
                {
                    bone.parentIndex = static_cast<unsigned>(parentIndex);
                }
                else
                {
                    // Treat invalid parents as roots.
                    bone.parentIndex = std::nullopt;
                }
            }
            else
                bone.parentIndex = std::nullopt;

            bone.scale[0] = 1.0f;
            bone.scale[1] = 1.0f;
            bone.scale[2] = 1.0f;

            const auto& baseMat = model.baseMat[boneNum];
            bone.globalOffset[0] = baseMat.trans[0];
            bone.globalOffset[1] = baseMat.trans[1];
            bone.globalOffset[2] = baseMat.trans[2];
            bone.globalRotation = {
                .x = baseMat.quat[0],
                .y = baseMat.quat[1],
                .z = baseMat.quat[2],
                .w = baseMat.quat[3],
            };

            if (boneNum < model.numRootBones)
            {
                bone.localOffset[0] = 0;
                bone.localOffset[1] = 0;
                bone.localOffset[2] = 0;
                bone.localRotation = {.x = 0, .y = 0, .z = 0, .w = 1};
            }
            else
            {
                const auto* trans = &model.trans[(boneNum - model.numRootBones) * 3];
                bone.localOffset[0] = trans[0];
                bone.localOffset[1] = trans[1];
                bone.localOffset[2] = trans[2];

                const auto* quat = &model.quats[(boneNum - model.numRootBones) * 4u];
                bone.localRotation = {
                    .x = QuatInt16::ToFloat(quat[0]),
                    .y = QuatInt16::ToFloat(quat[1]),
                    .z = QuatInt16::ToFloat(quat[2]),
                    .w = QuatInt16::ToFloat(quat[3]),
                };
            }

            out.m_bones.emplace_back(std::move(bone));
        }
    }

    const char* AssetName(const char* input)
    {
        if (input && input[0] == ',')
            return &input[1];

        return input;
    }

    void AddXModelMaterials(XModelCommon& out, DistinctMapper<Material*>& materialMapper, const XModel& model)
    {
        for (auto surfaceMaterialNum = 0u; surfaceMaterialNum < model.numsurfs; surfaceMaterialNum++)
        {
            Material* material = model.materialHandles[surfaceMaterialNum];
            if (materialMapper.Add(material))
            {
                XModelMaterial xMaterial;
                xMaterial.ApplyDefaults();

                xMaterial.name = AssetName(material->info.name);
                const auto* colorMap = GetMaterialColorMap(material);
                if (colorMap)
                    xMaterial.colorMapName = AssetName(colorMap->name);
                const auto* normalMap = GetMaterialNormalMap(material);
                if (normalMap)
                    xMaterial.normalMapName = AssetName(normalMap->name);

                const auto* specularMap = GetMaterialSpecularMap(material);
                if (specularMap)
                    xMaterial.specularMapName = AssetName(specularMap->name);

                out.m_materials.emplace_back(std::move(xMaterial));
            }
        }
    }

    void AddXModelObjects(XModelCommon& out, const XModel& model, const unsigned lod, const DistinctMapper<Material*>& materialMapper)
    {
        const auto surfCount = model.lodInfo[lod].numsurfs;
        const auto baseSurfaceIndex = model.lodInfo[lod].surfIndex;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            XModelObject object;
            object.name = std::format("surf{}", surfIndex);
            object.materialIndex = static_cast<int>(materialMapper.GetDistinctPositionByInputPosition(surfIndex + baseSurfaceIndex));

            out.m_objects.emplace_back(std::move(object));
        }
    }

    void AddXModelVertices(XModelCommon& out, const XModel& model, const unsigned lod)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            const auto& surface = surfs[surfIndex];

            for (auto vertexIndex = 0u; vertexIndex < surface.vertCount; vertexIndex++)
            {
                const auto& v = surface.verts[vertexIndex];

                XModelVertex vertex{};
                vertex.coordinates[0] = v.xyz[0];
                vertex.coordinates[1] = v.xyz[1];
                vertex.coordinates[2] = v.xyz[2];
                pack32::Vec3UnpackUnitVecThirdBased(v.normal.packed, vertex.normal);
                pack32::Vec4UnpackGfxColor(v.color.packed, vertex.color);
                pack32::Vec2UnpackTexCoordsVU(v.texCoord.packed, vertex.uv);

                out.m_vertices.emplace_back(vertex);
            }
        }
    }

    void AllocateXModelBoneWeights(const XModel& model, const unsigned lod, XModelVertexBoneWeightCollection& weightCollection)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        auto totalWeightCount = 0u;
        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            const auto& surface = surfs[surfIndex];

            if (surface.vertList)
            {
                totalWeightCount += surface.vertListCount;
            }

            if (surface.vertInfo.vertsBlend)
            {
                totalWeightCount += surface.vertInfo.vertCount[0] * 1;
                totalWeightCount += surface.vertInfo.vertCount[1] * 2;
                totalWeightCount += surface.vertInfo.vertCount[2] * 3;
                totalWeightCount += surface.vertInfo.vertCount[3] * 4;
            }
        }

        weightCollection.weights.resize(totalWeightCount);
    }

    float BoneWeight16(const uint16_t value)
    {
        return static_cast<float>(value) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    }

    void AddXModelVertexBoneWeights(XModelCommon& out, const XModel& model, const unsigned lod)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        auto& weightCollection = out.m_bone_weight_data;
        auto weightOffset = 0u;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            const auto& surface = surfs[surfIndex];
            auto handledVertices = 0u;

            if (surface.vertList)
            {
                assert(!surface.deformed);

                for (auto vertListIndex = 0u; vertListIndex < surface.vertListCount; vertListIndex++)
                {
                    const auto& vertList = surface.vertList[vertListIndex];
                    const auto boneWeightOffset = weightOffset;

                    weightCollection.weights[weightOffset++] =
                        XModelBoneWeight{.boneIndex = static_cast<unsigned>(vertList.boneOffset / BONE_MATRIX_SIZE), .weight = 1.0f};

                    for (auto vertListVertexOffset = 0u; vertListVertexOffset < vertList.vertCount; vertListVertexOffset++)
                    {
                        out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 1);
                    }
                    handledVertices += vertList.vertCount;
                }
            }

            auto vertsBlendOffset = 0u;
            if (surface.vertInfo.vertsBlend)
            {
                assert(surface.deformed);

                // 1 bone weight
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[0]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = surface.vertInfo.vertsBlend[vertsBlendOffset + 0] / BONE_MATRIX_SIZE;
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex0, .weight = 1.0f};

                    vertsBlendOffset += 1;

                    out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 1);
                }

                // 2 bone weights
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[1]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = surface.vertInfo.vertsBlend[vertsBlendOffset + 0] / BONE_MATRIX_SIZE;
                    const unsigned boneIndex1 = surface.vertInfo.vertsBlend[vertsBlendOffset + 1] / BONE_MATRIX_SIZE;
                    const auto boneWeight1 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]);
                    const auto boneWeight0 = 1.0f - boneWeight1;

                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex0, .weight = boneWeight0};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex1, .weight = boneWeight1};

                    vertsBlendOffset += 3;

                    out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 2);
                }

                // 3 bone weights
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[2]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = surface.vertInfo.vertsBlend[vertsBlendOffset + 0] / BONE_MATRIX_SIZE;
                    const unsigned boneIndex1 = surface.vertInfo.vertsBlend[vertsBlendOffset + 1] / BONE_MATRIX_SIZE;
                    const auto boneWeight1 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]);
                    const unsigned boneIndex2 = surface.vertInfo.vertsBlend[vertsBlendOffset + 3] / BONE_MATRIX_SIZE;
                    const auto boneWeight2 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 4]);
                    const auto boneWeight0 = 1.0f - boneWeight1 - boneWeight2;

                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex0, .weight = boneWeight0};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex1, .weight = boneWeight1};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex2, .weight = boneWeight2};

                    vertsBlendOffset += 5;

                    out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 3);
                }

                // 4 bone weights
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[3]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = surface.vertInfo.vertsBlend[vertsBlendOffset + 0] / BONE_MATRIX_SIZE;
                    const unsigned boneIndex1 = surface.vertInfo.vertsBlend[vertsBlendOffset + 1] / BONE_MATRIX_SIZE;
                    const auto boneWeight1 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]);
                    const unsigned boneIndex2 = surface.vertInfo.vertsBlend[vertsBlendOffset + 3] / BONE_MATRIX_SIZE;
                    const auto boneWeight2 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 4]);
                    const unsigned boneIndex3 = surface.vertInfo.vertsBlend[vertsBlendOffset + 5] / BONE_MATRIX_SIZE;
                    const auto boneWeight3 = BoneWeight16(surface.vertInfo.vertsBlend[vertsBlendOffset + 6]);
                    const auto boneWeight0 = 1.0f - boneWeight1 - boneWeight2 - boneWeight3;

                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex0, .weight = boneWeight0};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex1, .weight = boneWeight1};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex2, .weight = boneWeight2};
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex3, .weight = boneWeight3};

                    vertsBlendOffset += 7;

                    out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 4);
                }

                handledVertices +=
                    surface.vertInfo.vertCount[0] + surface.vertInfo.vertCount[1] + surface.vertInfo.vertCount[2] + surface.vertInfo.vertCount[3];
            }

            for (; handledVertices < surface.vertCount; handledVertices++)
            {
                out.m_vertex_bone_weights.emplace_back(0, 0);
            }
        }
    }

    void AddXModelFaces(XModelCommon& out, const XModel& model, const unsigned lod)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            const auto& surface = surfs[surfIndex];
            auto& object = out.m_objects[surfIndex];
            object.m_faces.reserve(surface.triCount);

            // Triangle indices are local to each surface. Accumulate their offsets into
            // the flattened common vertex buffer.
            auto surfaceVertexOffset = 0u;
            for (auto previousSurfIndex = 0u; previousSurfIndex < surfIndex; previousSurfIndex++)
                surfaceVertexOffset += surfs[previousSurfIndex].vertCount;

            for (auto triIndex = 0u; triIndex < surface.triCount; triIndex++)
            {
                const auto& tri = GetTriangles(surface)[triIndex];

                XModelFace face{};
                face.vertexIndex[0] = tri.i[0] + surfaceVertexOffset;
                face.vertexIndex[1] = tri.i[1] + surfaceVertexOffset;
                face.vertexIndex[2] = tri.i[2] + surfaceVertexOffset;
                object.m_faces.emplace_back(face);
            }
        }
    }

    bool CanOmitDefaultArmature()
    {
        return ObjWriting::Configuration.ModelOutputFormat != ModelOutputFormat_e::XMODEL_EXPORT
               && ObjWriting::Configuration.ModelOutputFormat != ModelOutputFormat_e::XMODEL_BIN;
    }
} // namespace

namespace xmodel
{
    std::optional<XModelCommon> ToCommonConverterIW3Xenon253::Convert(const XAssetInfoGeneric& assetInfo, const unsigned lod)
    {
        const auto& model = *reinterpret_cast<const XAssetInfo<XModel>*>(&assetInfo)->Asset();

        if (lod >= model.numLods)
            return std::nullopt;

        if (model.numLods > std::extent_v<decltype(model.lodInfo)> || (model.numsurfs && !model.materialHandles)
            || (model.numBones && (!model.boneNames || !model.baseMat)) || model.numRootBones > model.numBones
            || (model.numBones > model.numRootBones && (!model.parentList || !model.quats || !model.trans)))
        {
            con::error("Cannot dump incomplete xmodel \"{}\"", assetInfo.m_name);
            return std::nullopt;
        }
        for (auto surface = 0u; surface < model.numsurfs; ++surface)
            if (!model.materialHandles[surface])
            {
                con::error("Cannot dump xmodel \"{}\": missing material {}", assetInfo.m_name, surface);
                return std::nullopt;
            }
        XSurface* surfaces = nullptr;
        unsigned surfaceCount = 0;
        if ((model.lodInfo[lod].numsurfs && !GetSurfaces(model, lod, surfaces, surfaceCount)) || surfaceCount != model.lodInfo[lod].numsurfs
            || model.lodInfo[lod].surfIndex + surfaceCount > model.numsurfs)
        {
            con::error("Cannot dump xmodel \"{}\": missing surfaces for LOD {}", assetInfo.m_name, lod);
            return std::nullopt;
        }
        for (auto surface = 0u; surface < surfaceCount; ++surface)
            if ((surfaces[surface].vertCount && !surfaces[surface].verts) || (surfaces[surface].triCount && !surfaces[surface].triIndices)
                || (surfaces[surface].vertListCount && !surfaces[surface].vertList))
            {
                con::error("Cannot dump xmodel \"{}\": incomplete surface {} in LOD {}", assetInfo.m_name, surface, lod);
                return std::nullopt;
            }

        XModelCommon result;
        DistinctMapper<Material*> materialMapper(model.numsurfs);
        AllocateXModelBoneWeights(model, lod, result.m_bone_weight_data);

        result.m_name = std::format("{}_lod{}", model.name, lod);
        AddXModelMaterials(result, materialMapper, model);
        AddXModelObjects(result, model, lod, materialMapper);
        AddXModelVertices(result, model, lod);
        AddXModelFaces(result, model, lod);

        // All LODs must export the same armature for reloading.
        if (!CanOmitDefaultArmature() || !HasDefaultArmatureForAllLods(model))
        {
            AddXModelBones(result, assetInfo.m_zone->m_script_strings, model);
            AddXModelVertexBoneWeights(result, model, lod);
        }
        else
        {
            OmitDefaultArmature(result);
        }

        return result;
    }
} // namespace xmodel
