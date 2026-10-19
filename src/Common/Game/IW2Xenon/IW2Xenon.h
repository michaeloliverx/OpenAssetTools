#pragma once

#include "Game/IAsset.h"
#include "IW2Xenon_Assets.h"

namespace IW2Xenon
{
    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE_SET,
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_SHADER,
        SUB_ASSET_TYPE_COUNT
    };

    struct ZoneHeaderIW2
    {
        uint32_t version;
    };

    struct XStringList
    {
        int count;
        const char** strings;
    };

    union XAssetHeader
    {
        void* data;
        XAnimParts* xAnim;
        XModel* xModel;
        Material* material;
        GfxImage* image;
        snd_alias_list_t* sound;
        SndCurve* soundCurve;
        clipMap_t* clipMapMp;
        GfxWorld* gfxWorld;
        GfxLightDef* lightDef;
        Font_s* font;
        MenuList* menuList;
        menuDef_t* menu;
        LocalizeEntry* localize;
        WeaponDef* weapon;
        SndDriverGlobals* soundDriverGlobals;
        EffectTemplate* fx;
        FxImpactTable* impactFx;
        RawFile* rawFile;
    };

    struct XAsset
    {
        XAssetType type;
        XAssetHeader header;
    };

    struct XAssetList
    {
        XStringList rawStrings;
        XStringList tempStrings;
        int assetCount;
        XAsset* assets;
    };

    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetSoundCurve = Asset<ASSET_TYPE_SND_CURVE, SndCurve>;
    using AssetClipMapSp = Asset<ASSET_TYPE_CLIPMAP_SP, clipMap_t>;
    using AssetClipMapMp = Asset<ASSET_TYPE_CLIPMAP_MP, clipMap_t>;
    using AssetGfxWorld = Asset<ASSET_TYPE_GFXWORLD, GfxWorld>;
    using AssetLightDef = Asset<ASSET_TYPE_LIGHT_DEF, GfxLightDef>;
    using AssetFont = Asset<ASSET_TYPE_FONT, Font_s>;
    using AssetMenuList = Asset<ASSET_TYPE_MENUFILE, MenuList>;
    using AssetMenu = Asset<ASSET_TYPE_MENU, menuDef_t>;
    using AssetLocalize = Asset<ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry>;
    using AssetWeapon = Asset<ASSET_TYPE_WEAPON, WeaponDef>;
    using AssetSoundDriverGlobals = Asset<ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals>;
    using AssetFx = Asset<ASSET_TYPE_FX, EffectTemplate>;
    using AssetImpactFx = Asset<ASSET_TYPE_IMPACT_FX, FxImpactTable>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;

    using SubAssetTechniqueSet = SubAsset<SUB_ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetShader = SubAsset<SUB_ASSET_TYPE_SHADER, MaterialShader>;
} // namespace IW2Xenon

DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetSoundCurve, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetSoundDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetClipMapMp, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetSound, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetWeapon, name);

DEFINE_ASSET_NAME_ACCESSOR(IW2Xenon::AssetClipMapSp, name);
