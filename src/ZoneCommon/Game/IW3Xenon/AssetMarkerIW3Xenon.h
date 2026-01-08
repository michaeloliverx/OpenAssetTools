#pragma once

#include "Game/IW3Xenon/XAssets/font_s/font_s_mark_db.h"
#include "Game/IW3Xenon/XAssets/gfximage/gfximage_mark_db.h"
#include "Game/IW3Xenon/XAssets/gfxlightdef/gfxlightdef_mark_db.h"
#include "Game/IW3Xenon/XAssets/loadedsound/loadedsound_mark_db.h"
#include "Game/IW3Xenon/XAssets/localizeentry/localizeentry_mark_db.h"
#include "Game/IW3Xenon/XAssets/mapents/mapents_mark_db.h"
#include "Game/IW3Xenon/XAssets/material/material_mark_db.h"
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

static inline void EndianFixup_PhysPreset(IW3Xenon::PhysPreset* v) {}

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
}

static inline void EndianFixup_XAnimDynamicFrames(IW3Xenon::XAnimDynamicFrames* v) {}

static inline void EndianFixup_XAnimDeltaPartQuat_Partial(IW3Xenon::XAnimDeltaPartQuat* v, size_t loaded_size) {}

static inline void EndianFixup_XAnimDeltaPartQuatDataFrames_Partial(IW3Xenon::XAnimDeltaPartQuatDataFrames* v, size_t loaded_size) {}

static inline void EndianFixup_XAnimIndices(IW3Xenon::XAnimIndices* v) {}

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
    SwapBigEndianPtr32(v->dataShort);
    SwapBigEndianPtr32(v->dataInt);
    SwapBigEndianPtr32(v->randomDataShort);
    SwapBigEndianPtr32(v->randomDataByte);
    SwapBigEndianPtr32(v->randomDataInt);
    // indices: union chosen by numframes
    if (v->numframes < 256)
        SwapBigEndianPtr32(v->indices._1);
    else
        SwapBigEndianPtr32(v->indices._2);
    SwapBigEndianPtr32(v->notify);
    SwapBigEndianPtr32(v->deltaPart);
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
    // name: zone pointer (XString / offset / following)
    SwapBigEndianPtr32(v->name);

    // remappedTechniqueSet: zone pointer
    SwapBigEndianPtr32(v->remappedTechniqueSet);

    // techniques[26]: array of zone pointers
    for (int i = 0; i < 26; i++)
        SwapBigEndianPtr32(v->techniques[i]);

    // worldVertFormat + unused[] are uint8, no swap needed
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
    return;
}

static inline void EndianFixup_GfxVertexShaderLoadDef(IW3Xenon::GfxVertexShaderLoadDef* v)
{
    SWAP_BE_MEMBER(v, cachedPartSize);
    SWAP_BE_MEMBER(v, physicalPartSize);
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
    // Embedded loadDef is handled separately to avoid double-swapping
    return;
}

static inline void EndianFixup_GfxPixelShaderLoadDef(IW3Xenon::GfxPixelShaderLoadDef* v)
{
    SWAP_BE_MEMBER(v, cachedPartSize);
    SWAP_BE_MEMBER(v, physicalPartSize);
}

static inline void EndianFixup_MaterialShaderArgument(IW3Xenon::MaterialShaderArgument* v)
{
    SWAP_BE_MEMBER(v, type);
    SWAP_BE_MEMBER(v, dest);

    // The union field needs to be swapped based on type, but we do a generic swap
    // treating it as the largest field (uint32) which covers all union members
    SWAP_BE_MEMBER(v, u.nameHash);
}

static inline void EndianFixup_MaterialArgumentDef(IW3Xenon::MaterialArgumentDef* v)
{
    // This union is always part of MaterialShaderArgument, so endian fixup
    // is handled there. This function exists for API compatibility but does nothing.
    return;
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
    SwapBigEndianPtr32(v->loadDef);
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

static inline void EndianFixup_snd_alias_t(IW3Xenon::snd_alias_t* v) {}

static inline void EndianFixup_XAUDIOCHANNELMAP(IW3Xenon::XAUDIOCHANNELMAP* v) {}

static inline void EndianFixup_SoundFile(IW3Xenon::SoundFile* v) {}

static inline void EndianFixup_SoundFileRef(IW3Xenon::SoundFileRef* v) {}

static inline void EndianFixup_StreamedSound(IW3Xenon::StreamedSound* v) {}

static inline void EndianFixup_StreamFileName(IW3Xenon::StreamFileName* v) {}

static inline void EndianFixup_StreamFileInfo(IW3Xenon::StreamFileInfo* v) {}

static inline void EndianFixup_StreamFileNameRaw(IW3Xenon::StreamFileNameRaw* v) {}

static inline void EndianFixup_SpeakerMap(IW3Xenon::SpeakerMap* v) {}

static inline void EndianFixup_snd_alias_list_t(IW3Xenon::snd_alias_list_t* v) {}

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

static inline void EndianFixup_XaSound(IW3Xenon::XaSound* v) {}

static inline void EndianFixup_XAUDIOPACKET_ALIGNED(IW3Xenon::XAUDIOPACKET_ALIGNED* v) {}

static inline void EndianFixup_XaSeekTable(IW3Xenon::XaSeekTable* v) {}

static inline void EndianFixup_LoadedSound(IW3Xenon::LoadedSound* v) {}

// ---- MapEnts

static inline void EndianFixup_MapEnts(IW3Xenon::MapEnts* v) {}

// ---- GfxLightDef
static inline void EndianFixup_GfxLightImage(IW3Xenon::GfxLightImage* v) {}

static inline void EndianFixup_GfxLightDef(IW3Xenon::GfxLightDef* v) {}

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

static inline void EndianFixup_ItemKeyHandler(IW3Xenon::ItemKeyHandler* v) {}

static inline void EndianFixup_statement_s(IW3Xenon::statement_s* v) {}

static inline void EndianFixup_operandInternalDataUnion(IW3Xenon::operandInternalDataUnion* v)
{
    // Union - contents swapped based on context in Operand
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

static inline void EndianFixup_itemDefData_t(IW3Xenon::itemDefData_t* v) {}

static inline void EndianFixup_listBoxDef_s(IW3Xenon::listBoxDef_s* v) {}

static inline void EndianFixup_multiDef_s(IW3Xenon::multiDef_s* v) {}

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

static inline void EndianFixup_SndDriverGlobals(IW3Xenon::SndDriverGlobals* v) {}

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
    SWAP_BE_MEMBER(v, columnCount);
    SWAP_BE_MEMBER(v, rowCount);
}
