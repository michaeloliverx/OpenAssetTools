#pragma once

#ifndef __T4XENON_ASSETS_H
#define __T4XENON_ASSETS_H

#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace T4Xenon
{
#endif
    enum XAssetType
    {
        ASSET_TYPE_XMODELPIECES = 0x0,
        ASSET_TYPE_PHYSPRESET = 0x1,
        ASSET_TYPE_PHYSCONSTRAINTS = 0x2,
        ASSET_TYPE_DESTRUCTIBLEDEF = 0x3,
        ASSET_TYPE_XANIMPARTS = 0x4,
        ASSET_TYPE_XMODEL = 0x5,
        ASSET_TYPE_MATERIAL = 0x6,
        ASSET_TYPE_PIXELSHADER = 0x7,
        ASSET_TYPE_TECHNIQUE_SET = 0x8,
        ASSET_TYPE_IMAGE = 0x9,
        ASSET_TYPE_SOUND = 0xA,
        ASSET_TYPE_LOADED_SOUND = 0xB,
        ASSET_TYPE_CLIPMAP = 0xC,
        ASSET_TYPE_CLIPMAP_PVS = 0xD,
        ASSET_TYPE_COMWORLD = 0xE,
        ASSET_TYPE_GAMEWORLD_SP = 0xF,
        ASSET_TYPE_GAMEWORLD_MP = 0x10,
        ASSET_TYPE_MAP_ENTS = 0x11,
        ASSET_TYPE_GFXWORLD = 0x12,
        ASSET_TYPE_LIGHT_DEF = 0x13,
        ASSET_TYPE_UI_MAP = 0x14,
        ASSET_TYPE_FONT = 0x15,
        ASSET_TYPE_MENULIST = 0x16,
        ASSET_TYPE_MENU = 0x17,
        ASSET_TYPE_LOCALIZE_ENTRY = 0x18,
        ASSET_TYPE_WEAPON = 0x19,
        ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1A,
        ASSET_TYPE_FX = 0x1B,
        ASSET_TYPE_IMPACT_FX = 0x1C,
        ASSET_TYPE_AITYPE = 0x1D,
        ASSET_TYPE_MPTYPE = 0x1E,
        ASSET_TYPE_CHARACTER = 0x1F,
        ASSET_TYPE_XMODELALIAS = 0x20,
        ASSET_TYPE_RAWFILE = 0x21,
        ASSET_TYPE_STRINGTABLE = 0x22,
        ASSET_TYPE_PACKINDEX = 0x23,
        ASSET_TYPE_COUNT = 0x24,

        ASSET_TYPE_STRING = 0x24,
        ASSET_TYPE_ASSETLIST = 0x25,

        ASSET_TYPE_FULLCOUNT = 0x26
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

    struct XModelPieces;
    struct PhysPreset;
    struct PhysConstraints;
    struct DestructibleDef;
    struct XAnimParts;
    struct XModel;
    struct Material;
    struct MaterialPixelShader;
    struct MaterialVertexShader;
    struct MaterialTechniqueSet;
    struct GfxImage;
    struct snd_alias_list_t;
    struct snd_alias_list_name;
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
    struct PackIndex;

    typedef unsigned short ScriptString;

    union XAssetHeader
    {
        XModelPieces* xmodelPieces;
        PhysPreset* physPreset;
        PhysConstraints* physConstraints;
        DestructibleDef* destructibleDef;
        XAnimParts* parts;
        XModel* model;
        Material* material;
        MaterialPixelShader* pixelShader;
        MaterialVertexShader* vertexShader;
        MaterialTechniqueSet* techniqueSet;
        GfxImage* image;
        snd_alias_list_t* sound;
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
        PackIndex* packIndex;
        void* data;
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
        int canFloat;
    };

    struct XAnimNotifyInfo
    {
        ScriptString name;
        float time;
    };

    union XAnimDynamicFrames
    {
        unsigned __int8 (*_1)[3];
        unsigned __int16 (*_2)[3];
    };

    union XAnimDynamicIndicesTrans
    {
        unsigned __int8 _1[1];
        unsigned __int16 _2[1];
    };

    union XAnimDynamicIndicesQuat
    {
        unsigned __int8 _1[1];
        unsigned __int16 _2[1];
    };

    struct __declspec(align(4)) XAnimPartTransFrames
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
        unsigned __int16 size;
        unsigned __int8 smallTrans;
        XAnimPartTransData u;
    };

    struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames
    {
        __int16 (*frames)[2];
        XAnimDynamicIndicesQuat indices;
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
        unsigned __int16 dataByteCount;
        unsigned __int16 dataShortCount;
        unsigned __int16 dataIntCount;
        unsigned __int16 randomDataByteCount;
        unsigned __int16 randomDataIntCount;
        unsigned __int16 numframes;
        bool bLoop;
        bool bDelta;
        unsigned __int8 boneCount[PART_TYPE_COUNT];
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

    struct DObjAnimMat
    {
        float quat[4];
        float trans[3];
        float transWeight;
    };

    union GfxColor
    {
        unsigned int packed;
        unsigned __int8 array[4];
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
        float xyz[3];
        float binormalSign;
        GfxColor color;
        PackedTexCoords texCoord;
        PackedUnitVec normal;
        PackedUnitVec tangent;
    };

    struct XSurfaceCollisionAabb
    {
        unsigned __int16 mins[3];
        unsigned __int16 maxs[3];
    };

    struct XSurfaceCollisionNode
    {
        XSurfaceCollisionAabb aabb;
        unsigned __int16 childBeginIndex;
        unsigned __int16 childCount;
    };

    struct XSurfaceCollisionLeaf
    {
        unsigned __int16 triangleBeginIndex;
    };

    struct XSurfaceCollisionTree
    {
        float trans[3];
        float scale[3];
        unsigned int nodeCount;
        XSurfaceCollisionNode* nodes;
        unsigned int leafCount;
        XSurfaceCollisionLeaf* leafs;
    };

    struct XRigidVertList
    {
        unsigned __int16 boneOffset;
        unsigned __int16 vertCount;
        unsigned __int16 triOffset;
        unsigned __int16 triCount;
        XSurfaceCollisionTree* collisionTree;
    };

    struct XSurfaceVertexInfo
    {
        __int16 vertCount[4];
        unsigned __int16* vertsBlend;
    };

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
        unsigned __int8 tileMode;
        bool deformed;
        unsigned __int16 vertCount;
        unsigned __int16 triCount;
        unsigned __int16* triIndices;
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
        float bounds[2][3];
        float offset[3];
        float radiusSquared;
    };

    struct XModelHighMipBounds
    {
        float mins[3];
        float maxs[3];
    };

    struct cplane_s
    {
        float normal[3];
        float dist;
        unsigned __int8 type;
        unsigned __int8 signbits;
        unsigned __int8 pad[2];
    };

    struct cbrushside_t
    {
        cplane_s* plane;
        unsigned int materialNum;
        __int16 firstAdjacentSideOffset;
        unsigned __int8 edgeCount;
    };

    struct BrushWrapper
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        __int16 axialMaterialNum[2][3];
        char* baseAdjacentSide;
        __int16 firstAdjacentSideOffsets[2][3];
        char edgeCount[2][3];
        unsigned int numverts;
        float (*verts)[3];
        int totalEdgeCount;
        cplane_s* planes;
        char pad[8];
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
    };

    struct PhysGeomList
    {
        unsigned int count;
        PhysGeomInfo* geoms;
        PhysMass mass;
    };

    enum ConstraintType
    {
        CONSTRAINT_NONE = 0x0,
        CONSTRAINT_POINT = 0x1,
        CONSTRAINT_DISTANCE = 0x2,
        CONSTRAINT_HINGE = 0x3,
        CONSTRAINT_JOINT = 0x4,
        CONSTRAINT_ACTUATOR = 0x5,
        CONSTRAINT_FAKE_SHAKE = 0x6,
        CONSTRAINT_LAUNCH = 0x7,
        CONSTRAINT_ROPE = 0x8,
        CONSTRAINT_LIGHT = 0x9,

        NUM_CONSTRAINT_TYPES
    };

    enum AttachPointType
    {
        ATTACH_POINT_WORLD = 0x0,
        ATTACH_POINT_DYNENT = 0x1,
        ATTACH_POINT_ENT = 0x2,
        ATTACH_POINT_BONE = 0x3,
    };

    struct PhysConstraint
    {
        ScriptString targetname;
        ConstraintType type;
        AttachPointType attach_point_type1;
        int target_index1;
        ScriptString target_ent1;
        const char* target_bone1;
        AttachPointType attach_point_type2;
        int target_index2;
        ScriptString target_ent2;
        const char* target_bone2;
        float offset[3];
        float pos[3];
        float pos2[3];
        float dir[3];
        int flags;
        int timeout;
        int min_health;
        int max_health;
        float distance;
        float damp;
        float power;
        float scale[3];
        float spin_scale;
        float minAngle;
        float maxAngle;
        Material* material;
        int constraintHandle;
        int rope_index;
    };

    struct PhysConstraints
    {
        const char* name;
        unsigned int count;
        PhysConstraint data[16];
    };

    struct DestructibleModel
    {
        XModel* model;
        float breakHealth;
        unsigned int flags;
        FxEffectDef* breakEffect;
        snd_alias_list_name* breakSound;
        const char* breakNotify;
        const char* loopSound;
        XModel* spawnModel[3];
    };

    struct DestructiblePiece
    {
        DestructibleModel dmodels[5];
        const char* attachBone;
        ScriptString attachBoneHash;
        unsigned char parentPiece;
        float parentDamagePercent;
        float bulletDamageScale;
        float explosiveDamageScale;
        float entityDamageTransfer;
        PhysConstraints* physConstraints;
        int health;
        snd_alias_list_name* damageSound;
        FxEffectDef* burnEffect;
        snd_alias_list_name* burnSound;
        ScriptString enableLabel;
    };

    struct DestructibleDef
    {
        const char* name;
        XModel* baseModel;
        int numPieces;
        DestructiblePiece* pieces;
        int clientOnly;
    };

    struct XModelLodInfo
    {
        float dist;
        unsigned __int16 numsurfs;
        unsigned __int16 surfIndex;
        int partBits[4];
    };

    struct XModelStreamInfo
    {
        XModelHighMipBounds* highMipBounds;
    };

    struct XModel
    {
        const char* name;
        unsigned __int8 numBones;
        unsigned __int8 numRootBones;
        unsigned __int8 numsurfs;
        unsigned __int8 lodRampType;
        ScriptString* boneNames;
        unsigned __int8* parentList;
        __int16* quats;
        float* trans;
        unsigned __int8* partClassification;
        DObjAnimMat* baseMat;
        XSurface* surfs;
        Material** materialHandles;
        XModelLodInfo lodInfo[4];
        XModelCollSurf_s* collSurfs;
        int numCollSurfs;
        int contents;
        XBoneInfo* boneInfo;
        float radius;
        float mins[3];
        float maxs[3];
        __int16 numLods;
        __int16 collLod;
        XModelStreamInfo streamInfo;
        int memUsage;
        unsigned __int8 flags;
        PhysPreset* physPreset;
        PhysGeomList* physGeoms;
        PhysGeomList* collmap;
        PhysConstraints* physConstraints;
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
        char isMatureContent;
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

#pragma pack(push, 4)

    struct type_align32(4) GfxDrawSurfFields
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

    union type_align32(4) GfxDrawSurf
    {
        GfxDrawSurfFields fields;
        unsigned __int64 packed;
    };

    struct MaterialInfo
    {
        const char* name;
        unsigned __int8 gameFlags;
        unsigned __int8 sortKey;
        unsigned __int8 textureAtlasRowCount;
        unsigned __int8 textureAtlasColumnCount;
        GfxDrawSurf drawSurf;
        unsigned int surfaceTypeBits;
        unsigned int layeredSurfaceTypes;
    };

    struct Material
    {
        MaterialInfo info;
        unsigned __int8 stateBitsEntry[51];
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

#pragma pack(pop)

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
        VERTDECL_WATER = 0xF,

        VERTDECL_COUNT = 0x10,
    };

    union MaterialVertexStreamRouting
    {
        MaterialStreamRouting data[16];
        void /*D3DVertexDeclaration*/* decl[VERTDECL_COUNT];
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
        CONST_SRC_CODE_ALPHA_FADE = 0x13,
        CONST_SRC_CODE_MAYBE_DIRTY_PS_END = 0x14,
        CONST_SRC_CODE_ALWAYS_DIRTY_PS_BEGIN = 0x14,
        CONST_SRC_CODE_PIXEL_COST_FRACS = 0x14,
        CONST_SRC_CODE_PIXEL_COST_DECODE = 0x15,
        CONST_SRC_CODE_FILTER_TAP_0 = 0x16,
        CONST_SRC_CODE_FILTER_TAP_1 = 0x17,
        CONST_SRC_CODE_FILTER_TAP_2 = 0x18,
        CONST_SRC_CODE_FILTER_TAP_3 = 0x19,
        CONST_SRC_CODE_FILTER_TAP_4 = 0x1A,
        CONST_SRC_CODE_FILTER_TAP_5 = 0x1B,
        CONST_SRC_CODE_FILTER_TAP_6 = 0x1C,
        CONST_SRC_CODE_FILTER_TAP_7 = 0x1D,
        CONST_SRC_CODE_COLOR_MATRIX_R = 0x1E,
        CONST_SRC_CODE_COLOR_MATRIX_G = 0x1F,
        CONST_SRC_CODE_COLOR_MATRIX_B = 0x20,
        CONST_SRC_CODE_ALWAYS_DIRTY_PS_END = 0x21,
        CONST_SRC_CODE_NEVER_DIRTY_PS_BEGIN = 0x21,
        CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x21,
        CONST_SRC_CODE_SHADOWMAP_SCALE = 0x22,
        CONST_SRC_CODE_ZNEAR = 0x23,
        CONST_SRC_CODE_SUN_POSITION = 0x24,
        CONST_SRC_CODE_SUN_DIFFUSE = 0x25,
        CONST_SRC_CODE_SUN_SPECULAR = 0x26,
        CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x27,
        CONST_SRC_CODE_DEBUG_BUMPMAP = 0x28,
        CONST_SRC_CODE_MATERIAL_COLOR = 0x29,
        CONST_SRC_CODE_FOG = 0x2A,
        CONST_SRC_CODE_FOG_COLOR = 0x2B,
        CONST_SRC_CODE_GLOW_SETUP = 0x2C,
        CONST_SRC_CODE_GLOW_APPLY = 0x2D,
        CONST_SRC_CODE_COLOR_BIAS = 0x2E,
        CONST_SRC_CODE_COLOR_TINT_BASE = 0x2F,
        CONST_SRC_CODE_COLOR_TINT_DELTA = 0x30,
        CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x31,
        CONST_SRC_CODE_SKY_TRANSITION = 0x32,
        CONST_SRC_CODE_ENVMAP_PARMS = 0x33,
        CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x34,
        CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x35,
        CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x36,
        CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX = 0x37,
        CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x38,
        CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x39,
        CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x3A,
        CONST_SRC_CODE_CODE_MESH_ARG_LAST = 0x3A,
        CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x3B,
        CONST_SRC_CODE_WIND_DIRECTION = 0x3C,
        CONST_SRC_CODE_WATER_PARMS = 0x3D,
        CONST_SRC_CODE_GRASS_PARMS = 0x3E,
        CONST_SRC_CODE_GRASS_FORCE0 = 0x3F,
        CONST_SRC_CODE_GRASS_FORCE1 = 0x40,
        CONST_SRC_CODE_GRASS_FORCE0_EX = 0x41,
        CONST_SRC_CODE_GRASS_FORCE1_EX = 0x42,
        CONST_SRC_CODE_GRASS_WIND_FORCE0 = 0x43,
        CONST_SRC_CODE_MOTIONBLUR_DIRECTION_AND_MAGNITUDE = 0x44,
        CONST_SRC_CODE_COMPOSITE_FX_DISTORTION = 0x45,
        CONST_SRC_CODE_GLOW_BLOOM_SCALE = 0x46,
        CONST_SRC_CODE_COMPOSITE_FX_OVERLAY_TEXCOORD = 0x47,
        CONST_SRC_CODE_COLOR_BIAS1 = 0x48,
        CONST_SRC_CODE_COLOR_TINT_BASE1 = 0x49,
        CONST_SRC_CODE_COLOR_TINT_DELTA1 = 0x4A,
        CONST_SRC_CODE_POSTFX_FADE_EFFECT = 0x4B,
        CONST_SRC_CODE_VIEWPORT_DIMENSIONS = 0x4C,
        CONST_SRC_CODE_FRAMEBUFFER_READ = 0x4D,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_0 = 0x4E,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_1 = 0x4F,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_2 = 0x50,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_3 = 0x51,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_4 = 0x52,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_5 = 0x53,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_6 = 0x54,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_7 = 0x55,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_8 = 0x56,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_9 = 0x57,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_10 = 0x58,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_11 = 0x59,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_12 = 0x5A,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_13 = 0x5B,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_14 = 0x5C,
        CONST_SRC_CODE_VARIANT_WIND_SPRING_15 = 0x5D,
        CONST_SRC_CODE_DESTRUCTIBLE_PARMS = 0x5E,
        CONST_SRC_CODE_CLOUD_WORLD_AREA = 0x5F,
        CONST_SRC_CODE_WATER_SCROLL = 0x60,
        CONST_SRC_CODE_CROSSFADE_ALPHA = 0x61,
        CONST_SRC_CODE_CHARACTER_CHARRED_AMOUNT = 0x62,
        CONST_SRC_CODE_TREECANOPY_PARMS = 0x63,
        CONST_SRC_CODE_MARKS_HIT_NORMAL = 0x64,
        CONST_SRC_CODE_POSTFX_CONTROL0 = 0x65,
        CONST_SRC_CODE_POSTFX_CONTROL1 = 0x66,
        CONST_SRC_CODE_CINEMATIC_BLUR_BOX = 0x67,
        CONST_SRC_CODE_CINEMATIC_BLUR_BOX2 = 0x68,
        CONST_SRC_CODE_NEVER_DIRTY_PS_END = 0x69,
        CONST_SRC_CODE_COUNT_FLOAT4 = 0x69,
        CONST_SRC_FIRST_CODE_MATRIX = 0x69,
        CONST_SRC_CODE_WORLD_MATRIX = 0x69,
        CONST_SRC_CODE_INVERSE_WORLD_MATRIX = 0x6A,
        CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX = 0x6B,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX = 0x6C,
        CONST_SRC_CODE_VIEW_MATRIX = 0x6D,
        CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x6E,
        CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x6F,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x70,
        CONST_SRC_CODE_PROJECTION_MATRIX = 0x71,
        CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x72,
        CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x73,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x74,
        CONST_SRC_CODE_WORLD_VIEW_MATRIX = 0x75,
        CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX = 0x76,
        CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x77,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x78,
        CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x79,
        CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x7A,
        CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x7B,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x7C,
        CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX = 0x7D,
        CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX = 0x7E,
        CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x7F,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x80,
        CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x81,
        CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x82,
        CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x83,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x84,
        CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x85,
        CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x86,
        CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x87,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x88,
        CONST_SRC_CODE_MODEL_WORLD_TO_OBJECT_MATRIX = 0x89,
        CONST_SRC_CODE_INVERSE_MODEL_WORLD_TO_OBJECT_MATRIX = 0x8A,
        CONST_SRC_CODE_TRANSPOSE_MODEL_WORLD_TO_OBJECT_MATRIX = 0x8B,
        CONST_SRC_CODE_INVERSE_TRANSPOSE_MODEL_WORLD_TO_OBJECT_MATRIX = 0x8C,
        CONST_SRC_TOTAL_COUNT = 0x8D,
        CONST_SRC_NONE = 0x8E,
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
        TEXTURE_SRC_CODE_POST_EFFECT_SRC = 0xC,
        TEXTURE_SRC_CODE_POST_EFFECT_GODRAYS = 0xD,
        TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xE,
        TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xF,
        TEXTURE_SRC_CODE_SKY = 0x10,
        TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0x11,
        TEXTURE_SRC_CODE_DYNAMIC_SHADOWS = 0x12,
        TEXTURE_SRC_CODE_OUTDOOR = 0x13,
        TEXTURE_SRC_CODE_FLOATZ = 0x14,
        TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x15,
        TEXTURE_SRC_CODE_RAW_FLOATZ = 0x16,
        TEXTURE_SRC_CODE_HALF_PARTICLE_COLOR = 0x17,
        TEXTURE_SRC_CODE_HALF_PARTICLE_DEPTH = 0x18,
        TEXTURE_SRC_CODE_CASE_TEXTURE = 0x19,
        TEXTURE_SRC_CODE_CINEMATIC_Y = 0x1A,
        TEXTURE_SRC_CODE_CINEMATIC_CR = 0x1B,
        TEXTURE_SRC_CODE_CINEMATIC_CB = 0x1C,
        TEXTURE_SRC_CODE_CINEMATIC_A = 0x1D,
        TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x1E,
        TEXTURE_SRC_CODE_TERRAIN_SCORCH_TEXTURE_0 = 0x1F,
        TEXTURE_SRC_CODE_TERRAIN_SCORCH_TEXTURE_1 = 0x20,
        TEXTURE_SRC_CODE_TERRAIN_SCORCH_TEXTURE_2 = 0x21,
        TEXTURE_SRC_CODE_TERRAIN_SCORCH_TEXTURE_3 = 0x22,
        TEXTURE_SRC_CODE_TERRAIN_SCORCH_TEXTURE_4 = 0x23,

        TEXTURE_SRC_CODE_COUNT = 0x24,
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
        unsigned __int16 type;
        unsigned __int16 dest;
        MaterialArgumentDef u;
    };

    enum VertexShaderPrecompiledIndex : unsigned __int8
    {
        VERTEX_SHADER_NONE = 0x0,
        VERTEX_SHADER_MODEL_LIT = 0x1,
        VERTEX_SHADER_MODEL_UNLIT = 0x2,
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShaderArray[16];
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        unsigned __int8 perPrimArgCount;
        unsigned __int8 perObjArgCount;
        unsigned __int8 stableArgCount;
        unsigned __int8 customSamplerFlags;
        VertexShaderPrecompiledIndex precompiledIndex;
        MaterialShaderArgument* args;
    };

    enum TechniqueFlags
    {
        MTL_TECHFLAG_NEEDS_RESOLVED_POST_SUN = 0x1,
        MTL_TECHFLAG_NEEDS_RESOLVED_SCENE = 0x2,
        MTL_TECHFLAG_ZPREPASS = 0x4,
        MTL_TECHFLAG_DECL_HAS_OPTIONAL_SOURCE = 0x8,
        MTL_TECHFLAG_USES_LIGHT_SPOT_FACTORS = 0x10,
        MTL_TECHFLAG_USES_GRASS = 0x20,
        MTL_TECHFLAG_USES_FLOATZ = 0x40,
        TECHNIQUE_FLAG_80 = 0x80,
        MTL_TECHFLAG_USES_MARKS_HIT_NORMAL = 0x100,
        MTL_TECHFLAG_CHARACTER_OR_DESTRUCTIBLE = 0x200,
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
        TECHNIQUE_EMISSIVE_REFLECTED = 0x7,

        TECHNIQUE_LIT_BEGIN = 0x8,

        TECHNIQUE_LIT = 0x8,
        TECHNIQUE_LIT_FADE = 0x9,
        TECHNIQUE_LIT_SUN = 0xA,
        TECHNIQUE_LIT_SUN_FADE = 0xB,
        TECHNIQUE_LIT_SUN_SHADOW = 0xC,
        TECHNIQUE_LIT_SUN_SHADOW_FADE = 0xD,
        TECHNIQUE_LIT_SPOT = 0xE,
        TECHNIQUE_LIT_SPOT_FADE = 0xF,
        TECHNIQUE_LIT_SPOT_SHADOW = 0x10,
        TECHNIQUE_LIT_SPOT_SHADOW_FADE = 0x11,
        TECHNIQUE_LIT_OMNI = 0x12,
        TECHNIQUE_LIT_OMNI_FADE = 0x13,
        TECHNIQUE_LIT_OMNI_SHADOW = 0x14,
        TECHNIQUE_LIT_OMNI_SHADOW_FADE = 0x15,
        TECHNIQUE_LIT_CHARRED = 0x16,
        TECHNIQUE_LIT_FADE_CHARRED = 0x17,
        TECHNIQUE_LIT_SUN_CHARRED = 0x18,
        TECHNIQUE_LIT_SUN_FADE_CHARRED = 0x19,
        TECHNIQUE_LIT_SUN_SHADOW_CHARRED = 0x1A,
        TECHNIQUE_LIT_SUN_SHADOW_FADE_CHARRED = 0x1B,
        TECHNIQUE_LIT_SPOT_CHARRED = 0x1C,
        TECHNIQUE_LIT_SPOT_FADE_CHARRED = 0x1D,
        TECHNIQUE_LIT_SPOT_SHADOW_CHARRED = 0x1E,
        TECHNIQUE_LIT_SPOT_SHADOW_FADE_CHARRED = 0x1F,
        TECHNIQUE_LIT_OMNI_CHARRED = 0x20,
        TECHNIQUE_LIT_OMNI_FADE_CHARRED = 0x21,
        TECHNIQUE_LIT_OMNI_SHADOW_CHARRED = 0x22,
        TECHNIQUE_LIT_OMNI_SHADOW_FADE_CHARRED = 0x23,

        TECHNIQUE_LIT_END = 0x24,

        TECHNIQUE_LIGHT_SPOT = 0x24,
        TECHNIQUE_LIGHT_OMNI = 0x25,
        TECHNIQUE_LIGHT_SPOT_SHADOW = 0x26,
        TECHNIQUE_LIGHT_SPOT_CHARRED = 0x27,
        TECHNIQUE_LIGHT_OMNI_CHARRED = 0x28,
        TECHNIQUE_LIGHT_SPOT_SHADOW_CHARRED = 0x29,
        TECHNIQUE_FAKELIGHT_NORMAL = 0x2A,
        TECHNIQUE_FAKELIGHT_VIEW = 0x2B,
        TECHNIQUE_SUNLIGHT_PREVIEW = 0x2C,
        TECHNIQUE_CASE_TEXTURE = 0x2D,
        TECHNIQUE_WIREFRAME_SOLID = 0x2E,
        TECHNIQUE_WIREFRAME_SHADED = 0x2F,
        TECHNIQUE_SHADOWCOOKIE_CASTER = 0x30,
        TECHNIQUE_SHADOWCOOKIE_RECEIVER = 0x31,
        TECHNIQUE_DEBUG_BUMPMAP = 0x32,
        TECHNIQUE_COUNT = 0x33,
        TECHNIQUE_TOTAL_COUNT = 0x34,
        TECHNIQUE_NONE = 0x35,
    };

    enum MaterialWorldVertexFormat : unsigned __int8
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
        MaterialWorldVertexFormat worldVertFormat;
        unsigned __int8 hasBeenUploaded;
        unsigned __int8 unused[2];
        MaterialTechniqueSet* remappedTechniqueSet;
        MaterialTechnique* techniques[51];
    };

    enum GPUTEXTUREFORMAT
    {
        GPUTEXTUREFORMAT_1_REVERSE = 0,
        GPUTEXTUREFORMAT_1 = 1,
        GPUTEXTUREFORMAT_8 = 2,
        GPUTEXTUREFORMAT_1_5_5_5 = 3,
        GPUTEXTUREFORMAT_5_6_5 = 4,
        GPUTEXTUREFORMAT_6_5_5 = 5,
        GPUTEXTUREFORMAT_8_8_8_8 = 6,
        GPUTEXTUREFORMAT_2_10_10_10 = 7,
        GPUTEXTUREFORMAT_8_A = 8,
        GPUTEXTUREFORMAT_8_B = 9,
        GPUTEXTUREFORMAT_8_8 = 10,
        GPUTEXTUREFORMAT_Cr_Y1_Cb_Y0_REP = 11,
        GPUTEXTUREFORMAT_Y1_Cr_Y0_Cb_REP = 12,
        GPUTEXTUREFORMAT_16_16_EDRAM = 13, // EDRAM render target only
        GPUTEXTUREFORMAT_8_8_8_8_A = 14,
        GPUTEXTUREFORMAT_4_4_4_4 = 15,
        GPUTEXTUREFORMAT_10_11_11 = 16,
        GPUTEXTUREFORMAT_11_11_10 = 17,
        GPUTEXTUREFORMAT_DXT1 = 18,
        GPUTEXTUREFORMAT_DXT2_3 = 19,
        GPUTEXTUREFORMAT_DXT4_5 = 20,
        GPUTEXTUREFORMAT_16_16_16_16_EDRAM = 21, // EDRAM render target only
        GPUTEXTUREFORMAT_24_8 = 22,
        GPUTEXTUREFORMAT_24_8_FLOAT = 23,
        GPUTEXTUREFORMAT_16 = 24,
        GPUTEXTUREFORMAT_16_16 = 25,
        GPUTEXTUREFORMAT_16_16_16_16 = 26,
        GPUTEXTUREFORMAT_16_EXPAND = 27,
        GPUTEXTUREFORMAT_16_16_EXPAND = 28,
        GPUTEXTUREFORMAT_16_16_16_16_EXPAND = 29,
        GPUTEXTUREFORMAT_16_FLOAT = 30,
        GPUTEXTUREFORMAT_16_16_FLOAT = 31,
        GPUTEXTUREFORMAT_16_16_16_16_FLOAT = 32,
        GPUTEXTUREFORMAT_32 = 33,
        GPUTEXTUREFORMAT_32_32 = 34,
        GPUTEXTUREFORMAT_32_32_32_32 = 35,
        GPUTEXTUREFORMAT_32_FLOAT = 36,
        GPUTEXTUREFORMAT_32_32_FLOAT = 37,
        GPUTEXTUREFORMAT_32_32_32_32_FLOAT = 38,
        GPUTEXTUREFORMAT_32_AS_8 = 39,
        GPUTEXTUREFORMAT_32_AS_8_8 = 40,
        GPUTEXTUREFORMAT_16_MPEG = 41,
        GPUTEXTUREFORMAT_16_16_MPEG = 42,
        GPUTEXTUREFORMAT_8_INTERLACED = 43,
        GPUTEXTUREFORMAT_32_AS_8_INTERLACED = 44,
        GPUTEXTUREFORMAT_32_AS_8_8_INTERLACED = 45,
        GPUTEXTUREFORMAT_16_INTERLACED = 46,
        GPUTEXTUREFORMAT_16_MPEG_INTERLACED = 47,
        GPUTEXTUREFORMAT_16_16_MPEG_INTERLACED = 48,
        GPUTEXTUREFORMAT_DXN = 49,
        GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16 = 50,
        GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16 = 51,
        GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16 = 52,
        GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16 = 53,
        GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16 = 54,
        GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16 = 55,
        GPUTEXTUREFORMAT_11_11_10_AS_16_16_16_16 = 56,
        GPUTEXTUREFORMAT_32_32_32_FLOAT = 57,
        GPUTEXTUREFORMAT_DXT3A = 58,
        GPUTEXTUREFORMAT_DXT5A = 59,
        GPUTEXTUREFORMAT_CTX1 = 60,
        GPUTEXTUREFORMAT_DXT3A_AS_1_1_1_1 = 61,
        GPUTEXTUREFORMAT_8_8_8_8_GAMMA_EDRAM = 62,    // EDRAM render target only
        GPUTEXTUREFORMAT_2_10_10_10_FLOAT_EDRAM = 63, // EDRAM render target only
    };

    enum GPUENDIAN
    {
        GPUENDIAN_NONE = 0,
        GPUENDIAN_8IN16 = 1,
        GPUENDIAN_8IN32 = 2,
        GPUENDIAN_16IN32 = 3,
    };

    struct type_align(4) DummyD3DTexture
    {
        char pad[52];
    };

    struct type_align(4) DummyD3DVolumeTexture
    {
        char pad[52];
    };

    struct type_align(4) DummyD3DCubeTexture
    {
        char pad[52];
    };

    union GfxTexture
    {
        // DummyD3DBaseTexture* basemap;
        DummyD3DTexture* map;
        DummyD3DVolumeTexture* volmap;
        DummyD3DCubeTexture* cubemap;
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
        unsigned __int8 levelCount;
        unsigned __int8 flags;
        __int16 dimensions[3];
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

    struct StreamFileName
    {
        unsigned int hash;
        const char* dir;
        const char* name;
    };

    struct PrimedSound
    {
        const char* name;
        unsigned __int8* buffer;
        unsigned int size;
    };

    struct StreamedSound
    {
        StreamFileName filename;
        PrimedSound* primeSnd;
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
        int aliasNameHash;
        const char* subtitle;
        const char* secondaryAliasName;
        const char* chainAliasName;
        SoundFile* soundFile;
        unsigned int sequence;
        float volMin;
        float volMax;
        float pitchMin;
        float pitchMax;
        float distMin;
        float distMax;
        float distReverbMax;
        float slavePercentage;
        float probability;
        float lfePercentage;
        float centerPercentage;
        float envelopMin;
        float envelopMax;
        float envelopPercentage;
        float minPriority;
        float maxPriority;
        float minPriorityThreshold;
        float maxPriorityThreshold;
        float reverbSend;
        int gap2;
        int gap3;
        float occlusionLevel;
        float occlusionWetDry;
        unsigned int moveTime;
        unsigned int startDelay;
        unsigned int speakerMap;
        int flags;
        unsigned int volumeFalloffCurve;
        unsigned int reverbFalloffCurve;
        unsigned int volumeMinFalloffCurve;
        unsigned int reverbMinFalloffCurve;
        int limitType;
        int gap5;
        unsigned int entityLimitCount;
        int gap6;
        int randomizeType;
        int gap7;
        int gap8;
        int gap9;
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

    union __declspec(align(4)) XAUDIOSOURCEFORMAT_u1
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

    struct PackIndexEntry
    {
        unsigned int hash;
        unsigned int offset;
        unsigned int size;
    };

    struct PackIndexHeader
    {
        unsigned int magic;
        unsigned int timestamp;
        unsigned int count;
        unsigned int alignment;
        unsigned int data_start;
    };

    struct PackIndex
    {
        const char* name;
        PackIndexHeader header;
        PackIndexEntry* entries;
    };

    struct cStaticModelWritable
    {
        unsigned __int16 nextModelInWorldSector;
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
        __int16 children[2];
    };

    struct __declspec(align(4)) cLeaf_t
    {
        unsigned __int16 firstCollAabbIndex;
        unsigned __int16 collAabbCount;
        int brushContents;
        int terrainContents;
        float mins[3];
        float maxs[3];
        int leafBrushNode;
        __int16 cluster;
    };

    struct cLeafBrushNodeLeaf_t
    {
        unsigned __int16* brushes;
    };

    struct cLeafBrushNodeChildren_t
    {
        float dist;
        float range;
        unsigned __int16 childOffset[2];
    };

    union cLeafBrushNodeData_t
    {
        cLeafBrushNodeLeaf_t leaf;
        cLeafBrushNodeChildren_t children;
    };

    struct cLeafBrushNode_s
    {
        unsigned __int8 axis;
        __int16 leafBrushCount;
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
        unsigned __int8 triCount;
        unsigned __int8 borderCount;
        int firstTri;
        int nuinds;
        int fuind;
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
        unsigned __int16 materialIndex;
        unsigned __int16 childCount;
        float halfSize[3];
        CollisionAabbTreeIndex u;
    };

    struct __declspec(align(16)) cbrush_t
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        __int16 axialMaterialNum[2][3];
        unsigned __int8* baseAdjacentSide;
        __int16 firstAdjacentSideOffsets[2][3];
        unsigned __int8 edgeCount[2][3];
        unsigned int numverts;
        float (*verts)[3];
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
        unsigned __int16 brushModel;
        unsigned __int16 physicsBrushModel;
        FxEffectDef* destroyFx;
        XModelPieces* destroyPieces;
        PhysPreset* physPreset;
        unsigned __int8 physConstraints[4];
        int health;
        PhysMass mass;
        int flags;
        int contents;
        ScriptString targetname;
    };

    struct DynEntityPose
    {
        GfxPlacement pose;
        float radius;
    };

    struct DynEntityClient
    {
        int physObjId;
        unsigned __int16 flags;
        unsigned __int16 lightingHandle;
        int health;
        unsigned __int16 burnTime;
        unsigned __int16 fadeTime;
    };

    struct DynEntityServer
    {
        unsigned __int16 flags;
        int health;
    };

    struct DynEntityColl
    {
        unsigned __int16 sector;
        unsigned __int16 nextEntInSector;
        float linkMins[3];
        float linkMaxs[3];
        int contents;
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
        unsigned __int8* brushEdges;
        unsigned int numNodes;
        cNode_t* nodes;
        unsigned int numLeafs;
        cLeaf_t* leafs;
        unsigned int leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        unsigned int numLeafBrushes;
        unsigned __int16* leafbrushes;
        unsigned int numLeafSurfaces;
        unsigned int* leafsurfaces;
        unsigned int vertCount;
        float (*verts)[3];
        unsigned int numBrushVerts;
        float (*brushVerts)[3];
        unsigned int nuinds;
        unsigned __int16* uinds;
        int triCount;
        unsigned __int16* triIndices;
        unsigned __int8* triEdgeIsWalkable;
        int borderCount;
        CollisionBorder* borders;
        int partitionCount;
        CollisionPartition* partitions;
        int aabbTreeCount;
        CollisionAabbTree* aabbTrees;
        unsigned int numSubModels;
        cmodel_t* cmodels;
        unsigned __int16 numBrushes;
        cbrush_t* brushes;
        int numClusters;
        int clusterBytes;
        unsigned __int8* visibility;
        int vised;
        MapEnts* mapEnts;
        cbrush_t* box_brush;
        cmodel_t box_model;
        unsigned __int16 originalDynEntCount;
        unsigned __int16 dynEntCount[4];
        DynEntityDef* dynEntDefList[2];
        DynEntityPose* dynEntPoseList[2];
        DynEntityClient* dynEntClientList[2];
        DynEntityServer* dynEntServerList[2];
        DynEntityColl* dynEntCollList[4];
        int num_constraints;
        PhysConstraint* constraints;
        unsigned int checksum;
    };

    struct ComPrimaryLight
    {
        unsigned __int8 type;
        unsigned __int8 canUseShadowMap;
        unsigned __int8 exponent;
        char priority;
        __int16 cullDist;
        char _pad[2];
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

    struct ComWaterHeader
    {
        int minx;
        int miny;
        int maxx;
        int maxy;
    };

    struct type_align(4) ComWaterCell
    {
        __int16 waterheight;
        char flooroffset;
        unsigned __int8 shoredist;
        unsigned __int8 color[4];
    };

    struct ComBurnableHeader
    {
        int minx;
        int miny;
        int maxx;
        int maxy;
    };

    struct ComBurnableSample
    {
        unsigned __int8 state;
    };

    struct ComBurnableCell
    {
        int x;
        int y;
        ComBurnableSample* data;
    };

    struct ComWorld
    {
        const char* name;
        int isInUse;
        unsigned int primaryLightCount;
        ComPrimaryLight* primaryLights;
        ComWaterHeader waterHeader;
        unsigned int numWaterCells;
        ComWaterCell* waterCells;
        ComBurnableHeader burnableHeader;
        unsigned int numBurnableCells;
        ComBurnableCell* burnableCells;
    };

    struct pathlink_s
    {
        float fDist;
        unsigned __int16 nodeNum;
        unsigned __int8 disconnectCount;
        unsigned __int8 negotiationLink;
        unsigned __int8 ubBadPlaceCount[4];
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
        unsigned __int16 spawnflags;
        unsigned __int16 targetname;
        unsigned __int16 script_linkName;
        unsigned __int16 script_noteworthy;
        unsigned __int16 target;
        unsigned __int16 animscript;
        int animscriptfunc;
        float vOrigin[3];
        float fAngle;
        float forward[2];
        float fRadius;
        float minUseDistSq;
        __int16 wOverlapNode[2];
        __int16 wChainId;
        __int16 wChainDepth;
        __int16 wChainParent;
        unsigned __int16 totalLinkCount;
        pathlink_s* Links;
    };

    struct pathnode_dynamic_t
    {
        void* pOwner;
        int iFreeTime;
        int iValidTime[3];
        int inPlayerLOSTime;
        __int16 wLinkCount;
        __int16 wOverlapCount;
        __int16 turretEntNumber;
        __int16 userCount;
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
        unsigned __int16* nodes;
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
        unsigned __int16* chainNodeForNode;
        unsigned __int16* nodeForChainNode;
        int visBytes;
        unsigned __int8* pathVis;
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
        PathData path;
    };

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int numEntityChars;
    };

    struct GfxLight
    {
        unsigned __int8 type;
        unsigned __int8 canUseShadowMap;
        __int16 cullDist;
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
        unsigned __int16 firstItem;
        unsigned __int16 itemCount;
        unsigned __int16 firstChild;
        unsigned __int16 childCount;
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

    struct GfxLightCorona
    {
        float origin[3];
        float radius;
        float color[3];
        float intensity;
    };

    struct GfxAabbTree
    {
        float mins[3];
        float maxs[3];
        unsigned __int16 childCount;
        unsigned __int16 surfaceCount;
        unsigned __int16 startSurfIndex;
        unsigned __int16 smodelIndexCount;
        unsigned __int16* smodelIndexes;
        int childrenOffset;
    };

    struct GfxPortal;

    struct GfxPortalWritable
    {
        bool isQueued;
        bool isAncestor;
        unsigned __int8 recursionDepth;
        unsigned __int8 hullPointCount;
        float (*hullPoints)[2];
        GfxPortal* queuedParent;
    };

    struct DpvsPlane
    {
        float coeffs[4];
        unsigned __int8 side[3];
        unsigned __int8 pad;
    };

    struct GfxCell;

    struct GfxPortal
    {
        GfxPortalWritable writable;
        DpvsPlane plane;
        GfxCell* cell;
        float (*vertices)[3];
        unsigned __int8 vertexCount;
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
        unsigned __int8 reflectionProbeCount;
        unsigned __int8* reflectionProbes;
    };

    struct GfxLightmapArray
    {
        GfxImage* primary;
        GfxImage* secondary;
    };

    struct GfxLightGridEntry
    {
        unsigned __int16 colorsIndex;
        unsigned __int8 primaryLightIndex;
        unsigned __int8 needsTrace;
    };

    struct GfxLightGridColors
    {
        unsigned __int8 rgb[56][3];
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
        unsigned __int16 lod;
        unsigned __int16 surfId;
    };

    struct GfxSceneDynModel
    {
        XModelDrawInfo info;
        unsigned __int16 dynEntId;
    };

    struct BModelDrawInfo
    {
        unsigned __int16 surfId;
    };

    struct GfxSceneDynBrush
    {
        BModelDrawInfo info;
        unsigned __int16 dynEntId;
    };

    struct GfxShadowGeometry
    {
        unsigned __int16 surfaceCount;
        unsigned __int16 smodelCount;
        unsigned __int16* sortedSurfIndex;
        unsigned __int16* smodelIndex;
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
        unsigned __int16 vertexCount;
        unsigned __int16 triCount;
        int baseIndex;
        float topMipMins[3];
        float topMipMaxs[3];
    };

    struct GfxSurface
    {
        srfTriangles_t tris;
        int pad;
        Material* material;
        unsigned __int8 lightmapIndex;
        unsigned __int8 reflectionProbeIndex;
        unsigned __int8 primaryLightIndex;
        unsigned __int8 flags;
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

    struct __declspec(align(4)) GfxStaticModelDrawInst
    {
        float cullDist;
        GfxPackedPlacement placement;
        XModel* model;
        unsigned __int8 reflectionProbeIndex;
        unsigned __int8 primaryLightIndex;
        unsigned __int16 lightingHandle;
        unsigned __int8 flags;
        unsigned __int8 pad[19];
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
        unsigned __int8* data;
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
        float treeScatterIntensity;
        float treeScatterAmount;
    };

    struct GfxWorldDpvsPlanes
    {
        int cellCount;
        cplane_s* planes;
        unsigned __int16* nodes;
        unsigned int* sceneEntCellBits;
    };

    struct GfxLightGrid
    {
        bool hasLightRegions;
        unsigned int sunPrimaryLightIndex;
        unsigned __int16 mins[3];
        unsigned __int16 maxs[3];
        unsigned int rowAxis;
        unsigned int colAxis;
        unsigned __int16* rowDataStart;
        unsigned int rawRowDataSize;
        unsigned __int8* rawRowData;
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
        unsigned __int8* smodelVisData[3];
        unsigned __int8* surfaceVisData[3];
        unsigned int* lodData;
        unsigned __int16* sortedSurfIndex;
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
        unsigned __int8* dynEntVisData[2][3];
    };

    struct GfxWorldLodChain
    {
        float origin[3];
        float lastDist;
        unsigned int firstLodInfo;
        unsigned __int16 lodInfoCount;
    };

    struct GfxWorldLodInfo
    {
        float dist;
        unsigned int firstSurf;
        unsigned __int16 surfCount;
    };

    struct GfxWaterBuffer
    {
        unsigned int bufferSize;
        float (*buffer)[4];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        int planeCount;
        int nodeCount;
        int indexCount;
        unsigned __int16* indices;
        D3DIndexBuffer indexBuffer;
        int surfaceCount;
        GfxWorldStreamInfo streamInfo;
        int skySurfCount;
        int* skyStartSurfs;
        GfxImage* skyImage;
        unsigned __int8 skySamplerState;
        const char* skyBoxModel;
        unsigned int vertexCount;
        GfxWorldVertexData vd;
        unsigned int vertexLayerDataSize;
        GfxWorldVertexLayerData vld;
        unsigned int vertexStream2DataSize;
        SunLightParseParams sunParse;
        GfxLight* sunLight;
        float sunColorFromBsp[3];
        unsigned int sunPrimaryLightIndex;
        unsigned int primaryLightCount;
        int cullGroupCount;
        unsigned int reflectionProbeCount;
        GfxReflectionProbe* reflectionProbes;
        GfxTexture* reflectionProbeTextures;
        unsigned int coronaCount;
        GfxLightCorona* coronas;
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
        unsigned __int8* nonSunPrimaryLightForModelDynEnt;
        GfxShadowGeometry* shadowGeom;
        GfxLightRegion* lightRegion;
        GfxWorldDpvsStatic dpvs;
        GfxWorldDpvsDynamic dpvsDyn;
        unsigned int worldLodChainCount;
        GfxWorldLodChain* worldLodChains;
        unsigned int worldLodInfoCount;
        GfxWorldLodInfo* worldLodInfos;
        unsigned int worldLodSurfaceCount;
        unsigned int* worldLodSurfaces;
        float waterDirection;
        GfxWaterBuffer waterBuffers[2];
        Material* waterMaterial;
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
        float focusColor[4];
        Material* selectIcon;
        Material* backgroundItemListbox;
        Material* highlightTexture;
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
        OP_GAMEHOST = 0x50,
        OP_ISSPLITSCREEN = 0x51,
        OP_ISSPLITSCREENHOST = 0x52,
        OP_SPLITSCREENNUM = 0x53,
        OP_ISCINEMATICFINISHED = 0x54,
        OP_ISSQUADLEADER = 0x55,
        OP_GETSQUADID = 0x56,
        OP_ISARTILLERY = 0x57,
        OP_GETPLAYERSQUADID = 0x58,
        OP_HASFRIENDS = 0x59,
        OP_HASPENDINGFRIENDS = 0x5A,
        OP_HASINVITES = 0x5B,
        OP_SQUADINVITESENT = 0x5C,
        OP_ISINTERMISSION = 0x5D,
        NUM_OPERATORS = 0x5E,
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
        WINDOW_FLAG_VISIBLE = 0x4,
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

    enum ItemDefFlag : unsigned int
    {
        ITEM_FLAG_SAVE_GAME_INFO = 0x1,
        ITEM_FLAG_CINEMATIC_SUBTITLE = 0x2,
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
        const char* onListboxSelectionChange;
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
        const char* onFocus;
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

    struct flameTable
    {
        float flameVars[106];
        const char* name;
        Material* fire;
        Material* smoke;
        Material* heat;
        Material* drips;
        Material* streamFuel;
        Material* streamFuel2;
        Material* streamFlame;
        Material* streamFlame2;
        SndAliasCustom flameOffLoopSound;
        SndAliasCustom flameIgniteSound;
        SndAliasCustom flameOnLoopSound;
        SndAliasCustom flameCooldownSound;
    };

    struct WeaponDef
    {
        const char* szInternalName;
        const char* szDisplayName;
        const char* szOverlayName;
        XModel* gunXModel[16];
        XModel* handXModel;
        const char* szXAnims[35];
        const char* szModeName;
        unsigned __int16 hideTags[8];
        unsigned __int16 notetrackSoundMapKeys[20];
        unsigned __int16 notetrackSoundMapValues[20];
        int playerAnimType;
        weapType_t weapType;
        weapClass_t weapClass;
        PenetrateType penetrateType;
        ImpactType impactType;
        weapInventoryType_t inventoryType;
        weapFireType_t fireType;
        int clipType;
        int overheatWeapon;
        float overheatRate;
        float cooldownRate;
        float overheatEndVal;
        int coolWhileFiring;
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
        SndAliasCustom crackSound;
        SndAliasCustom whizbySound;
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
        SndAliasCustom rotateLoopSound;
        SndAliasCustom rotateLoopSoundPlayer;
        SndAliasCustom deploySound;
        SndAliasCustom deploySoundPlayer;
        SndAliasCustom finishDeploySound;
        SndAliasCustom finishDeploySoundPlayer;
        SndAliasCustom breakdownSound;
        SndAliasCustom breakdownSoundPlayer;
        SndAliasCustom finishBreakdownSound;
        SndAliasCustom finishBreakdownSoundPlayer;
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
        SndAliasCustom overheatSound;
        SndAliasCustom overheatSoundPlayer;
        SndAliasCustom* bounceSound;
        const char* standMountedWeapdef;
        const char* crouchMountedWeapdef;
        const char* proneMountedWeapdef;
        int StandMountedIndex;
        int CrouchMountedIndex;
        int ProneMountedIndex;
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
        float duckedSprintOfs[3];
        float duckedSprintRot[3];
        float duckedSprintBob[2];
        float duckedSprintScale;
        float sprintOfs[3];
        float sprintRot[3];
        float sprintBob[2];
        float sprintScale;
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
        XModel* mountedModel;
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
        int iHeatIndex;
        int iMaxAmmo;
        int iClipSize;
        int shotCount;
        const char* szSharedAmmoCapName;
        int iSharedAmmoCapIndex;
        int iSharedAmmoCap;
        int unlimitedAmmo;
        int damage;
        int damageDuration;
        int damageInterval;
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
        int reloadEmptyAddTime;
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
        int deployTime;
        int breakdownTime;
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
        int mountableWeapon;
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
        int canUseInVehicle;
        int noDropsOrRaises;
        int adsFireOnly;
        int cancelAutoHolsterWhenEmpty;
        int suppressAmmoReserveDisplay;
        int enhanced;
        int laserSightDuringNightvision;
        int bayonet;
        Material* killIcon;
        weaponIconRatioType_t killIconRatio;
        int flipKillIcon;
        Material* dpadIcon;
        weaponIconRatioType_t dpadIconRatio;
        int bNoPartialReload;
        int bSegmentedReload;
        int noADSAutoReload;
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
        SndAliasCustom mortarShellSound;
        SndAliasCustom tankShellSound;
        int bProjImpactExplode;
        WeapStickinessType stickiness;
        int hasDetonator;
        int timedDetonation;
        int rotate;
        int holdButtonToThrow;
        int freezeMovementWhenFiring;
        float lowAmmoWarningThreshold;
        float parallelBounce[31];
        float perpendicularBounce[31];
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
        float hipDofStart;
        float hipDofEnd;
        const char* flameTableFirstPerson;
        const char* flameTableThirdPerson;
        flameTable* flameTableFirstPersonPtr;
        flameTable* flameTableThirdPersonPtr;
        FxEffectDef* tagFx_preparationEffect;
        FxEffectDef* tagFlash_preparationEffect;
    };

    struct snd_reverb_patch_t
    {
        char name[32];
        float room;
        float roomHF;
        float roomRolloffFactor;
        float decayTime;
        float decayHFRatio;
        float reflections;
        float reflectionsDelay;
        float reverb;
        float reverbDelay;
        float diffusion;
        float density;
        float HFReference;
    };

    struct snd_bus_info_t
    {
        char name[32];
        float volumeMod;
        int maxVoices;
        int restricted;
        int pausable;
        int stopOnDeath;
        int isMusic;
        int category;
        int voiceLimitType;
    };

    struct snd_curve_t
    {
        char name[32];
        int pointCount;
        float points[8][2];
    };

    struct snd_speaker_map
    {
        int input_channel_count;
        int output_channel_count;
        float volumes[16];
    };

    struct snd_speaker_map_set
    {
        char name[32];
        snd_speaker_map channelMaps[4];
    };

    struct snd_master_effect_t
    {
        char name[32];
        float masterRingmod;
        float reverbRingmod;
        float masterLPRatio;
        float masterLPDB;
        float masterHPRatio;
        float masterHPDB;
    };

    struct SndDriverGlobals
    {
        const char* name;
        snd_reverb_patch_t reverbPatches[64];
        snd_bus_info_t buses[64];
        snd_curve_t curves[32];
        snd_speaker_map_set speakerMaps[32];
        snd_master_effect_t masterEffects[16];
    };

    enum FxElemType
    {
        FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
        FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
        FX_ELEM_TYPE_TAIL = 0x2,
        FX_ELEM_TYPE_LINE = 0x3,
        FX_ELEM_TYPE_TRAIL = 0x4,
        FX_ELEM_TYPE_CLOUD = 0x5,
        FX_ELEM_TYPE_MODEL = 0x6,
        FX_ELEM_TYPE_OMNI_LIGHT = 0x7,
        FX_ELEM_TYPE_SPOT_LIGHT = 0x8,
        FX_ELEM_TYPE_SOUND = 0x9,
        FX_ELEM_TYPE_DECAL = 0xA,
        FX_ELEM_TYPE_RUNNER = 0xB,
        FX_ELEM_TYPE_COUNT = 0xC,
        FX_ELEM_TYPE_LAST_SPRITE = 0x4,
        FX_ELEM_TYPE_LAST_DRAWN = 0x8,
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
        unsigned __int8 color[4];
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
        unsigned __int16* inds;
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
        unsigned __int8 behavior;
        unsigned __int8 index;
        unsigned __int8 fps;
        unsigned __int8 loopCount;
        unsigned __int8 colIndexBits;
        unsigned __int8 rowIndexBits;
        __int16 entryCount;
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
        float windInfluence;
        unsigned __int8 elemType;
        unsigned __int8 visualCount;
        unsigned __int8 velIntervalCount;
        unsigned __int8 visStateIntervalCount;
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
        unsigned __int8 sortOrder;
        unsigned __int8 lightingFrac;
        unsigned __int8 useItemClip;
        unsigned __int8 unused[1];
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
        unsigned __int8 efPriority;
        unsigned __int8 padding[3];
        FxElemDef* elemDefs;
    };

    struct FxImpactEntry
    {
        FxEffectDef* nonflesh[31];
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
} // namespace T4Xenon
#endif

#endif // __T4XENON_ASSETS_H
