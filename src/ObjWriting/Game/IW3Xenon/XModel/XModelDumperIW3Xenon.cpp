// Based on XModelDumper.cpp.template (IW3 variant).
// Modified for IW3Xenon struct differences.

#include "XModelDumperIW3Xenon.h"

// #include "Game/IW3Xenon/XModel/JsonXModelIW3Xenon.h"

#include "ObjWriting.h"
#include "Utils/DistinctMapper.h"
#include "Utils/Pack.h"
#include "Utils/QuatInt16.h"
#include "XModel/Export/XModelBinWriter.h"
#include "XModel/Export/XModelExportWriter.h"
#include "XModel/Gltf/GltfBinOutput.h"
#include "XModel/Gltf/GltfTextOutput.h"
#include "XModel/Gltf/GltfWriter.h"
#include "XModel/Obj/ObjWriter.h"
#include "XModel/XModelWriter.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <format>
#include <iostream>

using namespace IW3Xenon;

namespace
{
    // DObjSkelMat size: 3*4 floats (axis) + 4 floats (origin) = 64 bytes
    // Used to convert bone offsets to bone indices
    constexpr size_t DOBJSKELMAT_SIZE = 64;

    // ============================================
    // Temporary endian swap helpers
    // These should eventually be moved to the loader
    // ============================================
    inline uint16_t SwapU16(uint16_t v)
    {
        return (v >> 8) | (v << 8);
    }

    inline int16_t SwapI16(int16_t v)
    {
        return static_cast<int16_t>(SwapU16(static_cast<uint16_t>(v)));
    }

    inline uint32_t SwapU32(uint32_t v)
    {
        return ((v >> 24) & 0x000000FF) | ((v >> 8) & 0x0000FF00) | ((v << 8) & 0x00FF0000) | ((v << 24) & 0xFF000000);
    }

    inline float SwapFloat(float v)
    {
        uint32_t u;
        std::memcpy(&u, &v, sizeof(u));
        u = SwapU32(u);
        std::memcpy(&v, &u, sizeof(v));
        return v;
    }

    static inline float Len3(const float v[3])
    {
        return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }

    static inline void Normalize3(float (&n)[3])
    {
        const float len = Len3(n);
        if (len > 1e-20f)
        {
            const float inv = 1.0f / len;
            n[0] *= inv;
            n[1] *= inv;
            n[2] *= inv;
        }
        else
        {
            n[0] = 0.0f;
            n[1] = 0.0f;
            n[2] = 1.0f;
        }
    }

