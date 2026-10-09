#include "ContentWriterIW3Xenon.h"

#include "Game/IW3Xenon/AssetEndianSwapIW3Xenon.h"
#include "Game/IW3Xenon/AssetWriterIW3Xenon.h"
#include "Writing/WritingException.h"

#include <cassert>
#include <format>

using namespace IW3Xenon;

ContentWriter::ContentWriter(const Zone& zone)
    : ContentWriterBase(zone),
      varXAssetList(nullptr),
      varXAsset(nullptr),
      varScriptStringList(nullptr)
{
}

void ContentWriter::CreateXAssetList(XAssetList& xAssetList, MemoryManager& memory) const
{
    if (!m_zone.m_script_strings.Empty())
    {
        assert(m_zone.m_script_strings.Count() <= SCR_STRING_MAX + 1);
        xAssetList.stringList.count = static_cast<int>(m_zone.m_script_strings.Count());
        xAssetList.stringList.strings = memory.Alloc<const char*>(m_zone.m_script_strings.Count());

        for (auto i = 0u; i < m_zone.m_script_strings.Count(); i++)
            xAssetList.stringList.strings[i] = m_zone.m_script_strings.CValue(i);
    }

    const auto assetCount = m_zone.m_pools.GetTotalAssetCount();
    if (assetCount > 0)
    {
        xAssetList.assetCount = static_cast<int>(assetCount);
        xAssetList.assets = memory.Alloc<XAsset>(assetCount);

        auto index = 0u;
        for (auto i = m_zone.m_pools.begin(); i != m_zone.m_pools.end(); ++i)
        {
            auto& asset = xAssetList.assets[index++];
            asset.type = static_cast<XAssetType>((*i)->m_type);
            asset.header.data = (*i)->m_ptr;
        }
    }
}

void ContentWriter::WriteScriptStringList(const bool atStreamStart)
{
    assert(!atStreamStart);

    if (varScriptStringList->strings != nullptr)
    {
        m_stream->Align(4);
        varXString = varScriptStringList->strings;
        WriteXStringArray(true, varScriptStringList->count);
        m_stream->MarkFollowing(varScriptStringListWritten.AtOffset(4));
    }
}

