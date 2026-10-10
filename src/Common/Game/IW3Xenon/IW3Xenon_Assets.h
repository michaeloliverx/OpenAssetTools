#pragma once

#ifndef __IW3XENON_ASSETS_H
#define __IW3XENON_ASSETS_H

#include "../../Platform/Xenon/XenonGraphics.h"
#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace IW3Xenon
{
    using oat::xenon::GPUENDIAN;
    using oat::xenon::GPUTEXTUREFORMAT;
    using enum oat::xenon::GPUTEXTUREFORMAT;
    using enum oat::xenon::GPUENDIAN;
#endif

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

    struct XModelPieces;
    struct PhysPreset;
    struct XAnimParts;
    struct XModel;
    struct Material;
    struct MaterialPixelShader;
    struct MaterialVertexShader;
    struct MaterialTechniqueSet;
    struct GfxImage;
    struct snd_alias_list_t;
    struct SndCurve;
    struct LoadedSound;
    struct clipMap_t;
    struct ComWorld;
    struct GameWorldSp;
    struct GameWorldMp;
    struct MapEnts;
    struct GfxWorld;
    struct GfxLightDef;
    struct Font_s;
    struct MenuList;
    struct menuDef_t;
    struct LocalizeEntry;
    struct WeaponDef;
    struct SndDriverGlobals;
    struct FxEffectDef;
    struct FxImpactTable;
    struct RawFile;
    struct StringTable;

    typedef unsigned short ScriptString;

    union XAssetHeader
    {
        XModelPieces* xmodelPieces;
        PhysPreset* physPreset;
        XAnimParts* parts;
        XModel* model;
        Material* material;
        MaterialPixelShader* pixelShader;
        MaterialVertexShader* vertexShader;
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
        FxEffectDef* fx;
        FxImpactTable* impactFx;
        RawFile* rawfile;
        StringTable* stringTable;
        void* data;
    };

    union vec2_t
    {
        float v[2];

        struct
        {
            float x;
            float y;
        };
    };

    union vec3_t
    {
        struct
        {
            float x;
            float y;
            float z;
        };

        float v[3];
    };

    union vec4_t
    {
        float v[4];

        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
    };

    struct XModelPiece
    {
        XModel* model;
        float offset[3];
    };

    struct XModelPieces
    {
        const char* name;
        int numpieces;
        XModelPiece* pieces;
    };

    struct type_align32(4) PhysPreset
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
        ScriptString name;
        float time;
    };

    union XAnimDynamicFrames
    {
        uint8_t (*_1)[3];
        uint16_t (*_2)[3];
    };

    union XAnimDynamicIndicesTrans
    {
        uint8_t _1[1];
        uint16_t _2[1];
    };

    union XAnimDynamicIndicesQuat
    {
        uint8_t _1[1];
        uint16_t _2[1];
    };

    struct type_align32(4) XAnimPartTransFrames
    {
        float mins[3];
        float size[3];
        XAnimDynamicFrames frames;
        XAnimDynamicIndicesTrans indices;
    };

    union XAnimPartTransData
    {
        XAnimPartTransFrames frames;
        float frame0[3];
    };

    struct XAnimPartTrans
    {
        uint16_t size;
        uint8_t smallTrans;
        XAnimPartTransData u;
    };

    struct type_align32(4) XAnimDeltaPartQuatDataFrames
    {
        int16_t (*frames)[2];
        XAnimDynamicIndicesQuat indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatDataFrames frames;
        int16_t frame0[2];
    };

    struct XAnimDeltaPartQuat
    {
        uint16_t size;
        XAnimDeltaPartQuatData u;
    };

    struct XAnimDeltaPart
    {
        XAnimPartTrans* trans;
        XAnimDeltaPartQuat* quat;
    };

    union XAnimIndices
    {
        uint8_t* _1;
        uint16_t* _2;
        void* data;
    };

    enum XAnimPartType
    {
        PART_TYPE_NO_QUAT = 0x0,
        PART_TYPE_SIMPLE_QUAT = 0x1,
        PART_TYPE_NORMAL_QUAT = 0x2,
        PART_TYPE_PRECISION_QUAT = 0x3,
        PART_TYPE_SIMPLE_QUAT_NO_SIZE = 0x4,
        PART_TYPE_NORMAL_QUAT_NO_SIZE = 0x5,
        PART_TYPE_PRECISION_QUAT_NO_SIZE = 0x6,
        PART_TYPE_SMALL_TRANS = 0x7,
        PART_TYPE_TRANS = 0x8,
        PART_TYPE_TRANS_NO_SIZE = 0x9,
        PART_TYPE_NO_TRANS = 0xA,
        PART_TYPE_ALL = 0xB,

        PART_TYPE_COUNT
    };

    struct XAnimParts
    {
        const char* name;
        uint16_t dataByteCount;
        uint16_t dataShortCount;
        uint16_t dataIntCount;
        uint16_t randomDataByteCount;
        uint16_t randomDataIntCount;
        uint16_t numframes;
        bool bLoop;
        bool bDelta;
        uint8_t boneCount[PART_TYPE_COUNT];
        uint8_t notifyCount;
        uint8_t assetType;
        bool pad;
        unsigned int randomDataShortCount;
        unsigned int indexCount;
        float framerate;
        float frequency;
        ScriptString* names;
        uint8_t* dataByte;
        int16_t* dataShort;
        int* dataInt;
        int16_t* randomDataShort;
        uint8_t* randomDataByte;
        int* randomDataInt;
        XAnimIndices indices;
        XAnimNotifyInfo* notify;
        XAnimDeltaPart* deltaPart;
    };

    struct DObjAnimMat
    {
        vec4_t quat;
        vec3_t trans;
        float transWeight;
    };

    struct DObjSkelMat
    {
        float axis[3][4];
        float origin[4];
    };

    union GfxColor
    {
        unsigned int packed;
        uint8_t array[4];
    };

    union PackedTexCoords
    {
        unsigned int packed;
    };

    union PackedUnitVec
    {
        unsigned int packed;
    };

    struct GfxPackedVertex
    {
        vec3_t xyz;
        float binormalSign;
        GfxColor color;
        PackedTexCoords texCoord;
        PackedUnitVec normal;
        PackedUnitVec tangent;
    };

    struct XSurfaceCollisionAabb
    {
        uint16_t mins[3];
        uint16_t maxs[3];
    };

    struct XSurfaceCollisionNode
    {
        XSurfaceCollisionAabb aabb;
        uint16_t childBeginIndex;
        uint16_t childCount;
    };

    struct XSurfaceCollisionLeaf
    {
        uint16_t triangleBeginIndex;
    };

    struct XSurfaceCollisionTree
    {
        vec3_t trans;
        vec3_t scale;
        unsigned int nodeCount;
        XSurfaceCollisionNode* nodes;
        unsigned int leafCount;
        XSurfaceCollisionLeaf* leafs;
    };

    struct XRigidVertList
    {
        uint16_t boneOffset;
        uint16_t vertCount;
        uint16_t triOffset;
        uint16_t triCount;
        XSurfaceCollisionTree* collisionTree;
    };

    struct XSurfaceVertexInfo
    {
        int16_t vertCount[4];
        uint16_t* vertsBlend;
    };

    struct XSurfaceTri
    {
        uint16_t i[3];
    };

    typedef tdef_align32(16) XSurfaceTri XSurfaceTri16;

    struct D3DVertexBuffer
    {
        char data[0x20];
    };

    struct D3DIndexBuffer
    {
        char data[0x20];
    };

    struct XSurface
    {
        uint8_t tileMode;
        bool deformed;
        uint16_t vertCount;
        uint16_t triCount;
        XSurfaceTri16* triIndices;
        XSurfaceVertexInfo vertInfo;
        GfxPackedVertex* verts0;
        D3DVertexBuffer vb0;
        unsigned int vertListCount;
        XRigidVertList* vertList;
        D3DIndexBuffer indexBuffer;
        int partBits[4];
    };

    struct XModelCollSurf_s
    {
        float mins[3];
        float maxs[3];
        int boneIdx;
        int contents;
        int surfFlags;
    };

    struct XBoneInfo
    {
        vec3_t bounds[2];
        vec3_t offset;
        float radiusSquared;
    };

    struct XModelHighMipBounds
    {
        vec3_t mins;
        vec3_t maxs;
    };

    enum hitLocation_t
    {
        HITLOC_NONE = 0x0,
        HITLOC_HELMET = 0x1,
        HITLOC_HEAD = 0x2,
        HITLOC_NECK = 0x3,
        HITLOC_TORSO_UPR = 0x4,
        HITLOC_TORSO_LWR = 0x5,
        HITLOC_R_ARM_UPR = 0x6,
        HITLOC_L_ARM_UPR = 0x7,
        HITLOC_R_ARM_LWR = 0x8,
        HITLOC_L_ARM_LWR = 0x9,
        HITLOC_R_HAND = 0xA,
        HITLOC_L_HAND = 0xB,
        HITLOC_R_LEG_UPR = 0xC,
        HITLOC_L_LEG_UPR = 0xD,
        HITLOC_R_LEG_LWR = 0xE,
        HITLOC_L_LEG_LWR = 0xF,
        HITLOC_R_FOOT = 0x10,
        HITLOC_L_FOOT = 0x11,
        HITLOC_GUN = 0x12,

        HITLOC_COUNT,
    };

    struct cplane_s
    {
        float normal[3];
        float dist;
        uint8_t type;
        uint8_t signbits;
        uint8_t pad[2];
    };

    struct cbrushside_t
    {
        cplane_s* plane;
        unsigned int materialNum;
        int16_t firstAdjacentSideOffset;
        uint8_t edgeCount;
    };

    struct BrushWrapper
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        int16_t axialMaterialNum[2][3];
        uint8_t* baseAdjacentSide;
        int16_t firstAdjacentSideOffsets[2][3];
        uint8_t edgeCount[2][3];
        int totalEdgeCount;
        cplane_s* planes;
    };

    struct PhysGeomInfo
    {
        BrushWrapper* brush;
        int type;
        float orientation[3][3];
        float offset[3];
        float halfLengths[3];
    };

    struct PhysMass
    {
        float centerOfMass[3];
        float momentsOfInertia[3];
        float productsOfInertia[3];
    };

    struct PhysGeomList
    {
        unsigned int count;
        PhysGeomInfo* geoms;
        PhysMass mass;
    };

    struct XModelLodInfo
    {
        float dist;
        uint16_t numsurfs;
        uint16_t surfIndex;
        int partBits[4];
    };

    struct XModelStreamInfo
    {
        XModelHighMipBounds* highMipBounds;
    };

    struct XModelQuat
    {
        int16_t v[4];
    };

    struct XModel
    {
        const char* name;
        uint8_t numBones;
        uint8_t numRootBones;
        uint8_t numsurfs;
        uint8_t lodRampType;
        ScriptString* boneNames;
        uint8_t* parentList;
        XModelQuat* quats;
        float* trans;
        uint8_t* partClassification;
        DObjAnimMat* baseMat;
        XSurface* surfs;
        Material** materialHandles;
        XModelLodInfo lodInfo[4];
        XModelCollSurf_s* collSurfs;
        int numCollSurfs;
        int contents;
        XBoneInfo* boneInfo;
        float radius;
        vec3_t mins;
        vec3_t maxs;
        uint16_t numLods;
        int16_t collLod;
        XModelStreamInfo streamInfo;
        int memUsage;
        uint8_t flags;
        PhysPreset* physPreset;
        PhysGeomList* physGeoms;
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
        uint8_t samplerState;
        uint8_t semantic; // TextureSemantic
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
        uint64_t objectId : 16;
        uint64_t reflectionProbeIndex : 8;
        uint64_t customIndex : 5;
        uint64_t materialSortedIndex : 11;
        uint64_t prepass : 2;
        uint64_t primaryLightIndex : 8;
        uint64_t surfType : 4;
        uint64_t primarySortKey : 6;
        uint64_t unused : 4;
    };

    union GfxDrawSurf
    {
        GfxDrawSurfFields fields;
        uint64_t packed;
    };

    struct type_align32(8) MaterialInfo
    {
        const char* name;
        uint8_t gameFlags;
        uint8_t sortKey;
        uint8_t textureAtlasRowCount;
        uint8_t textureAtlasColumnCount;
        GfxDrawSurf drawSurf;
        unsigned int surfaceTypeBits;
    };

    struct Material
    {
        MaterialInfo info;
        uint8_t stateBitsEntry[26];
        uint8_t textureCount;
        uint8_t constantCount;
        uint8_t stateBitsCount;
        uint8_t stateFlags;
        uint8_t cameraRegion;
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
        uint8_t source;
        uint8_t dest;
    };

    enum MaterialVertexDeclType
    {
        VERTDECL_GENERIC = 0x0,
        VERTDECL_PACKED = 0x1,
        VERTDECL_WORLD = 0x2,
        VERTDECL_WORLD_T1N0 = 0x3,
        VERTDECL_WORLD_T1N1 = 0x4,
        VERTDECL_WORLD_T2N0 = 0x5,
        VERTDECL_WORLD_T2N1 = 0x6,
        VERTDECL_WORLD_T2N2 = 0x7,
        VERTDECL_WORLD_T3N0 = 0x8,
        VERTDECL_WORLD_T3N1 = 0x9,
        VERTDECL_WORLD_T3N2 = 0xA,
        VERTDECL_WORLD_T4N0 = 0xB,
        VERTDECL_WORLD_T4N1 = 0xC,
        VERTDECL_WORLD_T4N2 = 0xD,
        VERTDECL_POS_TEX = 0xE,
        VERTDECL_COUNT = 0xF,
    };

    union MaterialVertexStreamRouting
    {
        MaterialStreamRouting data[16];
        void /*D3DVertexDeclaration*/* decl[VERTDECL_COUNT];
    };

    struct MaterialVertexDeclaration
    {
        uint8_t streamCount;
        bool hasOptionalSource;
        MaterialVertexStreamRouting routing;
    };

    struct GfxVertexShaderLoadDef
    {
        uint8_t* cachedPart;
        uint8_t* physicalPart;
        uint16_t cachedPartSize;
        uint16_t physicalPartSize;
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
        uint8_t* cachedPart;
        uint8_t* physicalPart;
        uint16_t cachedPartSize;
        uint16_t physicalPartSize;
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
        uint16_t index;
        uint8_t firstRow;
        uint8_t rowCount;
    };

    union MaterialArgumentDef
    {
        float (*literalConst)[4];
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

    enum MaterialTextureSource
    {
        TEXTURE_SRC_CODE_BLACK = 0x0,
        TEXTURE_SRC_CODE_WHITE = 0x1,
        TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP = 0x2,
        TEXTURE_SRC_CODE_MODEL_LIGHTING = 0x3,
        TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY = 0x4,
        TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY = 0x5,
        TEXTURE_SRC_CODE_SHADOWCOOKIE = 0x6,
        TEXTURE_SRC_CODE_SHADOWMAP_SUN = 0x7,
        TEXTURE_SRC_CODE_SHADOWMAP_SPOT = 0x8,
        TEXTURE_SRC_CODE_FEEDBACK = 0x9,
        TEXTURE_SRC_CODE_RESOLVED_POST_SUN = 0xA,
        TEXTURE_SRC_CODE_RESOLVED_SCENE = 0xB,
        TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xC,
        TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xD,
        TEXTURE_SRC_CODE_SKY = 0xE,
        TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0xF,
        TEXTURE_SRC_CODE_DYNAMIC_SHADOWS = 0x10,
        TEXTURE_SRC_CODE_OUTDOOR = 0x11,
        TEXTURE_SRC_CODE_FLOATZ = 0x12,
        TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x13,
        TEXTURE_SRC_CODE_RAW_FLOATZ = 0x14,
        TEXTURE_SRC_CODE_CASE_TEXTURE = 0x15,
        TEXTURE_SRC_CODE_CINEMATIC_Y = 0x16,
        TEXTURE_SRC_CODE_CINEMATIC_CR = 0x17,
        TEXTURE_SRC_CODE_CINEMATIC_CB = 0x18,
        TEXTURE_SRC_CODE_CINEMATIC_A = 0x19,
        TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x1A,
        TEXTURE_SRC_CODE_COUNT = 0x1B,
    };

    enum CustomSamplers
    {
        CUSTOM_SAMPLER_REFLECTION_PROBE = 0x0,
        CUSTOM_SAMPLER_LIGHTMAP_PRIMARY = 0x1,
        CUSTOM_SAMPLER_LIGHTMAP_SECONDARY = 0x2,
        CUSTOM_SAMPLER_COUNT = 0x3,
    };

    enum MaterialUpdateFrequency
    {
        MTL_UPDATE_PER_PRIM = 0x0,
        MTL_UPDATE_PER_OBJECT = 0x1,
        MTL_UPDATE_RARELY = 0x2,
        MTL_UPDATE_CUSTOM = 0x3,
    };

    struct MaterialShaderArgument
    {
        uint16_t type;
        uint16_t dest;
        MaterialArgumentDef u;
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShaderArray[VERTDECL_COUNT];
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        uint8_t perPrimArgCount;
        uint8_t perObjArgCount;
        uint8_t stableArgCount;
        uint8_t customSamplerFlags;
        uint8_t precompiledIndex;
        MaterialShaderArgument* args;
    };

    struct MaterialTechnique
    {
        const char* name;
        uint16_t flags;
        uint16_t passCount;
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

    enum MaterialWorldVertexFormat : uint8_t
    {
        MTL_WORLDVERT_TEX_1_NRM_1 = 0x0,
        MTL_WORLDVERT_TEX_2_NRM_1 = 0x1,
        MTL_WORLDVERT_TEX_2_NRM_2 = 0x2,
        MTL_WORLDVERT_TEX_3_NRM_1 = 0x3,
        MTL_WORLDVERT_TEX_3_NRM_2 = 0x4,
        MTL_WORLDVERT_TEX_3_NRM_3 = 0x5,
        MTL_WORLDVERT_TEX_4_NRM_1 = 0x6,
        MTL_WORLDVERT_TEX_4_NRM_2 = 0x7,
        MTL_WORLDVERT_TEX_4_NRM_3 = 0x8,
        MTL_WORLDVERT_TEX_5_NRM_1 = 0x9,
        MTL_WORLDVERT_TEX_5_NRM_2 = 0xA,
        MTL_WORLDVERT_TEX_5_NRM_3 = 0xB,
    };

    struct MaterialTechniqueSet
    {
        const char* name;
        uint8_t worldVertFormat;
        uint8_t unused[2];
        MaterialTechniqueSet* remappedTechniqueSet;
        MaterialTechnique* techniques[26];
    };

    struct GPUTEXTURESIZE_1D
    {
        uint32_t Width : 24;
        uint32_t Unused : 8;
    };

    struct GPUTEXTURESIZE_2D
    {
        uint32_t Width : 13;
        uint32_t Height : 13;
        uint32_t Unused : 6;
    };

    struct GPUTEXTURESIZE_STACK
    {
        uint32_t Width : 13;
        uint32_t Height : 13;
        uint32_t Depth : 6;
    };

    struct GPUTEXTURESIZE_3D
    {
        uint32_t Width : 11;
        uint32_t Height : 11;
        uint32_t Depth : 10;
    };

    // Matches the XDK GPU fetch constant. The raw words are stored in GPU
    // little-endian order in the fastfile, so code that needs host values must
    // decode them explicitly rather than using the bit fields directly.
    union GPUTEXTURE_FETCH_CONSTANT
    {
        struct
        {
            // DWORD 0
            uint32_t Type : 2;
            uint32_t SignX : 2;
            uint32_t SignY : 2;
            uint32_t SignZ : 2;
            uint32_t SignW : 2;
            uint32_t ClampX : 3;
            uint32_t ClampY : 3;
            uint32_t ClampZ : 3;
            uint32_t Unused0 : 2;
            uint32_t Unused1 : 1;
            uint32_t Pitch : 9;
            uint32_t Tiled : 1;

            // DWORD 1
            uint32_t DataFormat : 6;
            uint32_t Endian : 2;
            uint32_t RequestSize : 2;
            uint32_t Stacked : 1;
            uint32_t ClampPolicy : 1;
            uint32_t BaseAddress : 20;

            // DWORD 2
            union
            {
                GPUTEXTURESIZE_1D OneD;
                GPUTEXTURESIZE_2D TwoD;
                GPUTEXTURESIZE_3D ThreeD;
                GPUTEXTURESIZE_STACK Stack;
            } Size;

            // DWORD 3
            uint32_t NumFormat : 1;
            uint32_t SwizzleX : 3;
            uint32_t SwizzleY : 3;
            uint32_t SwizzleZ : 3;
            uint32_t SwizzleW : 3;
            int32_t ExpAdjust : 6;
            uint32_t MagFilter : 2;
            uint32_t MinFilter : 2;
            uint32_t MipFilter : 2;
            uint32_t AnisoFilter : 3;
            uint32_t Unused2 : 3;
            uint32_t BorderSize : 1;

            // DWORD 4
            uint32_t VolMagFilter : 1;
            uint32_t VolMinFilter : 1;
            uint32_t MinMipLevel : 4;
            uint32_t MaxMipLevel : 4;
            uint32_t MagAnisoWalk : 1;
            uint32_t MinAnisoWalk : 1;
            int32_t LODBias : 10;
            int32_t GradExpAdjustH : 5;
            int32_t GradExpAdjustV : 5;

            // DWORD 5
            uint32_t BorderColor : 2;
            uint32_t ForceBCWToMax : 1;
            uint32_t TriClamp : 2;
            int32_t AnisoBias : 4;
            uint32_t Dimension : 2;
            uint32_t PackedMips : 1;
            uint32_t MipAddress : 20;
        } fields;

        uint32_t dword[6];
    };

    // This is the C layout of the XDK D3DBaseTexture used in the fastfile. It
    // is serialized metadata, not a live C++ D3D interface object.
    struct type_align(4) D3DBaseTexture
    {
        uint32_t Common;
        uint32_t ReferenceCount;
        uint32_t Fence;
        uint32_t ReadFence;
        uint32_t Identifier;
        uint32_t BaseFlush;
        uint32_t MipFlush;
        GPUTEXTURE_FETCH_CONSTANT Format;
    };

    // The three XDK texture interfaces add no serialized fields beyond
    // D3DBaseTexture; their distinction is only their C++ API surface.
    typedef D3DBaseTexture D3DTexture;
    typedef D3DBaseTexture D3DVolumeTexture;
    typedef D3DBaseTexture D3DCubeTexture;

    union GfxTexture
    {
        // D3DBaseTexture* basemap;
        D3DTexture* map;
        D3DVolumeTexture* volmap;
        D3DCubeTexture* cubemap;
    };

    enum file_image_flags_t
    {
        IMG_FLAG_NOPICMIP = 0x1,
        IMG_FLAG_NOMIPMAPS = 0x2,
        IMG_FLAG_CUBEMAP = 0x4,
        IMG_FLAG_VOLMAP = 0x8,
        IMG_FLAG_STREAMING = 0x10,
        IMG_FLAG_LEGACY_NORMALS = 0x20,
        IMG_FLAG_CLAMP_U = 0x40,
        IMG_FLAG_CLAMP_V = 0x80,
        IMG_FLAG_DYNAMIC = 0x10000,
        IMG_FLAG_RENDER_TARGET = 0x20000,
    };

    struct GfxImageLoadDefFormat
    {
        GPUTEXTUREFORMAT DataFormat : 6; // GPUTEXTUREFORMAT
        GPUENDIAN Endian : 2;            // GPUENDIAN
        int RequestSize : 2;             // GPUREQUESTSIZE
        int Stacked : 1;                 // BOOL
        int ClampPolicy : 1;             // GPUCLAMPPOLICY
        int BaseAddress : 20;            // DWORD
    };

    struct GfxImageLoadDef
    {
        uint8_t levelCount;
        uint8_t flags;
        int16_t dimensions[3];
        int format;
        GfxTexture texture;
    };

    union GfxTextureLoad
    {
        GfxImageLoadDef* loadDef;
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
        GfxTextureLoad texture;
        uint8_t semantic;
        CardMemory cardMemory;
        uint16_t width;
        uint16_t height;
        uint16_t depth;
        uint8_t category;
        bool delayLoadPixels;
        uint8_t* pixels;
        unsigned int baseSize;
        uint16_t streamSlot;
        bool streaming;
        const char* name;
    };

    struct XAUDIOCHANNELMAPENTRY
    {
        uint8_t InputChannel;
        uint8_t OutputChannel;
        float Volume;
    };

    struct XAUDIOCHANNELMAP
    {
        uint8_t EntryCount;
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

    enum snd_alias_members_t
    {
        SA_INVALID = 0x0,
        SA_NAME = 0x1,
        SA_SEQUENCE = 0x2,
        SA_FILE = 0x3,
        SA_SUBTITLE = 0x4,
        SA_VOL_MIN = 0x5,
        SA_VOL_MAX = 0x6,
        SA_VOL_MOD = 0x7,
        SA_PITCH_MIN = 0x8,
        SA_PITCH_MAX = 0x9,
        SA_DIST_MIN = 0xA,
        SA_DIST_MAX = 0xB,
        SA_CHANNEL = 0xC,
        SA_TYPE = 0xD,
        SA_LOOP = 0xE,
        SA_PROBABILITY = 0xF,
        SA_LOADSPEC = 0x10,
        SA_MASTERSLAVE = 0x11,
        SA_SECONDARYALIASNAME = 0x12,
        SA_CHAINALIASNAME = 0x13,
        SA_VOLUMEFALLOFFCURVE = 0x14,
        SA_STARTDELAY = 0x15,
        SA_SPEAKERMAP = 0x16,
        SA_REVERB = 0x17,
        SA_LFEPERCENTAGE = 0x18,
        SA_CENTERPERCENTAGE = 0x19,
        SA_ENVELOPMIN = 0x1A,
        SA_ENVELOPMAX = 0x1B,
        SA_ENVELOPPERCENTAGE = 0x1C,

        SA_NUMFIELDS
    };

    enum SA_SPKRMAPIDENTIFIERS
    {
        SA_MONOSOURCE = 0x0,
        SA_LEFTSOURCE = 0x1,
        SA_RIGHTSOURCE = 0x2,
        SA_LEFTSPEAKER = 0x3,
        SA_RIGHTSPEAKER = 0x4,
        SA_CENTERSPEAKER = 0x5,
        SA_LFESPEAKER = 0x6,
        SA_LEFTSURROUNDSPEAKER = 0x7,
        SA_RIGHTSURROUNDSPEAKER = 0x8,

        SA_SPKRMAPIDENTIFIERCOUNT
    };

    struct SoundFile
    {
        uint8_t type;
        uint8_t exists;
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

    struct type_align32(4) XMALOOPREGION
    {
        unsigned int LoopStart;
        unsigned int LoopEnd;
        uint8_t LoopSubframeEnd;
        uint8_t LoopSubframeSkip;
    };

    struct XAUDIOPACKET_ALIGNED
    {
        char* pBuffer;
        unsigned int BufferSize;
        unsigned int LoopCount;
        XMALOOPREGION XMALoop[6];
        char* pContext;
    };

    union type_align(4) XAUDIOSOURCEFORMAT_u1
    {
        uint8_t NumStreams;
        uint8_t ChannelCount;
    };

    struct type_align32(4) XAUDIOXMAFORMAT
    {
        unsigned int SampleRate;
        uint8_t ChannelCount;
        uint8_t DecodeBufferSize;
    };

    union type_align32(4) XAUDIOSOURCEFORMAT_u2
    {
        XAUDIOXMAFORMAT Stream[6];
        unsigned int SampleRate;
    };

    struct XAUDIOSOURCEFORMAT
    {
        uint8_t SampleType;
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

    struct cStaticModelWritable
    {
        uint16_t nextModelInWorldSector;
    };

    struct cStaticModel_s
    {
        cStaticModelWritable writable;
        XModel* xmodel;
        float origin[3];
        float invScaledAxis[3][3];
        float absmin[3];
        float absmax[3];
    };

    struct dmaterial_t
    {
        char material[64];
        int surfaceFlags;
        int contentFlags;
    };

    struct cNode_t
    {
        cplane_s* plane;
        int16_t children[2];
    };

    struct type_align32(4) cLeaf_t
    {
        uint16_t firstCollAabbIndex;
        uint16_t collAabbCount;
        int brushContents;
        int terrainContents;
        float mins[3];
        float maxs[3];
        int leafBrushNode;
        int16_t cluster;
    };

    struct cLeafBrushNodeLeaf_t
    {
        uint16_t* brushes;
    };

    struct cLeafBrushNodeChildren_t
    {
        float dist;
        float range;
        uint16_t childOffset[2];
    };

    union cLeafBrushNodeData_t
    {
        cLeafBrushNodeLeaf_t leaf;
        cLeafBrushNodeChildren_t children;
    };

    struct cLeafBrushNode_s
    {
        uint8_t axis;
        int16_t leafBrushCount;
        int contents;
        cLeafBrushNodeData_t data;
    };

    struct CollisionBorder
    {
        float distEq[3];
        float zBase;
        float zSlope;
        float start;
        float length;
    };

    struct CollisionPartition
    {
        uint8_t triCount;
        uint8_t borderCount;
        int firstTri;
        CollisionBorder* borders;
    };

    union CollisionAabbTreeIndex
    {
        int firstChildIndex;
        int partitionIndex;
    };

    struct CollisionAabbTree
    {
        float origin[3];
        float halfSize[3];
        uint16_t materialIndex;
        uint16_t childCount;
        CollisionAabbTreeIndex u;
    };

    struct type_align32(16) cbrush_t
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        int16_t axialMaterialNum[2][3];
        uint8_t* baseAdjacentSide;
        int16_t firstAdjacentSideOffsets[2][3];
        uint8_t edgeCount[2][3];
    };

    enum DynEntityType
    {
        DYNENT_TYPE_INVALID = 0x0,
        DYNENT_TYPE_CLUTTER = 0x1,
        DYNENT_TYPE_DESTRUCT = 0x2,
        DYNENT_TYPE_COUNT = 0x3,
    };

    struct GfxPlacement
    {
        float quat[4];
        float origin[3];
    };

    struct DynEntityDef
    {
        DynEntityType type;
        GfxPlacement pose;
        XModel* xModel;
        uint16_t brushModel;
        uint16_t physicsBrushModel;
        FxEffectDef* destroyFx;
        XModelPieces* destroyPieces;
        PhysPreset* physPreset;
        int health;
        PhysMass mass;
        int contents;
    };

    struct DynEntityPose
    {
        GfxPlacement pose;
        float radius;
    };

    struct DynEntityClient
    {
        int physObjId;
        uint16_t flags;
        uint16_t lightingHandle;
        int health;
    };

    struct DynEntityColl
    {
        uint16_t sector;
        uint16_t nextEntInSector;
        float linkMins[2];
        float linkMaxs[2];
    };

    struct cmodel_t
    {
        float mins[3];
        float maxs[3];
        float radius;
        cLeaf_t leaf;
    };

    struct clipMap_t
    {
        const char* name;
        int isInUse;
        int planeCount;
        cplane_s* planes;
        unsigned int numStaticModels;
        cStaticModel_s* staticModelList;
        unsigned int numMaterials;
        dmaterial_t* materials;
        unsigned int numBrushSides;
        cbrushside_t* brushsides;
        unsigned int numBrushEdges;
        uint8_t* brushEdges;
        unsigned int numNodes;
        cNode_t* nodes;
        unsigned int numLeafs;
        cLeaf_t* leafs;
        unsigned int leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        unsigned int numLeafBrushes;
        uint16_t* leafbrushes;
        unsigned int numLeafSurfaces;
        unsigned int* leafsurfaces;
        unsigned int vertCount;
        float (*verts)[3];
        int triCount;
        uint16_t* triIndices;
        uint8_t* triEdgeIsWalkable;
        int borderCount;
        CollisionBorder* borders;
        int partitionCount;
        CollisionPartition* partitions;
        int aabbTreeCount;
        CollisionAabbTree* aabbTrees;
        unsigned int numSubModels;
        cmodel_t* cmodels;
        uint16_t numBrushes;
        cbrush_t* brushes;
        int numClusters;
        int clusterBytes;
        uint8_t* visibility;
        int vised;
        MapEnts* mapEnts;
        cbrush_t* box_brush;
        cmodel_t box_model;
        uint16_t dynEntCount[2];
        DynEntityDef* dynEntDefList[2];
        DynEntityPose* dynEntPoseList[2];
        DynEntityClient* dynEntClientList[2];
        DynEntityColl* dynEntCollList[2];
        unsigned int checksum;
    };

    struct ComPrimaryLight
    {
        uint8_t type;
        uint8_t canUseShadowMap;
        uint8_t exponent;
        uint8_t unused;
        float color[3];
        float dir[3];
        float origin[3];
        float radius;
        float cosHalfFovOuter;
        float cosHalfFovInner;
        float cosHalfFovExpanded;
        float rotationLimit;
        float translationLimit;
        const char* defName;
    };

    struct ComWorld
    {
        const char* name;
        int isInUse;
        unsigned int primaryLightCount;
        ComPrimaryLight* primaryLights;
    };

    struct pathlink_s
    {
        float fDist;
        uint16_t nodeNum;
        uint8_t disconnectCount;
        uint8_t negotiationLink;
        uint8_t ubBadPlaceCount[4];
    };

    enum nodeType
    {
        NODE_BADNODE = 0x0,
        NODE_PATHNODE = 0x1,
        NODE_COVER_STAND = 0x2,
        NODE_COVER_CROUCH = 0x3,
        NODE_COVER_CROUCH_WINDOW = 0x4,
        NODE_COVER_PRONE = 0x5,
        NODE_COVER_RIGHT = 0x6,
        NODE_COVER_LEFT = 0x7,
        NODE_COVER_WIDE_RIGHT = 0x8,
        NODE_COVER_WIDE_LEFT = 0x9,
        NODE_CONCEALMENT_STAND = 0xA,
        NODE_CONCEALMENT_CROUCH = 0xB,
        NODE_CONCEALMENT_PRONE = 0xC,
        NODE_REACQUIRE = 0xD,
        NODE_BALCONY = 0xE,
        NODE_SCRIPTED = 0xF,
        NODE_NEGOTIATION_BEGIN = 0x10,
        NODE_NEGOTIATION_END = 0x11,
        NODE_TURRET = 0x12,
        NODE_GUARD = 0x13,
        NODE_NUMTYPES = 0x14,
        NODE_DONTLINK = 0x14,
    };

    struct pathnode_constant_t
    {
        nodeType type;
        uint16_t spawnflags;
        uint16_t targetname;
        uint16_t script_linkName;
        uint16_t script_noteworthy;
        uint16_t target;
        uint16_t animscript;
        int animscriptfunc;
        float vOrigin[3];
        float fAngle;
        float forward[2];
        float fRadius;
        float minUseDistSq;
        int16_t wOverlapNode[2];
        int16_t wChainId;
        int16_t wChainDepth;
        int16_t wChainParent;
        uint16_t totalLinkCount;
        pathlink_s* Links;
    };

    struct pathnode_dynamic_t
    {
        void* pOwner;
        int iFreeTime;
        int iValidTime[3];
        int inPlayerLOSTime;
        int16_t wLinkCount;
        int16_t wOverlapCount;
        int16_t turretEntNumber;
        int16_t userCount;
    };

    struct pathnode_t;

    struct pathnode_transient_t
    {
        int iSearchFrame;
        pathnode_t* pNextOpen;
        pathnode_t* pPrevOpen;
        pathnode_t* pParent;
        float fCost;
        float fHeuristic;
        float costFactor;
    };

    struct pathnode_t
    {
        pathnode_constant_t constant;
        pathnode_dynamic_t dynamic;
        pathnode_transient_t transient;
    };

    struct pathbasenode_t
    {
        float vOrigin[3];
        unsigned int type;
    };

    struct pathnode_tree_nodes_t
    {
        int nodeCount;
        uint16_t* nodes;
    };

    struct pathnode_tree_t;

    union pathnode_tree_info_t
    {
        pathnode_tree_t* child[2];
        pathnode_tree_nodes_t s;
    };

    struct pathnode_tree_t
    {
        int axis;
        float dist;
        pathnode_tree_info_t u;
    };

    struct PathData
    {
        unsigned int nodeCount;
        pathnode_t* nodes;
        pathbasenode_t* basenodes;
        unsigned int chainNodeCount;
        uint16_t* chainNodeForNode;
        uint16_t* nodeForChainNode;
        int visBytes;
        uint8_t* pathVis;
        int nodeTreeCount;
        pathnode_tree_t* nodeTree;
    };

    struct GameWorldSp
    {
        const char* name;
        PathData path;
    };

    struct GameWorldMp
    {
        const char* name;
    };

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int numEntityChars;
    };

    struct GfxLight
    {
        uint8_t type;
        uint8_t canUseShadowMap;
        uint8_t unused[2];
        float color[3];
        float dir[3];
        float origin[3];
        float radius;
        float cosHalfFovOuter;
        float cosHalfFovInner;
        int exponent;
        unsigned int spotShadowIndex;
        GfxLightDef* def;
    };

    struct GfxStreamingAabbTree
    {
        uint16_t firstItem;
        uint16_t itemCount;
        uint16_t firstChild;
        uint16_t childCount;
        float mins[3];
        float maxs[3];
    };

    struct GfxWorldVertex
    {
        float xyz[3];
        float binormalSign;
        GfxColor color;
        float texCoord[2];
        float lmapCoord[2];
        PackedUnitVec normal;
        PackedUnitVec tangent;
    };

    struct GfxReflectionProbe
    {
        float origin[3];
        GfxImage* reflectionImage;
    };

    struct GfxAabbTree
    {
        float mins[3];
        float maxs[3];
        uint16_t childCount;
        uint16_t surfaceCount;
        uint16_t startSurfIndex;
        uint16_t smodelIndexCount;
        uint16_t* smodelIndexes;
        int childrenOffset;
    };

    struct GfxPortal;

    struct GfxPortalWritable
    {
        bool isQueued;
        bool isAncestor;
        uint8_t recursionDepth;
        uint8_t hullPointCount;
        float (*hullPoints)[2];
        GfxPortal* queuedParent;
    };

    struct DpvsPlane
    {
        float coeffs[4];
        uint8_t side[3];
        uint8_t pad;
    };

    struct GfxCell;

    struct GfxPortal
    {
        GfxPortalWritable writable;
        DpvsPlane plane;
        GfxCell* cell;
        float (*vertices)[3];
        uint8_t vertexCount;
        float hullAxis[2][3];
    };

    struct GfxCell
    {
        float mins[3];
        float maxs[3];
        int aabbTreeCount;
        GfxAabbTree* aabbTree;
        int portalCount;
        GfxPortal* portals;
        int cullGroupCount;
        int* cullGroups;
        uint8_t reflectionProbeCount;
        uint8_t* reflectionProbes;
    };

    struct GfxLightmapArray
    {
        GfxImage* primary;
        GfxImage* secondary;
    };

    struct GfxLightGridEntry
    {
        uint16_t colorsIndex;
        uint8_t primaryLightIndex;
        uint8_t needsTrace;
    };

    struct GfxLightGridColors
    {
        uint8_t rgb[56][3];
    };

    struct GfxBrushModelWritable
    {
        float mins[3];
        float maxs[3];
    };

    struct GfxBrushModel
    {
        GfxBrushModelWritable writable;
        float bounds[2][3];
        unsigned int surfaceCount;
        unsigned int startSurfIndex;
    };

    struct MaterialMemory
    {
        Material* material;
        int memory;
    };

    struct XModelDrawInfo
    {
        uint16_t lod;
        uint16_t surfId;
    };

    struct GfxSceneDynModel
    {
        XModelDrawInfo info;
        uint16_t dynEntId;
    };

    struct BModelDrawInfo
    {
        uint16_t surfId;
    };

    struct GfxSceneDynBrush
    {
        BModelDrawInfo info;
        uint16_t dynEntId;
    };

    struct GfxShadowGeometry
    {
        uint16_t surfaceCount;
        uint16_t smodelCount;
        uint16_t* sortedSurfIndex;
        uint16_t* smodelIndex;
    };

    struct GfxLightRegionAxis
    {
        float dir[3];
        float midPoint;
        float halfSize;
    };

    struct GfxLightRegionHull
    {
        float kdopMidPoint[9];
        float kdopHalfSize[9];
        unsigned int axisCount;
        GfxLightRegionAxis* axis;
    };

    struct GfxLightRegion
    {
        unsigned int hullCount;
        GfxLightRegionHull* hulls;
    };

    struct GfxStaticModelInst
    {
        float mins[3];
        float maxs[3];
        GfxColor groundLighting;
    };

    struct srfTriangles_t
    {
        int vertexLayerData;
        int firstVertex;
        uint16_t vertexCount;
        uint16_t triCount;
        int baseIndex;
        float topMipMins[3];
        float topMipMaxs[3];
    };

    struct GfxSurface
    {
        srfTriangles_t tris;
        Material* material;
        uint8_t lightmapIndex;
        uint8_t reflectionProbeIndex;
        uint8_t primaryLightIndex;
        bool castsSunShadow;
        float bounds[2][3];
    };

    struct GfxCullGroup
    {
        float mins[3];
        float maxs[3];
        int surfaceCount;
        int startSurfIndex;
    };

    struct GfxPackedPlacement
    {
        float origin[3];
        PackedUnitVec axis[3];
        float scale;
    };

    struct type_align32(4) GfxStaticModelDrawInst
    {
        float cullDist;
        GfxPackedPlacement placement;
        XModel* model;
        uint8_t reflectionProbeIndex;
        uint8_t primaryLightIndex;
        uint16_t lightingHandle;
        uint8_t flags;
    };

    struct GfxWorldStreamInfo
    {
        int aabbTreeCount;
        GfxStreamingAabbTree* aabbTrees;
        int leafRefCount;
        int* leafRefs;
    };

    struct GfxWorldVertexData
    {
        GfxWorldVertex* vertices;
        D3DVertexBuffer worldVb;
    };

    struct GfxWorldVertexLayerData
    {
        uint8_t* data;
        D3DVertexBuffer layerVb;
    };

    struct SunLightParseParams
    {
        char name[64];
        float ambientScale;
        float ambientColor[3];
        float diffuseFraction;
        float sunLight;
        float sunColor[3];
        float diffuseColor[3];
        bool diffuseColorHasBeenSet;
        float angles[3];
    };

    struct GfxWorldDpvsPlanes
    {
        int cellCount;
        cplane_s* planes;
        uint16_t* nodes;
        unsigned int* sceneEntCellBits;
    };

    struct GfxLightGrid
    {
        bool hasLightRegions;
        unsigned int sunPrimaryLightIndex;
        uint16_t mins[3];
        uint16_t maxs[3];
        unsigned int rowAxis;
        unsigned int colAxis;
        uint16_t* rowDataStart;
        unsigned int rawRowDataSize;
        uint8_t* rawRowData;
        unsigned int entryCount;
        GfxLightGridEntry* entries;
        unsigned int colorCount;
        GfxLightGridColors* colors;
    };

    struct sunflare_t
    {
        bool hasValidData;
        Material* spriteMaterial;
        Material* flareMaterial;
        float spriteSize;
        float flareMinSize;
        float flareMinDot;
        float flareMaxSize;
        float flareMaxDot;
        float flareMaxAlpha;
        int flareFadeInTime;
        int flareFadeOutTime;
        float blindMinDot;
        float blindMaxDot;
        float blindMaxDarken;
        int blindFadeInTime;
        int blindFadeOutTime;
        float glareMinDot;
        float glareMaxDot;
        float glareMaxLighten;
        int glareFadeInTime;
        int glareFadeOutTime;
        float sunFxPosition[3];
    };

    struct GfxWorldDpvsStatic
    {
        unsigned int smodelCount;
        unsigned int staticSurfaceCount;
        unsigned int litSurfsBegin;
        unsigned int litSurfsEnd;
        unsigned int decalSurfsBegin;
        unsigned int decalSurfsEnd;
        unsigned int emissiveSurfsBegin;
        unsigned int emissiveSurfsEnd;
        unsigned int smodelVisDataCount;
        unsigned int surfaceVisDataCount;
        uint8_t* smodelVisData[3];
        uint8_t* surfaceVisData[3];
        unsigned int* lodData;
        uint16_t* sortedSurfIndex;
        GfxStaticModelInst* smodelInsts;
        GfxSurface* surfaces;
        GfxCullGroup* cullGroups;
        GfxStaticModelDrawInst* smodelDrawInsts;
        GfxDrawSurf* surfaceMaterials;
        unsigned int* surfaceCastsSunShadow;
        int usageCount;
    };

    struct GfxWorldDpvsDynamic
    {
        unsigned int dynEntClientWordCount[2];
        unsigned int dynEntClientCount[2];
        unsigned int* dynEntCellBits[2];
        uint8_t* dynEntVisData[2][3];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        int planeCount;
        int nodeCount;
        int indexCount;
        uint16_t* indices;
        D3DIndexBuffer indexBuffer;
        int surfaceCount;
        GfxWorldStreamInfo streamInfo;
        int skySurfCount;
        int* skyStartSurfs;
        GfxImage* skyImage;
        uint8_t skySamplerState;
        unsigned int vertexCount;
        GfxWorldVertexData vd;
        unsigned int vertexLayerDataSize;
        GfxWorldVertexLayerData vld;
        SunLightParseParams sunParse;
        GfxLight* sunLight;
        float sunColorFromBsp[3];
        unsigned int sunPrimaryLightIndex;
        unsigned int primaryLightCount;
        int cullGroupCount;
        unsigned int reflectionProbeCount;
        GfxReflectionProbe* reflectionProbes;
        GfxTexture* reflectionProbeTextures;
        GfxWorldDpvsPlanes dpvsPlanes;
        int cellBitsCount;
        GfxCell* cells;
        int lightmapCount;
        GfxLightmapArray* lightmaps;
        GfxLightGrid lightGrid;
        GfxTexture* lightmapPrimaryTextures;
        GfxTexture* lightmapSecondaryTextures;
        int modelCount;
        GfxBrushModel* models;
        float mins[3];
        float maxs[3];
        unsigned int checksum;
        int materialMemoryCount;
        MaterialMemory* materialMemory;
        sunflare_t sun;
        float outdoorLookupMatrix[4][4];
        GfxImage* outdoorImage;
        unsigned int* cellCasterBits;
        GfxSceneDynModel* sceneDynModel;
        GfxSceneDynBrush* sceneDynBrush;
        unsigned int* primaryLightEntityShadowVis;
        unsigned int* primaryLightDynEntShadowVis[2];
        uint8_t* nonSunPrimaryLightForModelDynEnt;
        GfxShadowGeometry* shadowGeom;
        GfxLightRegion* lightRegion;
        GfxWorldDpvsStatic dpvs;
        GfxWorldDpvsDynamic dpvsDyn;
    };

    struct type_align32(4) GfxLightImage
    {
        GfxImage* image;
        uint8_t samplerState;
    };

    struct GfxLightDef
    {
        const char* name;
        GfxLightImage attenuation;
        int lmapLookupStart;
    };

    struct Glyph
    {
        uint16_t letter;
        char x0;
        char y0;
        uint8_t dx;
        uint8_t pixelWidth;
        uint8_t pixelHeight;
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

    enum WindowDefStaticFlag : unsigned int
    {
        WINDOW_FLAG_DECORATION = 0x100000,
        WINDOW_FLAG_HORIZONTAL_SCROLL = 0x200000,
        WINDOW_FLAG_AUTO_WRAPPED = 0x800000,
        WINDOW_FLAG_POPUP = 0x1000000,
        WINDOW_FLAG_OUT_OF_BOUNDS_CLICK = 0x2000000,
        WINDOW_FLAG_LEGACY_SPLIT_SCREEN_SCALE = 0x4000000,
        WINDOW_FLAG_HIDDEN_DURING_FLASH_BANG = 0x10000000,
        WINDOW_FLAG_HIDDEN_DURING_SCOPE = 0x20000000,
        WINDOW_FLAG_HIDDEN_DURING_UI = 0x40000000,
    };

    enum WindowDefDynamicFlag : unsigned int
    {
        WINDOW_FLAG_1 = 0x1,
        WINDOW_FLAG_FOCUSED = 0x2,
        WINDOW_FLAG_VISIBLE = 0x4,
        WINDOW_FLAG_FADING_OUT = 0x10,
        WINDOW_FLAG_FADING_IN = 0x20,
        WINDOW_FLAG_HOVERED = 0x40,
        WINDOW_FLAG_LISTBOX_HOVER_100 = 0x100,
        WINDOW_FLAG_LISTBOX_HOVER_200 = 0x200,
        WINDOW_FLAG_LISTBOX_HOVER_400 = 0x400,
        WINDOW_FLAG_LISTBOX_HOVER_800 = 0x800,
        WINDOW_FLAG_LISTBOX_HOVER_1000 = 0x1000,
        WINDOW_FLAG_4000 = 0x4000,
        WINDOW_FLAG_NON_DEFAULT_BACKCOLOR = 0x8000,
        WINDOW_FLAG_NON_DEFAULT_FORECOLOR = 0x10000,
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

    enum ItemDefDvarFlag
    {
        ITEM_DVAR_FLAG_ENABLE = 0x1,
        ITEM_DVAR_FLAG_DISABLE = 0x2,
        ITEM_DVAR_FLAG_SHOW = 0x4,
        ITEM_DVAR_FLAG_HIDE = 0x8,
        ITEM_DVAR_FLAG_FOCUS = 0x10,
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

    enum weapType_t
    {
        WEAPTYPE_BULLET = 0x0,
        WEAPTYPE_GRENADE = 0x1,
        WEAPTYPE_PROJECTILE = 0x2,
        WEAPTYPE_BINOCULARS = 0x3,
        WEAPTYPE_NUM = 0x4,
    };

    enum weapClass_t
    {
        WEAPCLASS_RIFLE = 0x0,
        WEAPCLASS_MG = 0x1,
        WEAPCLASS_SMG = 0x2,
        WEAPCLASS_SPREAD = 0x3,
        WEAPCLASS_PISTOL = 0x4,
        WEAPCLASS_GRENADE = 0x5,
        WEAPCLASS_ROCKETLAUNCHER = 0x6,
        WEAPCLASS_TURRET = 0x7,
        WEAPCLASS_NON_PLAYER = 0x8,
        WEAPCLASS_ITEM = 0x9,
        WEAPCLASS_NUM = 0xA,
    };

    enum PenetrateType
    {
        PENETRATE_TYPE_NONE = 0x0,
        PENETRATE_TYPE_SMALL = 0x1,
        PENETRATE_TYPE_MEDIUM = 0x2,
        PENETRATE_TYPE_LARGE = 0x3,
        PENETRATE_TYPE_COUNT = 0x4,
    };

    enum ImpactType
    {
        IMPACT_TYPE_NONE = 0x0,
        IMPACT_TYPE_BULLET_SMALL = 0x1,
        IMPACT_TYPE_BULLET_LARGE = 0x2,
        IMPACT_TYPE_BULLET_AP = 0x3,
        IMPACT_TYPE_SHOTGUN = 0x4,
        IMPACT_TYPE_GRENADE_BOUNCE = 0x5,
        IMPACT_TYPE_GRENADE_EXPLODE = 0x6,
        IMPACT_TYPE_ROCKET_EXPLODE = 0x7,
        IMPACT_TYPE_PROJECTILE_DUD = 0x8,
        IMPACT_TYPE_COUNT = 0x9,
    };

    enum weapInventoryType_t
    {
        WEAPINVENTORY_PRIMARY = 0x0,
        WEAPINVENTORY_OFFHAND = 0x1,
        WEAPINVENTORY_ITEM = 0x2,
        WEAPINVENTORY_ALTMODE = 0x3,
        WEAPINVENTORYCOUNT = 0x4,
    };

    enum weapFireType_t
    {
        WEAPON_FIRETYPE_FULLAUTO = 0x0,
        WEAPON_FIRETYPE_SINGLESHOT = 0x1,
        WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
        WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
        WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
        WEAPON_FIRETYPECOUNT = 0x5,
    };

    enum OffhandClass
    {
        OFFHAND_CLASS_NONE = 0x0,
        OFFHAND_CLASS_FRAG_GRENADE = 0x1,
        OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
        OFFHAND_CLASS_FLASH_GRENADE = 0x3,
        OFFHAND_CLASS_COUNT = 0x4,
    };

    enum weapStance_t
    {
        WEAPSTANCE_STAND = 0x0,
        WEAPSTANCE_DUCK = 0x1,
        WEAPSTANCE_PRONE = 0x2,
        WEAPSTANCE_NUM = 0x3,
    };

    enum activeReticleType_t
    {
        VEH_ACTIVE_RETICLE_NONE = 0x0,
        VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
        VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
        VEH_ACTIVE_RETICLE_COUNT = 0x3,
    };

    enum weaponIconRatioType_t
    {
        WEAPON_ICON_RATIO_1TO1 = 0x0,
        WEAPON_ICON_RATIO_2TO1 = 0x1,
        WEAPON_ICON_RATIO_4TO1 = 0x2,
        WEAPON_ICON_RATIO_COUNT = 0x3,
    };

    enum ammoCounterClipType_t
    {
        AMMO_COUNTER_CLIP_NONE = 0x0,
        AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
        AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
        AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
        AMMO_COUNTER_CLIP_ROCKET = 0x4,
        AMMO_COUNTER_CLIP_BELTFED = 0x5,
        AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
        AMMO_COUNTER_CLIP_COUNT = 0x7,
    };

    enum weapOverlayReticle_t
    {
        WEAPOVERLAYRETICLE_NONE = 0x0,
        WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
        WEAPOVERLAYRETICLE_NUM = 0x2,
    };

    enum WeapOverlayInteface_t
    {
        WEAPOVERLAYINTERFACE_NONE = 0x0,
        WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
        WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
        WEAPOVERLAYINTERFACECOUNT = 0x3,
    };

    enum weapProjExposion_t
    {
        WEAPPROJEXP_GRENADE = 0x0,
        WEAPPROJEXP_ROCKET = 0x1,
        WEAPPROJEXP_FLASHBANG = 0x2,
        WEAPPROJEXP_NONE = 0x3,
        WEAPPROJEXP_DUD = 0x4,
        WEAPPROJEXP_SMOKE = 0x5,
        WEAPPROJEXP_HEAVY = 0x6,
        WEAPPROJEXP_NUM = 0x7,
    };

    enum WeapStickinessType
    {
        WEAPSTICKINESS_NONE = 0x0,
        WEAPSTICKINESS_ALL = 0x1,
        WEAPSTICKINESS_GROUND = 0x2,
        WEAPSTICKINESS_GROUND_WITH_YAW = 0x3,
        WEAPSTICKINESS_COUNT = 0x4,
    };

    enum guidedMissileType_t
    {
        MISSILE_GUIDANCE_NONE = 0x0,
        MISSILE_GUIDANCE_SIDEWINDER = 0x1,
        MISSILE_GUIDANCE_HELLFIRE = 0x2,
        MISSILE_GUIDANCE_JAVELIN = 0x3,
        MISSILE_GUIDANCE_COUNT = 0x4,
    };

    struct snd_alias_list_name
    {
        const char* soundName;
    };

    union SndAliasCustom
    {
        snd_alias_list_name* name;
        snd_alias_list_t* sound;
    };

    struct WeaponDef
    {
        const char* szInternalName;
        const char* szDisplayName;
        const char* szOverlayName;
        XModel* gunXModel[16];
        XModel* handXModel;
        const char* szXAnims[33];
        const char* szModeName;
        uint16_t hideTags[8];
        uint16_t notetrackSoundMapKeys[16];
        uint16_t notetrackSoundMapValues[16];
        int playerAnimType;
        weapType_t weapType;
        weapClass_t weapClass;
        PenetrateType penetrateType;
        ImpactType impactType;
        weapInventoryType_t inventoryType;
        weapFireType_t fireType;
        OffhandClass offhandClass;
        weapStance_t stance;
        FxEffectDef* viewFlashEffect;
        FxEffectDef* worldFlashEffect;
        SndAliasCustom pickupSound;
        SndAliasCustom pickupSoundPlayer;
        SndAliasCustom ammoPickupSound;
        SndAliasCustom ammoPickupSoundPlayer;
        SndAliasCustom projectileSound;
        SndAliasCustom pullbackSound;
        SndAliasCustom pullbackSoundPlayer;
        SndAliasCustom fireSound;
        SndAliasCustom fireSoundPlayer;
        SndAliasCustom fireLoopSound;
        SndAliasCustom fireLoopSoundPlayer;
        SndAliasCustom fireStopSound;
        SndAliasCustom fireStopSoundPlayer;
        SndAliasCustom fireLastSound;
        SndAliasCustom fireLastSoundPlayer;
        SndAliasCustom emptyFireSound;
        SndAliasCustom emptyFireSoundPlayer;
        SndAliasCustom meleeSwipeSound;
        SndAliasCustom meleeSwipeSoundPlayer;
        SndAliasCustom meleeHitSound;
        SndAliasCustom meleeMissSound;
        SndAliasCustom rechamberSound;
        SndAliasCustom rechamberSoundPlayer;
        SndAliasCustom reloadSound;
        SndAliasCustom reloadSoundPlayer;
        SndAliasCustom reloadEmptySound;
        SndAliasCustom reloadEmptySoundPlayer;
        SndAliasCustom reloadStartSound;
        SndAliasCustom reloadStartSoundPlayer;
        SndAliasCustom reloadEndSound;
        SndAliasCustom reloadEndSoundPlayer;
        SndAliasCustom detonateSound;
        SndAliasCustom detonateSoundPlayer;
        SndAliasCustom nightVisionWearSound;
        SndAliasCustom nightVisionWearSoundPlayer;
        SndAliasCustom nightVisionRemoveSound;
        SndAliasCustom nightVisionRemoveSoundPlayer;
        SndAliasCustom altSwitchSound;
        SndAliasCustom altSwitchSoundPlayer;
        SndAliasCustom raiseSound;
        SndAliasCustom raiseSoundPlayer;
        SndAliasCustom firstRaiseSound;
        SndAliasCustom firstRaiseSoundPlayer;
        SndAliasCustom putawaySound;
        SndAliasCustom putawaySoundPlayer;
        SndAliasCustom* bounceSound;
        FxEffectDef* viewShellEjectEffect;
        FxEffectDef* worldShellEjectEffect;
        FxEffectDef* viewLastShotEjectEffect;
        FxEffectDef* worldLastShotEjectEffect;
        Material* reticleCenter;
        Material* reticleSide;
        int iReticleCenterSize;
        int iReticleSideSize;
        int iReticleMinOfs;
        activeReticleType_t activeReticleType;
        float vStandMove[3];
        float vStandRot[3];
        float vDuckedOfs[3];
        float vDuckedMove[3];
        float vDuckedRot[3];
        float vProneOfs[3];
        float vProneMove[3];
        float vProneRot[3];
        float fPosMoveRate;
        float fPosProneMoveRate;
        float fStandMoveMinSpeed;
        float fDuckedMoveMinSpeed;
        float fProneMoveMinSpeed;
        float fPosRotRate;
        float fPosProneRotRate;
        float fStandRotMinSpeed;
        float fDuckedRotMinSpeed;
        float fProneRotMinSpeed;
        XModel* worldModel[16];
        XModel* worldClipModel;
        XModel* rocketModel;
        XModel* knifeModel;
        XModel* worldKnifeModel;
        Material* hudIcon;
        weaponIconRatioType_t hudIconRatio;
        Material* ammoCounterIcon;
        weaponIconRatioType_t ammoCounterIconRatio;
        ammoCounterClipType_t ammoCounterClip;
        int iStartAmmo;
        const char* szAmmoName;
        int iAmmoIndex;
        const char* szClipName;
        int iClipIndex;
        int iMaxAmmo;
        int iClipSize;
        int shotCount;
        const char* szSharedAmmoCapName;
        int iSharedAmmoCapIndex;
        int iSharedAmmoCap;
        int damage;
        int playerDamage;
        int iMeleeDamage;
        int iDamageType;
        int iFireDelay;
        int iMeleeDelay;
        int meleeChargeDelay;
        int iDetonateDelay;
        int iFireTime;
        int iRechamberTime;
        int iRechamberBoltTime;
        int iHoldFireTime;
        int iDetonateTime;
        int iMeleeTime;
        int meleeChargeTime;
        int iReloadTime;
        int reloadShowRocketTime;
        int iReloadEmptyTime;
        int iReloadAddTime;
        int iReloadStartTime;
        int iReloadStartAddTime;
        int iReloadEndTime;
        int iDropTime;
        int iRaiseTime;
        int iAltDropTime;
        int iAltRaiseTime;
        int quickDropTime;
        int quickRaiseTime;
        int iFirstRaiseTime;
        int iEmptyRaiseTime;
        int iEmptyDropTime;
        int sprintInTime;
        int sprintLoopTime;
        int sprintOutTime;
        int nightVisionWearTime;
        int nightVisionWearTimeFadeOutEnd;
        int nightVisionWearTimePowerUp;
        int nightVisionRemoveTime;
        int nightVisionRemoveTimePowerDown;
        int nightVisionRemoveTimeFadeInStart;
        int fuseTime;
        int aiFuseTime;
        int requireLockonToFire;
        int noAdsWhenMagEmpty;
        int avoidDropCleanup;
        float autoAimRange;
        float aimAssistRange;
        float aimAssistRangeAds;
        float aimPadding;
        float enemyCrosshairRange;
        int crosshairColorChange;
        float moveSpeedScale;
        float adsMoveSpeedScale;
        float sprintDurationScale;
        float fAdsZoomFov;
        float fAdsZoomInFrac;
        float fAdsZoomOutFrac;
        Material* overlayMaterial;
        Material* overlayMaterialLowRes;
        weapOverlayReticle_t overlayReticle;
        WeapOverlayInteface_t overlayInterface;
        float overlayWidth;
        float overlayHeight;
        float fAdsBobFactor;
        float fAdsViewBobMult;
        float fHipSpreadStandMin;
        float fHipSpreadDuckedMin;
        float fHipSpreadProneMin;
        float hipSpreadStandMax;
        float hipSpreadDuckedMax;
        float hipSpreadProneMax;
        float fHipSpreadDecayRate;
        float fHipSpreadFireAdd;
        float fHipSpreadTurnAdd;
        float fHipSpreadMoveAdd;
        float fHipSpreadDuckedDecay;
        float fHipSpreadProneDecay;
        float fHipReticleSidePos;
        int iAdsTransInTime;
        int iAdsTransOutTime;
        float fAdsIdleAmount;
        float fHipIdleAmount;
        float adsIdleSpeed;
        float hipIdleSpeed;
        float fIdleCrouchFactor;
        float fIdleProneFactor;
        float fGunMaxPitch;
        float fGunMaxYaw;
        float swayMaxAngle;
        float swayLerpSpeed;
        float swayPitchScale;
        float swayYawScale;
        float swayHorizScale;
        float swayVertScale;
        float swayShellShockScale;
        float adsSwayMaxAngle;
        float adsSwayLerpSpeed;
        float adsSwayPitchScale;
        float adsSwayYawScale;
        float adsSwayHorizScale;
        float adsSwayVertScale;
        int bRifleBullet;
        int armorPiercing;
        int bBoltAction;
        int aimDownSight;
        int bRechamberWhileAds;
        float adsViewErrorMin;
        float adsViewErrorMax;
        int bCookOffHold;
        int bClipOnly;
        int adsFireOnly;
        int cancelAutoHolsterWhenEmpty;
        int suppressAmmoReserveDisplay;
        int enhanced;
        int laserSightDuringNightvision;
        Material* killIcon;
        weaponIconRatioType_t killIconRatio;
        int flipKillIcon;
        Material* dpadIcon;
        weaponIconRatioType_t dpadIconRatio;
        int bNoPartialReload;
        int bSegmentedReload;
        int iReloadAmmoAdd;
        int iReloadStartAdd;
        const char* szAltWeaponName;
        unsigned int altWeaponIndex;
        int iDropAmmoMin;
        int iDropAmmoMax;
        int blocksProne;
        int silenced;
        int iExplosionRadius;
        int iExplosionRadiusMin;
        int iExplosionInnerDamage;
        int iExplosionOuterDamage;
        float damageConeAngle;
        int iProjectileSpeed;
        int iProjectileSpeedUp;
        int iProjectileSpeedForward;
        int iProjectileActivateDist;
        float projLifetime;
        float timeToAccelerate;
        float projectileCurvature;
        XModel* projectileModel;
        weapProjExposion_t projExplosion;
        FxEffectDef* projExplosionEffect;
        int projExplosionEffectForceNormalUp;
        FxEffectDef* projDudEffect;
        SndAliasCustom projExplosionSound;
        SndAliasCustom projDudSound;
        int bProjImpactExplode;
        WeapStickinessType stickiness;
        int hasDetonator;
        int timedDetonation;
        int rotate;
        int holdButtonToThrow;
        int freezeMovementWhenFiring;
        float lowAmmoWarningThreshold;
        float parallelBounce[29];
        float perpendicularBounce[29];
        FxEffectDef* projTrailEffect;
        float vProjectileColor[3];
        guidedMissileType_t guidedMissileType;
        float maxSteeringAccel;
        int projIgnitionDelay;
        FxEffectDef* projIgnitionEffect;
        SndAliasCustom projIgnitionSound;
        float fAdsAimPitch;
        float fAdsCrosshairInFrac;
        float fAdsCrosshairOutFrac;
        int adsGunKickReducedKickBullets;
        float adsGunKickReducedKickPercent;
        float fAdsGunKickPitchMin;
        float fAdsGunKickPitchMax;
        float fAdsGunKickYawMin;
        float fAdsGunKickYawMax;
        float fAdsGunKickAccel;
        float fAdsGunKickSpeedMax;
        float fAdsGunKickSpeedDecay;
        float fAdsGunKickStaticDecay;
        float fAdsViewKickPitchMin;
        float fAdsViewKickPitchMax;
        float fAdsViewKickYawMin;
        float fAdsViewKickYawMax;
        float fAdsViewKickCenterSpeed;
        float fAdsViewScatterMin;
        float fAdsViewScatterMax;
        float fAdsSpread;
        int hipGunKickReducedKickBullets;
        float hipGunKickReducedKickPercent;
        float fHipGunKickPitchMin;
        float fHipGunKickPitchMax;
        float fHipGunKickYawMin;
        float fHipGunKickYawMax;
        float fHipGunKickAccel;
        float fHipGunKickSpeedMax;
        float fHipGunKickSpeedDecay;
        float fHipGunKickStaticDecay;
        float fHipViewKickPitchMin;
        float fHipViewKickPitchMax;
        float fHipViewKickYawMin;
        float fHipViewKickYawMax;
        float fHipViewKickCenterSpeed;
        float fHipViewScatterMin;
        float fHipViewScatterMax;
        float fightDist;
        float maxDist;
        const char* aiVsAiAccuracyGraphName;
        const char* aiVsPlayerAccuracyGraphName;
        float (*aiVsAiAccuracyGraphKnots)[2];
        float (*aiVsPlayerAccuracyGraphKnots)[2];
        float (*originalAiVsAiAccuracyGraphKnots)[2];
        float (*originalAiVsPlayerAccuracyGraphKnots)[2];
        int aiVsAiAccuracyGraphKnotCount;
        int aiVsPlayerAccuracyGraphKnotCount;
        int originalAiVsAiAccuracyGraphKnotCount;
        int originalAiVsPlayerAccuracyGraphKnotCount;
        int iPositionReloadTransTime;
        float leftArc;
        float rightArc;
        float topArc;
        float bottomArc;
        float accuracy;
        float aiSpread;
        float playerSpread;
        float minTurnSpeed[2];
        float maxTurnSpeed[2];
        float pitchConvergenceTime;
        float yawConvergenceTime;
        float suppressTime;
        float maxRange;
        float fAnimHorRotateInc;
        float fPlayerPositionDist;
        const char* szUseHintString;
        const char* dropHintString;
        int iUseHintStringIndex;
        int dropHintStringIndex;
        float horizViewJitter;
        float vertViewJitter;
        const char* szScript;
        float fOOPosAnimLength[2];
        int minDamage;
        int minPlayerDamage;
        float fMaxDamageRange;
        float fMinDamageRange;
        float destabilizationRateTime;
        float destabilizationCurvatureMax;
        int destabilizeDistance;
        float locationDamageMultipliers[19];
        const char* fireRumble;
        const char* meleeImpactRumble;
        float adsDofStart;
        float adsDofEnd;
    };

    struct XAUDIOREVERBSETTINGS
    {
        unsigned int ReflectionsDelay;
        uint8_t ReverbDelay;
        uint8_t RearDelay;
        uint8_t PositionLeft;
        uint8_t PositionRight;
        uint8_t PositionMatrixLeft;
        uint8_t PositionMatrixRight;
        uint8_t EarlyDiffusion;
        uint8_t LateDiffusion;
        uint8_t LowEQGain;
        uint8_t LowEQCutoff;
        uint8_t HighEQGain;
        uint8_t HighEQCutoff;
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
        int presetOverridden;
        XAUDIOREVERBSETTINGS reverbSettings;
    };

    struct SndDriverGlobals
    {
        XaReverbSettings* reverbSettings;
        const char* name;
    };

    enum FxElemType
    {
        FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
        FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
        FX_ELEM_TYPE_TAIL = 0x2,
        FX_ELEM_TYPE_TRAIL = 0x3,
        FX_ELEM_TYPE_CLOUD = 0x4,
        FX_ELEM_TYPE_MODEL = 0x5,
        FX_ELEM_TYPE_OMNI_LIGHT = 0x6,
        FX_ELEM_TYPE_SPOT_LIGHT = 0x7,
        FX_ELEM_TYPE_SOUND = 0x8,
        FX_ELEM_TYPE_DECAL = 0x9,
        FX_ELEM_TYPE_RUNNER = 0xA,
        FX_ELEM_TYPE_COUNT = 0xB,
        FX_ELEM_TYPE_LAST_SPRITE = 0x3,
        FX_ELEM_TYPE_LAST_DRAWN = 0x7,
    };

    struct FxElemVec3Range
    {
        float base[3];
        float amplitude[3];
    };

    struct FxElemVelStateInFrame
    {
        FxElemVec3Range velocity;
        FxElemVec3Range totalDelta;
    };

    struct FxElemVelStateSample
    {
        FxElemVelStateInFrame local;
        FxElemVelStateInFrame world;
    };

    struct FxElemVisualState
    {
        uint8_t color[4];
        float rotationDelta;
        float rotationTotal;
        float size[2];
        float scale;
    };

    struct FxElemVisStateSample
    {
        FxElemVisualState base;
        FxElemVisualState amplitude;
    };

    struct FxElemMarkVisuals
    {
        Material* materials[2];
    };

    union FxEffectDefRef
    {
        FxEffectDef* handle;
        const char* name;
    };

    union FxElemVisuals
    {
        const void* anonymous;
        Material* material;
        XModel* model;
        FxEffectDefRef effectDef;
        const char* soundName;
    };

    union FxElemDefVisuals
    {
        FxElemMarkVisuals* markArray;
        FxElemVisuals* array;
        FxElemVisuals instance;
    };

    struct FxTrailVertex
    {
        float pos[2];
        float normal[2];
        float texCoord;
    };

    struct FxTrailDef
    {
        int scrollTimeMsec;
        int repeatDist;
        int splitDist;
        int vertCount;
        FxTrailVertex* verts;
        int indCount;
        uint16_t* inds;
    };

    struct FxSpawnDefLooping
    {
        int intervalMsec;
        int count;
    };

    struct FxIntRange
    {
        int base;
        int amplitude;
    };

    struct FxSpawnDefOneShot
    {
        FxIntRange count;
    };

    union FxSpawnDef
    {
        FxSpawnDefLooping looping;
        FxSpawnDefOneShot oneShot;
    };

    struct FxFloatRange
    {
        float base;
        float amplitude;
    };

    struct FxElemAtlas
    {
        uint8_t behavior;
        uint8_t index;
        uint8_t fps;
        uint8_t loopCount;
        uint8_t colIndexBits;
        uint8_t rowIndexBits;
        int16_t entryCount;
    };

    struct FxElemDef
    {
        int flags;
        FxSpawnDef spawn;
        FxFloatRange spawnRange;
        FxFloatRange fadeInRange;
        FxFloatRange fadeOutRange;
        float spawnFrustumCullRadius;
        FxIntRange spawnDelayMsec;
        FxIntRange lifeSpanMsec;
        FxFloatRange spawnOrigin[3];
        FxFloatRange spawnOffsetRadius;
        FxFloatRange spawnOffsetHeight;
        FxFloatRange spawnAngles[3];
        FxFloatRange angularVelocity[3];
        FxFloatRange initialRotation;
        FxFloatRange gravity;
        FxFloatRange reflectionFactor;
        FxElemAtlas atlas;
        uint8_t elemType;
        uint8_t visualCount;
        uint8_t velIntervalCount;
        uint8_t visStateIntervalCount;
        FxElemVelStateSample* velSamples;
        FxElemVisStateSample* visSamples;
        FxElemDefVisuals visuals;
        float collMins[3];
        float collMaxs[3];
        FxEffectDefRef effectOnImpact;
        FxEffectDefRef effectOnDeath;
        FxEffectDefRef effectEmitted;
        FxFloatRange emitDist;
        FxFloatRange emitDistVariance;
        FxTrailDef* trailDef;
        uint8_t sortOrder;
        uint8_t lightingFrac;
        uint8_t useItemClip;
        uint8_t unused[1];
    };

    struct FxEffectDef
    {
        const char* name;
        int flags;
        int totalSize;
        int msecLoopingLife;
        int elemDefCountLooping;
        int elemDefCountOneShot;
        int elemDefCountEmission;
        FxElemDef* elemDefs;
    };

    struct FxImpactEntry
    {
        FxEffectDef* nonflesh[29];
        FxEffectDef* flesh[4];
    };

    struct FxImpactTable
    {
        const char* name;
        FxImpactEntry* table;
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
