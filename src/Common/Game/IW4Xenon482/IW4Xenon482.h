#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "IW4Xenon482_Assets.h"

// clang-format on

namespace IW4Xenon482
{
    enum XAssetType
    {
        ASSET_TYPE_PHYSPRESET,
        ASSET_TYPE_PHYSCOLLMAP,
        ASSET_TYPE_XANIMPARTS,
        ASSET_TYPE_XMODEL_SURFS,
        ASSET_TYPE_XMODEL,
        ASSET_TYPE_MATERIAL,
        ASSET_TYPE_PIXELSHADER,
        ASSET_TYPE_TECHNIQUE_SET,
        ASSET_TYPE_IMAGE,
        ASSET_TYPE_SOUND,
        ASSET_TYPE_SOUND_CURVE,
        ASSET_TYPE_LOADED_SOUND,
        ASSET_TYPE_CLIPMAP_SP,
        ASSET_TYPE_CLIPMAP_MP,
        ASSET_TYPE_COMWORLD,
        ASSET_TYPE_GAMEWORLD_SP,
        ASSET_TYPE_GAMEWORLD_MP,
        ASSET_TYPE_MAP_ENTS,
        ASSET_TYPE_FXWORLD,
        ASSET_TYPE_GFXWORLD,
        ASSET_TYPE_LIGHT_DEF,
        ASSET_TYPE_UI_MAP,
        ASSET_TYPE_FONT,
        ASSET_TYPE_MENULIST,
        ASSET_TYPE_MENU,
        ASSET_TYPE_LOCALIZE_ENTRY,
        ASSET_TYPE_WEAPON,
        ASSET_TYPE_SNDDRIVER_GLOBALS,
        ASSET_TYPE_FX,
        ASSET_TYPE_IMPACT_FX,
        ASSET_TYPE_AITYPE,
        ASSET_TYPE_MPTYPE,
        ASSET_TYPE_CHARACTER,
        ASSET_TYPE_XMODELALIAS,
        ASSET_TYPE_RAWFILE,
        ASSET_TYPE_STRINGTABLE,
        ASSET_TYPE_LEADERBOARD,
        ASSET_TYPE_STRUCTURED_DATA_DEF,
        ASSET_TYPE_TRACER,
        ASSET_TYPE_VEHICLE,
        ASSET_TYPE_ADDON_MAP_ENTS,

        ASSET_TYPE_COUNT
    };

    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_VERTEX_SHADER,
        SUB_ASSET_TYPE_ACCURACY_GRAPH,

