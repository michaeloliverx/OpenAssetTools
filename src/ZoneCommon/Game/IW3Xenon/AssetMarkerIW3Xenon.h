#pragma once

#include "Game/IW3Xenon/XAssets/font_s/font_s_mark_db.h"
#include "Game/IW3Xenon/XAssets/fxeffectdef/fxeffectdef_mark_db.h"
#include "Game/IW3Xenon/XAssets/fximpacttable/fximpacttable_mark_db.h"
#include "Game/IW3Xenon/XAssets/gfximage/gfximage_mark_db.h"
#include "Game/IW3Xenon/XAssets/gfxlightdef/gfxlightdef_mark_db.h"
#include "Game/IW3Xenon/XAssets/loadedsound/loadedsound_mark_db.h"
#include "Game/IW3Xenon/XAssets/localizeentry/localizeentry_mark_db.h"
#include "Game/IW3Xenon/XAssets/mapents/mapents_mark_db.h"
#include "Game/IW3Xenon/XAssets/material/material_mark_db.h"
#include "Game/IW3Xenon/XAssets/materialpixelshader/materialpixelshader_mark_db.h"
#include "Game/IW3Xenon/XAssets/materialtechniqueset/materialtechniqueset_mark_db.h"
#include "Game/IW3Xenon/XAssets/menudef_t/menudef_t_mark_db.h"
#include "Game/IW3Xenon/XAssets/menulist/menulist_mark_db.h"
#include "Game/IW3Xenon/XAssets/physpreset/physpreset_mark_db.h"
#include "Game/IW3Xenon/XAssets/rawfile/rawfile_mark_db.h"
#include "Game/IW3Xenon/XAssets/snd_alias_list_t/snd_alias_list_t_mark_db.h"
#include "Game/IW3Xenon/XAssets/sndcurve/sndcurve_mark_db.h"
#include "Game/IW3Xenon/XAssets/snddriverglobals/snddriverglobals_mark_db.h"
#include "Game/IW3Xenon/XAssets/stringtable/stringtable_mark_db.h"
#include "Game/IW3Xenon/XAssets/xanimparts/xanimparts_mark_db.h"
#include "Game/IW3Xenon/XAssets/xmodel/xmodel_mark_db.h"
#include "Utils/Endianness.h"
// The following is a total hack for now just testing

// ---- Endian Swap Helpers

/**
 * Swaps a 32-bit zone pointer/handle from big-endian to host byte order in-place.
 * Handles pointer types by converting through uintptr_t.
 */
static inline auto SwapBigEndianPtr32 = [](auto& p)
{
    std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
    tmp = endianness::FromBigEndian(tmp);
    p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
};

/**
 * Swaps a 32-bit float from big-endian to host byte order in-place.
 */
static inline auto SwapBigEndianFloat = [](float& f)
{
    std::uint32_t tmp;
    std::memcpy(&tmp, &f, sizeof(float));
    tmp = endianness::FromBigEndian(tmp);
    std::memcpy(&f, &tmp, sizeof(float));
};

/**
 * Macro to swap a member field from big-endian to host byte order in-place.
 * Usage: SWAP_BE_MEMBER(ptr, memberName)
 * Expands to: ptr->memberName = endianness::FromBigEndian(ptr->memberName)
 */
#define SWAP_BE_MEMBER(ptr, member) (ptr)->member = endianness::FromBigEndian((ptr)->member)

// ---- PhysPreset

static inline void EndianFixup_PhysPreset(IW3Xenon::PhysPreset* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, type);
    SwapBigEndianFloat(v->mass);
    SwapBigEndianFloat(v->bounce);
    SwapBigEndianFloat(v->friction);
    SwapBigEndianFloat(v->bulletForceScale);
    SwapBigEndianFloat(v->explosiveForceScale);
    SwapBigEndianPtr32(v->sndAliasPrefix);
    SwapBigEndianFloat(v->piecesSpreadFraction);
    SwapBigEndianFloat(v->piecesUpwardVelocity);
    // bool tempDefaultToCylinder
}

// ---- XAnimParts

static inline void EndianFixup_XAnimNotifyInfo(IW3Xenon::XAnimNotifyInfo* v)
{
    SWAP_BE_MEMBER(v, name);
    SwapBigEndianFloat(v->time);
}

static inline void EndianFixup_XAnimDeltaPart(IW3Xenon::XAnimDeltaPart* v)
{
    SwapBigEndianPtr32(v->trans);
    SwapBigEndianPtr32(v->quat);
}

static inline void EndianFixup_XAnimPartTrans_Partial(IW3Xenon::XAnimPartTrans* v, size_t loaded_size)
{
    // hard coded not dynamic
    SWAP_BE_MEMBER(v, size);
}

static inline void EndianFixup_XAnimPartTransFrames_Partial(IW3Xenon::XAnimPartTransFrames* v, size_t loaded_size)
{
    SwapBigEndianFloat(v->mins[0]);
    SwapBigEndianFloat(v->mins[1]);
    SwapBigEndianFloat(v->mins[2]);
    SwapBigEndianFloat(v->size[0]);
    SwapBigEndianFloat(v->size[1]);
    SwapBigEndianFloat(v->size[2]);
    SwapBigEndianPtr32(v->frames._1);
}

static inline void EndianFixup_XAnimDynamicFrames(IW3Xenon::XAnimDynamicFrames* v)
{
    assert(false);
}

static inline void EndianFixup_XAnimDeltaPartQuat_Partial(IW3Xenon::XAnimDeltaPartQuat* v, size_t loaded_size)
{
    assert(false);
}

