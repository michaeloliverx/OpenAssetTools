#pragma once

#ifndef __IW3XENON_ASSETS_H
#define __IW3XENON_ASSETS_H

#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace IW3Xenon
{
#endif
    enum XAssetType
    {
        ASSET_TYPE_XMODELPIECES = 0x0,
        ASSET_TYPE_PHYSPRESET = 0x1,
        ASSET_TYPE_XANIMPARTS = 0x2,
        ASSET_TYPE_XMODEL = 0x3,
        ASSET_TYPE_MATERIAL = 0x4,
        ASSET_TYPE_PIXELSHADER = 0x5,
        ASSET_TYPE_TECHNIQUE_SET = 0x6,
        ASSET_TYPE_IMAGE = 0x7,
        ASSET_TYPE_SOUND = 0x8,
        ASSET_TYPE_SOUND_CURVE = 0x9,
        ASSET_TYPE_LOADED_SOUND = 0xA,
        ASSET_TYPE_CLIPMAP = 0xB,
        ASSET_TYPE_CLIPMAP_PVS = 0xC,
        ASSET_TYPE_COMWORLD = 0xD,
        ASSET_TYPE_GAMEWORLD_SP = 0xE,
        ASSET_TYPE_GAMEWORLD_MP = 0xF,
        ASSET_TYPE_MAP_ENTS = 0x10,
        ASSET_TYPE_GFXWORLD = 0x11,
        ASSET_TYPE_LIGHT_DEF = 0x12,
        ASSET_TYPE_UI_MAP = 0x13,
        ASSET_TYPE_FONT = 0x14,
        ASSET_TYPE_MENULIST = 0x15,
        ASSET_TYPE_MENU = 0x16,
        ASSET_TYPE_LOCALIZE_ENTRY = 0x17,
        ASSET_TYPE_WEAPON = 0x18,
        ASSET_TYPE_SNDDRIVER_GLOBALS = 0x19,
        ASSET_TYPE_FX = 0x1A,
        ASSET_TYPE_IMPACT_FX = 0x1B,
        ASSET_TYPE_AITYPE = 0x1C,
        ASSET_TYPE_MPTYPE = 0x1D,
        ASSET_TYPE_CHARACTER = 0x1E,
        ASSET_TYPE_XMODELALIAS = 0x1F,
        ASSET_TYPE_RAWFILE = 0x20,
        ASSET_TYPE_STRINGTABLE = 0x21,
        ASSET_TYPE_COUNT,

        ASSET_TYPE_STRING = ASSET_TYPE_COUNT,
        ASSET_TYPE_ASSETLIST,

        ASSET_TYPE_FULLCOUNT
    };

    enum XFileBlock
    {
        XFILE_BLOCK_TEMP = 0x0,
        XFILE_BLOCK_RUNTIME = 0x1,
        XFILE_BLOCK_LARGE_RUNTIME = 0x2,
        XFILE_BLOCK_PHYSICAL_RUNTIME = 0x3,
        XFILE_BLOCK_VIRTUAL = 0x4,
        XFILE_BLOCK_LARGE = 0x5,
        XFILE_BLOCK_PHYSICAL = 0x6,

        MAX_XFILE_COUNT
    };

    struct MaterialTechniqueSet;
    struct MaterialPixelShader;
    struct GfxImage;
    struct Material;
    struct Font_s;
    struct XAnimParts;
    struct PhysPreset;
    struct XModel;
    struct LoadedSound;
    struct SndCurve;
    struct GfxLightDef;
    struct FxEffectDef;
    struct snd_alias_list_t;
    struct menuDef_t;
    struct MenuList;
    struct StringTable;
    struct FxImpactTable;
    struct XModelPieces;
    struct MapEnts;
    struct clipMap_t;
    struct ComWorld;
    struct GameWorldSp;
    struct GameWorldMp;
    struct GfxWorld;
    struct LocalizeEntry;
    struct WeaponDef;
    struct SndDriverGlobals;
    struct RawFile;

    typedef unsigned short ScriptString;

    union XAssetHeader
    {
        XModelPieces* xmodelPieces;
        PhysPreset* physPreset;
        XAnimParts* parts;
        XModel* model;
        Material* material;
        MaterialPixelShader* pixelShader;
        MaterialTechniqueSet* techniqueSet;
        GfxImage* image;
        snd_alias_list_t* sound;
        SndCurve* sndCurve;
        LoadedSound* loadSnd;
        clipMap_t* clipMap;
        ComWorld* comWorld;
        GameWorldSp* gameWorldSp;
        GameWorldMp* gameWorldMp;
        MapEnts* mapEnts;
        GfxWorld* gfxWorld;
        GfxLightDef* lightDef;
        Font_s* font;
        MenuList* menuList;
        menuDef_t* menu;
        LocalizeEntry* localize;
        WeaponDef* weapon;
        SndDriverGlobals* sndDriverGlobals;
        const FxEffectDef* fx;
        FxImpactTable* impactFx;
        RawFile* rawfile;
        StringTable* stringTable;
        void* data;
    };

    struct LocalizeEntry
    {
        const char* value;
        const char* name;
    };

    struct RawFile
    {
        const char* name;
        int len;
        const char* buffer;
    };

    struct StringTable
    {
        const char* name;
        int columnCount;
        int rowCount;
        const char** values;
    };

#ifndef __zonecodegenerator
} // namespace IW3Xenon
#endif

#endif // __IW3XENON_ASSETS_H
