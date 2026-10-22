#pragma once

#include "Game/IAsset.h"
#include "IW3Xenon290_Assets.h"

namespace IW3Xenon290
{
    struct ZoneHeader290
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
} // namespace IW3Xenon290

DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetSoundCurve, filename);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetClipMap, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetClipMapPvs, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetSndDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon290::AssetStringTable, name);
