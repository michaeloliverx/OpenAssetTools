#pragma once

#include "Game/IW3Xenon/XAssets/gfximage/gfximage_mark_db.h"
#include "Game/IW3Xenon/XAssets/localizeentry/localizeentry_mark_db.h"
#include "Game/IW3Xenon/XAssets/material/material_mark_db.h"
#include "Game/IW3Xenon/XAssets/materialtechniqueset/materialtechniqueset_mark_db.h"
#include "Game/IW3Xenon/XAssets/rawfile/rawfile_mark_db.h"
#include "Game/IW3Xenon/XAssets/stringtable/stringtable_mark_db.h"
#include "Utils/Endianness.h"

// The following is a total hack for now just testing

// ---- Material

static inline void EndianFixup_MaterialTextureDef(IW3Xenon::MaterialTextureDef* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    v->nameHash = endianness::FromBigEndian(v->nameHash);
    swap_ptr32(v->u.image);
}

static inline void EndianFixup_MaterialInfo(IW3Xenon::MaterialInfo* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // name: XString handle (zone pointer)
    swap_ptr32(v->name);

    // gameFlags, sortKey, textureAtlasRowCount, textureAtlasColumnCount: all uint8, no swap needed

    // drawSurf: union with uint64 packed field
    v->drawSurf.packed = endianness::FromBigEndian(v->drawSurf.packed);

    // surfaceTypeBits: uint32
    v->surfaceTypeBits = endianness::FromBigEndian(v->surfaceTypeBits);
}

static inline void EndianFixup_MaterialTextureDefInfo(IW3Xenon::MaterialTextureDefInfo* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // Union: could be either image or water pointer
    // Both are pointers, so we swap as a generic 32-bit pointer
    swap_ptr32(v->image);
}

static inline void EndianFixup_water_t(IW3Xenon::water_t* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    auto swap_float = [](float& f)
    {
        std::uint32_t tmp;
        std::memcpy(&tmp, &f, sizeof(float));
        tmp = endianness::FromBigEndian(tmp);
        std::memcpy(&f, &tmp, sizeof(float));
    };

    // writable.floatTime: float (32-bit), needs swap
    swap_float(v->writable.floatTime);

    // Pointer fields
    swap_ptr32(v->H0X);
    swap_ptr32(v->H0Y);
    swap_ptr32(v->wTerm);

    // Integer fields
    v->M = endianness::FromBigEndian(v->M);
    v->N = endianness::FromBigEndian(v->N);

    // Float fields
    swap_float(v->Lx);
    swap_float(v->Lz);
    swap_float(v->gravity);
    swap_float(v->windvel);
    swap_float(v->winddir[0]);
    swap_float(v->winddir[1]);
    swap_float(v->amplitude);

    // Float array
    for (int i = 0; i < 4; i++)
        swap_float(v->codeConstant[i]);

    // Image pointer
    swap_ptr32(v->image);
}

static inline void EndianFixup_Material(IW3Xenon::Material* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // info: MaterialInfo struct - handle via its fixup function
    EndianFixup_MaterialInfo(&v->info);

    // stateBitsEntry[26]: array of uint8, no swap needed
    // textureCount, constantCount, stateBitsCount, stateFlags, cameraRegion: all uint8, no swap needed

    // Pointer fields
    swap_ptr32(v->techniqueSet);
    swap_ptr32(v->textureTable);
    swap_ptr32(v->constantTable);
    swap_ptr32(v->stateBitsTable);
}

// ---- MaterialTechniqueSet

static inline void EndianFixup_MaterialTechniqueSet(IW3Xenon::MaterialTechniqueSet* v)
{
    // name: zone pointer (XString / offset / following)
    {
        auto tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(v->name));
        tmp = endianness::FromBigEndian(tmp);
        v->name = reinterpret_cast<const char*>(static_cast<std::uintptr_t>(tmp));
    }

    // remappedTechniqueSet: zone pointer
    {
        auto tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(v->remappedTechniqueSet));
        tmp = endianness::FromBigEndian(tmp);
        v->remappedTechniqueSet = reinterpret_cast<IW3Xenon::MaterialTechniqueSet*>(static_cast<std::uintptr_t>(tmp));
    }

    // techniques[26]: array of zone pointers
    for (int i = 0; i < 26; i++)
    {
        auto tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(v->techniques[i]));
        tmp = endianness::FromBigEndian(tmp);
        v->techniques[i] = reinterpret_cast<IW3Xenon::MaterialTechnique*>(static_cast<std::uintptr_t>(tmp));
    }

    // worldVertFormat + unused[] are uint8, no swap needed
}

static inline void EndianFixup_MaterialVertexShader(IW3Xenon::MaterialVertexShader* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    swap_ptr32(v->name);

    auto& ld = v->prog.loadDef;
    swap_ptr32(ld.cachedPart);
    swap_ptr32(ld.physicalPart);
    ld.cachedPartSize = endianness::FromBigEndian(ld.cachedPartSize);
    ld.physicalPartSize = endianness::FromBigEndian(ld.physicalPartSize);
}

static inline void EndianFixup_MaterialVertexShaderProgram(IW3Xenon::MaterialVertexShaderProgram* v)
{
    // Embedded loadDef is handled separately to avoid double-swapping
    return;
}

static inline void EndianFixup_GfxVertexShaderLoadDef(IW3Xenon::GfxVertexShaderLoadDef* v)
{
    v->cachedPartSize = endianness::FromBigEndian(v->cachedPartSize);
    v->physicalPartSize = endianness::FromBigEndian(v->physicalPartSize);
}

