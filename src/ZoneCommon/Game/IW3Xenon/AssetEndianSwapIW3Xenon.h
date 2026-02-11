#pragma once

#include "Utils/Endianness.h"

// ---- Endian Swap Helpers

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

// ---- PhysPreset

static inline void EndianSwap_PhysPreset(IW3Xenon::PhysPreset* v)
{
    EndianSwap(v->name);
    EndianSwap(v->type);
    EndianSwap(v->mass);
    EndianSwap(v->bounce);
    EndianSwap(v->friction);
    EndianSwap(v->bulletForceScale);
    EndianSwap(v->explosiveForceScale);
    EndianSwap(v->sndAliasPrefix);
    EndianSwap(v->piecesSpreadFraction);
    EndianSwap(v->piecesUpwardVelocity);
    // bool tempDefaultToCylinder
}

// ---- XAnimParts

static inline void EndianSwap_XAnimNotifyInfo(IW3Xenon::XAnimNotifyInfo* v)
{
    EndianSwap(v->name);
    EndianSwap(v->time);
}

static inline void EndianSwap_XAnimDeltaPart(IW3Xenon::XAnimDeltaPart* v)
{
    EndianSwap(v->trans);
    EndianSwap(v->quat);
}

static inline void EndianSwap_XAnimPartTrans_Partial(IW3Xenon::XAnimPartTrans* v, size_t loaded_size)
{
    // hard coded not dynamic
    EndianSwap(v->size);
}

static inline void EndianSwap_XAnimPartTransFrames_Partial(IW3Xenon::XAnimPartTransFrames* v, size_t loaded_size)
{
    EndianSwap(v->mins[0]);
    EndianSwap(v->mins[1]);
    EndianSwap(v->mins[2]);
    EndianSwap(v->size[0]);
    EndianSwap(v->size[1]);
    EndianSwap(v->size[2]);
    EndianSwap(v->frames._1);
}

static inline void EndianSwap_XAnimDynamicFrames(IW3Xenon::XAnimDynamicFrames* v)
{
    assert(false);
}

static inline void EndianSwap_XAnimDeltaPartQuat_Partial(IW3Xenon::XAnimDeltaPartQuat* v, size_t loaded_size)
{
    EndianSwap(v->size);
}

static inline void EndianSwap_XAnimDeltaPartQuatDataFrames_Partial(IW3Xenon::XAnimDeltaPartQuatDataFrames* v, size_t loaded_size)
{
    EndianSwap(v->frames);
}

static inline void EndianSwap_XAnimIndices(IW3Xenon::XAnimIndices* v)
{
    assert(false);
}

static inline void EndianSwap_XAnimParts(IW3Xenon::XAnimParts* v)
{
    EndianSwap(v->name);
    EndianSwap(v->dataByteCount);
    EndianSwap(v->dataShortCount);
    EndianSwap(v->dataIntCount);
    EndianSwap(v->randomDataByteCount);
    EndianSwap(v->randomDataIntCount);
    EndianSwap(v->numframes);

    EndianSwap(v->randomDataShortCount);
    EndianSwap(v->indexCount);

    EndianSwap(v->framerate);
    EndianSwap(v->frequency);
    EndianSwap(v->names);
    EndianSwap(v->dataByte);
    EndianSwap(v->dataShort);
    EndianSwap(v->dataInt);
    EndianSwap(v->randomDataShort);
    EndianSwap(v->randomDataByte);
    EndianSwap(v->randomDataInt);

    // Union of pointers
    EndianSwap(v->indices.data);

    EndianSwap(v->notify);
    EndianSwap(v->deltaPart);
}

// ---- XModel

static inline void EndianSwap_GfxPackedVertex(IW3Xenon::GfxPackedVertex* v)
{
    EndianSwap(v->xyz);
    EndianSwap(v->binormalSign);
    // TODO: check how to handle this union
    // GfxColor color;
    EndianSwap(v->texCoord.packed);
    EndianSwap(v->normal.packed);
    EndianSwap(v->tangent.packed);
}

static inline void EndianSwap_XSurfaceCollisionNode(IW3Xenon::XSurfaceCollisionNode* v)
{
    // XSurfaceCollisionAabb aabb;
    EndianSwap(v->aabb.mins);
    EndianSwap(v->aabb.maxs);
    EndianSwap(v->childBeginIndex);
    EndianSwap(v->childCount);
}

static inline void EndianSwap_XSurfaceCollisionLeaf(IW3Xenon::XSurfaceCollisionLeaf* v)
{
    EndianSwap(v->triangleBeginIndex);
}

static inline void EndianSwap_XModelHighMipBounds(IW3Xenon::XModelHighMipBounds* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
}

static inline void EndianSwap_DObjAnimMat(IW3Xenon::DObjAnimMat* v)
{
    EndianSwap(v->quat);
    EndianSwap(v->trans);
    EndianSwap(v->transWeight);
}

static inline void EndianSwap_XModelCollSurf_s(IW3Xenon::XModelCollSurf_s* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->boneIdx);
    EndianSwap(v->contents);
    EndianSwap(v->surfFlags);
}

static inline void EndianSwap_XBoneInfo(IW3Xenon::XBoneInfo* v)
{
    EndianSwap(v->bounds);
    EndianSwap(v->offset);
    EndianSwap(v->radiusSquared);
}

static inline void EndianSwap_XSurface(IW3Xenon::XSurface* v)
{
    EndianSwap(v->vertCount);
    EndianSwap(v->triCount);
    EndianSwap(v->triIndices);
    EndianSwap(v->vertInfo.vertCount);
    EndianSwap(v->vertInfo.vertsBlend);
    EndianSwap(v->verts0);
    EndianSwap(v->vertListCount);
    EndianSwap(v->vertList);
    EndianSwap(v->partBits);
}

static inline void EndianSwap_XSurfaceVertexInfo(IW3Xenon::XSurfaceVertexInfo* v)
{
    EndianSwap(v->vertCount);
    EndianSwap(v->vertsBlend);
}

static inline void EndianSwap_XRigidVertList(IW3Xenon::XRigidVertList* v)
{
    EndianSwap(v->boneOffset);
    EndianSwap(v->vertCount);
    EndianSwap(v->triOffset);
    EndianSwap(v->triCount);
    EndianSwap(v->collisionTree);
}

static inline void EndianSwap_XSurfaceCollisionTree(IW3Xenon::XSurfaceCollisionTree* v)
{
    EndianSwap(v->trans);
    EndianSwap(v->scale);
    EndianSwap(v->nodeCount);
    EndianSwap(v->nodes);
    EndianSwap(v->leafCount);
    EndianSwap(v->leafs);
}

static inline void EndianSwap_XModelStreamInfo(IW3Xenon::XModelStreamInfo* v)
{
    assert(false);
}

static inline void EndianSwap_PhysGeomList(IW3Xenon::PhysGeomList* v)
{
    EndianSwap(v->count);
    EndianSwap(v->geoms);
    // PhysMass mass
    EndianSwap(v->mass.centerOfMass);
    EndianSwap(v->mass.momentsOfInertia);
    EndianSwap(v->mass.productsOfInertia);
}

static inline void EndianSwap_PhysGeomInfo(IW3Xenon::PhysGeomInfo* v)
{
    EndianSwap(v->brush);
    EndianSwap(v->type);
    EndianSwap(v->orientation);
    EndianSwap(v->offset);
    EndianSwap(v->halfLengths);
}

static inline void EndianSwap_BrushWrapper(IW3Xenon::BrushWrapper* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->contents);
    EndianSwap(v->maxs);
    EndianSwap(v->numsides);
    EndianSwap(v->sides);
    EndianSwap(v->axialMaterialNum);
    EndianSwap(v->baseAdjacentSide);
    EndianSwap(v->firstAdjacentSideOffsets);
    // edgeCount[2][3] - unsigned __int8, no swap
    EndianSwap(v->totalEdgeCount);
    EndianSwap(v->planes);
}

static inline void EndianSwap_cbrushside_t(IW3Xenon::cbrushside_t* v)
{
    EndianSwap(v->plane);
    EndianSwap(v->materialNum);
    EndianSwap(v->firstAdjacentSideOffset);
    // edgeCount - unsigned __int8, no swap
}

static inline void EndianSwap_XModel(IW3Xenon::XModel* v)
{
    EndianSwap(v->name);
    EndianSwap(v->boneNames);
    EndianSwap(v->parentList);
    EndianSwap(v->quats);
    EndianSwap(v->trans);
    EndianSwap(v->partClassification);
    EndianSwap(v->baseMat);
    EndianSwap(v->surfs);
    EndianSwap(v->materialHandles);
    for (int i = 0; i < 4; i++)
    {
        EndianSwap(v->lodInfo[i].dist);
        EndianSwap(v->lodInfo[i].numsurfs);
        EndianSwap(v->lodInfo[i].surfIndex);
        EndianSwap(v->lodInfo[i].partBits);
    }
    EndianSwap(v->collSurfs);
    EndianSwap(v->numCollSurfs);
    EndianSwap(v->contents);
    EndianSwap(v->boneInfo);
    EndianSwap(v->radius);
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->numLods);
    EndianSwap(v->collLod);
    EndianSwap(v->streamInfo.highMipBounds);
    EndianSwap(v->memUsage);
    EndianSwap(v->physPreset);
    EndianSwap(v->physGeoms);
}

// ---- Material

static inline void EndianSwap_MaterialVertexDeclaration(IW3Xenon::MaterialVertexDeclaration* v)
{
    // unsigned __int8 streamCount;
    // bool hasOptionalSource;
    // MaterialVertexStreamRouting routing; - all unsigned __int8 fields
}

static inline void EndianSwap_MaterialConstantDef(IW3Xenon::MaterialConstantDef* v)
{
    EndianSwap(v->nameHash);
    // char name[12];
    EndianSwap(v->literal);
}

static inline void EndianSwap_GfxStateBits(IW3Xenon::GfxStateBits* v)
{
    EndianSwap(v->loadBits);
}

static inline void EndianSwap_MaterialTextureDef(IW3Xenon::MaterialTextureDef* v)
{
    EndianSwap(v->nameHash);
    EndianSwap(v->u.image);
}

static inline void EndianSwap_MaterialInfo(IW3Xenon::MaterialInfo* v)
{
    // name: XString handle (zone pointer)
    EndianSwap(v->name);

    // gameFlags, sortKey, textureAtlasRowCount, textureAtlasColumnCount: all uint8, no swap needed

    // drawSurf: union with uint64 packed field
    EndianSwap(v->drawSurf.packed);

    // surfaceTypeBits: uint32
    EndianSwap(v->surfaceTypeBits);
}

static inline void EndianSwap_MaterialTextureDefInfo(IW3Xenon::MaterialTextureDefInfo* v)
{
    // Union: could be either image or water pointer
    // Both are pointers, so we swap as a generic 32-bit pointer
    EndianSwap(v->image);
}

static inline void EndianSwap_water_t(IW3Xenon::water_t* v)
{
    // writable.floatTime: float (32-bit), needs swap
    EndianSwap(v->writable.floatTime);

    // Pointer fields
    EndianSwap(v->H0X);
    EndianSwap(v->H0Y);
    EndianSwap(v->wTerm);

    // Integer fields
    EndianSwap(v->M);
    EndianSwap(v->N);

    // Float fields
    EndianSwap(v->Lx);
    EndianSwap(v->Lz);
    EndianSwap(v->gravity);
    EndianSwap(v->windvel);
    EndianSwap(v->winddir);
    EndianSwap(v->amplitude);

    // Float array
    EndianSwap(v->codeConstant);

    // Image pointer
    EndianSwap(v->image);
}

