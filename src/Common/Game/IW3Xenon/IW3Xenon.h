#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "IW3Xenon_Assets.h"

// clang-format on

namespace IW3Xenon
{
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
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXELSHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetSoundCurve = Asset<ASSET_TYPE_SOUND_CURVE, SndCurve>;
    using AssetLoadedSound = Asset<ASSET_TYPE_LOADED_SOUND, LoadedSound>;
    using AssetClipMap = Asset<ASSET_TYPE_CLIPMAP, clipMap_t>;
    using AssetClipMapPvs = Asset<ASSET_TYPE_CLIPMAP_PVS, clipMap_t>;
    using AssetComWorld = Asset<ASSET_TYPE_COMWORLD, ComWorld>;
    using AssetGameWorldMp = Asset<ASSET_TYPE_GAMEWORLD_MP, GameWorldMp>;
    using AssetMapEnts = Asset<ASSET_TYPE_MAP_ENTS, MapEnts>;
    using AssetGfxWorld = Asset<ASSET_TYPE_GFXWORLD, GfxWorld>;
    using AssetLightDef = Asset<ASSET_TYPE_LIGHT_DEF, GfxLightDef>;
    using AssetFont = Asset<ASSET_TYPE_FONT, Font_s>;
    using AssetMenuList = Asset<ASSET_TYPE_MENULIST, MenuList>;
    using AssetMenu = Asset<ASSET_TYPE_MENU, menuDef_t>;
    using AssetLocalize = Asset<ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry>;
    using AssetWeapon = Asset<ASSET_TYPE_WEAPON, WeaponDef>;
    using AssetSndDriverGlobals = Asset<ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals>;
    using AssetFx = Asset<ASSET_TYPE_FX, FxEffectDef>;
    using AssetImpactFx = Asset<ASSET_TYPE_IMPACT_FX, FxImpactTable>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
} // namespace IW3Xenon

DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetSoundCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetLoadedSound, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetClipMap, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetClipMapPvs, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetSndDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetStringTable, name);