static inline void EndianFixup_MaterialPixelShader(IW3Xenon::MaterialPixelShader* v)
{
    // Swap a 32-bit zone pointer/handle in-place
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // name: XString handle (FOLLOWING / OFFSET / etc.)
    swap_ptr32(v->name);

    // prog: union storage from the file is treated as loadDef for zone loading
    auto& ld = v->prog.loadDef;

    swap_ptr32(ld.cachedPart);
    swap_ptr32(ld.physicalPart);

    ld.cachedPartSize = endianness::FromBigEndian(ld.cachedPartSize);
    ld.physicalPartSize = endianness::FromBigEndian(ld.physicalPartSize);
}

static inline void EndianFixup_MaterialPixelShaderProgram(IW3Xenon::MaterialPixelShaderProgram* v)
{
    // Embedded loadDef is handled separately to avoid double-swapping
    return;
}

static inline void EndianFixup_GfxPixelShaderLoadDef(IW3Xenon::GfxPixelShaderLoadDef* v)
{
    v->cachedPartSize = endianness::FromBigEndian(v->cachedPartSize);
    v->physicalPartSize = endianness::FromBigEndian(v->physicalPartSize);
}

static inline void EndianFixup_MaterialShaderArgument(IW3Xenon::MaterialShaderArgument* v)
{
    v->type = endianness::FromBigEndian(v->type);
    v->dest = endianness::FromBigEndian(v->dest);

    // The union field needs to be swapped based on type, but we do a generic swap
    // treating it as the largest field (uint32) which covers all union members
    v->u.nameHash = endianness::FromBigEndian(v->u.nameHash);
}

static inline void EndianFixup_MaterialArgumentDef(IW3Xenon::MaterialArgumentDef* v)
{
    // This union is always part of MaterialShaderArgument, so endian fixup
    // is handled there. This function exists for API compatibility but does nothing.
    return;
}

static inline void EndianFixup_MaterialPass(IW3Xenon::MaterialPass* v)
{
    // Helper: swap a 32-bit zone pointer handle in-place
    auto swap_ptr32 = [](auto& p)
    {
        auto tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    swap_ptr32(v->vertexDecl);

    for (int i = 0; i < 15; i++)
        swap_ptr32(v->vertexShaderArray[i]);

    swap_ptr32(v->vertexShader);
    swap_ptr32(v->pixelShader);
    swap_ptr32(v->args);

    // uint8 fields: no swap
}

static inline void EndianFixup_MaterialTechnique_Partial(IW3Xenon::MaterialTechnique* v, size_t loaded_size)
{
    // TODO: not dynamic at the moment as we know the only callsite

    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    swap_ptr32(v->name);
    v->flags = endianness::FromBigEndian(v->flags);
    v->passCount = endianness::FromBigEndian(v->passCount);
}

// ---- GfxImage

static inline void EndianFixup_GfxTexture(IW3Xenon::GfxTexture* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // Union with loadDef pointer
    swap_ptr32(v->loadDef);
}

static inline void EndianFixup_GfxImageLoadDef(IW3Xenon::GfxImageLoadDef* v)
{
    // levelCount: uint8, no swap needed
    // flags: uint8, no swap needed

    // dimensions[3]: array of int16
    v->dimensions[0] = endianness::FromBigEndian(v->dimensions[0]);
    v->dimensions[1] = endianness::FromBigEndian(v->dimensions[1]);
    v->dimensions[2] = endianness::FromBigEndian(v->dimensions[2]);

    // format: int (32-bit)
    v->format = endianness::FromBigEndian(v->format);

    // texture: GfxTexture union - handle via its fixup function
    EndianFixup_GfxTexture(&v->texture);
}

static inline void EndianFixup_GfxImage(IW3Xenon::GfxImage* v)
{
    auto swap_ptr32 = [](auto& p)
    {
        std::uint32_t tmp = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p));
        tmp = endianness::FromBigEndian(tmp);
        p = reinterpret_cast<std::remove_reference_t<decltype(p)>>(static_cast<std::uintptr_t>(tmp));
    };

    // mapType: enum (int, 32-bit)
    v->mapType = static_cast<IW3Xenon::MapType>(endianness::FromBigEndian(static_cast<int>(v->mapType)));

    // texture: GfxTexture union
    swap_ptr32(v->texture.loadDef);

    // semantic: uint8, no swap needed

    // cardMemory.platform[1]: array of int (32-bit)
    v->cardMemory.platform[0] = endianness::FromBigEndian(v->cardMemory.platform[0]);

    // width, height, depth: uint16
    v->width = endianness::FromBigEndian(v->width);
    v->height = endianness::FromBigEndian(v->height);
    v->depth = endianness::FromBigEndian(v->depth);

    // category: uint8, no swap needed
    // delayLoadPixels: bool, no swap needed

    // pixels: pointer
    swap_ptr32(v->pixels);

    // baseSize: uint (32-bit)
    v->baseSize = endianness::FromBigEndian(v->baseSize);

    // streamSlot: uint16
    v->streamSlot = endianness::FromBigEndian(v->streamSlot);

    // streaming: bool, no swap needed

    // name: pointer
    swap_ptr32(v->name);
}

// ---- LocalizeEntry

static inline void EndianFixup_LocalizeEntry(IW3Xenon::LocalizeEntry* v)
{
    return;
}

// ---- RawFile

static inline void EndianFixup_RawFile(IW3Xenon::RawFile* v)
{
    v->len = endianness::FromBigEndian(v->len);
}

// ---- StringTable

static inline void EndianFixup_StringTable(IW3Xenon::StringTable* v)
{
    v->columnCount = endianness::FromBigEndian(v->columnCount);
    v->rowCount = endianness::FromBigEndian(v->rowCount);
}