static inline void EndianSwap_Material(IW3Xenon::Material* v)
{
    // info: MaterialInfo struct - handle via its fixup function
    EndianSwap_MaterialInfo(&v->info);

    // stateBitsEntry[26]: array of uint8, no swap needed
    // textureCount, constantCount, stateBitsCount, stateFlags, cameraRegion: all uint8, no swap needed

    // Pointer fields
    EndianSwap(v->techniqueSet);
    EndianSwap(v->textureTable);
    EndianSwap(v->constantTable);
    EndianSwap(v->stateBitsTable);
}

// ---- MaterialTechniqueSet

static inline void EndianSwap_MaterialTechniqueSet(IW3Xenon::MaterialTechniqueSet* v)
{
    EndianSwap(v->name);
    EndianSwap(v->remappedTechniqueSet);
    EndianSwap(v->techniques);
}

static inline void EndianSwap_MaterialVertexShader(IW3Xenon::MaterialVertexShader* v)
{
    EndianSwap(v->name);

    auto& ld = v->prog.loadDef;
    EndianSwap(ld.cachedPart);
    EndianSwap(ld.physicalPart);
    EndianSwap(ld.cachedPartSize);
    EndianSwap(ld.physicalPartSize);
}

static inline void EndianSwap_MaterialVertexShaderProgram(IW3Xenon::MaterialVertexShaderProgram* v)
{
    // Embedded loadDef is handled separately to avoid double-swapping
    assert(false);
}

static inline void EndianSwap_GfxVertexShaderLoadDef(IW3Xenon::GfxVertexShaderLoadDef* v)
{
    assert(false);
}

static inline void EndianSwap_MaterialPixelShader(IW3Xenon::MaterialPixelShader* v)
{
    // name: XString handle (FOLLOWING / OFFSET / etc.)
    EndianSwap(v->name);

    // prog: union storage from the file is treated as loadDef for zone loading
    auto& ld = v->prog.loadDef;

    EndianSwap(ld.cachedPart);
    EndianSwap(ld.physicalPart);

    EndianSwap(ld.cachedPartSize);
    EndianSwap(ld.physicalPartSize);
}

static inline void EndianSwap_MaterialPixelShaderProgram(IW3Xenon::MaterialPixelShaderProgram* v)
{
    assert(false);
}

static inline void EndianSwap_GfxPixelShaderLoadDef(IW3Xenon::GfxPixelShaderLoadDef* v)
{
    assert(false);
}

static inline void EndianSwap_MaterialShaderArgument(IW3Xenon::MaterialShaderArgument* v)
{
    EndianSwap(v->type);
    EndianSwap(v->dest);

    // Swap union field based on type
    switch (v->type)
    {
    case IW3Xenon::MTL_ARG_LITERAL_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_LITERAL_PIXEL_CONST:
        // literalConst: pointer
        EndianSwap(v->u.literalConst);
        break;

    case IW3Xenon::MTL_ARG_CODE_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_CODE_PIXEL_CONST:
        // codeConst: struct with uint16 index + uint8 firstRow + uint8 rowCount
        EndianSwap(v->u.codeConst.index);
        // firstRow and rowCount are uint8, no swap needed
        break;

    case IW3Xenon::MTL_ARG_CODE_PIXEL_SAMPLER:
        // codeSampler: uint32
        EndianSwap(v->u.codeSampler);
        break;

    case IW3Xenon::MTL_ARG_MATERIAL_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
    case IW3Xenon::MTL_ARG_MATERIAL_PIXEL_CONST:
        // nameHash: uint32
        EndianSwap(v->u.nameHash);
        break;

    default:
        assert(false);
        break;
    }
}

static inline void EndianSwap_MaterialArgumentDef(IW3Xenon::MaterialArgumentDef* v)
{
    assert(false);
}

static inline void EndianSwap_MaterialPass(IW3Xenon::MaterialPass* v)
{
    EndianSwap(v->vertexDecl);

    EndianSwap(v->vertexShaderArray);

    EndianSwap(v->vertexShader);
    EndianSwap(v->pixelShader);
    EndianSwap(v->args);

    // uint8 fields: no swap
}

static inline void EndianSwap_MaterialTechnique_Partial(IW3Xenon::MaterialTechnique* v, size_t loaded_size)
{
    // TODO: not dynamic at the moment as we know the only callsite

    EndianSwap(v->name);
    EndianSwap(v->flags);
    EndianSwap(v->passCount);
}

// ---- GfxImage

static inline void EndianSwap_DummyD3DVolumeTexture(IW3Xenon::DummyD3DVolumeTexture* v)
{
    return;
}

static inline void EndianSwap_DummyD3DCubeTexture(IW3Xenon::DummyD3DCubeTexture* v)
{
    return;
}

static inline void EndianSwap_DummyD3DTexture(IW3Xenon::DummyD3DTexture* v)
{
    return;
}

static inline void EndianSwap_GfxTexture(IW3Xenon::GfxTexture* v)
{
    // Union with loadDef pointer
    EndianSwap(v->cubemap);
}

static inline void EndianSwap_GfxImageLoadDef(IW3Xenon::GfxImageLoadDef* v)
{
    // levelCount: uint8, no swap needed
    // flags: uint8, no swap needed

    // dimensions[3]: array of int16
    EndianSwap(v->dimensions);

    // format: int (32-bit)
    EndianSwap(v->format);

    // texture: GfxTexture union - handle via its fixup function
    EndianSwap_GfxTexture(&v->texture);
}

static inline void EndianSwap_GfxTextureLoad(IW3Xenon::GfxTextureLoad* v)
{
    EndianSwap(v->loadDef);
}

static inline void EndianSwap_GfxImage(IW3Xenon::GfxImage* v)
{
    EndianSwap(v->mapType);

    // texture: GfxTexture union
    EndianSwap(v->texture.loadDef);

    // semantic: uint8, no swap needed

    // cardMemory.platform[1]: array of int (32-bit)
    EndianSwap(v->cardMemory.platform[0]);

    // width, height, depth: uint16
    EndianSwap(v->width);
    EndianSwap(v->height);
    EndianSwap(v->depth);

    // category: uint8, no swap needed
    // delayLoadPixels: bool, no swap needed

    // pixels: pointer
    EndianSwap(v->pixels);

    // baseSize: uint (32-bit)
    EndianSwap(v->baseSize);

    // streamSlot: uint16
    EndianSwap(v->streamSlot);

    // streaming: bool, no swap needed

    // name: pointer
    EndianSwap(v->name);
}

// ---- snd_alias_list_t

static inline void EndianSwap_XAUDIOCHANNELMAPENTRY(IW3Xenon::XAUDIOCHANNELMAPENTRY* v)
{
    // unsigned __int8 InputChannel;
    // unsigned __int8 OutputChannel;
    EndianSwap(v->Volume);
}

static inline void EndianSwap_snd_alias_t(IW3Xenon::snd_alias_t* v)
{
    EndianSwap(v->aliasName);
    EndianSwap(v->subtitle);
    EndianSwap(v->secondaryAliasName);
    EndianSwap(v->chainAliasName);
    EndianSwap(v->soundFile);
    EndianSwap(v->sequence);
    EndianSwap(v->volMin);
    EndianSwap(v->volMax);
    EndianSwap(v->pitchMin);
    EndianSwap(v->pitchMax);
    EndianSwap(v->distMin);
    EndianSwap(v->distMax);
    EndianSwap(v->flags);
    EndianSwap(v->slavePercentage);
    EndianSwap(v->probability);
    EndianSwap(v->lfePercentage);
    EndianSwap(v->centerPercentage);
    EndianSwap(v->startDelay);
    EndianSwap(v->volumeFalloffCurve);
    EndianSwap(v->envelopMin);
    EndianSwap(v->envelopMax);
    EndianSwap(v->envelopPercentage);
    EndianSwap(v->speakerMap);
}

static inline void EndianSwap_XAUDIOCHANNELMAP(IW3Xenon::XAUDIOCHANNELMAP* v)
{
    assert(false);
}

static inline void EndianSwap_SoundFile(IW3Xenon::SoundFile* v)
{
    // type and exists are bytes - no swap
    if (v->type == IW3Xenon::SAT_LOADED)
    {
        EndianSwap(v->u.loadSnd);
    }
    else if (v->type == IW3Xenon::SAT_STREAMED)
    {
        EndianSwap(v->u.streamSnd.filename.fileIndex);
        // StreamFileInfo: either raw (2 pointers) or packed (2 uints) - same byte sizes
        EndianSwap(v->u.streamSnd.filename.info.raw.dir);
        EndianSwap(v->u.streamSnd.filename.info.raw.name);
    }
}

static inline void EndianSwap_SoundFileRef(IW3Xenon::SoundFileRef* v)
{
    assert(false);
}

static inline void EndianSwap_StreamedSound(IW3Xenon::StreamedSound* v)
{
    assert(false);
}

static inline void EndianSwap_StreamFileName(IW3Xenon::StreamFileName* v)
{
    assert(false);
}

static inline void EndianSwap_StreamFileInfo(IW3Xenon::StreamFileInfo* v)
{
    assert(false);
}

static inline void EndianSwap_StreamFileNameRaw(IW3Xenon::StreamFileNameRaw* v)
{
    assert(false);
}

static inline void EndianSwap_SpeakerMap(IW3Xenon::SpeakerMap* v)
{
    // isDefault is bool (1 byte) - no swap
    EndianSwap(v->name);
    // XAUDIOCHANNELMAP channelMaps[2][2]
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            // EntryCount is byte - no swap
            EndianSwap(v->channelMaps[i][j].paEntries);
        }
    }
}

static inline void EndianSwap_snd_alias_list_t(IW3Xenon::snd_alias_list_t* v)
{
    EndianSwap(v->aliasName);
    EndianSwap(v->head);
    EndianSwap(v->count);
}

// ---- SndCurve

static inline void EndianSwap_SndCurve(IW3Xenon::SndCurve* v)
{
    EndianSwap(v->filename);
    EndianSwap(v->knotCount);

    EndianSwap(v->knots);
}

// ---- LoadedSound

static inline void EndianSwap_XaSound(IW3Xenon::XaSound* v)
{
    assert(false);
}

static inline void EndianSwap_XAUDIOPACKET_ALIGNED(IW3Xenon::XAUDIOPACKET_ALIGNED* v)
{
    assert(false);
}

static inline void EndianSwap_XaSeekTable(IW3Xenon::XaSeekTable* v)
{
    assert(false);
}

static inline void EndianSwap_LoadedSound(IW3Xenon::LoadedSound* v)
{
    EndianSwap(v->name);

    // XaSound sound
    // XAUDIOPACKET_ALIGNED packet
    EndianSwap(v->sound.packet.pBuffer);
    EndianSwap(v->sound.packet.BufferSize);
    EndianSwap(v->sound.packet.LoopCount);
    // XMALOOPREGION XMALoop[6]
    for (int i = 0; i < 6; i++)
    {
        EndianSwap(v->sound.packet.XMALoop[i].LoopStart);
        EndianSwap(v->sound.packet.XMALoop[i].LoopEnd);
        // LoopSubframeEnd and LoopSubframeSkip are bytes - no swap
    }
    EndianSwap(v->sound.packet.pContext);

    // XAUDIOSOURCEFORMAT format
    // SampleType is byte - no swap
    // TODO: find enum and correctly endian swap

    // XaIwXmaDataInfo xaIwXmaDataInfo
    EndianSwap(v->sound.xaIwXmaDataInfo.totalMsec);

    // XaSeekTable seekTable
    EndianSwap(v->sound.seekTable.size);
    EndianSwap(v->sound.seekTable.data);
}

