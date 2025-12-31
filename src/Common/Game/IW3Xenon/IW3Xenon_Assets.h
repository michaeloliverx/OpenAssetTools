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

    enum MaterialStreamStreamSource_e
    {
        STREAM_SRC_POSITION = 0x0,
        STREAM_SRC_COLOR = 0x1,
        STREAM_SRC_TEXCOORD_0 = 0x2,
        STREAM_SRC_NORMAL = 0x3,
        STREAM_SRC_TANGENT = 0x4,
        STREAM_SRC_OPTIONAL_BEGIN = 0x5,
        STREAM_SRC_PRE_OPTIONAL_BEGIN = 0x4,
        STREAM_SRC_TEXCOORD_1 = 0x5,
        STREAM_SRC_TEXCOORD_2 = 0x6,
        STREAM_SRC_NORMAL_TRANSFORM_0 = 0x7,
        STREAM_SRC_NORMAL_TRANSFORM_1 = 0x8,
        STREAM_SRC_COUNT = 0x9,
    };

    enum MaterialStreamDestination_e
    {
        STREAM_DST_POSITION = 0x0,
        STREAM_DST_NORMAL = 0x1,
        STREAM_DST_COLOR_0 = 0x2,
        STREAM_DST_COLOR_1 = 0x3,
        STREAM_DST_TEXCOORD_0 = 0x4,
        STREAM_DST_TEXCOORD_1 = 0x5,
        STREAM_DST_TEXCOORD_2 = 0x6,
        STREAM_DST_TEXCOORD_3 = 0x7,
        STREAM_DST_TEXCOORD_4 = 0x8,
        STREAM_DST_TEXCOORD_5 = 0x9,
        STREAM_DST_TEXCOORD_6 = 0xA,
        STREAM_DST_TEXCOORD_7 = 0xB,
        STREAM_DST_COUNT = 0xC,
    };

    struct MaterialStreamRouting
    {
        unsigned __int8 source;
        unsigned __int8 dest;
    };

    union MaterialVertexStreamRouting
    {
        MaterialStreamRouting data[16];
        void /*D3DVertexDeclaration*/* decl[15];
    };

    struct MaterialVertexDeclaration
    {
        unsigned __int8 streamCount;
        bool hasOptionalSource;
        MaterialVertexStreamRouting routing;
    };

    struct GfxVertexShaderLoadDef
    {
        unsigned __int8* cachedPart;
        unsigned __int8* physicalPart;
        unsigned __int16 cachedPartSize;
        unsigned __int16 physicalPartSize;
    };

    union MaterialVertexShaderProgram
    {
        void /*D3DVertexShader*/* vs;
        GfxVertexShaderLoadDef loadDef;
    };

    struct MaterialVertexShader
    {
        const char* name;
        MaterialVertexShaderProgram prog;
    };

    struct GfxPixelShaderLoadDef
    {
        unsigned __int8* cachedPart;
        unsigned __int8* physicalPart;
        unsigned __int16 cachedPartSize;
        unsigned __int16 physicalPartSize;
    };

    union MaterialPixelShaderProgram
    {
        void /*D3DPixelShader*/* ps;
        GfxPixelShaderLoadDef loadDef;
    };

    struct MaterialPixelShader
    {
        const char* name;
        MaterialPixelShaderProgram prog;
    };

    struct MaterialArgumentCodeConst
    {
        unsigned __int16 index;
        unsigned __int8 firstRow;
        unsigned __int8 rowCount;
    };

    union MaterialArgumentDef
    {
        const float* literalConst;
        MaterialArgumentCodeConst codeConst;
        unsigned int codeSampler;
        unsigned int nameHash;
    };

    enum MaterialShaderArgumentType
    {
        MTL_ARG_MATERIAL_VERTEX_CONST = 0x0,
        MTL_ARG_LITERAL_VERTEX_CONST = 0x1,
        MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x2,
        MTL_ARG_CODE_PRIM_BEGIN = 0x3,
        MTL_ARG_CODE_VERTEX_CONST = 0x3,
        MTL_ARG_CODE_PIXEL_SAMPLER = 0x4,
        MTL_ARG_CODE_PIXEL_CONST = 0x5,
        MTL_ARG_CODE_PRIM_END = 0x6,
        MTL_ARG_MATERIAL_PIXEL_CONST = 0x6,
        MTL_ARG_LITERAL_PIXEL_CONST = 0x7,
        MLT_ARG_COUNT = 0x8,
    };

    enum MaterialConstantSource
    {
        CONST_SRC_CODE_MAYBE_DIRTY_PS_BEGIN = 0x0,
        CONST_SRC_CODE_LIGHT_POSITION = 0x0,
        CONST_SRC_CODE_LIGHT_DIFFUSE = 0x1,
        CONST_SRC_CODE_LIGHT_SPECULAR = 0x2,
        CONST_SRC_CODE_LIGHT_SPOTDIR = 0x3,
        CONST_SRC_CODE_LIGHT_SPOTFACTORS = 0x4,
        CONST_SRC_CODE_NEARPLANE_ORG = 0x5,
        CONST_SRC_CODE_NEARPLANE_DX = 0x6,
        CONST_SRC_CODE_NEARPLANE_DY = 0x7,
        CONST_SRC_CODE_SHADOW_PARMS = 0x8,
        CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET = 0x9,
        CONST_SRC_CODE_RENDER_TARGET_SIZE = 0xA,
        CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT = 0xB,
        CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR = 0xC,
        CONST_SRC_CODE_DOF_EQUATION_SCENE = 0xD,
        CONST_SRC_CODE_DOF_LERP_SCALE = 0xE,
        CONST_SRC_CODE_DOF_LERP_BIAS = 0xF,
        CONST_SRC_CODE_DOF_ROW_DELTA = 0x10,
        CONST_SRC_CODE_PARTICLE_CLOUD_COLOR = 0x11,
        CONST_SRC_CODE_GAMETIME = 0x12,
        CONST_SRC_CODE_MAYBE_DIRTY_PS_END = 0x13,
        CONST_SRC_CODE_ALWAYS_DIRTY_PS_BEGIN = 0x13,
        CONST_SRC_CODE_PIXEL_COST_FRACS = 0x13,
        CONST_SRC_CODE_PIXEL_COST_DECODE = 0x14,
        CONST_SRC_CODE_FILTER_TAP_0 = 0x15,
        CONST_SRC_CODE_FILTER_TAP_1 = 0x16,
        CONST_SRC_CODE_FILTER_TAP_2 = 0x17,
        CONST_SRC_CODE_FILTER_TAP_3 = 0x18,
        CONST_SRC_CODE_FILTER_TAP_4 = 0x19,
        CONST_SRC_CODE_FILTER_TAP_5 = 0x1A,
        CONST_SRC_CODE_FILTER_TAP_6 = 0x1B,
        CONST_SRC_CODE_FILTER_TAP_7 = 0x1C,
        CONST_SRC_CODE_COLOR_MATRIX_R = 0x1D,
        CONST_SRC_CODE_COLOR_MATRIX_G = 0x1E,
        CONST_SRC_CODE_COLOR_MATRIX_B = 0x1F,
        CONST_SRC_CODE_ALWAYS_DIRTY_PS_END = 0x20,
        CONST_SRC_CODE_NEVER_DIRTY_PS_BEGIN = 0x20,
        CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x20,
        CONST_SRC_CODE_SHADOWMAP_SCALE = 0x21,
        CONST_SRC_CODE_ZNEAR = 0x22,
        CONST_SRC_CODE_SUN_POSITION = 0x23,
        CONST_SRC_CODE_SUN_DIFFUSE = 0x24,
        CONST_SRC_CODE_SUN_SPECULAR = 0x25,
        CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x26,
        CONST_SRC_CODE_DEBUG_BUMPMAP = 0x27,
        CONST_SRC_CODE_MATERIAL_COLOR = 0x28,
        CONST_SRC_CODE_FOG = 0x29,
        CONST_SRC_CODE_FOG_COLOR = 0x2A,
        CONST_SRC_CODE_GLOW_SETUP = 0x2B,
        CONST_SRC_CODE_GLOW_APPLY = 0x2C,
        CONST_SRC_CODE_COLOR_BIAS = 0x2D,
        CONST_SRC_CODE_COLOR_TINT_BASE = 0x2E,
        CONST_SRC_CODE_COLOR_TINT_DELTA = 0x2F,
        CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x30,
        CONST_SRC_CODE_ENVMAP_PARMS = 0x31,
        CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x32,
        CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x33,
        CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x34,
        CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX = 0x35,
        CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x36,
        CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x37,
        CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x38,
        CONST_SRC_CODE_CODE_MESH_ARG_LAST = 0x38,
        CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x39,
        CONST_SRC_CODE_NEVER_DIRTY_PS_END = 0x3A,
        CONST_SRC_CODE_COUNT_FLOAT4 = 0x3A,
        CONST_SRC_FIRST_CODE_MATRIX = 0x3A,
        CONST_SRC_CODE_WORLD_MATRIX = 0x3A,
        CONST_SRC_CODE_INVERSE_WORLD_MATRIX = 0x3B,
        CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX = 0x3C,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX = 0x3D,
        CONST_SRC_CODE_VIEW_MATRIX = 0x3E,
        CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x3F,
        CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x40,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x41,
        CONST_SRC_CODE_PROJECTION_MATRIX = 0x42,
        CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x43,
        CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x44,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x45,
        CONST_SRC_CODE_WORLD_VIEW_MATRIX = 0x46,
        CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX = 0x47,
        CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x48,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x49,
        CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x4A,
        CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x4B,
        CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x4C,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x4D,
        CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX = 0x4E,
        CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX = 0x4F,
        CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x50,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x51,
        CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x52,
        CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x53,
        CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x54,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x55,
        CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x56,
        CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x57,
        CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x58,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x59,
        CONST_SRC_TOTAL_COUNT = 0x5A,
        CONST_SRC_NONE = 0x5B,
    };

    struct MaterialShaderArgument
    {
        unsigned __int16 type;
        unsigned __int16 dest;
        MaterialArgumentDef u;
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShaderArray[15];
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        unsigned __int8 perPrimArgCount;
        unsigned __int8 perObjArgCount;
        unsigned __int8 stableArgCount;
        unsigned __int8 customSamplerFlags;
        unsigned __int8 precompiledIndex;
        MaterialShaderArgument* args;
    };

    struct MaterialTechnique
    {
        const char* name;
        unsigned __int16 flags;
        unsigned __int16 passCount;
        MaterialPass passArray[1];
    };

    enum MaterialTechniqueType
    {
        TECHNIQUE_DEPTH_PREPASS = 0x0,
        TECHNIQUE_BUILD_FLOAT_Z = 0x1,
        TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
        TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
        TECHNIQUE_UNLIT = 0x4,
        TECHNIQUE_EMISSIVE = 0x5,
        TECHNIQUE_EMISSIVE_SHADOW = 0x6,

        TECHNIQUE_LIT_BEGIN = 0x7,

        TECHNIQUE_LIT = 0x7,
        TECHNIQUE_LIT_SUN = 0x8,
        TECHNIQUE_LIT_SUN_SHADOW = 0x9,
        TECHNIQUE_LIT_SPOT = 0xA,
        TECHNIQUE_LIT_SPOT_SHADOW = 0xB,
        TECHNIQUE_LIT_OMNI = 0xC,
        TECHNIQUE_LIT_OMNI_SHADOW = 0xD,

        TECHNIQUE_LIT_END = 0xE,

        TECHNIQUE_LIGHT_SPOT = 0xE,
        TECHNIQUE_LIGHT_OMNI = 0xF,
        TECHNIQUE_LIGHT_SPOT_SHADOW = 0x10,
        TECHNIQUE_FAKELIGHT_NORMAL = 0x11,
        TECHNIQUE_FAKELIGHT_VIEW = 0x12,
        TECHNIQUE_SUNLIGHT_PREVIEW = 0x13,
        TECHNIQUE_CASE_TEXTURE = 0x14,
        TECHNIQUE_WIREFRAME_SOLID = 0x15,
        TECHNIQUE_WIREFRAME_SHADED = 0x16,
        TECHNIQUE_SHADOWCOOKIE_CASTER = 0x17,
        TECHNIQUE_SHADOWCOOKIE_RECEIVER = 0x18,
        TECHNIQUE_DEBUG_BUMPMAP = 0x19,
        TECHNIQUE_COUNT = 0x1A,
        TECHNIQUE_TOTAL_COUNT = 0x1B,
        TECHNIQUE_NONE = 0x1C,
    };

    struct MaterialTechniqueSet
    {
        const char* name;
        unsigned __int8 worldVertFormat;
        unsigned __int8 unused[2];
        MaterialTechniqueSet* remappedTechniqueSet;
        MaterialTechnique* techniques[26];
    };

#ifndef __zonecodegenerator
} // namespace IW3Xenon
#endif

#endif // __IW3XENON_ASSETS_H