static inline void EndianFixup_XAnimDeltaPartQuatDataFrames_Partial(IW3Xenon::XAnimDeltaPartQuatDataFrames* v, size_t loaded_size)
{
    assert(false);
}

static inline void EndianFixup_XAnimIndices(IW3Xenon::XAnimIndices* v)
{
    assert(false);
}

static inline void EndianFixup_XAnimParts(IW3Xenon::XAnimParts* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, dataByteCount);
    SWAP_BE_MEMBER(v, dataShortCount);
    SWAP_BE_MEMBER(v, dataIntCount);
    SWAP_BE_MEMBER(v, randomDataByteCount);
    SWAP_BE_MEMBER(v, randomDataIntCount);
    SWAP_BE_MEMBER(v, numframes);

    SWAP_BE_MEMBER(v, randomDataShortCount);
    SWAP_BE_MEMBER(v, indexCount);

    SwapBigEndianFloat(v->framerate);
    SwapBigEndianFloat(v->frequency);
    SwapBigEndianPtr32(v->names);
    SwapBigEndianPtr32(v->dataByte);
    SwapBigEndianPtr32(v->dataShort);
    SwapBigEndianPtr32(v->dataInt);
    SwapBigEndianPtr32(v->randomDataShort);
    SwapBigEndianPtr32(v->randomDataByte);
    SwapBigEndianPtr32(v->randomDataInt);

    // Union of pointers
    SwapBigEndianPtr32(v->indices.data);

    SwapBigEndianPtr32(v->notify);
    SwapBigEndianPtr32(v->deltaPart);
}

// ---- XModel

static inline void EndianFixup_XSurface(IW3Xenon::XSurface* v)
{
    SWAP_BE_MEMBER(v, vertCount);
    SWAP_BE_MEMBER(v, triCount);
    SwapBigEndianPtr32(v->triIndices);
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, vertInfo.vertCount[i]);
    SwapBigEndianPtr32(v->vertInfo.vertsBlend);
    SwapBigEndianPtr32(v->verts0);
    SWAP_BE_MEMBER(v, vertListCount);
    SwapBigEndianPtr32(v->vertList);
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, partBits[i]);
}

static inline void EndianFixup_XSurfaceVertexInfo(IW3Xenon::XSurfaceVertexInfo* v)
{
    assert(false);
}

static inline void EndianFixup_XRigidVertList(IW3Xenon::XRigidVertList* v)
{
    SWAP_BE_MEMBER(v, boneOffset);
    SWAP_BE_MEMBER(v, vertCount);
    SWAP_BE_MEMBER(v, triOffset);
    SWAP_BE_MEMBER(v, triCount);
    SwapBigEndianPtr32(v->collisionTree);
}

static inline void EndianFixup_XSurfaceCollisionTree(IW3Xenon::XSurfaceCollisionTree* v)
{
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->trans[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->scale[i]);
    SWAP_BE_MEMBER(v, nodeCount);
    SwapBigEndianPtr32(v->nodes);
    SWAP_BE_MEMBER(v, leafCount);
    SwapBigEndianPtr32(v->leafs);
}

static inline void EndianFixup_XModelStreamInfo(IW3Xenon::XModelStreamInfo* v)
{
    assert(false);
}

