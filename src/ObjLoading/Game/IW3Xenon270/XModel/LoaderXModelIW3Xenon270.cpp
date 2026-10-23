#include "LoaderXModelIW3Xenon270.h"

#include "Asset/AssetRegistration.h"
#include "Game/IW3Xenon270/XModel/JsonXModelIW3Xenon270.h"
#include "Game/IW3Xenon270/XModel/XModelConstantsIW3Xenon270.h"
#include "Utils/Alignment.h"
#include "Utils/Logging/Log.h"
#include "Utils/Pack.h"
#include "Utils/QuatInt16.h"
#include "Utils/StringUtils.h"
#include "XModel/Gltf/GltfBinInput.h"
#include "XModel/Gltf/GltfLoader.h"
#include "XModel/Gltf/GltfTextInput.h"
#include "XModel/XModelCommon.h"

#pragma warning(push, 0)
#include <Eigen>
#include <nlohmann/json.hpp>
#pragma warning(pop)

#include "Game/IW3Xenon270/XModel/XModelHighMipBoundsIW3Xenon270.h"
#include "XModel/PartClassificationState.h"
#include "XModel/TangentData.h"
#include "XModel/Tangentspace.h"

#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

using namespace IW3Xenon270;

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

    class XModelLoader final : public AssetCreator<AssetXModel>
    {
    public:
        XModelLoader(MemoryManager& memory, ISearchPath& searchPath, ZoneScriptStrings& scriptStrings)
            : m_gltf_bad_rotation_formulas(false),
              m_memory(memory),
              m_search_path(searchPath),
              m_script_strings(scriptStrings)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(std::format("xmodel/{}.json", assetName));
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            auto* xmodel = m_memory.Alloc<XModel>();
            xmodel->name = m_memory.Dup(assetName.c_str());

            m_materials.clear();
            m_surfaces.clear();

            AssetRegistration<AssetXModel> registration(assetName, xmodel);
            if (!LoadFromFile(*file.m_stream, *xmodel, context, registration))
            {
                con::error("Failed to load xmodel \"{}\"", assetName);
                return AssetCreationResult::Failure();
            }

            return AssetCreationResult::Success(context.AddAsset(std::move(registration)));
        }

    private:
        bool LoadFromFile(std::istream& jsonStream, XModel& xmodel, AssetCreationContext& context, AssetRegistration<AssetXModel>& registration)
        {
            try
            {
                const auto jRoot = nlohmann::json::parse(jsonStream);
                std::string type;
                unsigned version;

                jRoot.at("_type").get_to(type);
                jRoot.at("_version").get_to(version);

                if (type != "xmodel" || version < 1u || version > 2u)
                {
                    con::error("Tried to load xmodel \"{}\" but did not find expected type material of version 1 or 2", xmodel.name);
                    return false;
                }

                if (version == 1u)
                {
                    m_gltf_bad_rotation_formulas = true;
                    con::warn("DEPRECATED: XModel {} is version 1 that made use of bad GLTF bone rotations.", xmodel.name);
                }
                else
                {
                    m_gltf_bad_rotation_formulas = false;
                }

                const auto jXModel = jRoot.get<JsonXModel>();
                return CreateXModelFromJson(jXModel, xmodel, context, registration);
            }
            catch (const nlohmann::json::exception& e)
            {
                con::error("Failed to parse json of xmodel: {}", e.what());
            }

            return false;
        }

        static void PrintError(const XModel& xmodel, const std::string& message)
        {
            con::error("Cannot load xmodel \"{}\": {}", xmodel.name, message);
        }

        std::unique_ptr<XModelCommon> LoadModelByExtension(std::istream& stream, const std::string& extension) const
        {
            if (extension == ".glb")
            {
                gltf::BinInput input;
                if (!input.ReadGltfData(stream))
                    return nullptr;

                const auto loader = gltf::Loader::CreateLoader(input, m_gltf_bad_rotation_formulas);
                return loader->Load();
            }

            if (extension == ".gltf")
            {
                gltf::TextInput input;
                if (!input.ReadGltfData(stream))
                    return nullptr;

                const auto loader = gltf::Loader::CreateLoader(input, m_gltf_bad_rotation_formulas);
                return loader->Load();
            }

            return nullptr;
        }

        static void AutoGenerateArmature(XModelCommon& common, const JsonXModel& jXModel)
        {
            assert(common.m_bones.empty());
            assert(common.m_bone_weight_data.weights.empty());
            assert(common.m_vertex_bone_weights.size() == common.m_vertices.size());

            XModelBone rootBone{
                .name = jXModel.rootBoneName.value_or(xmodel::DEFAULT_XMODEL_ROOT_BONE_NAME),
                .parentIndex = std::nullopt,
                .scale = {1.0f, 1.0f, 1.0f},
                .globalOffset = {0.0f, 0.0f, 0.0f},
                .localOffset = {0.0f, 0.0f, 0.0f},
                .globalRotation = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                .localRotation = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
            };
            common.m_bones.emplace_back(std::move(rootBone));

            XModelBoneWeight rootWeight{.boneIndex = 0, .weight = 1.0f};
            common.m_bone_weight_data.weights.emplace_back(rootWeight);

            for (auto& vertexBoneWeights : common.m_vertex_bone_weights)
            {
                vertexBoneWeights.weightOffset = 0u;
                vertexBoneWeights.weightCount = 1u;
            }
        }

        static void CheckSpecifiedRootBoneName(XModelCommon& common, const JsonXModel& jXModel)
        {
            assert(!common.m_bones.empty());

            if (!jXModel.rootBoneName)
                return;

            if (*jXModel.rootBoneName != common.m_bones[0].name)
            {
                con::warn(
                    "Root bone name of model json {} does not match the model's root bone name. The name from the xmodel json is ignored and can be removed!",
                    common.m_name);
            }
        }

        static void ApplyBasePose(DObjAnimMat& baseMat, const XModelBone& bone)
        {
            baseMat.trans[0] = bone.globalOffset[0];
            baseMat.trans[1] = bone.globalOffset[1];
            baseMat.trans[2] = bone.globalOffset[2];
            baseMat.quat[0] = bone.globalRotation.x;
            baseMat.quat[1] = bone.globalRotation.y;
            baseMat.quat[2] = bone.globalRotation.z;
            baseMat.quat[3] = bone.globalRotation.w;

            const auto quatNormSquared = Eigen::Quaternionf(baseMat.quat[3], baseMat.quat[0], baseMat.quat[1], baseMat.quat[2]).squaredNorm();
            if (std::abs(quatNormSquared) < std::numeric_limits<float>::epsilon())
            {
                baseMat.quat[3] = 1.0f;
                baseMat.transWeight = 2.0f;
            }
            else
            {
                baseMat.transWeight = 2.0f / quatNormSquared;
            }
        }

        static void CalculateBoneBounds(XBoneInfo& info, const unsigned boneIndex, const XModelCommon& common)
        {
            if (common.m_bone_weight_data.weights.empty())
                return;

            auto& offset = info.offset;
            auto& minCoordinate = info.bounds[0];
            auto& maxCoordinate = info.bounds[1];

            minCoordinate[0] = 0.0f;
            minCoordinate[1] = 0.0f;
            minCoordinate[2] = 0.0f;
            maxCoordinate[0] = 0.0f;
            maxCoordinate[1] = 0.0f;
            maxCoordinate[2] = 0.0f;
            offset[0] = 0.0f;
            offset[1] = 0.0f;
            offset[2] = 0.0f;
            info.radiusSquared = 0.0f;

            const auto vertexCount = common.m_vertex_bone_weights.size();
            for (auto vertexIndex = 0u; vertexIndex < vertexCount; vertexIndex++)
            {
                const auto& vertex = common.m_vertices[vertexIndex];
                const auto& vertexWeights = common.m_vertex_bone_weights[vertexIndex];
                const auto* weights = &common.m_bone_weight_data.weights[vertexWeights.weightOffset];
                for (auto weightIndex = 0u; weightIndex < vertexWeights.weightCount; weightIndex++)
                {
                    const auto& weight = weights[weightIndex];
                    if (weight.boneIndex != boneIndex)
                        continue;

                    minCoordinate[0] = std::min(minCoordinate[0], vertex.coordinates[0]);
                    minCoordinate[1] = std::min(minCoordinate[1], vertex.coordinates[1]);
                    minCoordinate[2] = std::min(minCoordinate[2], vertex.coordinates[2]);
                    maxCoordinate[0] = std::max(maxCoordinate[0], vertex.coordinates[0]);
                    maxCoordinate[1] = std::max(maxCoordinate[1], vertex.coordinates[1]);
                    maxCoordinate[2] = std::max(maxCoordinate[2], vertex.coordinates[2]);
                }
            }

            const Eigen::Vector3f minEigen(minCoordinate[0], minCoordinate[1], minCoordinate[2]);
            const Eigen::Vector3f maxEigen(maxCoordinate[0], maxCoordinate[1], maxCoordinate[2]);
            const Eigen::Vector3f boundsCenter = (minEigen + maxEigen) * 0.5f;
            const Eigen::Vector3f halfSizeEigen = maxEigen - boundsCenter;

            offset[0] = boundsCenter.x();
            offset[1] = boundsCenter.y();
            offset[2] = boundsCenter.z();

            info.radiusSquared = halfSizeEigen.squaredNorm();
        }

        bool ApplyCommonBonesToXModel(const JsonXModel& jXModel,
                                      const JsonXModelLod& jLod,
                                      XModel& xmodel,
                                      unsigned lodNumber,
                                      const XModelCommon& common,
                                      AssetRegistration<AssetXModel>& registration)
        {
            if (common.m_bones.empty())
                return true;

            m_part_classification_state.Load(HITLOC_NAMES, std::extent_v<decltype(HITLOC_NAMES)>, m_search_path);

            const auto boneCount = common.m_bones.size();
            constexpr auto maxBones = std::extent_v<decltype(XSurface::partBits)> * 32u;
            if (boneCount > maxBones)
            {
                PrintError(xmodel, std::format("Model \"{}\" for lod {} contains too many bones ({} -> max={})", jLod.file, lodNumber, boneCount, maxBones));
                return false;
            }

            xmodel.numRootBones = 0u;
            xmodel.numBones = 0u;
            for (const auto& bone : common.m_bones)
            {
                if (!bone.parentIndex)
                {
                    // Make sure root bones are at the beginning
                    assert(xmodel.numRootBones == xmodel.numBones);
                    xmodel.numRootBones++;
                }

                xmodel.numBones++;
            }

            xmodel.boneNames = m_memory.Alloc<ScriptString>(xmodel.numBones);
            xmodel.partClassification = m_memory.Alloc<unsigned char>(xmodel.numBones);
            xmodel.baseMat = m_memory.Alloc<DObjAnimMat>(xmodel.numBones);
            xmodel.boneInfo = m_memory.Alloc<XBoneInfo>(xmodel.numBones);

            if (xmodel.numBones > xmodel.numRootBones)
            {
                xmodel.parentList = m_memory.Alloc<unsigned char>(xmodel.numBones - xmodel.numRootBones);

                // Storage reserves four floats per bone; translations are packed triples.
                xmodel.trans = m_memory.Alloc<float>((xmodel.numBones - xmodel.numRootBones) * 4u);
                xmodel.quats = m_memory.Alloc<int16_t>((xmodel.numBones - xmodel.numRootBones) * 4u);
            }
            else
            {
                xmodel.parentList = nullptr;
                xmodel.trans = nullptr;
                xmodel.quats = nullptr;
            }

            for (auto boneIndex = 0u; boneIndex < boneCount; boneIndex++)
            {
                const auto& bone = common.m_bones[boneIndex];
                xmodel.boneNames[boneIndex] = m_script_strings.AddOrGetScriptString(bone.name);
                registration.AddScriptString(xmodel.boneNames[boneIndex]);
                xmodel.partClassification[boneIndex] = static_cast<unsigned char>(m_part_classification_state.GetPartClassificationForBoneName(bone.name));

                ApplyBasePose(xmodel.baseMat[boneIndex], bone);
                CalculateBoneBounds(xmodel.boneInfo[boneIndex], boneIndex, common);

                if (xmodel.numRootBones <= boneIndex)
                {
                    const auto nonRootIndex = boneIndex - xmodel.numRootBones;
                    const auto parentBoneIndex = static_cast<unsigned char>(bone.parentIndex.value_or(0u));
                    assert(parentBoneIndex < boneIndex);

                    xmodel.parentList[nonRootIndex] = static_cast<unsigned char>(boneIndex - parentBoneIndex);

                    auto* trans = &xmodel.trans[nonRootIndex * 3];
                    trans[0] = bone.localOffset[0];
                    trans[1] = bone.localOffset[1];
                    trans[2] = bone.localOffset[2];

                    auto* quats = &xmodel.quats[nonRootIndex * 4u];
                    quats[0] = QuatInt16::ToInt16(bone.localRotation.x);
                    quats[1] = QuatInt16::ToInt16(bone.localRotation.y);
                    quats[2] = QuatInt16::ToInt16(bone.localRotation.z);
                    quats[3] = QuatInt16::ToInt16(bone.localRotation.w);
                }
            }

            // Viewhands use zeroed local translations.
            if (jXModel.type.value_or(JsonXModelType::RIGID) == JsonXModelType::VIEWHANDS)
            {
                memset(xmodel.trans, 0, sizeof(float) * 4 * (xmodel.numBones - xmodel.numRootBones));
            }

            return true;
        }

        [[nodiscard]] bool VerifyBones(const JsonXModelLod& jLod, const XModel& xmodel, unsigned lodNumber, const XModelCommon& common) const
        {
            // LODs must use the same bone names.

            const auto commonBoneCount = common.m_bones.size();
            if (xmodel.numBones != commonBoneCount)
            {
                PrintError(xmodel,
                           std::format(R"(Model "{}" for lod "{}" has different bone count compared to lod 0 ({} != {}))",
                                       jLod.file,
                                       lodNumber,
                                       xmodel.numBones,
                                       commonBoneCount));
                return false;
            }

            for (auto boneIndex = 0u; boneIndex < commonBoneCount; boneIndex++)
            {
                const auto& commonBone = common.m_bones[boneIndex];

                const auto& boneName = m_script_strings[xmodel.boneNames[boneIndex]];
                if (commonBone.name != boneName)
                {
                    PrintError(xmodel,
                               std::format(R"(Model "{}" for lod "{}" has different bone names compared to lod 0 (Index {}: {} != {}))",
                                           jLod.file,
                                           lodNumber,
                                           boneIndex,
                                           boneName,
                                           commonBone.name));
                    return false;
                }
            }

            return true;
        }

        static void
            CreateVertex(GfxPackedVertex& vertex, const XModelVertex& commonVertex, const std::array<float, 3>& tangent, const std::array<float, 3>& binormal)
        {
            const float tangentPlainArray[]{tangent[0], tangent[1], tangent[2]};

            vertex.xyz[0] = commonVertex.coordinates[0];
            vertex.xyz[1] = commonVertex.coordinates[1];
            vertex.xyz[2] = commonVertex.coordinates[2];
            const Eigen::Vector3f normal(commonVertex.normal[0], commonVertex.normal[1], commonVertex.normal[2]);
            const Eigen::Vector3f tangentVector(tangent[0], tangent[1], tangent[2]);
            const Eigen::Vector3f binormalVector(binormal[0], binormal[1], binormal[2]);
            vertex.binormalSign = normal.cross(tangentVector).dot(binormalVector) < 0.0f ? -1.0f : 1.0f;
            vertex.color = GfxColor{pack32::Vec4PackGfxColor(commonVertex.color)};
            vertex.texCoord = PackedTexCoords{pack32::Vec2PackTexCoordsVU(commonVertex.uv)};
            vertex.normal = PackedUnitVec{pack32::Vec3PackUnitVecThirdBased(commonVertex.normal)};
            vertex.tangent = PackedUnitVec{pack32::Vec3PackUnitVecThirdBased(tangentPlainArray)};
        }

        static size_t GetRigidBoneForVertex(const size_t vertexIndex, const XModelCommon& common)
        {
            return common.m_bone_weight_data.weights[common.m_vertex_bone_weights[vertexIndex].weightOffset].boneIndex;
        }

        static std::vector<std::optional<size_t>>
            GetRigidBoneIndicesForTris(const std::vector<size_t>& vertexIndices, const XSurface& surface, const XModelCommon& common)
        {
            std::vector<std::optional<size_t>> rigidBoneIndexForTri;
            rigidBoneIndexForTri.reserve(surface.triCount);
            for (auto triIndex = 0u; triIndex < surface.triCount; triIndex++)
            {
                const auto& tri = GetTriangles(surface)[triIndex];
                const auto vert0Bone = GetRigidBoneForVertex(vertexIndices[tri.i[0]], common);
                const auto vert1Bone = GetRigidBoneForVertex(vertexIndices[tri.i[1]], common);
                const auto vert2Bone = GetRigidBoneForVertex(vertexIndices[tri.i[2]], common);

                const auto hasSameBone = vert0Bone == vert1Bone && vert1Bone == vert2Bone;
                if (hasSameBone)
                    rigidBoneIndexForTri.emplace_back(vert0Bone);
                else
                    rigidBoneIndexForTri.emplace_back(std::nullopt);
            }

            return rigidBoneIndexForTri;
        }

        static void ReorderRigidTrisByBoneIndex(const std::vector<size_t>& vertexIndices, const XSurface& surface, const XModelCommon& common)
        {
            const auto rigidBoneIndexForTri = GetRigidBoneIndicesForTris(vertexIndices, surface, common);

            std::vector<size_t> triSortList(surface.triCount);
            std::ranges::iota(triSortList, 0);
            std::ranges::sort(triSortList,
                              [&rigidBoneIndexForTri](const size_t triIndex0, const size_t triIndex1)
                              {
                                  const auto rigidBone0 = rigidBoneIndexForTri[triIndex0];
                                  const auto rigidBone1 = rigidBoneIndexForTri[triIndex1];

                                  if (rigidBone0.has_value() != rigidBone1.has_value())
                                      return rigidBone0.has_value();
                                  if (!rigidBone0.has_value())
                                      return true;

                                  return *rigidBone0 < *rigidBone1;
                              });

            std::vector<XSurfaceTri> sortedTris(surface.triCount);
            for (auto i = 0u; i < surface.triCount; i++)
                memcpy(&sortedTris[i], &GetTriangles(surface)[triSortList[i]], sizeof(XSurfaceTri));
            memcpy(surface.triIndices, sortedTris.data(), sizeof(XSurfaceTri) * surface.triCount);
        }

        static void AddBoneToXSurfacePartBits(XSurface& surface, const size_t boneIndex)
        {
            const auto partBitsIndex = boneIndex / 32u;
            const auto shiftValue = 31u - (boneIndex % 32u);
            surface.partBits[partBitsIndex] |= 1 << shiftValue;
        }

        void CreateVertListData(XSurface& surface, const std::vector<size_t>& xmodelToCommonVertexIndexLookup, const XModelCommon& common) const
        {
            ReorderRigidTrisByBoneIndex(xmodelToCommonVertexIndexLookup, surface, common);
            const auto rigidBoneIndexForTri = GetRigidBoneIndicesForTris(xmodelToCommonVertexIndexLookup, surface, common);

            std::vector<XRigidVertList> vertLists;

            auto currentVertexTail = 0u;
            auto currentTriTail = 0u;

            const auto vertexCount = xmodelToCommonVertexIndexLookup.size();
            const auto triCount = static_cast<size_t>(surface.triCount);
            const auto boneCount = common.m_bones.size();
            for (auto boneIndex = 0u; boneIndex < boneCount; boneIndex++)
            {
                XRigidVertList boneVertList{};
                boneVertList.boneOffset = static_cast<uint16_t>(boneIndex * BONE_MATRIX_SIZE);

                auto currentVertexHead = currentVertexTail;
                while (currentVertexHead < vertexCount && GetRigidBoneForVertex(xmodelToCommonVertexIndexLookup[currentVertexHead], common) == boneIndex)
                    currentVertexHead++;

                auto currentTriHead = currentTriTail;
                while (currentTriHead < triCount && rigidBoneIndexForTri[currentTriHead] && *rigidBoneIndexForTri[currentTriHead] == boneIndex)
                    currentTriHead++;

                boneVertList.vertCount = static_cast<uint16_t>(currentVertexHead - currentVertexTail);
                boneVertList.triOffset = static_cast<uint16_t>(currentTriTail);
                boneVertList.triCount = static_cast<uint16_t>(currentTriHead - currentTriTail);

                if (boneVertList.triCount > 0 || boneVertList.vertCount > 0)
                {

                    vertLists.emplace_back(boneVertList);

                    currentVertexTail = currentVertexHead;
                    currentTriTail = currentTriHead;

                    AddBoneToXSurfacePartBits(surface, boneIndex);
                }
            }

            if (!vertLists.empty())
            {
                surface.vertListCount = static_cast<unsigned char>(vertLists.size());
                surface.vertList = m_memory.Alloc<XRigidVertList>(surface.vertListCount);

                memcpy(surface.vertList, vertLists.data(), sizeof(XRigidVertList) * surface.vertListCount);
            }
        }

        static uint16_t BoneWeight16(const float value)
        {
            return static_cast<uint16_t>(value * static_cast<float>(std::numeric_limits<uint16_t>::max()));
        }

        void CreateVertsBlendData(XSurface& surface, const std::vector<size_t>& vertexIndices, const XModelCommon& common) const
        {
            std::vector<uint16_t> vertsBlendData;
            const auto vertexCount = vertexIndices.size();
            auto vertexIndex = 0uz;

            // Reserve the minimum amount of data we know will follow
            vertsBlendData.reserve(vertexCount);
            while (vertexIndex < vertexCount)
            {
                const auto& boneWeights = common.m_vertex_bone_weights[vertexIndices[vertexIndex]];
                if (boneWeights.weightCount > 1)
                    break;

                const auto& weight0 = common.m_bone_weight_data.weights[boneWeights.weightOffset];

                vertsBlendData.emplace_back(static_cast<uint16_t>(weight0.boneIndex * BONE_MATRIX_SIZE));

                vertexIndex++;
                surface.vertInfo.vertCount[0]++;

                AddBoneToXSurfacePartBits(surface, weight0.boneIndex);
            }

            vertsBlendData.reserve(vertsBlendData.size() + (vertexCount - vertexIndex) * 3u);
            while (vertexIndex < vertexCount)
            {
                const auto& boneWeights = common.m_vertex_bone_weights[vertexIndices[vertexIndex]];
                if (boneWeights.weightCount > 2)
                    break;

                const auto& weight0 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 0];
                const auto& weight1 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 1];

                vertsBlendData.emplace_back(static_cast<uint16_t>(weight0.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight1.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight1.weight));

                vertexIndex++;
                surface.vertInfo.vertCount[1]++;

                AddBoneToXSurfacePartBits(surface, weight0.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight1.boneIndex);
            }

            vertsBlendData.reserve(vertsBlendData.size() + (vertexCount - vertexIndex) * 5u);
            while (vertexIndex < vertexCount)
            {
                const auto& boneWeights = common.m_vertex_bone_weights[vertexIndices[vertexIndex]];
                if (boneWeights.weightCount > 3)
                    break;

                const auto& weight0 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 0];
                const auto& weight1 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 1];
                const auto& weight2 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 2];

                vertsBlendData.emplace_back(static_cast<uint16_t>(weight0.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight1.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight1.weight));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight2.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight2.weight));

                vertexIndex++;
                surface.vertInfo.vertCount[2]++;

                AddBoneToXSurfacePartBits(surface, weight0.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight1.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight2.boneIndex);
            }

            vertsBlendData.reserve(vertsBlendData.size() + (vertexCount - vertexIndex) * 7u);
            while (vertexIndex < vertexCount)
            {
                const auto& boneWeights = common.m_vertex_bone_weights[vertexIndices[vertexIndex]];

                const auto& weight0 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 0];
                const auto& weight1 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 1];
                const auto& weight2 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 2];
                const auto& weight3 = common.m_bone_weight_data.weights[boneWeights.weightOffset + 3];

                vertsBlendData.emplace_back(static_cast<uint16_t>(weight0.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight1.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight1.weight));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight2.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight2.weight));
                vertsBlendData.emplace_back(static_cast<uint16_t>(weight3.boneIndex * BONE_MATRIX_SIZE));
                vertsBlendData.emplace_back(BoneWeight16(weight3.weight));

                vertexIndex++;
                surface.vertInfo.vertCount[3]++;

                AddBoneToXSurfacePartBits(surface, weight0.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight1.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight2.boneIndex);
                AddBoneToXSurfacePartBits(surface, weight3.boneIndex);
            }

            surface.vertInfo.vertsBlend = m_memory.Alloc<uint16_t>(vertsBlendData.size());
            std::memcpy(surface.vertInfo.vertsBlend, vertsBlendData.data(), sizeof(uint16_t) * vertsBlendData.size());
        }

        static void ReorderVerticesByWeightCount(std::vector<size_t>& vertexIndices, const XSurface& surface, const XModelCommon& common)
        {
            if (common.m_bone_weight_data.weights.empty())
                return;

            const auto vertexCount = vertexIndices.size();
            std::vector<size_t> reorderLookup(vertexCount);
            std::ranges::iota(reorderLookup, 0);
            std::ranges::sort(reorderLookup,
                              [&common, &vertexIndices](const size_t& i0, const size_t& i1)
                              {
                                  const auto& weights0 = common.m_vertex_bone_weights[vertexIndices[i0]];
                                  const auto& weights1 = common.m_vertex_bone_weights[vertexIndices[i1]];

                                  if (weights0.weightCount < weights1.weightCount)
                                      return true;

                                  // If there is only one weight, make sure all vertices of the same bone follow another
                                  if (weights0.weightCount == 1)
                                  {
                                      const auto bone0 = common.m_bone_weight_data.weights[weights0.weightOffset].boneIndex;
                                      const auto bone1 = common.m_bone_weight_data.weights[weights1.weightOffset].boneIndex;
                                      return bone0 < bone1;
                                  }

                                  return false;
                              });

            std::vector<XSurfaceTri> preSortTris(surface.triCount);
            std::memcpy(preSortTris.data(), surface.triIndices, sizeof(XSurfaceTri) * surface.triCount);

            std::vector<size_t> reverseLookup(reorderLookup.size());
            for (auto i = 0u; i < reverseLookup.size(); i++)
                reverseLookup[reorderLookup[i]] = i;

            for (auto triIndex = 0u; triIndex < surface.triCount; triIndex++)
            {
                const auto& preSortTriIndices = preSortTris[triIndex];
                auto& triIndices = GetTriangles(surface)[triIndex];

                triIndices.i[0] = static_cast<uint16_t>(reverseLookup[preSortTriIndices.i[0]]);
                triIndices.i[1] = static_cast<uint16_t>(reverseLookup[preSortTriIndices.i[1]]);
                triIndices.i[2] = static_cast<uint16_t>(reverseLookup[preSortTriIndices.i[2]]);
            }

            for (auto& entry : reorderLookup)
                entry = vertexIndices[entry];

            vertexIndices = std::move(reorderLookup);
        }

        bool CreateXSurface(
            XSurface& surface, const XModelObject& commonObject, const XModelCommon& common, const TangentData& tangentData, unsigned& vertexOffset)
        {
            std::vector<size_t> xmodelToCommonVertexIndexLookup;
            std::unordered_map<size_t, size_t> usedVertices;

            constexpr auto maxTriCount = std::numeric_limits<decltype(XSurface::triCount)>::max();
            if (commonObject.m_faces.size() > maxTriCount)
            {
                con::error("Surface cannot have more than {} faces", maxTriCount);
                return false;
            }

            surface.triCount = static_cast<uint16_t>(commonObject.m_faces.size());

            // Some games round up to an even tri count
            const auto allocatedTriCount = utils::Align<unsigned>(surface.triCount, 2);
            surface.triIndices = reinterpret_cast<uint16_t*>(m_memory.Alloc<XSurfaceTri>(allocatedTriCount));

            xmodelToCommonVertexIndexLookup.reserve(static_cast<size_t>(surface.triCount) * std::extent_v<decltype(XModelFace::vertexIndex)>);
            for (auto faceIndex = 0u; faceIndex < surface.triCount; faceIndex++)
            {
                const auto& face = commonObject.m_faces[faceIndex];
                auto& tris = GetTriangles(surface)[faceIndex];

                for (auto triVertIndex = 0u; triVertIndex < std::extent_v<decltype(XModelFace::vertexIndex)>; triVertIndex++)
                {
                    const auto commonVertexIndex = face.vertexIndex[triVertIndex];
                    const auto existingVertex = usedVertices.find(commonVertexIndex);
                    if (existingVertex == usedVertices.end())
                    {
                        const auto xmodelVertexIndex = xmodelToCommonVertexIndexLookup.size();
                        tris.i[triVertIndex] = static_cast<uint16_t>(xmodelVertexIndex);

                        xmodelToCommonVertexIndexLookup.emplace_back(commonVertexIndex);
                        usedVertices.emplace(commonVertexIndex, xmodelVertexIndex);
                    }
                    else
                        tris.i[triVertIndex] = static_cast<uint16_t>(existingVertex->second);
                }
            }

            ReorderVerticesByWeightCount(xmodelToCommonVertexIndexLookup, surface, common);

            // Since bone weights are sorted by weight count, the last must have the highest weight count
            const auto maxWeightCount = common.m_vertex_bone_weights[xmodelToCommonVertexIndexLookup[xmodelToCommonVertexIndexLookup.size() - 1]].weightCount;
            const auto surfaceIsRigid = maxWeightCount <= 1;

            surface.deformed = !surfaceIsRigid;

            if (surfaceIsRigid)
            {
                constexpr auto maxVerticesForRigid = static_cast<size_t>(std::numeric_limits<decltype(XSurface::vertCount)>::max());
                if (vertexOffset + xmodelToCommonVertexIndexLookup.size() > maxVerticesForRigid)
                {
                    con::error("Lod exceeds limit of {} vertices for rigid models", maxVerticesForRigid);
                    return false;
                }
            }
            else
            {
                constexpr auto maxVerticesForAnimated =
                    std::min(static_cast<size_t>(std::numeric_limits<decltype(XSurface::vertCount)>::max()),
                             static_cast<size_t>(std::numeric_limits<std::remove_extent_t<decltype(XSurfaceVertexInfo::vertCount)>>::max()));

                if (vertexOffset + xmodelToCommonVertexIndexLookup.size() > maxVerticesForAnimated)
                {
                    con::error("Lod exceeds limit of {} vertices for animated models", maxVerticesForAnimated);
                    return false;
                }
            }

            surface.vertCount = static_cast<uint16_t>(xmodelToCommonVertexIndexLookup.size());
            surface.verts = m_memory.Alloc<GfxPackedVertex>(surface.vertCount);
            vertexOffset += surface.vertCount;

            for (auto vertexIndex = 0u; vertexIndex < surface.vertCount; vertexIndex++)
            {
                const auto commonVertexIndex = xmodelToCommonVertexIndexLookup[vertexIndex];
                const auto& commonVertex = common.m_vertices[commonVertexIndex];
                CreateVertex(surface.verts[vertexIndex], commonVertex, tangentData.m_tangents[commonVertexIndex], tangentData.m_binormals[commonVertexIndex]);
            }

            if (!common.m_bone_weight_data.weights.empty())
            {
                if (surfaceIsRigid)
                {
                    CreateVertListData(surface, xmodelToCommonVertexIndexLookup, common);
                }
                else if (maxWeightCount < std::extent_v<decltype(XSurfaceVertexInfo::vertCount)> + 1)
                {
                    CreateVertsBlendData(surface, xmodelToCommonVertexIndexLookup, common);
                }
                else
                {
                    con::error("Models must not have vertices that are influenced by more than {} bones",
                               std::extent_v<decltype(XSurfaceVertexInfo::vertCount)> + 1);
                    return false;
                }
            }

            // Pad odd triangle counts with a degenerate triangle.
            if (allocatedTriCount > surface.triCount)
            {
                assert(allocatedTriCount == surface.triCount + 1);
                GetTriangles(surface)[allocatedTriCount - 1].i[0] = GetTriangles(surface)[surface.triCount - 1].i[2];
                GetTriangles(surface)[allocatedTriCount - 1].i[1] = GetTriangles(surface)[surface.triCount - 1].i[2];
                GetTriangles(surface)[allocatedTriCount - 1].i[2] = GetTriangles(surface)[surface.triCount - 1].i[2];
            }

            return true;
        }

        bool LoadLod(const JsonXModel& jXModel,
                     const JsonXModelLod& jLod,
                     XModel& xmodel,
                     unsigned lodNumber,
                     AssetCreationContext& context,
                     AssetRegistration<AssetXModel>& registration)
        {
            const auto file = m_search_path.Open(jLod.file);
            if (!file.IsOpen())
            {
                PrintError(xmodel, std::format("Failed to open file for lod {}: \"{}\"", lodNumber, jLod.file));
                return false;
            }

            auto extension = std::filesystem::path(jLod.file).extension().string();
            utils::MakeStringLowerCase(extension);

            const auto common = LoadModelByExtension(*file.m_stream, extension);
            if (!common)
            {
                PrintError(xmodel, std::format("Failure while trying to load model for lod {}: \"{}\"", lodNumber, jLod.file));
                return false;
            }

            if (common->m_bones.empty())
                AutoGenerateArmature(*common, jXModel);
            else
                CheckSpecifiedRootBoneName(*common, jXModel);

            if (lodNumber == 0u)
            {
                if (!ApplyCommonBonesToXModel(jXModel, jLod, xmodel, lodNumber, *common, registration))
                    return false;
            }
            else
            {
                if (!VerifyBones(jLod, xmodel, lodNumber, *common))
                    return false;
            }

            auto& lodInfo = xmodel.lodInfo[lodNumber];
            lodInfo.dist = jLod.distance;

            constexpr auto maxSurfaces = std::numeric_limits<decltype(XModelLodInfo::numsurfs)>::max();
            if (common->m_objects.size() > maxSurfaces)
            {
                PrintError(xmodel, std::format("Lod {} cannot have more than {} surfaces", lodNumber, maxSurfaces));
                return false;
            }

            lodInfo.surfIndex = static_cast<uint16_t>(m_surfaces.size());
            lodInfo.numsurfs = static_cast<uint16_t>(common->m_objects.size());

            std::vector<Material*> materialAssets;
            materialAssets.reserve(common->m_materials.size());
            for (const auto& commonMaterial : common->m_materials)
            {
                auto* assetInfo = context.LoadDependency<AssetMaterial>(commonMaterial.name);
                if (!assetInfo)
                    return false;

                registration.AddDependency(assetInfo);
                materialAssets.push_back(assetInfo->Asset());
            }

            auto vertexOffset = 0u;
            TangentData tangentData;
            tangentData.CreateTangentData(*common);
            const auto surfaceCreationSuccessful =
                std::ranges::all_of(common->m_objects,
                                    [this, &common, &materialAssets, &tangentData, &vertexOffset](const XModelObject& commonObject)
                                    {
                                        XSurface surface{};
                                        if (!CreateXSurface(surface, commonObject, *common, tangentData, vertexOffset))
                                            return false;

                                        m_surfaces.emplace_back(surface);
                                        m_materials.push_back(materialAssets[commonObject.materialIndex]);
                                        return true;
                                    });

            if (!surfaceCreationSuccessful)
                return false;

            // Lod part bits are the sum of part bits of all of its surfaces
            static_assert(std::extent_v<decltype(XModelLodInfo::partBits)> == std::extent_v<decltype(XSurface::partBits)>);
            for (auto surfaceOffset = 0u; surfaceOffset < lodInfo.numsurfs; surfaceOffset++)
            {
                const auto& surface = m_surfaces[lodInfo.surfIndex + surfaceOffset];
                for (auto i = 0u; i < std::extent_v<decltype(XModelLodInfo::partBits)>; i++)
                    lodInfo.partBits[i] |= surface.partBits[i];
            }

            return true;
        }

        static void CalculateModelBounds(XModel& xmodel)
        {
            if (!xmodel.surfs)
                return;

            auto& minCoordinate = xmodel.mins;
            auto& maxCoordinate = xmodel.maxs;

            for (auto surfaceIndex = 0u; surfaceIndex < xmodel.lodInfo[0].numsurfs; surfaceIndex++)
            {
                const auto& surface = xmodel.surfs[surfaceIndex + xmodel.lodInfo[0].surfIndex];

                if (!surface.verts)
                    continue;

                for (auto vertIndex = 0u; vertIndex < surface.vertCount; vertIndex++)
                {
                    const auto& vertex = surface.verts[vertIndex];

                    minCoordinate[0] = std::min(minCoordinate[0], vertex.xyz[0]);
                    minCoordinate[1] = std::min(minCoordinate[1], vertex.xyz[1]);
                    minCoordinate[2] = std::min(minCoordinate[2], vertex.xyz[2]);
                    maxCoordinate[0] = std::max(maxCoordinate[0], vertex.xyz[0]);
                    maxCoordinate[1] = std::max(maxCoordinate[1], vertex.xyz[1]);
                    maxCoordinate[2] = std::max(maxCoordinate[2], vertex.xyz[2]);
                }
            }

            const auto maxX = std::max(std::abs(minCoordinate[0]), std::abs(maxCoordinate[0]));
            const auto maxY = std::max(std::abs(minCoordinate[1]), std::abs(maxCoordinate[1]));
            const auto maxZ = std::max(std::abs(minCoordinate[2]), std::abs(maxCoordinate[2]));
            xmodel.radius = Eigen::Vector3f(maxX, maxY, maxZ).norm();
        }

        bool CreateXModelFromJson(const JsonXModel& jXModel, XModel& xmodel, AssetCreationContext& context, AssetRegistration<AssetXModel>& registration)
        {
            constexpr auto maxLods = std::extent_v<decltype(XModel::lodInfo)>;
            if (jXModel.lods.size() > maxLods)
            {
                PrintError(xmodel, std::format("Model cannot have more than {} lods", maxLods));
                return false;
            }

            auto lodNumber = 0u;
            xmodel.numLods = static_cast<decltype(XModel::numLods)>(jXModel.lods.size());
            for (const auto& jLod : jXModel.lods)
            {
                if (!LoadLod(jXModel, jLod, xmodel, lodNumber++, context, registration))
                    return false;
            }

            constexpr auto maxSurfaces = std::numeric_limits<decltype(XModel::numsurfs)>::max();
            if (m_surfaces.size() > maxSurfaces)
            {
                PrintError(xmodel, std::format("Model cannot have more than {} surfaces across all lods", maxSurfaces));
                return false;
            }

            xmodel.numsurfs = static_cast<decltype(XModel::numsurfs)>(m_surfaces.size());

            xmodel.surfs = m_memory.Alloc<XSurface>(xmodel.numsurfs);
            memcpy(xmodel.surfs, m_surfaces.data(), sizeof(XSurface) * xmodel.numsurfs);

            xmodel.materialHandles = m_memory.Alloc<Material*>(xmodel.numsurfs);
            memcpy(xmodel.materialHandles, m_materials.data(), sizeof(Material*) * xmodel.numsurfs);

            CalculateModelBounds(xmodel);

            if (jXModel.collLod && jXModel.collLod.value() >= 0)
            {
                if (static_cast<unsigned>(jXModel.collLod.value()) >= jXModel.lods.size())
                {
                    PrintError(xmodel, "Collision lod is not a valid lod");
                    return false;
                }
                xmodel.collLod = static_cast<decltype(XModel::collLod)>(jXModel.collLod.value());
            }
            else
                xmodel.collLod = -1;

            if (jXModel.physPreset)
            {
                auto* physPreset = context.LoadDependency<AssetPhysPreset>(jXModel.physPreset.value());
                if (!physPreset)
                {
                    PrintError(xmodel, "Could not find phys preset");
                    return false;
                }
                registration.AddDependency(physPreset);
                xmodel.physPreset = physPreset->Asset();
            }
            else
            {
                xmodel.physPreset = nullptr;
            }

            xmodel.flags = jXModel.flags;

            xmodel.streamInfo.highMipBounds = xmodel::GenerateHighMipBoundsIW3Xenon270(xmodel, m_memory);

            return true;
        }

        bool m_gltf_bad_rotation_formulas;

        std::vector<XSurface> m_surfaces;
        std::vector<Material*> m_materials;

        MemoryManager& m_memory;
        ISearchPath& m_search_path;
        ZoneScriptStrings& m_script_strings;
        PartClassificationState m_part_classification_state;
    };
} // namespace

namespace xmodel
{
    std::unique_ptr<AssetCreator<AssetXModel>> CreateLoaderIW3Xenon270(MemoryManager& memory, ISearchPath& searchPath, Zone& zone)
    {
        return std::make_unique<XModelLoader>(memory, searchPath, zone.m_script_strings);
    }
} // namespace xmodel
