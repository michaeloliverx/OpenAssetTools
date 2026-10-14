#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "T4Xenon_Assets.h"

// clang-format on

namespace T4Xenon
{
    enum SubAssetType
    {
        SUB_ASSET_TYPE_TECHNIQUE,
        SUB_ASSET_TYPE_VERTEX_DECL,
        SUB_ASSET_TYPE_VERTEX_SHADER,
        SUB_ASSET_TYPE_ACCURACY_GRAPH,
        SUB_ASSET_TYPE_FLAME_TABLE,

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
        char magic[8];
        unsigned int reserved;
        DB_AuthHash subheaderHash;
        DB_AuthSignature signedSubheaderHash;
        DB_AuthSubHeader subheader;
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

    struct AccuracyGraph
    {
        float (*graphKnots)[2];
        int graphKnotCount;
    };

    using AssetPhysPreset = Asset<ASSET_TYPE_PHYSPRESET, PhysPreset>;
    using AssetPhysConstraints = Asset<ASSET_TYPE_PHYSCONSTRAINTS, PhysConstraints>;
    using AssetDestructibleDef = Asset<ASSET_TYPE_DESTRUCTIBLEDEF, DestructibleDef>;
    using AssetXAnim = Asset<ASSET_TYPE_XANIMPARTS, XAnimParts>;
    using AssetXModel = Asset<ASSET_TYPE_XMODEL, XModel>;
    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetPixelShader = Asset<ASSET_TYPE_PIXELSHADER, MaterialPixelShader>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetSound = Asset<ASSET_TYPE_SOUND, snd_alias_list_t>;
    using AssetLoadedSound = Asset<ASSET_TYPE_LOADED_SOUND, LoadedSound>;
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
    using AssetPackIndex = Asset<ASSET_TYPE_PACKINDEX, PackIndex>;

    using SubAssetTechnique = SubAsset<SUB_ASSET_TYPE_TECHNIQUE, MaterialTechnique>;
    using SubAssetVertexDecl = SubAsset<SUB_ASSET_TYPE_VERTEX_DECL, MaterialVertexDeclaration>;
    using SubAssetVertexShader = SubAsset<SUB_ASSET_TYPE_VERTEX_SHADER, MaterialVertexShader>;
    using SubAssetAccuracyGraph = SubAsset<SUB_ASSET_TYPE_ACCURACY_GRAPH, AccuracyGraph>;
    using SubAssetFlameTable = SubAsset<SUB_ASSET_TYPE_FLAME_TABLE, flameTable>;
} // namespace T4Xenon

DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetPhysPreset, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetPhysConstraints, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetDestructibleDef, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetXAnim, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetXModel, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetPixelShader, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetSound, aliasName);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetLoadedSound, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetClipMap, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetClipMapPvs, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetComWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetGameWorldSp, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetGameWorldMp, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetMapEnts, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetGfxWorld, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetLightDef, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetFont, fontName);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetMenuList, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetMenu, window.name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetWeapon, szInternalName);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetSndDriverGlobals, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetFx, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetImpactFx, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetStringTable, name);
DEFINE_ASSET_NAME_ACCESSOR(T4Xenon::AssetPackIndex, name);
