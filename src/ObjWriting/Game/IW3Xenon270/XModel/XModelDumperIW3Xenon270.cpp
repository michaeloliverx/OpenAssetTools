
#include "XModelDumperIW3Xenon270.h"

#include "Game/IW3Xenon270/XModel/JsonXModelIW3Xenon270.h"
#include "Game/IW3Xenon270/XModel/XModelToCommonConverterIW3Xenon270.h"
#include "ObjWriting.h"
#include "Utils/DistinctMapper.h"
#include "Utils/QuatInt16.h"
#include "XModel/Export/XModelBinWriter.h"
#include "XModel/Export/XModelExportWriter.h"
#include "XModel/Gltf/GltfBinOutput.h"
#include "XModel/Gltf/GltfTextOutput.h"
#include "XModel/Gltf/GltfWriter.h"
#include "XModel/Obj/ObjWriter.h"
#include "XModel/XModelWriter.h"

#include <cassert>
#include <format>

using namespace IW3Xenon270;

namespace
{
    const char* AssetName(const char* input)
    {
        if (input && input[0] == ',')
            return &input[1];

        return input;
    }

    std::string GetFileNameForLod(const std::string& modelName, const unsigned lod, const std::string& extension)
    {
        return std::format("model_export/{}_lod{}{}", modelName, lod, extension);
    }