static inline void EndianFixup_PhysGeomList(IW3Xenon::PhysGeomList* v)
{
    SWAP_BE_MEMBER(v, count);
    SwapBigEndianPtr32(v->geoms);
    // PhysMass mass
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->mass.centerOfMass[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->mass.momentsOfInertia[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->mass.productsOfInertia[i]);
}

static inline void EndianFixup_PhysGeomInfo(IW3Xenon::PhysGeomInfo* v)
{
    SwapBigEndianPtr32(v->brush);
    SWAP_BE_MEMBER(v, type);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            SwapBigEndianFloat(v->orientation[i][j]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->offset[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->halfLengths[i]);
}

static inline void EndianFixup_BrushWrapper(IW3Xenon::BrushWrapper* v)
{
    assert(false);
}

static inline void EndianFixup_cbrushside_t(IW3Xenon::cbrushside_t* v)
{
    assert(false);
}

static inline void EndianFixup_XModel(IW3Xenon::XModel* v)
{
    SwapBigEndianPtr32(v->name);
    SwapBigEndianPtr32(v->boneNames);
    SwapBigEndianPtr32(v->parentList);
    SwapBigEndianPtr32(v->quats);
    SwapBigEndianPtr32(v->trans);
    SwapBigEndianPtr32(v->partClassification);
    SwapBigEndianPtr32(v->baseMat);
    SwapBigEndianPtr32(v->surfs);
    SwapBigEndianPtr32(v->materialHandles);
    for (int i = 0; i < 4; i++)
    {
        SwapBigEndianFloat(v->lodInfo[i].dist);
        SWAP_BE_MEMBER(v, lodInfo[i].numsurfs);
        SWAP_BE_MEMBER(v, lodInfo[i].surfIndex);
        for (int j = 0; j < 4; j++)
            SWAP_BE_MEMBER(v, lodInfo[i].partBits[j]);
    }
    SwapBigEndianPtr32(v->collSurfs);
    SWAP_BE_MEMBER(v, numCollSurfs);
    SWAP_BE_MEMBER(v, contents);
    SwapBigEndianPtr32(v->boneInfo);
    SwapBigEndianFloat(v->radius);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->mins[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->maxs[i]);
    SWAP_BE_MEMBER(v, numLods);
    SWAP_BE_MEMBER(v, collLod);
    SwapBigEndianPtr32(v->streamInfo.highMipBounds);
    SWAP_BE_MEMBER(v, memUsage);
    SwapBigEndianPtr32(v->physPreset);
    SwapBigEndianPtr32(v->physGeoms);
}

// ---- Material

static inline void EndianFixup_MaterialTextureDef(IW3Xenon::MaterialTextureDef* v)
{
    SWAP_BE_MEMBER(v, nameHash);
    SwapBigEndianPtr32(v->u.image);
}

static inline void EndianFixup_MaterialInfo(IW3Xenon::MaterialInfo* v)
{
    // name: XString handle (zone pointer)
    SwapBigEndianPtr32(v->name);

    // gameFlags, sortKey, textureAtlasRowCount, textureAtlasColumnCount: all uint8, no swap needed

    // drawSurf: union with uint64 packed field
    SWAP_BE_MEMBER(v, drawSurf.packed);

    // surfaceTypeBits: uint32
    SWAP_BE_MEMBER(v, surfaceTypeBits);
}

static inline void EndianFixup_MaterialTextureDefInfo(IW3Xenon::MaterialTextureDefInfo* v)
{
    // Union: could be either image or water pointer
    // Both are pointers, so we swap as a generic 32-bit pointer
    SwapBigEndianPtr32(v->image);
}

static inline void EndianFixup_water_t(IW3Xenon::water_t* v)
{
    // writable.floatTime: float (32-bit), needs swap
    SwapBigEndianFloat(v->writable.floatTime);

    // Pointer fields
    SwapBigEndianPtr32(v->H0X);
    SwapBigEndianPtr32(v->H0Y);
    SwapBigEndianPtr32(v->wTerm);

    // Integer fields
    SWAP_BE_MEMBER(v, M);
    SWAP_BE_MEMBER(v, N);

    // Float fields
    SwapBigEndianFloat(v->Lx);
    SwapBigEndianFloat(v->Lz);
    SwapBigEndianFloat(v->gravity);
    SwapBigEndianFloat(v->windvel);
    SwapBigEndianFloat(v->winddir[0]);
    SwapBigEndianFloat(v->winddir[1]);
    SwapBigEndianFloat(v->amplitude);

    // Float array
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->codeConstant[i]);

    // Image pointer
    SwapBigEndianPtr32(v->image);
}

static inline void EndianFixup_Material(IW3Xenon::Material* v)
{
    // info: MaterialInfo struct - handle via its fixup function
    EndianFixup_MaterialInfo(&v->info);

    // stateBitsEntry[26]: array of uint8, no swap needed
    // textureCount, constantCount, stateBitsCount, stateFlags, cameraRegion: all uint8, no swap needed

    // Pointer fields
    SwapBigEndianPtr32(v->techniqueSet);
    SwapBigEndianPtr32(v->textureTable);
    SwapBigEndianPtr32(v->constantTable);
    SwapBigEndianPtr32(v->stateBitsTable);
}

// ---- MaterialTechniqueSet

static inline void EndianFixup_MaterialTechniqueSet(IW3Xenon::MaterialTechniqueSet* v)
{
    SwapBigEndianPtr32(v->name);
    SwapBigEndianPtr32(v->remappedTechniqueSet);
    for (int i = 0; i < 26; i++)
        SwapBigEndianPtr32(v->techniques[i]);
}

static inline void EndianFixup_MaterialVertexShader(IW3Xenon::MaterialVertexShader* v)
{
    SwapBigEndianPtr32(v->name);

    auto& ld = v->prog.loadDef;
    SwapBigEndianPtr32(ld.cachedPart);
    SwapBigEndianPtr32(ld.physicalPart);
    SWAP_BE_MEMBER((&ld), cachedPartSize);
    SWAP_BE_MEMBER((&ld), physicalPartSize);
}

static inline void EndianFixup_MaterialVertexShaderProgram(IW3Xenon::MaterialVertexShaderProgram* v)
{
    // Embedded loadDef is handled separately to avoid double-swapping
    assert(false);
}

static inline void EndianFixup_GfxVertexShaderLoadDef(IW3Xenon::GfxVertexShaderLoadDef* v)
{
    assert(false);
}

static inline void EndianFixup_MaterialPixelShader(IW3Xenon::MaterialPixelShader* v)
{
    // name: XString handle (FOLLOWING / OFFSET / etc.)
    SwapBigEndianPtr32(v->name);

    // prog: union storage from the file is treated as loadDef for zone loading
    auto& ld = v->prog.loadDef;

    SwapBigEndianPtr32(ld.cachedPart);
    SwapBigEndianPtr32(ld.physicalPart);

    SWAP_BE_MEMBER((&ld), cachedPartSize);
    SWAP_BE_MEMBER((&ld), physicalPartSize);
}

static inline void EndianFixup_MaterialPixelShaderProgram(IW3Xenon::MaterialPixelShaderProgram* v)
{
    assert(false);
}

static inline void EndianFixup_GfxPixelShaderLoadDef(IW3Xenon::GfxPixelShaderLoadDef* v)
{
    assert(false);
}

static inline void EndianFixup_MaterialShaderArgument(IW3Xenon::MaterialShaderArgument* v)
{
    SWAP_BE_MEMBER(v, type);
    SWAP_BE_MEMBER(v, dest);

    // Swap union field based on type
    switch (v->type)
    {
    case IW3Xenon::MTL_ARG_LITERAL_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_LITERAL_PIXEL_CONST:
        // literalConst: pointer
        SwapBigEndianPtr32(v->u.literalConst);
        break;

    case IW3Xenon::MTL_ARG_CODE_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_CODE_PIXEL_CONST:
        // codeConst: struct with uint16 index + uint8 firstRow + uint8 rowCount
        SWAP_BE_MEMBER((&v->u.codeConst), index);
        // firstRow and rowCount are uint8, no swap needed
        break;

    case IW3Xenon::MTL_ARG_CODE_PIXEL_SAMPLER:
        // codeSampler: uint32
        SWAP_BE_MEMBER(v, u.codeSampler);
        break;

    case IW3Xenon::MTL_ARG_MATERIAL_VERTEX_CONST:
    case IW3Xenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
    case IW3Xenon::MTL_ARG_MATERIAL_PIXEL_CONST:
        // nameHash: uint32
        SWAP_BE_MEMBER(v, u.nameHash);
        break;

    default:
        assert(false);
        break;
    }
}

static inline void EndianFixup_MaterialArgumentDef(IW3Xenon::MaterialArgumentDef* v)
{
    assert(false);
}

static inline void EndianFixup_MaterialPass(IW3Xenon::MaterialPass* v)
{
    SwapBigEndianPtr32(v->vertexDecl);

    for (int i = 0; i < 15; i++)
        SwapBigEndianPtr32(v->vertexShaderArray[i]);

    SwapBigEndianPtr32(v->vertexShader);
    SwapBigEndianPtr32(v->pixelShader);
    SwapBigEndianPtr32(v->args);

    // uint8 fields: no swap
}

static inline void EndianFixup_MaterialTechnique_Partial(IW3Xenon::MaterialTechnique* v, size_t loaded_size)
{
    // TODO: not dynamic at the moment as we know the only callsite

    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, flags);
    SWAP_BE_MEMBER(v, passCount);
}

