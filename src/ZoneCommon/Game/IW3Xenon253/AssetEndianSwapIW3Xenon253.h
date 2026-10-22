#pragma once

#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace IW3Xenon253
{
    enum class EndianOperation
    {
        Decode,
        Encode
    };

    static inline void EndianSwap(uint8_t&) {}

    static inline void EndianSwap(int8_t&) {}

    static inline void EndianSwap(char&) {}

    static inline void EndianSwap(bool&) {}

    static inline void EndianSwap(uint16_t& value)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(int16_t& value)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(uint32_t& value)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(int32_t& value)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(uint64_t& value)
    {
        value = endianness::FromBigEndian(value);
    }

    static inline void EndianSwap(float& value)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(value));
    }

    template<typename T> static inline void EndianSwap(T*& value)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(value));
    }

    template<typename T>
        requires std::is_enum_v<T>
    static inline void EndianSwap(T& value)
    {
        EndianSwap(reinterpret_cast<std::underlying_type_t<T>&>(value));
    }

    template<typename T, size_t N> static inline void EndianSwap(T (&value)[N])
    {
        for (auto& item : value)
            EndianSwap(item);
    }

    template<typename T> static inline T EndianSwapDiscriminator(T& value, const EndianOperation operation)
    {
        if (operation == EndianOperation::Decode)
        {
            EndianSwap(value);
            return value;
        }
        const auto hostValue = value;
        EndianSwap(value);
        return hostValue;
    }

    static inline void EndianSwap(PhysPreset& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAnimNotifyInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAnimDeltaPart& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwapPartial(XAnimPartTrans& v, size_t loadedSize, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwapPartial(XAnimPartTransFrames& v, size_t loadedSize, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAnimDynamicFrames& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwapPartial(XAnimDeltaPartQuat& v, size_t loadedSize, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwapPartial(XAnimDeltaPartQuatDataFrames& v, size_t loadedSize, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAnimIndices& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAnimParts& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxColor& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxPackedVertex& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelHighMipBounds& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(DObjAnimMat_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelCollSurf_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XBoneInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XSurface& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XSurfaceVertexInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XRigidVertList& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelStreamInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(PhysGeomList& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(PhysGeomInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(BrushWrapper& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cbrushside_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModel& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialVertexDeclaration& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialConstantDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxStateBits& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialTextureDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialTextureDefInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialWaterDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(complex_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(water_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(Material& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialTechniqueSet& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialVertexShader& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialVertexShaderProgram& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxVertexShaderLoadDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialPixelShader& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialPixelShaderProgram& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxPixelShaderLoadDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialShaderArgument& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialArgumentDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialPass& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwapPartial(MaterialTechnique& v, size_t loadedSize, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(D3DVolumeTexture& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(D3DCubeTexture& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(D3DTexture& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxTexture& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxImageLoadDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxTextureLoad& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxImage& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAUDIOCHANNELMAPENTRY& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(snd_alias_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAUDIOCHANNELMAP& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(SoundFile& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(SpeakerMap& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(snd_alias_list_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(SndCurve& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XAUDIOPACKET_ALIGNED& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XaSeekTable& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XaSound& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(LoadedSound& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cplane_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(dmaterial_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cLeaf_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(CollisionBorder& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(CollisionAabbTree& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cmodel_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(DynEntityClient& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(DynEntityServer& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(DynEntityColl& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelPieces& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cLeafBrushNodeLeaf_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cLeafBrushNodeData_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelPiece& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(DynEntityDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cbrush_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(CollisionPartition& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cLeafBrushNode_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cNode_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(cStaticModel_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(clipMap_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(ComPrimaryLight& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(ComWorld& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathlink_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathnode_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathnode_tree_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(PathData& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathnode_constant_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathnode_tree_info_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(pathnode_tree_nodes_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GameWorldSp& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GameWorldMp& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MapEnts& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxStreamingAabbTree& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightGridEntry& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightGridColors& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldVertex& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightRegionAxis& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxStaticModelInst& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxCullGroup& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxDrawSurf& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxBrushModel& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxSceneDynModel& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxSceneDynBrush& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxDynEntCellRef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxReflectionProbe& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxCell& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxAabbTree& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxPortal& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightmapArray& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialMemory& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxShadowGeometry& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightRegion& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightRegionHull& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxSurface& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxStaticModelDrawInst& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldStreamInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLight& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldDpvsPlanes& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightGrid& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldVertexData& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldVertexLayerData& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(sunflare_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldDpvsStatic& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorldDpvsDynamic& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxWorld& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightImage& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(GfxLightDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(Glyph& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(Font_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MenuList& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(editFieldDef_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(windowDef_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(ItemKeyHandler& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(statement_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(operandInternalDataUnion& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(Operand& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(entryInternalData& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(expressionEntry& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(itemDef_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(itemDefData_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(listBoxDef_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(multiDef_s& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(menuDef_t& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(LocalizeEntry& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(snd_alias_list_name& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(SndAliasCustom& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(WeaponDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XaReverbSettings& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(SndDriverGlobals& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemVelStateSample& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemVisStateSample& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxTrailVertex& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemMarkVisuals& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemVisuals& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxElemDefVisuals& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxEffectDefRef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxTrailDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxEffectDef& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxImpactEntry& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(FxImpactTable& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(RawFile& v, EndianOperation operation = EndianOperation::Decode);

    static inline void EndianSwap(PhysPreset& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type);
        EndianSwap(v.mass);
        EndianSwap(v.bounce);
        EndianSwap(v.friction);
        EndianSwap(v.bulletForceScale);
        EndianSwap(v.explosiveForceScale);
        EndianSwap(v.sndAliasPrefix);
        EndianSwap(v.piecesSpreadFraction);
        EndianSwap(v.piecesUpwardVelocity);
    }

    static inline void EndianSwap(XAnimNotifyInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.time);
    }

    static inline void EndianSwap(XAnimDeltaPart& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.trans);
        EndianSwap(v.quat);
    }

    static inline void EndianSwapPartial(XAnimPartTrans& v, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void EndianSwapPartial(XAnimPartTransFrames& v, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins[0]);
        EndianSwap(v.mins[1]);
        EndianSwap(v.mins[2]);
        EndianSwap(v.size[0]);
        EndianSwap(v.size[1]);
        EndianSwap(v.size[2]);
        EndianSwap(v.frames._1);
    }

    static inline void EndianSwap(XAnimDynamicFrames& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartQuat& v, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartQuatDataFrames& v, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frames);
    }

    static inline void EndianSwap(XAnimIndices& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(XAnimParts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numframes);
        EndianSwap(v.framerate);
        EndianSwap(v.frequency);
        EndianSwap(v.names);
        EndianSwap(v.dataByteCount);
        EndianSwap(v.dataShortCount);
        EndianSwap(v.dataFloatCount);
        EndianSwap(v.dataByte);
        EndianSwap(v.dataShort);
        EndianSwap(v.dataFloat);
        EndianSwap(v.randomDataShortCount);
        EndianSwap(v.randomDataShort);
        EndianSwap(v.randomDataByteCount);
        EndianSwap(v.randomDataByte);
        EndianSwap(v.indexCount);
        EndianSwap(v.indices.data);
        EndianSwap(v.notify);
        EndianSwap(v.deltaPart);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(GfxColor& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(GfxPackedVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord.packed);
        EndianSwap(v.normal.packed);
        EndianSwap(v.tangent.packed);
    }

    static inline void EndianSwap(XModelHighMipBounds& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
    }

    static inline void EndianSwap(DObjAnimMat_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat);
        EndianSwap(v.trans);
        EndianSwap(v.transWeight);
    }

    static inline void EndianSwap(XModelCollSurf_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.boneIdx);
        EndianSwap(v.contents);
        EndianSwap(v.surfFlags);
    }

    static inline void EndianSwap(XBoneInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds);
        EndianSwap(v.offset);
        EndianSwap(v.radiusSquared);
    }

    static inline void EndianSwap(XSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.triCount);
        EndianSwap(v.triIndices);
        EndianSwap(v.vertInfo.vertCount);
        EndianSwap(v.vertInfo.vertsBlend);
        EndianSwap(v.verts);
        EndianSwap(v.vertListCount);
        EndianSwap(v.vertList);
        EndianSwap(v.partBits);
    }

    static inline void EndianSwap(XSurfaceVertexInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.vertsBlend);
    }

    static inline void EndianSwap(XRigidVertList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.boneOffset);
        EndianSwap(v.vertCount);
        EndianSwap(v.triOffset);
        EndianSwap(v.triCount);
    }

    static inline void EndianSwap(XModelStreamInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(PhysGeomList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count);
        EndianSwap(v.geoms);
        EndianSwap(v.mass.centerOfMass);
        EndianSwap(v.mass.momentsOfInertia);
        EndianSwap(v.mass.productsOfInertia);
    }

    static inline void EndianSwap(PhysGeomInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.brush);
        EndianSwap(v.type);
        EndianSwap(v.orientation);
        EndianSwap(v.offset);
        EndianSwap(v.halfLengths);
    }

    static inline void EndianSwap(BrushWrapper& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.contents);
        EndianSwap(v.maxs);
        EndianSwap(v.numsides);
        EndianSwap(v.sides);
        EndianSwap(v.axialMaterialNum);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.firstAdjacentSideOffsets);
        EndianSwap(v.totalEdgeCount);
        EndianSwap(v.planes);
    }

    static inline void EndianSwap(cbrushside_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.materialNum);
        EndianSwap(v.firstAdjacentSideOffset);
    }

    static inline void EndianSwap(XModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.boneNames);
        EndianSwap(v.parentList);
        EndianSwap(v.quats);
        EndianSwap(v.trans);
        EndianSwap(v.partClassification);
        EndianSwap(v.baseMat);
        EndianSwap(v.surfs);
        EndianSwap(v.materialHandles);
        for (int i = 0; i < 4; i++)
        {
            EndianSwap(v.lodInfo[i].dist);
            EndianSwap(v.lodInfo[i].numsurfs);
            EndianSwap(v.lodInfo[i].surfIndex);
            EndianSwap(v.lodInfo[i].partBits);
        }
        EndianSwap(v.collSurfs);
        EndianSwap(v.numCollSurfs);
        EndianSwap(v.contents);
        EndianSwap(v.boneInfo);
        EndianSwap(v.radius);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.numLods);
        EndianSwap(v.collLod);
        EndianSwap(v.streamInfo.highMipBounds);
        EndianSwap(v.memUsage);
        EndianSwap(v.physPreset);
        EndianSwap(v.physGeoms);
    }

    static inline void EndianSwap(MaterialVertexDeclaration& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(MaterialConstantDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.literal);
    }

    static inline void EndianSwap(GfxStateBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadBits);
    }

    static inline void EndianSwap(MaterialTextureDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.u.image);
    }

    static inline void EndianSwap(MaterialInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.drawSurf.packed);
    }

    static inline void EndianSwap(MaterialTextureDefInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.image);
    }

    static inline void EndianSwap(MaterialWaterDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.textureWidth);
        EndianSwap(v.horizontalWorldLength);
        EndianSwap(v.verticalWorldLength);
        EndianSwap(v.amplitude);
        EndianSwap(v.windSpeed);
        EndianSwap(v.windDirection);
        EndianSwap(v.map);
    }

    static inline void EndianSwap(complex_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.real);
        EndianSwap(v.imag);
    }

    static inline void EndianSwap(water_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable.floatTime);
        EndianSwap(v.H0);
        EndianSwap(v.wTerm);
        EndianSwap(v.M);
        EndianSwap(v.N);
        EndianSwap(v.Lx);
        EndianSwap(v.Lz);
        EndianSwap(v.gravity);
        EndianSwap(v.windvel);
        EndianSwap(v.winddir);
        EndianSwap(v.amplitude);
        EndianSwap(v.codeConstant);
        EndianSwap(v.image);
    }

    static inline void EndianSwap(Material& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.techniqueSet);
        EndianSwap(v.textureTable);
        EndianSwap(v.constantTable);
        EndianSwap(v.stateBitsTable);
    }

    static inline void EndianSwap(MaterialTechniqueSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.remappedTechniqueSet);
        EndianSwap(v.techniques);
    }

    static inline void EndianSwap(MaterialVertexShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        auto& ld = v.prog.loadDef;
        EndianSwap(ld.cachedPart);
        EndianSwap(ld.physicalPart);
        EndianSwap(ld.cachedPartSize);
        EndianSwap(ld.physicalPartSize);
    }

    static inline void EndianSwap(MaterialVertexShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxVertexShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(MaterialPixelShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        auto& ld = v.prog.loadDef;
        EndianSwap(ld.cachedPart);
        EndianSwap(ld.physicalPart);
        EndianSwap(ld.cachedPartSize);
        EndianSwap(ld.physicalPartSize);
    }

    static inline void EndianSwap(MaterialPixelShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxPixelShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(MaterialShaderArgument& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        EndianSwap(v.dest);
        switch (type)
        {
        case IW3Xenon253::MTL_ARG_LITERAL_VERTEX_CONST:
        case IW3Xenon253::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(v.u.literalConst);
            break;
        case IW3Xenon253::MTL_ARG_CODE_VERTEX_CONST:
        case IW3Xenon253::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(v.u.codeConst.index);
            break;
        case IW3Xenon253::MTL_ARG_CODE_PIXEL_SAMPLER:
            EndianSwap(v.u.codeSampler);
            break;
        case IW3Xenon253::MTL_ARG_MATERIAL_VERTEX_CONST:
        case IW3Xenon253::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
        case IW3Xenon253::MTL_ARG_MATERIAL_PIXEL_CONST:
            EndianSwap(v.u.nameHash);
            break;
        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(MaterialArgumentDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(MaterialPass& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertexDecl);
        EndianSwap(v.vertexShaderArray);
        EndianSwap(v.vertexShader);
        EndianSwap(v.pixelShader);
        EndianSwap(v.args);
    }

    static inline void EndianSwapPartial(MaterialTechnique& v, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.flags);
        EndianSwap(v.passCount);
    }

    static inline void EndianSwap(D3DVolumeTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        return;
    }

    static inline void EndianSwap(D3DCubeTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        return;
    }

    static inline void EndianSwap(D3DTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        return;
    }

    static inline void EndianSwap(GfxTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cubemap);
    }

    static inline void EndianSwap(GfxImageLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dimensions);
        EndianSwap(v.format);
        EndianSwap(v.texture, operation);
    }

    static inline void EndianSwap(GfxTextureLoad& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadDef);
    }

    static inline void EndianSwap(GfxImage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mapType);
        EndianSwap(v.texture.loadDef);
        EndianSwap(v.cardMemory.platform[0]);
        EndianSwap(v.width);
        EndianSwap(v.height);
        EndianSwap(v.depth);
        EndianSwap(v.pixels);
        EndianSwap(v.baseSize);
        EndianSwap(v.streamSlot);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(XAUDIOCHANNELMAPENTRY& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.Volume);
    }

    static inline void EndianSwap(snd_alias_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aliasName);
        EndianSwap(v.subtitle);
        EndianSwap(v.secondaryAliasName);
        EndianSwap(v.chainAliasName);
        EndianSwap(v.soundFile);
        EndianSwap(v.sequence);
        EndianSwap(v.volMin);
        EndianSwap(v.volMax);
        EndianSwap(v.pitchMin);
        EndianSwap(v.pitchMax);
        EndianSwap(v.distMin);
        EndianSwap(v.distMax);
        EndianSwap(v.flags);
        EndianSwap(v.slavePercentage);
        EndianSwap(v.probability);
        EndianSwap(v.lfePercentage);
        EndianSwap(v.centerPercentage);
        EndianSwap(v.startDelay);
        EndianSwap(v.volumeFalloffCurve);
        EndianSwap(v.envelopMin);
        EndianSwap(v.envelopMax);
        EndianSwap(v.envelopPercentage);
        EndianSwap(v.speakerMap);
    }

    static inline void EndianSwap(XAUDIOCHANNELMAP& v, [[maybe_unused]] const EndianOperation operation)
    {
        (void)v;
    }

    static inline void EndianSwap(SoundFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.name);
        EndianSwap(v.fileMem);
        EndianSwap(v.type);
    }

    static inline void EndianSwap(SpeakerMap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                EndianSwap(v.channelMaps[i][j].paEntries);
            }
        }
    }

    static inline void EndianSwap(snd_alias_list_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aliasName);
        EndianSwap(v.head);
        EndianSwap(v.count);
    }

    static inline void EndianSwap(SndCurve& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename);
        EndianSwap(v.knotCount);
        EndianSwap(v.knots);
    }

    static inline void EndianSwap(XAUDIOPACKET_ALIGNED& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pBuffer);
        EndianSwap(v.BufferSize);
        EndianSwap(v.LoopCount);
        for (int i = 0; i < 6; i++)
        {
            EndianSwap(v.XMALoop[i].LoopStart);
            EndianSwap(v.XMALoop[i].LoopEnd);
        }
        EndianSwap(v.pContext);
    }

    static inline void EndianSwap(XaSeekTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
        EndianSwap(v.data);
    }

    static inline void EndianSwap(XaSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packet, operation);
        EndianSwap(v.xaIwXmaDataInfo.totalMsec);
        EndianSwap(v.seekTable, operation);
    }

    static inline void EndianSwap(LoadedSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.sound, operation);
    }

    static inline void EndianSwap(cplane_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.normal);
        EndianSwap(v.dist);
    }

    static inline void EndianSwap(dmaterial_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfaceFlags);
        EndianSwap(v.contentFlags);
    }

    static inline void EndianSwap(cLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstCollAabbIndex);
        EndianSwap(v.collAabbCount);
        EndianSwap(v.brushContents);
        EndianSwap(v.terrainContents);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.leafBrushNode);
        EndianSwap(v.cluster);
    }

    static inline void EndianSwap(CollisionBorder& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.distEq);
        EndianSwap(v.zBase);
        EndianSwap(v.zSlope);
        EndianSwap(v.start);
        EndianSwap(v.length);
    }

    static inline void EndianSwap(CollisionAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        EndianSwap(v.halfSize);
        EndianSwap(v.materialIndex);
        EndianSwap(v.childCount);
        EndianSwap(v.u.firstChildIndex);
    }

    static inline void EndianSwap(cmodel_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.radius);
        EndianSwap(v.leaf, operation);
    }

    static inline void EndianSwap(DynEntityClient& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pose.quat);
        EndianSwap(v.pose.origin);
        EndianSwap(v.physObjId);
        EndianSwap(v.flags);
        EndianSwap(v.lightingHandle);
        EndianSwap(v.health);
    }

    static inline void EndianSwap(DynEntityServer& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pose.quat);
        EndianSwap(v.pose.origin);
        EndianSwap(v.flags);
        EndianSwap(v.health);
    }

    static inline void EndianSwap(DynEntityColl& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.sector);
        EndianSwap(v.nextEntInSector);
        EndianSwap(v.linkMins);
        EndianSwap(v.linkMaxs);
    }

    static inline void EndianSwap(XModelPieces& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(cLeafBrushNodeLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(cLeafBrushNodeData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(XModelPiece& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(DynEntityDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.type);
        EndianSwap(v.pose.quat);
        EndianSwap(v.pose.origin);
        EndianSwap(v.xModel);
        EndianSwap(v.brushModel);
        EndianSwap(v.physicsBrushModel);
        EndianSwap(v.destroyFx);
        EndianSwap(v.destroyPieces);
        EndianSwap(v.physPreset);
        EndianSwap(v.health);
        EndianSwap(v.mass.centerOfMass);
        EndianSwap(v.mass.momentsOfInertia);
        EndianSwap(v.mass.productsOfInertia);
    }

    static inline void EndianSwap(cbrush_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.contents);
        EndianSwap(v.maxs);
        EndianSwap(v.numsides);
        EndianSwap(v.sides);
        EndianSwap(v.axialMaterialNum);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.firstAdjacentSideOffsets);
    }

    static inline void EndianSwap(CollisionPartition& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstTri);
        EndianSwap(v.borders);
    }

    static inline void EndianSwap(cLeafBrushNode_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.leafBrushCount);
        EndianSwap(v.contents);
        if (v.axis >= 3)
        {
            EndianSwap(v.data.leaf.brushes);
        }
        else
        {
            EndianSwap(v.data.children.dist);
            EndianSwap(v.data.children.range);
            EndianSwap(v.data.children.childOffset);
        }
    }

    static inline void EndianSwap(cNode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.children);
    }

    static inline void EndianSwap(cStaticModel_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable.nextModelInWorldSector);
        EndianSwap(v.xmodel);
        EndianSwap(v.origin);
        EndianSwap(v.invScaledAxis);
        EndianSwap(v.absmin);
        EndianSwap(v.absmax);
    }

    static inline void EndianSwap(clipMap_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.planeCount);
        EndianSwap(v.planes);
        EndianSwap(v.numStaticModels);
        EndianSwap(v.staticModelList);
        EndianSwap(v.numMaterials);
        EndianSwap(v.materials);
        EndianSwap(v.numBrushSides);
        EndianSwap(v.brushsides);
        EndianSwap(v.numBrushEdges);
        EndianSwap(v.brushEdges);
        EndianSwap(v.numNodes);
        EndianSwap(v.nodes);
        EndianSwap(v.numLeafs);
        EndianSwap(v.leafs);
        EndianSwap(v.leafbrushNodesCount);
        EndianSwap(v.leafbrushNodes);
        EndianSwap(v.numLeafBrushes);
        EndianSwap(v.leafbrushes);
        EndianSwap(v.numLeafSurfaces);
        EndianSwap(v.leafsurfaces);
        EndianSwap(v.vertCount);
        EndianSwap(v.verts);
        EndianSwap(v.triCount);
        EndianSwap(v.triIndices);
        EndianSwap(v.triEdgeIsWalkable);
        EndianSwap(v.borderCount);
        EndianSwap(v.borders);
        EndianSwap(v.partitionCount);
        EndianSwap(v.partitions);
        EndianSwap(v.aabbTreeCount);
        EndianSwap(v.aabbTrees);
        EndianSwap(v.numSubModels);
        EndianSwap(v.cmodels);
        EndianSwap(v.numBrushes);
        EndianSwap(v.brushes);
        EndianSwap(v.numClusters);
        EndianSwap(v.clusterBytes);
        EndianSwap(v.visibility);
        EndianSwap(v.vised);
        EndianSwap(v.mapEnts);
        EndianSwap(v.box_brush);
        EndianSwap(v.box_model.mins);
        EndianSwap(v.box_model.maxs);
        EndianSwap(v.box_model.radius);
        EndianSwap(v.box_model.leaf.firstCollAabbIndex);
        EndianSwap(v.box_model.leaf.collAabbCount);
        EndianSwap(v.box_model.leaf.brushContents);
        EndianSwap(v.box_model.leaf.terrainContents);
        EndianSwap(v.box_model.leaf.mins);
        EndianSwap(v.box_model.leaf.maxs);
        EndianSwap(v.box_model.leaf.leafBrushNode);
        EndianSwap(v.box_model.leaf.cluster);
        EndianSwap(v.dynEntCount);
        EndianSwap(v.dynEntDefList);
        EndianSwap(v.dynEntClientList);
        EndianSwap(v.dynEntServerList);
        EndianSwap(v.dynEntCollList);
        EndianSwap(v.checksum);
    }

    static inline void EndianSwap(ComPrimaryLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.exponent);
        EndianSwap(v.rotationLimit);
        EndianSwap(v.translationLimit);
        EndianSwap(v.defName);
    }

    static inline void EndianSwap(ComWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.primaryLights);
    }

    static inline void EndianSwap(pathlink_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fDist);
        EndianSwap(v.nodeNum);
    }

    static inline void EndianSwap(pathnode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.constant.type);
        EndianSwap(v.constant.spawnflags);
        EndianSwap(v.constant.targetname);
        EndianSwap(v.constant.script_linkName);
        EndianSwap(v.constant.script_noteworthy);
        EndianSwap(v.constant.target);
        EndianSwap(v.constant.animscript);
        EndianSwap(v.constant.animscriptfunc);
        EndianSwap(v.constant.vOrigin);
        EndianSwap(v.constant.fAngle);
        EndianSwap(v.constant.forward);
        EndianSwap(v.constant.fRadius);
        EndianSwap(v.constant.wOverlapNode);
        EndianSwap(v.constant.wChainId);
        EndianSwap(v.constant.wChainDepth);
        EndianSwap(v.constant.wChainParent);
        EndianSwap(v.constant.totalLinkCount);
        EndianSwap(v.constant.Links);
        EndianSwap(v.dynamic.pOwner);
        EndianSwap(v.dynamic.iFreeTime);
        EndianSwap(v.dynamic.iValidTime);
        EndianSwap(v.dynamic.inPlayerLOSTime);
        EndianSwap(v.dynamic.wLinkCount);
        EndianSwap(v.dynamic.wOverlapCount);
        EndianSwap(v.dynamic.turretEntNumber);
        EndianSwap(v.dynamic.userCount);
        EndianSwap(v.transient.iSearchFrame);
        EndianSwap(v.transient.pNextOpen);
        EndianSwap(v.transient.pPrevOpen);
        EndianSwap(v.transient.pParent);
        EndianSwap(v.transient.fCost);
        EndianSwap(v.transient.fHeuristic);
        EndianSwap(v.transient.costFactor);
        EndianSwap(v.transient.inVolumeMark);
    }

    static inline void EndianSwap(pathnode_tree_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.axis);
        EndianSwap(v.dist);
        if (v.axis >= 0)
        {
            EndianSwap(v.u.child[0]);
            EndianSwap(v.u.child[1]);
        }
        else
        {
            EndianSwap(v.u.s.nodeCount);
            EndianSwap(v.u.s.nodes);
        }
    }

    static inline void EndianSwap(PathData& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(pathnode_constant_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(pathnode_tree_info_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(pathnode_tree_nodes_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
    }

    static inline void EndianSwap(GameWorldSp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.path.nodeCount);
        EndianSwap(v.path.nodes);
        EndianSwap(v.path.chainNodeCount);
        EndianSwap(v.path.chainNodeForNode);
        EndianSwap(v.path.nodeForChainNode);
        EndianSwap(v.path.visBytes);
        EndianSwap(v.path.pathVis);
        EndianSwap(v.path.nodeTreeCount);
        EndianSwap(v.path.nodeTree);
    }

    static inline void EndianSwap(GameWorldMp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(MapEnts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.entityString);
        EndianSwap(v.numEntityChars);
    }

    static inline void EndianSwap(GfxStreamingAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstItem);
        EndianSwap(v.itemCount);
        EndianSwap(v.firstChild);
        EndianSwap(v.childCount);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
    }

    static inline void EndianSwap(GfxLightGridEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.colorsIndex);
    }

    static inline void EndianSwap(GfxLightGridColors& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(GfxWorldVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord);
        EndianSwap(v.lmapCoord);
        EndianSwap(v.normal.packed);
        EndianSwap(v.tangent.packed);
    }

    static inline void EndianSwap(GfxLightRegionAxis& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.midPoint);
        EndianSwap(v.halfSize);
    }

    static inline void EndianSwap(GfxStaticModelInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.groundLighting, operation);
    }

    static inline void EndianSwap(GfxCullGroup& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
    }

    static inline void EndianSwap(GfxDrawSurf& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(GfxBrushModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable.mins);
        EndianSwap(v.writable.maxs);
        EndianSwap(v.bounds);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
    }

    static inline void EndianSwap(GfxSceneDynModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info.lod);
        EndianSwap(v.info.surfId);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(GfxSceneDynBrush& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info.surfId);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(GfxDynEntCellRef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dynEntIndex);
        EndianSwap(v.nextInCell);
        EndianSwap(v.prevInCell);
        EndianSwap(v.nextForDynEnt);
    }

    static inline void EndianSwap(GfxReflectionProbe& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        EndianSwap(v.reflectionImage);
    }

    static inline void EndianSwap(GfxCell& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.aabbTree);
        EndianSwap(v.portalCount);
        EndianSwap(v.portals);
        EndianSwap(v.cullGroupCount);
        EndianSwap(v.cullGroups);
    }

    static inline void EndianSwap(GfxAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
        EndianSwap(v.smodelIndexCount);
        EndianSwap(v.smodelIndexes);
        EndianSwap(v.childCount);
        EndianSwap(v.children);
    }

    static inline void EndianSwap(GfxPortal& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable.hullPoints);
        EndianSwap(v.writable.queuedParent);
        EndianSwap(v.plane.coeffs);
        EndianSwap(v.cell);
        EndianSwap(v.vertices);
        EndianSwap(v.hullAxis);
    }

    static inline void EndianSwap(GfxLightmapArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.primary);
        EndianSwap(v.secondary);
    }

    static inline void EndianSwap(MaterialMemory& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.material);
        EndianSwap(v.memory);
    }

    static inline void EndianSwap(GfxShadowGeometry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfaceCount);
        EndianSwap(v.smodelCount);
        EndianSwap(v.sortedSurfIndex);
        EndianSwap(v.smodelIndex);
    }

    static inline void EndianSwap(GfxLightRegion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hullCount);
        EndianSwap(v.hulls);
    }

    static inline void EndianSwap(GfxLightRegionHull& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.kdopMidPoint);
        EndianSwap(v.kdopHalfSize);
        EndianSwap(v.axisCount);
        EndianSwap(v.axis);
    }

    static inline void EndianSwap(GfxSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.tris.vertexLayerData);
        EndianSwap(v.tris.firstVertex);
        EndianSwap(v.tris.vertexCount);
        EndianSwap(v.tris.indexCount);
        EndianSwap(v.tris.baseIndex);
        EndianSwap(v.tris.topMipMins);
        EndianSwap(v.tris.topMipMaxs);
        EndianSwap(v.material);
        EndianSwap(v.bounds);
    }

    static inline void EndianSwap(GfxStaticModelDrawInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cullDist);
        EndianSwap(v.placement.origin);
        for (int i = 0; i < 3; i++)
            EndianSwap(v.placement.axis[i].packed);
        EndianSwap(v.placement.scale);
        EndianSwap(v.model);
    }

    static inline void EndianSwap(GfxWorldStreamInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTreeCount);
        EndianSwap(v.aabbTrees);
        EndianSwap(v.leafRefCount);
        EndianSwap(v.leafRefs);
    }

    static inline void EndianSwap(GfxLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.exponent);
        EndianSwap(v.spotShadowIndex);
        EndianSwap(v.def);
    }

    static inline void EndianSwap(GfxWorldDpvsPlanes& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxLightGrid& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.sunPrimaryLightIndex);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.rowAxis);
        EndianSwap(v.colAxis);
        EndianSwap(v.rowDataStart);
        EndianSwap(v.rawRowDataSize);
        EndianSwap(v.rawRowData);
        EndianSwap(v.entryCount);
        EndianSwap(v.entries);
        EndianSwap(v.colorCount);
        EndianSwap(v.colors);
    }

    static inline void EndianSwap(GfxWorldVertexData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertices);
    }

    static inline void EndianSwap(GfxWorldVertexLayerData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(sunflare_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.spriteMaterial);
        EndianSwap(v.flareMaterial);
        EndianSwap(v.spriteSize);
        EndianSwap(v.flareMinSize);
        EndianSwap(v.flareMinDot);
        EndianSwap(v.flareMaxSize);
        EndianSwap(v.flareMaxDot);
        EndianSwap(v.flareMaxAlpha);
        EndianSwap(v.flareFadeInTime);
        EndianSwap(v.flareFadeOutTime);
        EndianSwap(v.blindMinDot);
        EndianSwap(v.blindMaxDot);
        EndianSwap(v.blindMaxDarken);
        EndianSwap(v.blindFadeInTime);
        EndianSwap(v.blindFadeOutTime);
        EndianSwap(v.glareMinDot);
        EndianSwap(v.glareMaxDot);
        EndianSwap(v.glareMaxLighten);
        EndianSwap(v.glareFadeInTime);
        EndianSwap(v.glareFadeOutTime);
        EndianSwap(v.sunFxPosition);
    }

    static inline void EndianSwap(GfxWorldDpvsStatic& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxWorldDpvsDynamic& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.baseName);
        EndianSwap(v.planeCount);
        EndianSwap(v.planes);
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
        EndianSwap(v.indexCount);
        EndianSwap(v.indices);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.surfaces);
        EndianSwap(v.streamInfo, operation);
        EndianSwap(v.litSurfsBegin);
        EndianSwap(v.litSurfsEnd);
        EndianSwap(v.emissiveSurfsBegin);
        EndianSwap(v.emissiveSurfsEnd);
        EndianSwap(v.skySurfCount);
        EndianSwap(v.skyStartSurfs);
        EndianSwap(v.skyImage);
        EndianSwap(v.vertexCount);
        EndianSwap(v.vd, operation);
        EndianSwap(v.vertexLayerDataSize);
        EndianSwap(v.vld, operation);
        EndianSwap(v.sunParse.ambientScale);
        EndianSwap(v.sunParse.ambientColor);
        EndianSwap(v.sunParse.diffuseFraction);
        EndianSwap(v.sunParse.sunLight);
        EndianSwap(v.sunParse.sunColor);
        EndianSwap(v.sunParse.diffuseColor);
        EndianSwap(v.sunParse.angles);
        EndianSwap(v.sunLight);
        EndianSwap(v.sunColorFromBsp);
        EndianSwap(v.sunPrimaryLightIndex);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.cullGroupCount);
        EndianSwap(v.cullGroups);
        EndianSwap(v.smodelCount);
        EndianSwap(v.smodelDrawInsts);
        EndianSwap(v.smodelInsts);
        EndianSwap(v.reflectionProbeCount);
        EndianSwap(v.reflectionProbes);
        EndianSwap(v.cellCount);
        EndianSwap(v.cellBitsCount);
        EndianSwap(v.cells);
        EndianSwap(v.lightmapCount);
        EndianSwap(v.lightmaps);
        EndianSwap(v.lightGrid, operation);
        EndianSwap(v.modelCount);
        EndianSwap(v.models);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.checksum);
        EndianSwap(v.materialMemoryCount);
        EndianSwap(v.materialMemory);
        EndianSwap(v.sun, operation);
        EndianSwap(v.outdoorLookupMatrix);
        EndianSwap(v.outdoorImage);
        EndianSwap(v.dynEntClientCount);
        EndianSwap(v.dynEntRefsCount);
        EndianSwap(v.smodelVisData);
        EndianSwap(v.surfaceVisData);
        EndianSwap(v.surfaceMaterials);
        EndianSwap(v.surfaceCastsSunShadow);
        EndianSwap(v.cellCasterBits);
        EndianSwap(v.entCellBits);
        EndianSwap(v.dynEntVisData);
        EndianSwap(v.dynEntCellBits);
        EndianSwap(v.dynEntVisList);
        EndianSwap(v.dynEntRefs);
        EndianSwap(v.sceneDynModel);
        EndianSwap(v.sceneDynBrush);
        EndianSwap(v.sortedSurfIndex);
        EndianSwap(v.primaryLightEntityShadowVis);
        EndianSwap(v.primaryLightDynEntShadowVis);
        EndianSwap(v.shadowGeom);
    }

    static inline void EndianSwap(GfxLightImage& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(GfxLightDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.attenuation.image);
        EndianSwap(v.lmapLookupStart);
    }

    static inline void EndianSwap(Glyph& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.letter);
        EndianSwap(v.s0);
        EndianSwap(v.t0);
        EndianSwap(v.s1);
        EndianSwap(v.t1);
    }

    static inline void EndianSwap(Font_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fontName);
        EndianSwap(v.pixelHeight);
        EndianSwap(v.glyphCount);
        EndianSwap(v.material);
        EndianSwap(v.glyphs);
    }

    static inline void EndianSwap(MenuList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.menuCount);
        EndianSwap(v.menus);
    }

    static inline void EndianSwap(editFieldDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.minVal);
        EndianSwap(v.maxVal);
        EndianSwap(v.defVal);
        EndianSwap(v.range);
        EndianSwap(v.maxChars);
        EndianSwap(v.maxCharsGotoNext);
        EndianSwap(v.maxPaintChars);
        EndianSwap(v.paintOffset);
    }

    static inline void EndianSwap(windowDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.rect.x);
        EndianSwap(v.rect.y);
        EndianSwap(v.rect.w);
        EndianSwap(v.rect.h);
        EndianSwap(v.rect.horzAlign);
        EndianSwap(v.rect.vertAlign);
        EndianSwap(v.rectClient.x);
        EndianSwap(v.rectClient.y);
        EndianSwap(v.rectClient.w);
        EndianSwap(v.rectClient.h);
        EndianSwap(v.rectClient.horzAlign);
        EndianSwap(v.rectClient.vertAlign);
        EndianSwap(v.group);
        EndianSwap(v.style);
        EndianSwap(v.border);
        EndianSwap(v.ownerDraw);
        EndianSwap(v.ownerDrawFlags);
        EndianSwap(v.borderSize);
        EndianSwap(v.staticFlags);
        EndianSwap(v.dynamicFlags);
        EndianSwap(v.nextTime);
        EndianSwap(v.foreColor);
        EndianSwap(v.backColor);
        EndianSwap(v.borderColor);
        EndianSwap(v.outlineColor);
        EndianSwap(v.background);
    }

    static inline void EndianSwap(ItemKeyHandler& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.key);
        EndianSwap(v.action);
        EndianSwap(v.next);
    }

    static inline void EndianSwap(statement_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(operandInternalDataUnion& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(Operand& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto dataType = EndianSwapDiscriminator(v.dataType, operation);
        switch (dataType)
        {
        case IW3Xenon253::VAL_INT:
            EndianSwap(v.internals.intVal);
            break;
        case IW3Xenon253::VAL_FLOAT:
            EndianSwap(v.internals.floatVal);
            break;
        case IW3Xenon253::VAL_STRING:
            EndianSwap(v.internals.string);
            break;
        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(entryInternalData& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(expressionEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        switch (type)
        {
        case IW3Xenon253::EET_OPERATOR:
            EndianSwap(v.data.op);
            break;
        case IW3Xenon253::EET_OPERAND:
            EndianSwap(v.data.operand, operation);
            break;
        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(itemDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.window, operation);
        for (int i = 0; i < 4; i++)
        {
            EndianSwap(v.textRect[i].x);
            EndianSwap(v.textRect[i].y);
            EndianSwap(v.textRect[i].w);
            EndianSwap(v.textRect[i].h);
            EndianSwap(v.textRect[i].horzAlign);
            EndianSwap(v.textRect[i].vertAlign);
        }
        EndianSwap(v.type);
        EndianSwap(v.dataType);
        EndianSwap(v.alignment);
        EndianSwap(v.fontEnum);
        EndianSwap(v.textAlignMode);
        EndianSwap(v.textalignx);
        EndianSwap(v.textaligny);
        EndianSwap(v.textscale);
        EndianSwap(v.textStyle);
        EndianSwap(v.gameMsgWindowIndex);
        EndianSwap(v.gameMsgWindowMode);
        EndianSwap(v.text);
        EndianSwap(v.textSavegameInfo);
        EndianSwap(v.parent);
        EndianSwap(v.mouseEnterText);
        EndianSwap(v.mouseExitText);
        EndianSwap(v.mouseEnter);
        EndianSwap(v.mouseExit);
        EndianSwap(v.action);
        EndianSwap(v.onAccept);
        EndianSwap(v.onFocus);
        EndianSwap(v.leaveFocus);
        EndianSwap(v.dvar);
        EndianSwap(v.dvarTest);
        EndianSwap(v.onKey);
        EndianSwap(v.enableDvar);
        EndianSwap(v.dvarFlags);
        EndianSwap(v.focusSound);
        EndianSwap(v.special);
        EndianSwap(v.cursorPos);
        EndianSwap(v.typeData.data);
        EndianSwap(v.imageTrack);
        EndianSwap(v.visibleExp.numEntries);
        EndianSwap(v.visibleExp.entries);
        EndianSwap(v.textExp.numEntries);
        EndianSwap(v.textExp.entries);
        EndianSwap(v.materialExp.numEntries);
        EndianSwap(v.materialExp.entries);
    }

    static inline void EndianSwap(itemDefData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(listBoxDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.startPos);
        EndianSwap(v.endPos);
        EndianSwap(v.drawPadding);
        EndianSwap(v.elementWidth);
        EndianSwap(v.elementHeight);
        EndianSwap(v.elementStyle);
        EndianSwap(v.numColumns);
        for (int i = 0; i < 16; i++)
        {
            EndianSwap(v.columnInfo[i].pos);
            EndianSwap(v.columnInfo[i].width);
            EndianSwap(v.columnInfo[i].maxChars);
            EndianSwap(v.columnInfo[i].alignment);
        }
        EndianSwap(v.doubleClick);
        EndianSwap(v.notselectable);
        EndianSwap(v.noScrollBars);
        EndianSwap(v.usePaging);
        EndianSwap(v.selectBorder);
        EndianSwap(v.selectIcon);
    }

    static inline void EndianSwap(multiDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dvarList);
        EndianSwap(v.dvarStr);
        EndianSwap(v.dvarValue);
        EndianSwap(v.count);
        EndianSwap(v.strDef);
    }

    static inline void EndianSwap(menuDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.window, operation);
        EndianSwap(v.font);
        EndianSwap(v.fullScreen);
        EndianSwap(v.itemCount);
        EndianSwap(v.fontIndex);
        EndianSwap(v.cursorItem);
        EndianSwap(v.fadeCycle);
        EndianSwap(v.fadeClamp);
        EndianSwap(v.fadeAmount);
        EndianSwap(v.fadeInAmount);
        EndianSwap(v.blurRadius);
        EndianSwap(v.onOpen);
        EndianSwap(v.onClose);
        EndianSwap(v.onESC);
        EndianSwap(v.onKey);
        EndianSwap(v.visibleExp.numEntries);
        EndianSwap(v.visibleExp.entries);
        EndianSwap(v.soundName);
        EndianSwap(v.imageTrack);
        EndianSwap(v.focusColor);
        EndianSwap(v.disableColor);
        EndianSwap(v.items);
    }

    static inline void EndianSwap(LocalizeEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.value);
    }

    static inline void EndianSwap(snd_alias_list_name& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.soundName);
    }

    static inline void EndianSwap(SndAliasCustom& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(WeaponDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.szInternalName);
        EndianSwap(v.szDisplayName);
        EndianSwap(v.szOverlayName);
        EndianSwap(v.gunXModel);
        EndianSwap(v.handXModel);
        EndianSwap(v.szXAnims);
        EndianSwap(v.szModeName);
        EndianSwap(v.playerAnimType);
        EndianSwap(v.weapType);
        EndianSwap(v.weapClass);
        EndianSwap(v.penetrateType);
        EndianSwap(v.impactType);
        EndianSwap(v.inventoryType);
        EndianSwap(v.fireType);
        EndianSwap(v.offhandClass);
        EndianSwap(v.stance);
        EndianSwap(v.viewFlashEffect);
        EndianSwap(v.worldFlashEffect);
        EndianSwap(v.pickupSound.name);
        EndianSwap(v.pickupSoundPlayer.name);
        EndianSwap(v.ammoPickupSound.name);
        EndianSwap(v.ammoPickupSoundPlayer.name);
        EndianSwap(v.projectileSound.name);
        EndianSwap(v.pullbackSound.name);
        EndianSwap(v.pullbackSoundPlayer.name);
        EndianSwap(v.fireSound.name);
        EndianSwap(v.fireSoundPlayer.name);
        EndianSwap(v.fireLoopSound.name);
        EndianSwap(v.fireLoopSoundPlayer.name);
        EndianSwap(v.fireStopSound.name);
        EndianSwap(v.fireStopSoundPlayer.name);
        EndianSwap(v.fireLastSound.name);
        EndianSwap(v.fireLastSoundPlayer.name);
        EndianSwap(v.emptyFireSound.name);
        EndianSwap(v.emptyFireSoundPlayer.name);
        EndianSwap(v.meleeSwipeSound.name);
        EndianSwap(v.meleeSwipeSoundPlayer.name);
        EndianSwap(v.rechamberSound.name);
        EndianSwap(v.rechamberSoundPlayer.name);
        EndianSwap(v.reloadSound.name);
        EndianSwap(v.reloadSoundPlayer.name);
        EndianSwap(v.reloadEmptySound.name);
        EndianSwap(v.reloadEmptySoundPlayer.name);
        EndianSwap(v.reloadStartSound.name);
        EndianSwap(v.reloadStartSoundPlayer.name);
        EndianSwap(v.reloadEndSound.name);
        EndianSwap(v.reloadEndSoundPlayer.name);
        EndianSwap(v.detonateSound.name);
        EndianSwap(v.detonateSoundPlayer.name);
        EndianSwap(v.nightVisionWearSound.name);
        EndianSwap(v.nightVisionWearSoundPlayer.name);
        EndianSwap(v.nightVisionRemoveSound.name);
        EndianSwap(v.nightVisionRemoveSoundPlayer.name);
        EndianSwap(v.altSwitchSound.name);
        EndianSwap(v.altSwitchSoundPlayer.name);
        EndianSwap(v.raiseSound.name);
        EndianSwap(v.raiseSoundPlayer.name);
        EndianSwap(v.firstRaiseSound.name);
        EndianSwap(v.firstRaiseSoundPlayer.name);
        EndianSwap(v.putawaySound.name);
        EndianSwap(v.putawaySoundPlayer.name);
        EndianSwap(v.noteTrackSoundA.name);
        EndianSwap(v.noteTrackSoundB.name);
        EndianSwap(v.noteTrackSoundC.name);
        EndianSwap(v.noteTrackSoundD.name);
        EndianSwap(v.bounceSound);
        EndianSwap(v.viewShellEjectEffect);
        EndianSwap(v.worldShellEjectEffect);
        EndianSwap(v.viewLastShotEjectEffect);
        EndianSwap(v.worldLastShotEjectEffect);
        EndianSwap(v.reticleCenter);
        EndianSwap(v.reticleSide);
        EndianSwap(v.iReticleCenterSize);
        EndianSwap(v.iReticleSideSize);
        EndianSwap(v.iReticleMinOfs);
        EndianSwap(v.activeReticleType);
        EndianSwap(v.vStandMove);
        EndianSwap(v.vStandRot);
        EndianSwap(v.vDuckedOfs);
        EndianSwap(v.vDuckedMove);
        EndianSwap(v.vDuckedRot);
        EndianSwap(v.vProneOfs);
        EndianSwap(v.vProneMove);
        EndianSwap(v.vProneRot);
        EndianSwap(v.fPosMoveRate);
        EndianSwap(v.fPosProneMoveRate);
        EndianSwap(v.fStandMoveMinSpeed);
        EndianSwap(v.fDuckedMoveMinSpeed);
        EndianSwap(v.fProneMoveMinSpeed);
        EndianSwap(v.fPosRotRate);
        EndianSwap(v.fPosProneRotRate);
        EndianSwap(v.fStandRotMinSpeed);
        EndianSwap(v.fDuckedRotMinSpeed);
        EndianSwap(v.fProneRotMinSpeed);
        EndianSwap(v.worldModel);
        EndianSwap(v.worldClipModel);
        EndianSwap(v.rocketModel);
        EndianSwap(v.hudIcon);
        EndianSwap(v.hudIconRatio);
        EndianSwap(v.ammoCounterIcon);
        EndianSwap(v.ammoCounterIconRatio);
        EndianSwap(v.ammoCounterClip);
        EndianSwap(v.iStartAmmo);
        EndianSwap(v.szAmmoName);
        EndianSwap(v.iAmmoIndex);
        EndianSwap(v.szClipName);
        EndianSwap(v.iClipIndex);
        EndianSwap(v.iMaxAmmo);
        EndianSwap(v.iClipSize);
        EndianSwap(v.shotCount);
        EndianSwap(v.szSharedAmmoCapName);
        EndianSwap(v.iSharedAmmoCapIndex);
        EndianSwap(v.iSharedAmmoCap);
        EndianSwap(v.damage);
        EndianSwap(v.playerDamage);
        EndianSwap(v.iMeleeDamage);
        EndianSwap(v.iDamageType);
        EndianSwap(v.iFireDelay);
        EndianSwap(v.iMeleeDelay);
        EndianSwap(v.iDetonateDelay);
        EndianSwap(v.iFireTime);
        EndianSwap(v.iRechamberTime);
        EndianSwap(v.iRechamberBoltTime);
        EndianSwap(v.iHoldFireTime);
        EndianSwap(v.iDetonateTime);
        EndianSwap(v.iMeleeTime);
        EndianSwap(v.iReloadTime);
        EndianSwap(v.reloadShowRocketTime);
        EndianSwap(v.iReloadEmptyTime);
        EndianSwap(v.iReloadAddTime);
        EndianSwap(v.iReloadStartTime);
        EndianSwap(v.iReloadStartAddTime);
        EndianSwap(v.iReloadEndTime);
        EndianSwap(v.iDropTime);
        EndianSwap(v.iRaiseTime);
        EndianSwap(v.iAltDropTime);
        EndianSwap(v.iAltRaiseTime);
        EndianSwap(v.quickDropTime);
        EndianSwap(v.quickRaiseTime);
        EndianSwap(v.iFirstRaiseTime);
        EndianSwap(v.iEmptyRaiseTime);
        EndianSwap(v.iEmptyDropTime);
        EndianSwap(v.sprintInTime);
        EndianSwap(v.sprintLoopTime);
        EndianSwap(v.sprintOutTime);
        EndianSwap(v.nightVisionWearTime);
        EndianSwap(v.nightVisionWearTimeFadeOutEnd);
        EndianSwap(v.nightVisionWearTimePowerUp);
        EndianSwap(v.nightVisionRemoveTime);
        EndianSwap(v.nightVisionRemoveTimePowerDown);
        EndianSwap(v.nightVisionRemoveTimeFadeInStart);
        EndianSwap(v.fuseTime);
        EndianSwap(v.aiFuseTime);
        EndianSwap(v.autoAimRange);
        EndianSwap(v.slowdownAimRange);
        EndianSwap(v.slowdownAimRangeAds);
        EndianSwap(v.lockonAimRange);
        EndianSwap(v.lockonAimRangeAds);
        EndianSwap(v.aimPadding);
        EndianSwap(v.enemyCrosshairRange);
        EndianSwap(v.crosshairColorChange);
        EndianSwap(v.moveSpeedScale);
        EndianSwap(v.adsMoveSpeedScale);
        EndianSwap(v.sprintDurationScale);
        EndianSwap(v.fAdsZoomFov);
        EndianSwap(v.fAdsZoomInFrac);
        EndianSwap(v.fAdsZoomOutFrac);
        EndianSwap(v.overlayMaterial);
        EndianSwap(v.overlayReticle);
        EndianSwap(v.overlayInterface);
        EndianSwap(v.overlayWidth);
        EndianSwap(v.overlayHeight);
        EndianSwap(v.fAdsBobFactor);
        EndianSwap(v.fAdsViewBobMult);
        EndianSwap(v.fHipSpreadStandMin);
        EndianSwap(v.fHipSpreadDuckedMin);
        EndianSwap(v.fHipSpreadProneMin);
        EndianSwap(v.hipSpreadStandMax);
        EndianSwap(v.hipSpreadDuckedMax);
        EndianSwap(v.hipSpreadProneMax);
        EndianSwap(v.fHipSpreadDecayRate);
        EndianSwap(v.fHipSpreadFireAdd);
        EndianSwap(v.fHipSpreadTurnAdd);
        EndianSwap(v.fHipSpreadMoveAdd);
        EndianSwap(v.fHipSpreadDuckedDecay);
        EndianSwap(v.fHipSpreadProneDecay);
        EndianSwap(v.fHipReticleSidePos);
        EndianSwap(v.iAdsTransInTime);
        EndianSwap(v.iAdsTransOutTime);
        EndianSwap(v.fAdsIdleAmount);
        EndianSwap(v.fHipIdleAmount);
        EndianSwap(v.adsIdleSpeed);
        EndianSwap(v.hipIdleSpeed);
        EndianSwap(v.fIdleCrouchFactor);
        EndianSwap(v.fIdleProneFactor);
        EndianSwap(v.fGunMaxPitch);
        EndianSwap(v.fGunMaxYaw);
        EndianSwap(v.swayMaxAngle);
        EndianSwap(v.swayLerpSpeed);
        EndianSwap(v.swayPitchScale);
        EndianSwap(v.swayYawScale);
        EndianSwap(v.swayHorizScale);
        EndianSwap(v.swayVertScale);
        EndianSwap(v.swayShellShockScale);
        EndianSwap(v.adsSwayMaxAngle);
        EndianSwap(v.adsSwayLerpSpeed);
        EndianSwap(v.adsSwayPitchScale);
        EndianSwap(v.adsSwayYawScale);
        EndianSwap(v.adsSwayHorizScale);
        EndianSwap(v.adsSwayVertScale);
        EndianSwap(v.bRifleBullet);
        EndianSwap(v.armorPiercing);
        EndianSwap(v.bBoltAction);
        EndianSwap(v.aimDownSight);
        EndianSwap(v.bRechamberWhileAds);
        EndianSwap(v.adsViewErrorMin);
        EndianSwap(v.adsViewErrorMax);
        EndianSwap(v.bCookOffHold);
        EndianSwap(v.bClipOnly);
        EndianSwap(v.adsFireOnly);
        EndianSwap(v.cancelAutoHolsterWhenEmpty);
        EndianSwap(v.enhanced);
        EndianSwap(v.laserSightDuringNightvision);
        EndianSwap(v.killIcon);
        EndianSwap(v.killIconRatio);
        EndianSwap(v.flipKillIcon);
        EndianSwap(v.dpadIcon);
        EndianSwap(v.dpadIconRatio);
        EndianSwap(v.bNoPartialReload);
        EndianSwap(v.bSegmentedReload);
        EndianSwap(v.iReloadAmmoAdd);
        EndianSwap(v.iReloadStartAdd);
        EndianSwap(v.szAltWeaponName);
        EndianSwap(v.altWeaponIndex);
        EndianSwap(v.iDropAmmoMin);
        EndianSwap(v.iDropAmmoMax);
        EndianSwap(v.blocksProne);
        EndianSwap(v.silenced);
        EndianSwap(v.iExplosionRadius);
        EndianSwap(v.iExplosionRadiusMin);
        EndianSwap(v.iExplosionInnerDamage);
        EndianSwap(v.iExplosionOuterDamage);
        EndianSwap(v.damageConeAngle);
        EndianSwap(v.iProjectileSpeed);
        EndianSwap(v.iProjectileSpeedUp);
        EndianSwap(v.iProjectileSpeedForward);
        EndianSwap(v.iProjectileActivateDist);
        EndianSwap(v.projLifetime);
        EndianSwap(v.timeToAccelerate);
        EndianSwap(v.projectileCurvature);
        EndianSwap(v.projectileModel);
        EndianSwap(v.projExplosion);
        EndianSwap(v.projExplosionEffect);
        EndianSwap(v.projExplosionEffectForceNormalUp);
        EndianSwap(v.projDudEffect);
        EndianSwap(v.projExplosionSound.name);
        EndianSwap(v.projDudSound.name);
        EndianSwap(v.bProjImpactExplode);
        EndianSwap(v.stickiness);
        EndianSwap(v.hasDetonator);
        EndianSwap(v.timedDetonation);
        EndianSwap(v.rotate);
        EndianSwap(v.holdButtonToThrow);
        EndianSwap(v.parallelBounce);
        EndianSwap(v.perpendicularBounce);
        EndianSwap(v.projTrailEffect);
        EndianSwap(v.vProjectileColor);
        EndianSwap(v.guidedMissileType);
        EndianSwap(v.maxSteeringAccel);
        EndianSwap(v.projIgnitionDelay);
        EndianSwap(v.projIgnitionEffect);
        EndianSwap(v.projIgnitionSound.name);
        EndianSwap(v.fAdsAimPitch);
        EndianSwap(v.fAdsCrosshairInFrac);
        EndianSwap(v.fAdsCrosshairOutFrac);
        EndianSwap(v.adsGunKickReducedKickBullets);
        EndianSwap(v.adsGunKickReducedKickPercent);
        EndianSwap(v.fAdsGunKickPitchMin);
        EndianSwap(v.fAdsGunKickPitchMax);
        EndianSwap(v.fAdsGunKickYawMin);
        EndianSwap(v.fAdsGunKickYawMax);
        EndianSwap(v.fAdsGunKickAccel);
        EndianSwap(v.fAdsGunKickSpeedMax);
        EndianSwap(v.fAdsGunKickSpeedDecay);
        EndianSwap(v.fAdsGunKickStaticDecay);
        EndianSwap(v.fAdsViewKickPitchMin);
        EndianSwap(v.fAdsViewKickPitchMax);
        EndianSwap(v.fAdsViewKickYawMin);
        EndianSwap(v.fAdsViewKickYawMax);
        EndianSwap(v.fAdsViewKickCenterSpeed);
        EndianSwap(v.fAdsViewScatterMin);
        EndianSwap(v.fAdsViewScatterMax);
        EndianSwap(v.fAdsSpread);
        EndianSwap(v.hipGunKickReducedKickBullets);
        EndianSwap(v.hipGunKickReducedKickPercent);
        EndianSwap(v.fHipGunKickPitchMin);
        EndianSwap(v.fHipGunKickPitchMax);
        EndianSwap(v.fHipGunKickYawMin);
        EndianSwap(v.fHipGunKickYawMax);
        EndianSwap(v.fHipGunKickAccel);
        EndianSwap(v.fHipGunKickSpeedMax);
        EndianSwap(v.fHipGunKickSpeedDecay);
        EndianSwap(v.fHipGunKickStaticDecay);
        EndianSwap(v.fHipViewKickPitchMin);
        EndianSwap(v.fHipViewKickPitchMax);
        EndianSwap(v.fHipViewKickYawMin);
        EndianSwap(v.fHipViewKickYawMax);
        EndianSwap(v.fHipViewKickCenterSpeed);
        EndianSwap(v.fHipViewScatterMin);
        EndianSwap(v.fHipViewScatterMax);
        EndianSwap(v.fightDist);
        EndianSwap(v.maxDist);
        EndianSwap(v.aiVsAiAccuracyGraphName);
        EndianSwap(v.aiVsPlayerAccuracyGraphName);
        EndianSwap(v.aiVsAiAccuracyGraphKnots);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnots);
        EndianSwap(v.originalAiVsAiAccuracyGraphKnots);
        EndianSwap(v.originalAiVsPlayerAccuracyGraphKnots);
        EndianSwap(v.aiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnotCount);
        EndianSwap(v.originalAiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.originalAiVsPlayerAccuracyGraphKnotCount);
        EndianSwap(v.iPositionReloadTransTime);
        EndianSwap(v.leftArc);
        EndianSwap(v.rightArc);
        EndianSwap(v.topArc);
        EndianSwap(v.bottomArc);
        EndianSwap(v.accuracy);
        EndianSwap(v.aiSpread);
        EndianSwap(v.playerSpread);
        EndianSwap(v.minTurnSpeed);
        EndianSwap(v.maxTurnSpeed);
        EndianSwap(v.pitchConvergenceTime);
        EndianSwap(v.yawConvergenceTime);
        EndianSwap(v.suppressTime);
        EndianSwap(v.maxRange);
        EndianSwap(v.fAnimHorRotateInc);
        EndianSwap(v.fPlayerPositionDist);
        EndianSwap(v.szUseHintString);
        EndianSwap(v.dropHintString);
        EndianSwap(v.iUseHintStringIndex);
        EndianSwap(v.dropHintStringIndex);
        EndianSwap(v.horizViewJitter);
        EndianSwap(v.vertViewJitter);
        EndianSwap(v.szScript);
        EndianSwap(v.fOOPosAnimLength);
        EndianSwap(v.minDamage);
        EndianSwap(v.minPlayerDamage);
        EndianSwap(v.fMaxDamageRange);
        EndianSwap(v.fMinDamageRange);
        EndianSwap(v.destabilizationRateTime);
        EndianSwap(v.destabilizationCurvatureMax);
        EndianSwap(v.destabilizeDistance);
        EndianSwap(v.locationDamageMultipliers);
        EndianSwap(v.fireRumble);
        EndianSwap(v.meleeImpactRumble);
        EndianSwap(v.adsDofStart);
        EndianSwap(v.adsDofEnd);
        EndianSwap(v.hipDofStart);
        EndianSwap(v.hipDofEnd);
    }

    static inline void EndianSwap(XaReverbSettings& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.presetOverridden);
        EndianSwap(v.reverbSettings.ReflectionsDelay);
        EndianSwap(v.reverbSettings.RoomFilterFreq);
        EndianSwap(v.reverbSettings.RoomFilterMain);
        EndianSwap(v.reverbSettings.RoomFilterHF);
        EndianSwap(v.reverbSettings.ReflectionsGain);
        EndianSwap(v.reverbSettings.ReverbGain);
        EndianSwap(v.reverbSettings.DecayTime);
        EndianSwap(v.reverbSettings.Density);
        EndianSwap(v.reverbSettings.RoomSize);
    }

    static inline void EndianSwap(SndDriverGlobals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reverbSettings);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(FxElemVelStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.local.velocity.base);
        EndianSwap(v.local.velocity.amplitude);
        EndianSwap(v.local.totalDelta.base);
        EndianSwap(v.local.totalDelta.amplitude);
        EndianSwap(v.world.velocity.base);
        EndianSwap(v.world.velocity.amplitude);
        EndianSwap(v.world.totalDelta.base);
        EndianSwap(v.world.totalDelta.amplitude);
    }

    static inline void EndianSwap(FxElemVisStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base.rotationDelta);
        EndianSwap(v.base.rotationTotal);
        EndianSwap(v.base.size);
        EndianSwap(v.base.scale);
        EndianSwap(v.amplitude.rotationDelta);
        EndianSwap(v.amplitude.rotationTotal);
        EndianSwap(v.amplitude.size);
        EndianSwap(v.amplitude.scale);
    }

    static inline void EndianSwap(FxTrailVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pos);
        EndianSwap(v.normal);
        EndianSwap(v.texCoord);
    }

    static inline void EndianSwap(FxElemDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.flags);
        EndianSwap(v.spawn.looping.intervalMsec);
        EndianSwap(v.spawn.looping.count);
        EndianSwap(v.spawnRange.base);
        EndianSwap(v.spawnRange.amplitude);
        EndianSwap(v.fadeInRange.base);
        EndianSwap(v.fadeInRange.amplitude);
        EndianSwap(v.fadeOutRange.base);
        EndianSwap(v.fadeOutRange.amplitude);
        EndianSwap(v.spawnFrustumCullRadius);
        EndianSwap(v.spawnDelayMsec.base);
        EndianSwap(v.spawnDelayMsec.amplitude);
        EndianSwap(v.lifeSpanMsec.base);
        EndianSwap(v.lifeSpanMsec.amplitude);
        for (int i = 0; i < 3; i++)
        {
            EndianSwap(v.spawnOrigin[i].base);
            EndianSwap(v.spawnOrigin[i].amplitude);
        }
        EndianSwap(v.spawnOffsetRadius.base);
        EndianSwap(v.spawnOffsetRadius.amplitude);
        EndianSwap(v.spawnOffsetHeight.base);
        EndianSwap(v.spawnOffsetHeight.amplitude);
        for (int i = 0; i < 3; i++)
        {
            EndianSwap(v.spawnAngles[i].base);
            EndianSwap(v.spawnAngles[i].amplitude);
        }
        for (int i = 0; i < 3; i++)
        {
            EndianSwap(v.angularVelocity[i].base);
            EndianSwap(v.angularVelocity[i].amplitude);
        }
        EndianSwap(v.initialRotation.base);
        EndianSwap(v.initialRotation.amplitude);
        EndianSwap(v.gravity.base);
        EndianSwap(v.gravity.amplitude);
        EndianSwap(v.reflectionFactor.base);
        EndianSwap(v.reflectionFactor.amplitude);
        EndianSwap(v.atlas.entryCount);
        EndianSwap(v.velSamples);
        EndianSwap(v.visSamples);
        EndianSwap(v.visuals.markArray);
        EndianSwap(v.collMins);
        EndianSwap(v.collMaxs);
        EndianSwap(v.effectOnImpact.handle);
        EndianSwap(v.effectOnDeath.handle);
        EndianSwap(v.effectEmitted.handle);
        EndianSwap(v.emitDist.base);
        EndianSwap(v.emitDist.amplitude);
        EndianSwap(v.emitDistVariance.base);
        EndianSwap(v.emitDistVariance.amplitude);
        EndianSwap(v.trailDef);
    }

    static inline void EndianSwap(FxElemMarkVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.materials);
    }

    static inline void EndianSwap(FxElemVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.anonymous);
    }

    static inline void EndianSwap(FxElemDefVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.markArray);
    }

    static inline void EndianSwap(FxEffectDefRef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.handle);
    }

    static inline void EndianSwap(FxTrailDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.scrollTimeMsec);
        EndianSwap(v.repeatDist);
        EndianSwap(v.splitDist);
        EndianSwap(v.vertCount);
        EndianSwap(v.verts);
        EndianSwap(v.indCount);
        EndianSwap(v.inds);
    }

    static inline void EndianSwap(FxEffectDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.totalSize);
        EndianSwap(v.msecLoopingLife);
        EndianSwap(v.elemDefCountLooping);
        EndianSwap(v.elemDefCountOneShot);
        EndianSwap(v.elemDefCountEmission);
        EndianSwap(v.elemDefs);
    }

    static inline void EndianSwap(FxImpactEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nonflesh);
        EndianSwap(v.flesh);
    }

    static inline void EndianSwap(FxImpactTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.table);
    }

    static inline void EndianSwap(RawFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.len);
        EndianSwap(v.buffer);
    }
} // namespace IW3Xenon253