    void DumpObjMtl(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        const auto& model = *asset.Asset();
        const auto mtlFile = context.OpenAssetFile(std::format("model_export/{}.mtl", model.name));

        if (!mtlFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = obj::CreateMtlWriter(*mtlFile, game->GetShortName(), context.m_zone.m_name);
        DistinctMapper<Material*> materialMapper(model.numsurfs);

        writer->Write(common);
    }

    void DumpObjLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto& model = *asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model.name, lod, ".obj"));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = obj::CreateObjWriter(*assetFile, std::format("{}.mtl", model.name), game->GetShortName(), context.m_zone.m_name);
        DistinctMapper<Material*> materialMapper(model.numsurfs);

        writer->Write(common);
    }

    void DumpXModelExportLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto model = *asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model.name, lod, ".xmodel_export"));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto writer = xmodel_export::CreateWriterForVersion6(*assetFile, game->GetShortName(), context.m_zone.m_name);
        writer->Write(common);
    }

    void DumpXModelBinLod(const XModelCommon& common, const AssetDumpingContext& context, const XAssetInfo<XModel>& asset, const unsigned lod)
    {
        const auto& model = *asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model.name, lod, ".xmodel_bin"));

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
        const auto& model = *asset.Asset();
        const auto assetFile = context.OpenAssetFile(GetFileNameForLod(model.name, lod, extension));

        if (!assetFile)
            return;

        const auto* game = IGame::GetGameById(context.m_zone.m_game_id);
        const auto output = std::make_unique<T>(*assetFile);
        const auto writer = gltf::Writer::CreateWriter(output.get(), game->GetShortName(), context.m_zone.m_name);

        writer->Write(common);
    }

    bool DumpXModelSurfs(const AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        const auto& model = *asset.Asset();

        for (auto currentLod = 0u; currentLod < model.numLods; currentLod++)
        {
            const auto maybeCommon = xmodel::ToCommonConverterIW3Xenon270().Convert(asset, currentLod);
            if (!maybeCommon)
            {
                con::warn("Failed to convert xmodel \"{}\" (lod: {})", model.name, currentLod);
                return false;
            }

            switch (ObjWriting::Configuration.ModelOutputFormat)
            {
            case ModelOutputFormat_e::OBJ:
                DumpObjLod(*maybeCommon, context, asset, currentLod);
                if (currentLod == 0u)
                    DumpObjMtl(*maybeCommon, context, asset);
                break;

            case ModelOutputFormat_e::XMODEL_EXPORT:
                DumpXModelExportLod(*maybeCommon, context, asset, currentLod);
                break;

            case ModelOutputFormat_e::XMODEL_BIN:
                DumpXModelBinLod(*maybeCommon, context, asset, currentLod);
                break;

            case ModelOutputFormat_e::GLTF:
                DumpGltfLod<gltf::TextOutput>(*maybeCommon, context, asset, currentLod, ".gltf");
                break;

            case ModelOutputFormat_e::GLB:
                DumpGltfLod<gltf::BinOutput>(*maybeCommon, context, asset, currentLod, ".glb");
                break;

            default:
                assert(false);
                break;
            }
        }
        return true;
    }

    const char* GetExtensionForModelByConfig()
    {
        switch (ObjWriting::Configuration.ModelOutputFormat)
        {
        case ModelOutputFormat_e::XMODEL_EXPORT:
            return ".xmodel_export";
        case ModelOutputFormat_e::XMODEL_BIN:
            return ".xmodel_bin";
        case ModelOutputFormat_e::OBJ:
            return ".obj";
        case ModelOutputFormat_e::GLTF:
            return ".gltf";
        case ModelOutputFormat_e::GLB:
            return ".glb";
        default:
            assert(false);
            return "";
        }
    }

    bool IsAnimated(const XModel& model)
    {
        for (auto i = 0u; i < model.numsurfs; i++)
        {
            const auto& surf = model.surfs[i];
            if (surf.vertInfo.vertsBlend)
                return true;
        }

        return false;
    }

    bool HasNulledTrans(const XModel& model)
    {
        if (model.trans == nullptr)
            return true;

        const auto transCount = (model.numBones - model.numRootBones) * 3u;
        for (auto i = 0u; i < transCount; i++)
        {
            if (model.trans[i] != 0)
                return false;
        }

        return true;
    }

    bool HasNonNullBoneInfoTrans(const XModel& model)
    {
        if (model.boneInfo == nullptr)
            return false;

        for (auto i = 0u; i < model.numBones; i++)
        {
            const auto& boneInfo = model.boneInfo[i];
            if (boneInfo.offset[0] != 0 || boneInfo.offset[1] != 0 || boneInfo.offset[2] != 0)
                return true;
        }

        return false;
    }

    JsonXModelType GetType(const XModel& model)
    {
        if (!IsAnimated(model))
            return JsonXModelType::RIGID;

        if (HasNulledTrans(model) && HasNonNullBoneInfoTrans(model))
            return JsonXModelType::VIEWHANDS;

        return JsonXModelType::ANIMATED;
    }

    void SetJsonRootBoneName(AssetDumpingContext& context, JsonXModel& jXModel, const XModel& model)
    {
        assert(model.boneNames);
        assert(model.boneNames[0] < context.m_zone.m_script_strings.Count());
        assert(model.numRootBones <= 1);

        if (!model.boneNames || model.boneNames[0] >= context.m_zone.m_script_strings.Count() || model.numRootBones == 0)
            return;

        const auto& rootBoneName = context.m_zone.m_script_strings[model.boneNames[0]];

        if (rootBoneName != xmodel::DEFAULT_XMODEL_ROOT_BONE_NAME)
            jXModel.rootBoneName = rootBoneName;
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

    bool CanOmitDefaultArmature()
    {
        return ObjWriting::Configuration.ModelOutputFormat != ModelOutputFormat_e::XMODEL_EXPORT
               && ObjWriting::Configuration.ModelOutputFormat != ModelOutputFormat_e::XMODEL_BIN;
    }

    void CreateJsonXModel(AssetDumpingContext& context, JsonXModel& jXModel, const XModel& model)
    {
        if (model.collLod >= 0)
            jXModel.collLod = model.collLod;

        jXModel.type = GetType(model);

        if (CanOmitDefaultArmature() && HasDefaultArmatureForAllLods(model))
        {
            // Preserve attachment names when omitting the default armature.
            SetJsonRootBoneName(context, jXModel, model);
        }

        for (auto lodNumber = 0u; lodNumber < model.numLods; lodNumber++)
        {
            JsonXModelLod lod;
            lod.file = std::format("model_export/{}_lod{}{}", model.name, lodNumber, GetExtensionForModelByConfig());
            lod.distance = model.lodInfo[lodNumber].dist;

            jXModel.lods.emplace_back(std::move(lod));
        }

        if (model.physPreset && model.physPreset->name)
            jXModel.physPreset = AssetName(model.physPreset->name);

        jXModel.flags = model.flags;
    }

    void DumpXModelJson(AssetDumpingContext& context, const XAssetInfo<XModel>& asset)
    {
        const auto assetFile = context.OpenAssetFile(xmodel::GetJsonFileNameForAssetName(asset.m_name));
        if (!assetFile)
            return;

        const auto& model = *asset.Asset();

        JsonXModel jsonXModel;
        CreateJsonXModel(context, jsonXModel, model);
        nlohmann::json jRoot = jsonXModel;

        jRoot["$schema"] = "http://openassettools.dev/schema/xmodel.v1.json";
        jRoot["_type"] = "xmodel";
        jRoot["_version"] = 2;
        jRoot["_game"] = "iw3";

        *assetFile << std::setw(4) << jRoot << "\n";
    }
} // namespace

namespace xmodel
{
    void DumperIW3Xenon270::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetXModel::Type>& asset)
    {
        if (DumpXModelSurfs(context, asset))
            DumpXModelJson(context, asset);
    }
} // namespace xmodel
