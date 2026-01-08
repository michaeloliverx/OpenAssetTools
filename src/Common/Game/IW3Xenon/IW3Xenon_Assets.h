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

    struct __declspec(align(4)) PhysPreset
    {
        const char* name;
        int type;
        float mass;
        float bounce;
        float friction;
        float bulletForceScale;
        float explosiveForceScale;
        const char* sndAliasPrefix;
        float piecesSpreadFraction;
        float piecesUpwardVelocity;
        bool tempDefaultToCylinder;
    };

    struct XAnimNotifyInfo
    {
        unsigned __int16 name;
        float time;
    };

    union XAnimDynamicFrames
    {
        unsigned __int8 (*_1)[3];
        unsigned __int16 (*_2)[3];
    };

    union XAnimDynamicIndices
    {
        unsigned __int8 _1[1];
        unsigned __int16 _2[1];
    };

    struct __declspec(align(4)) XAnimPartTransFrames
    {
        float mins[3];
        float size[3];
        XAnimDynamicFrames frames;
        XAnimDynamicIndices indices;
    };

    union XAnimPartTransData
    {
        XAnimPartTransFrames frames;
        float frame0[3];
    };

    struct XAnimPartTrans
    {
        unsigned __int16 size;
        unsigned __int8 smallTrans;
        XAnimPartTransData u;
    };

    struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames
    {
        __int16 (*frames)[2];
        XAnimDynamicIndices indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatDataFrames frames;
        __int16 frame0[2];
    };

    struct XAnimDeltaPartQuat
    {
        unsigned __int16 size;
        XAnimDeltaPartQuatData u;
    };

    struct XAnimDeltaPart
    {
        XAnimPartTrans* trans;
        XAnimDeltaPartQuat* quat;
    };

    union XAnimIndices
    {
        unsigned __int8* _1;
        unsigned __int16* _2;
        void* data;
    };

    struct XAnimParts
    {
        const char* name;
        unsigned __int16 dataByteCount;
        unsigned __int16 dataShortCount;
        unsigned __int16 dataIntCount;
        unsigned __int16 randomDataByteCount;
        unsigned __int16 randomDataIntCount;
        unsigned __int16 numframes;
        bool bLoop;
        bool bDelta;
        unsigned __int8 boneCount[12];
        unsigned __int8 notifyCount;
        unsigned __int8 assetType;
        bool pad;
        unsigned int randomDataShortCount;
        unsigned int indexCount;
        float framerate;
        float frequency;
        ScriptString* names;
        unsigned __int8* dataByte;
        __int16* dataShort;
        int* dataInt;
        __int16* randomDataShort;
        unsigned __int8* randomDataByte;
        int* randomDataInt;
        XAnimIndices indices;
        XAnimNotifyInfo* notify;
        XAnimDeltaPart* deltaPart;
    };

    struct WaterWritable
    {
        float floatTime;
    };

    struct water_t
    {
        WaterWritable writable;
        float* H0X;
        float* H0Y;
        float* wTerm;
        int M;
        int N;
        float Lx;
        float Lz;
        float gravity;
        float windvel;
        float winddir[2];
        float amplitude;
        float codeConstant[4];
        GfxImage* image;
    };

    union MaterialTextureDefInfo
    {
        GfxImage* image;
        water_t* water;
    };

    struct MaterialTextureDef
    {
        unsigned int nameHash;
        char nameStart;
        char nameEnd;
        unsigned __int8 samplerState;
        unsigned __int8 semantic; // TextureSemantic
        MaterialTextureDefInfo u;
    };

    struct MaterialConstantDef
    {
        unsigned int nameHash;
        char name[12];
        float literal[4];
    };

    struct GfxStateBits
    {
        unsigned int loadBits[2];
    };

    struct GfxDrawSurfFields
    {
        unsigned __int64 objectId : 16;
        unsigned __int64 reflectionProbeIndex : 8;
        unsigned __int64 customIndex : 5;
        unsigned __int64 materialSortedIndex : 11;
        unsigned __int64 prepass : 2;
        unsigned __int64 primaryLightIndex : 8;
        unsigned __int64 surfType : 4;
        unsigned __int64 primarySortKey : 6;
        unsigned __int64 unused : 4;
    };

    union GfxDrawSurf
    {
        GfxDrawSurfFields fields;
        unsigned __int64 packed;
    };

    struct __declspec(align(8)) MaterialInfo
    {
        const char* name;
        unsigned __int8 gameFlags;
        unsigned __int8 sortKey;
        unsigned __int8 textureAtlasRowCount;
        unsigned __int8 textureAtlasColumnCount;
        GfxDrawSurf drawSurf;
        unsigned int surfaceTypeBits;
    };

    struct Material
    {
        MaterialInfo info;
        unsigned __int8 stateBitsEntry[26];
        unsigned __int8 textureCount;
        unsigned __int8 constantCount;
        unsigned __int8 stateBitsCount;
        unsigned __int8 stateFlags;
        unsigned __int8 cameraRegion;
        MaterialTechniqueSet* techniqueSet;
        MaterialTextureDef* textureTable;
        MaterialConstantDef* constantTable;
        GfxStateBits* stateBitsTable;
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

    struct GfxImageLoadDef;

    union GfxTexture
    {
        //   D3DBaseTexture *basemap;
        //   D3DTexture *map;
        //   D3DVolumeTexture *volmap;
        //   D3DCubeTexture *cubemap;
        GfxImageLoadDef* loadDef;
    };

    struct GfxImageLoadDef
    {
        unsigned __int8 levelCount;
        unsigned __int8 flags;
        __int16 dimensions[3];
        int format;
        GfxTexture texture;
    };

    enum MapType
    {
        MAPTYPE_NONE = 0x0,
        MAPTYPE_INVALID1 = 0x1,
        MAPTYPE_INVALID2 = 0x2,
        MAPTYPE_2D = 0x3,
        MAPTYPE_3D = 0x4,
        MAPTYPE_CUBE = 0x5,
        MAPTYPE_COUNT = 0x6,
    };

    enum TextureSemantic
    {
        TS_2D = 0x0,
        TS_FUNCTION = 0x1,
        TS_COLOR_MAP = 0x2,
        TS_UNUSED_1 = 0x3,
        TS_UNUSED_2 = 0x4,
        TS_NORMAL_MAP = 0x5,
        TS_UNUSED_3 = 0x6,
        TS_UNUSED_4 = 0x7,
        TS_SPECULAR_MAP = 0x8,
        TS_UNUSED_5 = 0x9,
        TS_UNUSED_6 = 0xA,
        TS_WATER_MAP = 0xB,
    };

    enum ImageCategory
    {
        IMG_CATEGORY_UNKNOWN = 0x0,
        IMG_CATEGORY_AUTO_GENERATED = 0x1,
        IMG_CATEGORY_LIGHTMAP = 0x2,
        IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
        IMG_CATEGORY_RAW = 0x4,
        IMG_CATEGORY_FIRST_UNMANAGED = 0x5,
        IMG_CATEGORY_WATER = 0x5,
        IMG_CATEGORY_RENDERTARGET = 0x6,
        IMG_CATEGORY_TEMP = 0x7,
    };

    struct CardMemory
    {
        int platform[1];
    };

    struct GfxImage
    {
        MapType mapType;
        GfxTexture texture;
        unsigned __int8 semantic;
        CardMemory cardMemory;
        unsigned __int16 width;
        unsigned __int16 height;
        unsigned __int16 depth;
        unsigned __int8 category;
        bool delayLoadPixels;
        unsigned __int8* pixels;
        unsigned int baseSize;
        unsigned __int16 streamSlot;
        bool streaming;
        const char* name;
    };

    struct XAUDIOCHANNELMAPENTRY
    {
        unsigned __int8 InputChannel;
        unsigned __int8 OutputChannel;
        float Volume;
    };

    struct XAUDIOCHANNELMAP
    {
        unsigned __int8 EntryCount;
        XAUDIOCHANNELMAPENTRY* paEntries;
    };

    struct SpeakerMap
    {
        bool isDefault;
        const char* name;
        XAUDIOCHANNELMAP channelMaps[2][2];
    };

    struct StreamFileNameRaw
    {
        const char* dir;
        const char* name;
    };

    struct StreamFileNamePacked
    {
        unsigned int offset;
        unsigned int length;
    };

    union StreamFileInfo
    {
        StreamFileNameRaw raw;
        StreamFileNamePacked packed;
    };

    struct StreamFileName
    {
        unsigned int fileIndex;
        StreamFileInfo info;
    };

    struct StreamedSound
    {
        StreamFileName filename;
    };

    union SoundFileRef
    {
        LoadedSound* loadSnd;
        StreamedSound streamSnd;
    };

    enum snd_alias_type_t
    {
        SAT_UNKNOWN = 0x0,
        SAT_LOADED = 0x1,
        SAT_STREAMED = 0x2,
        SAT_COUNT = 0x3,
    };

    struct SoundFile
    {
        unsigned __int8 type;
        unsigned __int8 exists;
        SoundFileRef u;
    };

    struct snd_alias_t
    {
        const char* aliasName;
        const char* subtitle;
        const char* secondaryAliasName;
        const char* chainAliasName;
        SoundFile* soundFile;
        int sequence;
        float volMin;
        float volMax;
        float pitchMin;
        float pitchMax;
        float distMin;
        float distMax;
        int flags;
        float slavePercentage;
        float probability;
        float lfePercentage;
        float centerPercentage;
        int startDelay;
        SndCurve* volumeFalloffCurve;
        float envelopMin;
        float envelopMax;
        float envelopPercentage;
        SpeakerMap* speakerMap;
    };

    struct snd_alias_list_t
    {
        const char* aliasName;
        snd_alias_t* head;
        int count;
    };

    struct SndCurve
    {
        const char* filename;
        int knotCount;
        float knots[8][2];
    };

    struct __declspec(align(4)) XMALOOPREGION
    {
        unsigned int LoopStart;
        unsigned int LoopEnd;
        unsigned __int8 LoopSubframeEnd;
        unsigned __int8 LoopSubframeSkip;
    };

    struct XAUDIOPACKET_ALIGNED
    {
        char* pBuffer;
        unsigned int BufferSize;
        unsigned int LoopCount;
        XMALOOPREGION XMALoop[6];
        char* pContext;
    };

    union XAUDIOSOURCEFORMAT_u1
    {
        unsigned __int8 NumStreams;
        unsigned __int8 ChannelCount;
    };

    struct __declspec(align(4)) XAUDIOXMAFORMAT
    {
        unsigned int SampleRate;
        unsigned __int8 ChannelCount;
        unsigned __int8 DecodeBufferSize;
    };

    union __declspec(align(4)) XAUDIOSOURCEFORMAT_u2
    {
        XAUDIOXMAFORMAT Stream[6];
        unsigned int SampleRate;
    };

    struct XAUDIOSOURCEFORMAT
    {
        unsigned __int8 SampleType;
        XAUDIOSOURCEFORMAT_u1 ___u1;
        XAUDIOSOURCEFORMAT_u2 ___u2;
    };

    struct XaIwXmaDataInfo
    {
        int totalMsec;
    };

    struct XaSeekTable
    {
        int size;
        unsigned int* data;
    };

    struct XaSound
    {
        XAUDIOPACKET_ALIGNED packet;
        XAUDIOSOURCEFORMAT format;
        XaIwXmaDataInfo xaIwXmaDataInfo;
        XaSeekTable seekTable;
    };

    struct LoadedSound
    {
        const char* name;
        XaSound sound;
    };

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int numEntityChars;
    };

    struct __declspec(align(4)) GfxLightImage
    {
        GfxImage* image;
        unsigned __int8 samplerState;
    };

    struct GfxLightDef
    {
        const char* name;
        GfxLightImage attenuation;
        int lmapLookupStart;
    };

    struct Glyph
    {
        unsigned __int16 letter;
        char x0;
        char y0;
        unsigned __int8 dx;
        unsigned __int8 pixelWidth;
        unsigned __int8 pixelHeight;
        float s0;
        float t0;
        float s1;
        float t1;
    };

    struct Font_s
    {
        const char* fontName;
        int pixelHeight;
        int glyphCount;
        Material* material;
        Material* glowMaterial;
        Glyph* glyphs;
    };

    struct MenuList
    {
        const char* name;
        int menuCount;
        menuDef_t** menus;
    };

    struct columnInfo_s
    {
        int pos;
        int width;
        int maxChars;
        int alignment;
    };

    struct listBoxDef_s
    {
        int startPos[4];
        int endPos[4];
        int drawPadding;
        float elementWidth;
        float elementHeight;
        int elementStyle;
        int numColumns;
        columnInfo_s columnInfo[16];
        const char* doubleClick;
        int notselectable;
        int noScrollBars;
        int usePaging;
        float selectBorder[4];
        float disableColor[4];
        Material* selectIcon;
    };

    struct editFieldDef_s
    {
        float minVal;
        float maxVal;
        float defVal;
        float range;
        int maxChars;
        int maxCharsGotoNext;
        int maxPaintChars;
        int paintOffset;
    };

    enum operationEnum
    {
        OP_NOOP = 0x0,
        OP_RIGHTPAREN = 0x1,
        OP_MULTIPLY = 0x2,
        OP_DIVIDE = 0x3,
        OP_MODULUS = 0x4,
        OP_ADD = 0x5,
        OP_SUBTRACT = 0x6,
        OP_NOT = 0x7,
        OP_LESSTHAN = 0x8,
        OP_LESSTHANEQUALTO = 0x9,
        OP_GREATERTHAN = 0xA,
        OP_GREATERTHANEQUALTO = 0xB,
        OP_EQUALS = 0xC,
        OP_NOTEQUAL = 0xD,
        OP_AND = 0xE,
        OP_OR = 0xF,
        OP_LEFTPAREN = 0x10,
        OP_COMMA = 0x11,
        OP_BITWISEAND = 0x12,
        OP_BITWISEOR = 0x13,
        OP_BITWISENOT = 0x14,
        OP_BITSHIFTLEFT = 0x15,
        OP_BITSHIFTRIGHT = 0x16,
        OP_SIN = 0x17,
        OP_FIRSTFUNCTIONCALL = 0x17,
        OP_COS = 0x18,
        OP_MIN = 0x19,
        OP_MAX = 0x1A,
        OP_MILLISECONDS = 0x1B,
        OP_DVARINT = 0x1C,
        OP_DVARBOOL = 0x1D,
        OP_DVARFLOAT = 0x1E,
        OP_DVARSTRING = 0x1F,
        OP_STAT = 0x20,
        OP_UIACTIVE = 0x21,
        OP_FLASHBANGED = 0x22,
        OP_SCOPED = 0x23,
        OP_SCOREBOARDVISIBLE = 0x24,
        OP_INKILLCAM = 0x25,
        OP_PLAYERFIELD = 0x26,
        OP_SELECTINGLOCATION = 0x27,
        OP_TEAMFIELD = 0x28,
        OP_OTHERTEAMFIELD = 0x29,
        OP_MARINESFIELD = 0x2A,
        OP_OPFORFIELD = 0x2B,
        OP_MENUISOPEN = 0x2C,
        OP_WRITINGDATA = 0x2D,
        OP_INLOBBY = 0x2E,
        OP_INPRIVATEPARTY = 0x2F,
        OP_PRIVATEPARTYHOST = 0x30,
        OP_PRIVATEPARTYHOSTINLOBBY = 0x31,
        OP_ALONEINPARTY = 0x32,
        OP_ADSJAVELIN = 0x33,
        OP_WEAPLOCKBLINK = 0x34,
        OP_WEAPATTACKTOP = 0x35,
        OP_WEAPATTACKDIRECT = 0x36,
        OP_SECONDSASTIME = 0x37,
        OP_TABLELOOKUP = 0x38,
        OP_LOCALIZESTRING = 0x39,
        OP_LOCALVARINT = 0x3A,
        OP_LOCALVARBOOL = 0x3B,
        OP_LOCALVARFLOAT = 0x3C,
        OP_LOCALVARSTRING = 0x3D,
        OP_TIMELEFT = 0x3E,
        OP_SECONDSASCOUNTDOWN = 0x3F,
        OP_GAMEMSGWNDACTIVE = 0x40,
        OP_TOINT = 0x41,
        OP_TOSTRING = 0x42,
        OP_TOFLOAT = 0x43,
        OP_GAMETYPENAME = 0x44,
        OP_GAMETYPE = 0x45,
        OP_GAMETYPEDESCRIPTION = 0x46,
        OP_SCORE = 0x47,
        OP_FRIENDSONLINE = 0x48,
        OP_FOLLOWING = 0x49,
        OP_STATRANGEBITSSET = 0x4A,
        OP_KEYBINDING = 0x4B,
        OP_ACTIONSLOTUSABLE = 0x4C,
        OP_HUDFADE = 0x4D,
        OP_MAXPLAYERS = 0x4E,
        OP_ACCEPTINGINVITE = 0x4F,
        NUM_OPERATORS = 0x50,
    };

    enum expDataType
    {
        VAL_INT = 0x0,
        VAL_FLOAT = 0x1,
        VAL_STRING = 0x2,
    };

    union operandInternalDataUnion
    {
        int intVal;
        float floatVal;
        const char* string;
    };

    struct Operand
    {
        expDataType dataType;
        operandInternalDataUnion internals;
    };

    union entryInternalData
    {
        operationEnum op;
        Operand operand;
    };

    enum expressionEntryType
    {
        EET_OPERATOR = 0x0,
        EET_OPERAND = 0x1,
    };

    struct expressionEntry
    {
        int type;
        entryInternalData data;
    };

    struct ItemKeyHandler
    {
        int key;
        const char* action;
        ItemKeyHandler* next;
    };

    struct rectDef_s
    {
        float x;
        float y;
        float w;
        float h;
        int horzAlign;
        int vertAlign;
    };

    struct windowDef_t
    {
        const char* name;
        rectDef_s rect;
        rectDef_s rectClient;
        const char* group;
        int style;
        int border;
        int ownerDraw;
        int ownerDrawFlags;
        float borderSize;
        int staticFlags;
        int dynamicFlags[4];
        int nextTime;
        float foreColor[4];
        float backColor[4];
        float borderColor[4];
        float outlineColor[4];
        Material* background;
    };

    struct statement_s
    {
        int numEntries;
        expressionEntry** entries;
    };

    struct multiDef_s
    {
        const char* dvarList[32];
        const char* dvarStr[32];
        float dvarValue[32];
        int count;
        int strDef;
    };

    union itemDefData_t
    {
        listBoxDef_s* listBox;
        editFieldDef_s* editField;
        multiDef_s* multi;
        const char* enumDvarName;
        void* data;
    };

    enum ItemDefType
    {
        ITEM_TYPE_TEXT = 0x0,
        ITEM_TYPE_BUTTON = 0x1,
        ITEM_TYPE_RADIOBUTTON = 0x2,
        ITEM_TYPE_CHECKBOX = 0x3,
        ITEM_TYPE_EDITFIELD = 0x4,
        ITEM_TYPE_COMBO = 0x5,
        ITEM_TYPE_LISTBOX = 0x6,
        ITEM_TYPE_MODEL = 0x7,
        ITEM_TYPE_OWNERDRAW = 0x8,
        ITEM_TYPE_NUMERICFIELD = 0x9,
        ITEM_TYPE_SLIDER = 0xA,
        ITEM_TYPE_YESNO = 0xB,
        ITEM_TYPE_MULTI = 0xC,
        ITEM_TYPE_DVARENUM = 0xD,
        ITEM_TYPE_BIND = 0xE,
        ITEM_TYPE_MENUMODEL = 0xF,
        ITEM_TYPE_VALIDFILEFIELD = 0x10,
        ITEM_TYPE_DECIMALFIELD = 0x11,
        ITEM_TYPE_UPREDITFIELD = 0x12,
        ITEM_TYPE_GAME_MESSAGE_WINDOW = 0x13
    };

    struct itemDef_s
    {
        windowDef_t window;
        rectDef_s textRect[4];
        int type;
        int dataType;
        int alignment;
        int fontEnum;
        int textAlignMode;
        float textalignx;
        float textaligny;
        float textscale;
        int textStyle;
        int gameMsgWindowIndex;
        int gameMsgWindowMode;
        const char* text;
        int itemFlags;
        menuDef_t* parent;
        const char* mouseEnterText;
        const char* mouseExitText;
        const char* mouseEnter;
        const char* mouseExit;
        const char* action;
        const char* onAccept;
        const char* onFocus;
        const char* leaveFocus;
        const char* dvar;
        const char* dvarTest;
        ItemKeyHandler* onKey;
        const char* enableDvar;
        int dvarFlags;
        snd_alias_list_t* focusSound;
        float special;
        int cursorPos[4];
        itemDefData_t typeData;
        int imageTrack;
        statement_s visibleExp;
        statement_s textExp;
        statement_s materialExp;
        statement_s rectXExp;
        statement_s rectYExp;
        statement_s rectWExp;
        statement_s rectHExp;
        statement_s forecolorAExp;
    };

    struct menuDef_t
    {
        windowDef_t window;
        const char* font;
        int fullScreen;
        int itemCount;
        int fontIndex;
        int cursorItem[4];
        int fadeCycle;
        float fadeClamp;
        float fadeAmount;
        float fadeInAmount;
        float blurRadius;
        const char* onOpen;
        const char* onClose;
        const char* onESC;
        ItemKeyHandler* onKey;
        statement_s visibleExp;
        const char* allowedBinding;
        const char* soundName;
        int imageTrack;
        float focusColor[4];
        float disableColor[4];
        statement_s rectXExp;
        statement_s rectYExp;
        itemDef_s** items;
    };

    struct LocalizeEntry
    {
        const char* value;
        const char* name;
    };

    struct XAUDIOREVERBSETTINGS
    {
        unsigned int ReflectionsDelay;
        unsigned __int8 ReverbDelay;
        unsigned __int8 RearDelay;
        unsigned __int8 PositionLeft;
        unsigned __int8 PositionRight;
        unsigned __int8 PositionMatrixLeft;
        unsigned __int8 PositionMatrixRight;
        unsigned __int8 EarlyDiffusion;
        unsigned __int8 LateDiffusion;
        unsigned __int8 LowEQGain;
        unsigned __int8 LowEQCutoff;
        unsigned __int8 HighEQGain;
        unsigned __int8 HighEQCutoff;
        float RoomFilterFreq;
        float RoomFilterMain;
        float RoomFilterHF;
        float ReflectionsGain;
        float ReverbGain;
        float DecayTime;
        float Density;
        float RoomSize;
    };

    struct XaReverbSettings
    {
        // int presetOverridden;
        // XAUDIOREVERBSETTINGS reverbSettings;
        // The engine loader loads a chunk this size idk why
        uint8_t data[0x548];
    };

    struct SndDriverGlobals
    {
        XaReverbSettings* reverbSettings;
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

#ifndef __zonecodegenerator
} // namespace IW3Xenon
#endif

#endif // __IW3XENON_ASSETS_H
