#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"
#include "QOSXenon_Assets.h"

// clang-format on

namespace QOSXenon
{
    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_VERTEX_SHADER,

        SUB_ASSET_TYPE_COUNT
    };

    struct ZoneHeaderQos
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
    using AssetPhysConstraints = Asset<ASSET_TYPE_PHYSCONSTRAINTS, PhysConstraints>;
    using AssetDestructibleDef = Asset<ASSET_TYPE_DESTRUCTIBLEDEF, DestructibleDef>;
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetFx = Asset<ASSET_TYPE_FX, FxEffectDef>;
    using AssetImpactFx = Asset<ASSET_TYPE_IMPACT_FX, FxImpactTable>;
    using AssetWeapon = Asset<ASSET_TYPE_WEAPON, WeaponDef>;
    using AssetSoundDriverGlobals = Asset<ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXELSHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetSoundCurve = Asset<ASSET_TYPE_SND_CURVE, SndCurve>;
    using AssetClipMapSp = Asset<ASSET_TYPE_CLIPMAP_SP, clipMap_t>;
    using AssetClipMapMp = Asset<ASSET_TYPE_CLIPMAP_MP, clipMap_t>;
    using AssetComWorld = Asset<ASSET_TYPE_COMWORLD, ComWorld>;
    using AssetGameWorldSp = Asset<ASSET_TYPE_GAMEWORLD_SP, GameWorldSp>;
    using AssetGameWorldMp = Asset<ASSET_TYPE_GAMEWORLD_MP, GameWorldMp>;
    using AssetMapEnts = Asset<ASSET_TYPE_MAP_ENTS, MapEnts>;
    using AssetGfxWorld = Asset<ASSET_TYPE_GFXWORLD, GfxWorld>;
    using AssetLightDef = Asset<ASSET_TYPE_LIGHT_DEF, GfxLightDef>;
    using AssetFont = Asset<ASSET_TYPE_FONT, Font_s>;
    using AssetMenuList = Asset<ASSET_TYPE_MENUFILE, MenuList>;
    using AssetMenu = Asset<ASSET_TYPE_MENU, menuDef_t>;
    using AssetLocalize = Asset<ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
    using AssetXmlTree = Asset<ASSET_TYPE_XMLTREE, XmlTree>;
    using AssetSceneAnimation = Asset<ASSET_TYPE_SCENE_ANIMATION, SceneAnimation>;
    using AssetCutscene = Asset<ASSET_TYPE_CUTSCENE, Cutscene>;
    using AssetCustomCamera = Asset<ASSET_TYPE_CUSTOM_CAMERA, CustomCamera>;
    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetVertexShader = SubAsset<SUB_ASSET_TYPE_VERTEX_SHADER, MaterialVertexShader>;
} // namespace QOSXenon

DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetPhysConstraints, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetDestructibleDef, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetWeapon, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetSoundDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetSound, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetSoundCurve, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetClipMapSp, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetClipMapMp, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetStringTable, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetXmlTree, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetSceneAnimation, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetCutscene, name);
DEFINE_ASSET_NAME_ACCESSOR(QOSXenon::AssetCustomCamera, name);
