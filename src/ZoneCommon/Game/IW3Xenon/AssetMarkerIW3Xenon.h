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
#include "Game/IW3Xenon/XAssets/weapondef/weapondef_mark_db.h"
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

/**
 * Macro to swap an enum member field from big-endian to host byte order in-place.
 * Usage: SWAP_BE_ENUM(ptr, memberName, EnumType)
 * Casts the result back to the enum type.
 */
#define SWAP_BE_ENUM(ptr, member, enumType) (ptr)->member = static_cast<enumType>(endianness::FromBigEndian(static_cast<int>((ptr)->member)))

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

// ---- WeaponDef

static inline void EndianFixup_snd_alias_list_name(IW3Xenon::snd_alias_list_name* v)
{
    SwapBigEndianPtr32(v->soundName);
}

static inline void EndianFixup_SndAliasCustom(IW3Xenon::SndAliasCustom* v)
{
    assert(false);
}

static inline void EndianFixup_WeaponDef(IW3Xenon::WeaponDef* v)
{
    SwapBigEndianPtr32(v->szInternalName);
    SwapBigEndianPtr32(v->szDisplayName);
    SwapBigEndianPtr32(v->szOverlayName);
    for (int i = 0; i < 16; i++)
        SwapBigEndianPtr32(v->gunXModel[i]);
    SwapBigEndianPtr32(v->handXModel);
    for (int i = 0; i < 33; i++)
        SwapBigEndianPtr32(v->szXAnims[i]);
    SwapBigEndianPtr32(v->szModeName);
    for (int i = 0; i < 8; i++)
        SWAP_BE_MEMBER(v, hideTags[i]);
    for (int i = 0; i < 16; i++)
        SWAP_BE_MEMBER(v, notetrackSoundMapKeys[i]);
    for (int i = 0; i < 16; i++)
        SWAP_BE_MEMBER(v, notetrackSoundMapValues[i]);
    SWAP_BE_MEMBER(v, playerAnimType);
    SWAP_BE_ENUM(v, weapType, IW3Xenon::weapType_t);
    SWAP_BE_ENUM(v, weapClass, IW3Xenon::weapClass_t);
    SWAP_BE_ENUM(v, penetrateType, IW3Xenon::PenetrateType);
    SWAP_BE_ENUM(v, impactType, IW3Xenon::ImpactType);
    SWAP_BE_ENUM(v, inventoryType, IW3Xenon::weapInventoryType_t);
    SWAP_BE_ENUM(v, fireType, IW3Xenon::weapFireType_t);
    SWAP_BE_ENUM(v, offhandClass, IW3Xenon::OffhandClass);
    SWAP_BE_ENUM(v, stance, IW3Xenon::weapStance_t);
    SwapBigEndianPtr32(v->viewFlashEffect);
    SwapBigEndianPtr32(v->worldFlashEffect);
    SwapBigEndianPtr32(v->pickupSound.name);
    SwapBigEndianPtr32(v->pickupSoundPlayer.name);
    SwapBigEndianPtr32(v->ammoPickupSound.name);
    SwapBigEndianPtr32(v->ammoPickupSoundPlayer.name);
    SwapBigEndianPtr32(v->projectileSound.name);
    SwapBigEndianPtr32(v->pullbackSound.name);
    SwapBigEndianPtr32(v->pullbackSoundPlayer.name);
    SwapBigEndianPtr32(v->fireSound.name);
    SwapBigEndianPtr32(v->fireSoundPlayer.name);
    SwapBigEndianPtr32(v->fireLoopSound.name);
    SwapBigEndianPtr32(v->fireLoopSoundPlayer.name);
    SwapBigEndianPtr32(v->fireStopSound.name);
    SwapBigEndianPtr32(v->fireStopSoundPlayer.name);
    SwapBigEndianPtr32(v->fireLastSound.name);
    SwapBigEndianPtr32(v->fireLastSoundPlayer.name);
    SwapBigEndianPtr32(v->emptyFireSound.name);
    SwapBigEndianPtr32(v->emptyFireSoundPlayer.name);
    SwapBigEndianPtr32(v->meleeSwipeSound.name);
    SwapBigEndianPtr32(v->meleeSwipeSoundPlayer.name);
    SwapBigEndianPtr32(v->meleeHitSound.name);
    SwapBigEndianPtr32(v->meleeMissSound.name);
    SwapBigEndianPtr32(v->rechamberSound.name);
    SwapBigEndianPtr32(v->rechamberSoundPlayer.name);
    SwapBigEndianPtr32(v->reloadSound.name);
    SwapBigEndianPtr32(v->reloadSoundPlayer.name);
    SwapBigEndianPtr32(v->reloadEmptySound.name);
    SwapBigEndianPtr32(v->reloadEmptySoundPlayer.name);
    SwapBigEndianPtr32(v->reloadStartSound.name);
    SwapBigEndianPtr32(v->reloadStartSoundPlayer.name);
    SwapBigEndianPtr32(v->reloadEndSound.name);
    SwapBigEndianPtr32(v->reloadEndSoundPlayer.name);
    SwapBigEndianPtr32(v->detonateSound.name);
    SwapBigEndianPtr32(v->detonateSoundPlayer.name);
    SwapBigEndianPtr32(v->nightVisionWearSound.name);
    SwapBigEndianPtr32(v->nightVisionWearSoundPlayer.name);
    SwapBigEndianPtr32(v->nightVisionRemoveSound.name);
    SwapBigEndianPtr32(v->nightVisionRemoveSoundPlayer.name);
    SwapBigEndianPtr32(v->altSwitchSound.name);
    SwapBigEndianPtr32(v->altSwitchSoundPlayer.name);
    SwapBigEndianPtr32(v->raiseSound.name);
    SwapBigEndianPtr32(v->raiseSoundPlayer.name);
    SwapBigEndianPtr32(v->firstRaiseSound.name);
    SwapBigEndianPtr32(v->firstRaiseSoundPlayer.name);
    SwapBigEndianPtr32(v->putawaySound.name);
    SwapBigEndianPtr32(v->putawaySoundPlayer.name);
    SwapBigEndianPtr32(v->bounceSound);
    SwapBigEndianPtr32(v->viewShellEjectEffect);
    SwapBigEndianPtr32(v->worldShellEjectEffect);
    SwapBigEndianPtr32(v->viewLastShotEjectEffect);
    SwapBigEndianPtr32(v->worldLastShotEjectEffect);
    SwapBigEndianPtr32(v->reticleCenter);
    SwapBigEndianPtr32(v->reticleSide);
    SWAP_BE_MEMBER(v, iReticleCenterSize);
    SWAP_BE_MEMBER(v, iReticleSideSize);
    SWAP_BE_MEMBER(v, iReticleMinOfs);
    SWAP_BE_ENUM(v, activeReticleType, IW3Xenon::activeReticleType_t);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vStandMove[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vStandRot[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vDuckedOfs[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vDuckedMove[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vDuckedRot[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vProneOfs[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vProneMove[i]);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vProneRot[i]);
    SwapBigEndianFloat(v->fPosMoveRate);
    SwapBigEndianFloat(v->fPosProneMoveRate);
    SwapBigEndianFloat(v->fStandMoveMinSpeed);
    SwapBigEndianFloat(v->fDuckedMoveMinSpeed);
    SwapBigEndianFloat(v->fProneMoveMinSpeed);
    SwapBigEndianFloat(v->fPosRotRate);
    SwapBigEndianFloat(v->fPosProneRotRate);
    SwapBigEndianFloat(v->fStandRotMinSpeed);
    SwapBigEndianFloat(v->fDuckedRotMinSpeed);
    SwapBigEndianFloat(v->fProneRotMinSpeed);
    for (int i = 0; i < 16; i++)
        SwapBigEndianPtr32(v->worldModel[i]);
    SwapBigEndianPtr32(v->worldClipModel);
    SwapBigEndianPtr32(v->rocketModel);
    SwapBigEndianPtr32(v->knifeModel);
    SwapBigEndianPtr32(v->worldKnifeModel);
    SwapBigEndianPtr32(v->hudIcon);
    SWAP_BE_ENUM(v, hudIconRatio, IW3Xenon::weaponIconRatioType_t);
    SwapBigEndianPtr32(v->ammoCounterIcon);
    SWAP_BE_ENUM(v, ammoCounterIconRatio, IW3Xenon::weaponIconRatioType_t);
    SWAP_BE_ENUM(v, ammoCounterClip, IW3Xenon::ammoCounterClipType_t);
    SWAP_BE_MEMBER(v, iStartAmmo);
    SwapBigEndianPtr32(v->szAmmoName);
    SWAP_BE_MEMBER(v, iAmmoIndex);
    SwapBigEndianPtr32(v->szClipName);
    SWAP_BE_MEMBER(v, iClipIndex);
    SWAP_BE_MEMBER(v, iMaxAmmo);
    SWAP_BE_MEMBER(v, iClipSize);
    SWAP_BE_MEMBER(v, shotCount);
    SwapBigEndianPtr32(v->szSharedAmmoCapName);
    SWAP_BE_MEMBER(v, iSharedAmmoCapIndex);
    SWAP_BE_MEMBER(v, iSharedAmmoCap);
    SWAP_BE_MEMBER(v, damage);
    SWAP_BE_MEMBER(v, playerDamage);
    SWAP_BE_MEMBER(v, iMeleeDamage);
    SWAP_BE_MEMBER(v, iDamageType);
    SWAP_BE_MEMBER(v, iFireDelay);
    SWAP_BE_MEMBER(v, iMeleeDelay);
    SWAP_BE_MEMBER(v, meleeChargeDelay);
    SWAP_BE_MEMBER(v, iDetonateDelay);
    SWAP_BE_MEMBER(v, iFireTime);
    SWAP_BE_MEMBER(v, iRechamberTime);
    SWAP_BE_MEMBER(v, iRechamberBoltTime);
    SWAP_BE_MEMBER(v, iHoldFireTime);
    SWAP_BE_MEMBER(v, iDetonateTime);
    SWAP_BE_MEMBER(v, iMeleeTime);
    SWAP_BE_MEMBER(v, meleeChargeTime);
    SWAP_BE_MEMBER(v, iReloadTime);
    SWAP_BE_MEMBER(v, reloadShowRocketTime);
    SWAP_BE_MEMBER(v, iReloadEmptyTime);
    SWAP_BE_MEMBER(v, iReloadAddTime);
    SWAP_BE_MEMBER(v, iReloadStartTime);
    SWAP_BE_MEMBER(v, iReloadStartAddTime);
    SWAP_BE_MEMBER(v, iReloadEndTime);
    SWAP_BE_MEMBER(v, iDropTime);
    SWAP_BE_MEMBER(v, iRaiseTime);
    SWAP_BE_MEMBER(v, iAltDropTime);
    SWAP_BE_MEMBER(v, iAltRaiseTime);
    SWAP_BE_MEMBER(v, quickDropTime);
    SWAP_BE_MEMBER(v, quickRaiseTime);
    SWAP_BE_MEMBER(v, iFirstRaiseTime);
    SWAP_BE_MEMBER(v, iEmptyRaiseTime);
    SWAP_BE_MEMBER(v, iEmptyDropTime);
    SWAP_BE_MEMBER(v, sprintInTime);
    SWAP_BE_MEMBER(v, sprintLoopTime);
    SWAP_BE_MEMBER(v, sprintOutTime);
    SWAP_BE_MEMBER(v, nightVisionWearTime);
    SWAP_BE_MEMBER(v, nightVisionWearTimeFadeOutEnd);
    SWAP_BE_MEMBER(v, nightVisionWearTimePowerUp);
    SWAP_BE_MEMBER(v, nightVisionRemoveTime);
    SWAP_BE_MEMBER(v, nightVisionRemoveTimePowerDown);
    SWAP_BE_MEMBER(v, nightVisionRemoveTimeFadeInStart);
    SWAP_BE_MEMBER(v, fuseTime);
    SWAP_BE_MEMBER(v, aiFuseTime);
    SWAP_BE_MEMBER(v, requireLockonToFire);
    SWAP_BE_MEMBER(v, noAdsWhenMagEmpty);
    SWAP_BE_MEMBER(v, avoidDropCleanup);
    SwapBigEndianFloat(v->autoAimRange);
    SwapBigEndianFloat(v->aimAssistRange);
    SwapBigEndianFloat(v->aimAssistRangeAds);
    SwapBigEndianFloat(v->aimPadding);
    SwapBigEndianFloat(v->enemyCrosshairRange);
    SWAP_BE_MEMBER(v, crosshairColorChange);
    SwapBigEndianFloat(v->moveSpeedScale);
    SwapBigEndianFloat(v->adsMoveSpeedScale);
    SwapBigEndianFloat(v->sprintDurationScale);
    SwapBigEndianFloat(v->fAdsZoomFov);
    SwapBigEndianFloat(v->fAdsZoomInFrac);
    SwapBigEndianFloat(v->fAdsZoomOutFrac);
    SwapBigEndianPtr32(v->overlayMaterial);
    SwapBigEndianPtr32(v->overlayMaterialLowRes);
    SWAP_BE_ENUM(v, overlayReticle, IW3Xenon::weapOverlayReticle_t);
    SWAP_BE_ENUM(v, overlayInterface, IW3Xenon::WeapOverlayInteface_t);
    SwapBigEndianFloat(v->overlayWidth);
    SwapBigEndianFloat(v->overlayHeight);
    SwapBigEndianFloat(v->fAdsBobFactor);
    SwapBigEndianFloat(v->fAdsViewBobMult);
    SwapBigEndianFloat(v->fHipSpreadStandMin);
    SwapBigEndianFloat(v->fHipSpreadDuckedMin);
    SwapBigEndianFloat(v->fHipSpreadProneMin);
    SwapBigEndianFloat(v->hipSpreadStandMax);
    SwapBigEndianFloat(v->hipSpreadDuckedMax);
    SwapBigEndianFloat(v->hipSpreadProneMax);
    SwapBigEndianFloat(v->fHipSpreadDecayRate);
    SwapBigEndianFloat(v->fHipSpreadFireAdd);
    SwapBigEndianFloat(v->fHipSpreadTurnAdd);
    SwapBigEndianFloat(v->fHipSpreadMoveAdd);
    SwapBigEndianFloat(v->fHipSpreadDuckedDecay);
    SwapBigEndianFloat(v->fHipSpreadProneDecay);
    SwapBigEndianFloat(v->fHipReticleSidePos);
    SWAP_BE_MEMBER(v, iAdsTransInTime);
    SWAP_BE_MEMBER(v, iAdsTransOutTime);
    SwapBigEndianFloat(v->fAdsIdleAmount);
    SwapBigEndianFloat(v->fHipIdleAmount);
    SwapBigEndianFloat(v->adsIdleSpeed);
    SwapBigEndianFloat(v->hipIdleSpeed);
    SwapBigEndianFloat(v->fIdleCrouchFactor);
    SwapBigEndianFloat(v->fIdleProneFactor);
    SwapBigEndianFloat(v->fGunMaxPitch);
    SwapBigEndianFloat(v->fGunMaxYaw);
    SwapBigEndianFloat(v->swayMaxAngle);
    SwapBigEndianFloat(v->swayLerpSpeed);
    SwapBigEndianFloat(v->swayPitchScale);
    SwapBigEndianFloat(v->swayYawScale);
    SwapBigEndianFloat(v->swayHorizScale);
    SwapBigEndianFloat(v->swayVertScale);
    SwapBigEndianFloat(v->swayShellShockScale);
    SwapBigEndianFloat(v->adsSwayMaxAngle);
    SwapBigEndianFloat(v->adsSwayLerpSpeed);
    SwapBigEndianFloat(v->adsSwayPitchScale);
    SwapBigEndianFloat(v->adsSwayYawScale);
    SwapBigEndianFloat(v->adsSwayHorizScale);
    SwapBigEndianFloat(v->adsSwayVertScale);
    SWAP_BE_MEMBER(v, bRifleBullet);
    SWAP_BE_MEMBER(v, armorPiercing);
    SWAP_BE_MEMBER(v, bBoltAction);
    SWAP_BE_MEMBER(v, aimDownSight);
    SWAP_BE_MEMBER(v, bRechamberWhileAds);
    SwapBigEndianFloat(v->adsViewErrorMin);
    SwapBigEndianFloat(v->adsViewErrorMax);
    SWAP_BE_MEMBER(v, bCookOffHold);
    SWAP_BE_MEMBER(v, bClipOnly);
    SWAP_BE_MEMBER(v, adsFireOnly);
    SWAP_BE_MEMBER(v, cancelAutoHolsterWhenEmpty);
    SWAP_BE_MEMBER(v, suppressAmmoReserveDisplay);
    SWAP_BE_MEMBER(v, enhanced);
    SWAP_BE_MEMBER(v, laserSightDuringNightvision);
    SwapBigEndianPtr32(v->killIcon);
    SWAP_BE_ENUM(v, killIconRatio, IW3Xenon::weaponIconRatioType_t);
    SWAP_BE_MEMBER(v, flipKillIcon);
    SwapBigEndianPtr32(v->dpadIcon);
    SWAP_BE_ENUM(v, dpadIconRatio, IW3Xenon::weaponIconRatioType_t);
    SWAP_BE_MEMBER(v, bNoPartialReload);
    SWAP_BE_MEMBER(v, bSegmentedReload);
    SWAP_BE_MEMBER(v, iReloadAmmoAdd);
    SWAP_BE_MEMBER(v, iReloadStartAdd);
    SwapBigEndianPtr32(v->szAltWeaponName);
    SWAP_BE_MEMBER(v, altWeaponIndex);
    SWAP_BE_MEMBER(v, iDropAmmoMin);
    SWAP_BE_MEMBER(v, iDropAmmoMax);
    SWAP_BE_MEMBER(v, blocksProne);
    SWAP_BE_MEMBER(v, silenced);
    SWAP_BE_MEMBER(v, iExplosionRadius);
    SWAP_BE_MEMBER(v, iExplosionRadiusMin);
    SWAP_BE_MEMBER(v, iExplosionInnerDamage);
    SWAP_BE_MEMBER(v, iExplosionOuterDamage);
    SwapBigEndianFloat(v->damageConeAngle);
    SWAP_BE_MEMBER(v, iProjectileSpeed);
    SWAP_BE_MEMBER(v, iProjectileSpeedUp);
    SWAP_BE_MEMBER(v, iProjectileSpeedForward);
    SWAP_BE_MEMBER(v, iProjectileActivateDist);
    SwapBigEndianFloat(v->projLifetime);
    SwapBigEndianFloat(v->timeToAccelerate);
    SwapBigEndianFloat(v->projectileCurvature);
    SwapBigEndianPtr32(v->projectileModel);
    SWAP_BE_ENUM(v, projExplosion, IW3Xenon::weapProjExposion_t);
    SwapBigEndianPtr32(v->projExplosionEffect);
    SWAP_BE_MEMBER(v, projExplosionEffectForceNormalUp);
    SwapBigEndianPtr32(v->projDudEffect);
    SwapBigEndianPtr32(v->projExplosionSound.name);
    SwapBigEndianPtr32(v->projDudSound.name);
    SWAP_BE_MEMBER(v, bProjImpactExplode);
    SWAP_BE_ENUM(v, stickiness, IW3Xenon::WeapStickinessType);
    SWAP_BE_MEMBER(v, hasDetonator);
    SWAP_BE_MEMBER(v, timedDetonation);
    SWAP_BE_MEMBER(v, rotate);
    SWAP_BE_MEMBER(v, holdButtonToThrow);
    SWAP_BE_MEMBER(v, freezeMovementWhenFiring);
    SwapBigEndianFloat(v->lowAmmoWarningThreshold);
    for (int i = 0; i < 29; i++)
        SwapBigEndianFloat(v->parallelBounce[i]);
    for (int i = 0; i < 29; i++)
        SwapBigEndianFloat(v->perpendicularBounce[i]);
    SwapBigEndianPtr32(v->projTrailEffect);
    for (int i = 0; i < 3; i++)
        SwapBigEndianFloat(v->vProjectileColor[i]);
    SWAP_BE_ENUM(v, guidedMissileType, IW3Xenon::guidedMissileType_t);
    SwapBigEndianFloat(v->maxSteeringAccel);
    SWAP_BE_MEMBER(v, projIgnitionDelay);
    SwapBigEndianPtr32(v->projIgnitionEffect);
    SwapBigEndianPtr32(v->projIgnitionSound.name);
    SwapBigEndianFloat(v->fAdsAimPitch);
    SwapBigEndianFloat(v->fAdsCrosshairInFrac);
    SwapBigEndianFloat(v->fAdsCrosshairOutFrac);
    SWAP_BE_MEMBER(v, adsGunKickReducedKickBullets);
    SwapBigEndianFloat(v->adsGunKickReducedKickPercent);
    SwapBigEndianFloat(v->fAdsGunKickPitchMin);
    SwapBigEndianFloat(v->fAdsGunKickPitchMax);
    SwapBigEndianFloat(v->fAdsGunKickYawMin);
    SwapBigEndianFloat(v->fAdsGunKickYawMax);
    SwapBigEndianFloat(v->fAdsGunKickAccel);
    SwapBigEndianFloat(v->fAdsGunKickSpeedMax);
    SwapBigEndianFloat(v->fAdsGunKickSpeedDecay);
    SwapBigEndianFloat(v->fAdsGunKickStaticDecay);
    SwapBigEndianFloat(v->fAdsViewKickPitchMin);
    SwapBigEndianFloat(v->fAdsViewKickPitchMax);
    SwapBigEndianFloat(v->fAdsViewKickYawMin);
    SwapBigEndianFloat(v->fAdsViewKickYawMax);
    SwapBigEndianFloat(v->fAdsViewKickCenterSpeed);
    SwapBigEndianFloat(v->fAdsViewScatterMin);
    SwapBigEndianFloat(v->fAdsViewScatterMax);
    SwapBigEndianFloat(v->fAdsSpread);
    SWAP_BE_MEMBER(v, hipGunKickReducedKickBullets);
    SwapBigEndianFloat(v->hipGunKickReducedKickPercent);
    SwapBigEndianFloat(v->fHipGunKickPitchMin);
    SwapBigEndianFloat(v->fHipGunKickPitchMax);
    SwapBigEndianFloat(v->fHipGunKickYawMin);
    SwapBigEndianFloat(v->fHipGunKickYawMax);
    SwapBigEndianFloat(v->fHipGunKickAccel);
    SwapBigEndianFloat(v->fHipGunKickSpeedMax);
    SwapBigEndianFloat(v->fHipGunKickSpeedDecay);
    SwapBigEndianFloat(v->fHipGunKickStaticDecay);
    SwapBigEndianFloat(v->fHipViewKickPitchMin);
    SwapBigEndianFloat(v->fHipViewKickPitchMax);
    SwapBigEndianFloat(v->fHipViewKickYawMin);
    SwapBigEndianFloat(v->fHipViewKickYawMax);
    SwapBigEndianFloat(v->fHipViewKickCenterSpeed);
    SwapBigEndianFloat(v->fHipViewScatterMin);
    SwapBigEndianFloat(v->fHipViewScatterMax);
    SwapBigEndianFloat(v->fightDist);
    SwapBigEndianFloat(v->maxDist);
    SwapBigEndianPtr32(v->aiVsAiAccuracyGraphName);
    SwapBigEndianPtr32(v->aiVsPlayerAccuracyGraphName);
    SwapBigEndianPtr32(v->aiVsAiAccuracyGraphKnots);
    SwapBigEndianPtr32(v->aiVsPlayerAccuracyGraphKnots);
    SwapBigEndianPtr32(v->originalAiVsAiAccuracyGraphKnots);
    SwapBigEndianPtr32(v->originalAiVsPlayerAccuracyGraphKnots);
    SWAP_BE_MEMBER(v, aiVsAiAccuracyGraphKnotCount);
    SWAP_BE_MEMBER(v, aiVsPlayerAccuracyGraphKnotCount);
    SWAP_BE_MEMBER(v, originalAiVsAiAccuracyGraphKnotCount);
    SWAP_BE_MEMBER(v, originalAiVsPlayerAccuracyGraphKnotCount);
    SWAP_BE_MEMBER(v, iPositionReloadTransTime);
    SwapBigEndianFloat(v->leftArc);
    SwapBigEndianFloat(v->rightArc);
    SwapBigEndianFloat(v->topArc);
    SwapBigEndianFloat(v->bottomArc);
    SwapBigEndianFloat(v->accuracy);
    SwapBigEndianFloat(v->aiSpread);
    SwapBigEndianFloat(v->playerSpread);
    for (int i = 0; i < 2; i++)
        SwapBigEndianFloat(v->minTurnSpeed[i]);
    for (int i = 0; i < 2; i++)
        SwapBigEndianFloat(v->maxTurnSpeed[i]);
    SwapBigEndianFloat(v->pitchConvergenceTime);
    SwapBigEndianFloat(v->yawConvergenceTime);
    SwapBigEndianFloat(v->suppressTime);
    SwapBigEndianFloat(v->maxRange);
    SwapBigEndianFloat(v->fAnimHorRotateInc);
    SwapBigEndianFloat(v->fPlayerPositionDist);
    SwapBigEndianPtr32(v->szUseHintString);
    SwapBigEndianPtr32(v->dropHintString);
    SWAP_BE_MEMBER(v, iUseHintStringIndex);
    SWAP_BE_MEMBER(v, dropHintStringIndex);
    SwapBigEndianFloat(v->horizViewJitter);
    SwapBigEndianFloat(v->vertViewJitter);
    SwapBigEndianPtr32(v->szScript);
    for (int i = 0; i < 2; i++)
        SwapBigEndianFloat(v->fOOPosAnimLength[i]);
    SWAP_BE_MEMBER(v, minDamage);
    SWAP_BE_MEMBER(v, minPlayerDamage);
    SwapBigEndianFloat(v->fMaxDamageRange);
    SwapBigEndianFloat(v->fMinDamageRange);
    SwapBigEndianFloat(v->destabilizationRateTime);
    SwapBigEndianFloat(v->destabilizationCurvatureMax);
    SWAP_BE_MEMBER(v, destabilizeDistance);
    for (int i = 0; i < 19; i++)
        SwapBigEndianFloat(v->locationDamageMultipliers[i]);
    SwapBigEndianPtr32(v->fireRumble);
    SwapBigEndianPtr32(v->meleeImpactRumble);
    SwapBigEndianFloat(v->adsDofStart);
    SwapBigEndianFloat(v->adsDofEnd);
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