// ---- GfxImage

static inline void EndianFixup_GfxTexture(IW3Xenon::GfxTexture* v)
{
    // Union with loadDef pointer
    SwapBigEndianPtr32(v->cubemap);
}

static inline void EndianFixup_GfxImageLoadDef(IW3Xenon::GfxImageLoadDef* v)
{
    // levelCount: uint8, no swap needed
    // flags: uint8, no swap needed

    // dimensions[3]: array of int16
    SWAP_BE_MEMBER(v, dimensions[0]);
    SWAP_BE_MEMBER(v, dimensions[1]);
    SWAP_BE_MEMBER(v, dimensions[2]);

    // format: int (32-bit)
    SWAP_BE_MEMBER(v, format);

    // texture: GfxTexture union - handle via its fixup function
    EndianFixup_GfxTexture(&v->texture);
}

static inline void EndianFixup_GfxTextureLoad(IW3Xenon::GfxTextureLoad* v)
{
    SwapBigEndianPtr32(v->loadDef);
}

static inline void EndianFixup_GfxImage(IW3Xenon::GfxImage* v)
{
    // mapType: enum (int, 32-bit)
    v->mapType = static_cast<IW3Xenon::MapType>(endianness::FromBigEndian(static_cast<int>(v->mapType)));

    // texture: GfxTexture union
    SwapBigEndianPtr32(v->texture.loadDef);

    // semantic: uint8, no swap needed

    // cardMemory.platform[1]: array of int (32-bit)
    SWAP_BE_MEMBER(v, cardMemory.platform[0]);

    // width, height, depth: uint16
    SWAP_BE_MEMBER(v, width);
    SWAP_BE_MEMBER(v, height);
    SWAP_BE_MEMBER(v, depth);

    // category: uint8, no swap needed
    // delayLoadPixels: bool, no swap needed

    // pixels: pointer
    SwapBigEndianPtr32(v->pixels);

    // baseSize: uint (32-bit)
    SWAP_BE_MEMBER(v, baseSize);

    // streamSlot: uint16
    SWAP_BE_MEMBER(v, streamSlot);

    // streaming: bool, no swap needed

    // name: pointer
    SwapBigEndianPtr32(v->name);
}

// ---- snd_alias_list_t

static inline void EndianFixup_snd_alias_t(IW3Xenon::snd_alias_t* v)
{
    SwapBigEndianPtr32(v->aliasName);
    SwapBigEndianPtr32(v->subtitle);
    SwapBigEndianPtr32(v->secondaryAliasName);
    SwapBigEndianPtr32(v->chainAliasName);
    SwapBigEndianPtr32(v->soundFile);
    SWAP_BE_MEMBER(v, sequence);
    SwapBigEndianFloat(v->volMin);
    SwapBigEndianFloat(v->volMax);
    SwapBigEndianFloat(v->pitchMin);
    SwapBigEndianFloat(v->pitchMax);
    SwapBigEndianFloat(v->distMin);
    SwapBigEndianFloat(v->distMax);
    SWAP_BE_MEMBER(v, flags);
    SwapBigEndianFloat(v->slavePercentage);
    SwapBigEndianFloat(v->probability);
    SwapBigEndianFloat(v->lfePercentage);
    SwapBigEndianFloat(v->centerPercentage);
    SWAP_BE_MEMBER(v, startDelay);
    SwapBigEndianPtr32(v->volumeFalloffCurve);
    SwapBigEndianFloat(v->envelopMin);
    SwapBigEndianFloat(v->envelopMax);
    SwapBigEndianFloat(v->envelopPercentage);
    SwapBigEndianPtr32(v->speakerMap);
}

static inline void EndianFixup_XAUDIOCHANNELMAP(IW3Xenon::XAUDIOCHANNELMAP* v)
{
    assert(false);
}

static inline void EndianFixup_SoundFile(IW3Xenon::SoundFile* v)
{
    // type and exists are bytes - no swap
    if (v->type == IW3Xenon::SAT_LOADED)
    {
        SwapBigEndianPtr32(v->u.loadSnd);
    }
    else if (v->type == IW3Xenon::SAT_STREAMED)
    {
        SWAP_BE_MEMBER(v, u.streamSnd.filename.fileIndex);
        // StreamFileInfo: either raw (2 pointers) or packed (2 uints) - same byte sizes
        SwapBigEndianPtr32(v->u.streamSnd.filename.info.raw.dir);
        SwapBigEndianPtr32(v->u.streamSnd.filename.info.raw.name);
    }
}

