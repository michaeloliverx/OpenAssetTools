#include "ContentLoaderIW3Xenon.h"

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Game/IW3Xenon/XAssets/comworld/comworld_load_db.h"
#include "Game/IW3Xenon/XAssets/font_s/font_s_load_db.h"
#include "Game/IW3Xenon/XAssets/fxeffectdef/fxeffectdef_load_db.h"
#include "Game/IW3Xenon/XAssets/fximpacttable/fximpacttable_load_db.h"
#include "Game/IW3Xenon/XAssets/gfximage/gfximage_load_db.h"
#include "Game/IW3Xenon/XAssets/gfxlightdef/gfxlightdef_load_db.h"
#include "Game/IW3Xenon/XAssets/gfxworld/gfxworld_load_db.h"
#include "Game/IW3Xenon/XAssets/loadedsound/loadedsound_load_db.h"
#include "Game/IW3Xenon/XAssets/localizeentry/localizeentry_load_db.h"
#include "Game/IW3Xenon/XAssets/mapents/mapents_load_db.h"
#include "Game/IW3Xenon/XAssets/material/material_load_db.h"
#include "Game/IW3Xenon/XAssets/materialtechniqueset/materialtechniqueset_load_db.h"
#include "Game/IW3Xenon/XAssets/menudef_t/menudef_t_load_db.h"
#include "Game/IW3Xenon/XAssets/menulist/menulist_load_db.h"
#include "Game/IW3Xenon/XAssets/physpreset/physpreset_load_db.h"
#include "Game/IW3Xenon/XAssets/rawfile/rawfile_load_db.h"
#include "Game/IW3Xenon/XAssets/snd_alias_list_t/snd_alias_list_t_load_db.h"
#include "Game/IW3Xenon/XAssets/sndcurve/sndcurve_load_db.h"
#include "Game/IW3Xenon/XAssets/snddriverglobals/snddriverglobals_load_db.h"
#include "Game/IW3Xenon/XAssets/stringtable/stringtable_load_db.h"
#include "Game/IW3Xenon/XAssets/weapondef/weapondef_load_db.h"
#include "Game/IW3Xenon/XAssets/xanimparts/xanimparts_load_db.h"
#include "Game/IW3Xenon/XAssets/xmodel/xmodel_load_db.h"
#include "Loading/Exception/UnsupportedAssetTypeException.h"
#include "Utils/Endianness.h"

#include <cassert>

using namespace IW3Xenon;

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
#define SKIP_ASSET(type_index, typeName, headerEntry)                                                                                                          \
    case type_index:                                                                                                                                           \
        break;

    assert(varXAsset != nullptr);

    if (atStreamStart)
        m_stream.Load<XAsset>(varXAsset);

    switch (varXAsset->type)
    {
        LOAD_ASSET(ASSET_TYPE_PHYSPRESET, PhysPreset, physPreset)
        LOAD_ASSET(ASSET_TYPE_XMODEL, XModel, model)
        LOAD_ASSET(ASSET_TYPE_XANIMPARTS, XAnimParts, parts)
        LOAD_ASSET(ASSET_TYPE_MATERIAL, Material, material)
        LOAD_ASSET(ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet, techniqueSet)
        LOAD_ASSET(ASSET_TYPE_IMAGE, GfxImage, image)
        LOAD_ASSET(ASSET_TYPE_SOUND, snd_alias_list_t, sound)
        LOAD_ASSET(ASSET_TYPE_SOUND_CURVE, SndCurve, sndCurve)
        LOAD_ASSET(ASSET_TYPE_LOADED_SOUND, LoadedSound, loadSnd)
        LOAD_ASSET(ASSET_TYPE_COMWORLD, ComWorld, comWorld)
        LOAD_ASSET(ASSET_TYPE_MAP_ENTS, MapEnts, mapEnts)
        LOAD_ASSET(ASSET_TYPE_GFXWORLD, GfxWorld, gfxWorld)
        LOAD_ASSET(ASSET_TYPE_LIGHT_DEF, GfxLightDef, lightDef)
        LOAD_ASSET(ASSET_TYPE_FONT, Font_s, font)
        LOAD_ASSET(ASSET_TYPE_MENULIST, MenuList, menuList)
        LOAD_ASSET(ASSET_TYPE_MENU, menuDef_t, menu)
        LOAD_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry, localize)
        LOAD_ASSET(ASSET_TYPE_WEAPON, WeaponDef, weapon)
        LOAD_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals, sndDriverGlobals)
        LOAD_ASSET(ASSET_TYPE_FX, FxEffectDef, fx)
        LOAD_ASSET(ASSET_TYPE_IMPACT_FX, FxImpactTable, impactFx)
        LOAD_ASSET(ASSET_TYPE_RAWFILE, RawFile, rawfile)
        LOAD_ASSET(ASSET_TYPE_STRINGTABLE, StringTable, stringTable)

    default:
    {
        throw UnsupportedAssetTypeException(varXAsset->type);
    }
    }

#undef LOAD_ASSET
#undef SKIP_ASSET
}

void ContentLoader::LoadXAssetArray(const bool atStreamStart, const size_t count)
{
    assert(varXAsset != nullptr);

    if (atStreamStart)
    {
        m_stream.Load<XAsset>(varXAsset, count);
        for (size_t index = 0; index < count; index++)
        {
            varXAsset[index].type = static_cast<XAssetType>(endianness::FromBigEndian(static_cast<int>(varXAsset[index].type)));
        }
    }

    for (size_t index = 0; index < count; index++)
    {
        printf("Loading asset %zu: type=%d\n", index, static_cast<int>(varXAsset->type));
        try
        {
            LoadXAsset(false);
        }
        catch (const UnsupportedAssetTypeException& e)
        {
            printf("ERROR: Asset %zu is not supported (type=%d). Cannot read any more of the fastfile, but will continue dumping what has been loaded.\n",
                   index,
                   static_cast<int>(varXAsset->type));
            break;
        }
        varXAsset++;

#ifdef DEBUG_OFFSETS
        m_stream.DebugOffsets(index);
#endif
    }
}

void ContentLoader::Load()
{
    XAssetList assetList{};
    varXAssetList = &assetList;

    m_stream.LoadDataRaw(&assetList, sizeof(assetList));

    // Endian swap the loaded data
    varXAssetList->stringList.count = endianness::FromBigEndian(varXAssetList->stringList.count);
    varXAssetList->assetCount = endianness::FromBigEndian(varXAssetList->assetCount);

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
}
