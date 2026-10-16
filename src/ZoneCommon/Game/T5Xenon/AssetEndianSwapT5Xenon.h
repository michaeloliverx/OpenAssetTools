#pragma once
#include "Game/T5Xenon/T5Xenon.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace T5Xenon
{
    enum class EndianOperation
    {
        Decode,
        Encode
    };
    static inline void EndianSwap(PhysPreset& value, EndianOperation operation);
    static inline void EndianSwap(PhysConstraint& value, EndianOperation operation);
    static inline void EndianSwap(PhysConstraints& value, EndianOperation operation);
    static inline void EndianSwap(DestructibleStage& value, EndianOperation operation);
    static inline void EndianSwap(DestructiblePiece& value, EndianOperation operation);
    static inline void EndianSwap(DestructibleDef& value, EndianOperation operation);
    static inline void EndianSwap(ComPrimaryLight& value, EndianOperation operation);
    static inline void EndianSwap(ComWaterHeader& value, EndianOperation operation);
    static inline void EndianSwap(ComWaterCell& value, EndianOperation operation);
    static inline void EndianSwap(ComBurnableHeader& value, EndianOperation operation);
    static inline void EndianSwap(ComBurnableSample& value, EndianOperation operation);
    static inline void EndianSwap(ComBurnableCell& value, EndianOperation operation);
    static inline void EndianSwap(ComWorld& value, EndianOperation operation);
    static inline void EndianSwap(pathbasenode_t& value, EndianOperation operation);
    static inline void EndianSwap(pathlink_s& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_constant_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_t& value, EndianOperation operation);
    static inline void EndianSwap(PathData& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_info_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_nodes_t& value, EndianOperation operation);
    static inline void EndianSwap(GameWorldSp& value, EndianOperation operation);
    static inline void EndianSwap(GameWorldMp& value, EndianOperation operation);
    static inline void EndianSwap(XAnimNotifyInfo& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPart& value, EndianOperation operation);
    static inline void EndianSwap(XAnimPartTrans& value, EndianOperation operation);
    static inline void EndianSwap(XAnimPartTransFrames& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDynamicFrames& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPartQuat& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPartQuatDataFrames& value, EndianOperation operation);
    static inline void EndianSwap(XAnimIndices& value, EndianOperation operation);
    static inline void EndianSwap(XAnimParts& value, EndianOperation operation);
    static inline void EndianSwap(DObjAnimMat& value, EndianOperation operation);
    static inline void EndianSwap(GfxPackedVertex& value, EndianOperation operation);
    static inline void EndianSwap(GfxPackedVertexQuant& value, EndianOperation operation);
    static inline void EndianSwap(XSurfaceCollisionAabb& value, EndianOperation operation);
    static inline void EndianSwap(XSurfaceCollisionNode& value, EndianOperation operation);
    static inline void EndianSwap(XSurfaceCollisionLeaf& value, EndianOperation operation);
    static inline void EndianSwap(XSurfaceCollisionTree& value, EndianOperation operation);
    static inline void EndianSwap(XRigidVertList& value, EndianOperation operation);
    static inline void EndianSwap(XSurfaceVertexInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxPackedVertex0Union& value, EndianOperation operation);
    static inline void EndianSwap(XSurface& value, EndianOperation operation);
    static inline void EndianSwap(XModelLodInfo& value, EndianOperation operation);
    static inline void EndianSwap(XModelCollSurf_s& value, EndianOperation operation);
    static inline void EndianSwap(XBoneInfo& value, EndianOperation operation);
    static inline void EndianSwap(XModelHighMipBounds& value, EndianOperation operation);
    static inline void EndianSwap(XModelStreamInfo& value, EndianOperation operation);
    static inline void EndianSwap(cplane_s& value, EndianOperation operation);
    static inline void EndianSwap(cbrushside_t& value, EndianOperation operation);
    static inline void EndianSwap(vec3_t& value, EndianOperation operation);
    static inline void EndianSwap(cStaticModel_s& value, EndianOperation operation);
    static inline void EndianSwap(dmaterial_t& value, EndianOperation operation);
    static inline void EndianSwap(cNode_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeaf_s& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNodeLeaf_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNodeData_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNode_s& value, EndianOperation operation);
    static inline void EndianSwap(CollisionBorder& value, EndianOperation operation);
    static inline void EndianSwap(CollisionPartition& value, EndianOperation operation);
    static inline void EndianSwap(CollisionAabbTree& value, EndianOperation operation);
    static inline void EndianSwap(cmodel_t& value, EndianOperation operation);
    static inline void EndianSwap(cbrush_t& value, EndianOperation operation);
    static inline void EndianSwap(XModelPiece& value, EndianOperation operation);
    static inline void EndianSwap(XModelPieces& value, EndianOperation operation);
    static inline void EndianSwap(DynEntityDef& value, EndianOperation operation);
    static inline void EndianSwap(DynEntityPose& value, EndianOperation operation);
    static inline void EndianSwap(DynEntityClient& value, EndianOperation operation);
    static inline void EndianSwap(DynEntityServer& value, EndianOperation operation);
    static inline void EndianSwap(DynEntityColl& value, EndianOperation operation);
    static inline void EndianSwap(par_t& value, EndianOperation operation);
    static inline void EndianSwap(constraint_t& value, EndianOperation operation);
    static inline void EndianSwap(rope_frame_verts_t& value, EndianOperation operation);
    static inline void EndianSwap(rope_client_verts_t& value, EndianOperation operation);
    static inline void EndianSwap(rope_t& value, EndianOperation operation);
    static inline void EndianSwap(clipMap_t& value, EndianOperation operation);
    static inline void EndianSwap(MapEnts& value, EndianOperation operation);
    static inline void EndianSwap(BrushWrapper& value, EndianOperation operation);
    static inline void EndianSwap(PhysGeomInfo& value, EndianOperation operation);
    static inline void EndianSwap(PhysGeomList& value, EndianOperation operation);
    static inline void EndianSwap(Collmap& value, EndianOperation operation);
    static inline void EndianSwap(XModelQuat& value, EndianOperation operation);
    static inline void EndianSwap(XModel& value, EndianOperation operation);
    static inline void EndianSwap(snd_asset& value, EndianOperation operation);
    static inline void EndianSwap(LoadedSound& value, EndianOperation operation);
    static inline void EndianSwap(PrimedSound& value, EndianOperation operation);
    static inline void EndianSwap(StreamedSound& value, EndianOperation operation);
    static inline void EndianSwap(SoundFileRef& value, EndianOperation operation);
    static inline void EndianSwap(SoundFile& value, EndianOperation operation);
    static inline void EndianSwap(snd_alias_t& value, EndianOperation operation);
    static inline void EndianSwap(snd_alias_list_t& value, EndianOperation operation);
    static inline void EndianSwap(SndIndexEntry& value, EndianOperation operation);
    static inline void EndianSwap(snd_radverb& value, EndianOperation operation);
    static inline void EndianSwap(snd_snapshot& value, EndianOperation operation);
    static inline void EndianSwap(SndBank& value, EndianOperation operation);
    static inline void EndianSwap(SndPatch& value, EndianOperation operation);
    static inline void EndianSwap(snd_group& value, EndianOperation operation);
    static inline void EndianSwap(snd_curve& value, EndianOperation operation);
    static inline void EndianSwap(snd_pan& value, EndianOperation operation);
    static inline void EndianSwap(snd_snapshot_group& value, EndianOperation operation);
    static inline void EndianSwap(snd_context& value, EndianOperation operation);
    static inline void EndianSwap(snd_master& value, EndianOperation operation);
    static inline void EndianSwap(SndDriverGlobals& value, EndianOperation operation);
    static inline void EndianSwap(Glyph& value, EndianOperation operation);
    static inline void EndianSwap(Font_s& value, EndianOperation operation);
    static inline void EndianSwap(MenuList& value, EndianOperation operation);
    static inline void EndianSwap(rectDef_s& value, EndianOperation operation);
    static inline void EndianSwap(windowDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ScriptCondition& value, EndianOperation operation);
    static inline void EndianSwap(operandInternalDataUnion& value, EndianOperation operation);
    static inline void EndianSwap(Operand& value, EndianOperation operation);
    static inline void EndianSwap(expressionRpnDataUnion& value, EndianOperation operation);
    static inline void EndianSwap(expressionRpn& value, EndianOperation operation);
    static inline void EndianSwap(ExpressionStatement& value, EndianOperation operation);
    static inline void EndianSwap(GenericEventScript& value, EndianOperation operation);
    static inline void EndianSwap(GenericEventHandler& value, EndianOperation operation);
    static inline void EndianSwap(ItemKeyHandler& value, EndianOperation operation);
    static inline void EndianSwap(textExp_s& value, EndianOperation operation);
    static inline void EndianSwap(enumDvarDef_s& value, EndianOperation operation);
    static inline void EndianSwap(editFieldDef_s& value, EndianOperation operation);
    static inline void EndianSwap(columnInfo_s& value, EndianOperation operation);
    static inline void EndianSwap(MenuCell& value, EndianOperation operation);
    static inline void EndianSwap(MenuRow& value, EndianOperation operation);
    static inline void EndianSwap(listBoxDef_s& value, EndianOperation operation);
    static inline void EndianSwap(multiDef_s& value, EndianOperation operation);
    static inline void EndianSwap(focusDefData_t& value, EndianOperation operation);
    static inline void EndianSwap(focusItemDef_s& value, EndianOperation operation);
    static inline void EndianSwap(gameMsgDef_s& value, EndianOperation operation);
    static inline void EndianSwap(textDefData_t& value, EndianOperation operation);
    static inline void EndianSwap(textDef_s& value, EndianOperation operation);
    static inline void EndianSwap(imageDef_s& value, EndianOperation operation);
    static inline void EndianSwap(ownerDrawDef_s& value, EndianOperation operation);
    static inline void EndianSwap(itemDefData_t& value, EndianOperation operation);
    static inline void EndianSwap(rectData_s& value, EndianOperation operation);
    static inline void EndianSwap(animParamsDef_t& value, EndianOperation operation);
    static inline void EndianSwap(UIAnimInfo& value, EndianOperation operation);
    static inline void EndianSwap(itemDef_s& value, EndianOperation operation);
    static inline void EndianSwap(menuDef_t& value, EndianOperation operation);
    static inline void EndianSwap(FxIntRange& value, EndianOperation operation);
    static inline void EndianSwap(FxFloatRange& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVec3Range& value, EndianOperation operation);
    static inline void EndianSwap(FxSpawnDef& value, EndianOperation operation);
    static inline void EndianSwap(FxElemAtlas& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVelStateInFrame& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVelStateSample& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVisualState& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVisStateSample& value, EndianOperation operation);
    static inline void EndianSwap(FxElemMarkVisuals& value, EndianOperation operation);
    static inline void EndianSwap(FxEffectDefRef& value, EndianOperation operation);
    static inline void EndianSwap(FxElemVisuals& value, EndianOperation operation);
    static inline void EndianSwap(FxElemDefVisuals& value, EndianOperation operation);
    static inline void EndianSwap(FxTrailVertex& value, EndianOperation operation);
    static inline void EndianSwap(FxTrailDef& value, EndianOperation operation);
    static inline void EndianSwap(FxElemDefUnion& value, EndianOperation operation);
    static inline void EndianSwap(FxElemSpawnSound& value, EndianOperation operation);
    static inline void EndianSwap(FxElemDef& value, EndianOperation operation);
    static inline void EndianSwap(FxEffectDef& value, EndianOperation operation);
    static inline void EndianSwap(FxImpactEntry& value, EndianOperation operation);
    static inline void EndianSwap(FxImpactTable& value, EndianOperation operation);
    static inline void EndianSwap(StringTableCell& value, EndianOperation operation);
    static inline void EndianSwap(StringTable& value, EndianOperation operation);
    static inline void EndianSwap(PackIndex& value, EndianOperation operation);
    static inline void EndianSwap(XGlobals& value, EndianOperation operation);
    static inline void EndianSwap(TextureListElement& value, EndianOperation operation);
    static inline void EndianSwap(TextureList& value, EndianOperation operation);
    static inline void EndianSwap(ddlMemberDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ddlStructDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ddlEnumDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ddlDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ddlRoot_t& value, EndianOperation operation);
    static inline void EndianSwap(GlassDef& value, EndianOperation operation);
    static inline void EndianSwap(Glass& value, EndianOperation operation);
    static inline void EndianSwap(Glasses& value, EndianOperation operation);
    static inline void EndianSwap(MaterialVertexDeclaration& value, EndianOperation operation);
    static inline void EndianSwap(GfxVertexShaderLoadDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialVertexShaderProgram& value, EndianOperation operation);
    static inline void EndianSwap(MaterialVertexShader& value, EndianOperation operation);
    static inline void EndianSwap(GfxPixelShaderLoadDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialPixelShaderProgram& value, EndianOperation operation);
    static inline void EndianSwap(MaterialPixelShader& value, EndianOperation operation);
    static inline void EndianSwap(MaterialArgumentDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialShaderArgument& value, EndianOperation operation);
    static inline void EndianSwap(MaterialPass& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTechnique& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTechniqueSet& value, EndianOperation operation);
    static inline void EndianSwap(GfxImage& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightImage& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightDef& value, EndianOperation operation);
    static inline void EndianSwap(WaterWritable& value, EndianOperation operation);
    static inline void EndianSwap(water_t& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTextureDefInfo& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTextureDef& value, EndianOperation operation);
    static inline void EndianSwap(GfxStateBits& value, EndianOperation operation);
    static inline void EndianSwap(MaterialConstantDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialInfo& value, EndianOperation operation);
    static inline void EndianSwap(Material& value, EndianOperation operation);
    static inline void EndianSwap(vec2_t& value, EndianOperation operation);
    static inline void EndianSwap(flameTable& value, EndianOperation operation);
    static inline void EndianSwap(WeaponDef& value, EndianOperation operation);
    static inline void EndianSwap(WeaponVariantDef& value, EndianOperation operation);
    static inline void EndianSwap(RawFile& value, EndianOperation operation);
    static inline void EndianSwap(LocalizeEntry& value, EndianOperation operation);
    static inline void EndianSwap(EmblemLayer& value, EndianOperation operation);
    static inline void EndianSwap(EmblemCategory& value, EndianOperation operation);
    static inline void EndianSwap(EmblemIcon& value, EndianOperation operation);
    static inline void EndianSwap(EmblemBackground& value, EndianOperation operation);
    static inline void EndianSwap(EmblemSet& value, EndianOperation operation);
    static inline void EndianSwap(GfxTexture& value, EndianOperation operation);
    static inline void EndianSwap(GfxColor& value, EndianOperation operation);
    static inline void EndianSwap(PackedUnitVec& value, EndianOperation operation);
    static inline void EndianSwap(GfxDrawSurf& value, EndianOperation operation);
    static inline void EndianSwap(GfxStreamingAabbTree& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldStreamInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldXModelStreamInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldSunColor& value, EndianOperation operation);
    static inline void EndianSwap(SunLightParseParams& value, EndianOperation operation);
    static inline void EndianSwap(float44& value, EndianOperation operation);
    static inline void EndianSwap(GfxLight& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightCorona& value, EndianOperation operation);
    static inline void EndianSwap(GfxShadowMapVolume& value, EndianOperation operation);
    static inline void EndianSwap(GfxVolumePlane& value, EndianOperation operation);
    static inline void EndianSwap(GfxExposureVolume& value, EndianOperation operation);
    static inline void EndianSwap(GfxSkyDynamicIntensity& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldDpvsPlanes& value, EndianOperation operation);
    static inline void EndianSwap(GfxPortalWritable& value, EndianOperation operation);
    static inline void EndianSwap(DpvsPlane& value, EndianOperation operation);
    static inline void EndianSwap(GfxPortal& value, EndianOperation operation);
    static inline void EndianSwap(GfxAabbTree& value, EndianOperation operation);
    static inline void EndianSwap(GfxCell& value, EndianOperation operation);
    static inline void EndianSwap(GfxReflectionProbeVolumeData& value, EndianOperation operation);
    static inline void EndianSwap(GfxReflectionProbe& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightmapArray& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldVertex& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldVertexData& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldVertexLayerData& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldDraw& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightGridEntry& value, EndianOperation operation);
    static inline void EndianSwap(GfxCompressedLightGridColors& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightGrid& value, EndianOperation operation);
    static inline void EndianSwap(GfxBrushModelWritable& value, EndianOperation operation);
    static inline void EndianSwap(GfxBrushModel& value, EndianOperation operation);
    static inline void EndianSwap(MaterialMemory& value, EndianOperation operation);
    static inline void EndianSwap(sunflare_t& value, EndianOperation operation);
    static inline void EndianSwap(XModelDrawInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxSceneDynModel& value, EndianOperation operation);
    static inline void EndianSwap(BModelDrawInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxSceneDynBrush& value, EndianOperation operation);
    static inline void EndianSwap(GfxShadowGeometry& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightRegionAxis& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightRegionHull& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightRegion& value, EndianOperation operation);
    static inline void EndianSwap(GfxStaticModelInst& value, EndianOperation operation);
    static inline void EndianSwap(srfTriangles_t& value, EndianOperation operation);
    static inline void EndianSwap(GfxSurface& value, EndianOperation operation);
    static inline void EndianSwap(GfxCullGroup& value, EndianOperation operation);
    static inline void EndianSwap(GfxPackedPlacement& value, EndianOperation operation);
    static inline void EndianSwap(GfxStaticModelDrawInst& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldDpvsStatic& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldDpvsDynamic& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldLodChain& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldLodInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxWaterBuffer& value, EndianOperation operation);
    static inline void EndianSwap(Occluder& value, EndianOperation operation);
    static inline void EndianSwap(GfxOutdoorBounds& value, EndianOperation operation);
    static inline void EndianSwap(GfxHeroLight& value, EndianOperation operation);
    static inline void EndianSwap(GfxHeroLightTree& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorld& value, EndianOperation operation);

    static inline void EndianSwap(uint8_t&, EndianOperation = EndianOperation::Decode) {}

    static inline void EndianSwap(int8_t&, EndianOperation = EndianOperation::Decode) {}

    static inline void EndianSwap(char&, EndianOperation = EndianOperation::Decode) {}

    static inline void EndianSwap(bool&, EndianOperation = EndianOperation::Decode) {}

    static inline void EndianSwap(uint16_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(int16_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(uint32_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(int32_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(uint64_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(int64_t& value, EndianOperation = EndianOperation::Decode)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(float& value, EndianOperation = EndianOperation::Decode)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(value));
    }

    template<typename T> static inline void EndianSwap(T*& value, EndianOperation = EndianOperation::Decode)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(value));
    }

    template<typename T>
        requires std::is_enum_v<T>
    static inline void EndianSwap(T& value, EndianOperation = EndianOperation::Decode)
    {
        EndianSwap(reinterpret_cast<std::underlying_type_t<T>&>(value));
    }

    template<typename T, size_t N> static inline void EndianSwap(T (&value)[N], EndianOperation operation = EndianOperation::Decode)
    {
        for (auto& item : value)
            EndianSwap(item, operation);
    }

    template<typename T> static inline T EndianSwapDiscriminator(T& value, EndianOperation operation)
    {
        if (operation == EndianOperation::Decode)
        {
            EndianSwap(value);
            return value;
        }
        const auto host = value;
        EndianSwap(value);
        return host;
    }

    static inline void EndianSwap(PhysPreset& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.mass, operation);
        EndianSwap(value.bounce, operation);
        EndianSwap(value.friction, operation);
        EndianSwap(value.bulletForceScale, operation);
        EndianSwap(value.explosiveForceScale, operation);
        EndianSwap(value.sndAliasPrefix, operation);
        EndianSwap(value.piecesSpreadFraction, operation);
        EndianSwap(value.piecesUpwardVelocity, operation);
        EndianSwap(value.canFloat, operation);
        EndianSwap(value.gravityScale, operation);
        EndianSwap(value.centerOfMassOffset, operation);
        EndianSwap(value.buoyancyBoxMin, operation);
        EndianSwap(value.buoyancyBoxMax, operation);
    }

    static inline void EndianSwap(PhysConstraint& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.targetname, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.attach_point_type1, operation);
        EndianSwap(value.target_index1, operation);
        EndianSwap(value.target_ent1, operation);
        EndianSwap(value.target_bone1, operation);
        EndianSwap(value.attach_point_type2, operation);
        EndianSwap(value.target_index2, operation);
        EndianSwap(value.target_ent2, operation);
        EndianSwap(value.target_bone2, operation);
        EndianSwap(value.offset.v, operation);
        EndianSwap(value.pos.v, operation);
        EndianSwap(value.pos2.v, operation);
        EndianSwap(value.dir.v, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.timeout, operation);
        EndianSwap(value.min_health, operation);
        EndianSwap(value.max_health, operation);
        EndianSwap(value.distance, operation);
        EndianSwap(value.damp, operation);
        EndianSwap(value.power, operation);
        EndianSwap(value.scale.v, operation);
        EndianSwap(value.spin_scale, operation);
        EndianSwap(value.minAngle, operation);
        EndianSwap(value.maxAngle, operation);
        EndianSwap(value.material, operation);
        EndianSwap(value.constraintHandle, operation);
        EndianSwap(value.rope_index, operation);
        EndianSwap(value.centity_num, operation);
    }

    static inline void EndianSwap(PhysConstraints& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.count, operation);
        for (auto& constraint : value.data)
            EndianSwap(constraint, operation);
    }

    static inline void EndianSwap(DestructibleStage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.showBone, operation);
        EndianSwap(value.breakHealth, operation);
        EndianSwap(value.maxTime, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.breakEffect, operation);
        EndianSwap(value.breakSound, operation);
        EndianSwap(value.breakNotify, operation);
        EndianSwap(value.loopSound, operation);
        EndianSwap(value.spawnModel, operation);
        EndianSwap(value.physPreset, operation);
    }

    static inline void EndianSwap(DestructiblePiece& value, [[maybe_unused]] const EndianOperation operation)
    {
        for (auto& stage : value.stages)
            EndianSwap(stage, operation);
        EndianSwap(value.parentDamagePercent, operation);
        EndianSwap(value.bulletDamageScale, operation);
        EndianSwap(value.explosiveDamageScale, operation);
        EndianSwap(value.meleeDamageScale, operation);
        EndianSwap(value.impactDamageScale, operation);
        EndianSwap(value.entityDamageTransfer, operation);
        EndianSwap(value.physConstraints, operation);
        EndianSwap(value.health, operation);
        EndianSwap(value.damageSound, operation);
        EndianSwap(value.burnEffect, operation);
        EndianSwap(value.burnSound, operation);
        EndianSwap(value.enableLabel, operation);
        EndianSwap(value.hideBones, operation);
    }

    static inline void EndianSwap(DestructibleDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.model, operation);
        EndianSwap(value.pristineModel, operation);
        EndianSwap(value.numPieces, operation);
        EndianSwap(value.pieces, operation);
        EndianSwap(value.clientOnly, operation);
    }

    static inline void EndianSwap(ComPrimaryLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cullDist, operation);
        EndianSwap(value.color, operation);
        EndianSwap(value.dir, operation);
        EndianSwap(value.origin, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.cosHalfFovOuter, operation);
        EndianSwap(value.cosHalfFovInner, operation);
        EndianSwap(value.cosHalfFovExpanded, operation);
        EndianSwap(value.rotationLimit, operation);
        EndianSwap(value.translationLimit, operation);
        EndianSwap(value.mipDistance, operation);
        EndianSwap(value.diffuseColor, operation);
        EndianSwap(value.specularColor, operation);
        EndianSwap(value.attenuation, operation);
        EndianSwap(value.falloff, operation);
        EndianSwap(value.angle, operation);
        EndianSwap(value.aAbB, operation);
        EndianSwap(value.cookieControl0, operation);
        EndianSwap(value.cookieControl1, operation);
        EndianSwap(value.cookieControl2, operation);
        EndianSwap(value.defName, operation);
    }

    static inline void EndianSwap(ComWaterHeader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.minx, operation);
        EndianSwap(value.miny, operation);
        EndianSwap(value.maxx, operation);
        EndianSwap(value.maxy, operation);
    }

    static inline void EndianSwap(ComWaterCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.waterheight, operation);
    }

    static inline void EndianSwap(ComBurnableHeader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.minx, operation);
        EndianSwap(value.miny, operation);
        EndianSwap(value.maxx, operation);
        EndianSwap(value.maxy, operation);
    }

    static inline void EndianSwap(ComBurnableSample& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(ComBurnableCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.x, operation);
        EndianSwap(value.y, operation);
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(ComWorld& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.isInUse, operation);
        EndianSwap(value.primaryLightCount, operation);
        EndianSwap(value.primaryLights, operation);
        EndianSwap(value.waterHeader, operation);
        EndianSwap(value.numWaterCells, operation);
        EndianSwap(value.waterCells, operation);
        EndianSwap(value.burnableHeader, operation);
        EndianSwap(value.numBurnableCells, operation);
        EndianSwap(value.burnableCells, operation);
    }

    static inline void EndianSwap(pathbasenode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vOrigin, operation);
        EndianSwap(value.type, operation);
    }

    static inline void EndianSwap(pathlink_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.fDist, operation);
        EndianSwap(value.nodeNum, operation);
    }

    static inline void EndianSwap(pathnode_constant_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type, operation);
        EndianSwap(value.spawnflags, operation);
        EndianSwap(value.targetname, operation);
        EndianSwap(value.script_linkName, operation);
        EndianSwap(value.script_noteworthy, operation);
        EndianSwap(value.target, operation);
        EndianSwap(value.animscript, operation);
        EndianSwap(value.animscriptfunc, operation);
        EndianSwap(value.vOrigin, operation);
        EndianSwap(value.fAngle, operation);
        EndianSwap(value.forward, operation);
        EndianSwap(value.fRadius, operation);
        EndianSwap(value.minUseDistSq, operation);
        EndianSwap(value.wOverlapNode, operation);
        EndianSwap(value.wChainId, operation);
        EndianSwap(value.wChainDepth, operation);
        EndianSwap(value.wChainParent, operation);
        EndianSwap(value.totalLinkCount, operation);
        EndianSwap(value.Links, operation);
    }

    static inline void EndianSwap(pathnode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.constant, operation);

        EndianSwap(value.dynamic.pOwner.number, operation);
        EndianSwap(value.dynamic.pOwner.infoIndex, operation);
        EndianSwap(value.dynamic.iFreeTime, operation);
        EndianSwap(value.dynamic.iValidTime, operation);
        EndianSwap(value.dynamic.inPlayerLOSTime, operation);
        EndianSwap(value.dynamic.wLinkCount, operation);
        EndianSwap(value.dynamic.wOverlapCount, operation);
        EndianSwap(value.dynamic.turretEntNumber, operation);
        EndianSwap(value.dynamic.userCount, operation);

        EndianSwap(value.transient.iSearchFrame, operation);
        EndianSwap(value.transient.pNextOpen, operation);
        EndianSwap(value.transient.pPrevOpen, operation);
        EndianSwap(value.transient.pParent, operation);
        EndianSwap(value.transient.fCost, operation);
        EndianSwap(value.transient.fHeuristic, operation);
        EndianSwap(value.transient.costFactor, operation);
    }

    static inline void EndianSwap(pathnode_tree_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.axis, operation);
        EndianSwap(value.dist, operation);

        if (value.axis < 0)
        {
            EndianSwap(value.u.s.nodeCount, operation);
            EndianSwap(value.u.s.nodes, operation);
        }
        else
        {
            EndianSwap(value.u.child, operation);
        }
    }

    static inline void EndianSwap(PathData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount, operation);
        EndianSwap(value.nodes, operation);
        EndianSwap(value.basenodes, operation);
        EndianSwap(value.chainNodeCount, operation);
        EndianSwap(value.chainNodeForNode, operation);
        EndianSwap(value.nodeForChainNode, operation);
        EndianSwap(value.visBytes, operation);
        EndianSwap(value.pathVis, operation);
        EndianSwap(value.nodeTreeCount, operation);
        EndianSwap(value.nodeTree, operation);
    }

    static inline void EndianSwap(pathnode_tree_info_t& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(pathnode_tree_nodes_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount, operation);
        EndianSwap(value.nodes, operation);
    }

    static inline void EndianSwap(GameWorldSp& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.path, operation);
    }

    static inline void EndianSwap(GameWorldMp& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.path, operation);
    }

    static inline void EndianSwap(XAnimNotifyInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.time, operation);
    }

    static inline void EndianSwap(XAnimDeltaPart& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.trans, operation);
        EndianSwap(value.quat, operation);
    }

    static inline void EndianSwap(XAnimPartTrans& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size, operation);
    }

    static inline void EndianSwap(XAnimPartTransFrames& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.size, operation);
        EndianSwap(value.frames._1, operation);
    }

    static inline void EndianSwap(XAnimDynamicFrames& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(XAnimDeltaPartQuat& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size, operation);
    }

    static inline void EndianSwap(XAnimDeltaPartQuatDataFrames& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames, operation);
    }

    static inline void EndianSwap(XAnimIndices& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(XAnimParts& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.dataByteCount, operation);
        EndianSwap(value.dataShortCount, operation);
        EndianSwap(value.dataIntCount, operation);
        EndianSwap(value.randomDataByteCount, operation);
        EndianSwap(value.randomDataIntCount, operation);
        EndianSwap(value.numframes, operation);
        EndianSwap(value.streamedFileSize, operation);
        EndianSwap(value.randomDataShortCount, operation);
        EndianSwap(value.indexCount, operation);
        EndianSwap(value.framerate, operation);
        EndianSwap(value.frequency, operation);
        EndianSwap(value.primedLength, operation);
        EndianSwap(value.loopEntryTime, operation);
        EndianSwap(value.names, operation);
        EndianSwap(value.dataByte, operation);
        EndianSwap(value.dataShort, operation);
        EndianSwap(value.dataInt, operation);
        EndianSwap(value.randomDataShort, operation);
        EndianSwap(value.randomDataByte, operation);
        EndianSwap(value.randomDataInt, operation);
        EndianSwap(value.indices.data, operation);
        EndianSwap(value.notify, operation);
        EndianSwap(value.deltaPart, operation);
    }

    static inline void EndianSwap(DObjAnimMat& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.quat, operation);
        EndianSwap(value.trans, operation);
        EndianSwap(value.transWeight, operation);
    }

    static inline void EndianSwap(GfxPackedVertex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xyz, operation);
        EndianSwap(value.binormalSign, operation);
        EndianSwap(value.color, operation);
        EndianSwap(value.texCoord, operation);
        EndianSwap(value.normal, operation);
        EndianSwap(value.tangent, operation);
    }

    static inline void EndianSwap(GfxPackedVertexQuant& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(XSurfaceCollisionAabb& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
    }

    static inline void EndianSwap(XSurfaceCollisionNode& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aabb, operation);
        EndianSwap(value.childBeginIndex, operation);
        EndianSwap(value.childCount, operation);
    }

    static inline void EndianSwap(XSurfaceCollisionLeaf& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.triangleBeginIndex, operation);
    }

    static inline void EndianSwap(XSurfaceCollisionTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.trans, operation);
        EndianSwap(value.scale, operation);
        EndianSwap(value.nodeCount, operation);
        EndianSwap(value.nodes, operation);
        EndianSwap(value.leafCount, operation);
        EndianSwap(value.leafs, operation);
    }

    static inline void EndianSwap(XRigidVertList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.boneOffset, operation);
        EndianSwap(value.vertCount, operation);
        EndianSwap(value.triOffset, operation);
        EndianSwap(value.triCount, operation);
        EndianSwap(value.collisionTree, operation);
    }

    static inline void EndianSwap(XSurfaceVertexInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertCount, operation);
        EndianSwap(value.vertsBlend, operation);
        EndianSwap(value.tensionData, operation);
    }

    static inline void EndianSwap(GfxPackedVertex0Union& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(XSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flags, operation);
        EndianSwap(value.vertCount, operation);
        EndianSwap(value.triCount, operation);
        EndianSwap(value.triIndices, operation);
        EndianSwap(value.vertInfo, operation);
        EndianSwap(value.verts0.data, operation);
        EndianSwap(value.vertList, operation);
        EndianSwap(value.partBits, operation);
    }

    static inline void EndianSwap(XModelLodInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dist, operation);
        EndianSwap(value.numsurfs, operation);
        EndianSwap(value.surfIndex, operation);
        EndianSwap(value.partBits, operation);
    }

    static inline void EndianSwap(XModelCollSurf_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.boneIdx, operation);
        EndianSwap(value.contents, operation);
        EndianSwap(value.surfFlags, operation);
    }

    static inline void EndianSwap(XBoneInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.bounds, operation);
        EndianSwap(value.offset, operation);
        EndianSwap(value.radiusSquared, operation);
    }

    static inline void EndianSwap(XModelHighMipBounds& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.center, operation);
        EndianSwap(value.himipRadiusSq, operation);
    }

    static inline void EndianSwap(XModelStreamInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.highMipBounds, operation);
    }

    static inline void EndianSwap(cplane_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.normal, operation);
        EndianSwap(value.dist, operation);
    }

    static inline void EndianSwap(cbrushside_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane, operation);
        EndianSwap(value.cflags, operation);
        EndianSwap(value.sflags, operation);
    }

    static inline void EndianSwap(vec3_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.v, operation);
    }

    static inline void EndianSwap(cStaticModel_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.writable.nextModelInWorldSector, operation);
        EndianSwap(value.xmodel, operation);
        EndianSwap(value.origin, operation);
        EndianSwap(value.invScaledAxis, operation);
        EndianSwap(value.absmin, operation);
        EndianSwap(value.absmax, operation);
    }

    static inline void EndianSwap(dmaterial_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfaceFlags, operation);
        EndianSwap(value.contentFlags, operation);
    }

    static inline void EndianSwap(cNode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane, operation);
        EndianSwap(value.children, operation);
    }

    static inline void EndianSwap(cLeaf_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstCollAabbIndex, operation);
        EndianSwap(value.collAabbCount, operation);
        EndianSwap(value.brushContents, operation);
        EndianSwap(value.terrainContents, operation);
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.leafBrushNode, operation);
        EndianSwap(value.cluster, operation);
    }

    static inline void EndianSwap(cLeafBrushNodeLeaf_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.brushes, operation);
    }

    static inline void EndianSwap(cLeafBrushNodeData_t& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(cLeafBrushNode_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto leafBrushCount = EndianSwapDiscriminator(value.leafBrushCount, operation);
        EndianSwap(value.contents, operation);

        if (leafBrushCount > 0)
        {
            EndianSwap(value.data.leaf.brushes, operation);
        }
        else
        {
            EndianSwap(value.data.children.dist, operation);
            EndianSwap(value.data.children.range, operation);
            EndianSwap(value.data.children.childOffset, operation);
        }
    }

    static inline void EndianSwap(CollisionBorder& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.distEq, operation);
        EndianSwap(value.zBase, operation);
        EndianSwap(value.zSlope, operation);
        EndianSwap(value.start, operation);
        EndianSwap(value.length, operation);
    }

    static inline void EndianSwap(CollisionPartition& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstTri, operation);
        EndianSwap(value.nuinds, operation);
        EndianSwap(value.fuind, operation);
        EndianSwap(value.borders, operation);
    }

    static inline void EndianSwap(CollisionAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin, operation);
        EndianSwap(value.materialIndex, operation);
        EndianSwap(value.childCount, operation);
        EndianSwap(value.halfSize, operation);
        EndianSwap(value.u.firstChildIndex, operation);
    }

    static inline void EndianSwap(cmodel_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.leaf, operation);
    }

    static inline void EndianSwap(cbrush_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.contents, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.numsides, operation);
        EndianSwap(value.sides, operation);
        EndianSwap(value.axial_cflags, operation);
        EndianSwap(value.axial_sflags, operation);
        EndianSwap(value.numverts, operation);
        EndianSwap(value.verts, operation);
    }

    static inline void EndianSwap(XModelPiece& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model, operation);
        EndianSwap(value.offset, operation);
    }

    static inline void EndianSwap(XModelPieces& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.numpieces, operation);
        EndianSwap(value.pieces, operation);
    }

    static inline void EndianSwap(DynEntityDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type, operation);
        EndianSwap(value.pose.quat, operation);
        EndianSwap(value.pose.origin, operation);
        EndianSwap(value.xModel, operation);
        EndianSwap(value.destroyedxModel, operation);
        EndianSwap(value.brushModel, operation);
        EndianSwap(value.physicsBrushModel, operation);
        EndianSwap(value.destroyFx, operation);
        EndianSwap(value.destroySound, operation);
        EndianSwap(value.destroyPieces, operation);
        EndianSwap(value.physPreset, operation);
        EndianSwap(value.physConstraints, operation);
        EndianSwap(value.health, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.contents, operation);
        EndianSwap(value.targetname, operation);
        EndianSwap(value.target, operation);
    }

    static inline void EndianSwap(DynEntityPose& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pose.quat, operation);
        EndianSwap(value.pose.origin, operation);
        EndianSwap(value.radius, operation);
    }

    static inline void EndianSwap(DynEntityClient& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.physObjId, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.lightingHandle, operation);
        EndianSwap(value.health, operation);
        EndianSwap(value.burnTime, operation);
        EndianSwap(value.fadeTime, operation);
        EndianSwap(value.physicsStartTime, operation);
    }

    static inline void EndianSwap(DynEntityServer& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flags, operation);
        EndianSwap(value.health, operation);
    }

    static inline void EndianSwap(DynEntityColl& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.sector, operation);
        EndianSwap(value.nextEntInSector, operation);
        EndianSwap(value.linkMins, operation);
        EndianSwap(value.linkMaxs, operation);
        EndianSwap(value.contents, operation);
    }

    static inline void EndianSwap(par_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.p, operation);
        EndianSwap(value.p0, operation);
        EndianSwap(value.p_prev, operation);
        EndianSwap(value.flags, operation);
    }

    static inline void EndianSwap(constraint_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.p, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.entity_index, operation);
        EndianSwap(value.bone_name_hash, operation);
    }

    static inline void EndianSwap(rope_frame_verts_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.num_verts, operation);
        EndianSwap(value.v, operation);
    }

    static inline void EndianSwap(rope_client_verts_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        for (auto& frame : value.frame_verts)
            EndianSwap(frame, operation);
        EndianSwap(value.frame_index, operation);
    }

    static inline void EndianSwap(rope_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        for (auto& particle : value.m_particles)
            EndianSwap(particle, operation);
        for (auto& constraint : value.m_constraints)
            EndianSwap(constraint, operation);
        EndianSwap(value.m_entity_anchors, operation);
        EndianSwap(value.m_num_particles, operation);
        EndianSwap(value.m_num_constraints, operation);
        EndianSwap(value.m_num_entity_anchors, operation);
        EndianSwap(value.m_num_draw_verts, operation);
        EndianSwap(value.m_client_verts, operation);
        EndianSwap(value.m_min, operation);
        EndianSwap(value.m_max, operation);
        EndianSwap(value.m_start, operation);
        EndianSwap(value.m_end, operation);
        EndianSwap(value.m_in_use, operation);
        EndianSwap(value.m_visible, operation);
        EndianSwap(value.m_dist_constraint, operation);
        EndianSwap(value.m_flags, operation);
        EndianSwap(value.m_material, operation);
        EndianSwap(value.m_seglen, operation);
        EndianSwap(value.m_length, operation);
        EndianSwap(value.m_width, operation);
        EndianSwap(value.m_scale, operation);
        EndianSwap(value.m_force_scale, operation);
        EndianSwap(value.m_health, operation);
        EndianSwap(value.m_frame, operation);
        EndianSwap(value.m_stable_count, operation);
        EndianSwap(value.m_static_rope, operation);
        EndianSwap(value.m_lightingHandle, operation);
    }

    static inline void EndianSwap(clipMap_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.isInUse, operation);
        EndianSwap(value.planeCount, operation);
        EndianSwap(value.planes, operation);
        EndianSwap(value.numStaticModels, operation);
        EndianSwap(value.staticModelList, operation);
        EndianSwap(value.numMaterials, operation);
        EndianSwap(value.materials, operation);
        EndianSwap(value.numBrushSides, operation);
        EndianSwap(value.brushsides, operation);
        EndianSwap(value.numNodes, operation);
        EndianSwap(value.nodes, operation);
        EndianSwap(value.numLeafs, operation);
        EndianSwap(value.leafs, operation);
        EndianSwap(value.leafbrushNodesCount, operation);
        EndianSwap(value.leafbrushNodes, operation);
        EndianSwap(value.numLeafBrushes, operation);
        EndianSwap(value.leafbrushes, operation);
        EndianSwap(value.numLeafSurfaces, operation);
        EndianSwap(value.leafsurfaces, operation);
        EndianSwap(value.vertCount, operation);
        EndianSwap(value.verts, operation);
        EndianSwap(value.numBrushVerts, operation);
        EndianSwap(value.brushVerts, operation);
        EndianSwap(value.nuinds, operation);
        EndianSwap(value.uinds, operation);
        EndianSwap(value.triCount, operation);
        EndianSwap(value.triIndices, operation);
        EndianSwap(value.triEdgeIsWalkable, operation);
        EndianSwap(value.borderCount, operation);
        EndianSwap(value.borders, operation);
        EndianSwap(value.partitionCount, operation);
        EndianSwap(value.partitions, operation);
        EndianSwap(value.aabbTreeCount, operation);
        EndianSwap(value.aabbTrees, operation);
        EndianSwap(value.numSubModels, operation);
        EndianSwap(value.cmodels, operation);
        EndianSwap(value.numBrushes, operation);
        EndianSwap(value.brushes, operation);
        EndianSwap(value.numClusters, operation);
        EndianSwap(value.clusterBytes, operation);
        EndianSwap(value.visibility, operation);
        EndianSwap(value.vised, operation);
        EndianSwap(value.mapEnts, operation);
        EndianSwap(value.box_brush, operation);
        EndianSwap(value.box_model, operation);
        EndianSwap(value.originalDynEntCount, operation);
        EndianSwap(value.dynEntCount, operation);
        EndianSwap(value.dynEntDefList, operation);
        EndianSwap(value.dynEntPoseList, operation);
        EndianSwap(value.dynEntClientList, operation);
        EndianSwap(value.dynEntServerList, operation);
        EndianSwap(value.dynEntCollList, operation);
        EndianSwap(value.num_constraints, operation);
        EndianSwap(value.constraints, operation);
        EndianSwap(value.max_ropes, operation);
        EndianSwap(value.ropes, operation);
        EndianSwap(value.checksum, operation);
    }

    static inline void EndianSwap(MapEnts& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.entityString, operation);
        EndianSwap(value.numEntityChars, operation);
    }

    static inline void EndianSwap(BrushWrapper& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.contents, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.numsides, operation);
        EndianSwap(value.sides, operation);
        EndianSwap(value.axialCFlags, operation);
        EndianSwap(value.axialSFlags, operation);
        EndianSwap(value.numverts, operation);
        EndianSwap(value.verts, operation);
        EndianSwap(value.planes, operation);
    }

    static inline void EndianSwap(PhysGeomInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.brush, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.orientation, operation);
        EndianSwap(value.offset, operation);
        EndianSwap(value.halfLengths, operation);
    }

    static inline void EndianSwap(PhysGeomList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.count, operation);
        EndianSwap(value.geoms, operation);
        EndianSwap(value.contents, operation);
    }

    static inline void EndianSwap(Collmap& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.geomList, operation);
    }

    static inline void EndianSwap(XModelQuat& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.v, operation);
    }

    static inline void EndianSwap(XModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.boneNames, operation);
        EndianSwap(value.parentList, operation);
        EndianSwap(value.quats, operation);
        EndianSwap(value.trans, operation);
        EndianSwap(value.partClassification, operation);
        EndianSwap(value.baseMat, operation);
        EndianSwap(value.surfs, operation);
        EndianSwap(value.materialHandles, operation);
        for (auto& lodInfo : value.lodInfo)
            EndianSwap(lodInfo, operation);
        EndianSwap(value.collSurfs, operation);
        EndianSwap(value.numCollSurfs, operation);
        EndianSwap(value.contents, operation);
        EndianSwap(value.boneInfo, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.numLods, operation);
        EndianSwap(value.collLod, operation);
        EndianSwap(value.streamInfo, operation);
        EndianSwap(value.memUsage, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.physPreset, operation);
        EndianSwap(value.collmaps, operation);
        EndianSwap(value.physConstraints, operation);
    }

    static inline void EndianSwap(snd_asset& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.version, operation);
        EndianSwap(value.frame_count, operation);
        EndianSwap(value.frame_rate, operation);
        EndianSwap(value.channel_count, operation);
        EndianSwap(value.header_size, operation);
        EndianSwap(value.block_size, operation);
        EndianSwap(value.buffer_size, operation);
        EndianSwap(value.format, operation);
        EndianSwap(value.channel_flags, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.seek_table_count, operation);
        EndianSwap(value.seek_table, operation);
        EndianSwap(value.data_size, operation);
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(LoadedSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.sound, operation);
    }

    static inline void EndianSwap(PrimedSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.buffer, operation);
        EndianSwap(value.size, operation);
    }

    static inline void EndianSwap(StreamedSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.filename, operation);
        EndianSwap(value.primeSnd, operation);
    }

    static inline void EndianSwap(SoundFileRef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadSnd, operation);
    }

    static inline void EndianSwap(SoundFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.u, operation);
    }

    static inline void EndianSwap(snd_alias_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.id, operation);
        EndianSwap(value.subtitle, operation);
        EndianSwap(value.secondaryname, operation);
        EndianSwap(value.soundFile, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.duck, operation);
        EndianSwap(value.contextType, operation);
        EndianSwap(value.contextValue, operation);
        EndianSwap(value.fluxTime, operation);
        EndianSwap(value.startDelay, operation);
        EndianSwap(value.reverbSend, operation);
        EndianSwap(value.centerSend, operation);
        EndianSwap(value.volMin, operation);
        EndianSwap(value.volMax, operation);
        EndianSwap(value.teamVolMod, operation);
        EndianSwap(value.pitchMin, operation);
        EndianSwap(value.pitchMax, operation);
        EndianSwap(value.teamPitchMod, operation);
        EndianSwap(value.distMin, operation);
        EndianSwap(value.distMax, operation);
        EndianSwap(value.distReverbMax, operation);
        EndianSwap(value.envelopMin, operation);
        EndianSwap(value.envelopMax, operation);
        EndianSwap(value.envelopPercentage, operation);
    }

    static inline void EndianSwap(snd_alias_list_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.id, operation);
        EndianSwap(value.head, operation);
        EndianSwap(value.count, operation);
        EndianSwap(value.sequence, operation);
    }

    static inline void EndianSwap(SndIndexEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.value, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(snd_radverb& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.smoothing, operation);
        EndianSwap(value.earlyTime, operation);
        EndianSwap(value.lateTime, operation);
        EndianSwap(value.earlyGain, operation);
        EndianSwap(value.lateGain, operation);
        EndianSwap(value.returnGain, operation);
        EndianSwap(value.earlyLpf, operation);
        EndianSwap(value.lateLpf, operation);
        EndianSwap(value.inputLpf, operation);
        EndianSwap(value.dampLpf, operation);
        EndianSwap(value.wallReflect, operation);
        EndianSwap(value.dryGain, operation);
        EndianSwap(value.earlySize, operation);
        EndianSwap(value.lateSize, operation);
        EndianSwap(value.diffusion, operation);
    }

    static inline void EndianSwap(snd_snapshot& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.occlusionId, operation);
        EndianSwap(value.fadeIn, operation);
        EndianSwap(value.fadeOut, operation);
        EndianSwap(value.distance, operation);
        EndianSwap(value.fadeInCurve, operation);
        EndianSwap(value.fadeOutCurve, operation);
        EndianSwap(value.attenuation, operation);
    }

    static inline void EndianSwap(SndBank& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.aliasCount, operation);
        EndianSwap(value.alias, operation);
        EndianSwap(value.aliasIndex, operation);
        EndianSwap(value.packHash, operation);
        EndianSwap(value.packLocation, operation);
        EndianSwap(value.radverbCount, operation);
        EndianSwap(value.radverbs, operation);
        EndianSwap(value.snapshotCount, operation);
        EndianSwap(value.snapshots, operation);
    }

    static inline void EndianSwap(SndPatch& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.elementCount, operation);
        EndianSwap(value.elements, operation);
        EndianSwap(value.fileCount, operation);
        EndianSwap(value.files, operation);
    }

    static inline void EndianSwap(snd_group& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.parentIndex, operation);
        EndianSwap(value.category, operation);
        EndianSwap(value.attenuationSp, operation);
        EndianSwap(value.attenuationMp, operation);
    }

    static inline void EndianSwap(snd_curve& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.points, operation);
    }

    static inline void EndianSwap(snd_pan& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.front, operation);
        EndianSwap(value.back, operation);
        EndianSwap(value.center, operation);
        EndianSwap(value.lfe, operation);
        EndianSwap(value.left, operation);
        EndianSwap(value.right, operation);
    }

    static inline void EndianSwap(snd_snapshot_group& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(snd_context& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type, operation);
        EndianSwap(value.valueCount, operation);
        EndianSwap(value.values, operation);
    }

    static inline void EndianSwap(snd_master& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.id, operation);
        EndianSwap(value.params, operation);
    }

    static inline void EndianSwap(SndDriverGlobals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.groupCount, operation);
        EndianSwap(value.groups, operation);
        EndianSwap(value.curveCount, operation);
        EndianSwap(value.curves, operation);
        EndianSwap(value.panCount, operation);
        EndianSwap(value.pans, operation);
        EndianSwap(value.snapshotGroupCount, operation);
        EndianSwap(value.snapshotGroups, operation);
        EndianSwap(value.contextCount, operation);
        EndianSwap(value.contexts, operation);
        EndianSwap(value.masterCount, operation);
        EndianSwap(value.masters, operation);
    }

    static inline void EndianSwap(Glyph& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.letter, operation);
        EndianSwap(value.s0, operation);
        EndianSwap(value.t0, operation);
        EndianSwap(value.s1, operation);
        EndianSwap(value.t1, operation);
    }

    static inline void EndianSwap(Font_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.fontName, operation);
        EndianSwap(value.pixelHeight, operation);
        EndianSwap(value.glyphCount, operation);
        EndianSwap(value.material, operation);
        EndianSwap(value.glowMaterial, operation);
        EndianSwap(value.glyphs, operation);
    }

    static inline void EndianSwap(MenuList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.menuCount, operation);
        EndianSwap(value.menus, operation);
    }

    static inline void EndianSwap(rectDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.x, operation);
        EndianSwap(value.y, operation);
        EndianSwap(value.w, operation);
        EndianSwap(value.h, operation);
        EndianSwap(value.horzAlign, operation);
        EndianSwap(value.vertAlign, operation);
    }

    static inline void EndianSwap(windowDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.rect, operation);
        EndianSwap(value.rectClient, operation);
        EndianSwap(value.group, operation);
        EndianSwap(value.frameTexSize, operation);
        EndianSwap(value.frameSize, operation);
        EndianSwap(value.ownerDraw, operation);
        EndianSwap(value.ownerDrawFlags, operation);
        EndianSwap(value.borderSize, operation);
        EndianSwap(value.staticFlags, operation);
        EndianSwap(value.dynamicFlags, operation);
        EndianSwap(value.nextTime, operation);
        EndianSwap(value.foreColor, operation);
        EndianSwap(value.backColor, operation);
        EndianSwap(value.borderColor, operation);
        EndianSwap(value.outlineColor, operation);
        EndianSwap(value.rotation, operation);
        EndianSwap(value.background, operation);
    }

    static inline void EndianSwap(ScriptCondition& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.constructID, operation);
        EndianSwap(value.blockID, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(operandInternalDataUnion& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.intVal, operation);
    }

    static inline void EndianSwap(Operand& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto dataType = EndianSwapDiscriminator(value.dataType, operation);

        switch (dataType)
        {
        case T5Xenon::VAL_INT:
            EndianSwap(value.internals.intVal, operation);
            break;

        case T5Xenon::VAL_FLOAT:
            EndianSwap(value.internals.floatVal, operation);
            break;

        case T5Xenon::VAL_STRING:
            EndianSwap(value.internals.string, operation);
            break;

        default:
            assert(false);
            break;
        }
    }

    // expressionRpn selects the union member before swapping.
    static inline void EndianSwap(expressionRpnDataUnion&, const EndianOperation) {}

    static inline void EndianSwap(expressionRpn& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(value.type, operation);

        switch (type)
        {
        case T5Xenon::RPN_CONSTANT:
            EndianSwap(value.data.constant, operation);
            break;

        case T5Xenon::RPN_CMD_IDX:
            EndianSwap(value.data.cmdIdx, operation);
            break;

        case T5Xenon::RPN_CMD:
            EndianSwap(value.data.cmd, operation);
            break;

        case T5Xenon::RPN_END:
            break;

        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(ExpressionStatement& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.filename, operation);
        EndianSwap(value.line, operation);
        EndianSwap(value.numRpn, operation);
        EndianSwap(value.rpn, operation);
    }

    static inline void EndianSwap(GenericEventScript& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.prerequisites, operation);
        EndianSwap(value.condition, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.action, operation);
        EndianSwap(value.blockID, operation);
        EndianSwap(value.constructID, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(GenericEventHandler& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.eventScript, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(ItemKeyHandler& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.key, operation);
        EndianSwap(value.keyScript, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(textExp_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.textExp, operation);
    }

    static inline void EndianSwap(enumDvarDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.enumDvarName, operation);
    }

    static inline void EndianSwap(editFieldDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cursorPos, operation);
        EndianSwap(value.minVal, operation);
        EndianSwap(value.maxVal, operation);
        EndianSwap(value.defVal, operation);
        EndianSwap(value.range, operation);
        EndianSwap(value.maxChars, operation);
        EndianSwap(value.maxCharsGotoNext, operation);
        EndianSwap(value.maxPaintChars, operation);
        EndianSwap(value.paintOffset, operation);
    }

    static inline void EndianSwap(columnInfo_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.elementStyle, operation);
        EndianSwap(value.maxChars, operation);
        EndianSwap(value.rect, operation);
    }

    static inline void EndianSwap(MenuCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type, operation);
        EndianSwap(value.maxChars, operation);
        EndianSwap(value.stringValue, operation);
    }

    static inline void EndianSwap(MenuRow& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cells, operation);
        EndianSwap(value.eventName, operation);
        EndianSwap(value.onFocusEventName, operation);
        EndianSwap(value.status, operation);
        EndianSwap(value.name, operation);
    }

    static inline void EndianSwap(listBoxDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cursorPos, operation);
        EndianSwap(value.startPos, operation);
        EndianSwap(value.endPos, operation);
        EndianSwap(value.drawPadding, operation);
        EndianSwap(value.elementWidth, operation);
        EndianSwap(value.elementHeight, operation);
        EndianSwap(value.numColumns, operation);
        EndianSwap(value.special, operation);
        for (auto& column : value.columnInfo)
            EndianSwap(column, operation);
        EndianSwap(value.notselectable, operation);
        EndianSwap(value.noScrollBars, operation);
        EndianSwap(value.usePaging, operation);
        EndianSwap(value.selectBorder, operation);
        EndianSwap(value.disableColor, operation);
        EndianSwap(value.focusColor, operation);
        EndianSwap(value.elementHighlightColor, operation);
        EndianSwap(value.elementBackgroundColor, operation);
        EndianSwap(value.selectIcon, operation);
        EndianSwap(value.backgroundItemListbox, operation);
        EndianSwap(value.highlightTexture, operation);
        EndianSwap(value.noBlinkingHighlight, operation);
        EndianSwap(value.rows, operation);
        EndianSwap(value.maxRows, operation);
        EndianSwap(value.rowCount, operation);
    }

    static inline void EndianSwap(multiDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dvarList, operation);
        EndianSwap(value.dvarStr, operation);
        EndianSwap(value.dvarValue, operation);
        EndianSwap(value.count, operation);
        EndianSwap(value.actionOnEnterPressOnly, operation);
        EndianSwap(value.strDef, operation);
    }

    static inline void EndianSwap(focusDefData_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(focusItemDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.onKey, operation);
        EndianSwap(value.focusTypeData.data, operation);
    }

    static inline void EndianSwap(gameMsgDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.gameMsgWindowIndex, operation);
        EndianSwap(value.gameMsgWindowMode, operation);
    }

    static inline void EndianSwap(textDefData_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(textDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        for (auto& rect : value.textRect)
            EndianSwap(rect, operation);
        EndianSwap(value.alignment, operation);
        EndianSwap(value.fontEnum, operation);
        EndianSwap(value.itemFlags, operation);
        EndianSwap(value.textAlignMode, operation);
        EndianSwap(value.textalignx, operation);
        EndianSwap(value.textaligny, operation);
        EndianSwap(value.textscale, operation);
        EndianSwap(value.textStyle, operation);
        EndianSwap(value.text, operation);
        EndianSwap(value.textExpData, operation);
        EndianSwap(value.textTypeData.data, operation);
    }

    static inline void EndianSwap(imageDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.materialExp, operation);
    }

    static inline void EndianSwap(ownerDrawDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dataExp, operation);
    }

    static inline void EndianSwap(itemDefData_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(rectData_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.rectXExp, operation);
        EndianSwap(value.rectYExp, operation);
        EndianSwap(value.rectWExp, operation);
        EndianSwap(value.rectHExp, operation);
    }

    static inline void EndianSwap(animParamsDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.rectClient, operation);
        EndianSwap(value.borderSize, operation);
        EndianSwap(value.foreColor, operation);
        EndianSwap(value.backColor, operation);
        EndianSwap(value.borderColor, operation);
        EndianSwap(value.outlineColor, operation);
        EndianSwap(value.textScale, operation);
        EndianSwap(value.rotation, operation);
        EndianSwap(value.onEvent, operation);
    }

    static inline void EndianSwap(UIAnimInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.animStateCount, operation);
        EndianSwap(value.animStates, operation);
        EndianSwap(value.animating, operation);
        EndianSwap(value.animStartTime, operation);
        EndianSwap(value.animDuration, operation);
    }

    static inline void EndianSwap(itemDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.dataType, operation);
        EndianSwap(value.imageTrack, operation);
        EndianSwap(value.dvar, operation);
        EndianSwap(value.dvarTest, operation);
        EndianSwap(value.enableDvar, operation);
        EndianSwap(value.dvarFlags, operation);
        EndianSwap(value.typeData.data, operation);
        EndianSwap(value.parent, operation);
        EndianSwap(value.rectExpData, operation);
        EndianSwap(value.visibleExp, operation);
        EndianSwap(value.showBits, operation);
        EndianSwap(value.hideBits, operation);
        EndianSwap(value.forecolorAExp, operation);
        EndianSwap(value.ui3dWindowId, operation);
        EndianSwap(value.onEvent, operation);
        EndianSwap(value.animInfo, operation);
    }

    static inline void EndianSwap(menuDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);
        EndianSwap(value.font, operation);
        EndianSwap(value.fullScreen, operation);
        EndianSwap(value.ui3dWindowId, operation);
        EndianSwap(value.itemCount, operation);
        EndianSwap(value.fontIndex, operation);
        EndianSwap(value.cursorItem, operation);
        EndianSwap(value.fadeCycle, operation);
        EndianSwap(value.priority, operation);
        EndianSwap(value.fadeClamp, operation);
        EndianSwap(value.fadeAmount, operation);
        EndianSwap(value.fadeInAmount, operation);
        EndianSwap(value.blurRadius, operation);
        EndianSwap(value.openSlideSpeed, operation);
        EndianSwap(value.closeSlideSpeed, operation);
        EndianSwap(value.openSlideDirection, operation);
        EndianSwap(value.closeSlideDirection, operation);
        EndianSwap(value.initialRectInfo, operation);
        EndianSwap(value.openFadingTime, operation);
        EndianSwap(value.closeFadingTime, operation);
        EndianSwap(value.fadeTimeCounter, operation);
        EndianSwap(value.slideTimeCounter, operation);
        EndianSwap(value.onEvent, operation);
        EndianSwap(value.onKey, operation);
        EndianSwap(value.visibleExp, operation);
        EndianSwap(value.showBits, operation);
        EndianSwap(value.hideBits, operation);
        EndianSwap(value.allowedBinding, operation);
        EndianSwap(value.soundName, operation);
        EndianSwap(value.imageTrack, operation);
        EndianSwap(value.control, operation);
        EndianSwap(value.focusColor, operation);
        EndianSwap(value.disableColor, operation);
        EndianSwap(value.rectXExp, operation);
        EndianSwap(value.rectYExp, operation);
        EndianSwap(value.items, operation);
    }

    static inline void EndianSwap(FxIntRange& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base, operation);
        EndianSwap(value.amplitude, operation);
    }

    static inline void EndianSwap(FxFloatRange& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base, operation);
        EndianSwap(value.amplitude, operation);
    }

    static inline void EndianSwap(FxElemVec3Range& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base, operation);
        EndianSwap(value.amplitude, operation);
    }

    static inline void EndianSwap(FxSpawnDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.looping.intervalMsec, operation);
        EndianSwap(value.looping.count, operation);
    }

    static inline void EndianSwap(FxElemAtlas& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.entryCountAndIndexRange, operation);
    }

    static inline void EndianSwap(FxElemVelStateInFrame& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.velocity, operation);
        EndianSwap(value.totalDelta, operation);
    }

    static inline void EndianSwap(FxElemVelStateSample& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.local, operation);
        EndianSwap(value.world, operation);
    }

    static inline void EndianSwap(FxElemVisualState& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.rotationDelta, operation);
        EndianSwap(value.rotationTotal, operation);
        EndianSwap(value.size, operation);
        EndianSwap(value.scale, operation);
    }

    static inline void EndianSwap(FxElemVisStateSample& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base, operation);
        EndianSwap(value.amplitude, operation);
    }

    static inline void EndianSwap(FxElemMarkVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.materials, operation);
    }

    static inline void EndianSwap(FxEffectDefRef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
    }

    static inline void EndianSwap(FxElemVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.anonymous, operation);
    }

    static inline void EndianSwap(FxElemDefVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.markArray, operation);
    }

    static inline void EndianSwap(FxTrailVertex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pos, operation);
        EndianSwap(value.normal, operation);
        EndianSwap(value.texCoord, operation);
    }

    static inline void EndianSwap(FxTrailDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.scrollTimeMsec, operation);
        EndianSwap(value.repeatDist, operation);
        EndianSwap(value.splitDist, operation);
        EndianSwap(value.vertCount, operation);
        EndianSwap(value.verts, operation);
        EndianSwap(value.indCount, operation);
        EndianSwap(value.inds, operation);
    }

    static inline void EndianSwap(FxElemDefUnion& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cloudDensityRange, operation);
    }

    static inline void EndianSwap(FxElemSpawnSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.spawnSound, operation);
    }

    static inline void EndianSwap(FxElemDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flags, operation);
        EndianSwap(value.spawn, operation);
        EndianSwap(value.spawnRange, operation);
        EndianSwap(value.fadeInRange, operation);
        EndianSwap(value.fadeOutRange, operation);
        EndianSwap(value.spawnFrustumCullRadius, operation);
        EndianSwap(value.spawnDelayMsec, operation);
        EndianSwap(value.lifeSpanMsec, operation);
        for (auto& range : value.spawnOrigin)
            EndianSwap(range, operation);
        EndianSwap(value.spawnOffsetRadius, operation);
        EndianSwap(value.spawnOffsetHeight, operation);
        for (auto& range : value.spawnAngles)
            EndianSwap(range, operation);
        for (auto& range : value.angularVelocity)
            EndianSwap(range, operation);
        EndianSwap(value.initialRotation, operation);
        EndianSwap(value.rotationAxis, operation);
        EndianSwap(value.gravity, operation);
        EndianSwap(value.reflectionFactor, operation);
        EndianSwap(value.atlas, operation);
        EndianSwap(value.windInfluence, operation);
        EndianSwap(value.velSamples, operation);
        EndianSwap(value.visSamples, operation);
        EndianSwap(value.visuals, operation);
        EndianSwap(value.collMins, operation);
        EndianSwap(value.collMaxs, operation);
        EndianSwap(value.effectOnImpact, operation);
        EndianSwap(value.effectOnDeath, operation);
        EndianSwap(value.effectEmitted, operation);
        EndianSwap(value.emitDist, operation);
        EndianSwap(value.emitDistVariance, operation);
        EndianSwap(value.effectAttached, operation);
        EndianSwap(value.trailDef, operation);
        EndianSwap(value.alphaFadeTimeMsec, operation);
        EndianSwap(value.maxWindStrength, operation);
        EndianSwap(value.spawnIntervalAtMaxWind, operation);
        EndianSwap(value.lifespanAtMaxWind, operation);
        EndianSwap(value.u, operation);
        EndianSwap(value.spawnSound, operation);
        EndianSwap(value.billboardPivot, operation);
    }

    static inline void EndianSwap(FxEffectDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.totalSize, operation);
        EndianSwap(value.msecLoopingLife, operation);
        EndianSwap(value.elemDefCountLooping, operation);
        EndianSwap(value.elemDefCountOneShot, operation);
        EndianSwap(value.elemDefCountEmission, operation);
        EndianSwap(value.elemDefs, operation);
        EndianSwap(value.boundingBoxDim, operation);
        EndianSwap(value.boundingSphere, operation);
    }

    static inline void EndianSwap(FxImpactEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nonflesh, operation);
        EndianSwap(value.flesh, operation);
    }

    static inline void EndianSwap(FxImpactTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.table, operation);
    }

    static inline void EndianSwap(StringTableCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.string, operation);
        EndianSwap(value.hash, operation);
    }

    static inline void EndianSwap(StringTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.columnCount, operation);
        EndianSwap(value.rowCount, operation);
        EndianSwap(value.values, operation);
        EndianSwap(value.cellIndex, operation);
    }

    static inline void EndianSwap(PackIndex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.packIndex, operation);
        EndianSwap(value.packType, operation);
    }

    static inline void EndianSwap(XGlobals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.xanimStreamBufferSize, operation);
        EndianSwap(value.cinematicMaxWidth, operation);
        EndianSwap(value.cinematicMaxHeight, operation);
        EndianSwap(value.extracamResolution, operation);
        EndianSwap(value.gumpReserve, operation);
        EndianSwap(value.screenClearColor, operation);
    }

    static inline void EndianSwap(TextureListElement& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.texture, operation);
    }

    static inline void EndianSwap(TextureList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.count, operation);
        EndianSwap(value.elements, operation);
    }

    static inline void EndianSwap(ddlMemberDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.size, operation);
        EndianSwap(value.offset, operation);
        EndianSwap(value.type, operation);
        EndianSwap(value.externalIndex, operation);
        EndianSwap(value.min, operation);
        EndianSwap(value.max, operation);
        EndianSwap(value.serverDelta, operation);
        EndianSwap(value.clientDelta, operation);
        EndianSwap(value.arraySize, operation);
        EndianSwap(value.enumIndex, operation);
        EndianSwap(value.permission, operation);
    }

    static inline void EndianSwap(ddlStructDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.size, operation);
        EndianSwap(value.memberCount, operation);
        EndianSwap(value.members, operation);
    }

    static inline void EndianSwap(ddlEnumDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.memberCount, operation);
        EndianSwap(value.members, operation);
    }

    static inline void EndianSwap(ddlDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.version, operation);
        EndianSwap(value.size, operation);
        EndianSwap(value.structList, operation);
        EndianSwap(value.structCount, operation);
        EndianSwap(value.enumList, operation);
        EndianSwap(value.enumCount, operation);
        EndianSwap(value.next, operation);
    }

    static inline void EndianSwap(ddlRoot_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.ddlDef, operation);
    }

    static inline void EndianSwap(GlassDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.maxHealth, operation);
        EndianSwap(value.thickness, operation);
        EndianSwap(value.minShardSize, operation);
        EndianSwap(value.maxShardSize, operation);
        EndianSwap(value.shardLifeProbablility, operation);
        EndianSwap(value.maxShards, operation);
        EndianSwap(value.pristineMaterial, operation);
        EndianSwap(value.crackedMaterial, operation);
        EndianSwap(value.shardMaterial, operation);
        EndianSwap(value.crackSound, operation);
        EndianSwap(value.shatterShound, operation);
        EndianSwap(value.autoShatterShound, operation);
        EndianSwap(value.crackEffect, operation);
        EndianSwap(value.shatterEffect, operation);
    }

    static inline void EndianSwap(Glass& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.glassDef, operation);
        EndianSwap(value.index, operation);
        EndianSwap(value.brushModel, operation);
        EndianSwap(value.origin, operation);
        EndianSwap(value.angles, operation);
        EndianSwap(value.absmin, operation);
        EndianSwap(value.absmax, operation);
        EndianSwap(value.outline, operation);
        for (auto& axis : value.outlineAxis)
            EndianSwap(axis, operation);
        EndianSwap(value.outlineOrigin, operation);
        EndianSwap(value.uvScale, operation);
        EndianSwap(value.thickness, operation);
    }

    static inline void EndianSwap(Glasses& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.numGlasses, operation);
        EndianSwap(value.glasses, operation);
        EndianSwap(value.workMemory, operation);
        EndianSwap(value.workMemorySize, operation);
        EndianSwap(value.smallAllocatorBlocks, operation);
        EndianSwap(value.maxGroups, operation);
        EndianSwap(value.maxShards, operation);
        EndianSwap(value.maxPhysics, operation);
        EndianSwap(value.shardMemorySize, operation);
        EndianSwap(value.maxFreeCmd, operation);
        EndianSwap(value.numSlots, operation);
        EndianSwap(value.numVerts, operation);
        EndianSwap(value.numIndices, operation);
    }

    static inline void EndianSwap(MaterialVertexDeclaration& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(GfxVertexShaderLoadDef& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialVertexShaderProgram& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialVertexShader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.prog.loadDef.cachedPart, operation);
        EndianSwap(value.prog.loadDef.physicalPart, operation);
        EndianSwap(value.prog.loadDef.cachedPartSize, operation);
        EndianSwap(value.prog.loadDef.physicalPartSize, operation);
    }

    static inline void EndianSwap(GfxPixelShaderLoadDef& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialPixelShaderProgram& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialPixelShader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.prog.loadDef.cachedPart, operation);
        EndianSwap(value.prog.loadDef.physicalPart, operation);
        EndianSwap(value.prog.loadDef.cachedPartSize, operation);
        EndianSwap(value.prog.loadDef.physicalPartSize, operation);
    }

    static inline void EndianSwap(MaterialArgumentDef& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialShaderArgument& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(value.type, operation);

        EndianSwap(value.dest, operation);

        switch (type)
        {
        case T5Xenon::MTL_ARG_LITERAL_VERTEX_CONST:
        case T5Xenon::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(value.u.literalConst, operation);
            break;

        case T5Xenon::MTL_ARG_CODE_VERTEX_CONST:
        case T5Xenon::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(value.u.codeConst.index, operation);
            break;

        case T5Xenon::MTL_ARG_CODE_PIXEL_SAMPLER:
            EndianSwap(value.u.codeSampler, operation);
            break;

        case T5Xenon::MTL_ARG_MATERIAL_VERTEX_CONST:
        case T5Xenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
        case T5Xenon::MTL_ARG_MATERIAL_PIXEL_CONST:
            EndianSwap(value.u.nameHash, operation);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(MaterialPass& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertexDecl, operation);
        EndianSwap(value.vertexShaderArray, operation);
        EndianSwap(value.vertexShader, operation);
        EndianSwap(value.pixelShader, operation);
        EndianSwap(value.args, operation);
    }

    static inline void EndianSwap(MaterialTechnique& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.passCount, operation);
    }

    static inline void EndianSwap(MaterialTechniqueSet& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.techniques, operation);
    }

    static inline void EndianSwap(GfxImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cardMemory.platform, operation);
        EndianSwap(value.width, operation);
        EndianSwap(value.height, operation);
        EndianSwap(value.depth, operation);
        EndianSwap(value.baseSize, operation);
        EndianSwap(value.pixels, operation);
        EndianSwap(value.format, operation);
        EndianSwap(value.unknown80, operation);
        for (auto& stream : value.streams)
        {
            EndianSwap(stream.pixelSize, operation);
            EndianSwap(stream.width, operation);
            EndianSwap(stream.height, operation);
            EndianSwap(stream.packIndex, operation);
        }
        EndianSwap(value.name, operation);
        EndianSwap(value.hash, operation);
    }

    static inline void EndianSwap(GfxLightImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image, operation);
    }

    static inline void EndianSwap(GfxLightDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.attenuation, operation);
        EndianSwap(value.lmapLookupStart, operation);
    }

    static inline void EndianSwap(WaterWritable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.floatTime, operation);
    }

    static inline void EndianSwap(water_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.writable, operation);
        EndianSwap(value.H0X, operation);
        EndianSwap(value.H0Y, operation);
        EndianSwap(value.wTerm, operation);
        EndianSwap(value.M, operation);
        EndianSwap(value.N, operation);
        EndianSwap(value.Lx, operation);
        EndianSwap(value.Lz, operation);
        EndianSwap(value.gravity, operation);
        EndianSwap(value.windvel, operation);
        EndianSwap(value.winddir, operation);
        EndianSwap(value.amplitude, operation);
        EndianSwap(value.codeConstant, operation);
        EndianSwap(value.image, operation);
    }

    static inline void EndianSwap(MaterialTextureDefInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image, operation);
    }

    static inline void EndianSwap(MaterialTextureDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nameHash, operation);
        EndianSwap(value.u, operation);
    }

    static inline void EndianSwap(GfxStateBits& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadBits, operation);
    }

    static inline void EndianSwap(MaterialConstantDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nameHash, operation);
        EndianSwap(value.literal, operation);
    }

    static inline void EndianSwap(MaterialInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.gameFlags, operation);
        EndianSwap(value.drawSurf, operation);
        EndianSwap(value.surfaceTypeBits, operation);
        EndianSwap(value.layeredSurfaceTypes, operation);
    }

    static inline void EndianSwap(Material& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.info, operation);
        EndianSwap(value.techniqueSet, operation);
        EndianSwap(value.textureTable, operation);
        EndianSwap(value.stateBitsTable, operation);
        EndianSwap(value.constantTable, operation);
    }

    static inline void EndianSwap(vec2_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.v, operation);
    }

    static inline void EndianSwap(flameTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flameVar_streamChunkGravityStart, operation);
        EndianSwap(value.flameVar_streamChunkGravityEnd, operation);
        EndianSwap(value.flameVar_streamChunkMaxSize, operation);
        EndianSwap(value.flameVar_streamChunkStartSize, operation);
        EndianSwap(value.flameVar_streamChunkEndSize, operation);
        EndianSwap(value.flameVar_streamChunkStartSizeRand, operation);
        EndianSwap(value.flameVar_streamChunkEndSizeRand, operation);
        EndianSwap(value.flameVar_streamChunkDistScalar, operation);
        EndianSwap(value.flameVar_streamChunkDistSwayScale, operation);
        EndianSwap(value.flameVar_streamChunkDistSwayVelMax, operation);
        EndianSwap(value.flameVar_streamChunkSpeed, operation);
        EndianSwap(value.flameVar_streamChunkDecel, operation);
        EndianSwap(value.flameVar_streamChunkVelocityAddScale, operation);
        EndianSwap(value.flameVar_streamChunkDuration, operation);
        EndianSwap(value.flameVar_streamChunkDurationScaleMaxVel, operation);
        EndianSwap(value.flameVar_streamChunkDurationVelScalar, operation);
        EndianSwap(value.flameVar_streamChunkSizeSpeedScale, operation);
        EndianSwap(value.flameVar_streamChunkSizeAgeScale, operation);
        EndianSwap(value.flameVar_streamChunkSpawnFireIntervalStart, operation);
        EndianSwap(value.flameVar_streamChunkSpawnFireIntervalEnd, operation);
        EndianSwap(value.flameVar_streamChunkSpawnFireMinLifeFrac, operation);
        EndianSwap(value.flameVar_streamChunkSpawnFireMaxLifeFrac, operation);
        EndianSwap(value.flameVar_streamChunkFireMinLifeFrac, operation);
        EndianSwap(value.flameVar_streamChunkFireMinLifeFracStart, operation);
        EndianSwap(value.flameVar_streamChunkFireMinLifeFracEnd, operation);
        EndianSwap(value.flameVar_streamChunkDripsMinLifeFrac, operation);
        EndianSwap(value.flameVar_streamChunkDripsMinLifeFracStart, operation);
        EndianSwap(value.flameVar_streamChunkDripsMinLifeFracEnd, operation);
        EndianSwap(value.flameVar_streamChunkRotationRange, operation);
        EndianSwap(value.flameVar_streamSizeRandSinWave, operation);
        EndianSwap(value.flameVar_streamSizeRandCosWave, operation);
        EndianSwap(value.flameVar_streamDripsChunkInterval, operation);
        EndianSwap(value.flameVar_streamDripsChunkMinFrac, operation);
        EndianSwap(value.flameVar_streamDripsChunkRandFrac, operation);
        EndianSwap(value.flameVar_streamSmokeChunkInterval, operation);
        EndianSwap(value.flameVar_streamSmokeChunkMinFrac, operation);
        EndianSwap(value.flameVar_streamSmokeChunkRandFrac, operation);
        EndianSwap(value.flameVar_streamChunkCullDistSizeFrac, operation);
        EndianSwap(value.flameVar_streamChunkCullMinLife, operation);
        EndianSwap(value.flameVar_streamChunkCullMaxLife, operation);
        EndianSwap(value.flameVar_streamFuelSizeStart, operation);
        EndianSwap(value.flameVar_streamFuelSizeEnd, operation);
        EndianSwap(value.flameVar_streamFuelLength, operation);
        EndianSwap(value.flameVar_streamFuelNumSegments, operation);
        EndianSwap(value.flameVar_streamFuelAnimLoopTime, operation);
        EndianSwap(value.flameVar_streamFlameSizeStart, operation);
        EndianSwap(value.flameVar_streamFlameSizeEnd, operation);
        EndianSwap(value.flameVar_streamFlameLength, operation);
        EndianSwap(value.flameVar_streamFlameNumSegments, operation);
        EndianSwap(value.flameVar_streamFlameAnimLoopTime, operation);
        EndianSwap(value.flameVar_streamPrimaryLightRadius, operation);
        EndianSwap(value.flameVar_streamPrimaryLightRadiusFlutter, operation);
        EndianSwap(value.flameVar_streamPrimaryLightR, operation);
        EndianSwap(value.flameVar_streamPrimaryLightG, operation);
        EndianSwap(value.flameVar_streamPrimaryLightB, operation);
        EndianSwap(value.flameVar_streamPrimaryLightFlutterR, operation);
        EndianSwap(value.flameVar_streamPrimaryLightFlutterG, operation);
        EndianSwap(value.flameVar_streamPrimaryLightFlutterB, operation);
        EndianSwap(value.flameVar_fireLife, operation);
        EndianSwap(value.flameVar_fireLifeRand, operation);
        EndianSwap(value.flameVar_fireSpeedScale, operation);
        EndianSwap(value.flameVar_fireSpeedScaleRand, operation);
        EndianSwap(value.flameVar_fireVelocityAddZ, operation);
        EndianSwap(value.flameVar_fireVelocityAddZRand, operation);
        EndianSwap(value.flameVar_fireVelocityAddSideways, operation);
        EndianSwap(value.flameVar_fireGravity, operation);
        EndianSwap(value.flameVar_fireGravityEnd, operation);
        EndianSwap(value.flameVar_fireMaxRotVel, operation);
        EndianSwap(value.flameVar_fireFriction, operation);
        EndianSwap(value.flameVar_fireEndSizeAdd, operation);
        EndianSwap(value.flameVar_fireStartSizeScale, operation);
        EndianSwap(value.flameVar_fireEndSizeScale, operation);
        EndianSwap(value.flameVar_dripsLife, operation);
        EndianSwap(value.flameVar_dripsLifeRand, operation);
        EndianSwap(value.flameVar_dripsSpeedScale, operation);
        EndianSwap(value.flameVar_dripsSpeedScaleRand, operation);
        EndianSwap(value.flameVar_dripsVelocityAddZ, operation);
        EndianSwap(value.flameVar_dripsVelocityAddZRand, operation);
        EndianSwap(value.flameVar_dripsVelocityAddSideways, operation);
        EndianSwap(value.flameVar_dripsGravity, operation);
        EndianSwap(value.flameVar_dripsGravityEnd, operation);
        EndianSwap(value.flameVar_dripsMaxRotVel, operation);
        EndianSwap(value.flameVar_dripsFriction, operation);
        EndianSwap(value.flameVar_dripsEndSizeAdd, operation);
        EndianSwap(value.flameVar_dripsStartSizeScale, operation);
        EndianSwap(value.flameVar_dripsEndSizeScale, operation);
        EndianSwap(value.flameVar_smokeLife, operation);
        EndianSwap(value.flameVar_smokeLifeRand, operation);
        EndianSwap(value.flameVar_smokeSpeedScale, operation);
        EndianSwap(value.flameVar_smokeVelocityAddZ, operation);
        EndianSwap(value.flameVar_smokeGravity, operation);
        EndianSwap(value.flameVar_smokeGravityEnd, operation);
        EndianSwap(value.flameVar_smokeMaxRotation, operation);
        EndianSwap(value.flameVar_smokeMaxRotVel, operation);
        EndianSwap(value.flameVar_smokeFriction, operation);
        EndianSwap(value.flameVar_smokeEndSizeAdd, operation);
        EndianSwap(value.flameVar_smokeStartSizeAdd, operation);
        EndianSwap(value.flameVar_smokeOriginSizeOfsZScale, operation);
        EndianSwap(value.flameVar_smokeOriginOfsZ, operation);
        EndianSwap(value.flameVar_smokeFadein, operation);
        EndianSwap(value.flameVar_smokeFadeout, operation);
        EndianSwap(value.flameVar_smokeMaxAlpha, operation);
        EndianSwap(value.flameVar_smokeBrightness, operation);
        EndianSwap(value.flameVar_smokeOriginOffset, operation);
        EndianSwap(value.flameVar_collisionSpeedScale, operation);
        EndianSwap(value.flameVar_collisionVolumeScale, operation);
        EndianSwap(value.name, operation);
        EndianSwap(value.fire, operation);
        EndianSwap(value.smoke, operation);
        EndianSwap(value.heat, operation);
        EndianSwap(value.drips, operation);
        EndianSwap(value.streamFuel, operation);
        EndianSwap(value.streamFuel2, operation);
        EndianSwap(value.streamFlame, operation);
        EndianSwap(value.streamFlame2, operation);
        EndianSwap(value.flameOffLoopSound, operation);
        EndianSwap(value.flameIgniteSound, operation);
        EndianSwap(value.flameOnLoopSound, operation);
        EndianSwap(value.flameCooldownSound, operation);
    }

    static inline void EndianSwap(WeaponDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.szOverlayName, operation);
        EndianSwap(value.gunXModel, operation);
        EndianSwap(value.handXModel, operation);
        EndianSwap(value.szModeName, operation);
        EndianSwap(value.notetrackSoundMapKeys, operation);
        EndianSwap(value.notetrackSoundMapValues, operation);
        EndianSwap(value.playerAnimType, operation);
        EndianSwap(value.weapType, operation);
        EndianSwap(value.weapClass, operation);
        EndianSwap(value.penetrateType, operation);
        EndianSwap(value.impactType, operation);
        EndianSwap(value.inventoryType, operation);
        EndianSwap(value.fireType, operation);
        EndianSwap(value.clipType, operation);
        EndianSwap(value.itemIndex, operation);
        EndianSwap(value.parentWeaponName, operation);
        EndianSwap(value.iJamFireTime, operation);
        EndianSwap(value.tracerFrequency, operation);
        EndianSwap(value.tracerWidth, operation);
        EndianSwap(value.tracerLength, operation);
        EndianSwap(value.overheatWeapon, operation);
        EndianSwap(value.overheatRate, operation);
        EndianSwap(value.cooldownRate, operation);
        EndianSwap(value.overheatEndVal, operation);
        EndianSwap(value.iTankLifeTime, operation);
        EndianSwap(value.offhandClass, operation);
        EndianSwap(value.offhandSlot, operation);
        EndianSwap(value.stance, operation);
        EndianSwap(value.viewFlashEffect, operation);
        EndianSwap(value.worldFlashEffect, operation);
        EndianSwap(value.pickupSound, operation);
        EndianSwap(value.pickupSoundPlayer, operation);
        EndianSwap(value.ammoPickupSound, operation);
        EndianSwap(value.ammoPickupSoundPlayer, operation);
        EndianSwap(value.projectileSound, operation);
        EndianSwap(value.pullbackSound, operation);
        EndianSwap(value.pullbackSoundPlayer, operation);
        EndianSwap(value.fireSound, operation);
        EndianSwap(value.fireSoundPlayer, operation);
        EndianSwap(value.fireLoopSound, operation);
        EndianSwap(value.fireLoopSoundPlayer, operation);
        EndianSwap(value.fireLoopEndSound, operation);
        EndianSwap(value.fireLoopEndSoundPlayer, operation);
        EndianSwap(value.fireStopSound, operation);
        EndianSwap(value.fireStopSoundPlayer, operation);
        EndianSwap(value.fireLastSound, operation);
        EndianSwap(value.fireLastSoundPlayer, operation);
        EndianSwap(value.emptyFireSound, operation);
        EndianSwap(value.emptyFireSoundPlayer, operation);
        EndianSwap(value.crackSound, operation);
        EndianSwap(value.whizbySound, operation);
        EndianSwap(value.meleeSwipeSound, operation);
        EndianSwap(value.meleeSwipeSoundPlayer, operation);
        EndianSwap(value.meleeHitSound, operation);
        EndianSwap(value.meleeMissSound, operation);
        EndianSwap(value.rechamberSound, operation);
        EndianSwap(value.rechamberSoundPlayer, operation);
        EndianSwap(value.reloadSound, operation);
        EndianSwap(value.reloadSoundPlayer, operation);
        EndianSwap(value.reloadEmptySound, operation);
        EndianSwap(value.reloadEmptySoundPlayer, operation);
        EndianSwap(value.reloadStartSound, operation);
        EndianSwap(value.reloadStartSoundPlayer, operation);
        EndianSwap(value.reloadEndSound, operation);
        EndianSwap(value.reloadEndSoundPlayer, operation);
        EndianSwap(value.rotateLoopSound, operation);
        EndianSwap(value.rotateLoopSoundPlayer, operation);
        EndianSwap(value.deploySound, operation);
        EndianSwap(value.deploySoundPlayer, operation);
        EndianSwap(value.finishDeploySound, operation);
        EndianSwap(value.finishDeploySoundPlayer, operation);
        EndianSwap(value.breakdownSound, operation);
        EndianSwap(value.breakdownSoundPlayer, operation);
        EndianSwap(value.finishBreakdownSound, operation);
        EndianSwap(value.finishBreakdownSoundPlayer, operation);
        EndianSwap(value.detonateSound, operation);
        EndianSwap(value.detonateSoundPlayer, operation);
        EndianSwap(value.nightVisionWearSound, operation);
        EndianSwap(value.nightVisionWearSoundPlayer, operation);
        EndianSwap(value.nightVisionRemoveSound, operation);
        EndianSwap(value.nightVisionRemoveSoundPlayer, operation);
        EndianSwap(value.altSwitchSound, operation);
        EndianSwap(value.altSwitchSoundPlayer, operation);
        EndianSwap(value.raiseSound, operation);
        EndianSwap(value.raiseSoundPlayer, operation);
        EndianSwap(value.firstRaiseSound, operation);
        EndianSwap(value.firstRaiseSoundPlayer, operation);
        EndianSwap(value.putawaySound, operation);
        EndianSwap(value.putawaySoundPlayer, operation);
        EndianSwap(value.overheatSound, operation);
        EndianSwap(value.overheatSoundPlayer, operation);
        EndianSwap(value.adsZoomSound, operation);
        EndianSwap(value.bounceSound, operation);
        EndianSwap(value.standMountedWeapdef, operation);
        EndianSwap(value.crouchMountedWeapdef, operation);
        EndianSwap(value.proneMountedWeapdef, operation);
        EndianSwap(value.standMountedIndex, operation);
        EndianSwap(value.crouchMountedIndex, operation);
        EndianSwap(value.proneMountedIndex, operation);
        EndianSwap(value.viewShellEjectEffect, operation);
        EndianSwap(value.worldShellEjectEffect, operation);
        EndianSwap(value.viewLastShotEjectEffect, operation);
        EndianSwap(value.worldLastShotEjectEffect, operation);
        EndianSwap(value.reticleCenter, operation);
        EndianSwap(value.reticleSide, operation);
        EndianSwap(value.iReticleCenterSize, operation);
        EndianSwap(value.iReticleSideSize, operation);
        EndianSwap(value.iReticleMinOfs, operation);
        EndianSwap(value.activeReticleType, operation);
        EndianSwap(value.vStandMove, operation);
        EndianSwap(value.vStandRot, operation);
        EndianSwap(value.vDuckedOfs, operation);
        EndianSwap(value.vDuckedMove, operation);
        EndianSwap(value.vDuckedSprintOfs, operation);
        EndianSwap(value.vDuckedSprintRot, operation);
        EndianSwap(value.vDuckedSprintBob, operation);
        EndianSwap(value.fDuckedSprintCycleScale, operation);
        EndianSwap(value.vSprintOfs, operation);
        EndianSwap(value.vSprintRot, operation);
        EndianSwap(value.vSprintBob, operation);
        EndianSwap(value.fSprintCycleScale, operation);
        EndianSwap(value.vLowReadyOfs, operation);
        EndianSwap(value.vLowReadyRot, operation);
        EndianSwap(value.vDtpOfs, operation);
        EndianSwap(value.vDtpRot, operation);
        EndianSwap(value.vDtpBob, operation);
        EndianSwap(value.fDtpCycleScale, operation);
        EndianSwap(value.vMantleOfs, operation);
        EndianSwap(value.vMantleRot, operation);
        EndianSwap(value.vSlideOfs, operation);
        EndianSwap(value.vSlideRot, operation);
        EndianSwap(value.vDuckedRot, operation);
        EndianSwap(value.vProneOfs, operation);
        EndianSwap(value.vProneMove, operation);
        EndianSwap(value.vProneRot, operation);
        EndianSwap(value.vStrafeMove, operation);
        EndianSwap(value.vStrafeRot, operation);
        EndianSwap(value.fPosMoveRate, operation);
        EndianSwap(value.fPosProneMoveRate, operation);
        EndianSwap(value.fStandMoveMinSpeed, operation);
        EndianSwap(value.fDuckedMoveMinSpeed, operation);
        EndianSwap(value.fProneMoveMinSpeed, operation);
        EndianSwap(value.fPosRotRate, operation);
        EndianSwap(value.fPosProneRotRate, operation);
        EndianSwap(value.fStandRotMinSpeed, operation);
        EndianSwap(value.fDuckedRotMinSpeed, operation);
        EndianSwap(value.fProneRotMinSpeed, operation);
        EndianSwap(value.worldModel, operation);
        EndianSwap(value.worldClipModel, operation);
        EndianSwap(value.rocketModel, operation);
        EndianSwap(value.mountedModel, operation);
        EndianSwap(value.additionalMeleeModel, operation);
        EndianSwap(value.hudIcon, operation);
        EndianSwap(value.hudIconRatio, operation);
        EndianSwap(value.indicatorIcon, operation);
        EndianSwap(value.indicatorIconRatio, operation);
        EndianSwap(value.ammoCounterIcon, operation);
        EndianSwap(value.ammoCounterIconRatio, operation);
        EndianSwap(value.ammoCounterClip, operation);
        EndianSwap(value.iStartAmmo, operation);
        EndianSwap(value.iHeatIndex, operation);
        EndianSwap(value.iMaxAmmo, operation);
        EndianSwap(value.shotCount, operation);
        EndianSwap(value.szSharedAmmoCapName, operation);
        EndianSwap(value.iSharedAmmoCapIndex, operation);
        EndianSwap(value.iSharedAmmoCap, operation);
        EndianSwap(value.damage, operation);
        EndianSwap(value.damageDuration, operation);
        EndianSwap(value.damageInterval, operation);
        EndianSwap(value.playerDamage, operation);
        EndianSwap(value.iMeleeDamage, operation);
        EndianSwap(value.iDamageType, operation);
        EndianSwap(value.explosionTag, operation);
        EndianSwap(value.iFireDelay, operation);
        EndianSwap(value.iMeleeDelay, operation);
        EndianSwap(value.meleeChargeDelay, operation);
        EndianSwap(value.iDetonateDelay, operation);
        EndianSwap(value.iSpinUpTime, operation);
        EndianSwap(value.iSpinDownTime, operation);
        EndianSwap(value.spinRate, operation);
        EndianSwap(value.spinLoopSound, operation);
        EndianSwap(value.spinLoopSoundPlayer, operation);
        EndianSwap(value.startSpinSound, operation);
        EndianSwap(value.startSpinSoundPlayer, operation);
        EndianSwap(value.stopSpinSound, operation);
        EndianSwap(value.stopSpinSoundPlayer, operation);
        EndianSwap(value.iFireTime, operation);
        EndianSwap(value.iLastFireTime, operation);
        EndianSwap(value.iRechamberTime, operation);
        EndianSwap(value.iRechamberBoltTime, operation);
        EndianSwap(value.iHoldFireTime, operation);
        EndianSwap(value.iDetonateTime, operation);
        EndianSwap(value.iMeleeTime, operation);
        EndianSwap(value.meleeChargeTime, operation);
        EndianSwap(value.iReloadTimeRight, operation);
        EndianSwap(value.iReloadTimeLeft, operation);
        EndianSwap(value.reloadShowRocketTime, operation);
        EndianSwap(value.iReloadEmptyTimeLeft, operation);
        EndianSwap(value.iReloadAddTime, operation);
        EndianSwap(value.iReloadEmptyAddTime, operation);
        EndianSwap(value.iReloadQuickAddTime, operation);
        EndianSwap(value.iReloadQuickEmptyAddTime, operation);
        EndianSwap(value.iReloadStartTime, operation);
        EndianSwap(value.iReloadStartAddTime, operation);
        EndianSwap(value.iReloadEndTime, operation);
        EndianSwap(value.iDropTime, operation);
        EndianSwap(value.iRaiseTime, operation);
        EndianSwap(value.iAltDropTime, operation);
        EndianSwap(value.quickDropTime, operation);
        EndianSwap(value.quickRaiseTime, operation);
        EndianSwap(value.iFirstRaiseTime, operation);
        EndianSwap(value.iEmptyRaiseTime, operation);
        EndianSwap(value.iEmptyDropTime, operation);
        EndianSwap(value.sprintInTime, operation);
        EndianSwap(value.sprintLoopTime, operation);
        EndianSwap(value.sprintOutTime, operation);
        EndianSwap(value.lowReadyInTime, operation);
        EndianSwap(value.lowReadyLoopTime, operation);
        EndianSwap(value.lowReadyOutTime, operation);
        EndianSwap(value.contFireInTime, operation);
        EndianSwap(value.contFireLoopTime, operation);
        EndianSwap(value.contFireOutTime, operation);
        EndianSwap(value.dtpInTime, operation);
        EndianSwap(value.dtpLoopTime, operation);
        EndianSwap(value.dtpOutTime, operation);
        EndianSwap(value.slideInTime, operation);
        EndianSwap(value.deployTime, operation);
        EndianSwap(value.breakdownTime, operation);
        EndianSwap(value.nightVisionWearTime, operation);
        EndianSwap(value.nightVisionWearTimeFadeOutEnd, operation);
        EndianSwap(value.nightVisionWearTimePowerUp, operation);
        EndianSwap(value.nightVisionRemoveTime, operation);
        EndianSwap(value.nightVisionRemoveTimePowerDown, operation);
        EndianSwap(value.nightVisionRemoveTimeFadeInStart, operation);
        EndianSwap(value.fuseTime, operation);
        EndianSwap(value.aiFuseTime, operation);
        EndianSwap(value.lockOnRadius, operation);
        EndianSwap(value.lockOnSpeed, operation);
        EndianSwap(value.stackFire, operation);
        EndianSwap(value.stackFireSpread, operation);
        EndianSwap(value.stackFireAccuracyDecay, operation);
        EndianSwap(value.stackSound, operation);
        EndianSwap(value.autoAimRange, operation);
        EndianSwap(value.aimAssistRange, operation);
        EndianSwap(value.aimPadding, operation);
        EndianSwap(value.enemyCrosshairRange, operation);
        EndianSwap(value.moveSpeedScale, operation);
        EndianSwap(value.adsMoveSpeedScale, operation);
        EndianSwap(value.sprintDurationScale, operation);
        EndianSwap(value.overlayReticle, operation);
        EndianSwap(value.overlayInterface, operation);
        EndianSwap(value.overlayWidth, operation);
        EndianSwap(value.overlayHeight, operation);
        EndianSwap(value.fAdsBobFactor, operation);
        EndianSwap(value.fAdsViewBobMult, operation);
        EndianSwap(value.fHipSpreadStandMin, operation);
        EndianSwap(value.fHipSpreadDuckedMin, operation);
        EndianSwap(value.fHipSpreadProneMin, operation);
        EndianSwap(value.hipSpreadStandMax, operation);
        EndianSwap(value.hipSpreadDuckedMax, operation);
        EndianSwap(value.hipSpreadProneMax, operation);
        EndianSwap(value.fHipSpreadDecayRate, operation);
        EndianSwap(value.fHipSpreadFireAdd, operation);
        EndianSwap(value.fHipSpreadTurnAdd, operation);
        EndianSwap(value.fHipSpreadMoveAdd, operation);
        EndianSwap(value.fHipSpreadDuckedDecay, operation);
        EndianSwap(value.fHipSpreadProneDecay, operation);
        EndianSwap(value.fHipReticleSidePos, operation);
        EndianSwap(value.fAdsIdleAmount, operation);
        EndianSwap(value.fHipIdleAmount, operation);
        EndianSwap(value.adsIdleSpeed, operation);
        EndianSwap(value.hipIdleSpeed, operation);
        EndianSwap(value.fIdleCrouchFactor, operation);
        EndianSwap(value.fIdleProneFactor, operation);
        EndianSwap(value.fGunMaxPitch, operation);
        EndianSwap(value.fGunMaxYaw, operation);
        EndianSwap(value.swayMaxAngle, operation);
        EndianSwap(value.swayLerpSpeed, operation);
        EndianSwap(value.swayPitchScale, operation);
        EndianSwap(value.swayYawScale, operation);
        EndianSwap(value.swayHorizScale, operation);
        EndianSwap(value.swayVertScale, operation);
        EndianSwap(value.swayShellShockScale, operation);
        EndianSwap(value.adsSwayMaxAngle, operation);
        EndianSwap(value.adsSwayLerpSpeed, operation);
        EndianSwap(value.adsSwayPitchScale, operation);
        EndianSwap(value.adsSwayYawScale, operation);
        EndianSwap(value.adsViewErrorMin, operation);
        EndianSwap(value.adsViewErrorMax, operation);
        EndianSwap(value.killIcon, operation);
        EndianSwap(value.killIconRatio, operation);
        EndianSwap(value.iReloadAmmoAdd, operation);
        EndianSwap(value.iReloadStartAdd, operation);
        EndianSwap(value.szSpawnedGrenadeWeaponName, operation);
        EndianSwap(value.szDualWieldWeaponName, operation);
        EndianSwap(value.dualWieldWeaponIndex, operation);
        EndianSwap(value.iDropAmmoMin, operation);
        EndianSwap(value.iDropAmmoMax, operation);
        EndianSwap(value.iDropClipAmmoMin, operation);
        EndianSwap(value.iDropClipAmmoMax, operation);
        EndianSwap(value.isRollingGrenade, operation);
        EndianSwap(value.iExplosionRadius, operation);
        EndianSwap(value.iExplosionRadiusMin, operation);
        EndianSwap(value.iIndicatorRadius, operation);
        EndianSwap(value.iExplosionInnerDamage, operation);
        EndianSwap(value.iExplosionOuterDamage, operation);
        EndianSwap(value.damageConeAngle, operation);
        EndianSwap(value.iProjectileSpeed, operation);
        EndianSwap(value.iProjectileSpeedUp, operation);
        EndianSwap(value.iProjectileSpeedRelativeUp, operation);
        EndianSwap(value.iProjectileSpeedForward, operation);
        EndianSwap(value.iProjectileActivateDist, operation);
        EndianSwap(value.projLifetime, operation);
        EndianSwap(value.timeToAccelerate, operation);
        EndianSwap(value.projectileCurvature, operation);
        EndianSwap(value.projectileModel, operation);
        EndianSwap(value.projExplosion, operation);
        EndianSwap(value.projExplosionEffect, operation);
        EndianSwap(value.projExplosionEffect2, operation);
        EndianSwap(value.projExplosionEffect3, operation);
        EndianSwap(value.projExplosionEffect4, operation);
        EndianSwap(value.projExplosionEffect5, operation);
        EndianSwap(value.projDudEffect, operation);
        EndianSwap(value.projExplosionSound, operation);
        EndianSwap(value.projDudSound, operation);
        EndianSwap(value.mortarShellSound, operation);
        EndianSwap(value.tankShellSound, operation);
        EndianSwap(value.stickiness, operation);
        EndianSwap(value.rotateType, operation);
        EndianSwap(value.lowAmmoWarningThreshold, operation);
        EndianSwap(value.meleeChargeRange, operation);
        EndianSwap(value.parallelBounce, operation);
        EndianSwap(value.perpendicularBounce, operation);
        EndianSwap(value.projTrailEffect, operation);
        EndianSwap(value.vProjectileColor, operation);
        EndianSwap(value.guidedMissileType, operation);
        EndianSwap(value.maxSteeringAccel, operation);
        EndianSwap(value.projIgnitionDelay, operation);
        EndianSwap(value.projIgnitionEffect, operation);
        EndianSwap(value.projIgnitionSound, operation);
        EndianSwap(value.fAdsAimPitch, operation);
        EndianSwap(value.fAdsCrosshairInFrac, operation);
        EndianSwap(value.fAdsCrosshairOutFrac, operation);
        EndianSwap(value.adsGunKickReducedKickBullets, operation);
        EndianSwap(value.adsGunKickReducedKickPercent, operation);
        EndianSwap(value.fAdsGunKickPitchMin, operation);
        EndianSwap(value.fAdsGunKickPitchMax, operation);
        EndianSwap(value.fAdsGunKickYawMin, operation);
        EndianSwap(value.fAdsGunKickYawMax, operation);
        EndianSwap(value.fAdsGunKickAccel, operation);
        EndianSwap(value.fAdsGunKickSpeedMax, operation);
        EndianSwap(value.fAdsGunKickSpeedDecay, operation);
        EndianSwap(value.fAdsGunKickStaticDecay, operation);
        EndianSwap(value.fAdsViewKickPitchMin, operation);
        EndianSwap(value.fAdsViewKickPitchMax, operation);
        EndianSwap(value.fAdsViewKickYawMin, operation);
        EndianSwap(value.fAdsViewKickYawMax, operation);
        EndianSwap(value.fAdsViewScatterMin, operation);
        EndianSwap(value.fAdsViewScatterMax, operation);
        EndianSwap(value.fAdsSpread, operation);
        EndianSwap(value.hipGunKickReducedKickBullets, operation);
        EndianSwap(value.hipGunKickReducedKickPercent, operation);
        EndianSwap(value.fHipGunKickPitchMin, operation);
        EndianSwap(value.fHipGunKickPitchMax, operation);
        EndianSwap(value.fHipGunKickYawMin, operation);
        EndianSwap(value.fHipGunKickYawMax, operation);
        EndianSwap(value.fHipGunKickAccel, operation);
        EndianSwap(value.fHipGunKickSpeedMax, operation);
        EndianSwap(value.fHipGunKickSpeedDecay, operation);
        EndianSwap(value.fHipGunKickStaticDecay, operation);
        EndianSwap(value.fHipViewKickPitchMin, operation);
        EndianSwap(value.fHipViewKickPitchMax, operation);
        EndianSwap(value.fHipViewKickYawMin, operation);
        EndianSwap(value.fHipViewKickYawMax, operation);
        EndianSwap(value.fHipViewScatterMin, operation);
        EndianSwap(value.fHipViewScatterMax, operation);
        EndianSwap(value.fightDist, operation);
        EndianSwap(value.maxDist, operation);
        EndianSwap(value.aiVsAiAccuracyGraphName, operation);
        EndianSwap(value.aiVsPlayerAccuracyGraphName, operation);
        EndianSwap(value.aiVsAiAccuracyGraphKnots, operation);
        EndianSwap(value.aiVsPlayerAccuracyGraphKnots, operation);
        EndianSwap(value.originalAiVsAiAccuracyGraphKnots, operation);
        EndianSwap(value.originalAiVsPlayerAccuracyGraphKnots, operation);
        EndianSwap(value.aiVsAiAccuracyGraphKnotCount, operation);
        EndianSwap(value.aiVsPlayerAccuracyGraphKnotCount, operation);
        EndianSwap(value.originalAiVsAiAccuracyGraphKnotCount, operation);
        EndianSwap(value.originalAiVsPlayerAccuracyGraphKnotCount, operation);
        EndianSwap(value.iPositionReloadTransTime, operation);
        EndianSwap(value.leftArc, operation);
        EndianSwap(value.rightArc, operation);
        EndianSwap(value.topArc, operation);
        EndianSwap(value.bottomArc, operation);
        EndianSwap(value.accuracy, operation);
        EndianSwap(value.aiSpread, operation);
        EndianSwap(value.playerSpread, operation);
        EndianSwap(value.minTurnSpeed, operation);
        EndianSwap(value.maxTurnSpeed, operation);
        EndianSwap(value.pitchConvergenceTime, operation);
        EndianSwap(value.yawConvergenceTime, operation);
        EndianSwap(value.suppressTime, operation);
        EndianSwap(value.maxRange, operation);
        EndianSwap(value.fAnimHorRotateInc, operation);
        EndianSwap(value.fPlayerPositionDist, operation);
        EndianSwap(value.szUseHintString, operation);
        EndianSwap(value.dropHintString, operation);
        EndianSwap(value.iUseHintStringIndex, operation);
        EndianSwap(value.dropHintStringIndex, operation);
        EndianSwap(value.horizViewJitter, operation);
        EndianSwap(value.vertViewJitter, operation);
        EndianSwap(value.szScript, operation);
        EndianSwap(value.minDamage, operation);
        EndianSwap(value.minPlayerDamage, operation);
        EndianSwap(value.fMaxDamageRange, operation);
        EndianSwap(value.fMinDamageRange, operation);
        EndianSwap(value.destabilizationRateTime, operation);
        EndianSwap(value.destabilizationCurvatureMax, operation);
        EndianSwap(value.destabilizeDistance, operation);
        EndianSwap(value.locationDamageMultipliers, operation);
        EndianSwap(value.fireRumble, operation);
        EndianSwap(value.meleeImpactRumble, operation);
        EndianSwap(value.reloadRumble, operation);
        EndianSwap(value.adsDofStart, operation);
        EndianSwap(value.adsDofEnd, operation);
        EndianSwap(value.hipDofStart, operation);
        EndianSwap(value.hipDofEnd, operation);
        EndianSwap(value.scanSpeed, operation);
        EndianSwap(value.scanAccel, operation);
        EndianSwap(value.scanPauseTime, operation);
        EndianSwap(value.flameTableFirstPerson, operation);
        EndianSwap(value.flameTableThirdPerson, operation);
        EndianSwap(value.flameTableFirstPersonPtr, operation);
        EndianSwap(value.flameTableThirdPersonPtr, operation);
        EndianSwap(value.tagFx_preparationEffect, operation);
        EndianSwap(value.tagFlash_preparationEffect, operation);
        EndianSwap(value.maxGibDistance, operation);
    }

    static inline void EndianSwap(WeaponVariantDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.szInternalName, operation);
        EndianSwap(value.iVariantCount, operation);
        EndianSwap(value.weapDef, operation);
        EndianSwap(value.szDisplayName, operation);
        EndianSwap(value.szXAnims, operation);
        EndianSwap(value.szAltWeaponName, operation);
        EndianSwap(value.hideTags, operation);
        EndianSwap(value.altWeaponIndex, operation);
        EndianSwap(value.iClipSize, operation);
        EndianSwap(value.iReloadTime, operation);
        EndianSwap(value.iReloadEmptyTime, operation);
        EndianSwap(value.iReloadQuickTime, operation);
        EndianSwap(value.iReloadQuickEmptyTime, operation);
        EndianSwap(value.iAdsTransInTime, operation);
        EndianSwap(value.iAdsTransOutTime, operation);
        EndianSwap(value.iAltRaiseTime, operation);
        EndianSwap(value.szAmmoName, operation);
        EndianSwap(value.iAmmoIndex, operation);
        EndianSwap(value.szClipName, operation);
        EndianSwap(value.iClipIndex, operation);
        EndianSwap(value.fAimAssistRangeAds, operation);
        EndianSwap(value.fAdsSwayHorizScale, operation);
        EndianSwap(value.fAdsSwayVertScale, operation);
        EndianSwap(value.fAdsViewKickCenterSpeed, operation);
        EndianSwap(value.fHipViewKickCenterSpeed, operation);
        EndianSwap(value.fAdsZoomFov1, operation);
        EndianSwap(value.fAdsZoomFov2, operation);
        EndianSwap(value.fAdsZoomFov3, operation);
        EndianSwap(value.fAdsZoomInFrac, operation);
        EndianSwap(value.fAdsZoomOutFrac, operation);
        EndianSwap(value.fOverlayAlphaScale, operation);
        EndianSwap(value.fOOPosAnimLength, operation);
        EndianSwap(value.overlayMaterial, operation);
        EndianSwap(value.overlayMaterialLowRes, operation);
        EndianSwap(value.dpadIcon, operation);
        EndianSwap(value.dpadIconRatio, operation);
        EndianSwap(value.ikLeftHandOffset, operation);
        EndianSwap(value.ikLeftHandRotation, operation);
        EndianSwap(value.ikLeftHandProneOffset, operation);
        EndianSwap(value.ikLeftHandProneRotation, operation);
        EndianSwap(value.ikLeftHandUiViewerOffset, operation);
        EndianSwap(value.ikLeftHandUiViewerRotation, operation);
    }

    static inline void EndianSwap(RawFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.len, operation);
        EndianSwap(value.buffer, operation);
    }

    static inline void EndianSwap(LocalizeEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.value, operation);
        EndianSwap(value.name, operation);
    }

    static inline void EndianSwap(EmblemLayer& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cost, operation);
        EndianSwap(value.unlockLevel, operation);
        EndianSwap(value.unlockPLevel, operation);
    }

    static inline void EndianSwap(EmblemCategory& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.description, operation);
    }

    static inline void EndianSwap(EmblemIcon& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image, operation);
        EndianSwap(value.description, operation);
        EndianSwap(value.outlineSize, operation);
        EndianSwap(value.defaultColor, operation);
        EndianSwap(value.cost, operation);
        EndianSwap(value.unlockLevel, operation);
        EndianSwap(value.unlockPLevel, operation);
        EndianSwap(value.unclassifyAt, operation);
        EndianSwap(value.sortKey, operation);
        EndianSwap(value.category, operation);
    }

    static inline void EndianSwap(EmblemBackground& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.material, operation);
        EndianSwap(value.description, operation);
        EndianSwap(value.cost, operation);
        EndianSwap(value.unlockLevel, operation);
        EndianSwap(value.unlockPLevel, operation);
        EndianSwap(value.unclassifyAt, operation);
    }

    static inline void EndianSwap(EmblemSet& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.colorCount, operation);
        EndianSwap(value.layerCount, operation);
        EndianSwap(value.layers, operation);
        EndianSwap(value.categoryCount, operation);
        EndianSwap(value.categories, operation);
        EndianSwap(value.iconCount, operation);
        EndianSwap(value.icons, operation);
        EndianSwap(value.backgroundCount, operation);
        EndianSwap(value.backgrounds, operation);
        EndianSwap(value.backgroundLookupCount, operation);
        EndianSwap(value.backgroundLookup, operation);
    }

    static inline void EndianSwap(GfxTexture& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(GfxColor& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed, operation);
    }

    static inline void EndianSwap(PackedUnitVec& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed, operation);
    }

    static inline void EndianSwap(GfxDrawSurf& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed, operation);
    }

    static inline void EndianSwap(GfxStreamingAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstItem, operation);
        EndianSwap(value.itemCount, operation);
        EndianSwap(value.firstChild, operation);
        EndianSwap(value.childCount, operation);
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
    }

    static inline void EndianSwap(GfxWorldStreamInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aabbTreeCount, operation);
        EndianSwap(value.aabbTrees, operation);
        EndianSwap(value.leafRefCount, operation);
        EndianSwap(value.leafRefs, operation);
    }

    static inline void EndianSwap(GfxWorldXModelStreamInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aabbTreeCount, operation);
        EndianSwap(value.aabbTrees, operation);
        EndianSwap(value.smodelIndexCount, operation);
        EndianSwap(value.smodelIndices, operation);
        EndianSwap(value.streamBufferSize, operation);
    }

    static inline void EndianSwap(GfxWorldSunColor& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.control, operation);
        EndianSwap(value.angles, operation);
        EndianSwap(value.ambientColor, operation);
        EndianSwap(value.sunDiffuseColor, operation);
        EndianSwap(value.sunSpecularColor, operation);
        EndianSwap(value.skyColor, operation);
        EndianSwap(value.groundColor, operation);
        EndianSwap(value.exposure, operation);
        EndianSwap(value.sunShadowSampleSizeNear, operation);
        EndianSwap(value.skyboxHDRScale, operation);
    }

    static inline void EndianSwap(SunLightParseParams& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.treeScatterIntensity, operation);
        EndianSwap(value.treeScatterAmount, operation);
        EndianSwap(value.sunSettings[0], operation);
    }

    static inline void EndianSwap(float44& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.m, operation);
    }

    static inline void EndianSwap(GfxLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cullDist, operation);
        EndianSwap(value.color, operation);
        EndianSwap(value.dir, operation);
        EndianSwap(value.origin, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.cosHalfFovOuter, operation);
        EndianSwap(value.cosHalfFovInner, operation);
        EndianSwap(value.exponent, operation);
        EndianSwap(value.spotShadowIndex, operation);
        EndianSwap(value.angles, operation);
        EndianSwap(value.spotShadowHiDistance, operation);
        EndianSwap(value.diffuseColor, operation);
        EndianSwap(value.specularColor, operation);
        EndianSwap(value.shadowColor, operation);
        EndianSwap(value.falloff, operation);
        EndianSwap(value.attenuation, operation);
        EndianSwap(value.aAbB, operation);
        EndianSwap(value.cookieControl0, operation);
        EndianSwap(value.cookieControl1, operation);
        EndianSwap(value.cookieControl2, operation);
        EndianSwap(value.viewMatrix, operation);
        EndianSwap(value.projMatrix, operation);
        EndianSwap(value.def, operation);
    }

    static inline void EndianSwap(GfxLightCorona& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.color, operation);
        EndianSwap(value.intensity, operation);
    }

    static inline void EndianSwap(GfxShadowMapVolume& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.control, operation);
        EndianSwap(value.padding1, operation);
        EndianSwap(value.padding2, operation);
        EndianSwap(value.padding3, operation);
    }

    static inline void EndianSwap(GfxVolumePlane& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane, operation);
    }

    static inline void EndianSwap(GfxExposureVolume& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.control, operation);
        EndianSwap(value.exposure, operation);
        EndianSwap(value.luminanceIncreaseScale, operation);
        EndianSwap(value.luminanceDecreaseScale, operation);
        EndianSwap(value.featherRange, operation);
        EndianSwap(value.featherAdjust, operation);
    }

    static inline void EndianSwap(GfxSkyDynamicIntensity& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.angle0, operation);
        EndianSwap(value.angle1, operation);
        EndianSwap(value.factor0, operation);
        EndianSwap(value.factor1, operation);
    }

    static inline void EndianSwap(GfxWorldDpvsPlanes& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cellCount, operation);
        EndianSwap(value.planes, operation);
        EndianSwap(value.nodes, operation);
        EndianSwap(value.sceneEntCellBits, operation);
    }

    static inline void EndianSwap(GfxPortalWritable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.hullPoints, operation);
        EndianSwap(value.queuedParent, operation);
    }

    static inline void EndianSwap(DpvsPlane& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.coeffs, operation);
    }

    static inline void EndianSwap(GfxPortal& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.writable, operation);
        EndianSwap(value.plane, operation);
        EndianSwap(value.cell, operation);
        EndianSwap(value.vertices, operation);
        EndianSwap(value.hullAxis, operation);
    }

    static inline void EndianSwap(GfxAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.childCount, operation);
        EndianSwap(value.surfaceCount, operation);
        EndianSwap(value.startSurfIndex, operation);
        EndianSwap(value.smodelIndexCount, operation);
        EndianSwap(value.smodelIndexes, operation);
        EndianSwap(value.childrenOffset, operation);
    }

    static inline void EndianSwap(GfxCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.aabbTreeCount, operation);
        EndianSwap(value.aabbTree, operation);
        EndianSwap(value.portalCount, operation);
        EndianSwap(value.portals, operation);
        EndianSwap(value.cullGroupCount, operation);
        EndianSwap(value.cullGroups, operation);
        EndianSwap(value.reflectionProbes, operation);
    }

    static inline void EndianSwap(GfxReflectionProbeVolumeData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.volumePlanes, operation);
    }

    static inline void EndianSwap(GfxReflectionProbe& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin, operation);
        EndianSwap(value.reflectionImage, operation);
        EndianSwap(value.probeVolumes, operation);
        EndianSwap(value.probeVolumeCount, operation);
    }

    static inline void EndianSwap(GfxLightmapArray& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.primary, operation);
        EndianSwap(value.secondary, operation);
        EndianSwap(value.secondaryB, operation);
    }

    static inline void EndianSwap(GfxWorldVertex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xyz, operation);
        EndianSwap(value.binormalSign, operation);
        EndianSwap(value.color, operation);
        EndianSwap(value.texCoord, operation);
        EndianSwap(value.lmapCoord, operation);
        EndianSwap(value.normal, operation);
        EndianSwap(value.tangent, operation);
    }

    static inline void EndianSwap(GfxWorldVertexData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertices, operation);
    }

    static inline void EndianSwap(GfxWorldVertexLayerData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data, operation);
    }

    static inline void EndianSwap(GfxWorldDraw& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.reflectionProbeCount, operation);
        EndianSwap(value.reflectionProbes, operation);
        EndianSwap(value.reflectionProbeTextures, operation);
        EndianSwap(value.lightmapCount, operation);
        EndianSwap(value.lightmaps, operation);
        EndianSwap(value.lightmapPrimaryTextures, operation);
        EndianSwap(value.lightmapSecondaryTextures, operation);
        EndianSwap(value.lightmapSecondaryTexturesB, operation);
        EndianSwap(value.terrainScorchImages, operation);
        EndianSwap(value.vertexCount, operation);
        EndianSwap(value.vd, operation);
        EndianSwap(value.vertexLayerDataSize, operation);
        EndianSwap(value.vld, operation);
        EndianSwap(value.vertexStream2DataSize, operation);
        EndianSwap(value.indexCount, operation);
        EndianSwap(value.indices, operation);
    }

    static inline void EndianSwap(GfxLightGridEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.colorsIndex, operation);
    }

    static inline void EndianSwap(GfxCompressedLightGridColors& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(GfxLightGrid& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.sunPrimaryLightIndex, operation);
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.rowAxis, operation);
        EndianSwap(value.colAxis, operation);
        EndianSwap(value.rowDataStart, operation);
        EndianSwap(value.rawRowDataSize, operation);
        EndianSwap(value.rawRowData, operation);
        EndianSwap(value.entryCount, operation);
        EndianSwap(value.entries, operation);
        EndianSwap(value.colorCount, operation);
        EndianSwap(value.colors, operation);
    }

    static inline void EndianSwap(GfxBrushModelWritable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.mip1radiusSq, operation);
    }

    static inline void EndianSwap(GfxBrushModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.writable, operation);
        EndianSwap(value.bounds, operation);
        EndianSwap(value.surfaceCount, operation);
        EndianSwap(value.startSurfIndex, operation);
    }

    static inline void EndianSwap(MaterialMemory& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.material, operation);
        EndianSwap(value.memory, operation);
    }

    static inline void EndianSwap(sunflare_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.spriteMaterial, operation);
        EndianSwap(value.flareMaterial, operation);
        EndianSwap(value.spriteSize, operation);
        EndianSwap(value.flareMinSize, operation);
        EndianSwap(value.flareMinDot, operation);
        EndianSwap(value.flareMaxSize, operation);
        EndianSwap(value.flareMaxDot, operation);
        EndianSwap(value.flareMaxAlpha, operation);
        EndianSwap(value.flareFadeInTime, operation);
        EndianSwap(value.flareFadeOutTime, operation);
        EndianSwap(value.blindMinDot, operation);
        EndianSwap(value.blindMaxDot, operation);
        EndianSwap(value.blindMaxDarken, operation);
        EndianSwap(value.blindFadeInTime, operation);
        EndianSwap(value.blindFadeOutTime, operation);
        EndianSwap(value.glareMinDot, operation);
        EndianSwap(value.glareMaxDot, operation);
        EndianSwap(value.glareMaxLighten, operation);
        EndianSwap(value.glareFadeInTime, operation);
        EndianSwap(value.glareFadeOutTime, operation);
        EndianSwap(value.sunFxPosition, operation);
    }

    static inline void EndianSwap(XModelDrawInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.lod, operation);
        EndianSwap(value.surfId, operation);
    }

    static inline void EndianSwap(GfxSceneDynModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.info, operation);
        EndianSwap(value.dynEntId, operation);
    }

    static inline void EndianSwap(BModelDrawInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfId, operation);
    }

    static inline void EndianSwap(GfxSceneDynBrush& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.info, operation);
        EndianSwap(value.dynEntId, operation);
    }

    static inline void EndianSwap(GfxShadowGeometry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfaceCount, operation);
        EndianSwap(value.smodelCount, operation);
        EndianSwap(value.sortedSurfIndex, operation);
        EndianSwap(value.smodelIndex, operation);
    }

    static inline void EndianSwap(GfxLightRegionAxis& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dir, operation);
        EndianSwap(value.midPoint, operation);
        EndianSwap(value.halfSize, operation);
    }

    static inline void EndianSwap(GfxLightRegionHull& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.kdopMidPoint, operation);
        EndianSwap(value.kdopHalfSize, operation);
        EndianSwap(value.axisCount, operation);
        EndianSwap(value.axis, operation);
    }

    static inline void EndianSwap(GfxLightRegion& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.hullCount, operation);
        EndianSwap(value.hulls, operation);
    }

    static inline void EndianSwap(GfxStaticModelInst& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.lightingOrigin, operation);
        EndianSwap(value.groundLighting, operation);
    }

    static inline void EndianSwap(srfTriangles_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.vertexLayerData, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.firstVertex, operation);
        EndianSwap(value.vertexCount, operation);
        EndianSwap(value.triCount, operation);
        EndianSwap(value.baseIndex, operation);
        EndianSwap(value.himipRadiusSq, operation);
        EndianSwap(value.stream2ByteOffset, operation);
    }

    static inline void EndianSwap(GfxSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.tris, operation);
        EndianSwap(value.material, operation);
        EndianSwap(value.bounds, operation);
    }

    static inline void EndianSwap(GfxCullGroup& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.surfaceCount, operation);
        EndianSwap(value.startSurfIndex, operation);
    }

    static inline void EndianSwap(GfxPackedPlacement& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin, operation);
        for (auto& axis : value.axis)
            EndianSwap(axis, operation);
        EndianSwap(value.scale, operation);
    }

    static inline void EndianSwap(GfxStaticModelDrawInst& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cullDist, operation);
        EndianSwap(value.placement, operation);
        EndianSwap(value.model, operation);
        EndianSwap(value.flags, operation);
        EndianSwap(value.lightingHandle, operation);
    }

    static inline void EndianSwap(GfxWorldDpvsStatic& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.smodelCount, operation);
        EndianSwap(value.dynamicSModelCount, operation);
        EndianSwap(value.staticSurfaceCount, operation);
        EndianSwap(value.litSurfsBegin, operation);
        EndianSwap(value.litSurfsEnd, operation);
        EndianSwap(value.decalSurfsBegin, operation);
        EndianSwap(value.decalSurfsEnd, operation);
        EndianSwap(value.emissiveSurfsBegin, operation);
        EndianSwap(value.emissiveSurfsEnd, operation);
        EndianSwap(value.smodelVisDataCount, operation);
        EndianSwap(value.surfaceVisDataCount, operation);
        EndianSwap(value.smodelVisData, operation);
        EndianSwap(value.surfaceVisData, operation);
        EndianSwap(value.smodelVisDataCameraSaved, operation);
        EndianSwap(value.surfaceVisDataCameraSaved, operation);
        EndianSwap(value.lodData, operation);
        EndianSwap(value.sortedSurfIndex, operation);
        EndianSwap(value.smodelInsts, operation);
        EndianSwap(value.surfaces, operation);
        EndianSwap(value.cullGroups, operation);
        EndianSwap(value.smodelDrawInsts, operation);
        EndianSwap(value.surfaceMaterials, operation);
        EndianSwap(value.surfaceCastsSunShadow, operation);
        const auto usageCount = static_cast<std::uint32_t>(value.usageCount);
        value.usageCount = static_cast<int>(endianness::FromBigEndian(usageCount));
    }

    static inline void EndianSwap(GfxWorldDpvsDynamic& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dynEntClientWordCount, operation);
        EndianSwap(value.dynEntClientCount, operation);
        EndianSwap(value.dynEntCellBits, operation);
        EndianSwap(value.dynEntVisData, operation);
    }

    static inline void EndianSwap(GfxWorldLodChain& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin, operation);
        EndianSwap(value.lastDist, operation);
        EndianSwap(value.firstLodInfo, operation);
        EndianSwap(value.lodInfoCount, operation);
    }

    static inline void EndianSwap(GfxWorldLodInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dist, operation);
        EndianSwap(value.firstSurf, operation);
        EndianSwap(value.surfCount, operation);
    }

    static inline void EndianSwap(GfxWaterBuffer& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.bufferSize, operation);
        EndianSwap(value.buffer, operation);
    }

    static inline void EndianSwap(Occluder& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flags, operation);
        EndianSwap(value.points, operation);
    }

    static inline void EndianSwap(GfxOutdoorBounds& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.bounds, operation);
    }

    static inline void EndianSwap(GfxHeroLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.color, operation);
        EndianSwap(value.dir, operation);
        EndianSwap(value.origin, operation);
        EndianSwap(value.radius, operation);
        EndianSwap(value.cosHalfFovOuter, operation);
        EndianSwap(value.cosHalfFovInner, operation);
        EndianSwap(value.exponent, operation);
    }

    static inline void EndianSwap(GfxHeroLightTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
    }

    static inline void EndianSwap(GfxWorld& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name, operation);
        EndianSwap(value.baseName, operation);
        EndianSwap(value.planeCount, operation);
        EndianSwap(value.nodeCount, operation);
        EndianSwap(value.surfaceCount, operation);
        EndianSwap(value.streamInfo, operation);
        EndianSwap(value.xmodelStreamInfo, operation);
        EndianSwap(value.skySurfCount, operation);
        EndianSwap(value.skyStartSurfs, operation);
        EndianSwap(value.skyImage, operation);
        EndianSwap(value.skyBoxModel, operation);
        EndianSwap(value.sunParse, operation);
        EndianSwap(value.sunLight, operation);
        EndianSwap(value.sunColorFromBsp, operation);
        EndianSwap(value.sunPrimaryLightIndex, operation);
        EndianSwap(value.primaryLightCount, operation);
        EndianSwap(value.cullGroupCount, operation);
        EndianSwap(value.coronaCount, operation);
        EndianSwap(value.coronas, operation);
        EndianSwap(value.shadowMapVolumeCount, operation);
        EndianSwap(value.shadowMapVolumes, operation);
        EndianSwap(value.shadowMapVolumePlaneCount, operation);
        EndianSwap(value.shadowMapVolumePlanes, operation);
        EndianSwap(value.exposureVolumeCount, operation);
        EndianSwap(value.exposureVolumes, operation);
        EndianSwap(value.exposureVolumePlaneCount, operation);
        EndianSwap(value.exposureVolumePlanes, operation);
        EndianSwap(value.skyDynIntensity, operation);
        EndianSwap(value.dpvsPlanes, operation);
        EndianSwap(value.cellBitsCount, operation);
        EndianSwap(value.cells, operation);
        EndianSwap(value.draw, operation);
        EndianSwap(value.lightGrid, operation);
        EndianSwap(value.modelCount, operation);
        EndianSwap(value.models, operation);
        EndianSwap(value.mins, operation);
        EndianSwap(value.maxs, operation);
        EndianSwap(value.checksum, operation);
        EndianSwap(value.materialMemoryCount, operation);
        EndianSwap(value.materialMemory, operation);
        EndianSwap(value.sun, operation);
        EndianSwap(value.outdoorLookupMatrix, operation);
        EndianSwap(value.outdoorImage, operation);
        EndianSwap(value.cellCasterBits, operation);
        EndianSwap(value.sceneDynModel, operation);
        EndianSwap(value.sceneDynBrush, operation);
        EndianSwap(value.primaryLightEntityShadowVis, operation);
        EndianSwap(value.primaryLightDynEntShadowVis, operation);
        EndianSwap(value.nonSunPrimaryLightForModelDynEnt, operation);
        EndianSwap(value.shadowGeom, operation);
        EndianSwap(value.lightRegion, operation);
        EndianSwap(value.dpvs, operation);
        EndianSwap(value.dpvsDyn, operation);
        EndianSwap(value.worldLodChainCount, operation);
        EndianSwap(value.worldLodChains, operation);
        EndianSwap(value.worldLodInfoCount, operation);
        EndianSwap(value.worldLodInfos, operation);
        EndianSwap(value.worldLodSurfaceCount, operation);
        EndianSwap(value.worldLodSurfaces, operation);
        EndianSwap(value.waterDirection, operation);
        EndianSwap(value.waterBuffers[0], operation);
        EndianSwap(value.waterBuffers[1], operation);
        EndianSwap(value.waterMaterial, operation);
        EndianSwap(value.coronaMaterial, operation);
        EndianSwap(value.ropeMaterial, operation);
        EndianSwap(value.numOccluders, operation);
        EndianSwap(value.occluders, operation);
        EndianSwap(value.numOutdoorBounds, operation);
        EndianSwap(value.outdoorBounds, operation);
        EndianSwap(value.heroLightCount, operation);
        EndianSwap(value.heroLightTreeCount, operation);
        EndianSwap(value.heroLights, operation);
        EndianSwap(value.heroLightTree, operation);
    }

    static inline void EndianSwapPartial(XAnimPartTrans& value, [[maybe_unused]] size_t loadedSize, EndianOperation operation)
    {
        assert(loadedSize == offsetof(XAnimPartTrans, u));
        EndianSwap(value, operation);
    }

    static inline void EndianSwapPartial(XAnimPartTransFrames& value, [[maybe_unused]] size_t loadedSize, EndianOperation operation)
    {
        assert(loadedSize == offsetof(XAnimPartTransFrames, indices));
        EndianSwap(value, operation);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartQuat& value, [[maybe_unused]] size_t loadedSize, EndianOperation operation)
    {
        assert(loadedSize == offsetof(XAnimDeltaPartQuat, u));
        EndianSwap(value, operation);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartQuatDataFrames& value, [[maybe_unused]] size_t loadedSize, EndianOperation operation)
    {
        assert(loadedSize == offsetof(XAnimDeltaPartQuatDataFrames, indices));
        EndianSwap(value, operation);
    }

    static inline void EndianSwapPartial(MaterialTechnique& value, [[maybe_unused]] size_t loadedSize, EndianOperation operation)
    {
        assert(loadedSize == offsetof(MaterialTechnique, passArray));
        EndianSwap(value, operation);
    }
} // namespace T5Xenon