static inline void EndianFixup_SoundFileRef(IW3Xenon::SoundFileRef* v)
{
    assert(false);
}

static inline void EndianFixup_StreamedSound(IW3Xenon::StreamedSound* v)
{
    assert(false);
}

static inline void EndianFixup_StreamFileName(IW3Xenon::StreamFileName* v)
{
    assert(false);
}

static inline void EndianFixup_StreamFileInfo(IW3Xenon::StreamFileInfo* v)
{
    assert(false);
}

static inline void EndianFixup_StreamFileNameRaw(IW3Xenon::StreamFileNameRaw* v)
{
    assert(false);
}

static inline void EndianFixup_SpeakerMap(IW3Xenon::SpeakerMap* v)
{
    // isDefault is bool (1 byte) - no swap
    SwapBigEndianPtr32(v->name);
    // XAUDIOCHANNELMAP channelMaps[2][2]
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            // EntryCount is byte - no swap
            SwapBigEndianPtr32(v->channelMaps[i][j].paEntries);
        }
    }
}

static inline void EndianFixup_snd_alias_list_t(IW3Xenon::snd_alias_list_t* v)
{
    SwapBigEndianPtr32(v->aliasName);
    SwapBigEndianPtr32(v->head);
    SWAP_BE_MEMBER(v, count);
}

// ---- SndCurve

static inline void EndianFixup_SndCurve(IW3Xenon::SndCurve* v)
{
    SwapBigEndianPtr32(v->filename);
    SWAP_BE_MEMBER(v, knotCount);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            SwapBigEndianFloat(v->knots[i][j]);
        }
    }
}

// ---- LoadedSound

static inline void EndianFixup_XaSound(IW3Xenon::XaSound* v)
{
    assert(false);
}

static inline void EndianFixup_XAUDIOPACKET_ALIGNED(IW3Xenon::XAUDIOPACKET_ALIGNED* v)
{
    assert(false);
}

static inline void EndianFixup_XaSeekTable(IW3Xenon::XaSeekTable* v)
{
    assert(false);
}

static inline void EndianFixup_LoadedSound(IW3Xenon::LoadedSound* v)
{
    SwapBigEndianPtr32(v->name);

    // XaSound sound
    // XAUDIOPACKET_ALIGNED packet
    SwapBigEndianPtr32(v->sound.packet.pBuffer);
    SWAP_BE_MEMBER(v, sound.packet.BufferSize);
    SWAP_BE_MEMBER(v, sound.packet.LoopCount);
    // XMALOOPREGION XMALoop[6]
    for (int i = 0; i < 6; i++)
    {
        SWAP_BE_MEMBER(v, sound.packet.XMALoop[i].LoopStart);
        SWAP_BE_MEMBER(v, sound.packet.XMALoop[i].LoopEnd);
        // LoopSubframeEnd and LoopSubframeSkip are bytes - no swap
    }
    SwapBigEndianPtr32(v->sound.packet.pContext);

    // XAUDIOSOURCEFORMAT format
    // SampleType is byte - no swap
    // TODO: find enum and correctly endian swap

    // XaIwXmaDataInfo xaIwXmaDataInfo
    SWAP_BE_MEMBER(v, sound.xaIwXmaDataInfo.totalMsec);

    // XaSeekTable seekTable
    SWAP_BE_MEMBER(v, sound.seekTable.size);
    SwapBigEndianPtr32(v->sound.seekTable.data);
}

// ---- MapEnts

static inline void EndianFixup_MapEnts(IW3Xenon::MapEnts* v)
{
    assert(false);
}

// ---- GfxLightDef
static inline void EndianFixup_GfxLightImage(IW3Xenon::GfxLightImage* v)
{
    assert(false);
}

static inline void EndianFixup_GfxLightDef(IW3Xenon::GfxLightDef* v)
{
    SwapBigEndianPtr32(v->name);
    // GfxLightImage attenuation
    SwapBigEndianPtr32(v->attenuation.image);
    // samplerState is a byte - no swap
    SWAP_BE_MEMBER(v, lmapLookupStart);
}

// ---- Font_s

static inline void EndianFixup_Font_s(IW3Xenon::Font_s* v)
{
    SwapBigEndianPtr32(v->fontName);
    SWAP_BE_MEMBER(v, pixelHeight);
    SWAP_BE_MEMBER(v, glyphCount);
    SwapBigEndianPtr32(v->material);
    SwapBigEndianPtr32(v->glowMaterial);
    SwapBigEndianPtr32(v->glyphs);
}

// ---- MenuList

static inline void EndianFixup_MenuList(IW3Xenon::MenuList* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, menuCount);
    SwapBigEndianPtr32(v->menus);
}

// ---- menuDef_t

