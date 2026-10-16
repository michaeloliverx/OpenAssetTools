#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "T5Xenon_Assets.h"

// clang-format on

namespace T5Xenon
{
    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_VERTEX_SHADER,
        SUB_ASSET_TYPE_COUNT
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
    using AssetPhysConstraints = Asset<ASSET_TYPE_PHYSCONSTRAINTS, PhysConstraints>;
    using AssetDestructibleDef = Asset<ASSET_TYPE_DESTRUCTIBLEDEF, DestructibleDef>;
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXEL_SHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSoundBank = Asset<ASSET_TYPE_SOUND, SndBank>;
    using AssetSoundPatch = Asset<ASSET_TYPE_SOUND_PATCH, SndPatch>;
    using AssetClipMapSp = Asset<ASSET_TYPE_CLIPMAP_SP, clipMap_t>;
    using AssetClipMapMp = Asset<ASSET_TYPE_CLIPMAP_MP, clipMap_t>;
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
    using AssetWeapon = Asset<ASSET_TYPE_WEAPON, WeaponVariantDef>;
    using AssetSoundDriverGlobals = Asset<ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals>;
    using AssetFx = Asset<ASSET_TYPE_FX, FxEffectDef>;
    using AssetImpactFx = Asset<ASSET_TYPE_IMPACT_FX, FxImpactTable>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
    using AssetPackIndex = Asset<ASSET_TYPE_PACK_INDEX, PackIndex>;
    using AssetXGlobals = Asset<ASSET_TYPE_XGLOBALS, XGlobals>;
    using AssetTextureList = Asset<ASSET_TYPE_TEXTURE_LIST, TextureList>;
    using AssetDDL = Asset<ASSET_TYPE_DDL, ddlRoot_t>;
    using AssetGlasses = Asset<ASSET_TYPE_GLASSES, Glasses>;
    using AssetEmblemSet = Asset<ASSET_TYPE_EMBLEMSET, EmblemSet>;
    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetVertexShader = SubAsset<SUB_ASSET_TYPE_VERTEX_SHADER, MaterialVertexShader>;
} // namespace T5Xenon

DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetPhysConstraints, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetDestructibleDef, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetSoundBank, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetSoundPatch, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetClipMapSp, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetClipMapMp, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetSoundDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR_SINGLETON(T5Xenon::AssetImpactFx, "ImpactFx");
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetStringTable, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetPackIndex, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetXGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR_SINGLETON(T5Xenon::AssetTextureList, "TextureList");
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetDDL, name);
DEFINE_ASSET_NAME_ACCESSOR(T5Xenon::AssetGlasses, name);
DEFINE_ASSET_NAME_ACCESSOR_SINGLETON(T5Xenon::AssetEmblemSet, "EmblemSet");