// ---- clipMap_t

static inline void EndianSwap_cplane_s(IW3Xenon::cplane_s* v)
{
    EndianSwap(v->normal);
    EndianSwap(v->dist);
    // unsigned __int8 type;
    // unsigned __int8 signbits;
    // unsigned __int8 pad[2];
}

static inline void EndianSwap_dmaterial_t(IW3Xenon::dmaterial_t* v)
{
    // char material[64];
    EndianSwap(v->surfaceFlags);
    EndianSwap(v->contentFlags);
}

static inline void EndianSwap_cLeaf_t(IW3Xenon::cLeaf_t* v)
{
    EndianSwap(v->firstCollAabbIndex);
    EndianSwap(v->collAabbCount);
    EndianSwap(v->brushContents);
    EndianSwap(v->terrainContents);
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->leafBrushNode);
    EndianSwap(v->cluster);
}

static inline void EndianSwap_CollisionBorder(IW3Xenon::CollisionBorder* v)
{
    EndianSwap(v->distEq);
    EndianSwap(v->zBase);
    EndianSwap(v->zSlope);
    EndianSwap(v->start);
    EndianSwap(v->length);
}

static inline void EndianSwap_CollisionAabbTree(IW3Xenon::CollisionAabbTree* v)
{
    EndianSwap(v->origin);
    EndianSwap(v->halfSize);
    EndianSwap(v->materialIndex);
    EndianSwap(v->childCount);
    // Union of ints
    EndianSwap(v->u.firstChildIndex);
}

static inline void EndianSwap_cmodel_t(IW3Xenon::cmodel_t* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->radius);
    EndianSwap_cLeaf_t(&v->leaf);
}

static inline void EndianSwap_DynEntityPose(IW3Xenon::DynEntityPose* v)
{
    // GfxPlacement pose;
    EndianSwap(v->pose.quat);
    EndianSwap(v->pose.origin);
    EndianSwap(v->radius);
}

static inline void EndianSwap_DynEntityClient(IW3Xenon::DynEntityClient* v)
{
    EndianSwap(v->physObjId);
    EndianSwap(v->flags);
    EndianSwap(v->lightingHandle);
    EndianSwap(v->health);
}

static inline void EndianSwap_DynEntityColl(IW3Xenon::DynEntityColl* v)
{
    EndianSwap(v->sector);
    EndianSwap(v->nextEntInSector);
    EndianSwap(v->linkMins);
    EndianSwap(v->linkMaxs);
}

static inline void EndianSwap_XModelPieces(IW3Xenon::XModelPieces* v)
{
    assert(false);
}

static inline void EndianSwap_cLeafBrushNodeLeaf_t(IW3Xenon::cLeafBrushNodeLeaf_t* v)
{
    assert(false);
}

static inline void EndianSwap_cLeafBrushNodeData_t(IW3Xenon::cLeafBrushNodeData_t* v)
{
    assert(false);
}

static inline void EndianSwap_XModelPiece(IW3Xenon::XModelPiece* v)
{
    assert(false);
}

static inline void EndianSwap_DynEntityDef(IW3Xenon::DynEntityDef* v)
{
    EndianSwap(v->type);

    // GfxPlacement pose (embedded)
    EndianSwap(v->pose.quat);
    EndianSwap(v->pose.origin);

    EndianSwap(v->xModel);
    EndianSwap(v->brushModel);
    EndianSwap(v->physicsBrushModel);
    EndianSwap(v->destroyFx);
    EndianSwap(v->destroyPieces);
    EndianSwap(v->physPreset);
    EndianSwap(v->health);

    // PhysMass mass (embedded)
    EndianSwap(v->mass.centerOfMass);
    EndianSwap(v->mass.momentsOfInertia);
    EndianSwap(v->mass.productsOfInertia);

    EndianSwap(v->contents);
}

static inline void EndianSwap_cbrush_t(IW3Xenon::cbrush_t* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->contents);
    EndianSwap(v->maxs);
    EndianSwap(v->numsides);
    EndianSwap(v->sides);
    EndianSwap(v->axialMaterialNum);
    EndianSwap(v->baseAdjacentSide);
    EndianSwap(v->firstAdjacentSideOffsets);
    // edgeCount[2][3] - unsigned __int8, no swap
}

static inline void EndianSwap_CollisionPartition(IW3Xenon::CollisionPartition* v)
{
    // triCount - unsigned __int8, no swap
    // borderCount - unsigned __int8, no swap
    EndianSwap(v->firstTri);
    EndianSwap(v->borders);
}

static inline void EndianSwap_cLeafBrushNode_s(IW3Xenon::cLeafBrushNode_s* v)
{
    // axis - unsigned __int8, no swap
    EndianSwap(v->leafBrushCount);
    EndianSwap(v->contents);

    // cLeafBrushNodeData_t data (union) - axis determines which member
    if (v->axis >= 3)
    {
        // leaf
        EndianSwap(v->data.leaf.brushes);
    }
    else
    {
        // children
        EndianSwap(v->data.children.dist);
        EndianSwap(v->data.children.range);
        EndianSwap(v->data.children.childOffset);
    }
}

static inline void EndianSwap_cNode_t(IW3Xenon::cNode_t* v)
{
    EndianSwap(v->plane);
    EndianSwap(v->children);
}

static inline void EndianSwap_cStaticModel_s(IW3Xenon::cStaticModel_s* v)
{
    // cStaticModelWritable writable (embedded)
    EndianSwap(v->writable.nextModelInWorldSector);

    EndianSwap(v->xmodel);
    EndianSwap(v->origin);
    EndianSwap(v->invScaledAxis);
    EndianSwap(v->absmin);
    EndianSwap(v->absmax);
}

static inline void EndianSwap_clipMap_t(IW3Xenon::clipMap_t* v)
{
    EndianSwap(v->name);
    EndianSwap(v->isInUse);
    EndianSwap(v->planeCount);
    EndianSwap(v->planes);
    EndianSwap(v->numStaticModels);
    EndianSwap(v->staticModelList);
    EndianSwap(v->numMaterials);
    EndianSwap(v->materials);
    EndianSwap(v->numBrushSides);
    EndianSwap(v->brushsides);
    EndianSwap(v->numBrushEdges);
    EndianSwap(v->brushEdges);
    EndianSwap(v->numNodes);
    EndianSwap(v->nodes);
    EndianSwap(v->numLeafs);
    EndianSwap(v->leafs);
    EndianSwap(v->leafbrushNodesCount);
    EndianSwap(v->leafbrushNodes);
    EndianSwap(v->numLeafBrushes);
    EndianSwap(v->leafbrushes);
    EndianSwap(v->numLeafSurfaces);
    EndianSwap(v->leafsurfaces);
    EndianSwap(v->vertCount);
    EndianSwap(v->verts);
    EndianSwap(v->triCount);
    EndianSwap(v->triIndices);
    EndianSwap(v->triEdgeIsWalkable);
    EndianSwap(v->borderCount);
    EndianSwap(v->borders);
    EndianSwap(v->partitionCount);
    EndianSwap(v->partitions);
    EndianSwap(v->aabbTreeCount);
    EndianSwap(v->aabbTrees);
    EndianSwap(v->numSubModels);
    EndianSwap(v->cmodels);
    EndianSwap(v->numBrushes);
    EndianSwap(v->brushes);
    EndianSwap(v->numClusters);
    EndianSwap(v->clusterBytes);
    EndianSwap(v->visibility);
    EndianSwap(v->vised);
    EndianSwap(v->mapEnts);
    EndianSwap(v->box_brush);

    // cmodel_t box_model (embedded)
    EndianSwap(v->box_model.mins);
    EndianSwap(v->box_model.maxs);
    EndianSwap(v->box_model.radius);
    // cLeaf_t leaf (embedded in box_model)
    EndianSwap(v->box_model.leaf.firstCollAabbIndex);
    EndianSwap(v->box_model.leaf.collAabbCount);
    EndianSwap(v->box_model.leaf.brushContents);
    EndianSwap(v->box_model.leaf.terrainContents);
    EndianSwap(v->box_model.leaf.mins);
    EndianSwap(v->box_model.leaf.maxs);
    EndianSwap(v->box_model.leaf.leafBrushNode);
    EndianSwap(v->box_model.leaf.cluster);

    EndianSwap(v->dynEntCount);
    EndianSwap(v->dynEntDefList);
    EndianSwap(v->dynEntPoseList);
    EndianSwap(v->dynEntClientList);
    EndianSwap(v->dynEntCollList);
    EndianSwap(v->checksum);
}

// ---- ComWorld

static inline void EndianSwap_ComPrimaryLight(IW3Xenon::ComPrimaryLight* v)
{
    EndianSwap(v->color);
    EndianSwap(v->dir);
    EndianSwap(v->origin);
    EndianSwap(v->radius);
    EndianSwap(v->cosHalfFovOuter);
    EndianSwap(v->cosHalfFovInner);
    EndianSwap(v->cosHalfFovExpanded);
    EndianSwap(v->rotationLimit);
    EndianSwap(v->translationLimit);
    EndianSwap(v->defName);
}

static inline void EndianSwap_ComWorld(IW3Xenon::ComWorld* v)
{
    EndianSwap(v->name);
    EndianSwap(v->isInUse);
    EndianSwap(v->primaryLightCount);
    EndianSwap(v->primaryLights);
}

// ---- GameWorldSp

static inline void EndianSwap_pathbasenode_t(IW3Xenon::pathbasenode_t* v)
{
    EndianSwap(v->vOrigin);
    EndianSwap(v->type);
}

static inline void EndianSwap_pathlink_s(IW3Xenon::pathlink_s* v)
{
    EndianSwap(v->fDist);
    EndianSwap(v->nodeNum);
    //     unsigned __int8 disconnectCount;
    // unsigned __int8 negotiationLink;
    // unsigned __int8 ubBadPlaceCount[4];
}

static inline void EndianSwap_pathnode_t(IW3Xenon::pathnode_t* v)
{
    // pathnode_constant_t constant (embedded)
    EndianSwap(v->constant.type);
    EndianSwap(v->constant.spawnflags);
    EndianSwap(v->constant.targetname);
    EndianSwap(v->constant.script_linkName);
    EndianSwap(v->constant.script_noteworthy);
    EndianSwap(v->constant.target);
    EndianSwap(v->constant.animscript);
    EndianSwap(v->constant.animscriptfunc);
    EndianSwap(v->constant.vOrigin);
    EndianSwap(v->constant.fAngle);
    EndianSwap(v->constant.forward);
    EndianSwap(v->constant.fRadius);
    EndianSwap(v->constant.minUseDistSq);
    EndianSwap(v->constant.wOverlapNode);
    EndianSwap(v->constant.wChainId);
    EndianSwap(v->constant.wChainDepth);
    EndianSwap(v->constant.wChainParent);
    EndianSwap(v->constant.totalLinkCount);
    EndianSwap(v->constant.Links);

    // pathnode_dynamic_t dynamic (embedded)
    EndianSwap(v->dynamic.pOwner);
    EndianSwap(v->dynamic.iFreeTime);
    EndianSwap(v->dynamic.iValidTime);
    EndianSwap(v->dynamic.inPlayerLOSTime);
    EndianSwap(v->dynamic.wLinkCount);
    EndianSwap(v->dynamic.wOverlapCount);
    EndianSwap(v->dynamic.turretEntNumber);
    EndianSwap(v->dynamic.userCount);

    // pathnode_transient_t transient (embedded)
    EndianSwap(v->transient.iSearchFrame);
    EndianSwap(v->transient.pNextOpen);
    EndianSwap(v->transient.pPrevOpen);
    EndianSwap(v->transient.pParent);
    EndianSwap(v->transient.fCost);
    EndianSwap(v->transient.fHeuristic);
    EndianSwap(v->transient.costFactor);
}

