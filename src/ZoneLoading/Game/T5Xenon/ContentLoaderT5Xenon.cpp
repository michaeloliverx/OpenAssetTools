#include "ContentLoaderT5Xenon.h"

#include "Game/T5Xenon/AssetEndianSwapT5Xenon.h"
#include "Game/T5Xenon/AssetLoaderT5Xenon.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "Loading/Exception/UnsupportedAssetTypeException.h"
#include "Utils/Endianness.h"

#include <cassert>

using namespace T5Xenon;

ContentLoader::ContentLoader(Zone& zone, ZoneInputStream& stream)
    : ContentLoaderBase(zone, stream),
      varXAssetList(nullptr),
      varXAsset(nullptr),
      varScriptStringList(nullptr)
{
}

void ContentLoader::LoadScriptStringList(const bool atStreamStart)
{
    assert(!atStreamStart);

    if (varScriptStringList->strings != nullptr)
    {
        assert(GetZonePointerType(varScriptStringList->strings) == ZonePointerType::FOLLOWING);

        varScriptStringList->strings = m_stream.Alloc<const char*>(4);
        varXString = varScriptStringList->strings;
        LoadXStringArray(true, varScriptStringList->count);

        if (varScriptStringList->strings && varScriptStringList->count > 0)
            m_zone.m_script_strings.InitializeForExistingZone(varScriptStringList->strings, static_cast<size_t>(varScriptStringList->count));
    }

    assert(m_zone.m_script_strings.Count() <= SCR_STRING_MAX + 1);
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
        LOAD_ASSET(ASSET_TYPE_PHYSPRESET, PhysPreset, physPreset)
        LOAD_ASSET(ASSET_TYPE_PHYSCONSTRAINTS, PhysConstraints, physConstraints)
        LOAD_ASSET(ASSET_TYPE_DESTRUCTIBLEDEF, DestructibleDef, destructibleDef)
        LOAD_ASSET(ASSET_TYPE_XANIMPARTS, XAnimParts, parts)
        LOAD_ASSET(ASSET_TYPE_XMODEL, XModel, model)
        LOAD_ASSET(ASSET_TYPE_MATERIAL, Material, material)
        LOAD_ASSET(ASSET_TYPE_PIXEL_SHADER, MaterialPixelShader, pixelShader)
        LOAD_ASSET(ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet, techniqueSet)
        LOAD_ASSET(ASSET_TYPE_IMAGE, GfxImage, image)
        LOAD_ASSET(ASSET_TYPE_SOUND, SndBank, sound)
        LOAD_ASSET(ASSET_TYPE_SOUND_PATCH, SndPatch, soundPatch)
        LOAD_ASSET(ASSET_TYPE_CLIPMAP_MP, clipMap_t, clipMap)
        LOAD_ASSET(ASSET_TYPE_COMWORLD, ComWorld, comWorld)
        LOAD_ASSET(ASSET_TYPE_GAMEWORLD_SP, GameWorldSp, gameWorldSp)
        LOAD_ASSET(ASSET_TYPE_GAMEWORLD_MP, GameWorldMp, gameWorldMp)
        LOAD_ASSET(ASSET_TYPE_MAP_ENTS, MapEnts, mapEnts)
        LOAD_ASSET(ASSET_TYPE_GFXWORLD, GfxWorld, gfxWorld)
        LOAD_ASSET(ASSET_TYPE_LIGHT_DEF, GfxLightDef, lightDef)
        LOAD_ASSET(ASSET_TYPE_FONT, Font_s, font)
        LOAD_ASSET(ASSET_TYPE_MENULIST, MenuList, menuList)
        LOAD_ASSET(ASSET_TYPE_MENU, menuDef_t, menu)
        LOAD_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry, localize)
        LOAD_ASSET(ASSET_TYPE_WEAPON, WeaponVariantDef, weapon)
        LOAD_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals, sndDriverGlobals)
        LOAD_ASSET(ASSET_TYPE_FX, FxEffectDef, fx)
        LOAD_ASSET(ASSET_TYPE_IMPACT_FX, FxImpactTable, impactFx)
        LOAD_ASSET(ASSET_TYPE_RAWFILE, RawFile, rawfile)
        LOAD_ASSET(ASSET_TYPE_STRINGTABLE, StringTable, stringTable)
        LOAD_ASSET(ASSET_TYPE_PACK_INDEX, PackIndex, packIndex)
        LOAD_ASSET(ASSET_TYPE_XGLOBALS, XGlobals, xGlobals)
        LOAD_ASSET(ASSET_TYPE_TEXTURE_LIST, TextureList, textureList)
        LOAD_ASSET(ASSET_TYPE_DDL, ddlRoot_t, ddlRoot)
        LOAD_ASSET(ASSET_TYPE_GLASSES, Glasses, glasses)
        LOAD_ASSET(ASSET_TYPE_EMBLEMSET, EmblemSet, emblemSet)
        LOAD_ASSET(ASSET_TYPE_CLIPMAP_SP, clipMap_t, clipMap)
    case ASSET_TYPE_XMODELPIECES:
    case ASSET_TYPE_UI_MAP:
    case ASSET_TYPE_WEAPONDEF:
    case ASSET_TYPE_WEAPON_VARIANT:
    case ASSET_TYPE_AITYPE:
    case ASSET_TYPE_MPTYPE:
    case ASSET_TYPE_MPBODY:
    case ASSET_TYPE_MPHEAD:
    case ASSET_TYPE_CHARACTER:
    case ASSET_TYPE_XMODELALIAS:
        break;

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

void ContentLoader::LoadDelayStream()
{
    for (const auto& delay : m_stream.GetDelayedLoads())
    {
        m_stream.LoadDataRaw(delay.dst, delay.size);
    }
}

void ContentLoader::Load()
{
    XAssetList assetList{};
    varXAssetList = &assetList;

    m_stream.LoadDataRaw(&assetList, sizeof(assetList));

    EndianSwap(varXAssetList->stringList.count);
    EndianSwap(varXAssetList->assetCount);

    m_stream.PushBlock(XFILE_BLOCK_VIRTUAL);

    varScriptStringList = &assetList.stringList;
    LoadScriptStringList(false);

    if (assetList.assets != nullptr)
    {
        assert(GetZonePointerType(assetList.assets) == ZonePointerType::FOLLOWING);

        assetList.assets = m_stream.Alloc<XAsset>(4);
        varXAsset = assetList.assets;
        LoadXAssetArray(true, assetList.assetCount);
    }

    m_stream.PopBlock();

    LoadDelayStream();
}
