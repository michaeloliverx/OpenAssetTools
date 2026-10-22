#pragma once

#include "Game/IAsset.h"
#include "IW3Xenon270_Assets.h"

namespace IW3Xenon270
{
    struct ZoneHeader270
    {
        uint32_t version;
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
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXELSHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetSoundCurve = Asset<ASSET_TYPE_SOUND_CURVE, SndCurve>;
    using AssetClipMap = Asset<ASSET_TYPE_CLIPMAP, clipMap_t>;
    using AssetClipMapPvs = Asset<ASSET_TYPE_CLIPMAP_PVS, clipMap_t>;
    using AssetComWorld = Asset<ASSET_TYPE_COMWORLD, ComWorld>;
    using AssetGameWorldSp = Asset<ASSET_TYPE_GAMEWORLD_SP, GameWorldSp>;
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
} // namespace IW3Xenon270

DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetSoundCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetClipMap, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetClipMapPvs, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetSndDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon270::AssetStringTable, name);