static inline void EndianSwap_pathnode_tree_t(IW3Xenon::pathnode_tree_t* v)
{
    EndianSwap(v->axis);
    EndianSwap(v->dist);
    if (v->axis >= 0)
    {
        EndianSwap(v->u.child[0]);
        EndianSwap(v->u.child[1]);
    }
    else
    {
        EndianSwap(v->u.s.nodeCount);
        EndianSwap(v->u.s.nodes);
    }
}

static inline void EndianSwap_PathData(IW3Xenon::PathData* v)
{
    assert(false);
}

static inline void EndianSwap_pathnode_constant_t(IW3Xenon::pathnode_constant_t* v)
{
    assert(false);
}

static inline void EndianSwap_pathnode_tree_info_t(IW3Xenon::pathnode_tree_info_t* v)
{
    assert(false);
}

static inline void EndianSwap_pathnode_tree_nodes_t(IW3Xenon::pathnode_tree_nodes_t* v)
{
    EndianSwap(v->nodeCount);
    EndianSwap(v->nodes);
}

static inline void EndianSwap_GameWorldSp(IW3Xenon::GameWorldSp* v)
{
    EndianSwap(v->name);

    // PathData path (embedded)
    EndianSwap(v->path.nodeCount);
    EndianSwap(v->path.nodes);
    EndianSwap(v->path.basenodes);
    EndianSwap(v->path.chainNodeCount);
    EndianSwap(v->path.chainNodeForNode);
    EndianSwap(v->path.nodeForChainNode);
    EndianSwap(v->path.visBytes);
    EndianSwap(v->path.pathVis);
    EndianSwap(v->path.nodeTreeCount);
    EndianSwap(v->path.nodeTree);
}

// ---- GameWorldMp

static inline void EndianSwap_GameWorldMp(IW3Xenon::GameWorldMp* v)
{
    EndianSwap(v->name);
}

// ---- MapEnts

static inline void EndianSwap_MapEnts(IW3Xenon::MapEnts* v)
{
    EndianSwap(v->name);
    EndianSwap(v->entityString);
    EndianSwap(v->numEntityChars);
}

// ---- GfxWorld

static inline void EndianSwap_GfxStreamingAabbTree(IW3Xenon::GfxStreamingAabbTree* v)
{
    EndianSwap(v->firstItem);
    EndianSwap(v->itemCount);
    EndianSwap(v->firstChild);
    EndianSwap(v->childCount);
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
}

static inline void EndianSwap_GfxLightGridEntry(IW3Xenon::GfxLightGridEntry* v)
{
    EndianSwap(v->colorsIndex);
    // unsigned __int8 primaryLightIndex;
    // unsigned __int8 needsTrace;
}

static inline void EndianSwap_GfxLightGridColors(IW3Xenon::GfxLightGridColors* v)
{
    // unsigned __int8 rgb[56][3];
}

static inline void EndianSwap_GfxWorldVertex(IW3Xenon::GfxWorldVertex* v)
{
    EndianSwap(v->xyz);
    EndianSwap(v->binormalSign);
    // GfxColor color;
    EndianSwap(v->texCoord);
    EndianSwap(v->lmapCoord);
    EndianSwap(v->normal.packed);
    EndianSwap(v->tangent.packed);
}

static inline void EndianSwap_GfxLightRegionAxis(IW3Xenon::GfxLightRegionAxis* v)
{
    EndianSwap(v->dir);
    EndianSwap(v->midPoint);
    EndianSwap(v->halfSize);
}

static inline void EndianSwap_GfxStaticModelInst(IW3Xenon::GfxStaticModelInst* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    // GfxColor groundLighting;
}

static inline void EndianSwap_GfxCullGroup(IW3Xenon::GfxCullGroup* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->surfaceCount);
    EndianSwap(v->startSurfIndex);
}

static inline void EndianSwap_GfxDrawSurf(IW3Xenon::GfxDrawSurf* v)
{
    EndianSwap(v->packed);
}

static inline void EndianSwap_GfxBrushModel(IW3Xenon::GfxBrushModel* v)
{
    // GfxBrushModelWritable writable;
    EndianSwap(v->writable.mins);
    EndianSwap(v->writable.maxs);
    EndianSwap(v->bounds);
    EndianSwap(v->surfaceCount);
    EndianSwap(v->startSurfIndex);
}

static inline void EndianSwap_GfxSceneDynModel(IW3Xenon::GfxSceneDynModel* v)
{
    // XModelDrawInfo info;
    EndianSwap(v->info.lod);
    EndianSwap(v->info.surfId);
    EndianSwap(v->dynEntId);
}

static inline void EndianSwap_GfxSceneDynBrush(IW3Xenon::GfxSceneDynBrush* v)
{
    // BModelDrawInfo info;
    EndianSwap(v->info.surfId);
    EndianSwap(v->dynEntId);
}

static inline void EndianSwap_GfxReflectionProbe(IW3Xenon::GfxReflectionProbe* v)
{
    EndianSwap(v->origin);
    EndianSwap(v->reflectionImage);
}

static inline void EndianSwap_GfxCell(IW3Xenon::GfxCell* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->aabbTreeCount);
    EndianSwap(v->aabbTree);
    EndianSwap(v->portalCount);
    EndianSwap(v->portals);
    EndianSwap(v->cullGroupCount);
    EndianSwap(v->cullGroups);
    // reflectionProbeCount is byte - no swap
    EndianSwap(v->reflectionProbes);
}

static inline void EndianSwap_GfxAabbTree(IW3Xenon::GfxAabbTree* v)
{
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->childCount);
    EndianSwap(v->surfaceCount);
    EndianSwap(v->startSurfIndex);
    EndianSwap(v->smodelIndexCount);
    EndianSwap(v->smodelIndexes);
    EndianSwap(v->childrenOffset);
}

static inline void EndianSwap_GfxPortal(IW3Xenon::GfxPortal* v)
{
    // GfxPortalWritable writable - embedded struct
    // isQueued, isAncestor, recursionDepth, hullPointCount are bytes - no swap
    EndianSwap(v->writable.hullPoints);
    EndianSwap(v->writable.queuedParent);
    // DpvsPlane plane - embedded struct
    EndianSwap(v->plane.coeffs);
    // side[3] and pad are bytes - no swap
    EndianSwap(v->cell);
    EndianSwap(v->vertices);
    // vertexCount is byte - no swap
    EndianSwap(v->hullAxis);
}

static inline void EndianSwap_GfxLightmapArray(IW3Xenon::GfxLightmapArray* v)
{
    EndianSwap(v->primary);
    EndianSwap(v->secondary);
}

static inline void EndianSwap_MaterialMemory(IW3Xenon::MaterialMemory* v)
{
    EndianSwap(v->material);
    EndianSwap(v->memory);
}

static inline void EndianSwap_GfxShadowGeometry(IW3Xenon::GfxShadowGeometry* v)
{
    EndianSwap(v->surfaceCount);
    EndianSwap(v->smodelCount);
    EndianSwap(v->sortedSurfIndex);
    EndianSwap(v->smodelIndex);
}

static inline void EndianSwap_GfxLightRegion(IW3Xenon::GfxLightRegion* v)
{
    EndianSwap(v->hullCount);
    EndianSwap(v->hulls);
}

static inline void EndianSwap_GfxLightRegionHull(IW3Xenon::GfxLightRegionHull* v)
{
    EndianSwap(v->kdopMidPoint);
    EndianSwap(v->kdopHalfSize);
    EndianSwap(v->axisCount);
    EndianSwap(v->axis);
}

static inline void EndianSwap_GfxSurface(IW3Xenon::GfxSurface* v)
{
    // srfTriangles_t tris (embedded)
    EndianSwap(v->tris.vertexLayerData);
    EndianSwap(v->tris.firstVertex);
    EndianSwap(v->tris.vertexCount);
    EndianSwap(v->tris.triCount);
    EndianSwap(v->tris.baseIndex);
    EndianSwap(v->tris.topMipMins);
    EndianSwap(v->tris.topMipMaxs);

    EndianSwap(v->material);
    // lightmapIndex - unsigned __int8, no swap
    // reflectionProbeIndex - unsigned __int8, no swap
    // primaryLightIndex - unsigned __int8, no swap
    // castsSunShadow - bool, no swap
    EndianSwap(v->bounds);
}

static inline void EndianSwap_GfxStaticModelDrawInst(IW3Xenon::GfxStaticModelDrawInst* v)
{
    EndianSwap(v->cullDist);
    // GfxPackedPlacement placement
    EndianSwap(v->placement.origin);
    for (int i = 0; i < 3; i++)
        EndianSwap(v->placement.axis[i].packed);
    EndianSwap(v->placement.scale);
    EndianSwap(v->model);
    // unsigned __int8 reflectionProbeIndex;
    // unsigned __int8 primaryLightIndex;
    EndianSwap(v->lightingHandle);
    // unsigned __int8 flags;
}

static inline void EndianSwap_GfxWorldStreamInfo(IW3Xenon::GfxWorldStreamInfo* v)
{
    assert(false);
}

static inline void EndianSwap_GfxLight(IW3Xenon::GfxLight* v)
{
    // type, canUseShadowMap, unused[2] are bytes - no swap
    EndianSwap(v->color);
    EndianSwap(v->dir);
    EndianSwap(v->origin);
    EndianSwap(v->radius);
    EndianSwap(v->cosHalfFovOuter);
    EndianSwap(v->cosHalfFovInner);
    EndianSwap(v->exponent);
    EndianSwap(v->spotShadowIndex);
    EndianSwap(v->def);
}

static inline void EndianSwap_GfxWorldDpvsPlanes(IW3Xenon::GfxWorldDpvsPlanes* v)
{
    assert(false);
}

static inline void EndianSwap_GfxLightGrid(IW3Xenon::GfxLightGrid* v)
{
    assert(false);
}

static inline void EndianSwap_GfxWorldVertexData(IW3Xenon::GfxWorldVertexData* v)
{
    assert(false);
}

static inline void EndianSwap_GfxWorldVertexLayerData(IW3Xenon::GfxWorldVertexLayerData* v)
{
    assert(false);
}

static inline void EndianSwap_sunflare_t(IW3Xenon::sunflare_t* v)
{
    assert(false);
}

static inline void EndianSwap_GfxWorldDpvsStatic(IW3Xenon::GfxWorldDpvsStatic* v)
{
    assert(false);
}

static inline void EndianSwap_GfxWorldDpvsDynamic(IW3Xenon::GfxWorldDpvsDynamic* v)
{
    assert(false);
}