void ContentWriter::WriteXAsset(const bool atStreamStart)
{
#define WRITE_ASSET(type_index, typeName, headerEntry)                                                                                                         \
    case type_index:                                                                                                                                           \
    {                                                                                                                                                          \
        Writer_##typeName writer(varXAsset->header.headerEntry, m_zone, *m_stream);                                                                            \
        writer.Write(varXAsset->header.headerEntry, varXAssetWritten.AtOffset(4));                                                                             \
        break;                                                                                                                                                 \
    }

    assert(varXAsset != nullptr);

    if (atStreamStart)
    {
        varXAssetWritten = m_stream->Write(varXAsset);
        auto* writtenAsset = static_cast<XAsset*>(varXAssetWritten.Offset());
        EndianSwap(writtenAsset->type);
        EndianSwap(writtenAsset->header.data);
    }

    switch (varXAsset->type)
    {
        WRITE_ASSET(ASSET_TYPE_PHYSPRESET, PhysPreset, physPreset)
        WRITE_ASSET(ASSET_TYPE_XANIMPARTS, XAnimParts, parts)
        WRITE_ASSET(ASSET_TYPE_XMODEL, XModel, model)
        WRITE_ASSET(ASSET_TYPE_MATERIAL, Material, material)
        WRITE_ASSET(ASSET_TYPE_PIXELSHADER, MaterialPixelShader, pixelShader)
        WRITE_ASSET(ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet, techniqueSet)
        WRITE_ASSET(ASSET_TYPE_IMAGE, GfxImage, image)
        WRITE_ASSET(ASSET_TYPE_SOUND, snd_alias_list_t, sound)
        WRITE_ASSET(ASSET_TYPE_SOUND_CURVE, SndCurve, sndCurve)
        WRITE_ASSET(ASSET_TYPE_LOADED_SOUND, LoadedSound, loadSnd)
        WRITE_ASSET(ASSET_TYPE_CLIPMAP, clipMap_t, clipMap)
        WRITE_ASSET(ASSET_TYPE_CLIPMAP_PVS, clipMap_t, clipMap)
        WRITE_ASSET(ASSET_TYPE_COMWORLD, ComWorld, comWorld)
        WRITE_ASSET(ASSET_TYPE_GAMEWORLD_SP, GameWorldSp, gameWorldSp)
        WRITE_ASSET(ASSET_TYPE_GAMEWORLD_MP, GameWorldMp, gameWorldMp)
        WRITE_ASSET(ASSET_TYPE_MAP_ENTS, MapEnts, mapEnts)
        WRITE_ASSET(ASSET_TYPE_GFXWORLD, GfxWorld, gfxWorld)
        WRITE_ASSET(ASSET_TYPE_LIGHT_DEF, GfxLightDef, lightDef)
        WRITE_ASSET(ASSET_TYPE_FONT, Font_s, font)
        WRITE_ASSET(ASSET_TYPE_MENULIST, MenuList, menuList)
        WRITE_ASSET(ASSET_TYPE_MENU, menuDef_t, menu)
        WRITE_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry, localize)
        WRITE_ASSET(ASSET_TYPE_WEAPON, WeaponDef, weapon)
        WRITE_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals, sndDriverGlobals)
        WRITE_ASSET(ASSET_TYPE_FX, FxEffectDef, fx)
        WRITE_ASSET(ASSET_TYPE_IMPACT_FX, FxImpactTable, impactFx)
        WRITE_ASSET(ASSET_TYPE_RAWFILE, RawFile, rawfile)
        WRITE_ASSET(ASSET_TYPE_STRINGTABLE, StringTable, stringTable)

    default:
        throw WritingException(std::format("Unsupported asset type: {}.", static_cast<unsigned>(varXAsset->type)));
    }

#undef WRITE_ASSET
}

void ContentWriter::WriteXAssetArray(const bool atStreamStart, const size_t count)
{
    assert(varXAsset != nullptr);
    static_assert(sizeof(void*) != 4u || sizeof(XAsset) == 8u);

    if (atStreamStart)
    {
        varXAssetWritten = m_stream->Write(varXAsset, count);
        auto* writtenAssets = static_cast<XAsset*>(varXAssetWritten.Offset());
        for (size_t index = 0; index < count; index++)
        {
            EndianSwap(writtenAssets[index].type);
            EndianSwap(writtenAssets[index].header.data);
        }
    }

    for (size_t index = 0; index < count; index++)
    {
        WriteXAsset(false);
        varXAsset++;
        varXAssetWritten.Inc(sizeof(XAsset));
    }
}

void ContentWriter::WriteContent(ZoneOutputStream& stream)
{
    m_stream = &stream;

    MemoryManager memory;
    XAssetList assetList{};
    CreateXAssetList(assetList, memory);
    varXAssetList = &assetList;

    static_assert(sizeof(void*) != 4u || sizeof(XAssetList) == 16u);
    varXAssetListWritten = m_stream->WriteDataRaw(&assetList, sizeof(assetList));
    auto* writtenAssetList = static_cast<XAssetList*>(varXAssetListWritten.Offset());
    EndianSwap(writtenAssetList->stringList.count);
    EndianSwap(writtenAssetList->assetCount);

    m_stream->PushBlock(XFILE_BLOCK_VIRTUAL);

    varScriptStringList = &varXAssetList->stringList;
    varScriptStringListWritten = varXAssetListWritten;
    WriteScriptStringList(false);

    if (varXAssetList->assets != nullptr)
    {
        m_stream->Align(4);
        varXAsset = varXAssetList->assets;
        WriteXAssetArray(true, varXAssetList->assetCount);
        m_stream->MarkFollowing(varXAssetListWritten.AtOffset(12));
    }

    m_stream->PopBlock();
}