static inline void EndianFixup_windowDef_t(IW3Xenon::windowDef_t* v)
{
    SwapBigEndianPtr32(v->name);
    SwapBigEndianFloat(v->rect.x);
    SwapBigEndianFloat(v->rect.y);
    SwapBigEndianFloat(v->rect.w);
    SwapBigEndianFloat(v->rect.h);
    SWAP_BE_MEMBER(v, rect.horzAlign);
    SWAP_BE_MEMBER(v, rect.vertAlign);
    SwapBigEndianFloat(v->rectClient.x);
    SwapBigEndianFloat(v->rectClient.y);
    SwapBigEndianFloat(v->rectClient.w);
    SwapBigEndianFloat(v->rectClient.h);
    SWAP_BE_MEMBER(v, rectClient.horzAlign);
    SWAP_BE_MEMBER(v, rectClient.vertAlign);
    SwapBigEndianPtr32(v->group);
    SWAP_BE_MEMBER(v, style);
    SWAP_BE_MEMBER(v, border);
    SWAP_BE_MEMBER(v, ownerDraw);
    SWAP_BE_MEMBER(v, ownerDrawFlags);
    SwapBigEndianFloat(v->borderSize);
    SWAP_BE_MEMBER(v, staticFlags);
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, dynamicFlags[i]);
    SWAP_BE_MEMBER(v, nextTime);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->foreColor[i]);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->backColor[i]);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->borderColor[i]);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->outlineColor[i]);
    SwapBigEndianPtr32(v->background);
}

static inline void EndianFixup_ItemKeyHandler(IW3Xenon::ItemKeyHandler* v)
{
    SWAP_BE_MEMBER(v, key);
    SwapBigEndianPtr32(v->action);
    SwapBigEndianPtr32(v->next);
}

static inline void EndianFixup_statement_s(IW3Xenon::statement_s* v)
{
    assert(false);
}

static inline void EndianFixup_operandInternalDataUnion(IW3Xenon::operandInternalDataUnion* v)
{
    assert(false);
}

static inline void EndianFixup_Operand(IW3Xenon::Operand* v)
{
    v->dataType = static_cast<IW3Xenon::expDataType>(endianness::FromBigEndian(static_cast<int>(v->dataType)));

    // Swap union based on dataType
    if (v->dataType == IW3Xenon::VAL_INT)
    {
        SWAP_BE_MEMBER((&v->internals), intVal);
    }
    else if (v->dataType == IW3Xenon::VAL_FLOAT)
    {
        SwapBigEndianFloat(v->internals.floatVal);
    }
    else if (v->dataType == IW3Xenon::VAL_STRING)
    {
        SwapBigEndianPtr32(v->internals.string);
    }
    else
    {
        assert(false);
    }
}

static inline void EndianFixup_entryInternalData(IW3Xenon::entryInternalData* v)
{
    // Union - contents swapped based on context in expressionEntry
}

static inline void EndianFixup_expressionEntry(IW3Xenon::expressionEntry* v)
{
    SWAP_BE_MEMBER(v, type);

    if (v->type == IW3Xenon::EET_OPERATOR)
    {
        v->data.op = static_cast<IW3Xenon::operationEnum>(endianness::FromBigEndian(static_cast<int>(v->data.op)));
    }
    else if (v->type == IW3Xenon::EET_OPERAND)
    {
        EndianFixup_Operand(&v->data.operand);
    }
    else
    {
        assert(false);
    }
}

static inline void EndianFixup_itemDef_s(IW3Xenon::itemDef_s* v)
{
    // windowDef_t window
    EndianFixup_windowDef_t(&v->window);

    // rectDef_s textRect[4]
    for (int i = 0; i < 4; i++)
    {
        SwapBigEndianFloat(v->textRect[i].x);
        SwapBigEndianFloat(v->textRect[i].y);
        SwapBigEndianFloat(v->textRect[i].w);
        SwapBigEndianFloat(v->textRect[i].h);
        SWAP_BE_MEMBER(v, textRect[i].horzAlign);
        SWAP_BE_MEMBER(v, textRect[i].vertAlign);
    }

    // itemDef_s members
    SWAP_BE_MEMBER(v, type);
    SWAP_BE_MEMBER(v, dataType);
    SWAP_BE_MEMBER(v, alignment);
    SWAP_BE_MEMBER(v, fontEnum);
    SWAP_BE_MEMBER(v, textAlignMode);
    SwapBigEndianFloat(v->textalignx);
    SwapBigEndianFloat(v->textaligny);
    SwapBigEndianFloat(v->textscale);
    SWAP_BE_MEMBER(v, textStyle);
    SWAP_BE_MEMBER(v, gameMsgWindowIndex);
    SWAP_BE_MEMBER(v, gameMsgWindowMode);
    SwapBigEndianPtr32(v->text);
    SWAP_BE_MEMBER(v, itemFlags);
    SwapBigEndianPtr32(v->parent);
    SwapBigEndianPtr32(v->mouseEnterText);
    SwapBigEndianPtr32(v->mouseExitText);
    SwapBigEndianPtr32(v->mouseEnter);
    SwapBigEndianPtr32(v->mouseExit);
    SwapBigEndianPtr32(v->action);
    SwapBigEndianPtr32(v->onAccept);
    SwapBigEndianPtr32(v->onFocus);
    SwapBigEndianPtr32(v->leaveFocus);
    SwapBigEndianPtr32(v->dvar);
    SwapBigEndianPtr32(v->dvarTest);
    SwapBigEndianPtr32(v->onKey);
    SwapBigEndianPtr32(v->enableDvar);
    SWAP_BE_MEMBER(v, dvarFlags);
    SwapBigEndianPtr32(v->focusSound);
    SwapBigEndianFloat(v->special);
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, cursorPos[i]);

    // itemDefData_t typeData (union with pointer)
    SwapBigEndianPtr32(v->typeData.data);

    SWAP_BE_MEMBER(v, imageTrack);

    // statement_s visibleExp
    SWAP_BE_MEMBER(v, visibleExp.numEntries);
    SwapBigEndianPtr32(v->visibleExp.entries);

    // statement_s textExp
    SWAP_BE_MEMBER(v, textExp.numEntries);
    SwapBigEndianPtr32(v->textExp.entries);

    // statement_s materialExp
    SWAP_BE_MEMBER(v, materialExp.numEntries);
    SwapBigEndianPtr32(v->materialExp.entries);

    // statement_s rectXExp
    SWAP_BE_MEMBER(v, rectXExp.numEntries);
    SwapBigEndianPtr32(v->rectXExp.entries);

    // statement_s rectYExp
    SWAP_BE_MEMBER(v, rectYExp.numEntries);
    SwapBigEndianPtr32(v->rectYExp.entries);

    // statement_s rectWExp
    SWAP_BE_MEMBER(v, rectWExp.numEntries);
    SwapBigEndianPtr32(v->rectWExp.entries);

    // statement_s rectHExp
    SWAP_BE_MEMBER(v, rectHExp.numEntries);
    SwapBigEndianPtr32(v->rectHExp.entries);

    // statement_s forecolorAExp
    SWAP_BE_MEMBER(v, forecolorAExp.numEntries);
    SwapBigEndianPtr32(v->forecolorAExp.entries);
}