static inline void EndianSwap_GfxWorld(IW3Xenon::GfxWorld* v)
{
    EndianSwap(v->name);
    EndianSwap(v->baseName);
    EndianSwap(v->planeCount);
    EndianSwap(v->nodeCount);
    EndianSwap(v->indexCount);
    EndianSwap(v->indices);
    // D3DIndexBuffer indexBuffer - opaque 0x20 bytes, no swap needed
    EndianSwap(v->surfaceCount);
    // GfxWorldStreamInfo streamInfo - embedded struct, swap inline
    EndianSwap(v->streamInfo.aabbTreeCount);
    EndianSwap(v->streamInfo.aabbTrees);
    EndianSwap(v->streamInfo.leafRefCount);
    EndianSwap(v->streamInfo.leafRefs);
    EndianSwap(v->skySurfCount);
    EndianSwap(v->skyStartSurfs);
    EndianSwap(v->skyImage);
    // skySamplerState is byte - no swap
    EndianSwap(v->vertexCount);
    // GfxWorldVertexData vd - embedded struct
    EndianSwap(v->vd.vertices);
    // D3DVertexBuffer worldVb - opaque 0x20 bytes, no swap needed
    EndianSwap(v->vertexLayerDataSize);
    // GfxWorldVertexLayerData vld - embedded struct
    EndianSwap(v->vld.data);
    // D3DVertexBuffer layerVb - opaque 0x20 bytes, no swap needed
    // SunLightParseParams sunParse - embedded struct
    // name[64] is char array - no swap
    EndianSwap(v->sunParse.ambientScale);
    EndianSwap(v->sunParse.ambientColor);
    EndianSwap(v->sunParse.diffuseFraction);
    EndianSwap(v->sunParse.sunLight);
    EndianSwap(v->sunParse.sunColor);
    EndianSwap(v->sunParse.diffuseColor);
    // diffuseColorHasBeenSet is bool - no swap
    EndianSwap(v->sunParse.angles);
    EndianSwap(v->sunLight);
    EndianSwap(v->sunColorFromBsp);
    EndianSwap(v->sunPrimaryLightIndex);
    EndianSwap(v->primaryLightCount);
    EndianSwap(v->cullGroupCount);
    EndianSwap(v->reflectionProbeCount);
    EndianSwap(v->reflectionProbes);
    EndianSwap(v->reflectionProbeTextures);
    // GfxWorldDpvsPlanes dpvsPlanes - embedded struct
    EndianSwap(v->dpvsPlanes.cellCount);
    EndianSwap(v->dpvsPlanes.planes);
    EndianSwap(v->dpvsPlanes.nodes);
    EndianSwap(v->dpvsPlanes.sceneEntCellBits);
    EndianSwap(v->cellBitsCount);
    EndianSwap(v->cells);
    EndianSwap(v->lightmapCount);
    EndianSwap(v->lightmaps);
    // GfxLightGrid lightGrid - embedded struct
    // hasLightRegions is bool - no swap
    EndianSwap(v->lightGrid.sunPrimaryLightIndex);
    EndianSwap(v->lightGrid.mins);
    EndianSwap(v->lightGrid.maxs);
    EndianSwap(v->lightGrid.rowAxis);
    EndianSwap(v->lightGrid.colAxis);
    EndianSwap(v->lightGrid.rowDataStart);
    EndianSwap(v->lightGrid.rawRowDataSize);
    EndianSwap(v->lightGrid.rawRowData);
    EndianSwap(v->lightGrid.entryCount);
    EndianSwap(v->lightGrid.entries);
    EndianSwap(v->lightGrid.colorCount);
    EndianSwap(v->lightGrid.colors);
    EndianSwap(v->lightmapPrimaryTextures);
    EndianSwap(v->lightmapSecondaryTextures);
    EndianSwap(v->modelCount);
    EndianSwap(v->models);
    EndianSwap(v->mins);
    EndianSwap(v->maxs);
    EndianSwap(v->checksum);
    EndianSwap(v->materialMemoryCount);
    EndianSwap(v->materialMemory);
    // sunflare_t sun - embedded struct
    // hasValidData is bool - no swap
    EndianSwap(v->sun.spriteMaterial);
    EndianSwap(v->sun.flareMaterial);
    EndianSwap(v->sun.spriteSize);
    EndianSwap(v->sun.flareMinSize);
    EndianSwap(v->sun.flareMinDot);
    EndianSwap(v->sun.flareMaxSize);
    EndianSwap(v->sun.flareMaxDot);
    EndianSwap(v->sun.flareMaxAlpha);
    EndianSwap(v->sun.flareFadeInTime);
    EndianSwap(v->sun.flareFadeOutTime);
    EndianSwap(v->sun.blindMinDot);
    EndianSwap(v->sun.blindMaxDot);
    EndianSwap(v->sun.blindMaxDarken);
    EndianSwap(v->sun.blindFadeInTime);
    EndianSwap(v->sun.blindFadeOutTime);
    EndianSwap(v->sun.glareMinDot);
    EndianSwap(v->sun.glareMaxDot);
    EndianSwap(v->sun.glareMaxLighten);
    EndianSwap(v->sun.glareFadeInTime);
    EndianSwap(v->sun.glareFadeOutTime);
    EndianSwap(v->sun.sunFxPosition);
    // outdoorLookupMatrix[4][4]
    EndianSwap(v->outdoorLookupMatrix);
    EndianSwap(v->outdoorImage);
    EndianSwap(v->cellCasterBits);
    EndianSwap(v->sceneDynModel);
    EndianSwap(v->sceneDynBrush);
    EndianSwap(v->primaryLightEntityShadowVis);
    EndianSwap(v->primaryLightDynEntShadowVis);
    EndianSwap(v->nonSunPrimaryLightForModelDynEnt);
    EndianSwap(v->shadowGeom);
    EndianSwap(v->lightRegion);
    // GfxWorldDpvsStatic dpvs - embedded struct
    EndianSwap(v->dpvs.smodelCount);
    EndianSwap(v->dpvs.staticSurfaceCount);
    EndianSwap(v->dpvs.litSurfsBegin);
    EndianSwap(v->dpvs.litSurfsEnd);
    EndianSwap(v->dpvs.decalSurfsBegin);
    EndianSwap(v->dpvs.decalSurfsEnd);
    EndianSwap(v->dpvs.emissiveSurfsBegin);
    EndianSwap(v->dpvs.emissiveSurfsEnd);
    EndianSwap(v->dpvs.smodelVisDataCount);
    EndianSwap(v->dpvs.surfaceVisDataCount);
    EndianSwap(v->dpvs.smodelVisData);
    EndianSwap(v->dpvs.surfaceVisData);
    EndianSwap(v->dpvs.lodData);
    EndianSwap(v->dpvs.sortedSurfIndex);
    EndianSwap(v->dpvs.smodelInsts);
    EndianSwap(v->dpvs.surfaces);
    EndianSwap(v->dpvs.cullGroups);
    EndianSwap(v->dpvs.smodelDrawInsts);
    EndianSwap(v->dpvs.surfaceMaterials);
    EndianSwap(v->dpvs.surfaceCastsSunShadow);
    EndianSwap(v->dpvs.usageCount);
    // GfxWorldDpvsDynamic dpvsDyn - embedded struct
    EndianSwap(v->dpvsDyn.dynEntClientWordCount);
    EndianSwap(v->dpvsDyn.dynEntClientCount);
    EndianSwap(v->dpvsDyn.dynEntCellBits);
    EndianSwap(v->dpvsDyn.dynEntVisData);
}

// ---- GfxLightDef
static inline void EndianSwap_GfxLightImage(IW3Xenon::GfxLightImage* v)
{
    assert(false);
}

static inline void EndianSwap_GfxLightDef(IW3Xenon::GfxLightDef* v)
{
    EndianSwap(v->name);
    // GfxLightImage attenuation
    EndianSwap(v->attenuation.image);
    // samplerState is a byte - no swap
    EndianSwap(v->lmapLookupStart);
}

// ---- Font_s

static inline void EndianSwap_Glyph(IW3Xenon::Glyph* v)
{
    EndianSwap(v->letter);
    // char x0;
    // char y0;
    // unsigned __int8 dx;
    // unsigned __int8 pixelWidth;
    // unsigned __int8 pixelHeight;
    EndianSwap(v->s0);
    EndianSwap(v->t0);
    EndianSwap(v->s1);
    EndianSwap(v->t1);
}

static inline void EndianSwap_Font_s(IW3Xenon::Font_s* v)
{
    EndianSwap(v->fontName);
    EndianSwap(v->pixelHeight);
    EndianSwap(v->glyphCount);
    EndianSwap(v->material);
    EndianSwap(v->glowMaterial);
    EndianSwap(v->glyphs);
}

// ---- MenuList

static inline void EndianSwap_MenuList(IW3Xenon::MenuList* v)
{
    EndianSwap(v->name);
    EndianSwap(v->menuCount);
    EndianSwap(v->menus);
}

// ---- menuDef_t

static inline void EndianSwap_editFieldDef_s(IW3Xenon::editFieldDef_s* v)
{
    EndianSwap(v->minVal);
    EndianSwap(v->maxVal);
    EndianSwap(v->defVal);
    EndianSwap(v->range);
    EndianSwap(v->maxChars);
    EndianSwap(v->maxCharsGotoNext);
    EndianSwap(v->maxPaintChars);
    EndianSwap(v->paintOffset);
}

static inline void EndianSwap_windowDef_t(IW3Xenon::windowDef_t* v)
{
    EndianSwap(v->name);
    EndianSwap(v->rect.x);
    EndianSwap(v->rect.y);
    EndianSwap(v->rect.w);
    EndianSwap(v->rect.h);
    EndianSwap(v->rect.horzAlign);
    EndianSwap(v->rect.vertAlign);
    EndianSwap(v->rectClient.x);
    EndianSwap(v->rectClient.y);
    EndianSwap(v->rectClient.w);
    EndianSwap(v->rectClient.h);
    EndianSwap(v->rectClient.horzAlign);
    EndianSwap(v->rectClient.vertAlign);
    EndianSwap(v->group);
    EndianSwap(v->style);
    EndianSwap(v->border);
    EndianSwap(v->ownerDraw);
    EndianSwap(v->ownerDrawFlags);
    EndianSwap(v->borderSize);
    EndianSwap(v->staticFlags);
    EndianSwap(v->dynamicFlags);
    EndianSwap(v->nextTime);
    EndianSwap(v->foreColor);
    EndianSwap(v->backColor);
    EndianSwap(v->borderColor);
    EndianSwap(v->outlineColor);
    EndianSwap(v->background);
}

static inline void EndianSwap_ItemKeyHandler(IW3Xenon::ItemKeyHandler* v)
{
    EndianSwap(v->key);
    EndianSwap(v->action);
    EndianSwap(v->next);
}

static inline void EndianSwap_statement_s(IW3Xenon::statement_s* v)
{
    assert(false);
}

static inline void EndianSwap_operandInternalDataUnion(IW3Xenon::operandInternalDataUnion* v)
{
    assert(false);
}

static inline void EndianSwap_Operand(IW3Xenon::Operand* v)
{
    EndianSwap(v->dataType);

    // Swap union based on dataType
    if (v->dataType == IW3Xenon::VAL_INT)
    {
        EndianSwap(v->internals.intVal);
    }
    else if (v->dataType == IW3Xenon::VAL_FLOAT)
    {
        EndianSwap(v->internals.floatVal);
    }
    else if (v->dataType == IW3Xenon::VAL_STRING)
    {
        EndianSwap(v->internals.string);
    }
    else
    {
        assert(false);
    }
}

static inline void EndianSwap_entryInternalData(IW3Xenon::entryInternalData* v)
{
    // Union - contents swapped based on context in expressionEntry
}

static inline void EndianSwap_expressionEntry(IW3Xenon::expressionEntry* v)
{
    EndianSwap(v->type);

    if (v->type == IW3Xenon::EET_OPERATOR)
    {
        EndianSwap(v->data.op);
    }
    else if (v->type == IW3Xenon::EET_OPERAND)
    {
        EndianSwap_Operand(&v->data.operand);
    }
    else
    {
        assert(false);
    }
}

