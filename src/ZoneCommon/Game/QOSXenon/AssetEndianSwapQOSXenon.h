#pragma once

#include "Game/QOSXenon/QOSXenon.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace QOSXenon
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

    static inline void EndianSwap(QOSXenon::XAnimNotifyInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.time);
    }

    static inline void EndianSwap(QOSXenon::XAnimDeltaPart& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.trans);
        EndianSwap(value.quat);
    }

    static inline void
        EndianSwapPartial(QOSXenon::XAnimPartTrans& value, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void
        EndianSwapPartial(QOSXenon::XAnimPartTransFrames& value, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.size);
        EndianSwap(value.frames._1);
    }

    static inline void EndianSwap(QOSXenon::XAnimDynamicFrames& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void
        EndianSwapPartial(QOSXenon::XAnimDeltaPartQuat& value, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
    }

    static inline void EndianSwapPartial(QOSXenon::XAnimDeltaPartQuatDataFrames& value,
                                         [[maybe_unused]] const size_t loadedSize,
                                         [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.frames);
    }

    static inline void EndianSwap(QOSXenon::XQuat2& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.value);
    }

    static inline void EndianSwap(QOSXenon::XAnimIndices& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::XAnimParts& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknownString);
        EndianSwap(value.dataByteCount);
        EndianSwap(value.dataShortCount);
        EndianSwap(value.dataIntCount);
        EndianSwap(value.randomDataByteCount);
        EndianSwap(value.randomDataIntCount);
        EndianSwap(value.numframes);
        EndianSwap(value.randomDataShortCount);
        EndianSwap(value.indexCount);
        EndianSwap(value.framerate);
        EndianSwap(value.frequency);
        EndianSwap(value.names);
        EndianSwap(value.dataByte);
        EndianSwap(value.dataShort);
        EndianSwap(value.dataInt);
        EndianSwap(value.randomDataShort);
        EndianSwap(value.randomDataByte);
        EndianSwap(value.randomDataInt);
        EndianSwap(value.indices.data);
        EndianSwap(value.notify);
        EndianSwap(value.deltaPart);
    }

    static inline void EndianSwap(QOSXenon::PhysPreset& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknownString);
    }

    static inline void EndianSwap(QOSXenon::PhysConstraint& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.target_bone1);
        EndianSwap(value.target_bone2);
    }

    static inline void EndianSwap(QOSXenon::PhysConstraints& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.count);

        for (auto& constraint : value.data)
            EndianSwap(constraint, operation);
    }

    static inline void EndianSwap(QOSXenon::XModelQuat& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.v);
    }

    static inline void EndianSwap(QOSXenon::DObjAnimMat& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.quat);
        EndianSwap(value.trans);
        EndianSwap(value.transWeight);
    }

    static inline void EndianSwap(QOSXenon::GfxPackedVertex16& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::D3DVertexBuffer& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::D3DIndexBuffer& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::XSurfaceVertexInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertCount);
        EndianSwap(value.vertsBlend);
        EndianSwap(value.tensionData);
    }

    static inline void EndianSwap(QOSXenon::XRigidVertList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.boneOffset);
        EndianSwap(value.vertCount);
        EndianSwap(value.triOffset);
        EndianSwap(value.triCount);
    }

    static inline void EndianSwap(QOSXenon::XSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertCount);
        EndianSwap(value.triCount);
        EndianSwap(value.unknown6);
        EndianSwap(value.triIndices);
        EndianSwap(value.vertInfo, operation);
        EndianSwap(value.verts0);
        EndianSwap(value.verts1);
        EndianSwap(value.verts2);
        EndianSwap(value.vertListCount);
        EndianSwap(value.vertList);
        EndianSwap(value.partBits);
    }

    static inline void EndianSwap(QOSXenon::XModelLodInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dist);
        EndianSwap(value.numsurfs);
        EndianSwap(value.surfIndex);
        EndianSwap(value.partBits);
    }

    static inline void EndianSwap(QOSXenon::XModelCollSurf_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.boneIdx);
        EndianSwap(value.contents);
        EndianSwap(value.surfFlags);
    }

    static inline void EndianSwap(QOSXenon::XBoneInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.bounds);
        EndianSwap(value.offset);
        EndianSwap(value.radiusSquared);
    }

    static inline void EndianSwap(QOSXenon::XModelHighMipBounds& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
    }

    static inline void EndianSwap(QOSXenon::XModelStreamInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.highMipBounds);
    }

    static inline void EndianSwap(QOSXenon::cplane_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.normal);
        EndianSwap(value.dist);
    }

    static inline void EndianSwap(QOSXenon::cbrushside_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane);
        EndianSwap(value.materialNum);
        EndianSwap(value.firstAdjacentSideOffset);
    }

    static inline void EndianSwap(QOSXenon::BrushWrapper& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.contents);
        EndianSwap(value.maxs);
        EndianSwap(value.numsides);
        EndianSwap(value.sides);
        EndianSwap(value.axialMaterialNum);
        EndianSwap(value.baseAdjacentSide);
        EndianSwap(value.firstAdjacentSideOffsets);
        EndianSwap(value.pad70);
        EndianSwap(value.numverts);
        EndianSwap(value.verts);
        EndianSwap(value.totalEdgeCount);
        EndianSwap(value.planes);
    }

    static inline void EndianSwap(QOSXenon::PhysGeomInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.brush);
        EndianSwap(value.type);
        EndianSwap(value.orientation);
        EndianSwap(value.offset);
        EndianSwap(value.halfLengths);
    }

    static inline void EndianSwap(QOSXenon::PhysGeomList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.count);
        EndianSwap(value.geoms);
        EndianSwap(value.mass);
    }

    static inline void EndianSwap(QOSXenon::XModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.boneNames);
        EndianSwap(value.parentList);
        EndianSwap(value.quats);
        EndianSwap(value.trans);
        EndianSwap(value.partClassification);
        EndianSwap(value.baseMat);
        EndianSwap(value.surfs);
        EndianSwap(value.materialHandles);

        for (auto& lod : value.lodInfo)
            EndianSwap(lod, operation);

        EndianSwap(value.collSurfs);
        EndianSwap(value.numCollSurfs);
        EndianSwap(value.contents);
        EndianSwap(value.boneInfo);
        EndianSwap(value.radius);
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.numLods);
        EndianSwap(value.collLod);
        EndianSwap(value.streamInfo, operation);
        EndianSwap(value.memUsage);
        EndianSwap(value.physPreset);
        EndianSwap(value.physGeoms);
        EndianSwap(value.physConstraints);
    }

    static inline void EndianSwap(QOSXenon::snd_alias_list_name& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.soundName);
    }

    static inline void EndianSwap(QOSXenon::SndAliasCustom& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::SndCurve& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.knotCount);
        EndianSwap(value.knots);
    }

    static inline void EndianSwap(QOSXenon::QosMssSoundSeekTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.size);
        EndianSwap(value.data);
    }

    static inline void EndianSwap(QOSXenon::QosMssSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data);
        EndianSwap(value.dataSize);
        EndianSwap(value.unknownData);
        EndianSwap(value.seekTable, operation);
    }

    static inline void EndianSwap(QOSXenon::QosPrimedSound& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.buffer);
        EndianSwap(value.size);
    }

    static inline void EndianSwap(QOSXenon::QosSoundFileRef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mssSound);
    }

    static inline void EndianSwap(QOSXenon::SoundFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknownString0);
        EndianSwap(value.unknownString1);
        EndianSwap(value.u, operation);
        EndianSwap(value.unknown0);
        EndianSwap(value.type);
    }

    static inline void EndianSwap(QOSXenon::XAUDIOCHANNELMAPENTRY& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.volume);
    }

    static inline void EndianSwap(QOSXenon::XAUDIOCHANNELMAP& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.entries);
    }

    static inline void EndianSwap(QOSXenon::SpeakerMap& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);

        for (auto& channelMapRow : value.channelMaps)
        {
            for (auto& channelMap : channelMapRow)
                EndianSwap(channelMap, operation);
        }
    }

    static inline void EndianSwap(QOSXenon::snd_alias_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aliasName);
        EndianSwap(value.subtitle);
        EndianSwap(value.secondaryAliasName);
        EndianSwap(value.chainAliasName);
        EndianSwap(value.soundFile);
        EndianSwap(value.volumeFalloffCurve);
        EndianSwap(value.speakerMap);
    }

    static inline void EndianSwap(QOSXenon::snd_alias_list_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.head);
        EndianSwap(value.count);
    }

    static inline void EndianSwap(QOSXenon::XModelPiece& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model);
        EndianSwap(value.offset);
    }

    static inline void EndianSwap(QOSXenon::XModelPieces& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.numpieces);
        EndianSwap(value.pieces);
    }

    static inline void EndianSwap(QOSXenon::MapEnts& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.entityString);
        EndianSwap(value.numEntityChars);
    }

    static inline void EndianSwap(QOSXenon::cStaticModel_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xmodel);
        EndianSwap(value.origin);
        EndianSwap(value.invScaledAxis);
        EndianSwap(value.absmin);
        EndianSwap(value.absmax);
    }

    static inline void EndianSwap(QOSXenon::dmaterial_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfaceFlags);
        EndianSwap(value.contentFlags);
    }

    static inline void EndianSwap(QOSXenon::cNode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.plane);
        EndianSwap(value.children);
    }

    static inline void EndianSwap(QOSXenon::cLeaf_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstCollAabbIndex);
        EndianSwap(value.collAabbCount);
        EndianSwap(value.brushContents);
        EndianSwap(value.terrainContents);
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.leafBrushNode);
        EndianSwap(value.cluster);
    }

    static inline void EndianSwap(QOSXenon::cLeafBrushNodeLeaf_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.brushes);
    }

    static inline void EndianSwap(QOSXenon::cLeafBrushNodeData_t& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::cLeafBrushNode_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto leafBrushCount = EndianSwapDiscriminator(value.leafBrushCount, operation);
        EndianSwap(value.contents);

        if (leafBrushCount > 0)
        {
            EndianSwap(value.data.leaf.brushes);
        }
        else
        {
            EndianSwap(value.data.children.dist);
            EndianSwap(value.data.children.range);
            EndianSwap(value.data.children.childOffset);
        }
    }

    static inline void EndianSwap(QOSXenon::CollisionBorder& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.distEq);
        EndianSwap(value.zBase);
        EndianSwap(value.zSlope);
        EndianSwap(value.start);
        EndianSwap(value.length);
    }

    static inline void EndianSwap(QOSXenon::CollisionPartition& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstTri);
        EndianSwap(value.unknown1);
        EndianSwap(value.unknown2);
        EndianSwap(value.borders);
    }

    static inline void EndianSwap(QOSXenon::CollisionAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin);
        EndianSwap(value.materialIndex);
        EndianSwap(value.childCount);
        EndianSwap(value.halfSize);
        EndianSwap(value.u.firstChildIndex);
    }

    static inline void EndianSwap(QOSXenon::cmodel_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.radius);
        EndianSwap(value.leaf, operation);
    }

    static inline void EndianSwap(QOSXenon::cbrush_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.contents);
        EndianSwap(value.maxs);
        EndianSwap(value.numsides);
        EndianSwap(value.sides);
        EndianSwap(value.axialMaterialNum);
        EndianSwap(value.baseAdjacentSide);
        EndianSwap(value.firstAdjacentSideOffsets);
        EndianSwap(value.unknown1);
        EndianSwap(value.verts);
    }

    static inline void EndianSwap(QOSXenon::DynEntityDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xModel);
        EndianSwap(value.destroyFx);
        EndianSwap(value.destroyPieces);
        EndianSwap(value.physPreset);
    }

    static inline void EndianSwap(QOSXenon::DynEntityClient& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::DynEntityServer& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::DynEntityColl& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::DynEntityPhysConstraint& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::clipMap_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.isInUse);
        EndianSwap(value.planeCount);
        EndianSwap(value.planes);
        EndianSwap(value.numStaticModels);
        EndianSwap(value.staticModelList);
        EndianSwap(value.numMaterials);
        EndianSwap(value.materials);
        EndianSwap(value.numBrushSides);
        EndianSwap(value.brushsides);
        EndianSwap(value.numBrushEdges);
        EndianSwap(value.brushEdges);
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
        EndianSwap(value.numBrushVerts);
        EndianSwap(value.brushVerts);
        EndianSwap(value.nuinds);
        EndianSwap(value.uinds);
        EndianSwap(value.triCount);
        EndianSwap(value.triIndices);
        EndianSwap(value.triEdgeIsWalkable);
        EndianSwap(value.borderCount);
        EndianSwap(value.borders);
        EndianSwap(value.partitionCount);
        EndianSwap(value.partitions);
        EndianSwap(value.aabbTreeCount);
        EndianSwap(value.aabbTrees);
        EndianSwap(value.numSubModels);
        EndianSwap(value.cmodels);
        EndianSwap(value.numBrushes);
        EndianSwap(value.unknown0);
        EndianSwap(value.brushes);
        EndianSwap(value.numClusters);
        EndianSwap(value.clusterBytes);
        EndianSwap(value.visibility);
        EndianSwap(value.unknown2);
        EndianSwap(value.mapEnts);
        EndianSwap(value.box_brush);
        EndianSwap(value.dynEntCount);
        EndianSwap(value.dynEntDefList);
        EndianSwap(value.dynEntClientList);
        EndianSwap(value.dynEntServerList);
        EndianSwap(value.dynEntCollList);
        EndianSwap(value.num_constraints);
        EndianSwap(value.constraints);
        EndianSwap(value.unknown3);
    }

    static inline void EndianSwap(QOSXenon::ComPrimaryLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.color);
        EndianSwap(value.dir);
        EndianSwap(value.origin);
        EndianSwap(value.radius);
        EndianSwap(value.cosHalfFovOuter);
        EndianSwap(value.cosHalfFovInner);
        EndianSwap(value.cosHalfFovExpanded);
        EndianSwap(value.rotationLimit);
        EndianSwap(value.translationLimit);
        EndianSwap(value.defName);
    }

    static inline void EndianSwap(QOSXenon::ComWorldUnknownCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data);
    }

    static inline void EndianSwap(QOSXenon::ComWorld& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.isInUse);
        EndianSwap(value.unknown0);
        EndianSwap(value.primaryLightCount);
        EndianSwap(value.primaryLights);
        EndianSwap(value.unknownCellCount);
        EndianSwap(value.unknownCells);
    }

    static inline void EndianSwap(QOSXenon::pathbasenode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vOrigin);
        EndianSwap(value.type);
    }

    static inline void EndianSwap(QOSXenon::pathlink_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.fDist);
        EndianSwap(value.nodeNum);
    }

    static inline void EndianSwap(QOSXenon::pathnode_constant_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.type);
        EndianSwap(value.spawnflags);
        EndianSwap(value.targetname);
        EndianSwap(value.script_linkName);
        EndianSwap(value.script_noteworthy);
        EndianSwap(value.target);
        EndianSwap(value.animscript);
        EndianSwap(value.animscriptfunc);
        EndianSwap(value.vOrigin);
        EndianSwap(value.fAngle);
        EndianSwap(value.forward);
        EndianSwap(value.fRadius);
        EndianSwap(value.minUseDistSq);
        EndianSwap(value.wOverlapNode);
        EndianSwap(value.wChainId);
        EndianSwap(value.wChainDepth);
        EndianSwap(value.wChainParent);
        EndianSwap(value.totalLinkCount);
        EndianSwap(value.Links);
    }

    static inline void EndianSwap(QOSXenon::pathnode_dynamic_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pOwner);
        EndianSwap(value.iFreeTime);
        EndianSwap(value.iValidTime);
        EndianSwap(value.inPlayerLOSTime);
        EndianSwap(value.wLinkCount);
        EndianSwap(value.wOverlapCount);
        EndianSwap(value.turretEntNumber);
        EndianSwap(value.userCount);
    }

    static inline void EndianSwap(QOSXenon::pathnode_transient_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.iSearchFrame);
        EndianSwap(value.pNextOpen);
        EndianSwap(value.pPrevOpen);
        EndianSwap(value.pParent);
        EndianSwap(value.fCost);
        EndianSwap(value.fHeuristic);
        EndianSwap(value.costFactor);
        EndianSwap(value.unknown0);
    }

    static inline void EndianSwap(QOSXenon::pathnode_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.constant, operation);
        EndianSwap(value.dynamic, operation);
        EndianSwap(value.transient, operation);
    }

    static inline void EndianSwap(QOSXenon::pathnode_tree_nodes_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
    }

    static inline void EndianSwap(QOSXenon::pathnode_tree_info_t& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::pathnode_tree_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto axis = EndianSwapDiscriminator(value.axis, operation);
        EndianSwap(value.dist);

        if (axis >= 0)
        {
            EndianSwap(value.u.child[0]);
            EndianSwap(value.u.child[1]);
        }
        else
        {
            EndianSwap(value.u.s, operation);
        }
    }

    static inline void EndianSwap(QOSXenon::PathData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
        EndianSwap(value.basenodes);
        EndianSwap(value.chainNodeForNode);
        EndianSwap(value.nodeForChainNode);
        EndianSwap(value.visBytes);
        EndianSwap(value.pathVis);
        EndianSwap(value.nodeTreeCount);
        EndianSwap(value.nodeTree);
    }

    static inline void EndianSwap(QOSXenon::GameWorldSp& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.path, operation);
    }

    static inline void EndianSwap(QOSXenon::GameWorldMp& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::GfxColor& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed);
    }

    static inline void EndianSwap(QOSXenon::PackedUnitVec& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed);
    }

    static inline void EndianSwap(QOSXenon::GfxWorldVertex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.xyz);
        EndianSwap(value.binormalSign);
        EndianSwap(value.color, operation);
        EndianSwap(value.texCoord);
        EndianSwap(value.lmapCoord);
        EndianSwap(value.normal, operation);
        EndianSwap(value.tangent, operation);
    }

    static inline void EndianSwap(QOSXenon::GfxStreamingAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.firstItem);
        EndianSwap(value.itemCount);
        EndianSwap(value.firstChild);
        EndianSwap(value.childCount);
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
    }

    static inline void EndianSwap(QOSXenon::GfxWorldStreamInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.aabbTreeCount);
        EndianSwap(value.aabbTrees);
        EndianSwap(value.leafRefCount);
        EndianSwap(value.leafRefs);
    }

    static inline void EndianSwap(QOSXenon::GfxWorldVertexData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertices);
    }

    static inline void EndianSwap(QOSXenon::GfxWorldVertexLayerData& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data);
    }

    static inline void EndianSwap(QOSXenon::GfxLight& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.color);
        EndianSwap(value.dir);
        EndianSwap(value.origin);
        EndianSwap(value.radius);
        EndianSwap(value.cosHalfFovOuter);
        EndianSwap(value.cosHalfFovInner);
        EndianSwap(value.exponent);
        EndianSwap(value.spotShadowIndex);
        EndianSwap(value.unknown60);
        EndianSwap(value.def);
    }

    static inline void EndianSwap(QOSXenon::GfxReflectionProbe& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.origin);
        EndianSwap(value.reflectionImage);
    }

    static inline void EndianSwap(QOSXenon::srfTriangles_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertexLayerData);
        EndianSwap(value.firstVertex);
        EndianSwap(value.vertexCount);
        EndianSwap(value.triCount);
        EndianSwap(value.baseIndex);
        EndianSwap(value.topMipMins);
        EndianSwap(value.topMipMaxs);
    }

    static inline void EndianSwap(QOSXenon::GfxSurface& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.tris, operation);
        EndianSwap(value.material);
        EndianSwap(value.bounds);
    }

    static inline void EndianSwap(QOSXenon::GfxCullGroup& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.surfaceCount);
        EndianSwap(value.startSurfIndex);
    }

    static inline void EndianSwap(QOSXenon::GfxStaticModelDrawInst& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model);
        EndianSwap(value.unknown36);
    }

    static inline void EndianSwap(QOSXenon::GfxStaticModelInst& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.groundLighting, operation);
        EndianSwap(value.unknown28);
    }

    static inline void EndianSwap(QOSXenon::GfxAabbTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.smodelIndexCount);
        EndianSwap(value.smodelIndexes);
        EndianSwap(value.childCount);
        EndianSwap(value.children);
    }

    static inline void EndianSwap(QOSXenon::GfxAabbTreeChildren& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.child, operation);
    }

    static inline void EndianSwap(QOSXenon::GfxPortal& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cell);
        EndianSwap(value.vertices);
    }

    static inline void EndianSwap(QOSXenon::GfxCell& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.aabbTree);
        EndianSwap(value.portalCount);
        EndianSwap(value.portals);
        EndianSwap(value.cullGroupCount);
        EndianSwap(value.cullGroups);
        EndianSwap(value.reflectionProbes);
    }

    static inline void EndianSwap(QOSXenon::GfxLightmapArray& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.primary);
        EndianSwap(value.secondary);
    }

    static inline void EndianSwap(QOSXenon::GfxLightGridEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.colorsIndex);
    }

    static inline void EndianSwap(QOSXenon::GfxLightGridColors& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::GfxLightGrid& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mins);
        EndianSwap(value.maxs);
        EndianSwap(value.rowAxis);
        EndianSwap(value.colAxis);
        EndianSwap(value.rowDataStart);
        EndianSwap(value.rawRowDataSize);
        EndianSwap(value.rawRowData);
        EndianSwap(value.entryCount);
        EndianSwap(value.entries);
        EndianSwap(value.colorCount);
        EndianSwap(value.colors);
    }

    static inline void EndianSwap(QOSXenon::GfxBrushModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfaceCount);
        EndianSwap(value.startSurfIndex);
        EndianSwap(value.unknown56);
    }

    static inline void EndianSwap(QOSXenon::MaterialMemory& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.material);
        EndianSwap(value.memory);
    }

    static inline void EndianSwap(QOSXenon::sunflare_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.spriteMaterial);
        EndianSwap(value.flareMaterial);
        EndianSwap(value.spriteSize);
        EndianSwap(value.flareMinSize);
        EndianSwap(value.flareMinDot);
        EndianSwap(value.flareMaxSize);
        EndianSwap(value.flareMaxDot);
        EndianSwap(value.flareMaxAlpha);
        EndianSwap(value.flareFadeInTime);
        EndianSwap(value.flareFadeOutTime);
        EndianSwap(value.blindMinDot);
        EndianSwap(value.blindMaxDot);
        EndianSwap(value.blindMaxDarken);
        EndianSwap(value.blindFadeInTime);
        EndianSwap(value.blindFadeOutTime);
        EndianSwap(value.glareMinDot);
        EndianSwap(value.glareMaxDot);
        EndianSwap(value.glareMaxLighten);
        EndianSwap(value.glareFadeInTime);
        EndianSwap(value.glareFadeOutTime);
        EndianSwap(value.sunFxPosition);
    }

    static inline void EndianSwap(QOSXenon::GfxDrawSurf& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.packed);
    }

    static inline void EndianSwap(QOSXenon::GfxSceneDynModel& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::GfxSceneDynBrush& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::GfxDynEntCellRef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dynEntIndex);
        EndianSwap(value.cellIndex);
        EndianSwap(value.nextIndex);
    }

    static inline void EndianSwap(QOSXenon::GfxShadowGeometry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.surfaceCount);
        EndianSwap(value.smodelCount);
        EndianSwap(value.sortedSurfIndex);
        EndianSwap(value.smodelIndex);
    }

    static inline void EndianSwap(QOSXenon::GfxLightRegion& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::GfxWorld& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.baseName);
        EndianSwap(value.planeCount);
        EndianSwap(value.planes);
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
        EndianSwap(value.indexCount);
        EndianSwap(value.indices);
        EndianSwap(value.surfaceCount);
        EndianSwap(value.surfaces);
        EndianSwap(value.streamInfo, operation);
        EndianSwap(value.skySurfCount);
        EndianSwap(value.skyStartSurfs);
        EndianSwap(value.skyImage);
        EndianSwap(value.skyBoxModel);
        EndianSwap(value.vertexCount);
        EndianSwap(value.vd, operation);
        EndianSwap(value.vertexLayerDataSize);
        EndianSwap(value.vld, operation);
        EndianSwap(value.sunLight);
        EndianSwap(value.sunColorFromBsp);
        EndianSwap(value.sunPrimaryLightIndex);
        EndianSwap(value.primaryLightCount);
        EndianSwap(value.unknown356);
        EndianSwap(value.cullGroupCount);
        EndianSwap(value.reflectionProbeCount);
        EndianSwap(value.reflectionProbes);
        EndianSwap(value.cullGroups);
        EndianSwap(value.smodelCount);
        EndianSwap(value.smodelDrawInsts);
        EndianSwap(value.smodelInsts);
        EndianSwap(value.cellCount);
        EndianSwap(value.cellBitsCount);
        EndianSwap(value.cells);
        EndianSwap(value.lightmapCount);
        EndianSwap(value.lightmaps);
        EndianSwap(value.lightGrid, operation);
        EndianSwap(value.modelCount);
        EndianSwap(value.models);
        EndianSwap(value.materialMemoryCount);
        EndianSwap(value.materialMemory);
        EndianSwap(value.sun, operation);
        EndianSwap(value.outdoorLookupMatrix);
        EndianSwap(value.outdoorImage);
        EndianSwap(value.dynEntDataSize0);
        EndianSwap(value.dynEntDataSize1);
        EndianSwap(value.sceneDynModelCount);
        EndianSwap(value.sceneDynBrushCount);
        EndianSwap(value.smodelVisData0);
        EndianSwap(value.smodelVisData1);
        EndianSwap(value.smodelVisData2);
        EndianSwap(value.smodelVisData3);
        EndianSwap(value.surfaceVisData0);
        EndianSwap(value.surfaceVisData1);
        EndianSwap(value.surfaceVisData2);
        EndianSwap(value.surfaceVisData3);
        EndianSwap(value.surfaceMaterials);
        EndianSwap(value.surfaceCastsSunShadow);
        EndianSwap(value.cellCasterBits);
        EndianSwap(value.cellHasSunLitSurfsBits);
        EndianSwap(value.dynEntVisData0_0);
        EndianSwap(value.dynEntVisData0_1);
        EndianSwap(value.dynEntVisData0_2);
        EndianSwap(value.dynEntVisData0_3);
        EndianSwap(value.dynEntVisData1_0);
        EndianSwap(value.dynEntVisData1_1);
        EndianSwap(value.dynEntVisData1_2);
        EndianSwap(value.dynEntVisData1_3);
        EndianSwap(value.dynEntCellBits0);
        EndianSwap(value.dynEntCellBits1);
        EndianSwap(value.dynEntSortList0);
        EndianSwap(value.dynEntSortList1);
        EndianSwap(value.sceneDynModel);
        EndianSwap(value.sceneDynBrush);
        EndianSwap(value.dynEntCellRefs);
        EndianSwap(value.dynEntUnknown);
        EndianSwap(value.sortedSurfIndex);
        EndianSwap(value.primaryLightEntityShadowVis);
        EndianSwap(value.primaryLightDynEntShadowVis0);
        EndianSwap(value.primaryLightDynEntShadowVis1);
        EndianSwap(value.shadowGeom);
        EndianSwap(value.lightRegionCount);
        EndianSwap(value.lightRegion);
        EndianSwap(value.unknownMaterial);
    }

    static inline void EndianSwap(QOSXenon::GfxLightImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image);
    }

    static inline void EndianSwap(QOSXenon::GfxLightDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.attenuation, operation);
        EndianSwap(value.lmapLookupStart);
    }

    static inline void EndianSwap(QOSXenon::Glyph& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.letter);
        EndianSwap(value.s0);
        EndianSwap(value.t0);
        EndianSwap(value.s1);
        EndianSwap(value.t1);
    }

    static inline void EndianSwap(QOSXenon::Font_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.fontName);
        EndianSwap(value.pixelHeight);
        EndianSwap(value.glyphCount);
        EndianSwap(value.material);
        EndianSwap(value.glowMaterial);
        EndianSwap(value.glyphs);
    }

    static inline void EndianSwap(QOSXenon::MenuList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.menuCount);
        EndianSwap(value.menus);
    }

    static inline void EndianSwap(QOSXenon::rectDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.x);
        EndianSwap(value.y);
        EndianSwap(value.w);
        EndianSwap(value.h);
        EndianSwap(value.horzAlign);
        EndianSwap(value.vertAlign);
    }

    static inline void EndianSwap(QOSXenon::windowDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.rect, operation);
        EndianSwap(value.rectClient, operation);
        EndianSwap(value.group);
        EndianSwap(value.style);
        EndianSwap(value.border);
        EndianSwap(value.ownerDraw);
        EndianSwap(value.ownerDrawFlags);
        EndianSwap(value.borderSize);
        EndianSwap(value.staticFlags);
        EndianSwap(value.dynamicFlags);
        EndianSwap(value.nextTime);
        EndianSwap(value.foreColor);
        EndianSwap(value.backColor);
        EndianSwap(value.borderColor);
        EndianSwap(value.outlineColor);
        EndianSwap(value.background);
    }

    static inline void EndianSwap(QOSXenon::ItemKeyHandler& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.key);
        EndianSwap(value.action);
        EndianSwap(value.next);
    }

    static inline void EndianSwap(QOSXenon::operandInternalDataUnion&, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::Operand& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto dataType = EndianSwapDiscriminator(value.dataType, operation);

        switch (dataType)
        {
        case QOSXenon::VAL_INT:
            EndianSwap(value.internals.intVal);
            break;
        case QOSXenon::VAL_FLOAT:
            EndianSwap(value.internals.floatVal);
            break;
        case QOSXenon::VAL_STRING:
            EndianSwap(value.internals.string);
            break;
        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(QOSXenon::entryInternalData&, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::expressionEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(value.type, operation);

        switch (type)
        {
        case QOSXenon::EET_OPERATOR:
            EndianSwap(value.data.op);
            break;
        case QOSXenon::EET_OPERAND:
            EndianSwap(value.data.operand, operation);
            break;
        default:
            assert(false);
            break;
        }
    }

    static inline void EndianSwap(QOSXenon::statement_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.numEntries);
        EndianSwap(value.entries);
    }

    static inline void EndianSwap(QOSXenon::columnInfo_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pos);
        EndianSwap(value.width);
        EndianSwap(value.maxChars);
        EndianSwap(value.alignment);
    }

    static inline void EndianSwap(QOSXenon::listBoxDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mousePos);
        EndianSwap(value.startPos);
        EndianSwap(value.endPos);
        EndianSwap(value.drawPadding);
        EndianSwap(value.elementWidth);
        EndianSwap(value.elementHeight);
        EndianSwap(value.elementStyle);
        EndianSwap(value.numColumns);

        for (auto& column : value.columnInfo)
            EndianSwap(column, operation);

        EndianSwap(value.doubleClick);
        EndianSwap(value.notselectable);
        EndianSwap(value.noScrollBars);
        EndianSwap(value.usePaging);
        EndianSwap(value.selectBorder);
        EndianSwap(value.disableColor);
        EndianSwap(value.selectIcon);
    }

    static inline void EndianSwap(QOSXenon::editFieldDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.minVal);
        EndianSwap(value.maxVal);
        EndianSwap(value.defVal);
        EndianSwap(value.range);
        EndianSwap(value.maxChars);
        EndianSwap(value.maxCharsGotoNext);
        EndianSwap(value.maxPaintChars);
        EndianSwap(value.paintOffset);
    }

    static inline void EndianSwap(QOSXenon::multiDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dvarList);
        EndianSwap(value.dvarStr);
        EndianSwap(value.dvarValue);
        EndianSwap(value.count);
        EndianSwap(value.strDef);
    }

    static inline void EndianSwap(QOSXenon::itemDefData_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.data);
    }

    static inline void EndianSwap(QOSXenon::itemDef_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);

        for (auto& rect : value.textRect)
            EndianSwap(rect, operation);

        EndianSwap(value.type);
        EndianSwap(value.dataType);
        EndianSwap(value.alignment);
        EndianSwap(value.fontEnum);
        EndianSwap(value.textAlignMode);
        EndianSwap(value.textalignx);
        EndianSwap(value.textaligny);
        EndianSwap(value.textscale);
        EndianSwap(value.textStyle);
        EndianSwap(value.gameMsgWindowIndex);
        EndianSwap(value.gameMsgWindowMode);
        EndianSwap(value.text);
        EndianSwap(value.textSavegameInfo);
        EndianSwap(value.parent);
        EndianSwap(value.mouseEnterText);
        EndianSwap(value.mouseExitText);
        EndianSwap(value.mouseEnter);
        EndianSwap(value.mouseExit);
        EndianSwap(value.action);
        EndianSwap(value.onAccept);
        EndianSwap(value.onFocus);
        EndianSwap(value.leaveFocus);
        EndianSwap(value.dvar);
        EndianSwap(value.dvarTest);
        EndianSwap(value.onKey);
        EndianSwap(value.enableDvar);
        EndianSwap(value.dvarFlags);
        EndianSwap(value.focusSound);
        EndianSwap(value.feeder);
        EndianSwap(value.cursorPos);
        EndianSwap(value.typeData, operation);
        EndianSwap(value.imageTrack);
        EndianSwap(value.visibleExp, operation);
        EndianSwap(value.textExp, operation);
        EndianSwap(value.materialExp, operation);
        EndianSwap(value.rectXExp, operation);
        EndianSwap(value.rectYExp, operation);
        EndianSwap(value.rectWExp, operation);
        EndianSwap(value.rectHExp, operation);
        EndianSwap(value.unknownStatement0, operation);
    }

    static inline void EndianSwap(QOSXenon::menuDef_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.window, operation);
        EndianSwap(value.font);
        EndianSwap(value.fullScreen);
        EndianSwap(value.itemCount);
        EndianSwap(value.fontIndex);
        EndianSwap(value.cursorItem);
        EndianSwap(value.fadeCycle);
        EndianSwap(value.fadeClamp);
        EndianSwap(value.fadeAmount);
        EndianSwap(value.fadeInAmount);
        EndianSwap(value.blurRadius);
        EndianSwap(value.onOpen);
        EndianSwap(value.onClose);
        EndianSwap(value.onESC);
        EndianSwap(value.onKey);
        EndianSwap(value.visibleExp, operation);
        EndianSwap(value.allowedBinding);
        EndianSwap(value.soundName);
        EndianSwap(value.imageTrack);
        EndianSwap(value.focusColor);
        EndianSwap(value.disableColor);
        EndianSwap(value.rectXExp, operation);
        EndianSwap(value.rectYExp, operation);
        EndianSwap(value.items);
    }

    static inline void EndianSwap(QOSXenon::WeaponDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.displayName);
        EndianSwap(value.overlayName);
        EndianSwap(value.unknownModel12);
        EndianSwap(value.unknownModel16);
        EndianSwap(value.xanimNames);
        EndianSwap(value.modeName);
        EndianSwap(value.unknownString196);
        EndianSwap(value.unknownFx204);
        EndianSwap(value.unknownFx208);

        for (auto& sound : value.unknownSoundAliases)
            EndianSwap(sound, operation);

        EndianSwap(value.bounceSound);
        EndianSwap(value.unknownFx428);
        EndianSwap(value.unknownFx432);
        EndianSwap(value.unknownFx436);
        EndianSwap(value.unknownFx440);
        EndianSwap(value.unknownMaterial444);
        EndianSwap(value.unknownMaterial448);
        EndianSwap(value.unknownModels604);
        EndianSwap(value.unknownString616);
        EndianSwap(value.unknownString620);
        EndianSwap(value.unknownModel624);
        EndianSwap(value.unknownMaterial628);
        EndianSwap(value.unknownMaterial636);
        EndianSwap(value.unknownString652);
        EndianSwap(value.unknownString660);
        EndianSwap(value.unknownString680);
        EndianSwap(value.unknownMaterial964);
        EndianSwap(value.unknownString992);
        EndianSwap(value.unknownString1220);
        EndianSwap(value.unknownMaterial1264);
        EndianSwap(value.unknownMaterial1276);
        EndianSwap(value.unknownString1300);
        EndianSwap(value.unknownString1308);
        EndianSwap(value.unknownModels1316);
        EndianSwap(value.unknownStrings1380);
        EndianSwap(value.unknownModel1524);
        EndianSwap(value.unknownFx1532);
        EndianSwap(value.unknownFx1540);
        EndianSwap(value.unknownSound1544, operation);
        EndianSwap(value.unknownSound1548, operation);
        EndianSwap(value.unknownFx1584);
        EndianSwap(value.unknownFx1588);
        EndianSwap(value.unknownSound1592, operation);
        EndianSwap(value.unknownFx1836);
        EndianSwap(value.unknownFx1864);
        EndianSwap(value.unknownSound1868, operation);
        EndianSwap(value.aiVsAiAccuracyGraphName);
        EndianSwap(value.aiVsPlayerAccuracyGraphName);
        EndianSwap(value.aiVsAiAccuracyGraphKnots);
        EndianSwap(value.aiVsPlayerAccuracyGraphKnots);
        EndianSwap(value.originalAiVsAiAccuracyGraphKnots);
        EndianSwap(value.originalAiVsPlayerAccuracyGraphKnots);
        EndianSwap(value.aiVsAiAccuracyGraphKnotCount);
        EndianSwap(value.aiVsPlayerAccuracyGraphKnotCount);
        EndianSwap(value.unknownString2192);
        EndianSwap(value.unknownString2196);
        EndianSwap(value.unknownString2216);
        EndianSwap(value.unknownString2332);
        EndianSwap(value.unknownString2336);
    }

    static inline void EndianSwap(QOSXenon::XAUDIOREVERBSETTINGS& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.ReflectionsDelay);
        EndianSwap(value.RoomFilterFreq);
        EndianSwap(value.RoomFilterMain);
        EndianSwap(value.RoomFilterHF);
        EndianSwap(value.ReflectionsGain);
        EndianSwap(value.ReverbGain);
        EndianSwap(value.DecayTime);
        EndianSwap(value.Density);
        EndianSwap(value.RoomSize);
    }

    static inline void EndianSwap(QOSXenon::XaReverbSettings& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.presetOverridden);
        EndianSwap(value.reverbSettings, operation);
    }

    static inline void EndianSwap(QOSXenon::SndDriverGlobals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.reverbSettings);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::FxIntRange& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base);
        EndianSwap(value.amplitude);
    }

    static inline void EndianSwap(QOSXenon::FxFloatRange& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base);
        EndianSwap(value.amplitude);
    }

    static inline void EndianSwap(QOSXenon::FxSpawnDefOneShot& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.count, operation);
    }

    static inline void EndianSwap(QOSXenon::FxSpawnDefLooping& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.intervalMsec);
        EndianSwap(value.count);
    }

    static inline void EndianSwap(QOSXenon::FxSpawnDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.looping, operation);
    }

    static inline void EndianSwap(QOSXenon::FxElemAtlas& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.entryCount);
    }

    static inline void EndianSwap(QOSXenon::FxElemVec3Range& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base);
        EndianSwap(value.amplitude);
    }

    static inline void EndianSwap(QOSXenon::FxElemVelStateInFrame& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.velocity, operation);
        EndianSwap(value.totalDelta, operation);
    }

    static inline void EndianSwap(QOSXenon::FxElemVelStateSample& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.local, operation);
        EndianSwap(value.world, operation);
    }

    static inline void EndianSwap(QOSXenon::FxElemVisualState& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.rotationDelta);
        EndianSwap(value.rotationTotal);
        EndianSwap(value.size);
        EndianSwap(value.scale);
    }

    static inline void EndianSwap(QOSXenon::FxElemVisStateSample& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.base, operation);
        EndianSwap(value.amplitude, operation);
    }

    static inline void EndianSwap(QOSXenon::FxEffectDefRef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.handle);
    }

    static inline void EndianSwap(QOSXenon::FxElemVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.anonymous);
    }

    static inline void EndianSwap(QOSXenon::FxElemMarkVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.materials);
    }

    static inline void EndianSwap(QOSXenon::FxElemDefVisuals& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.markArray);
    }

    static inline void EndianSwap(QOSXenon::FxTrailVertex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.pos);
        EndianSwap(value.normal);
        EndianSwap(value.texCoord);
    }

    static inline void EndianSwap(QOSXenon::FxTrailDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.scrollTimeMsec);
        EndianSwap(value.repeatDist);
        EndianSwap(value.splitDist);
        EndianSwap(value.vertCount);
        EndianSwap(value.verts);
        EndianSwap(value.indCount);
        EndianSwap(value.inds);
    }

    static inline void EndianSwap(QOSXenon::FxElemDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.flags);
        EndianSwap(value.spawn, operation);
        EndianSwap(value.spawnRange, operation);
        EndianSwap(value.fadeInRange, operation);
        EndianSwap(value.fadeOutRange, operation);
        EndianSwap(value.spawnFrustumCullRadius);
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
        EndianSwap(value.gravity, operation);
        EndianSwap(value.reflectionFactor, operation);
        EndianSwap(value.atlas, operation);
        EndianSwap(value.velSamples);
        EndianSwap(value.visSamples);
        EndianSwap(value.visuals, operation);
        EndianSwap(value.collMins);
        EndianSwap(value.collMaxs);
        EndianSwap(value.effectOnImpact, operation);
        EndianSwap(value.effectOnDeath, operation);
        EndianSwap(value.effectEmitted, operation);
        EndianSwap(value.emitDist, operation);
        EndianSwap(value.emitDistVariance, operation);
        EndianSwap(value.trailDef);
    }

    static inline void EndianSwap(QOSXenon::FxEffectDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.flags);
        EndianSwap(value.totalSize);
        EndianSwap(value.msecLoopingLife);
        EndianSwap(value.elemDefCountLooping);
        EndianSwap(value.elemDefCountOneShot);
        EndianSwap(value.elemDefCountEmission);
        EndianSwap(value.elemDefs);
    }

    static inline void EndianSwap(QOSXenon::DestructibleModel& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.model);
        EndianSwap(value.breakHealth);
        EndianSwap(value.breakEffect);
        EndianSwap(value.breakSound, operation);
        EndianSwap(value.breakNotify);
        EndianSwap(value.spawnModel);
    }

    static inline void EndianSwap(QOSXenon::DestructiblePiece& value, [[maybe_unused]] const EndianOperation operation)
    {
        for (auto& model : value.dmodels)
            EndianSwap(model, operation);

        EndianSwap(value.attachBone);
        EndianSwap(value.attachBoneHash);
        EndianSwap(value.parentDamagePercent);
        EndianSwap(value.bulletDamageScale);
        EndianSwap(value.explosiveDamageScale);
        EndianSwap(value.physConstraints);
        EndianSwap(value.health);
        EndianSwap(value.burnEffect);
        EndianSwap(value.burnSound, operation);
    }

    static inline void EndianSwap(QOSXenon::DestructibleDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknown0);
        EndianSwap(value.numPieces);
        EndianSwap(value.pieces);
    }

    static inline void EndianSwap(QOSXenon::FxImpactEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nonflesh);
        EndianSwap(value.flesh);
    }

    static inline void EndianSwap(QOSXenon::FxImpactTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.table);
    }

    static inline void EndianSwap(QOSXenon::complex_s& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.real);
        EndianSwap(value.imag);
    }

    static inline void EndianSwap(QOSXenon::WaterWritable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.floatTime);
    }

    static inline void EndianSwap(QOSXenon::water_t& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.writable.floatTime);
        EndianSwap(value.H0);
        EndianSwap(value.wTerm);
        EndianSwap(value.M);
        EndianSwap(value.N);
        EndianSwap(value.Lx);
        EndianSwap(value.Lz);
        EndianSwap(value.gravity);
        EndianSwap(value.windvel);
        EndianSwap(value.winddir);
        EndianSwap(value.amplitude);
        EndianSwap(value.codeConstant);
        EndianSwap(value.image);
    }

    static inline void EndianSwap(QOSXenon::MaterialWaterDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.water);
    }

    static inline void EndianSwap(QOSXenon::MaterialTextureDefInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.image);
    }

    static inline void EndianSwap(QOSXenon::MaterialTextureDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nameHash);
        EndianSwap(value.u.image);
    }

    static inline void EndianSwap(QOSXenon::MaterialConstantDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknown);
    }

    static inline void EndianSwap(QOSXenon::GfxStateBits& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadBits);
    }

    static inline void EndianSwap(QOSXenon::MaterialInfo& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::Material& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.info, operation);
        EndianSwap(value.techniqueSet);
        EndianSwap(value.textureTable);
        EndianSwap(value.constantTable);
        EndianSwap(value.stateBitsTable);
    }

    static inline void EndianSwap(QOSXenon::D3DTexture&, [[maybe_unused]] const EndianOperation operation)
    {
        // GPU resource and fetch words retain their serialized little-endian order.
    }

    static inline void EndianSwap(QOSXenon::GfxTexture& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.map);
    }

    static inline void EndianSwap(QOSXenon::GfxImageLoadDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.dimensions);
        EndianSwap(value.format);
        EndianSwap(value.texture, operation);
    }

    static inline void EndianSwap(QOSXenon::GfxTextureLoad& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadDef);
    }

    static inline void EndianSwap(QOSXenon::GfxImage& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.mapType);
        EndianSwap(value.texture.loadDef);
        EndianSwap(value.cardMemory.platform[0]);
        EndianSwap(value.width);
        EndianSwap(value.height);
        EndianSwap(value.depth);
        EndianSwap(value.pixels);
        EndianSwap(value.baseSize);
        EndianSwap(value.streamSlot);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::LocalizeEntry& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.value);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::RawFile& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.len);
        EndianSwap(value.buffer);
    }

    static inline void EndianSwap(QOSXenon::StringTable& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.columnCount);
        EndianSwap(value.rowCount);
        EndianSwap(value.values);
    }

    static inline void EndianSwap(QOSXenon::xmlAttribute& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.value);
        EndianSwap(value.unknown0);
    }

    static inline void EndianSwap(QOSXenon::xmlNodeList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeCount);
        EndianSwap(value.nodes);
    }

    static inline void EndianSwap(QOSXenon::xmlAttribList& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknown0);
        for (auto& attribute : value.attributes)
            EndianSwap(attribute);
    }

    static inline void EndianSwap(QOSXenon::xmlNodeComplex& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.attribList, operation);
        EndianSwap(value.childList, operation);
    }

    static inline void EndianSwap(QOSXenon::xmlNode& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknown0);
        EndianSwap(value.complex);
    }

    static inline void EndianSwap(QOSXenon::XmlTree& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.nodeList, operation);
        EndianSwap(value.name);
    }

    static inline void EndianSwap(QOSXenon::AnimReference& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknownString0);
        EndianSwap(value.unknownScriptString0);
        EndianSwap(value.animation);
    }

    static inline void EndianSwap(QOSXenon::SceneAnimation& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.animReferenceCount);
        EndianSwap(value.unknown0);
        EndianSwap(value.animReferences);
    }

    static inline void EndianSwap(QOSXenon::CutsceneModelAttachment& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknownString0);
        EndianSwap(value.unknownString1);
    }

    static inline void EndianSwap(QOSXenon::CutsceneEntityDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.unknownString0);
        EndianSwap(value.unknownString1);
        EndianSwap(value.unknownScriptString0);
        EndianSwap(value.modelAttachmentCount);
        EndianSwap(value.modelAttachments);
    }

    static inline void EndianSwap(QOSXenon::CutsceneSectionDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.entityCount);
        EndianSwap(value.entities);
        EndianSwap(value.sceneAnimationCount);
        EndianSwap(value.sceneAnimations);
    }

    static inline void EndianSwap(QOSXenon::Cutscene& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.entityCount);
        EndianSwap(value.entities);
        EndianSwap(value.sectionCount);
        EndianSwap(value.sections);
    }

    static inline void EndianSwap(QOSXenon::CustomCameraDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.unknown4);
        EndianSwap(value.unknownString0);
        EndianSwap(value.unknownString1);
    }

    static inline void EndianSwap(QOSXenon::CustomCamera& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.cameraCount);
        EndianSwap(value.cameras);
    }

    static inline void EndianSwap(QOSXenon::MaterialTechniqueSet& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.techniques);
    }

    static inline void EndianSwap(QOSXenon::MaterialVertexDeclaration& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::MaterialVertexShader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.prog.loadDef.cachedPart);
        EndianSwap(value.prog.loadDef.physicalPart);
        EndianSwap(value.prog.loadDef.cachedPartSize);
        EndianSwap(value.prog.loadDef.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::MaterialVertexShaderProgram& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadDef.cachedPart);
        EndianSwap(value.loadDef.physicalPart);
        EndianSwap(value.loadDef.cachedPartSize);
        EndianSwap(value.loadDef.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::GfxVertexShaderLoadDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cachedPart);
        EndianSwap(value.physicalPart);
        EndianSwap(value.cachedPartSize);
        EndianSwap(value.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::MaterialPixelShader& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.prog.loadDef.cachedPart);
        EndianSwap(value.prog.loadDef.physicalPart);
        EndianSwap(value.prog.loadDef.cachedPartSize);
        EndianSwap(value.prog.loadDef.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::MaterialPixelShaderProgram& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.loadDef.cachedPart);
        EndianSwap(value.loadDef.physicalPart);
        EndianSwap(value.loadDef.cachedPartSize);
        EndianSwap(value.loadDef.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::GfxPixelShaderLoadDef& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.cachedPart);
        EndianSwap(value.physicalPart);
        EndianSwap(value.cachedPartSize);
        EndianSwap(value.physicalPartSize);
    }

    static inline void EndianSwap(QOSXenon::MaterialShaderArgument& value, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(value.type, operation);

        EndianSwap(value.dest);

        switch (type)
        {
        case QOSXenon::MTL_ARG_LITERAL_VERTEX_CONST:
        case QOSXenon::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(value.u.literalConst);
            break;

        case QOSXenon::MTL_ARG_CODE_VERTEX_CONST:
        case QOSXenon::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(value.u.codeConst.index);
            break;

        case QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER:
            EndianSwap(value.u.codeSampler);
            break;

        case QOSXenon::MTL_ARG_MATERIAL_VERTEX_CONST:
        case QOSXenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
        case QOSXenon::MTL_ARG_MATERIAL_PIXEL_CONST:
            EndianSwap(value.u.nameHash);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(QOSXenon::MaterialArgumentDef& value, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(QOSXenon::MaterialPass& value, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.vertexDecl);
        EndianSwap(value.vertexShaderArray);
        EndianSwap(value.vertexShader);
        EndianSwap(value.pixelShader);
        EndianSwap(value.args);
    }

    static inline void
        EndianSwapPartial(QOSXenon::MaterialTechnique& value, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(value.name);
        EndianSwap(value.flags);
        EndianSwap(value.passCount);
    }

} // namespace QOSXenon
