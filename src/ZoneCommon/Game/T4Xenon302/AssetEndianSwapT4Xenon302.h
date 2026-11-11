#pragma once

#include "Game/T4Xenon302/T4Xenon302.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace T4Xenon302
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

    static inline void EndianSwap(XModelSurfs& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(XModelLodInfo& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(D3DVertexDeclaration& v, EndianOperation operation = EndianOperation::Decode);
    static inline void EndianSwap(MaterialStreamRouting& v, EndianOperation operation = EndianOperation::Decode);
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

    static inline void EndianSwap(XSurface& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.tileMode);
        EndianSwap(v.deformed);
        EndianSwap(v.vertCount);
        EndianSwap(v.triCount);
        EndianSwap(v.unknown_0x06);
        EndianSwap(v.triIndices);
        EndianSwap(v.vertInfo);
        EndianSwap(v.verts);
        EndianSwap(v.vertListCount);
        EndianSwap(v.vertList);
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

    static inline void EndianSwap(XModelStreamInfo& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.highMipBounds);
    }

    static inline void EndianSwap(PhysGeomList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count);
        EndianSwap(v.geoms);
        EndianSwap(v.mass.centerOfMass);
        EndianSwap(v.mass.momentsOfInertia);
        EndianSwap(v.mass.productsOfInertia);
    }

    static inline void EndianSwap(PhysGeomInfo& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.brush);
        EndianSwap(v.unknown_0x04);
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

    static inline void EndianSwap(XModel& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.name);
        EndianSwap(v.numBones);
        EndianSwap(v.numRootBones);
        EndianSwap(v.boneNames);
        EndianSwap(v.parentList);
        EndianSwap(v.quats);
        EndianSwap(v.trans);
        EndianSwap(v.partClassification);
        EndianSwap(v.baseMat);
        EndianSwap(v.lodInfo);
        EndianSwap(v.collSurfs);
        EndianSwap(v.numCollSurfs);
        EndianSwap(v.contents);
        EndianSwap(v.boneInfo);
        EndianSwap(v.unknown_0xd0);
        EndianSwap(v.streamInfo);
        EndianSwap(v.unknown_0xf4);
        EndianSwap(v.physPreset);
        EndianSwap(v.physGeoms);
    }

    static inline void EndianSwap(MaterialVertexDeclaration& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.routing);
        EndianSwap(v.streamCount);
        EndianSwap(v.decl);
    }

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

    static inline void EndianSwap(MaterialInfo& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.name);
        EndianSwap(v.unknownName);
        EndianSwap(v.drawSurf);
        EndianSwap(v.gameFlags);
        EndianSwap(v.sortKey);
        EndianSwap(v.textureAtlasRowCount);
        EndianSwap(v.textureAtlasColumnCount);
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

    static inline void EndianSwap(Material& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.info);
        EndianSwap(v.stateBitsEntry);
        EndianSwap(v.unknown_0x3e);
        EndianSwap(v.textureCount);
        EndianSwap(v.constantCount);
        EndianSwap(v.stateBitsCount);
        EndianSwap(v.techniqueSet);
        EndianSwap(v.textureTable);
        EndianSwap(v.constantTable);
        EndianSwap(v.stateBitsTable);
        EndianSwap(v.unknown_0x54);
    }

    static inline void EndianSwap(MaterialTechniqueSet& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.name);
        EndianSwap(v.worldVertFormat);
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
        case T4Xenon302::MTL_ARG_LITERAL_VERTEX_CONST:
        case T4Xenon302::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(v.u.literalConst);
            break;
        case T4Xenon302::MTL_ARG_CODE_VERTEX_CONST:
        case T4Xenon302::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(v.u.codeConst.index);
            break;
        case T4Xenon302::MTL_ARG_UNKNOWN_4:
            EndianSwap(v.u.codeSampler);
            break;
        case T4Xenon302::MTL_ARG_UNKNOWN_2:
        case T4Xenon302::MTL_ARG_UNKNOWN_3:
        case T4Xenon302::MTL_ARG_UNKNOWN_5:
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

    static inline void EndianSwap(MaterialPass& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.vertexDecl);
        EndianSwap(v.vertexShaderArray);
        EndianSwap(v.vertexShader);
        EndianSwap(v.pixelShader);
        EndianSwap(v.perPrimArgCount);
        EndianSwap(v.perObjArgCount);
        EndianSwap(v.stableArgCount);
        EndianSwap(v.unknownArgCount);
        EndianSwap(v.unknown_0x50);
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

    static inline void EndianSwap(snd_alias_t& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.aliasName);
        EndianSwap(v.subtitle);
        EndianSwap(v.secondaryAliasName);
        EndianSwap(v.chainAliasName);
        EndianSwap(v.soundFile);
        EndianSwap(v.unknown_0x14);
        EndianSwap(v.volumeFalloffCurve);
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

    static inline void EndianSwap(DynEntityClient& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.unknown);
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

    static inline void EndianSwap(XModelPieces& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.name);
        EndianSwap(v.numpieces);
        EndianSwap(v.pieces);
    }

    static inline void EndianSwap(cLeafBrushNodeLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(cLeafBrushNodeData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        assert(false);
    }

    static inline void EndianSwap(XModelPiece& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.model);
        EndianSwap(v.offset);
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

    static inline void EndianSwap(clipMap_t& v, const EndianOperation operation)
    {
        (void)operation;
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
        EndianSwap(v.numEntityChars);
        EndianSwap(v.entityString);
        EndianSwap(v.box_brush);
        EndianSwap(v.box_model);
        EndianSwap(v.path);
        EndianSwap(v.dynEntCount);
        EndianSwap(v.dynEntDefList);
        EndianSwap(v.dynEntClientList);
        EndianSwap(v.dynEntServerList);
        EndianSwap(v.dynEntCollList);
        EndianSwap(v.checksum);
    }

    static inline void EndianSwap(ComPrimaryLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.type);
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

    static inline void EndianSwap(pathnode_t& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.constant);
        EndianSwap(v.unknown_0x40);
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

    static inline void EndianSwap(PathData& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
        EndianSwap(v.chainNodeCount);
        EndianSwap(v.chainNodeForNode);
        EndianSwap(v.nodeForChainNode);
        EndianSwap(v.visBytes);
        EndianSwap(v.pathVis);
        EndianSwap(v.nodeTreeCount);
        EndianSwap(v.nodeTree);
    }

    static inline void EndianSwap(pathnode_constant_t& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.type);
        EndianSwap(v.spawnflags);
        EndianSwap(v.targetname);
        EndianSwap(v.script_linkName);
        EndianSwap(v.script_noteworthy);
        EndianSwap(v.target);
        EndianSwap(v.animscript);
        EndianSwap(v.animscriptfunc);
        EndianSwap(v.vOrigin);
        EndianSwap(v.fAngle);
        EndianSwap(v.forward);
        EndianSwap(v.fRadius);
        EndianSwap(v.wOverlapNode);
        EndianSwap(v.wChainId);
        EndianSwap(v.wChainDepth);
        EndianSwap(v.wChainParent);
        EndianSwap(v.totalLinkCount);
        EndianSwap(v.Links);
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

    static inline void EndianSwap(GfxStaticModelDrawInst& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.cullDist);
        EndianSwap(v.placement.origin);
        for (auto& axis : v.placement.axis)
            EndianSwap(axis.packed);
        EndianSwap(v.placement.scale);
        EndianSwap(v.model);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.groundLighting, operation);
        EndianSwap(v.cachedSurfaces);
        EndianSwap(v.reflectionProbeIndex);
    }

    static inline void EndianSwap(GfxWorldStreamInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTreeCount);
        EndianSwap(v.aabbTrees);
        EndianSwap(v.leafRefCount);
        EndianSwap(v.leafRefs);
    }

    static inline void EndianSwap(GfxLight& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.type);
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.exponent);
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

    static inline void EndianSwap(GfxWorld& v, const EndianOperation operation)
    {
        (void)operation;
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
        EndianSwap(v.streamInfo);
        EndianSwap(v.skySurfCount);
        EndianSwap(v.skyStartSurfs);
        EndianSwap(v.skyImage);
        EndianSwap(v.unknown_0x64);
        EndianSwap(v.vertexCount);
        EndianSwap(v.vd);
        EndianSwap(v.vertexLayerDataSize);
        EndianSwap(v.vld);
        EndianSwap(v.unknown_0xb8);
        EndianSwap(v.sunLight);
        EndianSwap(v.unknown_0x13c);
        EndianSwap(v.primaryLights);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.sunPrimaryLightIndex);
        EndianSwap(v.cullGroupCount);
        EndianSwap(v.cullGroups);
        EndianSwap(v.smodelCount);
        EndianSwap(v.smodelDrawInsts);
        EndianSwap(v.reflectionProbeCount);
        EndianSwap(v.reflectionProbes);
        EndianSwap(v.cellCount);
        EndianSwap(v.cellBitsCount);
        EndianSwap(v.cells);
        EndianSwap(v.lightmapCount);
        EndianSwap(v.lightmaps);
        EndianSwap(v.lightGrid);
        EndianSwap(v.modelCount);
        EndianSwap(v.models);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.checksum);
        EndianSwap(v.materialMemoryCount);
        EndianSwap(v.materialMemory);
        EndianSwap(v.sun);
        EndianSwap(v.outdoorLookupMatrix);
        EndianSwap(v.outdoorImage);
        EndianSwap(v.dynEntClientCount);
        EndianSwap(v.dynEntRefsCount);
        EndianSwap(v.smodelVisData);
        EndianSwap(v.surfaceVisData);
        EndianSwap(v.surfaceMaterials);
        EndianSwap(v.entCellBits);
        EndianSwap(v.dynEntVisData00);
        EndianSwap(v.dynEntVisData01);
        EndianSwap(v.dynEntVisData02);
        EndianSwap(v.dynEntVisData10);
        EndianSwap(v.dynEntVisData11);
        EndianSwap(v.dynEntVisData12);
        EndianSwap(v.dynEntCellBits);
        EndianSwap(v.dynEntVisList);
        EndianSwap(v.dynEntRefs);
        EndianSwap(v.sceneDynModel);
        EndianSwap(v.sceneDynBrush);
        EndianSwap(v.sortedSurfIndex);
    }

    static inline void EndianSwap(GfxLightImage& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.image);
        EndianSwap(v.samplerState);
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

    static inline void EndianSwap(statement_s& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.numEntries);
        EndianSwap(v.entries);
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
        case T4Xenon302::VAL_INT:
            EndianSwap(v.internals.intVal);
            break;
        case T4Xenon302::VAL_FLOAT:
            EndianSwap(v.internals.floatVal);
            break;
        case T4Xenon302::VAL_STRING:
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
        case T4Xenon302::EET_OPERATOR:
            EndianSwap(v.data.op);
            break;
        case T4Xenon302::EET_OPERAND:
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

    static inline void EndianSwap(listBoxDef_s& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.unknown_0x00);
        EndianSwap(v.doubleClick);
        EndianSwap(v.unknown_0xf8);
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

    static inline void EndianSwap(WeaponDef& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.szInternalName);
        EndianSwap(v.szDisplayName);
        EndianSwap(v.szOverlayName);
        EndianSwap(v.gunXModel);
        EndianSwap(v.handXModel);
        EndianSwap(v.szXAnims);
        EndianSwap(v.szModeName);
        EndianSwap(v.unknown_0x98);
        EndianSwap(v.viewFlashEffect);
        EndianSwap(v.worldFlashEffect);
        EndianSwap(v.unknownSounds);
        EndianSwap(v.bounceSound);
        EndianSwap(v.unknownEffects);
        EndianSwap(v.reticleCenter);
        EndianSwap(v.reticleSide);
        EndianSwap(v.unknown_0x198);
        EndianSwap(v.unknownModel_0x230);
        EndianSwap(v.unknownModel_0x234);
        EndianSwap(v.unknownMaterial_0x238);
        EndianSwap(v.unknownMaterial_0x23c);
        EndianSwap(v.unknown_0x240);
        EndianSwap(v.unknownString_0x244);
        EndianSwap(v.unknown_0x248);
        EndianSwap(v.unknownString_0x24c);
        EndianSwap(v.unknown_0x250);
        EndianSwap(v.unknownString_0x260);
        EndianSwap(v.unknown_0x264);
        EndianSwap(v.unknownMaterial_0x32c);
        EndianSwap(v.unknown_0x330);
        EndianSwap(v.unknownMaterial_0x414);
        EndianSwap(v.unknown_0x418);
        EndianSwap(v.unknownString_0x430);
        EndianSwap(v.unknown_0x434);
        EndianSwap(v.unknownModel_0x470);
        EndianSwap(v.unknown_0x474);
        EndianSwap(v.unknownEffect_0x478);
        EndianSwap(v.unknownEffect_0x47c);
        EndianSwap(v.unknownSound_0x480);
        EndianSwap(v.unknownSound_0x484);
        EndianSwap(v.unknown_0x488);
        EndianSwap(v.unknownEffect_0x588);
        EndianSwap(v.unknown_0x58c);
        EndianSwap(v.unknownEffect_0x5a4);
        EndianSwap(v.unknownSound_0x5a8);
        EndianSwap(v.unknown_0x5ac);
        EndianSwap(v.aiVsAiAccuracyGraphName);
        EndianSwap(v.aiVsPlayerAccuracyGraphName);
        EndianSwap(v.aiVsAiAccuracyGraphKnots);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnots);
        EndianSwap(v.originalAiVsAiAccuracyGraphKnots);
        EndianSwap(v.originalAiVsPlayerAccuracyGraphKnots);
        EndianSwap(v.aiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnotCount);
        EndianSwap(v.unknown_0x66c);
        EndianSwap(v.unknownString_0x6bc);
        EndianSwap(v.unknownString_0x6c0);
        EndianSwap(v.unknown_0x6c4);
        EndianSwap(v.unknownString_0x6d4);
        EndianSwap(v.unknown_0x6d8);
        EndianSwap(v.unknownString_0x748);
        EndianSwap(v.unknownString_0x74c);
        EndianSwap(v.unknown_0x750);
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

    static inline void EndianSwap(FxTrailVertex& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.unknown);
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

    static inline void EndianSwap(XModelSurfs& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.surfs);
        EndianSwap(v.unknown_0x04);
    }

    static inline void EndianSwap(XModelLodInfo& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.dist);
        EndianSwap(v.unknownName);
        EndianSwap(v.numsurfs);
        EndianSwap(v.unknown_0x0a);
        EndianSwap(v.modelSurfs);
        EndianSwap(v.materialHandles);
    }

    static inline void EndianSwap(D3DVertexDeclaration& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.unknown);
    }

    static inline void EndianSwap(MaterialStreamRouting& v, const EndianOperation operation)
    {
        (void)operation;
        EndianSwap(v.source);
        EndianSwap(v.dest);
    }
} // namespace T4Xenon302
