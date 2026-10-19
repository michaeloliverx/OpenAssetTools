#include "ContentLoaderIW2Xenon.h"

#include "Game/IW2Xenon/AssetEndianSwapIW2Xenon.h"
#include "Game/IW2Xenon/AssetLoaderIW2Xenon.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "Loading/Exception/UnsupportedAssetTypeException.h"
#include "Utils/Endianness.h"

#include <cassert>

using namespace IW2Xenon;

ContentLoader::ContentLoader(Zone& zone, ZoneInputStream& stream)
    : ContentLoaderBase(zone, stream),
      varXAssetList(nullptr),
      varXAsset(nullptr)
{
}

void ContentLoader::LoadRawStringList(XStringList& list)
{
    if (!list.strings)
        return;

    list.strings = m_stream.Alloc<const char*>(4);
    m_stream.Load(list.strings, list.count);
    for (int i = 0; i < list.count; ++i)
    {
        auto& string = list.strings[i];
        EndianSwap(string);
        if (!string)
            continue;
        if (GetZonePointerType(string) == ZonePointerType::FOLLOWING)
        {
            string = m_stream.Alloc<const char>(1);
            m_stream.LoadNullTerminated(const_cast<char*>(string));
        }
        else
            string = m_stream.ConvertOffsetToAliasNative(string);
    }
}

void ContentLoader::LoadXAsset(const bool atStreamStart) const
{
#define LOAD_ASSET(type_index, typeName, headerEntry)                                                                                                          \
    case type_index:                                                                                                                                           \
    {                                                                                                                                                          \
        Loader_##typeName loader(m_zone, m_stream);                                                                                                            \
        loader.Load(&varXAsset->header.headerEntry);                                                                                                           \
        break;                                                                                                                                                 \
    }
    assert(varXAsset != nullptr);

    if (atStreamStart)
        m_stream.Load<XAsset>(varXAsset);

    switch (varXAsset->type)
    {
        LOAD_ASSET(ASSET_TYPE_XANIMPARTS, XAnimParts, xAnim)
        LOAD_ASSET(ASSET_TYPE_XMODEL, XModel, xModel)
        LOAD_ASSET(ASSET_TYPE_MATERIAL, Material, material)
        LOAD_ASSET(ASSET_TYPE_IMAGE, GfxImage, image)
        LOAD_ASSET(ASSET_TYPE_SOUND, snd_alias_list_t, sound)
        LOAD_ASSET(ASSET_TYPE_SND_CURVE, SndCurve, soundCurve)
    case ASSET_TYPE_CLIPMAP_SP:
        LOAD_ASSET(ASSET_TYPE_CLIPMAP_MP, clipMap_t, clipMapMp)
        LOAD_ASSET(ASSET_TYPE_GFXWORLD, GfxWorld, gfxWorld)
        LOAD_ASSET(ASSET_TYPE_LIGHT_DEF, GfxLightDef, lightDef)
        LOAD_ASSET(ASSET_TYPE_FONT, Font_s, font)
        LOAD_ASSET(ASSET_TYPE_MENUFILE, MenuList, menuList)
        LOAD_ASSET(ASSET_TYPE_MENU, menuDef_t, menu)
        LOAD_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry, localize)
        LOAD_ASSET(ASSET_TYPE_WEAPON, WeaponDef, weapon)
        LOAD_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals, soundDriverGlobals)
        LOAD_ASSET(ASSET_TYPE_FX, EffectTemplate, fx)
        LOAD_ASSET(ASSET_TYPE_IMPACT_FX, FxImpactTable, impactFx)
        LOAD_ASSET(ASSET_TYPE_RAWFILE, RawFile, rawFile)

    default:
    {
        throw UnsupportedAssetTypeException(varXAsset->type);
    }
    }

#undef LOAD_ASSET
}

void ContentLoader::LoadXAssetArray(const bool atStreamStart, const size_t count)
{
    assert(varXAsset != nullptr);

    if (atStreamStart)
    {
        m_stream.Load<XAsset>(varXAsset, count);
        for (size_t index = 0; index < count; index++)
        {
            EndianSwap(varXAsset[index].type);
            EndianSwap(varXAsset[index].header.data);
        }
    }

    for (size_t index = 0; index < count; index++)
    {
        LoadXAsset(false);
        varXAsset++;
    }
}

void ContentLoader::Load()
{
    m_stream.PushBlock(XFILE_BLOCK_TEMP);
    varXAssetList = m_stream.Alloc<XAssetList>(4);
    m_stream.Load(varXAssetList);
    EndianSwap(varXAssetList->rawStrings.count);
    EndianSwap(varXAssetList->tempStrings.count);
    EndianSwap(varXAssetList->assetCount);

    m_stream.PushBlock(XFILE_BLOCK_VIRTUAL);
    LoadRawStringList(varXAssetList->rawStrings);
    m_stream.PopBlock();
    LoadRawStringList(varXAssetList->tempStrings);
    m_zone.m_script_strings.InitializeForExistingZone(varXAssetList->tempStrings.strings, varXAssetList->tempStrings.count);
    m_stream.SetIndexedXStrings({varXAssetList->rawStrings.strings, static_cast<size_t>(varXAssetList->rawStrings.count)});

    m_stream.PushBlock(XFILE_BLOCK_VIRTUAL);
    if (varXAssetList->assets)
    {
        varXAssetList->assets = m_stream.Alloc<XAsset>(4);
        varXAsset = varXAssetList->assets;
        LoadXAssetArray(true, varXAssetList->assetCount);
    }
    m_stream.PopBlock();
    m_stream.PopBlock();
}
