#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "NX1_Assets.h"

// clang-format on

namespace NX1
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
        ASSET_TYPE_SND_VOLUME_FALLOFF_CURVE,
        ASSET_TYPE_SND_LPF_CURVE,
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
        ASSET_TYPE_SURFACE_FX,
        ASSET_TYPE_AITYPE,
        ASSET_TYPE_MPTYPE,
        ASSET_TYPE_CHARACTER,
        ASSET_TYPE_XMODELALIAS,
        ASSET_TYPE_RAWFILE,
        ASSET_TYPE_STRINGTABLE,
        ASSET_TYPE_LEADERBOARD,
        ASSET_TYPE_STRUCTURED_DATA_DEF,
        ASSET_TYPE_TRACER,
        ASSET_TYPE_LASER,
        ASSET_TYPE_VEHICLE,
        ASSET_TYPE_ADDON_MAP_ENTS,
        ASSET_TYPE_SHOCKFILE,
        ASSET_TYPE_VOLUMESETTING,
        ASSET_TYPE_REVERBPRESET,
        ASSET_TYPE_FOG,

        ASSET_TYPE_COUNT
    };

    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_VERTEX_SHADER,

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
    using AssetSndVfCurve = Asset<ASSET_TYPE_SND_VOLUME_FALLOFF_CURVE, SndVfCurve>;
    using AssetSndLpfCurve = Asset<ASSET_TYPE_SND_LPF_CURVE, SndLpfCurve>;
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
    using AssetSurfaceFx = Asset<ASSET_TYPE_SURFACE_FX, SurfaceFxTable>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
    using AssetLeaderboard = Asset<ASSET_TYPE_LEADERBOARD, LeaderboardDef>;
    using AssetStructuredDataDef = Asset<ASSET_TYPE_STRUCTURED_DATA_DEF, StructuredDataDefSet>;
    using AssetTracer = Asset<ASSET_TYPE_TRACER, TracerDef>;
    using AssetLaser = Asset<ASSET_TYPE_LASER, LaserDef>;
    using AssetVehicle = Asset<ASSET_TYPE_VEHICLE, VehicleDef>;
    using AssetAddonMapEnts = Asset<ASSET_TYPE_ADDON_MAP_ENTS, AddonMapEnts>;
    using AssetShockFile = Asset<ASSET_TYPE_SHOCKFILE, ShellShockParams>;
    using AssetVolumeSetting = Asset<ASSET_TYPE_VOLUMESETTING, AudioVolumeSetting>;
    using AssetReverbPreset = Asset<ASSET_TYPE_REVERBPRESET, RadVerbPreset>;
    using AssetFog = Asset<ASSET_TYPE_FOG, FogDef>;

    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetVertexShader = SubAsset<SUB_ASSET_TYPE_VERTEX_SHADER, MaterialVertexShader>;
} // namespace NX1

DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetPhysCollMap, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetXModelSurfs, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetSndVfCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetSndLpfCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetLoadedSound, p_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetClipMapSp, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetClipMapMp, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetFxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetSndDriverGlobals, p_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetSurfaceFx, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetStringTable, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetLeaderboard, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetStructuredDataDef, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetTracer, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetLaser, mp_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetVehicle, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetAddonMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetShockFile, p_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetVolumeSetting, p_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetReverbPreset, p_name);
DEFINE_ASSET_NAME_ACCESSOR(NX1::AssetFog, mp_name);
