#include "Game/IW4Xenon482/CommonIW4Xenon482.h"
#include "Game/IW4Xenon482/XModel/XModelToCommonConverterIW4Xenon482.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "XModel/Gltf/GltfBinOutput.h"
#include "XModel/Gltf/GltfTextOutput.h"
#include "XModel/Gltf/GltfWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <utility>

#ifdef ARCH_x86

using namespace IW4Xenon482;

namespace
{
    struct RestoreObjWritingConfiguration
    {
        ObjWriting::Configuration_t original{ObjWriting::Configuration};

        ~RestoreObjWritingConfiguration()
        {
            ObjWriting::Configuration = original;
        }
    };

    XModelCommon CreateModel(const bool skinned)
    {
        XModelCommon common;
        common.m_name = "test_model";
        constexpr std::array boneNames{"j_root", "j_head", "j_hand", "j_gun"};
        for (auto boneIndex = 0u; boneIndex < boneNames.size(); ++boneIndex)
        {
            XModelBone bone{};
            bone.name = boneNames[boneIndex];
            if (boneIndex)
                bone.parentIndex = boneIndex - 1u;
            bone.scale[0] = bone.scale[1] = bone.scale[2] = 1.0f;
            bone.globalOffset[0] = static_cast<float>(boneIndex);
            bone.globalRotation.w = 1.0f;
            if (boneIndex == 1u)
            {
                bone.globalRotation.z = 0.6f;
                bone.globalRotation.w = 0.8f;
            }
            common.m_bones.emplace_back(std::move(bone));
        }
        common.CalculateBoneLocalsFromGlobals();

        for (auto vertexIndex = 0u; vertexIndex < 4u; ++vertexIndex)
        {
            common.m_vertices.push_back(XModelVertex{
                .coordinates = {static_cast<float>(vertexIndex % 2u), static_cast<float>(vertexIndex / 2u), 0.0f},
                .normal = {0.0f, -0.6f, 0.8f},
                .color = {1.0f, 0.5f, 0.25f, 1.0f},
                .uv = {static_cast<float>(vertexIndex % 2u), static_cast<float>(vertexIndex / 2u)},
            });
            const auto weightCount = skinned ? vertexIndex + 1u : 1u;
            const auto weightOffset = static_cast<unsigned>(common.m_bone_weight_data.weights.size());
            const auto weightSum = static_cast<float>(weightCount * (weightCount + 1u)) / 2.0f;
            for (auto weightIndex = 0u; weightIndex < weightCount; ++weightIndex)
                common.m_bone_weight_data.weights.push_back(
                    XModelBoneWeight{.boneIndex = skinned ? weightIndex : 1u, .weight = static_cast<float>(weightCount - weightIndex) / weightSum});
            common.m_vertex_bone_weights.push_back(XModelVertexBoneWeights{weightOffset, weightCount});
        }

        for (const auto* materialName : {"mat_a", "mat_b"})
        {
            XModelMaterial material{};
            material.ApplyDefaults();
            material.name = materialName;
            common.m_materials.emplace_back(std::move(material));
        }
        common.m_objects.push_back(XModelObject{.name = "first", .materialIndex = 0u, .m_faces = {XModelFace{{0u, 1u, 2u}}}});
        common.m_objects.push_back(XModelObject{.name = "second", .materialIndex = 1u, .m_faces = {XModelFace{{1u, 3u, 2u}}}});
        return common;
    }