static inline void EndianFixup_itemDefData_t(IW3Xenon::itemDefData_t* v)
{
    assert(false);
}

static inline void EndianFixup_listBoxDef_s(IW3Xenon::listBoxDef_s* v)
{
    // int startPos[4];
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, startPos[i]);

    // int endPos[4];
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, endPos[i]);

    SWAP_BE_MEMBER(v, drawPadding);
    SwapBigEndianFloat(v->elementWidth);
    SwapBigEndianFloat(v->elementHeight);
    SWAP_BE_MEMBER(v, elementStyle);
    SWAP_BE_MEMBER(v, numColumns);

    for (int i = 0; i < 16; i++)
    {
        SWAP_BE_MEMBER(v, columnInfo[i].pos);
        SWAP_BE_MEMBER(v, columnInfo[i].width);
        SWAP_BE_MEMBER(v, columnInfo[i].maxChars);
        SWAP_BE_MEMBER(v, columnInfo[i].alignment);
    }

    SwapBigEndianPtr32(v->doubleClick);
    SWAP_BE_MEMBER(v, notselectable);
    SWAP_BE_MEMBER(v, noScrollBars);
    SWAP_BE_MEMBER(v, usePaging);

    // int selectBorder[4];
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->selectBorder[i]);

    // int disableColor[4];
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->disableColor[i]);

    SwapBigEndianPtr32(v->selectIcon);
}

static inline void EndianFixup_multiDef_s(IW3Xenon::multiDef_s* v)
{
    assert(false);
}

static inline void EndianFixup_menuDef_t(IW3Xenon::menuDef_t* v)
{
    // windowDef_t window
    EndianFixup_windowDef_t(&v->window);

    // menuDef_t members
    SwapBigEndianPtr32(v->font);
    SWAP_BE_MEMBER(v, fullScreen);
    SWAP_BE_MEMBER(v, itemCount);
    SWAP_BE_MEMBER(v, fontIndex);
    for (int i = 0; i < 4; i++)
        SWAP_BE_MEMBER(v, cursorItem[i]);
    SWAP_BE_MEMBER(v, fadeCycle);
    SwapBigEndianFloat(v->fadeClamp);
    SwapBigEndianFloat(v->fadeAmount);
    SwapBigEndianFloat(v->fadeInAmount);
    SwapBigEndianFloat(v->blurRadius);
    SwapBigEndianPtr32(v->onOpen);
    SwapBigEndianPtr32(v->onClose);
    SwapBigEndianPtr32(v->onESC);
    SwapBigEndianPtr32(v->onKey);
    SWAP_BE_MEMBER(v, visibleExp.numEntries);
    SwapBigEndianPtr32(v->visibleExp.entries);
    SwapBigEndianPtr32(v->allowedBinding);
    SwapBigEndianPtr32(v->soundName);
    SWAP_BE_MEMBER(v, imageTrack);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->focusColor[i]);
    for (int i = 0; i < 4; i++)
        SwapBigEndianFloat(v->disableColor[i]);
    SWAP_BE_MEMBER(v, rectXExp.numEntries);
    SwapBigEndianPtr32(v->rectXExp.entries);
    SWAP_BE_MEMBER(v, rectYExp.numEntries);
    SwapBigEndianPtr32(v->rectYExp.entries);
    SwapBigEndianPtr32(v->items);
}

// ---- LocalizeEntry

static inline void EndianFixup_LocalizeEntry(IW3Xenon::LocalizeEntry* v)
{
    SwapBigEndianPtr32(v->name);
    SwapBigEndianPtr32(v->value);
}

// ---- SndDriverGlobals

static inline void EndianFixup_SndDriverGlobals(IW3Xenon::SndDriverGlobals* v)
{
    SwapBigEndianPtr32(v->reverbSettings);
    SwapBigEndianPtr32(v->name);
}

// ---- FXEffectDef