static inline void EndianSwap_itemDef_s(IW3Xenon::itemDef_s* v)
{
    // windowDef_t window
    EndianSwap_windowDef_t(&v->window);

    // rectDef_s textRect[4]
    for (int i = 0; i < 4; i++)
    {
        EndianSwap(v->textRect[i].x);
        EndianSwap(v->textRect[i].y);
        EndianSwap(v->textRect[i].w);
        EndianSwap(v->textRect[i].h);
        EndianSwap(v->textRect[i].horzAlign);
        EndianSwap(v->textRect[i].vertAlign);
    }

    // itemDef_s members
    EndianSwap(v->type);
    EndianSwap(v->dataType);
    EndianSwap(v->alignment);
    EndianSwap(v->fontEnum);
    EndianSwap(v->textAlignMode);
    EndianSwap(v->textalignx);
    EndianSwap(v->textaligny);
    EndianSwap(v->textscale);
    EndianSwap(v->textStyle);
    EndianSwap(v->gameMsgWindowIndex);
    EndianSwap(v->gameMsgWindowMode);
    EndianSwap(v->text);
    EndianSwap(v->itemFlags);
    EndianSwap(v->parent);
    EndianSwap(v->mouseEnterText);
    EndianSwap(v->mouseExitText);
    EndianSwap(v->mouseEnter);
    EndianSwap(v->mouseExit);
    EndianSwap(v->action);
    EndianSwap(v->onAccept);
    EndianSwap(v->onFocus);
    EndianSwap(v->leaveFocus);
    EndianSwap(v->dvar);
    EndianSwap(v->dvarTest);
    EndianSwap(v->onKey);
    EndianSwap(v->enableDvar);
    EndianSwap(v->dvarFlags);
    EndianSwap(v->focusSound);
    EndianSwap(v->special);
    EndianSwap(v->cursorPos);

    // itemDefData_t typeData (union with pointer)
    EndianSwap(v->typeData.data);

    EndianSwap(v->imageTrack);

    // statement_s visibleExp
    EndianSwap(v->visibleExp.numEntries);
    EndianSwap(v->visibleExp.entries);

    // statement_s textExp
    EndianSwap(v->textExp.numEntries);
    EndianSwap(v->textExp.entries);

    // statement_s materialExp
    EndianSwap(v->materialExp.numEntries);
    EndianSwap(v->materialExp.entries);

    // statement_s rectXExp
    EndianSwap(v->rectXExp.numEntries);
    EndianSwap(v->rectXExp.entries);

    // statement_s rectYExp
    EndianSwap(v->rectYExp.numEntries);
    EndianSwap(v->rectYExp.entries);

    // statement_s rectWExp
    EndianSwap(v->rectWExp.numEntries);
    EndianSwap(v->rectWExp.entries);

    // statement_s rectHExp
    EndianSwap(v->rectHExp.numEntries);
    EndianSwap(v->rectHExp.entries);

    // statement_s forecolorAExp
    EndianSwap(v->forecolorAExp.numEntries);
    EndianSwap(v->forecolorAExp.entries);
}

static inline void EndianSwap_itemDefData_t(IW3Xenon::itemDefData_t* v)
{
    assert(false);
}

static inline void EndianSwap_listBoxDef_s(IW3Xenon::listBoxDef_s* v)
{
    // int startPos[4];
    EndianSwap(v->startPos);

    // int endPos[4];
    EndianSwap(v->endPos);

    EndianSwap(v->drawPadding);
    EndianSwap(v->elementWidth);
    EndianSwap(v->elementHeight);
    EndianSwap(v->elementStyle);
    EndianSwap(v->numColumns);

    for (int i = 0; i < 16; i++)
    {
        EndianSwap(v->columnInfo[i].pos);
        EndianSwap(v->columnInfo[i].width);
        EndianSwap(v->columnInfo[i].maxChars);
        EndianSwap(v->columnInfo[i].alignment);
    }

    EndianSwap(v->doubleClick);
    EndianSwap(v->notselectable);
    EndianSwap(v->noScrollBars);
    EndianSwap(v->usePaging);

    // int selectBorder[4];
    EndianSwap(v->selectBorder);

    // int disableColor[4];
    EndianSwap(v->disableColor);

    EndianSwap(v->selectIcon);
}

static inline void EndianSwap_multiDef_s(IW3Xenon::multiDef_s* v)
{
    EndianSwap(v->dvarList);
    EndianSwap(v->dvarStr);
    EndianSwap(v->dvarValue);
    EndianSwap(v->count);
    EndianSwap(v->strDef);
}

static inline void EndianSwap_menuDef_t(IW3Xenon::menuDef_t* v)
{
    // windowDef_t window
    EndianSwap_windowDef_t(&v->window);

    // menuDef_t members
    EndianSwap(v->font);
    EndianSwap(v->fullScreen);
    EndianSwap(v->itemCount);
    EndianSwap(v->fontIndex);
    EndianSwap(v->cursorItem);
    EndianSwap(v->fadeCycle);
    EndianSwap(v->fadeClamp);
    EndianSwap(v->fadeAmount);
    EndianSwap(v->fadeInAmount);
    EndianSwap(v->blurRadius);
    EndianSwap(v->onOpen);
    EndianSwap(v->onClose);
    EndianSwap(v->onESC);
    EndianSwap(v->onKey);
    EndianSwap(v->visibleExp.numEntries);
    EndianSwap(v->visibleExp.entries);
    EndianSwap(v->allowedBinding);
    EndianSwap(v->soundName);
    EndianSwap(v->imageTrack);
    EndianSwap(v->focusColor);
    EndianSwap(v->disableColor);
    EndianSwap(v->rectXExp.numEntries);
    EndianSwap(v->rectXExp.entries);
    EndianSwap(v->rectYExp.numEntries);
    EndianSwap(v->rectYExp.entries);
    EndianSwap(v->items);
}

// ---- LocalizeEntry

static inline void EndianSwap_LocalizeEntry(IW3Xenon::LocalizeEntry* v)
{
    EndianSwap(v->name);
    EndianSwap(v->value);
}

// ---- WeaponDef

static inline void EndianSwap_snd_alias_list_name(IW3Xenon::snd_alias_list_name* v)
{
    EndianSwap(v->soundName);
}

static inline void EndianSwap_SndAliasCustom(IW3Xenon::SndAliasCustom* v)
{
    // union of pointers so swap any
    EndianSwap(v->name);
}