    void CheckModel(const XModelCommon& actual, const XModelCommon& expected)
    {
        REQUIRE(actual.m_bones.size() == expected.m_bones.size());
        for (auto boneIndex = 0uz; boneIndex < expected.m_bones.size(); ++boneIndex)
        {
            CHECK(actual.m_bones[boneIndex].name == expected.m_bones[boneIndex].name);
            CHECK(actual.m_bones[boneIndex].parentIndex == expected.m_bones[boneIndex].parentIndex);
            CHECK_THAT(actual.m_bones[boneIndex].localRotation.z, Catch::Matchers::WithinAbs(expected.m_bones[boneIndex].localRotation.z, 0.0001f));
            for (auto axis = 0u; axis < 3u; ++axis)
                CHECK_THAT(actual.m_bones[boneIndex].globalOffset[axis], Catch::Matchers::WithinAbs(expected.m_bones[boneIndex].globalOffset[axis], 0.0001f));
        }
        REQUIRE(actual.m_objects.size() == expected.m_objects.size());
        for (auto objectIndex = 0uz; objectIndex < expected.m_objects.size(); ++objectIndex)
        {
            const auto& actualObject = actual.m_objects[objectIndex];
            const auto& expectedObject = expected.m_objects[objectIndex];
            CHECK(actual.m_materials[actualObject.materialIndex].name == expected.m_materials[expectedObject.materialIndex].name);
            REQUIRE(actualObject.m_faces.size() == expectedObject.m_faces.size());
            for (auto corner = 0u; corner < 3u; ++corner)
            {
                const auto actualVertexIndex = actualObject.m_faces[0].vertexIndex[corner];
                const auto expectedVertexIndex = expectedObject.m_faces[0].vertexIndex[corner];
                REQUIRE(actualVertexIndex < actual.m_vertices.size());
                const auto& actualVertex = actual.m_vertices[actualVertexIndex];
                const auto& expectedVertex = expected.m_vertices[expectedVertexIndex];
                for (auto axis = 0u; axis < 3u; ++axis)
                {
                    CHECK_THAT(actualVertex.coordinates[axis], Catch::Matchers::WithinAbs(expectedVertex.coordinates[axis], 0.0001f));
                    CHECK_THAT(actualVertex.normal[axis], Catch::Matchers::WithinAbs(expectedVertex.normal[axis], 0.002f));
                }
                for (auto axis = 0u; axis < 2u; ++axis)
                    CHECK_THAT(actualVertex.uv[axis], Catch::Matchers::WithinAbs(expectedVertex.uv[axis], 0.0001f));
                for (auto channel = 0u; channel < 4u; ++channel)
                    CHECK_THAT(actualVertex.color[channel], Catch::Matchers::WithinAbs(expectedVertex.color[channel], 0.004f));

                std::array<float, 4> actualWeights{}, expectedWeights{};
                const auto& actualWeightInfo = actual.m_vertex_bone_weights[actualVertexIndex];
                const auto& expectedWeightInfo = expected.m_vertex_bone_weights[expectedVertexIndex];
                for (auto weightIndex = 0u; weightIndex < actualWeightInfo.weightCount; ++weightIndex)
                {
                    const auto& weight = actual.m_bone_weight_data.weights[actualWeightInfo.weightOffset + weightIndex];
                    REQUIRE(weight.boneIndex < actualWeights.size());
                    actualWeights[weight.boneIndex] += weight.weight;
                }
                for (auto weightIndex = 0u; weightIndex < expectedWeightInfo.weightCount; ++weightIndex)
                {
                    const auto& weight = expected.m_bone_weight_data.weights[expectedWeightInfo.weightOffset + weightIndex];
                    expectedWeights[weight.boneIndex] += weight.weight;
                }
                for (auto boneIndex = 0uz; boneIndex < actualWeights.size(); ++boneIndex)
                    CHECK_THAT(actualWeights[boneIndex], Catch::Matchers::WithinAbs(expectedWeights[boneIndex], 0.0001f));
            }
        }
    }

