#pragma once

#include "Game/IW3Xenon/XAssets/localizeentry/localizeentry_mark_db.h"
#include "Game/IW3Xenon/XAssets/materialtechniqueset/materialtechniqueset_mark_db.h"
#include "Game/IW3Xenon/XAssets/rawfile/rawfile_mark_db.h"
#include "Game/IW3Xenon/XAssets/stringtable/stringtable_mark_db.h"
#include "Utils/Endianness.h"

// The following is a total hack for now just testing

static inline void EndianFixup_LocalizeEntry(IW3Xenon::LocalizeEntry* v)
{
    return;
}

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

static inline void EndianFixup_RawFile(IW3Xenon::RawFile* v)
{
    v->len = endianness::FromBigEndian(v->len);
}

static inline void EndianFixup_StringTable(IW3Xenon::StringTable* v)
{
    v->columnCount = endianness::FromBigEndian(v->columnCount);
    v->rowCount = endianness::FromBigEndian(v->rowCount);
}