        SUB_ASSET_TYPE_COUNT
    };

    struct DB_AuthHash
    {
        char bytes[32];
    };

    struct DB_AuthSignature
    {
        char bytes[256];
    };

    struct DB_AuthSubHeader
    {
        char fastfileName[32];
        unsigned int reserved;
        DB_AuthHash masterBlockHashes[244];
    };

    struct DB_AuthHeader
    {
        char magic[8];                        // + 0
        unsigned int reserved;                // + 8
        DB_AuthHash subheaderHash;            // + 12
        DB_AuthSignature signedSubheaderHash; // + 44
        DB_AuthSubHeader subheader;           // + 300
    };

    struct ScriptStringList
    {
        int count;
        const char** strings;
    };

    struct XAsset
    {
        XAssetType type;
        XAssetHeader header;
    };

    struct XAssetList
    {
        ScriptStringList stringList;
        int assetCount;
        XAsset* assets;
    };

    struct cspField_t
    {
        const char* szName;
        int iOffset;
        int iFieldType;
    };

    enum csParseFieldType_t
    {
        CSPFT_STRING = 0,
        CSPFT_STRING_MAX_STRING_CHARS,
        CSPFT_STRING_MAX_QPATH,
        CSPFT_STRING_MAX_OSPATH,
        CSPFT_INT,
        CSPFT_QBOOLEAN,
        CSPFT_BOOL,
        CSPFT_FLOAT,
        CSPFT_MPH_TO_INCHES_PER_SEC,
        CSPFT_MILLISECONDS,
        CSPFT_FX,
        CSPFT_XMODEL,
        CSPFT_MATERIAL,
        CSPFT_PHYS_COLLMAP,
        CSPFT_SOUND,
        CSPFT_TRACER,

        CSPFT_NUM_BASE_FIELD_TYPES,
    };

    enum weapFieldType_t
    {
        WFT_WEAPONTYPE = CSPFT_NUM_BASE_FIELD_TYPES,
        WFT_WEAPONCLASS,
        WFT_OVERLAYRETICLE,
        WFT_PENETRATE_TYPE,
        WFT_IMPACT_TYPE,
        WFT_STANCE,
        WFT_PROJ_EXPLOSION,
        WFT_OFFHAND_CLASS,
        WFT_ANIMTYPE,
        WFT_ACTIVE_RETICLE_TYPE,
        WFT_GUIDED_MISSILE_TYPE,
        WFT_BOUNCE_SOUND,
        WFT_STICKINESS,
        WFT_OVERLAYINTERFACE,
        WFT_INVENTORYTYPE,
        WFT_FIRETYPE,
        WFT_AMMOCOUNTER_CLIPTYPE,
        WFT_ICONRATIO_HUD,
        WFT_ICONRATIO_PICKUP,
        WFT_ICONRATIO_AMMOCOUNTER,
        WFT_ICONRATIO_KILL,
        WFT_ICONRATIO_DPAD,
        WFT_HIDETAGS,
        WFT_NOTETRACKSOUNDMAP,
        WFT_NOTETRACKRUMBLEMAP,

        // Custom
        WFT_ANIM_NAME,

        WFT_NUM_FIELD_TYPES,
    };

    enum VehicleFieldType
    {
        VFT_TYPE = CSPFT_NUM_BASE_FIELD_TYPES,
        VFT_AXLE_STEERING,
        VFT_AXLE_POWER,
        VFT_AXLE_BRAKING,
        VFT_TROPHY_TAGS,

        VFT_NUM,
    };

    struct AccuracyGraph
    {
        vec2_t* graphKnots;
        int graphKnotCount;
    };

    using AssetPhysPreset = Asset<ASSET_TYPE_PHYSPRESET, PhysPreset>;
    using AssetPhysCollMap = Asset<ASSET_TYPE_PHYSCOLLMAP, PhysCollmap>;
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModelSurfs = Asset<ASSET_TYPE_XMODEL_SURFS, XModelSurfs>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXELSHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetSoundCurve = Asset<ASSET_TYPE_SOUND_CURVE, SndCurve>;
    using AssetLoadedSound = Asset<ASSET_TYPE_LOADED_SOUND, LoadedSound>;
    using AssetClipMapSp = Asset<ASSET_TYPE_CLIPMAP_SP, clipMap_t>;
    using AssetClipMapMp = Asset<ASSET_TYPE_CLIPMAP_MP, clipMap_t>;
    using AssetComWorld = Asset<ASSET_TYPE_COMWORLD, ComWorld>;
    using AssetGameWorldSp = Asset<ASSET_TYPE_GAMEWORLD_SP, GameWorldSp>;
    using AssetGameWorldMp = Asset<ASSET_TYPE_GAMEWORLD_MP, GameWorldMp>;
    using AssetMapEnts = Asset<ASSET_TYPE_MAP_ENTS, MapEnts>;
    using AssetFxWorld = Asset<ASSET_TYPE_FXWORLD, FxWorld>;
    using AssetGfxWorld = Asset<ASSET_TYPE_GFXWORLD, GfxWorld>;
    using AssetLightDef = Asset<ASSET_TYPE_LIGHT_DEF, GfxLightDef>;
    using AssetFont = Asset<ASSET_TYPE_FONT, Font_s>;
    using AssetMenuList = Asset<ASSET_TYPE_MENULIST, MenuList>;
    using AssetMenu = Asset<ASSET_TYPE_MENU, menuDef_t>;
    using AssetLocalize = Asset<ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry>;
    using AssetWeapon = Asset<ASSET_TYPE_WEAPON, WeaponCompleteDef>;
    using AssetSndDriverGlobals = Asset<ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals>;
    using AssetFx = Asset<ASSET_TYPE_FX, FxEffectDef>;
    using AssetImpactFx = Asset<ASSET_TYPE_IMPACT_FX, FxImpactTable>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
    using AssetLeaderboard = Asset<ASSET_TYPE_LEADERBOARD, LeaderboardDef>;
    using AssetStructuredDataDef = Asset<ASSET_TYPE_STRUCTURED_DATA_DEF, StructuredDataDefSet>;
    using AssetTracer = Asset<ASSET_TYPE_TRACER, TracerDef>;
    using AssetVehicle = Asset<ASSET_TYPE_VEHICLE, VehicleDef>;
    using AssetAddonMapEnts = Asset<ASSET_TYPE_ADDON_MAP_ENTS, AddonMapEnts>;

    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetVertexShader = SubAsset<SUB_ASSET_TYPE_VERTEX_SHADER, MaterialVertexShader>;
    using SubAssetAccuracyGraph = SubAsset<SUB_ASSET_TYPE_ACCURACY_GRAPH, AccuracyGraph>;
} // namespace IW4Xenon482

DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetPhysCollMap, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetXModelSurfs, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetSoundCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetLoadedSound, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetClipMapSp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetClipMapMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetFxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetSndDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetStringTable, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetLeaderboard, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetStructuredDataDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetTracer, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetVehicle, name);
DEFINE_ASSET_NAME_ACCESSOR(IW4Xenon482::AssetAddonMapEnts, name);