    std::string GetFileNameForLod(const std::string& modelName, const unsigned lod, const std::string& extension)
    {
        return std::format("model_export/{}_lod{}{}", modelName, lod, extension);
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

    bool GetSurfaces(const XModel* model, const unsigned lod, XSurface*& surfs, unsigned& surfCount)
    {
        if (!model->surfs)
            return false;

        surfs = &model->surfs[model->lodInfo[lod].surfIndex];
        surfCount = model->lodInfo[lod].numsurfs;

        return true;
    }

    bool HasDefaultArmature(const XModel* model, const unsigned lod)
    {
        if (model->numRootBones != 1 || model->numBones != 1)
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
            if (vertList.boneOffset != 0 || vertList.triOffset != 0 || vertList.triCount != surface.triCount || vertList.vertCount != surface.vertCount)
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

    void AddXModelBones(XModelCommon& out, const AssetDumpingContext& context, const XModel* model)
    {
        for (auto boneNum = 0u; boneNum < model->numBones; boneNum++)
        {
            XModelBone bone;
            // Endian swap boneNames (uint16)
            const auto boneNameIndex = SwapU16(model->boneNames[boneNum]);
            if (boneNameIndex < context.m_zone.m_script_strings.Count())
                bone.name = context.m_zone.m_script_strings[boneNameIndex];
            else
                bone.name = "INVALID_BONE_NAME";

            if (boneNum >= model->numRootBones)
                bone.parentIndex = static_cast<int>(boneNum - static_cast<unsigned int>(model->parentList[boneNum - model->numRootBones]));
            else
                bone.parentIndex = std::nullopt;

            bone.scale[0] = 1.0f;
            bone.scale[1] = 1.0f;
            bone.scale[2] = 1.0f;

            const auto& baseMat = model->baseMat[boneNum];
            // IW3Xenon DObjAnimMat uses float arrays - endian swap needed
            bone.globalOffset[0] = SwapFloat(baseMat.trans[0]);
            bone.globalOffset[1] = SwapFloat(baseMat.trans[1]);
            bone.globalOffset[2] = SwapFloat(baseMat.trans[2]);
            bone.globalRotation = {
                .x = SwapFloat(baseMat.quat[0]),
                .y = SwapFloat(baseMat.quat[1]),
                .z = SwapFloat(baseMat.quat[2]),
                .w = SwapFloat(baseMat.quat[3]),
            };

            if (boneNum < model->numRootBones)
            {
                bone.localOffset[0] = 0;
                bone.localOffset[1] = 0;
                bone.localOffset[2] = 0;
                bone.localRotation = {.x = 0, .y = 0, .z = 0, .w = 1};
            }
            else
            {
                const auto* trans = &model->trans[(boneNum - model->numRootBones) * 3];
                bone.localOffset[0] = SwapFloat(trans[0]);
                bone.localOffset[1] = SwapFloat(trans[1]);
                bone.localOffset[2] = SwapFloat(trans[2]);

                // IW3Xenon uses raw int16* quats instead of XModelQuat struct with v[4]
                // Endian swap each int16 before converting to float
                const auto* quat = &model->quats[(boneNum - model->numRootBones) * 4];
                bone.localRotation = {
                    .x = QuatInt16::ToFloat(SwapI16(quat[0])),
                    .y = QuatInt16::ToFloat(SwapI16(quat[1])),
                    .z = QuatInt16::ToFloat(SwapI16(quat[2])),
                    .w = QuatInt16::ToFloat(SwapI16(quat[3])),
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

    void AddXModelMaterials(XModelCommon& out, DistinctMapper<Material*>& materialMapper, const XModel* model)
    {
        for (auto surfaceMaterialNum = 0u; surfaceMaterialNum < model->numsurfs; surfaceMaterialNum++)
        {
            Material* material = model->materialHandles[surfaceMaterialNum];
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

    void AddXModelObjects(XModelCommon& out, const XModel* model, const unsigned lod, const DistinctMapper<Material*>& materialMapper)
    {
        const auto surfCount = model->lodInfo[lod].numsurfs;
        const auto baseSurfaceIndex = model->lodInfo[lod].surfIndex;

        for (auto surfIndex = 0u; surfIndex < surfCount; surfIndex++)
        {
            XModelObject object;
            object.name = std::format("surf{}", surfIndex);
            object.materialIndex = static_cast<int>(materialMapper.GetDistinctPositionByInputPosition(surfIndex + baseSurfaceIndex));

            out.m_objects.emplace_back(std::move(object));
        }
    }

    static inline int32_t SignExtend10(uint32_t v10)
    {
        // 10-bit signed two's complement: [-512, 511]
        return (v10 & 0x200u) ? static_cast<int32_t>(v10 | ~0x3FFu) : static_cast<int32_t>(v10);
    }

    static inline void DecodeNormal_SNorm101010(uint32_t packed, float (&out)[3])
    {
        // x: bits 0..9, y: 10..19, z: 20..29
        const int32_t xi = SignExtend10((packed >> 0) & 0x3FFu);
        const int32_t yi = SignExtend10((packed >> 10) & 0x3FFu);
        const int32_t zi = SignExtend10((packed >> 20) & 0x3FFu);

        // Map to [-1, 1]. Use 511 so +511 hits +1.0 exactly.
        constexpr float denom = 511.0f;
        out[0] = static_cast<float>(xi) / denom;
        out[1] = static_cast<float>(yi) / denom;
        out[2] = static_cast<float>(zi) / denom;
    }

    void AddXModelVertices(XModelCommon& out, const XModel* model, unsigned lod, std::vector<uint32_t>& baseVert)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        baseVert.resize(surfCount);

        for (unsigned surfIndex = 0; surfIndex < surfCount; ++surfIndex)
        {
            const auto& surface = surfs[surfIndex];

            baseVert[surfIndex] = static_cast<uint32_t>(out.m_vertices.size());

            for (unsigned vertexIndex = 0; vertexIndex < surface.vertCount; ++vertexIndex)
            {
                const auto& v = surface.verts0[vertexIndex];

                XModelVertex vertex{};
                vertex.coordinates[0] = SwapFloat(v.xyz[0]);
                vertex.coordinates[1] = SwapFloat(v.xyz[1]);
                vertex.coordinates[2] = SwapFloat(v.xyz[2]);

                // Xenon IW3: PackedUnitVec is SNORM 10:10:10 in low 30 bits.
                const uint32_t nPacked = SwapU32(v.normal.packed);
                DecodeNormal_SNorm101010(nPacked, vertex.normal);
                Normalize3(vertex.normal);

                pack32::Vec4UnpackGfxColor(SwapU32(v.color.packed), vertex.color);
                pack32::Vec2UnpackTexCoordsVU(SwapU32(v.texCoord.packed), vertex.uv);

                out.m_vertices.emplace_back(vertex);
            }
        }
    }

    void AllocateXModelBoneWeights(const XModel* model, const unsigned lod, XModelVertexBoneWeightCollection& weightCollection)
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

    void AddXModelVertexBoneWeights(XModelCommon& out, const XModel* model, const unsigned lod)
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
                for (auto vertListIndex = 0u; vertListIndex < surface.vertListCount; vertListIndex++)
                {
                    const auto& vertList = surface.vertList[vertListIndex];
                    const auto boneWeightOffset = weightOffset;

                    // boneOffset is already swapped by EndianFixup_XRigidVertList
                    weightCollection.weights[weightOffset++] =
                        XModelBoneWeight{.boneIndex = static_cast<unsigned>(vertList.boneOffset / DOBJSKELMAT_SIZE), .weight = 1.0f};

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
                // 1 bone weight - endian swap vertsBlend uint16 values
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[0]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 0]) / DOBJSKELMAT_SIZE;
                    weightCollection.weights[weightOffset++] = XModelBoneWeight{.boneIndex = boneIndex0, .weight = 1.0f};

                    vertsBlendOffset += 1;

                    out.m_vertex_bone_weights.emplace_back(boneWeightOffset, 1);
                }

                // 2 bone weights
                for (auto vertIndex = 0; vertIndex < surface.vertInfo.vertCount[1]; vertIndex++)
                {
                    const auto boneWeightOffset = weightOffset;
                    const unsigned boneIndex0 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 0]) / DOBJSKELMAT_SIZE;
                    const unsigned boneIndex1 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 1]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight1 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]));
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
                    const unsigned boneIndex0 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 0]) / DOBJSKELMAT_SIZE;
                    const unsigned boneIndex1 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 1]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight1 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]));
                    const unsigned boneIndex2 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 3]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight2 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 4]));
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
                    const unsigned boneIndex0 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 0]) / DOBJSKELMAT_SIZE;
                    const unsigned boneIndex1 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 1]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight1 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 2]));
                    const unsigned boneIndex2 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 3]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight2 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 4]));
                    const unsigned boneIndex3 = SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 5]) / DOBJSKELMAT_SIZE;
                    const auto boneWeight3 = BoneWeight16(SwapU16(surface.vertInfo.vertsBlend[vertsBlendOffset + 6]));
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

    void AddXModelFaces(XModelCommon& out, const XModel* model, unsigned lod, const std::vector<uint32_t>& baseVert)
    {
        XSurface* surfs;
        unsigned surfCount;
        if (!GetSurfaces(model, lod, surfs, surfCount))
            return;

        for (unsigned surfIndex = 0; surfIndex < surfCount; ++surfIndex)
        {
            const auto& surface = surfs[surfIndex];
            auto& object = out.m_objects[surfIndex];
            object.m_faces.reserve(surface.triCount);

            const uint32_t base = baseVert[surfIndex];

            for (unsigned triIndex = 0; triIndex < surface.triCount; ++triIndex)
            {
                const uint16_t i0 = SwapU16(surface.triIndices[triIndex * 3 + 0]);
                const uint16_t i1 = SwapU16(surface.triIndices[triIndex * 3 + 1]);
                const uint16_t i2 = SwapU16(surface.triIndices[triIndex * 3 + 2]);

                XModelFace face{};
                face.vertexIndex[0] = base + i0;
                // Xenon needs to flip i1/i2
                face.vertexIndex[1] = base + i2;
                face.vertexIndex[2] = base + i1;

                object.m_faces.emplace_back(face);
            }
        }
    }

    bool CanOmitDefaultArmature()
    {
        return ObjWriting::Configuration.ModelOutputFormat != ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_EXPORT
               && ObjWriting::Configuration.ModelOutputFormat != ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_BIN;
    }

    void PopulateXModelWriter(XModelCommon& out, const AssetDumpingContext& context, const unsigned lod, const XModel* model)
    {
        DistinctMapper<Material*> materialMapper(model->numsurfs);
        AllocateXModelBoneWeights(model, lod, out.m_bone_weight_data);

        out.m_name = std::format("{}_lod{}", model->name, lod);
        AddXModelMaterials(out, materialMapper, model);
        AddXModelObjects(out, model, lod, materialMapper);
        std::vector<uint32_t> baseVert;
        AddXModelVertices(out, model, lod, baseVert);
        AddXModelFaces(out, model, lod, baseVert);

        if (!CanOmitDefaultArmature() || !HasDefaultArmature(model, lod))
        {
            AddXModelBones(out, context, model);
            AddXModelVertexBoneWeights(out, model, lod);
        }
        else
        {
            OmitDefaultArmature(out);
        }
    }

    void DumpObjMtl(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        const auto* model = asset.Asset();
        const auto mtlFile = context.OpenAssetFile(std::format("model_export/{}.mtl", model->name));

        if (!mtlFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = obj::CreateMtlWriter(*mtlFile, game->GetShortName(), context.m_zone.m_name);
        DistinctMapper<Material*> materialMapper(model->numsurfs);

        writer->Write(common);
    }

    void DumpObjLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto* model = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model->name, lod, ".obj"));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = obj::CreateObjWriter(*assetFile, std::format("{}.mtl", model->name), game->GetShortName(), context.m_zone.m_name);
        DistinctMapper<Material*> materialMapper(model->numsurfs);

        writer->Write(common);
    }

    void DumpXModelExportLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto* model = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model->name, lod, ".xmodel_export"));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = xmodel_export::CreateWriterForVersion6(*assetFile, game->GetShortName(), context.m_zone.m_name);
        writer->Write(common);
    }

    void DumpXModelBinLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto* model = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model->name, lod, ".xmodel_bin"));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = xmodel_bin::CreateWriterForVersion7(*assetFile, game->GetShortName(), context.m_zone.m_name);
        writer->Write(common);
    }

    template<typename T>
    void DumpGltfLod(
        const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod, const std::string& extension)
    {
        const auto* model = asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model->name, lod, extension));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto output = std::make_unique<T>(*assetFile);
        const auto writer = gltf::Writer::CreateWriter(output.get(), game->GetShortName(), context.m_zone.m_name);

        writer->Write(common);
    }

    void DumpXModelSurfs(const AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        const auto* model = asset.Asset();

        for (auto currentLod = 0u; currentLod < model->numLods; currentLod++)
        {
            XModelCommon common;
            PopulateXModelWriter(common, context, currentLod, asset.Asset());

            switch (ObjWriting::Configuration.ModelOutputFormat)
            {
            case ObjWriting::Configuration_t::ModelOutputFormat_e::OBJ:
                DumpObjLod(common, context, asset, currentLod);
                if (currentLod == 0u)
                    DumpObjMtl(common, context, asset);
                break;

            case ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_EXPORT:
                DumpXModelExportLod(common, context, asset, currentLod);
                break;

            case ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_BIN:
                DumpXModelBinLod(common, context, asset, currentLod);
                break;

            case ObjWriting::Configuration_t::ModelOutputFormat_e::GLTF:
                DumpGltfLod<gltf::TextOutput>(common, context, asset, currentLod, ".gltf");
                break;

            case ObjWriting::Configuration_t::ModelOutputFormat_e::GLB:
                DumpGltfLod<gltf::BinOutput>(common, context, asset, currentLod, ".glb");
                break;

            default:
                assert(false);
                break;
            }
        }
    }

    // class JsonDumper
    // {
    // public:
    //     JsonDumper(AssetDumpingContext& context, std::ostream& stream)
    //         : m_stream(stream)
    //     {
    //     }

    //     void Dump(const XModel* xmodel) const
    //     {
    //         JsonXModel jsonXModel;
    //         CreateJsonXModel(jsonXModel, *xmodel);
    //         nlohmann::json jRoot = jsonXModel;

    //         jRoot["$schema"] = "http://openassettools.dev/schema/xmodel.v1.json";
    //         jRoot["_type"] = "xmodel";
    //         jRoot["_version"] = 2;
    //         jRoot["_game"] = "IW3Xenon";

    //         m_stream << std::setw(4) << jRoot << "\n";
    //     }

    // private:
    //     static const char* AssetName(const char* input)
    //     {
    //         if (input && input[0] == ',')
    //             return &input[1];

    //         return input;
    //     }

    //     static const char* GetExtensionForModelByConfig()
    //     {
    //         switch (ObjWriting::Configuration.ModelOutputFormat)
    //         {
    //         case ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_EXPORT:
    //             return ".xmodel_export";
    //         case ObjWriting::Configuration_t::ModelOutputFormat_e::XMODEL_BIN:
    //             return ".xmodel_bin";
    //         case ObjWriting::Configuration_t::ModelOutputFormat_e::OBJ:
    //             return ".obj";
    //         case ObjWriting::Configuration_t::ModelOutputFormat_e::GLTF:
    //             return ".gltf";
    //         case ObjWriting::Configuration_t::ModelOutputFormat_e::GLB:
    //             return ".glb";
    //         default:
    //             assert(false);
    //             return "";
    //         }
    //     }

    //     static bool IsAnimated(const XModel& xmodel)
    //     {
    //         for (auto i = 0u; i < xmodel.numsurfs; i++)
    //         {
    //             const auto& surf = xmodel.surfs[i];
    //             if (surf.vertInfo.vertsBlend)
    //                 return true;
    //         }

    //         return false;
    //     }

    //     static bool HasNulledTrans(const XModel& xmodel)
    //     {
    //         if (xmodel.trans == nullptr)
    //             return true;

    //         const auto transCount = (xmodel.numBones - xmodel.numRootBones) * 3u;
    //         for (auto i = 0u; i < transCount; i++)
    //         {
    //             if (xmodel.trans[i] != 0)
    //                 return false;
    //         }

    //         return true;
    //     }

    //     static bool HasNonNullBoneInfoTrans(const XModel& xmodel)
    //     {
    //         if (xmodel.boneInfo == nullptr)
    //             return false;

    //         for (auto i = 0u; i < xmodel.numBones; i++)
    //         {
    //             const auto& boneInfo = xmodel.boneInfo[i];
    //             // IW3Xenon uses float offset[3] array instead of vec3_t struct
    //             if (boneInfo.offset[0] != 0 || boneInfo.offset[1] != 0 || boneInfo.offset[2] != 0)
    //                 return true;
    //         }

    //         return false;
    //     }

    //     static JsonXModelType GetType(const XModel& xmodel)
    //     {
    //         if (!IsAnimated(xmodel))
    //             return JsonXModelType::RIGID;

    //         if (HasNulledTrans(xmodel) && HasNonNullBoneInfoTrans(xmodel))
    //             return JsonXModelType::VIEWHANDS;

    //         return JsonXModelType::ANIMATED;
    //     }

    //     static void CreateJsonXModel(JsonXModel& jXModel, const XModel& xmodel)
    //     {
    //         if (xmodel.collLod >= 0)
    //             jXModel.collLod = xmodel.collLod;

    //         jXModel.type = GetType(xmodel);

    //         for (auto lodNumber = 0u; lodNumber < xmodel.numLods; lodNumber++)
    //         {
    //             JsonXModelLod lod;
    //             lod.file = std::format("model_export/{}_lod{}{}", xmodel.name, lodNumber, GetExtensionForModelByConfig());
    //             lod.distance = xmodel.lodInfo[lodNumber].dist;

    //             jXModel.lods.emplace_back(std::move(lod));
    //         }

    //         if (xmodel.physPreset && xmodel.physPreset->name)
    //             jXModel.physPreset = AssetName(xmodel.physPreset->name);

    //         jXModel.flags = xmodel.flags;

    //     }

    //     std::ostream& m_stream;
    // };

    // void DumpXModelJson(AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    // {
    //     const auto assetFile = context.OpenAssetFile(xmodel::GetJsonFileNameForAssetName(asset.m_name));
    //     if (!assetFile)
    //         return;

    //     const JsonDumper dumper(context, *assetFile);
    //     dumper.Dump(asset.Asset());
    // }
} // namespace

namespace xmodel
{
    DumperIW3Xenon::DumperIW3Xenon(const AssetPool<IW3Xenon::AssetXModel::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void DumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetXModel::Type>& asset)
    {
        // DumpXModelJson(context, asset);
        DumpXModelSurfs(context, asset);
    }
} // namespace xmodel