    TEST_CASE("IW4 Xenon 482 XModels preserve LODs, surface indices and bone weights through disk and native fastfiles",
              "[alphaxmodel][iw4xenon482][system][xmodel]")
    {
        const auto skinned = GENERATE(false, true);
        const auto binary = GENERATE(false, true);
        CAPTURE(skinned, binary);
        const RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.ModelOutputFormat = binary ? ModelOutputFormat_e::GLB : ModelOutputFormat_e::GLTF;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.clear();
        const auto source = CreateModel(skinned);
        std::ostringstream modelOutput(std::ios::binary);
        if (binary)
        {
            const gltf::BinOutput output(modelOutput);
            gltf::Writer::CreateWriter(&output, "iw4", "test")->Write(source);
        }
        else
        {
            const gltf::TextOutput output(modelOutput);
            gltf::Writer::CreateWriter(&output, "iw4", "test")->Write(source);
        }
        const auto modelFile = binary ? "model_export/source.glb" : "model_export/source.gltf";
        nlohmann::json metadata{
            {"_type",       "xmodel"                                                                                  },
            {"_version",    2u                                                                                        },
            {"_game",       "iw4"                                                                                     },
            {"lods",        {{{"file", modelFile}, {"distance", 100.0f}}, {{"file", modelFile}, {"distance", 200.0f}}}},
            {"collLod",     1                                                                                         },
            {"physCollmap", "test_constraints"                                                                        },
            {"physPreset",  "test_phys"                                                                               },
            {"flags",       4u                                                                                        },
        };
        const auto nativeMipMetrics = GENERATE(false, true);
        const auto mipMetrics = nativeMipMetrics ? std::vector<uint16_t>{1u, 123u, 32768u, 65535u} : std::vector<uint16_t>(4u);
        if (nativeMipMetrics)
            metadata["invHighMipRadius"] = mipMetrics;
        MockSearchPath searchPath;
        searchPath.AddFileData(modelFile, modelOutput.str());
        searchPath.AddFileData("xmodel/test_model.json", metadata.dump());
        searchPath.AddFileData("partclassification.csv", "j_head,head\nj_hand,right_hand\nj_gun,gun\n");
        Zone zone("xmodel_round_trip", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext creationContext(zone, &creators, &ignoredAssets);
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_MATERIAL, ",mat_a", creationContext).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_MATERIAL, ",mat_b", creationContext).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_PHYSPRESET, ",test_phys", creationContext).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_PHYSCOLLMAP, ",test_constraints", creationContext).HasBeenSuccessful());
        REQUIRE(creators.CreateAsset(ASSET_TYPE_XMODEL, "test_model", creationContext).HasBeenSuccessful());

        const auto path = oat::paths::GetTempDirectory("IW4Xenon482XModelRoundTrip") / "xmodel.ff";
        {
            std::ofstream fastfile(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(fastfile, zone));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        const auto* asset = (*loaded)->m_pools.GetAsset<AssetXModel>("test_model");
        REQUIRE(asset);
        const auto& model = *asset->Asset();
        REQUIRE(model.numLods == 2u);
        REQUIRE(model.numsurfs == 4u);
        CHECK(model.lodInfo[1].surfIndex == 2u);
        CHECK(model.collLod == 1);
        CHECK(model.flags == 4u);
        REQUIRE(model.physCollmap);
        CHECK(std::string(model.physCollmap->name) == ",test_constraints");
        CHECK(model.partClassification[1] == HITLOC_HEAD);
        CHECK(model.partClassification[2] == HITLOC_R_HAND);
        CHECK(model.partClassification[3] == HITLOC_GUN);
        REQUIRE(model.invHighMipRadius);
        for (auto i = 0u; i < model.numsurfs; ++i)
            CHECK(model.invHighMipRadius[i] == mipMetrics[i]);
        CHECK_FALSE((*loaded)->m_pools.GetAsset<AssetXModelSurfs>("test_model_lod0"));
        if (!skinned)
        {
            REQUIRE(model.lodInfo[0].modelSurfs->surfs[0].vertListCount == 1u);
            CHECK(model.lodInfo[0].modelSurfs->surfs[0].vertList[0].boneOffset == 64u);
        }
        for (auto lod = 0u; lod < 2u; ++lod)
        {
            const auto common = xmodel::ToCommonConverterIW4Xenon482().Convert(*asset, lod);
            REQUIRE(common);
            CheckModel(*common, source);
        }

        MockOutputPath outputPath;
        const std::string basePath;
        AssetDumpingContext dumpingContext(**loaded, basePath, outputPath, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4Xenon482)->DumpZone(dumpingContext));
        const auto* jsonFile = outputPath.GetMockedFile("xmodel/test_model.json");
        REQUIRE(jsonFile);
        const auto dumpedMetadata = nlohmann::json::parse(jsonFile->AsString());
        CHECK(dumpedMetadata["_game"] == "iw4");
        CHECK(dumpedMetadata["lods"].size() == 2u);
        CHECK(dumpedMetadata["physPreset"] == "test_phys");
        CHECK(dumpedMetadata["physCollmap"] == "test_constraints");
        CHECK(dumpedMetadata["invHighMipRadius"] == nlohmann::json(mipMetrics));
        REQUIRE(outputPath.GetMockedFile(binary ? "model_export/test_model_lod1.glb" : "model_export/test_model_lod1.gltf"));

        MockSearchPath dumpedSearch;
        for (const auto& file : outputPath.GetMockedFileList())
            dumpedSearch.AddFileData(file.m_name, file.AsString());
        dumpedSearch.AddFileData("partclassification.csv", "j_head,head\nj_hand,right_hand\nj_gun,gun\n");
        Zone rebuilt("xmodel_rebuilt", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        AssetCreatorCollection rebuiltCreators(rebuilt);
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(rebuiltCreators, rebuilt, dumpedSearch, gdt);
        AssetCreationContext rebuiltContext(rebuilt, &rebuiltCreators, &ignoredAssets);
        REQUIRE(rebuiltCreators.CreateDefaultAsset(ASSET_TYPE_MATERIAL, ",mat_a", rebuiltContext).HasBeenSuccessful());
        REQUIRE(rebuiltCreators.CreateDefaultAsset(ASSET_TYPE_MATERIAL, ",mat_b", rebuiltContext).HasBeenSuccessful());
        REQUIRE(rebuiltCreators.CreateDefaultAsset(ASSET_TYPE_PHYSPRESET, ",test_phys", rebuiltContext).HasBeenSuccessful());
        REQUIRE(rebuiltCreators.CreateDefaultAsset(ASSET_TYPE_PHYSCOLLMAP, ",test_constraints", rebuiltContext).HasBeenSuccessful());
        REQUIRE(rebuiltCreators.CreateAsset(ASSET_TYPE_XMODEL, "test_model", rebuiltContext).HasBeenSuccessful());
        const auto* rebuiltModel = rebuilt.m_pools.GetAsset<AssetXModel>("test_model");
        REQUIRE(rebuiltModel);
        for (auto lod = 0u; lod < 2u; ++lod)
        {
            const auto common = xmodel::ToCommonConverterIW4Xenon482().Convert(*rebuiltModel, lod);
            REQUIRE(common);
            CheckModel(*common, source);
        }
    }

    TEST_CASE("IW4 Xenon 482 unit vectors use signed normalized ten-bit components", "[alphaxmodel][iw4xenon482][system][xmodel][packing]")
    {
        static_assert(sizeof(DObjSkelMat) == 64u);
        constexpr float negativeX[]{-1.0f, 0.0f, 0.0f};
        CHECK(Common::Vec3PackUnitVec(negativeX).packed == 0x201u);
        float decoded[3];
        Common::Vec3UnpackUnitVec(PackedUnitVec{0x201u | (0x1ffu << 10u)}, decoded);
        CHECK_THAT(decoded[0], Catch::Matchers::WithinAbs(-1.0f, 0.00001f));
        CHECK_THAT(decoded[1], Catch::Matchers::WithinAbs(1.0f, 0.00001f));
        CHECK(decoded[2] == 0.0f);
    }

    TEST_CASE("IW4Xenon482 rejects models exceeding the bone mask capacity", "[alphaxmodel][iw4xenon482][xmodel]")
    {
        auto source = CreateModel(false);
        for (auto boneIndex = source.m_bones.size(); boneIndex <= 160u; ++boneIndex)
        {
            XModelBone bone{};
            bone.name = "extra_" + std::to_string(boneIndex);
            bone.parentIndex = 0;
            bone.scale[0] = bone.scale[1] = bone.scale[2] = 1.0f;
            bone.globalRotation.w = 1.0f;
            source.m_bones.emplace_back(std::move(bone));
        }
        source.CalculateBoneLocalsFromGlobals();
        std::ostringstream modelOutput;
        const gltf::TextOutput output(modelOutput);
        gltf::Writer::CreateWriter(&output, "iw4", "test")->Write(source);
        MockSearchPath search;
        search.AddFileData("model_export/source.gltf", modelOutput.str());
        search.AddFileData("partclassification.csv", "j_head,head\n");
        const nlohmann::json metadata{
            {"_type",    "xmodel"                                                      },
            {"_version", 2u                                                            },
            {"_game",    "iw4"                                                         },
            {"lods",     {{{"file", "model_export/source.gltf"}, {"distance", 100.0f}}}},
            {"flags",    0u                                                            }
        };
        search.AddFileData("xmodel/too_many_bones.json", metadata.dump());
        Zone zone("invalid_model", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, zone, search, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(zone, &creators, &ignored);
        REQUIRE_FALSE(creators.CreateAsset(ASSET_TYPE_XMODEL, "too_many_bones", context).HasBeenSuccessful());
    }
} // namespace

#endif
