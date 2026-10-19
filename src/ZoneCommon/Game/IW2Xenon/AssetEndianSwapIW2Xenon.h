#pragma once

#include "Game/IW2Xenon/IW2Xenon.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace IW2Xenon
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

    static inline void EndianSwap(uint16_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(int16_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(uint32_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(int32_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(uint64_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(float& v)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(v));
    }

    template<typename T> static inline void EndianSwap(T*& p)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(p));
    }

    template<typename T>
        requires std::is_enum_v<T>
    static inline void EndianSwap(T& v)
    {
        EndianSwap(reinterpret_cast<std::underlying_type_t<T>&>(v));
    }

    template<typename T, size_t N> static inline void EndianSwap(T (&v)[N])
    {
        for (size_t i = 0; i < N; i++)
            EndianSwap(v[i]);
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

    static inline void EndianSwap(pathnode_ptr& value, const EndianOperation operation)
    {
        EndianSwap(value.node);
    }

    static inline void EndianSwap(GfxOccluderRef& value, const EndianOperation operation)
    {
        EndianSwap(value.occluder);
    }

    static inline void EndianSwap(RawFile& value, EndianOperation operation);
    static inline void EndianSwap(LocalizeEntry& value, EndianOperation operation);
    static inline void EndianSwap(SndCurve& value, EndianOperation operation);
    static inline void EndianSwap(D3DTexture& value, EndianOperation operation);
    static inline void EndianSwap(GfxTexture& value, EndianOperation operation);
    static inline void EndianSwap(GfxImageLoadDef& value, EndianOperation operation);
    static inline void EndianSwap(GfxTextureLoad& value, EndianOperation operation);
    static inline void EndianSwap(GfxImage& value, EndianOperation operation);
    static inline void EndianSwap(MaterialInfo& value, EndianOperation operation);
    static inline void EndianSwap(MaterialStateMapRule& value, EndianOperation operation);
    static inline void EndianSwapPartial(MaterialStateMapRuleSet& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(MaterialStateMap& value, EndianOperation operation);
    static inline void EndianSwap(MaterialStreamRouting& value, EndianOperation operation);
    static inline void EndianSwap(D3DVertexDeclaration& value, EndianOperation operation);
    static inline void EndianSwap(MaterialVertexDeclaration& value, EndianOperation operation);
    static inline void EndianSwap(MaterialShader& value, EndianOperation operation);
    static inline void EndianSwap(MaterialArgumentCodeConst& value, EndianOperation operation);
    static inline void EndianSwap(MaterialArgumentDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialShaderArgument& value, EndianOperation operation);
    static inline void EndianSwap(MaterialPassDx9& value, EndianOperation operation);
    static inline void EndianSwapPartial(MaterialTechnique& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(MaterialTechniqueSet& value, EndianOperation operation);
    static inline void EndianSwap(complex_s& value, EndianOperation operation);
    static inline void EndianSwap(water_t& value, EndianOperation operation);
    static inline void EndianSwap(MaterialWaterDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTextureDefInfo& value, EndianOperation operation);
    static inline void EndianSwap(MaterialTextureDef& value, EndianOperation operation);
    static inline void EndianSwap(MaterialConstantDef& value, EndianOperation operation);
    static inline void EndianSwap(Material& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightImage& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightDef& value, EndianOperation operation);
    static inline void EndianSwap(Glyph& value, EndianOperation operation);
    static inline void EndianSwap(Font_s& value, EndianOperation operation);
    static inline void EndianSwap(XaReverbSettings& value, EndianOperation operation);
    static inline void EndianSwap(SndDriverGlobals& value, EndianOperation operation);
    static inline void EndianSwap(cplane_t& value, EndianOperation operation);
    static inline void EndianSwap(dmaterial_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeaf_t& value, EndianOperation operation);
    static inline void EndianSwap(CollisionVertex& value, EndianOperation operation);
    static inline void EndianSwap(CollisionEdge& value, EndianOperation operation);
    static inline void EndianSwap(CollisionTriangle& value, EndianOperation operation);
    static inline void EndianSwap(CollisionBorder& value, EndianOperation operation);
    static inline void EndianSwap(CollisionAabbTree& value, EndianOperation operation);
    static inline void EndianSwap(cmodel_t& value, EndianOperation operation);
    static inline void EndianSwap(pathlink_t& value, EndianOperation operation);
    static inline void EndianSwap(cStaticModel_t& value, EndianOperation operation);
    static inline void EndianSwap(cbrushside_t& value, EndianOperation operation);
    static inline void EndianSwap(cNode_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNodeLeaf_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNodeChildren_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNodeData_t& value, EndianOperation operation);
    static inline void EndianSwap(cLeafBrushNode_s& value, EndianOperation operation);
    static inline void EndianSwap(CollisionPartition& value, EndianOperation operation);
    static inline void EndianSwap(cbrush_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_constant_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_nodes_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_info_t& value, EndianOperation operation);
    static inline void EndianSwap(pathnode_tree_t& value, EndianOperation operation);
    static inline void EndianSwap(clipMap_t& value, EndianOperation operation);
    static inline void EndianSwap(mnode_node_t& value, EndianOperation operation);
    static inline void EndianSwap(mnode_leaf_t& value, EndianOperation operation);
    static inline void EndianSwap(mnode_info_t& value, EndianOperation operation);
    static inline void EndianSwap(mnode_t& value, EndianOperation operation);
    static inline void EndianSwap(srfTriangles_t& value, EndianOperation operation);
    static inline void EndianSwap(GfxSurface& value, EndianOperation operation);
    static inline void EndianSwap(GfxCullGroup& value, EndianOperation operation);
    static inline void EndianSwap(GfxBrushModel& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldVertex& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightGridPoint& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightGridColor& value, EndianOperation operation);
    static inline void EndianSwap(DpvsPlane& value, EndianOperation operation);
    static inline void EndianSwap(vec3_t& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorldVertexData& value, EndianOperation operation);
    static inline void EndianSwap(GfxLight& value, EndianOperation operation);
    static inline void EndianSwap(GfxStaticModelInstance& value, EndianOperation operation);
    static inline void EndianSwap(GfxAabbTree& value, EndianOperation operation);
    static inline void EndianSwap(GfxPortal& value, EndianOperation operation);
    static inline void EndianSwap(GfxOccluderEdge& value, EndianOperation operation);
    static inline void EndianSwap(GfxOccluder& value, EndianOperation operation);
    static inline void EndianSwap(GfxCell& value, EndianOperation operation);
    static inline void EndianSwap(LightmapArray& value, EndianOperation operation);
    static inline void EndianSwap(GfxLightGrid& value, EndianOperation operation);
    static inline void EndianSwap(MaterialMemory& value, EndianOperation operation);
    static inline void EndianSwap(sunflare_t& value, EndianOperation operation);
    static inline void EndianSwap(GfxWorld& value, EndianOperation operation);
    static inline void EndianSwap(DObjAnimMat& value, EndianOperation operation);
    static inline void EndianSwap(XBlendInfo& value, EndianOperation operation);
    static inline void EndianSwap(GfxVertex& value, EndianOperation operation);
    static inline void EndianSwap(XBoneInfo& value, EndianOperation operation);
    static inline void EndianSwap(XModelCollTri& value, EndianOperation operation);
    static inline void EndianSwapPartial(DSkel& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwapPartial(XBoneHierarchy& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwapPartial(XModelParts& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwapPartial(XVertexBuffer& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XRigidSurface& value, EndianOperation operation);
    static inline void EndianSwap(XSurface& value, EndianOperation operation);
    static inline void EndianSwap(XModelSurfs& value, EndianOperation operation);
    static inline void EndianSwap(XModelLodInfo& value, EndianOperation operation);
    static inline void EndianSwap(XModelCollSurf& value, EndianOperation operation);
    static inline void EndianSwap(trXSkin_t& value, EndianOperation operation);
    static inline void EndianSwap(XModel& value, EndianOperation operation);
    static inline void EndianSwap(XQuat& value, EndianOperation operation);
    static inline void EndianSwap(XQuat2& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDynamicIndicesPartTrans& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDynamicIndicesPartQuat& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDynamicIndicesDeltaPartTrans& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDynamicIndicesDeltaPartQuat& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimPartTransFrames& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimPartTransData& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimPartTrans& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimQuatFrames& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimPartQuatFrames& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimPartQuatData& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimPartQuat& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimDeltaPartTransFrames& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPartTransData& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimDeltaPartTrans& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimDeltaPartQuatFrames& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPartQuatData& value, EndianOperation operation);
    static inline void EndianSwapPartial(XAnimDeltaPartQuat& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XAnimPart& value, EndianOperation operation);
    static inline void EndianSwap(XAnimDeltaPart& value, EndianOperation operation);
    static inline void EndianSwap(XAnimNotifyInfo& value, EndianOperation operation);
    static inline void EndianSwap(XAnimParts& value, EndianOperation operation);
    static inline void EndianSwap(XaSeekTable& value, EndianOperation operation);
    static inline void EndianSwap(XAUDIOPACKET_ALIGNED& value, EndianOperation operation);
    static inline void EndianSwap(XaSound& value, EndianOperation operation);
    static inline void EndianSwapPartial(XaPrimedStream& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(XaSoundInfo& value, EndianOperation operation);
    static inline void EndianSwap(SoundFile& value, EndianOperation operation);
    static inline void EndianSwap(XAUDIOCHANNELMAPENTRY& value, EndianOperation operation);
    static inline void EndianSwap(XAUDIOCHANNELMAP& value, EndianOperation operation);
    static inline void EndianSwap(SpeakerMap& value, EndianOperation operation);
    static inline void EndianSwap(snd_alias_t& value, EndianOperation operation);
    static inline void EndianSwap(snd_alias_list_t& value, EndianOperation operation);
    static inline void EndianSwap(TMediaElement& value, EndianOperation operation);
    static inline void EndianSwap(TMediaList& value, EndianOperation operation);
    static inline void EndianSwap(FxList& value, EndianOperation operation);
    static inline void EndianSwapPartial(FxCurve& value, size_t loadedSize, EndianOperation operation);
    static inline void EndianSwap(FxChannel& value, EndianOperation operation);
    static inline void EndianSwap(PrimitiveTemplate& value, EndianOperation operation);
    static inline void EndianSwap(EffectTemplate& value, EndianOperation operation);
    static inline void EndianSwap(FxImpactEntry& value, EndianOperation operation);
    static inline void EndianSwap(FxImpactTable& value, EndianOperation operation);
    static inline void EndianSwap(windowDef_t& value, EndianOperation operation);
    static inline void EndianSwap(ItemKeyHandler& value, EndianOperation operation);
    static inline void EndianSwap(listBoxDef_t& value, EndianOperation operation);
    static inline void EndianSwap(editFieldDef_t& value, EndianOperation operation);
    static inline void EndianSwap(multiDef_t& value, EndianOperation operation);
    static inline void EndianSwap(itemDefData_t& value, EndianOperation operation);
    static inline void EndianSwap(itemDef_t& value, EndianOperation operation);
    static inline void EndianSwap(menuDef_t& value, EndianOperation operation);
    static inline void EndianSwap(MenuList& value, EndianOperation operation);
    static inline void EndianSwap(vec2_t& value, EndianOperation operation);
    static inline void EndianSwap(WeaponDef& value, EndianOperation operation);

    static inline void EndianSwap(RawFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.len);
        EndianSwap(value.buffer);
    }

    static inline void EndianSwap(LocalizeEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.value);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(SndCurve& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.knotCount);
        EndianSwap(value.knots);
    }

    static inline void EndianSwap(D3DTexture&, EndianOperation) {}

    static inline void EndianSwap(GfxTexture& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.map);
    }

    static inline void EndianSwap(GfxImageLoadDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.levelCount);
        EndianSwap(value.flags);
        EndianSwap(value.dimensions);
        EndianSwap(value.format);
        EndianSwap(value.texture, operation);
    }

    static inline void EndianSwap(GfxTextureLoad& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadDef);
    }

    static inline void EndianSwap(GfxImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mapType);
        EndianSwap(value.texture, operation);
        EndianSwap(value.semantic);
        EndianSwap(value.dataSize);
        EndianSwap(value.width);
        EndianSwap(value.height);
        EndianSwap(value.depth);
        EndianSwap(value.pixels);
        EndianSwap(value.baseSize);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(MaterialInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknownString4);
    }

    static inline void EndianSwap(MaterialStateMapRule&, EndianOperation) {}

    static inline void EndianSwapPartial(MaterialStateMapRuleSet& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.ruleCount);
    }

    static inline void EndianSwap(MaterialStateMap& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.ruleSets);
    }

    static inline void EndianSwap(MaterialStreamRouting&, EndianOperation) {}

    static inline void EndianSwap(D3DVertexDeclaration&, EndianOperation) {}

    static inline void EndianSwap(MaterialVertexDeclaration& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.routing);
        EndianSwap(value.streamCount);
        EndianSwap(value.declarations);
    }

    static inline void EndianSwap(MaterialShader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.program);
        EndianSwap(value.programSize);
        EndianSwap(value.runtimeShader);
    }

    static inline void EndianSwap(MaterialArgumentCodeConst& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.index);
        EndianSwap(value.firstRow);
        EndianSwap(value.rowCount);
    }

    static inline void EndianSwap(MaterialArgumentDef&, EndianOperation) {}

    static inline void EndianSwap(MaterialShaderArgument& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(value.type, operation);
        EndianSwap(value.dest);
        if (type == 1)
            EndianSwap(value.u.codeConst, operation);
        else
            EndianSwap(value.u.literalConst);
    }

    static inline void EndianSwap(MaterialPassDx9& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.stateMap);
        EndianSwap(value.vertexDecl);
        EndianSwap(value.vertexShader);
        EndianSwap(value.pixelShader);
        EndianSwap(value.vertexArgCount);
        EndianSwap(value.pixelArgCount);
        EndianSwap(value.vertexArgs);
        EndianSwap(value.pixelArgs);
    }

    static inline void EndianSwapPartial(MaterialTechnique& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.flags);
        EndianSwap(value.passCount);
    }

    static inline void EndianSwap(MaterialTechniqueSet& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.techniques);
    }

    static inline void EndianSwap(complex_s&, EndianOperation) {}

    static inline void EndianSwap(water_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.H0);
        EndianSwap(value.wTerm);
        EndianSwap(value.M);
        EndianSwap(value.N);
        EndianSwap(value.image);
    }

    static inline void EndianSwap(MaterialWaterDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.water);
    }

    static inline void EndianSwap(MaterialTextureDefInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image);
    }

    static inline void EndianSwap(MaterialTextureDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.semantic);
        EndianSwap(value.u, operation);
    }

    static inline void EndianSwap(MaterialConstantDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
    }

    static inline void EndianSwap(Material& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.info, operation);
        EndianSwap(value.textureCount);
        EndianSwap(value.constantCount);
        EndianSwap(value.techniqueSet);
        EndianSwap(value.textureTable);
        EndianSwap(value.constantTable);
    }

    static inline void EndianSwap(GfxLightImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image);
    }

    static inline void EndianSwap(GfxLightDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.attenuation, operation);
        EndianSwap(value.unknownImage12, operation);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(Glyph&, EndianOperation) {}

    static inline void EndianSwap(Font_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.fontName);
        EndianSwap(value.pixelHeight);
        EndianSwap(value.glyphCount);
        EndianSwap(value.material);
        EndianSwap(value.glyphs);
    }

    static inline void EndianSwap(XaReverbSettings&, EndianOperation) {}

    static inline void EndianSwap(SndDriverGlobals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.reverbSettings);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(cplane_t&, EndianOperation) {}

    static inline void EndianSwap(dmaterial_t&, EndianOperation) {}

    static inline void EndianSwap(cLeaf_t&, EndianOperation) {}

    static inline void EndianSwap(CollisionVertex&, EndianOperation) {}

    static inline void EndianSwap(CollisionEdge&, EndianOperation) {}

    static inline void EndianSwap(CollisionTriangle&, EndianOperation) {}

    static inline void EndianSwap(CollisionBorder&, EndianOperation) {}

    static inline void EndianSwap(CollisionAabbTree&, EndianOperation) {}

    static inline void EndianSwap(cmodel_t&, EndianOperation) {}

    static inline void EndianSwap(pathlink_t&, EndianOperation) {}

    static inline void EndianSwap(cStaticModel_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xmodel);
    }

    static inline void EndianSwap(cbrushside_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane);
    }

    static inline void EndianSwap(cNode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane);
    }

    static inline void EndianSwap(cLeafBrushNodeLeaf_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.brushes);
    }

    static inline void EndianSwap(cLeafBrushNodeChildren_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dist);
        EndianSwap(value.range);
        EndianSwap(value.childOffset);
    }

    static inline void EndianSwap(cLeafBrushNodeData_t&, EndianOperation) {}

    static inline void EndianSwap(cLeafBrushNode_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.axis);
        const auto count = EndianSwapDiscriminator(value.leafBrushCount, operation);
        EndianSwap(value.contents);
        if (count > 0)
            EndianSwap(value.data.leaf, operation);
        else
            EndianSwap(value.data.children, operation);
    }

    static inline void EndianSwap(CollisionPartition& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.triangles);
        EndianSwap(value.borders);
    }

    static inline void EndianSwap(cbrush_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.sides);
    }

    static inline void EndianSwap(pathnode_constant_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknownScriptStrings6);
        EndianSwap(value.totalLinkCount);
        EndianSwap(value.Links);
    }

    static inline void EndianSwap(pathnode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.constant, operation);
    }

    static inline void EndianSwap(pathnode_tree_nodes_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
    }

    static inline void EndianSwap(pathnode_tree_info_t&, EndianOperation) {}

    static inline void EndianSwap(pathnode_tree_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto axis = EndianSwapDiscriminator(value.axis, operation);
        if (axis < 0)
            EndianSwap(value.u.nodes, operation);
        else
            EndianSwap(value.u.children);
    }

    static inline void EndianSwap(clipMap_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.numStaticModels);
        EndianSwap(value.staticModelList);
        EndianSwap(value.numMaterials);
        EndianSwap(value.materials);
        EndianSwap(value.numBrushSides);
        EndianSwap(value.brushsides);
        EndianSwap(value.numNodes);
        EndianSwap(value.nodes);
        EndianSwap(value.numLeafs);
        EndianSwap(value.leafs);
        EndianSwap(value.leafbrushNodesCount);
        EndianSwap(value.leafbrushNodes);
        EndianSwap(value.numLeafBrushes);
        EndianSwap(value.leafbrushes);
        EndianSwap(value.numLeafSurfaces);
        EndianSwap(value.leafsurfaces);
        EndianSwap(value.vertCount);
        EndianSwap(value.verts);
        EndianSwap(value.edgeCount);
        EndianSwap(value.edges);
        EndianSwap(value.triCount);
        EndianSwap(value.triangles);
        EndianSwap(value.borderCount);
        EndianSwap(value.borders);
        EndianSwap(value.partitionCount);
        EndianSwap(value.partitions);
        EndianSwap(value.aabbTreeCount);
        EndianSwap(value.aabbTrees);
        EndianSwap(value.numSubModels);
        EndianSwap(value.cmodels);
        EndianSwap(value.numBrushes);
        EndianSwap(value.brushes);
        EndianSwap(value.numClusters);
        EndianSwap(value.clusterBytes);
        EndianSwap(value.visibility);
        EndianSwap(value.numEntityChars);
        EndianSwap(value.entityString);
        EndianSwap(value.box_brush);
        EndianSwap(value.pathNodeCount);
        EndianSwap(value.pathNodes);
        EndianSwap(value.chainNodeForNode);
        EndianSwap(value.nodeForChainNode);
        EndianSwap(value.visBytes);
        EndianSwap(value.pathVis);
        EndianSwap(value.nodeTreeCount);
        EndianSwap(value.nodeTree);
    }

    static inline void EndianSwap(mnode_node_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane);
        EndianSwap(value.children);
    }

    static inline void EndianSwap(mnode_leaf_t&, EndianOperation) {}

    static inline void EndianSwap(mnode_info_t&, EndianOperation) {}

    static inline void EndianSwap(mnode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto contents = EndianSwapDiscriminator(value.contents, operation);
        EndianSwap(value.parent);
        if (contents == -1)
            EndianSwap(value.u.node, operation);
    }

    static inline void EndianSwap(srfTriangles_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.indexCount);
        EndianSwap(value.indices);
    }

    static inline void EndianSwap(GfxSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.material);
        EndianSwap(value.tris);
    }

    static inline void EndianSwap(GfxCullGroup&, EndianOperation) {}

    static inline void EndianSwap(GfxBrushModel&, EndianOperation) {}

    static inline void EndianSwap(GfxWorldVertex&, EndianOperation) {}

    static inline void EndianSwap(GfxLightGridPoint&, EndianOperation) {}

    static inline void EndianSwap(GfxLightGridColor&, EndianOperation) {}

    static inline void EndianSwap(DpvsPlane&, EndianOperation) {}

    static inline void EndianSwap(vec3_t&, EndianOperation) {}

    static inline void EndianSwap(GfxWorldVertexData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertices);
    }

    static inline void EndianSwap(GfxLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.def);
    }

    static inline void EndianSwap(GfxStaticModelInstance& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model);
    }

    static inline void EndianSwap(GfxAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.staticModelCount);
        EndianSwap(value.staticModelIndices);
        EndianSwap(value.childrenCount);
        EndianSwap(value.children);
    }

    static inline void EndianSwap(GfxPortal& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cell);
        EndianSwap(value.vertices);
        EndianSwap(value.vertexCount);
    }

    static inline void EndianSwap(GfxOccluderEdge& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.planes);
        EndianSwap(value.vertices);
    }

    static inline void EndianSwap(GfxOccluder& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.planeCount);
        EndianSwap(value.planes);
        EndianSwap(value.edgeCount);
        EndianSwap(value.edges);
        EndianSwap(value.vertexCount);
        EndianSwap(value.vertices);
    }

    static inline void EndianSwap(GfxCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aabbTree);
        EndianSwap(value.portalCount);
        EndianSwap(value.portals);
        EndianSwap(value.cullGroupCount);
        EndianSwap(value.cullGroups);
        EndianSwap(value.occluderCount);
        EndianSwap(value.occluders);
    }

    static inline void EndianSwap(LightmapArray& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.images);
    }

    static inline void EndianSwap(GfxLightGrid& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pointCount);
        EndianSwap(value.points);
        EndianSwap(value.colorCount);
        EndianSwap(value.colors);
    }

    static inline void EndianSwap(MaterialMemory& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.material);
    }

    static inline void EndianSwap(sunflare_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.spriteMaterial);
        EndianSwap(value.flareMaterial);
    }

    static inline void EndianSwap(GfxWorld& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.baseName);
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
        EndianSwap(value.surfaceCount);
        EndianSwap(value.surfaces);
        EndianSwap(value.unknownCount24);
        EndianSwap(value.unknownIndices28);
        EndianSwap(value.skyImage);
        EndianSwap(value.vertexCount);
        EndianSwap(value.vertexData, operation);
        EndianSwap(value.light, operation);
        EndianSwap(value.cullGroupCount);
        EndianSwap(value.cullGroups);
        EndianSwap(value.staticModelCount);
        EndianSwap(value.staticModels);
        EndianSwap(value.cellCount);
        EndianSwap(value.cells);
        EndianSwap(value.lightmapCount);
        EndianSwap(value.lightmaps);
        EndianSwap(value.unknownImage284);
        EndianSwap(value.lightGrid, operation);
        EndianSwap(value.brushModelCount);
        EndianSwap(value.brushModels);
        EndianSwap(value.materialMemoryCount);
        EndianSwap(value.materialMemory);
        EndianSwap(value.sun, operation);
        EndianSwap(value.unknownImage520);
    }

    static inline void EndianSwap(DObjAnimMat&, EndianOperation) {}

    static inline void EndianSwap(XBlendInfo&, EndianOperation) {}

    static inline void EndianSwap(GfxVertex&, EndianOperation) {}

    static inline void EndianSwap(XBoneInfo&, EndianOperation) {}

    static inline void EndianSwap(XModelCollTri&, EndianOperation) {}

    static inline void EndianSwapPartial(DSkel& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwapPartial(XBoneHierarchy& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.names);
    }

    static inline void EndianSwapPartial(XModelParts& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.numBones);
        EndianSwap(value.numRootBones);
        EndianSwap(value.hierarchy);
        EndianSwap(value.quats);
        EndianSwap(value.trans);
        EndianSwap(value.partClassification);
    }

    static inline void EndianSwapPartial(XVertexBuffer& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.blendCount);
    }

    static inline void EndianSwap(XRigidSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.verts);
    }

    static inline void EndianSwap(XSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertCount);
        EndianSwap(value.triCount);
        EndianSwap(value.triIndices);
        EndianSwap(value.verts);
        EndianSwap(value.rigid, operation);
    }

    static inline void EndianSwap(XModelSurfs& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfs);
    }

    static inline void EndianSwap(XModelLodInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dist);
        EndianSwap(value.filename);
        EndianSwap(value.numsurfs);
        EndianSwap(value.surfNames);
        EndianSwap(value.surfs);
    }

    static inline void EndianSwap(XModelCollSurf& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.collTris);
        EndianSwap(value.numCollTris);
    }

    static inline void EndianSwap(trXSkin_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.materials);
    }

    static inline void EndianSwap(XModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.parts);
        for (auto& item : value.lodInfo)
            EndianSwap(item, operation);
        EndianSwap(value.collSurfs);
        EndianSwap(value.numCollSurfs);
        EndianSwap(value.boneInfo);
        EndianSwap(value.numLods);
        EndianSwap(value.collLod);
        EndianSwap(value.xskins);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(XQuat&, EndianOperation) {}

    static inline void EndianSwap(XQuat2&, EndianOperation) {}

    static inline void EndianSwap(XAnimDynamicIndicesPartTrans&, EndianOperation) {}

    static inline void EndianSwap(XAnimDynamicIndicesPartQuat&, EndianOperation) {}

    static inline void EndianSwap(XAnimDynamicIndicesDeltaPartTrans&, EndianOperation) {}

    static inline void EndianSwap(XAnimDynamicIndicesDeltaPartQuat&, EndianOperation) {}

    static inline void EndianSwapPartial(XAnimPartTransFrames& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames);
    }

    static inline void EndianSwap(XAnimPartTransData&, EndianOperation) {}

    static inline void EndianSwapPartial(XAnimPartTrans& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void EndianSwap(XAnimQuatFrames& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.normal);
    }

    static inline void EndianSwapPartial(XAnimPartQuatFrames& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames, operation);
    }

    static inline void EndianSwap(XAnimPartQuatData&, EndianOperation) {}

    static inline void EndianSwapPartial(XAnimPartQuat& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartTransFrames& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames);
    }

    static inline void EndianSwap(XAnimDeltaPartTransData&, EndianOperation) {}

    static inline void EndianSwapPartial(XAnimDeltaPartTrans& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void EndianSwapPartial(XAnimDeltaPartQuatFrames& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames);
    }

    static inline void EndianSwap(XAnimDeltaPartQuatData&, EndianOperation) {}

    static inline void EndianSwapPartial(XAnimDeltaPartQuat& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void EndianSwap(XAnimPart& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.trans);
        EndianSwap(value.quat);
    }

    static inline void EndianSwap(XAnimDeltaPart& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.trans);
        EndianSwap(value.quat);
    }

    static inline void EndianSwap(XAnimNotifyInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.time);
    }

    static inline void EndianSwap(XAnimParts& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.numframes);
        EndianSwap(value.notifyCount);
        EndianSwap(value.boneCount);
        EndianSwap(value.names);
        EndianSwap(value.bSmallQuat);
        EndianSwap(value.parts);
        EndianSwap(value.notify);
        EndianSwap(value.deltaPart);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(XaSeekTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.count);
        EndianSwap(value.values);
    }

    static inline void EndianSwap(XAUDIOPACKET_ALIGNED& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.buffer);
        EndianSwap(value.size);
        EndianSwap(value.unknownPointer84);
    }

    static inline void EndianSwap(XaSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packet, operation);
        EndianSwap(value.seekTable, operation);
    }

    static inline void EndianSwapPartial(XaPrimedStream& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.seekTable, operation);
        EndianSwap(value.bufferSize);
    }

    static inline void EndianSwap(XaSoundInfo&, EndianOperation) {}

    static inline void EndianSwap(SoundFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.info);
        EndianSwap(value.type);
    }

    static inline void EndianSwap(XAUDIOCHANNELMAPENTRY&, EndianOperation) {}

    static inline void EndianSwap(XAUDIOCHANNELMAP& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.entryCount);
        EndianSwap(value.entries);
    }

    static inline void EndianSwap(SpeakerMap& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        for (auto& item : value.channelMaps)
            EndianSwap(item, operation);
    }

    static inline void EndianSwap(snd_alias_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aliasName);
        EndianSwap(value.unknownString4);
        EndianSwap(value.unknownString8);
        EndianSwap(value.soundFile);
        EndianSwap(value.volumeFalloffCurve);
        EndianSwap(value.speakerMap);
    }

    static inline void EndianSwap(snd_alias_list_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.head);
        EndianSwap(value.count);
    }

    static inline void EndianSwap(TMediaElement& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model);
    }

    static inline void EndianSwap(TMediaList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.items);
        EndianSwap(value.count);
    }

    static inline void EndianSwap(FxList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.names);
        EndianSwap(value.count);
    }

    static inline void EndianSwapPartial(FxCurve& value, [[maybe_unused]] size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dimension);
        EndianSwap(value.keyCount);
    }

    static inline void EndianSwap(FxChannel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.curve);
    }

    static inline void EndianSwap(PrimitiveTemplate& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type);
        EndianSwap(value.media, operation);
        for (auto& item : value.effects)
            EndianSwap(item, operation);
        for (auto& item : value.channels)
            EndianSwap(item, operation);
    }

    static inline void EndianSwap(EffectTemplate& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.primitives);
    }

    static inline void EndianSwap(FxImpactEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.effects);
    }

    static inline void EndianSwap(FxImpactTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.table);
    }

    static inline void EndianSwap(windowDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.group);
        EndianSwap(value.unknownString200);
        EndianSwap(value.background);
    }

    static inline void EndianSwap(ItemKeyHandler& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.key);
        EndianSwap(value.action);
        EndianSwap(value.next);
    }

    static inline void EndianSwap(listBoxDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.doubleClick);
    }

    static inline void EndianSwap(editFieldDef_t&, EndianOperation) {}

    static inline void EndianSwap(multiDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dvarList);
        EndianSwap(value.dvarStr);
    }

    static inline void EndianSwap(itemDefData_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.listBox);
    }

    static inline void EndianSwap(itemDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);
        EndianSwap(value.type);
        EndianSwap(value.unknownString660);
        EndianSwap(value.unknownString672);
        EndianSwap(value.unknownString676);
        EndianSwap(value.unknownString680);
        EndianSwap(value.unknownString684);
        EndianSwap(value.unknownString688);
        EndianSwap(value.unknownString692);
        EndianSwap(value.unknownString696);
        EndianSwap(value.unknownString700);
        EndianSwap(value.unknownString704);
        EndianSwap(value.unknownString708);
        EndianSwap(value.onKey);
        EndianSwap(value.unknownString716);
        EndianSwap(value.focusSound);
        EndianSwap(value.typeData, operation);
    }

    static inline void EndianSwap(menuDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);
        EndianSwap(value.font);
        EndianSwap(value.itemCount);
        EndianSwap(value.unknownString580);
        EndianSwap(value.unknownString584);
        EndianSwap(value.unknownString588);
        EndianSwap(value.onKey);
        EndianSwap(value.unknownString596);
        EndianSwap(value.items);
    }

    static inline void EndianSwap(MenuList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.menuCount);
        EndianSwap(value.menus);
    }

    static inline void EndianSwap(vec2_t&, EndianOperation) {}

    static inline void EndianSwap(WeaponDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknownString4);
        EndianSwap(value.unknownString8);
        EndianSwap(value.unknownString12);
        EndianSwap(value.unknownString16);
        EndianSwap(value.unknownString20);
        EndianSwap(value.unknownString24);
        EndianSwap(value.unknownString28);
        EndianSwap(value.unknownString32);
        EndianSwap(value.unknownString36);
        EndianSwap(value.unknownString40);
        EndianSwap(value.unknownString44);
        EndianSwap(value.unknownString48);
        EndianSwap(value.unknownString52);
        EndianSwap(value.unknownString56);
        EndianSwap(value.unknownString60);
        EndianSwap(value.unknownString64);
        EndianSwap(value.unknownString68);
        EndianSwap(value.unknownString72);
        EndianSwap(value.unknownString76);
        EndianSwap(value.unknownString80);
        EndianSwap(value.unknownString84);
        EndianSwap(value.unknownString88);
        EndianSwap(value.unknownString92);
        EndianSwap(value.unknownString96);
        EndianSwap(value.unknownString100);
        EndianSwap(value.unknownString104);
        EndianSwap(value.unknownString108);
        EndianSwap(value.unknownString112);
        EndianSwap(value.unknownString144);
        EndianSwap(value.unknownString148);
        EndianSwap(value.unknownString152);
        EndianSwap(value.unknownString156);
        EndianSwap(value.unknownString160);
        EndianSwap(value.unknownString164);
        EndianSwap(value.unknownString168);
        EndianSwap(value.unknownString172);
        EndianSwap(value.unknownString176);
        EndianSwap(value.unknownString180);
        EndianSwap(value.unknownString184);
        EndianSwap(value.unknownString188);
        EndianSwap(value.unknownString192);
        EndianSwap(value.unknownString196);
        EndianSwap(value.unknownString200);
        EndianSwap(value.unknownString204);
        EndianSwap(value.unknownString208);
        EndianSwap(value.unknownString212);
        EndianSwap(value.unknownString216);
        EndianSwap(value.unknownString220);
        EndianSwap(value.unknownString224);
        EndianSwap(value.unknownString228);
        EndianSwap(value.unknownString232);
        EndianSwap(value.unknownString236);
        EndianSwap(value.unknownString240);
        EndianSwap(value.unknownString244);
        EndianSwap(value.unknownString248);
        EndianSwap(value.unknownString252);
        EndianSwap(value.unknownString256);
        EndianSwap(value.unknownString260);
        EndianSwap(value.unknownString264);
        EndianSwap(value.unknownString268);
        EndianSwap(value.unknownString272);
        EndianSwap(value.unknownString276);
        EndianSwap(value.unknownString280);
        EndianSwap(value.unknownString284);
        EndianSwap(value.unknownString436);
        EndianSwap(value.unknownString440);
        EndianSwap(value.unknownString444);
        EndianSwap(value.unknownString452);
        EndianSwap(value.unknownString460);
        EndianSwap(value.unknownString480);
        EndianSwap(value.unknownString628);
        EndianSwap(value.unknownString844);
        EndianSwap(value.unknownString872);
        EndianSwap(value.unknownString908);
        EndianSwap(value.unknownString916);
        EndianSwap(value.unknownString920);
        EndianSwap(value.unknownString1112);
        EndianSwap(value.unknownString1292);
        EndianSwap(value.unknownString1296);
        EndianSwap(value.unknownString1388);
        EndianSwap(value.unknownString1392);
        EndianSwap(value.unknownString1412);
        EndianSwap(value.unknownString1532);
        EndianSwap(value.unknownString1536);
        EndianSwap(value.accuracyGraph0);
        EndianSwap(value.accuracyGraph1);
        EndianSwap(value.accuracyGraphCount0);
        EndianSwap(value.accuracyGraphCount1);
    }
} // namespace IW2Xenon