static inline void EndianSwap_WeaponDef(IW3Xenon::WeaponDef* v)
{
    EndianSwap(v->szInternalName);
    EndianSwap(v->szDisplayName);
    EndianSwap(v->szOverlayName);
    EndianSwap(v->gunXModel);
    EndianSwap(v->handXModel);
    EndianSwap(v->szXAnims);
    EndianSwap(v->szModeName);
    EndianSwap(v->hideTags);
    EndianSwap(v->notetrackSoundMapKeys);
    EndianSwap(v->notetrackSoundMapValues);
    EndianSwap(v->playerAnimType);
    EndianSwap(v->weapType);
    EndianSwap(v->weapClass);
    EndianSwap(v->penetrateType);
    EndianSwap(v->impactType);
    EndianSwap(v->inventoryType);
    EndianSwap(v->fireType);
    EndianSwap(v->offhandClass);
    EndianSwap(v->stance);
    EndianSwap(v->viewFlashEffect);
    EndianSwap(v->worldFlashEffect);
    EndianSwap(v->pickupSound.name);
    EndianSwap(v->pickupSoundPlayer.name);
    EndianSwap(v->ammoPickupSound.name);
    EndianSwap(v->ammoPickupSoundPlayer.name);
    EndianSwap(v->projectileSound.name);
    EndianSwap(v->pullbackSound.name);
    EndianSwap(v->pullbackSoundPlayer.name);
    EndianSwap(v->fireSound.name);
    EndianSwap(v->fireSoundPlayer.name);
    EndianSwap(v->fireLoopSound.name);
    EndianSwap(v->fireLoopSoundPlayer.name);
    EndianSwap(v->fireStopSound.name);
    EndianSwap(v->fireStopSoundPlayer.name);
    EndianSwap(v->fireLastSound.name);
    EndianSwap(v->fireLastSoundPlayer.name);
    EndianSwap(v->emptyFireSound.name);
    EndianSwap(v->emptyFireSoundPlayer.name);
    EndianSwap(v->meleeSwipeSound.name);
    EndianSwap(v->meleeSwipeSoundPlayer.name);
    EndianSwap(v->meleeHitSound.name);
    EndianSwap(v->meleeMissSound.name);
    EndianSwap(v->rechamberSound.name);
    EndianSwap(v->rechamberSoundPlayer.name);
    EndianSwap(v->reloadSound.name);
    EndianSwap(v->reloadSoundPlayer.name);
    EndianSwap(v->reloadEmptySound.name);
    EndianSwap(v->reloadEmptySoundPlayer.name);
    EndianSwap(v->reloadStartSound.name);
    EndianSwap(v->reloadStartSoundPlayer.name);
    EndianSwap(v->reloadEndSound.name);
    EndianSwap(v->reloadEndSoundPlayer.name);
    EndianSwap(v->detonateSound.name);
    EndianSwap(v->detonateSoundPlayer.name);
    EndianSwap(v->nightVisionWearSound.name);
    EndianSwap(v->nightVisionWearSoundPlayer.name);
    EndianSwap(v->nightVisionRemoveSound.name);
    EndianSwap(v->nightVisionRemoveSoundPlayer.name);
    EndianSwap(v->altSwitchSound.name);
    EndianSwap(v->altSwitchSoundPlayer.name);
    EndianSwap(v->raiseSound.name);
    EndianSwap(v->raiseSoundPlayer.name);
    EndianSwap(v->firstRaiseSound.name);
    EndianSwap(v->firstRaiseSoundPlayer.name);
    EndianSwap(v->putawaySound.name);
    EndianSwap(v->putawaySoundPlayer.name);
    EndianSwap(v->bounceSound);
    EndianSwap(v->viewShellEjectEffect);
    EndianSwap(v->worldShellEjectEffect);
    EndianSwap(v->viewLastShotEjectEffect);
    EndianSwap(v->worldLastShotEjectEffect);
    EndianSwap(v->reticleCenter);
    EndianSwap(v->reticleSide);
    EndianSwap(v->iReticleCenterSize);
    EndianSwap(v->iReticleSideSize);
    EndianSwap(v->iReticleMinOfs);
    EndianSwap(v->activeReticleType);
    EndianSwap(v->vStandMove);
    EndianSwap(v->vStandRot);
    EndianSwap(v->vDuckedOfs);
    EndianSwap(v->vDuckedMove);
    EndianSwap(v->vDuckedRot);
    EndianSwap(v->vProneOfs);
    EndianSwap(v->vProneMove);
    EndianSwap(v->vProneRot);
    EndianSwap(v->fPosMoveRate);
    EndianSwap(v->fPosProneMoveRate);
    EndianSwap(v->fStandMoveMinSpeed);
    EndianSwap(v->fDuckedMoveMinSpeed);
    EndianSwap(v->fProneMoveMinSpeed);
    EndianSwap(v->fPosRotRate);
    EndianSwap(v->fPosProneRotRate);
    EndianSwap(v->fStandRotMinSpeed);
    EndianSwap(v->fDuckedRotMinSpeed);
    EndianSwap(v->fProneRotMinSpeed);
    EndianSwap(v->worldModel);
    EndianSwap(v->worldClipModel);
    EndianSwap(v->rocketModel);
    EndianSwap(v->knifeModel);
    EndianSwap(v->worldKnifeModel);
    EndianSwap(v->hudIcon);
    EndianSwap(v->hudIconRatio);
    EndianSwap(v->ammoCounterIcon);
    EndianSwap(v->ammoCounterIconRatio);
    EndianSwap(v->ammoCounterClip);
    EndianSwap(v->iStartAmmo);
    EndianSwap(v->szAmmoName);
    EndianSwap(v->iAmmoIndex);
    EndianSwap(v->szClipName);
    EndianSwap(v->iClipIndex);
    EndianSwap(v->iMaxAmmo);
    EndianSwap(v->iClipSize);
    EndianSwap(v->shotCount);
    EndianSwap(v->szSharedAmmoCapName);
    EndianSwap(v->iSharedAmmoCapIndex);
    EndianSwap(v->iSharedAmmoCap);
    EndianSwap(v->damage);
    EndianSwap(v->playerDamage);
    EndianSwap(v->iMeleeDamage);
    EndianSwap(v->iDamageType);
    EndianSwap(v->iFireDelay);
    EndianSwap(v->iMeleeDelay);
    EndianSwap(v->meleeChargeDelay);
    EndianSwap(v->iDetonateDelay);
    EndianSwap(v->iFireTime);
    EndianSwap(v->iRechamberTime);
    EndianSwap(v->iRechamberBoltTime);
    EndianSwap(v->iHoldFireTime);
    EndianSwap(v->iDetonateTime);
    EndianSwap(v->iMeleeTime);
    EndianSwap(v->meleeChargeTime);
    EndianSwap(v->iReloadTime);
    EndianSwap(v->reloadShowRocketTime);
    EndianSwap(v->iReloadEmptyTime);
    EndianSwap(v->iReloadAddTime);
    EndianSwap(v->iReloadStartTime);
    EndianSwap(v->iReloadStartAddTime);
    EndianSwap(v->iReloadEndTime);
    EndianSwap(v->iDropTime);
    EndianSwap(v->iRaiseTime);
    EndianSwap(v->iAltDropTime);
    EndianSwap(v->iAltRaiseTime);
    EndianSwap(v->quickDropTime);
    EndianSwap(v->quickRaiseTime);
    EndianSwap(v->iFirstRaiseTime);
    EndianSwap(v->iEmptyRaiseTime);
    EndianSwap(v->iEmptyDropTime);
    EndianSwap(v->sprintInTime);
    EndianSwap(v->sprintLoopTime);
    EndianSwap(v->sprintOutTime);
    EndianSwap(v->nightVisionWearTime);
    EndianSwap(v->nightVisionWearTimeFadeOutEnd);
    EndianSwap(v->nightVisionWearTimePowerUp);
    EndianSwap(v->nightVisionRemoveTime);
    EndianSwap(v->nightVisionRemoveTimePowerDown);
    EndianSwap(v->nightVisionRemoveTimeFadeInStart);
    EndianSwap(v->fuseTime);
    EndianSwap(v->aiFuseTime);
    EndianSwap(v->requireLockonToFire);
    EndianSwap(v->noAdsWhenMagEmpty);
    EndianSwap(v->avoidDropCleanup);
    EndianSwap(v->autoAimRange);
    EndianSwap(v->aimAssistRange);
    EndianSwap(v->aimAssistRangeAds);
    EndianSwap(v->aimPadding);
    EndianSwap(v->enemyCrosshairRange);
    EndianSwap(v->crosshairColorChange);
    EndianSwap(v->moveSpeedScale);
    EndianSwap(v->adsMoveSpeedScale);
    EndianSwap(v->sprintDurationScale);
    EndianSwap(v->fAdsZoomFov);
    EndianSwap(v->fAdsZoomInFrac);
    EndianSwap(v->fAdsZoomOutFrac);
    EndianSwap(v->overlayMaterial);
    EndianSwap(v->overlayMaterialLowRes);
    EndianSwap(v->overlayReticle);
    EndianSwap(v->overlayInterface);
    EndianSwap(v->overlayWidth);
    EndianSwap(v->overlayHeight);
    EndianSwap(v->fAdsBobFactor);
    EndianSwap(v->fAdsViewBobMult);
    EndianSwap(v->fHipSpreadStandMin);
    EndianSwap(v->fHipSpreadDuckedMin);
    EndianSwap(v->fHipSpreadProneMin);
    EndianSwap(v->hipSpreadStandMax);
    EndianSwap(v->hipSpreadDuckedMax);
    EndianSwap(v->hipSpreadProneMax);
    EndianSwap(v->fHipSpreadDecayRate);
    EndianSwap(v->fHipSpreadFireAdd);
    EndianSwap(v->fHipSpreadTurnAdd);
    EndianSwap(v->fHipSpreadMoveAdd);
    EndianSwap(v->fHipSpreadDuckedDecay);
    EndianSwap(v->fHipSpreadProneDecay);
    EndianSwap(v->fHipReticleSidePos);
    EndianSwap(v->iAdsTransInTime);
    EndianSwap(v->iAdsTransOutTime);
    EndianSwap(v->fAdsIdleAmount);
    EndianSwap(v->fHipIdleAmount);
    EndianSwap(v->adsIdleSpeed);
    EndianSwap(v->hipIdleSpeed);
    EndianSwap(v->fIdleCrouchFactor);
    EndianSwap(v->fIdleProneFactor);
    EndianSwap(v->fGunMaxPitch);
    EndianSwap(v->fGunMaxYaw);
    EndianSwap(v->swayMaxAngle);
    EndianSwap(v->swayLerpSpeed);
    EndianSwap(v->swayPitchScale);
    EndianSwap(v->swayYawScale);
    EndianSwap(v->swayHorizScale);
    EndianSwap(v->swayVertScale);
    EndianSwap(v->swayShellShockScale);
    EndianSwap(v->adsSwayMaxAngle);
    EndianSwap(v->adsSwayLerpSpeed);
    EndianSwap(v->adsSwayPitchScale);
    EndianSwap(v->adsSwayYawScale);
    EndianSwap(v->adsSwayHorizScale);
    EndianSwap(v->adsSwayVertScale);
    EndianSwap(v->bRifleBullet);
    EndianSwap(v->armorPiercing);
    EndianSwap(v->bBoltAction);
    EndianSwap(v->aimDownSight);
    EndianSwap(v->bRechamberWhileAds);
    EndianSwap(v->adsViewErrorMin);
    EndianSwap(v->adsViewErrorMax);
    EndianSwap(v->bCookOffHold);
    EndianSwap(v->bClipOnly);
    EndianSwap(v->adsFireOnly);
    EndianSwap(v->cancelAutoHolsterWhenEmpty);
    EndianSwap(v->suppressAmmoReserveDisplay);
    EndianSwap(v->enhanced);
    EndianSwap(v->laserSightDuringNightvision);
    EndianSwap(v->killIcon);
    EndianSwap(v->killIconRatio);
    EndianSwap(v->flipKillIcon);
    EndianSwap(v->dpadIcon);
    EndianSwap(v->dpadIconRatio);
    EndianSwap(v->bNoPartialReload);
    EndianSwap(v->bSegmentedReload);
    EndianSwap(v->iReloadAmmoAdd);
    EndianSwap(v->iReloadStartAdd);
    EndianSwap(v->szAltWeaponName);
    EndianSwap(v->altWeaponIndex);
    EndianSwap(v->iDropAmmoMin);
    EndianSwap(v->iDropAmmoMax);
    EndianSwap(v->blocksProne);
    EndianSwap(v->silenced);
    EndianSwap(v->iExplosionRadius);
    EndianSwap(v->iExplosionRadiusMin);
    EndianSwap(v->iExplosionInnerDamage);
    EndianSwap(v->iExplosionOuterDamage);
    EndianSwap(v->damageConeAngle);
    EndianSwap(v->iProjectileSpeed);
    EndianSwap(v->iProjectileSpeedUp);
    EndianSwap(v->iProjectileSpeedForward);
    EndianSwap(v->iProjectileActivateDist);
    EndianSwap(v->projLifetime);
    EndianSwap(v->timeToAccelerate);
    EndianSwap(v->projectileCurvature);
    EndianSwap(v->projectileModel);
    EndianSwap(v->projExplosion);
    EndianSwap(v->projExplosionEffect);
    EndianSwap(v->projExplosionEffectForceNormalUp);
    EndianSwap(v->projDudEffect);
    EndianSwap(v->projExplosionSound.name);
    EndianSwap(v->projDudSound.name);
    EndianSwap(v->bProjImpactExplode);
    EndianSwap(v->stickiness);
    EndianSwap(v->hasDetonator);
    EndianSwap(v->timedDetonation);
    EndianSwap(v->rotate);
    EndianSwap(v->holdButtonToThrow);
    EndianSwap(v->freezeMovementWhenFiring);
    EndianSwap(v->lowAmmoWarningThreshold);
    EndianSwap(v->parallelBounce);
    EndianSwap(v->perpendicularBounce);
    EndianSwap(v->projTrailEffect);
    EndianSwap(v->vProjectileColor);
    EndianSwap(v->guidedMissileType);
    EndianSwap(v->maxSteeringAccel);
    EndianSwap(v->projIgnitionDelay);
    EndianSwap(v->projIgnitionEffect);
    EndianSwap(v->projIgnitionSound.name);
    EndianSwap(v->fAdsAimPitch);
    EndianSwap(v->fAdsCrosshairInFrac);
    EndianSwap(v->fAdsCrosshairOutFrac);
    EndianSwap(v->adsGunKickReducedKickBullets);
    EndianSwap(v->adsGunKickReducedKickPercent);
    EndianSwap(v->fAdsGunKickPitchMin);
    EndianSwap(v->fAdsGunKickPitchMax);
    EndianSwap(v->fAdsGunKickYawMin);
    EndianSwap(v->fAdsGunKickYawMax);
    EndianSwap(v->fAdsGunKickAccel);
    EndianSwap(v->fAdsGunKickSpeedMax);
    EndianSwap(v->fAdsGunKickSpeedDecay);
    EndianSwap(v->fAdsGunKickStaticDecay);
    EndianSwap(v->fAdsViewKickPitchMin);
    EndianSwap(v->fAdsViewKickPitchMax);
    EndianSwap(v->fAdsViewKickYawMin);
    EndianSwap(v->fAdsViewKickYawMax);
    EndianSwap(v->fAdsViewKickCenterSpeed);
    EndianSwap(v->fAdsViewScatterMin);
    EndianSwap(v->fAdsViewScatterMax);
    EndianSwap(v->fAdsSpread);
    EndianSwap(v->hipGunKickReducedKickBullets);
    EndianSwap(v->hipGunKickReducedKickPercent);
    EndianSwap(v->fHipGunKickPitchMin);
    EndianSwap(v->fHipGunKickPitchMax);
    EndianSwap(v->fHipGunKickYawMin);
    EndianSwap(v->fHipGunKickYawMax);
    EndianSwap(v->fHipGunKickAccel);
    EndianSwap(v->fHipGunKickSpeedMax);
    EndianSwap(v->fHipGunKickSpeedDecay);
    EndianSwap(v->fHipGunKickStaticDecay);
    EndianSwap(v->fHipViewKickPitchMin);
    EndianSwap(v->fHipViewKickPitchMax);
    EndianSwap(v->fHipViewKickYawMin);
    EndianSwap(v->fHipViewKickYawMax);
    EndianSwap(v->fHipViewKickCenterSpeed);
    EndianSwap(v->fHipViewScatterMin);
    EndianSwap(v->fHipViewScatterMax);
    EndianSwap(v->fightDist);
    EndianSwap(v->maxDist);
    EndianSwap(v->aiVsAiAccuracyGraphName);
    EndianSwap(v->aiVsPlayerAccuracyGraphName);
    EndianSwap(v->aiVsAiAccuracyGraphKnots);
    EndianSwap(v->aiVsPlayerAccuracyGraphKnots);
    EndianSwap(v->originalAiVsAiAccuracyGraphKnots);
    EndianSwap(v->originalAiVsPlayerAccuracyGraphKnots);
    EndianSwap(v->aiVsAiAccuracyGraphKnotCount);
    EndianSwap(v->aiVsPlayerAccuracyGraphKnotCount);
    EndianSwap(v->originalAiVsAiAccuracyGraphKnotCount);
    EndianSwap(v->originalAiVsPlayerAccuracyGraphKnotCount);
    EndianSwap(v->iPositionReloadTransTime);
    EndianSwap(v->leftArc);
    EndianSwap(v->rightArc);
    EndianSwap(v->topArc);
    EndianSwap(v->bottomArc);
    EndianSwap(v->accuracy);
    EndianSwap(v->aiSpread);
    EndianSwap(v->playerSpread);
    EndianSwap(v->minTurnSpeed);
    EndianSwap(v->maxTurnSpeed);
    EndianSwap(v->pitchConvergenceTime);
    EndianSwap(v->yawConvergenceTime);
    EndianSwap(v->suppressTime);
    EndianSwap(v->maxRange);
    EndianSwap(v->fAnimHorRotateInc);
    EndianSwap(v->fPlayerPositionDist);
    EndianSwap(v->szUseHintString);
    EndianSwap(v->dropHintString);
    EndianSwap(v->iUseHintStringIndex);
    EndianSwap(v->dropHintStringIndex);
    EndianSwap(v->horizViewJitter);
    EndianSwap(v->vertViewJitter);
    EndianSwap(v->szScript);
    EndianSwap(v->fOOPosAnimLength);
    EndianSwap(v->minDamage);
    EndianSwap(v->minPlayerDamage);
    EndianSwap(v->fMaxDamageRange);
    EndianSwap(v->fMinDamageRange);
    EndianSwap(v->destabilizationRateTime);
    EndianSwap(v->destabilizationCurvatureMax);
    EndianSwap(v->destabilizeDistance);
    EndianSwap(v->locationDamageMultipliers);
    EndianSwap(v->fireRumble);
    EndianSwap(v->meleeImpactRumble);
    EndianSwap(v->adsDofStart);
    EndianSwap(v->adsDofEnd);
}