static inline void EndianFixup_FxElemDef(IW3Xenon::FxElemDef* v)
{
    SWAP_BE_MEMBER(v, flags);

    // FxSpawnDef spawn (union - both members have same layout: 2 ints)
    SWAP_BE_MEMBER(v, spawn.looping.intervalMsec);
    SWAP_BE_MEMBER(v, spawn.looping.count);

    // FxFloatRange spawnRange
    SwapBigEndianFloat(v->spawnRange.base);
    SwapBigEndianFloat(v->spawnRange.amplitude);

    // FxFloatRange fadeInRange
    SwapBigEndianFloat(v->fadeInRange.base);
    SwapBigEndianFloat(v->fadeInRange.amplitude);

    // FxFloatRange fadeOutRange
    SwapBigEndianFloat(v->fadeOutRange.base);
    SwapBigEndianFloat(v->fadeOutRange.amplitude);

    SwapBigEndianFloat(v->spawnFrustumCullRadius);

    // FxIntRange spawnDelayMsec
    SWAP_BE_MEMBER(v, spawnDelayMsec.base);
    SWAP_BE_MEMBER(v, spawnDelayMsec.amplitude);

    // FxIntRange lifeSpanMsec
    SWAP_BE_MEMBER(v, lifeSpanMsec.base);
    SWAP_BE_MEMBER(v, lifeSpanMsec.amplitude);

    // FxFloatRange spawnOrigin[3]
    for (int i = 0; i < 3; i++)
    {
        SwapBigEndianFloat(v->spawnOrigin[i].base);
        SwapBigEndianFloat(v->spawnOrigin[i].amplitude);
    }

    // FxFloatRange spawnOffsetRadius
    SwapBigEndianFloat(v->spawnOffsetRadius.base);
    SwapBigEndianFloat(v->spawnOffsetRadius.amplitude);

    // FxFloatRange spawnOffsetHeight
    SwapBigEndianFloat(v->spawnOffsetHeight.base);
    SwapBigEndianFloat(v->spawnOffsetHeight.amplitude);

    // FxFloatRange spawnAngles[3]
    for (int i = 0; i < 3; i++)
    {
        SwapBigEndianFloat(v->spawnAngles[i].base);
        SwapBigEndianFloat(v->spawnAngles[i].amplitude);
    }

    // FxFloatRange angularVelocity[3]
    for (int i = 0; i < 3; i++)
    {
        SwapBigEndianFloat(v->angularVelocity[i].base);
        SwapBigEndianFloat(v->angularVelocity[i].amplitude);
    }

    // FxFloatRange initialRotation
    SwapBigEndianFloat(v->initialRotation.base);
    SwapBigEndianFloat(v->initialRotation.amplitude);

    // FxFloatRange gravity
    SwapBigEndianFloat(v->gravity.base);
    SwapBigEndianFloat(v->gravity.amplitude);

    // FxFloatRange reflectionFactor
    SwapBigEndianFloat(v->reflectionFactor.base);
    SwapBigEndianFloat(v->reflectionFactor.amplitude);

    // FxElemAtlas atlas - only entryCount needs swap (rest are bytes)
    SWAP_BE_MEMBER(v, atlas.entryCount);

    // bytes: elemType, visualCount, velIntervalCount, visStateIntervalCount - no swap

    SwapBigEndianPtr32(v->velSamples);
    SwapBigEndianPtr32(v->visSamples);

    // FxElemDefVisuals visuals - union of pointers, swap as pointer
    SwapBigEndianPtr32(v->visuals.markArray);

    // float collMins[3]
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->collMins[i]);

    // float collMaxs[3]
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->collMaxs[i]);

    // FxEffectDefRef - union of pointers
    SwapBigEndianPtr32(v->effectOnImpact.handle);
    SwapBigEndianPtr32(v->effectOnDeath.handle);
    SwapBigEndianPtr32(v->effectEmitted.handle);

    // FxFloatRange emitDist
    SwapBigEndianFloat(v->emitDist.base);
    SwapBigEndianFloat(v->emitDist.amplitude);

    // FxFloatRange emitDistVariance
    SwapBigEndianFloat(v->emitDistVariance.base);
    SwapBigEndianFloat(v->emitDistVariance.amplitude);

    SwapBigEndianPtr32(v->trailDef);

    // bytes: sortOrder, lightingFrac, useItemClip, unused - no swap
}

static inline void EndianFixup_FxElemMarkVisuals(IW3Xenon::FxElemMarkVisuals* v)
{
    assert(false);
}

static inline void EndianFixup_FxElemVisuals(IW3Xenon::FxElemVisuals* v)
{
    assert(false);
}

static inline void EndianFixup_FxElemDefVisuals(IW3Xenon::FxElemDefVisuals* v)
{
    assert(false);
}

static inline void EndianFixup_FxEffectDefRef(IW3Xenon::FxEffectDefRef* v)
{
    assert(false);
}

static inline void EndianFixup_FxTrailDef(IW3Xenon::FxTrailDef* v)
{
    assert(false);
}

static inline void EndianFixup_FxEffectDef(IW3Xenon::FxEffectDef* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, flags);
    SWAP_BE_MEMBER(v, totalSize);
    SWAP_BE_MEMBER(v, msecLoopingLife);
    SWAP_BE_MEMBER(v, elemDefCountLooping);
    SWAP_BE_MEMBER(v, elemDefCountOneShot);
    SWAP_BE_MEMBER(v, elemDefCountEmission);
    SwapBigEndianPtr32(v->elemDefs);
}

// ---- FxImpactTable

static inline void EndianFixup_FxImpactEntry(IW3Xenon::FxImpactEntry* v)
{
    for (int i = 0; i < 29; i++)
        SwapBigEndianPtr32(v->nonflesh[i]);
    for (int i = 0; i < 4; i++)
        SwapBigEndianPtr32(v->flesh[i]);
}

static inline void EndianFixup_FxImpactTable(IW3Xenon::FxImpactTable* v)
{
    SwapBigEndianPtr32(v->name);
    SwapBigEndianPtr32(v->table);
}

// ---- RawFile

static inline void EndianFixup_RawFile(IW3Xenon::RawFile* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, len);
    SwapBigEndianPtr32(v->buffer);
}

// ---- StringTable

static inline void EndianFixup_StringTable(IW3Xenon::StringTable* v)
{
    SwapBigEndianPtr32(v->name);
    SWAP_BE_MEMBER(v, columnCount);
    SWAP_BE_MEMBER(v, rowCount);
    SwapBigEndianPtr32(v->values);
}