// ---- SndDriverGlobals

static inline void EndianSwap_XaReverbSettings(IW3Xenon::XaReverbSettings* v)
{
    EndianSwap(v->presetOverridden);
    // XAUDIOREVERBSETTINGS reverbSettings;
    EndianSwap(v->reverbSettings.ReflectionsDelay);
    // unsigned __int8 ReverbDelay;
    // unsigned __int8 RearDelay;
    // unsigned __int8 PositionLeft;
    // unsigned __int8 PositionRight;
    // unsigned __int8 PositionMatrixLeft;
    // unsigned __int8 PositionMatrixRight;
    // unsigned __int8 EarlyDiffusion;
    // unsigned __int8 LateDiffusion;
    // unsigned __int8 LowEQGain;
    // unsigned __int8 LowEQCutoff;
    // unsigned __int8 HighEQGain;
    // unsigned __int8 HighEQCutoff;
    EndianSwap(v->reverbSettings.RoomFilterFreq);
    EndianSwap(v->reverbSettings.RoomFilterMain);
    EndianSwap(v->reverbSettings.RoomFilterHF);
    EndianSwap(v->reverbSettings.ReflectionsGain);
    EndianSwap(v->reverbSettings.ReverbGain);
    EndianSwap(v->reverbSettings.DecayTime);
    EndianSwap(v->reverbSettings.Density);
    EndianSwap(v->reverbSettings.RoomSize);
}

static inline void EndianSwap_SndDriverGlobals(IW3Xenon::SndDriverGlobals* v)
{
    EndianSwap(v->reverbSettings);
    EndianSwap(v->name);
}

// ---- FXEffectDef

static inline void EndianSwap_FxElemVelStateSample(IW3Xenon::FxElemVelStateSample* v)
{
    EndianSwap(v->local.velocity.base);
    EndianSwap(v->local.velocity.amplitude);
    EndianSwap(v->local.totalDelta.base);
    EndianSwap(v->local.totalDelta.amplitude);
    EndianSwap(v->world.velocity.base);
    EndianSwap(v->world.velocity.amplitude);
    EndianSwap(v->world.totalDelta.base);
    EndianSwap(v->world.totalDelta.amplitude);
}

static inline void EndianSwap_FxElemVisStateSample(IW3Xenon::FxElemVisStateSample* v)
{
    EndianSwap(v->base.rotationDelta);
    EndianSwap(v->base.rotationTotal);
    EndianSwap(v->base.size);
    EndianSwap(v->base.scale);
    EndianSwap(v->amplitude.rotationDelta);
    EndianSwap(v->amplitude.rotationTotal);
    EndianSwap(v->amplitude.size);
    EndianSwap(v->amplitude.scale);
}

static inline void EndianSwap_FxTrailVertex(IW3Xenon::FxTrailVertex* v)
{
    EndianSwap(v->pos);
    EndianSwap(v->normal);
    EndianSwap(v->texCoord);
}

static inline void EndianSwap_FxElemDef(IW3Xenon::FxElemDef* v)
{
    EndianSwap(v->flags);

    // FxSpawnDef spawn (union - both members have same layout: 2 ints)
    EndianSwap(v->spawn.looping.intervalMsec);
    EndianSwap(v->spawn.looping.count);

    // FxFloatRange spawnRange
    EndianSwap(v->spawnRange.base);
    EndianSwap(v->spawnRange.amplitude);

    // FxFloatRange fadeInRange
    EndianSwap(v->fadeInRange.base);
    EndianSwap(v->fadeInRange.amplitude);

    // FxFloatRange fadeOutRange
    EndianSwap(v->fadeOutRange.base);
    EndianSwap(v->fadeOutRange.amplitude);

    EndianSwap(v->spawnFrustumCullRadius);

    // FxIntRange spawnDelayMsec
    EndianSwap(v->spawnDelayMsec.base);
    EndianSwap(v->spawnDelayMsec.amplitude);

    // FxIntRange lifeSpanMsec
    EndianSwap(v->lifeSpanMsec.base);
    EndianSwap(v->lifeSpanMsec.amplitude);

    // FxFloatRange spawnOrigin[3]
    for (int i = 0; i < 3; i++)
    {
        EndianSwap(v->spawnOrigin[i].base);
        EndianSwap(v->spawnOrigin[i].amplitude);
    }

    // FxFloatRange spawnOffsetRadius
    EndianSwap(v->spawnOffsetRadius.base);
    EndianSwap(v->spawnOffsetRadius.amplitude);

    // FxFloatRange spawnOffsetHeight
    EndianSwap(v->spawnOffsetHeight.base);
    EndianSwap(v->spawnOffsetHeight.amplitude);

    // FxFloatRange spawnAngles[3]
    for (int i = 0; i < 3; i++)
    {
        EndianSwap(v->spawnAngles[i].base);
        EndianSwap(v->spawnAngles[i].amplitude);
    }

    // FxFloatRange angularVelocity[3]
    for (int i = 0; i < 3; i++)
    {
        EndianSwap(v->angularVelocity[i].base);
        EndianSwap(v->angularVelocity[i].amplitude);
    }

    // FxFloatRange initialRotation
    EndianSwap(v->initialRotation.base);
    EndianSwap(v->initialRotation.amplitude);

    // FxFloatRange gravity
    EndianSwap(v->gravity.base);
    EndianSwap(v->gravity.amplitude);

    // FxFloatRange reflectionFactor
    EndianSwap(v->reflectionFactor.base);
    EndianSwap(v->reflectionFactor.amplitude);

    // FxElemAtlas atlas - only entryCount needs swap (rest are bytes)
    EndianSwap(v->atlas.entryCount);

    // bytes: elemType, visualCount, velIntervalCount, visStateIntervalCount - no swap

    EndianSwap(v->velSamples);
    EndianSwap(v->visSamples);

    // FxElemDefVisuals visuals - union of pointers, swap as pointer
    EndianSwap(v->visuals.markArray);

    // float collMins[3]
    EndianSwap(v->collMins);

    // float collMaxs[3]
    EndianSwap(v->collMaxs);

    // FxEffectDefRef - union of pointers
    EndianSwap(v->effectOnImpact.handle);
    EndianSwap(v->effectOnDeath.handle);
    EndianSwap(v->effectEmitted.handle);

    // FxFloatRange emitDist
    EndianSwap(v->emitDist.base);
    EndianSwap(v->emitDist.amplitude);

    // FxFloatRange emitDistVariance
    EndianSwap(v->emitDistVariance.base);
    EndianSwap(v->emitDistVariance.amplitude);

    EndianSwap(v->trailDef);

    // bytes: sortOrder, lightingFrac, useItemClip, unused - no swap
}

static inline void EndianSwap_FxElemMarkVisuals(IW3Xenon::FxElemMarkVisuals* v)
{
    EndianSwap(v->materials);
}

static inline void EndianSwap_FxElemVisuals(IW3Xenon::FxElemVisuals* v)
{
    // Union of pointers - all members overlap, swap any one
    EndianSwap(v->anonymous);
}

static inline void EndianSwap_FxElemDefVisuals(IW3Xenon::FxElemDefVisuals* v)
{
    assert(false);
}

static inline void EndianSwap_FxEffectDefRef(IW3Xenon::FxEffectDefRef* v)
{
    assert(false);
}

static inline void EndianSwap_FxTrailDef(IW3Xenon::FxTrailDef* v)
{
    EndianSwap(v->scrollTimeMsec);
    EndianSwap(v->repeatDist);
    EndianSwap(v->splitDist);
    EndianSwap(v->vertCount);
    EndianSwap(v->verts);
    EndianSwap(v->indCount);
    EndianSwap(v->inds);
}

static inline void EndianSwap_FxEffectDef(IW3Xenon::FxEffectDef* v)
{
    EndianSwap(v->name);
    EndianSwap(v->flags);
    EndianSwap(v->totalSize);
    EndianSwap(v->msecLoopingLife);
    EndianSwap(v->elemDefCountLooping);
    EndianSwap(v->elemDefCountOneShot);
    EndianSwap(v->elemDefCountEmission);
    EndianSwap(v->elemDefs);
}

// ---- FxImpactTable

static inline void EndianSwap_FxImpactEntry(IW3Xenon::FxImpactEntry* v)
{
    EndianSwap(v->nonflesh);
    EndianSwap(v->flesh);
}

static inline void EndianSwap_FxImpactTable(IW3Xenon::FxImpactTable* v)
{
    EndianSwap(v->name);
    EndianSwap(v->table);
}

// ---- RawFile

static inline void EndianSwap_RawFile(IW3Xenon::RawFile* v)
{
    EndianSwap(v->name);
    EndianSwap(v->len);
    EndianSwap(v->buffer);
}

// ---- StringTable

static inline void EndianSwap_StringTable(IW3Xenon::StringTable* v)
{
    EndianSwap(v->name);
    EndianSwap(v->columnCount);
    EndianSwap(v->rowCount);
    EndianSwap(v->values);
}
