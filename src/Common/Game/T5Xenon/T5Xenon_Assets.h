#pragma once

#ifndef __T5XENON_ASSETS_H
#define __T5XENON_ASSETS_H

#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace T5Xenon
{
#endif
    struct PhysPreset;
    struct PhysConstraints;
    struct FxEffectDef;
    struct Material;
    struct menuDef_t;
    struct XModel;
    struct ComWorld;
    struct GameWorldSp;
    struct GameWorldMp;
    struct MapEnts;
    struct clipMap_t;

    enum XAssetType
    {
        ASSET_TYPE_XMODELPIECES,
        ASSET_TYPE_PHYSPRESET,
        ASSET_TYPE_PHYSCONSTRAINTS,
        ASSET_TYPE_DESTRUCTIBLEDEF,
        ASSET_TYPE_XANIMPARTS,
        ASSET_TYPE_XMODEL,
        ASSET_TYPE_MATERIAL,
        ASSET_TYPE_PIXEL_SHADER,
        ASSET_TYPE_TECHNIQUE_SET,
        ASSET_TYPE_IMAGE,
        ASSET_TYPE_SOUND,
        ASSET_TYPE_SOUND_PATCH,
        ASSET_TYPE_CLIPMAP_SP,
        ASSET_TYPE_CLIPMAP_MP,
        ASSET_TYPE_COMWORLD,
        ASSET_TYPE_GAMEWORLD_SP,
        ASSET_TYPE_GAMEWORLD_MP,
        ASSET_TYPE_MAP_ENTS,
        ASSET_TYPE_GFXWORLD,
        ASSET_TYPE_LIGHT_DEF,
        ASSET_TYPE_UI_MAP,
        ASSET_TYPE_FONT,
        ASSET_TYPE_MENULIST,
        ASSET_TYPE_MENU,
        ASSET_TYPE_LOCALIZE_ENTRY,
        ASSET_TYPE_WEAPON,
        ASSET_TYPE_WEAPONDEF,
        ASSET_TYPE_WEAPON_VARIANT,
        ASSET_TYPE_SNDDRIVER_GLOBALS,
        ASSET_TYPE_FX,
        ASSET_TYPE_IMPACT_FX,
        ASSET_TYPE_AITYPE,
        ASSET_TYPE_MPTYPE,
        ASSET_TYPE_MPBODY,
        ASSET_TYPE_MPHEAD,
        ASSET_TYPE_CHARACTER,
        ASSET_TYPE_XMODELALIAS,
        ASSET_TYPE_RAWFILE,
        ASSET_TYPE_STRINGTABLE,
        ASSET_TYPE_PACK_INDEX,
        ASSET_TYPE_XGLOBALS,
        ASSET_TYPE_DDL,
        ASSET_TYPE_GLASSES,
        ASSET_TYPE_TEXTURE_LIST,
        ASSET_TYPE_EMBLEMSET,

        ASSET_TYPE_COUNT,

        // DB-only types, excluded from assetNames.
        ASSET_TYPE_STRING = ASSET_TYPE_COUNT,
        ASSET_TYPE_ASSETLIST,
        ASSET_TYPE_FULLCOUNT
    };

    enum XFileBlock
    {
        XFILE_BLOCK_TEMP = 0,
        XFILE_BLOCK_RUNTIME = 1,
        XFILE_BLOCK_LARGE_RUNTIME = 2,
        XFILE_BLOCK_PHYSICAL_RUNTIME = 3,
        XFILE_BLOCK_VIRTUAL = 4,
        XFILE_BLOCK_LARGE = 5,
        XFILE_BLOCK_PHYSICAL = 6,

        MAX_XFILE_COUNT
    };

    typedef unsigned short ScriptString;

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
        float v[3];

        struct
        {
            float x;
            float y;
            float z;
        };
    };

    struct PhysPreset
    {
        const char* name;
        int flags;
        float mass;
        float bounce;
        float friction;
        float bulletForceScale;
        float explosiveForceScale;
        const char* sndAliasPrefix;
        float piecesSpreadFraction;
        float piecesUpwardVelocity;
        int canFloat;
        float gravityScale;
        float centerOfMassOffset[3];
        float buoyancyBoxMin[3];
        float buoyancyBoxMax[3];
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
        vec3_t offset;
        vec3_t pos;
        vec3_t pos2;
        vec3_t dir;
        int flags;
        int timeout;
        int min_health;
        int max_health;
        float distance;
        float damp;
        float power;
        vec3_t scale;
        float spin_scale;
        float minAngle;
        float maxAngle;
        Material* material;
        int constraintHandle;
        int rope_index;
        int centity_num[4];
    };

    struct PhysConstraints
    {
        const char* name;
        unsigned int count;
        PhysConstraint data[16];
    };

    struct DestructibleStage
    {
        ScriptString showBone;
        float breakHealth;
        float maxTime;
        unsigned int flags;
        FxEffectDef* breakEffect;
        const char* breakSound;
        const char* breakNotify;
        const char* loopSound;
        XModel* spawnModel[3];
        PhysPreset* physPreset;
    };

    struct DestructiblePiece
    {
        DestructibleStage stages[5];
        char parentPiece;
        float parentDamagePercent;
        float bulletDamageScale;
        float explosiveDamageScale;
        float meleeDamageScale;
        float impactDamageScale;
        float entityDamageTransfer;
        PhysConstraints* physConstraints;
        int health;
        const char* damageSound;
        FxEffectDef* burnEffect;
        const char* burnSound;
        ScriptString enableLabel;
        int hideBones[5];
    };

    struct DestructibleDef
    {
        const char* name;
        XModel* model;
        XModel* pristineModel;
        int numPieces;
        DestructiblePiece* pieces;
        int clientOnly;
    };

    struct ComPrimaryLight
    {
        char type;
        char canUseShadowMap;
        char exponent;
        char priority;
        int16_t cullDist;
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
        float mipDistance;
        float diffuseColor[4];
        float specularColor[4];
        float attenuation[4];
        float falloff[4];
        float angle[4];
        float aAbB[4];
        float cookieControl0[4];
        float cookieControl1[4];
        float cookieControl2[4];
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
        int16_t waterheight;
        char flooroffset;
        char shoredist;
        char color[4];
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
        char state;
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

    struct pathnode_t;

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
        NODE_COVER_PILLAR = 0xA,
        NODE_CONCEALMENT_STAND = 0xB,
        NODE_CONCEALMENT_CROUCH = 0xC,
        NODE_CONCEALMENT_PRONE = 0xD,
        NODE_REACQUIRE = 0xE,
        NODE_BALCONY = 0xF,
        NODE_SCRIPTED = 0x10,
        NODE_NEGOTIATION_BEGIN = 0x11,
        NODE_NEGOTIATION_END = 0x12,
        NODE_TURRET = 0x13,
        NODE_GUARD = 0x14,
        NODE_NUMTYPES = 0x15,
        NODE_DONTLINK = 0x15,
    };

    struct pathlink_s
    {
        float fDist;
        uint16_t nodeNum;
        char disconnectCount;
        char negotiationLink;
        char ubBadPlaceCount[4];
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

    struct SentientHandle
    {
        uint16_t number;
        uint16_t infoIndex;
    };

    struct pathnode_dynamic_t
    {
        SentientHandle pOwner;
        int iFreeTime;
        int iValidTime[3];
        int inPlayerLOSTime;
        int16_t wLinkCount;
        int16_t wOverlapCount;
        int16_t turretEntNumber;
        int16_t userCount;
    };

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

    struct type_align(16) pathbasenode_t
    {
        float vOrigin[3];
        unsigned int type;
    };

    struct pathnode_tree_t;

    struct pathnode_tree_nodes_t
    {
        int nodeCount;
        uint16_t* nodes;
    };

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
        char* pathVis;
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

    struct XAnimNotifyInfo
    {
        ScriptString name;
        float time;
    };

    union XAnimDynamicFrames
    {
        unsigned char (*_1)[3];
        unsigned short (*_2)[3];
    };

    union XAnimDynamicIndicesTrans
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    struct type_align(4) XAnimPartTransFrames
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
        unsigned short size;
        unsigned char smallTrans;
        XAnimPartTransData u;
    };

    union XAnimDynamicIndicesQuat
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    struct type_align(4) XAnimDeltaPartQuatDataFrames
    {
        short (*frames)[2];
        XAnimDynamicIndicesQuat indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatDataFrames frames;
        short frame0[2];
    };

    struct XAnimDeltaPartQuat
    {
        unsigned short size;
        XAnimDeltaPartQuatData u;
    };

    struct XAnimDeltaPart
    {
        XAnimPartTrans* trans;
        XAnimDeltaPartQuat* quat;
    };

    union XAnimIndices
    {
        unsigned char* _1;
        unsigned short* _2;
        void* data;
    };

    struct XAnimParts
    {
        const char* name;
        unsigned short dataByteCount;
        unsigned short dataShortCount;
        unsigned short dataIntCount;
        unsigned short randomDataByteCount;
        unsigned short randomDataIntCount;
        unsigned short numframes;
        bool bLoop;
        bool bDelta;
        bool bLeftHandGripIK;
        bool bStreamable;
        unsigned int streamedFileSize;
        unsigned char boneCount[12];
        unsigned char notifyCount;
        unsigned char assetType;
        bool isDefault;
        unsigned char pad;
        unsigned int randomDataShortCount;
        unsigned int indexCount;
        float framerate;
        float frequency;
        float primedLength;
        float loopEntryTime;
        ScriptString* names;
        unsigned char* dataByte;
        short* dataShort;
        int* dataInt;
        short* randomDataShort;
        unsigned char* randomDataByte;
        int* randomDataInt;
        XAnimIndices indices;
        XAnimNotifyInfo* notify;
        XAnimDeltaPart* deltaPart;
    };

    struct MaterialStreamRouting
    {
        unsigned char source;
        unsigned char dest;
    };

    union MaterialVertexStreamRouting
    {
        MaterialStreamRouting data[16];
        // Overwrites routing data after loading.
        void* decl[17];
    };

    struct MaterialVertexDeclaration
    {
        unsigned char streamCount;
        unsigned char unknown1[3];
        MaterialVertexStreamRouting routing;
    };

    struct GfxVertexShaderLoadDef
    {
        unsigned char* cachedPart;
        unsigned char* physicalPart;
        unsigned short cachedPartSize;
        unsigned short physicalPartSize;
    };

    union MaterialVertexShaderProgram
    {
        void* vs;
        GfxVertexShaderLoadDef loadDef;
    };

    struct MaterialVertexShader
    {
        const char* name;
        MaterialVertexShaderProgram prog;
    };

    struct GfxPixelShaderLoadDef
    {
        unsigned char* cachedPart;
        unsigned char* physicalPart;
        unsigned short cachedPartSize;
        unsigned short physicalPartSize;
    };

    union MaterialPixelShaderProgram
    {
        void* ps;
        GfxPixelShaderLoadDef loadDef;
    };

    struct MaterialPixelShader
    {
        const char* name;
        MaterialPixelShaderProgram prog;
    };

    struct MaterialArgumentCodeConst
    {
        unsigned short index;
        unsigned char firstRow;
        unsigned char rowCount;
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
        MTL_ARG_MATERIAL_VERTEX_CONST = 0,
        MTL_ARG_LITERAL_VERTEX_CONST = 1,
        MTL_ARG_MATERIAL_PIXEL_SAMPLER = 2,
        MTL_ARG_CODE_VERTEX_CONST = 3,
        MTL_ARG_CODE_PIXEL_SAMPLER = 4,
        MTL_ARG_CODE_PIXEL_CONST = 5,
        MTL_ARG_MATERIAL_PIXEL_CONST = 6,
        MTL_ARG_LITERAL_PIXEL_CONST = 7,
    };

    struct MaterialShaderArgument
    {
        unsigned short type;
        unsigned short dest;
        MaterialArgumentDef u;
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShaderArray[17];
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        unsigned char perPrimArgCount;
        unsigned char perObjArgCount;
        unsigned char stableArgCount;
        unsigned char customSamplerFlags;
        unsigned char precompiledIndex;
        unsigned char unknown85[3];
        MaterialShaderArgument* args;
    };

    struct MaterialTechnique
    {
        const char* name;
        unsigned short flags;
        unsigned short passCount;
        MaterialPass passArray[1];
    };

    struct MaterialTechniqueSet
    {
        const char* name;
        unsigned char worldVertFormat;
        unsigned char unknown5[3];
        MaterialTechnique* techniques[71];
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

    enum MapType : unsigned char
    {
        MAPTYPE_NONE = 0,
        MAPTYPE_INVALID1 = 1,
        MAPTYPE_INVALID2 = 2,
        MAPTYPE_2D = 3,
        MAPTYPE_3D = 4,
        MAPTYPE_CUBE = 5,
    };

    struct CardMemory
    {
        int platform[1];
    };

    struct GfxImageStream
    {
        unsigned int pixelSize; // Cumulative bytes in bits 4-31, mip count in bits 0-3.
        uint16_t width;
        uint16_t height;
        unsigned int packIndex;
    };

    struct GfxImage
    {
        unsigned char texture[52];
        MapType mapType;
        unsigned char semantic;
        unsigned char category;
        bool delayLoadPixels;
        CardMemory cardMemory;
        uint16_t width;
        uint16_t height;
        uint16_t depth;
        unsigned char levelCount;
        // 3 marks temporary streamed images.
        unsigned char streaming;
        unsigned int baseSize;
        unsigned char* pixels;
        unsigned int format;
        unsigned int unknown80; // Used for stream importance.
        GfxImageStream streams[5];
        unsigned char streamedPartCount;
        unsigned char track;
        unsigned char unknown146[2]; // Mip-fading state.
        const char* name;
        unsigned int hash;
    };

    struct GfxLightImage
    {
        GfxImage* image;
        unsigned char samplerState;
    };

    struct GfxLightDef
    {
        const char* name;
        GfxLightImage attenuation;
        int lmapLookupStart;
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
        TS_COLOR0_MAP = 0xC,
        TS_COLOR1_MAP = 0xD,
        TS_COLOR2_MAP = 0xE,
        TS_COLOR3_MAP = 0xF,
        TS_COLOR4_MAP = 0x10,
        TS_COLOR5_MAP = 0x11,
        TS_COLOR6_MAP = 0x12,
        TS_COLOR7_MAP = 0x13,
        TS_COLOR8_MAP = 0x14,
        TS_COLOR9_MAP = 0x15,
        TS_COLOR10_MAP = 0x16,
        TS_COLOR11_MAP = 0x17,
        TS_COLOR12_MAP = 0x18,
        TS_COLOR13_MAP = 0x19,
        TS_COLOR14_MAP = 0x1A,
        TS_COLOR15_MAP = 0x1B,
        TS_THROW_MAP = 0x1C,
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
        unsigned char samplerState;
        unsigned char semantic;
        bool isMatureContent;
        unsigned char pad[3];
        MaterialTextureDefInfo u;
    };

    struct GfxStateBits
    {
        unsigned int loadBits[2];
    };

    struct type_align(16) MaterialConstantDef
    {
        unsigned int nameHash;
        char name[12];
        float literal[4];
    };

    struct MaterialInfo
    {
        const char* name;
        unsigned int gameFlags;
        unsigned char unknown8;
        unsigned char sortKey;
        unsigned char textureAtlasRowCount;
        unsigned char textureAtlasColumnCount;
        unsigned char unknown12[4];
        // Two words preserve four-byte alignment.
        unsigned int drawSurf[2];
        unsigned int surfaceTypeBits;
        unsigned int layeredSurfaceTypes;
    };

    struct Material
    {
        MaterialInfo info;
        char stateBitsEntry[71];
        unsigned char textureCount;
        unsigned char constantCount;
        unsigned char stateBitsCount;
        unsigned char stateFlags;
        unsigned char cameraRegion;
        unsigned char unknown108[4];
        MaterialTechniqueSet* techniqueSet;
        MaterialTextureDef* textureTable;
        MaterialConstantDef* constantTable;
        GfxStateBits* stateBitsTable;
    };

    struct DObjAnimMat
    {
        float quat[4];
        float trans[3];
        float transWeight;
    };

    struct type_align(16) GfxPackedVertex
    {
        float xyz[3];
        float binormalSign;
        unsigned int color;
        unsigned int texCoord;
        unsigned int normal;
        unsigned int tangent;
    };

    struct type_align(8) GfxPackedVertexQuant
    {
        unsigned char data[24];
    };

    struct XSurfaceCollisionAabb
    {
        unsigned short mins[3];
        unsigned short maxs[3];
    };

    struct type_align(16) XSurfaceCollisionNode
    {
        XSurfaceCollisionAabb aabb;
        unsigned short childBeginIndex;
        unsigned short childCount;
    };

    struct XSurfaceCollisionLeaf
    {
        unsigned short triangleBeginIndex;
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
        unsigned short boneOffset;
        unsigned short vertCount;
        unsigned short triOffset;
        unsigned short triCount;
        XSurfaceCollisionTree* collisionTree;
    };

    struct XSurfaceVertexInfo
    {
        short vertCount[4];
        unsigned short* vertsBlend;
        float* tensionData;
    };

    union GfxPackedVertex0Union
    {
        GfxPackedVertex* packed;
        GfxPackedVertexQuant* quantized;
        void* data;
    };

    struct D3DVertexBuffer
    {
        unsigned char data[32];
    };

    struct D3DIndexBuffer
    {
        unsigned char data[32];
    };

    enum XSurfaceFlags
    {
        XSURFACE_FLAG_QUANTIZED = 0x1,
    };

    struct XSurface
    {
        unsigned char tileMode;
        unsigned char vertListCount;
        unsigned short flags;
        unsigned short vertCount;
        unsigned short triCount;
        unsigned short* triIndices;
        XSurfaceVertexInfo vertInfo;
        GfxPackedVertex0Union verts0;
        D3DVertexBuffer vb0;
        XRigidVertList* vertList;
        D3DIndexBuffer indexBuffer;
        int partBits[5];
        unsigned char unknown120[16];
    };

    struct XModelLodInfo
    {
        float dist;
        unsigned short numsurfs;
        unsigned short surfIndex;
        int partBits[5];
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
        unsigned char collmap;
        unsigned char pad[3];
    };

    struct XModelHighMipBounds
    {
        float center[3];
        float himipRadiusSq;
    };

    struct XModelStreamInfo
    {
        XModelHighMipBounds* highMipBounds;
        unsigned char unknown4[12];
    };

    struct cplane_s
    {
        float normal[3];
        float dist;
        unsigned char type;
        unsigned char signbits;
        unsigned char pad[2];
    };

    struct cbrushside_t
    {
        cplane_s* plane;
        int cflags;
        int sflags;
    };

    struct type_align(16) BrushWrapper
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        int axialCFlags[2][3];
        int axialSFlags[2][3];
        unsigned int numverts;
        float (*verts)[3];
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

    struct PhysGeomList
    {
        unsigned int count;
        PhysGeomInfo* geoms;
        int contents;
    };

    struct Collmap
    {
        PhysGeomList* geomList;
    };

    struct XModelQuat
    {
        short v[4];
    };

    struct XModel
    {
        const char* name;
        unsigned char numBones;
        unsigned char numRootBones;
        unsigned char numsurfs;
        unsigned char lodRampType;
        ScriptString* boneNames;
        unsigned char* parentList;
        XModelQuat* quats;
        float* trans;
        unsigned char* partClassification;
        DObjAnimMat* baseMat;
        XSurface* surfs;
        Material** materialHandles;
        XModelLodInfo lodInfo[4];
        unsigned char unknown152[8];
        XModelCollSurf_s* collSurfs;
        int numCollSurfs;
        int contents;
        XBoneInfo* boneInfo;
        float radius;
        float mins[3];
        float maxs[3];
        unsigned short numLods;
        short collLod;
        XModelStreamInfo streamInfo;
        int memUsage;
        unsigned int flags;
        PhysPreset* physPreset;
        unsigned char numCollmaps;
        unsigned char pad237[3];
        Collmap* collmaps;
        PhysConstraints* physConstraints;
    };

    enum snd_asset_format
    {
        SND_ASSET_FORMAT_PCMS16 = 0x0,
        SND_ASSET_FORMAT_PCMS24 = 0x1,
        SND_ASSET_FORMAT_PCMS32 = 0x2,
        SND_ASSET_FORMAT_IEEE = 0x3,
        SND_ASSET_FORMAT_XMA4 = 0x4,
        SND_ASSET_FORMAT_MP3 = 0x5,
        SND_ASSET_FORMAT_MSADPCM = 0x6,
        SND_ASSET_FORMAT_WMA = 0x7,
    };

    enum snd_asset_channel
    {
        SND_ASSET_CHANNEL_L = 0x1,
        SND_ASSET_CHANNEL_R = 0x2,
        SND_ASSET_CHANNEL_C = 0x4,
        SND_ASSET_CHANNEL_LFE = 0x8,
        SND_ASSET_CHANNEL_LS = 0x10,
        SND_ASSET_CHANNEL_RS = 0x20,
        SND_ASSET_CHANNEL_LB = 0x40,
        SND_ASSET_CHANNEL_RB = 0x80,
    };

    enum snd_asset_flags
    {
        SND_ASSET_FLAG_DEFAULT = 0x0,
        SND_ASSET_FLAG_LOOPING = 0x1,
        SND_ASSET_FLAG_PAD_LOOP_BUFFER = 0x2,
    };

    typedef tdef_align32(2048) char snd_align_char;

    struct snd_asset
    {
        unsigned int version;
        unsigned int frame_count;
        unsigned int frame_rate;
        unsigned int channel_count;
        unsigned int header_size;
        unsigned int block_size;
        unsigned int buffer_size;
        snd_asset_format format;
        snd_asset_channel channel_flags;
        snd_asset_flags flags;
        unsigned int seek_table_count;
        unsigned int* seek_table;
        unsigned int data_size;
        snd_align_char* data;
    };

    struct LoadedSound
    {
        const char* name;
        snd_asset sound;
    };

    typedef tdef_align32(2048) char char_align_2048;

    struct PrimedSound
    {
        const char* name;
        char_align_2048* buffer;
        unsigned int size;
    };

    struct StreamedSound
    {
        char* filename;
        PrimedSound* primeSnd;
        unsigned char unknown8[16];
    };

    union SoundFileRef
    {
        LoadedSound* loadSnd;
        StreamedSound* streamSnd;
    };

    enum snd_alias_type_t
    {
        SAT_UNKNOWN = 0x0,
        SAT_LOADED = 0x1,
        SAT_STREAMED = 0x2,
        SAT_PRIMED = 0x3,
        SAT_COUNT = 0x4,
    };

    struct SoundFile
    {
        SoundFileRef u;
        char type;
        char exists;
    };

    struct snd_alias_t
    {
        const char* name;
        unsigned int id;
        const char* subtitle;
        const char* secondaryname;
        SoundFile* soundFile;
        unsigned int flags;
        unsigned int duck;
        unsigned int contextType;
        unsigned int contextValue;
        uint16_t fluxTime;
        uint16_t startDelay;
        uint16_t reverbSend;
        uint16_t centerSend;
        uint16_t volMin;
        uint16_t volMax;
        uint16_t teamVolMod;
        uint16_t pitchMin;
        uint16_t pitchMax;
        uint16_t teamPitchMod;
        uint16_t distMin;
        uint16_t distMax;
        uint16_t distReverbMax;
        uint16_t envelopMin;
        uint16_t envelopMax;
        uint16_t envelopPercentage;
        char minPriorityThreshold;
        char maxPriorityThreshold;
        char probability;
        char occlusionLevel;
        char occlusionWetDry;
        char minPriority;
        char maxPriority;
        char pan;
        char dryCurve;
        char wetCurve;
        char dryMinCurve;
        char wetMinCurve;
        char limitCount;
        char entityLimitCount;
        char snapshotGroup;
    };

    struct snd_alias_list_t
    {
        const char* name;
        unsigned int id;
        snd_alias_t* head;
        int count;
        int sequence;
    };

    struct type_align(4) SndIndexEntry
    {
        uint16_t value;
        uint16_t next;
    };

    struct snd_radverb
    {
        char name[32];
        unsigned int id;
        float smoothing;
        float earlyTime;
        float lateTime;
        float earlyGain;
        float lateGain;
        float returnGain;
        float earlyLpf;
        float lateLpf;
        float inputLpf;
        float dampLpf;
        float wallReflect;
        float dryGain;
        float earlySize;
        float lateSize;
        float diffusion;
    };

    struct snd_snapshot
    {
        char name[32];
        unsigned int id;
        char occlusionName[32];
        unsigned int occlusionId;
        float fadeIn;
        float fadeOut;
        float distance;
        unsigned int fadeInCurve;
        unsigned int fadeOutCurve;
        float attenuation[64];
    };

    struct SndBank
    {
        const char* name;
        unsigned int aliasCount;
        snd_alias_list_t* alias;
        SndIndexEntry* aliasIndex;
        unsigned int packHash;
        unsigned int packLocation;
        unsigned int radverbCount;
        snd_radverb* radverbs;
        unsigned int snapshotCount;
        snd_snapshot* snapshots;
    };

    struct SndPatch
    {
        const char* name;
        unsigned int elementCount;
        unsigned int* elements;
        unsigned int fileCount;
        SoundFile* files;
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

    struct cLeaf_s
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

    struct cLeafBrushNodeChildren_t
    {
        float dist;
        float range;
        uint16_t childOffset[2];
    };

    struct cLeafBrushNodeLeaf_t
    {
        uint16_t* brushes;
    };

    union cLeafBrushNodeData_t
    {
        cLeafBrushNodeLeaf_t leaf;
        cLeafBrushNodeChildren_t children;
    };

    struct cLeafBrushNode_s
    {
        char axis;
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
        char triCount;
        char borderCount;
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

    struct type_align(16) CollisionAabbTree
    {
        float origin[3];
        uint16_t materialIndex;
        uint16_t childCount;
        float halfSize[3];
        CollisionAabbTreeIndex u;
    };

    struct cmodel_t
    {
        float mins[3];
        float maxs[3];
        float radius;
        cLeaf_s leaf;
    };

    struct type_align32(16) cbrush_t
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        int axial_cflags[2][3];
        int axial_sflags[2][3];
        unsigned int numverts;
        vec3_t* verts;
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

    struct DynEntityDef
    {
        DynEntityType type;
        GfxPlacement pose;
        XModel* xModel;
        XModel* destroyedxModel;
        uint16_t brushModel;
        uint16_t physicsBrushModel;
        FxEffectDef* destroyFx;
        unsigned int destroySound;
        XModelPieces* destroyPieces;
        PhysPreset* physPreset;
        int16_t physConstraints[4];
        int health;
        int flags;
        int contents;
        uint16_t targetname;
        uint16_t target;
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
        uint16_t burnTime;
        uint16_t fadeTime;
        int physicsStartTime;
    };

    struct DynEntityServer
    {
        uint16_t flags;
        int health;
    };

    struct DynEntityColl
    {
        uint16_t sector;
        uint16_t nextEntInSector;
        float linkMins[3];
        float linkMaxs[3];
        int contents;
    };

    struct par_t
    {
        float p[3];
        float p0[3];
        float p_prev[3];
        int flags;
    };

    enum rope_constraint_e
    {
        ROPE_PAIR_CONSTRAINT = 0x0,
        ROPE_WORLD_CONSTRAINT = 0x1,
        ROPE_DENTITY_CONSTRAINT = 0x2,
        ROPE_CENTITY_CONSTRAINT = 0x3,
    };

    struct constraint_t
    {
        float p[3];
        rope_constraint_e type;
        int entity_index;
        int bone_name_hash;
        char pi1;
        char pi2;
    };

    struct rope_frame_verts_t
    {
        int num_verts;
        float v[50][3];
    };

    struct rope_client_verts_t
    {
        rope_frame_verts_t frame_verts[2];
        unsigned int frame_index;
    };

    struct rope_t
    {
        par_t m_particles[25];
        constraint_t m_constraints[30];
        int m_entity_anchors[3];
        int m_num_particles;
        int m_num_constraints;
        int m_num_entity_anchors;
        int m_num_draw_verts;
        rope_client_verts_t m_client_verts;
        float m_min[3];
        float m_max[3];
        float m_start[3];
        float m_end[3];
        int m_in_use;
        int m_visible;
        int m_dist_constraint;
        int m_flags;
        Material* m_material;
        float m_seglen;
        float m_length;
        float m_width;
        float m_scale;
        float m_force_scale;
        int m_health;
        int m_frame;
        int m_stable_count;
        int m_static_rope;
        uint16_t m_lightingHandle;
    };

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int numEntityChars;
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
        unsigned int numNodes;
        cNode_t* nodes;
        unsigned int numLeafs;
        cLeaf_s* leafs;
        unsigned int leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        unsigned int numLeafBrushes;
        uint16_t* leafbrushes;
        unsigned int numLeafSurfaces;
        unsigned int* leafsurfaces;
        unsigned int vertCount;
        vec3_t* verts;
        unsigned int numBrushVerts;
        vec3_t* brushVerts;
        unsigned int nuinds;
        uint16_t* uinds;
        int triCount;
        uint16_t* triIndices;
        char* triEdgeIsWalkable;
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
        char* visibility;
        int vised;
        MapEnts* mapEnts;
        cbrush_t* box_brush;
        cmodel_t box_model;
        uint16_t originalDynEntCount;
        uint16_t dynEntCount[4];
        DynEntityDef* dynEntDefList[2];
        DynEntityPose* dynEntPoseList[2];
        DynEntityClient* dynEntClientList[2];
        DynEntityServer* dynEntServerList[2];
        DynEntityColl* dynEntCollList[4];
        int num_constraints;
        PhysConstraint* constraints;
        int max_ropes;
        rope_t* ropes;
        unsigned int checksum;
    };

    struct snd_group
    {
        char name[32];
        char parentName[32];
        unsigned int id;
        int parentIndex;
        int category;
        unsigned short attenuationSp;
        unsigned short attenuationMp;
    };

    struct snd_curve
    {
        char name[32];
        unsigned int id;
        float points[8][2];
    };

    struct snd_pan
    {
        char name[32];
        unsigned int id;
        float front;
        float back;
        float center;
        float lfe;
        float left;
        float right;
    };

    struct snd_snapshot_group
    {
        char name[32];
    };

    struct snd_context
    {
        unsigned int type;
        unsigned int valueCount;
        unsigned int values[8];
    };

    struct snd_master
    {
        char name[32];
        unsigned int id;
        float params[35];
    };

    struct SndDriverGlobals
    {
        const char* name;
        unsigned int groupCount;
        snd_group* groups;
        unsigned int curveCount;
        snd_curve* curves;
        unsigned int panCount;
        snd_pan* pans;
        unsigned int snapshotGroupCount;
        snd_snapshot_group* snapshotGroups;
        unsigned int contextCount;
        snd_context* contexts;
        unsigned int masterCount;
        snd_master* masters;
    };

    struct Glyph
    {
        uint16_t letter;
        char x0;
        char y0;
        char dx;
        char pixelWidth;
        char pixelHeight;
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
        char style;
        char border;
        char modal;
        char frameSides;
        float frameTexSize;
        float frameSize;
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
        float rotation;
        Material* background;
    };

    struct ScriptCondition
    {
        bool fireOnTrue;
        int constructID;
        int blockID;
        ScriptCondition* next;
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

    enum expressionRpnEnum
    {
        RPN_CONSTANT = 0x0,
        RPN_CMD_IDX = 0x1,
        RPN_CMD = 0x2,
        RPN_END = 0x3,
    };

    enum expressionOperatorType_e
    {
        OP_NOOP = 0x0,
        OP_RIGHTPAREN = 0x1,
        OP_MUL = 0x2,
        OP_DIV = 0x3,
        OP_MOD = 0x4,
        OP_PLUS = 0x5,
        OP_MINUS = 0x6,
        OP_MINUS_2 = 0x7,
        OP_NEG = 0x8,
        OP_SMALLER = 0x9,
        OP_SMALLEREQ = 0xA,
        OP_GREATER = 0xB,
        OP_GREATEREQ = 0xC,
        OP_EQ = 0xD,
        OP_NOTEQ = 0xE,
        OP_LOGAND = 0xF,
        OP_LOGOR = 0x10,
        OP_LEFTPAREN = 0x11,
        OP_COMMA = 0x12,
        OP_BITAND = 0x13,
        OP_BITOR = 0x14,
        OP_BITNEG = 0x15,
        OP_SHIFTLEFT = 0x16,
        OP_SHIFTRIGHT = 0x17,

        OP_COUNT
    };

    union expressionRpnDataUnion
    {
        Operand constant;
        void* cmd;
        int cmdIdx;
    };

    struct expressionRpn
    {
        int type;
        expressionRpnDataUnion data;
    };

    struct ExpressionStatement
    {
        char* filename;
        int line;
        int numRpn;
        expressionRpn* rpn;
    };

    struct GenericEventScript
    {
        ScriptCondition* prerequisites;
        ExpressionStatement condition;
        int type;
        bool fireOnTrue;
        const char* action;
        int blockID;
        int constructID;
        GenericEventScript* next;
    };

    struct GenericEventHandler
    {
        const char* name;
        GenericEventScript* eventScript;
        GenericEventHandler* next;
    };

    struct ItemKeyHandler
    {
        int key;
        GenericEventScript* keyScript;
        ItemKeyHandler* next;
    };

    struct textExp_s
    {
        ExpressionStatement textExp;
    };

    struct enumDvarDef_s
    {
        const char* enumDvarName;
    };

    struct editFieldDef_s
    {
        int cursorPos[4];
        float minVal;
        float maxVal;
        float defVal;
        float range;
        int maxChars;
        int maxCharsGotoNext;
        int maxPaintChars;
        int paintOffset;
    };

    struct columnInfo_s
    {
        int elementStyle;
        int maxChars;
        rectDef_s rect;
    };

    struct MenuCell
    {
        int type;
        int maxChars;
        char* stringValue;
    };

    struct MenuRow
    {
        MenuCell* cells;
        char* eventName;
        char* onFocusEventName;
        bool disableArg;
        int status;
        int name;
    };

    struct listBoxDef_s
    {
        int cursorPos[4];
        int startPos[4];
        int endPos[4];
        int drawPadding;
        float elementWidth;
        float elementHeight;
        int numColumns;
        float special;
        columnInfo_s columnInfo[16];
        int notselectable;
        int noScrollBars;
        int usePaging;
        float selectBorder[4];
        float disableColor[4];
        float focusColor[4];
        float elementHighlightColor[4];
        float elementBackgroundColor[4];
        Material* selectIcon;
        Material* backgroundItemListbox;
        Material* highlightTexture;
        int noBlinkingHighlight;
        MenuRow* rows;
        int maxRows;
        int rowCount;
    };

    struct multiDef_s
    {
        const char* dvarList[32];
        const char* dvarStr[32];
        float dvarValue[32];
        int count;
        int actionOnEnterPressOnly;
        int strDef;
    };

    union focusDefData_t
    {
        listBoxDef_s* listBox;
        multiDef_s* multi;
        editFieldDef_s* editField;
        enumDvarDef_s* enumDvar;
        void* data;
    };

    struct focusItemDef_s
    {
        ItemKeyHandler* onKey;
        focusDefData_t focusTypeData;
    };

    struct gameMsgDef_s
    {
        int gameMsgWindowIndex;
        int gameMsgWindowMode;
    };

    union textDefData_t
    {
        focusItemDef_s* focusItemDef;
        gameMsgDef_s* gameMsgDef;
        void* data;
    };

    struct textDef_s
    {
        rectDef_s textRect[4];
        int alignment;
        int fontEnum;
        int itemFlags;
        int textAlignMode;
        float textalignx;
        float textaligny;
        float textscale;
        int textStyle;
        const char* text;
        textExp_s* textExpData;
        textDefData_t textTypeData;
    };

    struct imageDef_s
    {
        ExpressionStatement materialExp;
    };

    struct ownerDrawDef_s
    {
        ExpressionStatement dataExp;
    };

    union itemDefData_t
    {
        textDef_s* textDef;
        imageDef_s* imageDef;
        focusItemDef_s* blankButtonDef;
        ownerDrawDef_s* ownerDrawDef;
        void* data;
    };

    struct rectData_s
    {
        ExpressionStatement rectXExp;
        ExpressionStatement rectYExp;
        ExpressionStatement rectWExp;
        ExpressionStatement rectHExp;
    };

    struct animParamsDef_t
    {
        const char* name;
        rectDef_s rectClient;
        float borderSize;
        float foreColor[4];
        float backColor[4];
        float borderColor[4];
        float outlineColor[4];
        float textScale;
        float rotation;
        GenericEventHandler* onEvent;
    };

    struct UIAnimInfo
    {
        int animStateCount;
        animParamsDef_t** animStates;
        animParamsDef_t currentAnimState;
        animParamsDef_t nextAnimState;
        int animating;
        int animStartTime;
        int animDuration;
    };

    enum ItemDefType
    {
        ITEM_TYPE_DEFAULT = 0x0,
        ITEM_TYPE_TEXT = 0x1,
        ITEM_TYPE_IMAGE = 0x2,
        ITEM_TYPE_BUTTON = 0x3,
        ITEM_TYPE_LISTBOX = 0x4,
        ITEM_TYPE_EDITFIELD = 0x5,
        ITEM_TYPE_OWNERDRAW = 0x6,
        ITEM_TYPE_NUMERICFIELD = 0x7,
        ITEM_TYPE_SLIDER = 0x8,
        ITEM_TYPE_YESNO = 0x9,
        ITEM_TYPE_MULTI = 0xA,
        ITEM_TYPE_DVARENUM = 0xB,
        ITEM_TYPE_BIND = 0xC,
        ITEM_TYPE_VALIDFILEFIELD = 0xD,
        ITEM_TYPE_UPREDITFIELD = 0xE,
        ITEM_TYPE_GAME_MESSAGE_WINDOW = 0xF,
        ITEM_TYPE_BIND2 = 0x10,
        ITEM_TYPE_HIGHLIGHT = 0x11,
        ITEM_TYPE_OWNERDRAW_TEXT = 0x12,
        ITEM_TYPE_OD_BUTTON = 0x13,
        ITEM_TYPE_OD_TEXT_BUTTON = 0x14,
        ITEM_TYPE_BUTTON_NO_TEXT = 0x15,
        ITEM_TYPE_ALPHANUMERICFIELD = 0x16,
        ITEM_TYPE_RADIOBUTTON = 0x19,
        ITEM_TYPE_MODEL = 0x1A,
        ITEM_TYPE_CHECKBOX = 0x1B,
        ITEM_TYPE_COMBO = 0x1C,
        ITEM_TYPE_DECIMALFIELD = 0x1E,
        ITEM_TYPE_CONFEDITFIELD = 0x1F,
        ITEM_TYPE_MENUMODEL = 0x27
    };

    struct type_align32(8) itemDef_s
    {
        windowDef_t window;
        int type;
        int dataType;
        int imageTrack;
        const char* dvar;
        const char* dvarTest;
        const char* enableDvar;
        int dvarFlags;
        itemDefData_t typeData;
        menuDef_t* parent;
        rectData_s* rectExpData;
        ExpressionStatement visibleExp;
        gcc_align32(8) uint64_t showBits;
        gcc_align32(8) uint64_t hideBits;
        ExpressionStatement forecolorAExp;
        int ui3dWindowId;
        GenericEventHandler* onEvent;
        UIAnimInfo* animInfo;
    };

    struct type_align32(8) menuDef_t
    {
        windowDef_t window;
        const char* font;
        int fullScreen;
        int ui3dWindowId;
        int itemCount;
        int fontIndex;
        int cursorItem[4];
        int fadeCycle;
        int priority;
        float fadeClamp;
        float fadeAmount;
        float fadeInAmount;
        float blurRadius;
        int openSlideSpeed;
        int closeSlideSpeed;
        int openSlideDirection;
        int closeSlideDirection;
        rectDef_s initialRectInfo;
        int openFadingTime;
        int closeFadingTime;
        int fadeTimeCounter;
        int slideTimeCounter;
        GenericEventHandler* onEvent;
        ItemKeyHandler* onKey;
        ExpressionStatement visibleExp;
        gcc_align32(8) uint64_t showBits;
        gcc_align32(8) uint64_t hideBits;
        const char* allowedBinding;
        const char* soundName;
        int imageTrack;
        int control;
        float focusColor[4];
        float disableColor[4];
        ExpressionStatement rectXExp;
        ExpressionStatement rectYExp;
        itemDef_s** items;
    };

    struct FxIntRange
    {
        int base;
        int amplitude;
    };

    struct FxFloatRange
    {
        float base;
        float amplitude;
    };

    struct FxElemVec3Range
    {
        float base[3];
        float amplitude[3];
    };

    struct FxSpawnDefLooping
    {
        int intervalMsec;
        int count;
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

    struct FxElemAtlas
    {
        char behavior;
        char index;
        char fps;
        char loopCount;
        char colIndexBits;
        char rowIndexBits;
        uint16_t entryCountAndIndexRange;
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
        char color[4];
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
        void* anonymous;
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

    struct FxBillboardTrim
    {
        float topWidth;
        float bottomWidth;
    };

    union FxElemDefUnion
    {
        FxBillboardTrim billboard;
        FxIntRange cloudDensityRange;
    };

    struct FxElemSpawnSound
    {
        const char* spawnSound;
    };

    enum FxElemType : char
    {
        FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
        FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
        FX_ELEM_TYPE_SPRITE_ROTATED = 0x2,
        FX_ELEM_TYPE_TAIL = 0x3,
        FX_ELEM_TYPE_LINE = 0x4,
        FX_ELEM_TYPE_TRAIL = 0x5,
        FX_ELEM_TYPE_CLOUD = 0x6,
        FX_ELEM_TYPE_MODEL = 0x7,
        FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
        FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
        FX_ELEM_TYPE_SOUND = 0xA,
        FX_ELEM_TYPE_DECAL = 0xB,
        FX_ELEM_TYPE_RUNNER = 0xC,
        FX_ELEM_TYPE_COUNT = 0xD,
        FX_ELEM_TYPE_LAST_SPRITE = 0x5,
        FX_ELEM_TYPE_LAST_DRAWN = 0x9,
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
        unsigned int rotationAxis;
        FxFloatRange gravity;
        FxFloatRange reflectionFactor;
        FxElemAtlas atlas;
        float windInfluence;
        char elemType;
        char visualCount;
        char velIntervalCount;
        char visStateIntervalCount;
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
        FxEffectDefRef effectAttached;
        FxTrailDef* trailDef;
        char sortOrder;
        char lightingFrac;
        char unused[2];
        uint16_t alphaFadeTimeMsec;
        uint16_t maxWindStrength;
        uint16_t spawnIntervalAtMaxWind;
        uint16_t lifespanAtMaxWind;
        FxElemDefUnion u;
        FxElemSpawnSound spawnSound;
        float billboardPivot[2];
    };

    struct FxEffectDef
    {
        const char* name;
        char flags;
        char efPriority;
        char reserved[2];
        int totalSize;
        int msecLoopingLife;
        int elemDefCountLooping;
        int elemDefCountOneShot;
        int elemDefCountEmission;
        FxElemDef* elemDefs;
        float boundingBoxDim[3];
        float boundingSphere[4];
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

    struct flameTable
    {
        float flameVar_streamChunkGravityStart;
        float flameVar_streamChunkGravityEnd;
        float flameVar_streamChunkMaxSize;
        float flameVar_streamChunkStartSize;
        float flameVar_streamChunkEndSize;
        float flameVar_streamChunkStartSizeRand;
        float flameVar_streamChunkEndSizeRand;
        float flameVar_streamChunkDistScalar;
        float flameVar_streamChunkDistSwayScale;
        float flameVar_streamChunkDistSwayVelMax;
        float flameVar_streamChunkSpeed;
        float flameVar_streamChunkDecel;
        float flameVar_streamChunkVelocityAddScale;
        float flameVar_streamChunkDuration;
        float flameVar_streamChunkDurationScaleMaxVel;
        float flameVar_streamChunkDurationVelScalar;
        float flameVar_streamChunkSizeSpeedScale;
        float flameVar_streamChunkSizeAgeScale;
        float flameVar_streamChunkSpawnFireIntervalStart;
        float flameVar_streamChunkSpawnFireIntervalEnd;
        float flameVar_streamChunkSpawnFireMinLifeFrac;
        float flameVar_streamChunkSpawnFireMaxLifeFrac;
        float flameVar_streamChunkFireMinLifeFrac;
        float flameVar_streamChunkFireMinLifeFracStart;
        float flameVar_streamChunkFireMinLifeFracEnd;
        float flameVar_streamChunkDripsMinLifeFrac;
        float flameVar_streamChunkDripsMinLifeFracStart;
        float flameVar_streamChunkDripsMinLifeFracEnd;
        float flameVar_streamChunkRotationRange;
        float flameVar_streamSizeRandSinWave;
        float flameVar_streamSizeRandCosWave;
        float flameVar_streamDripsChunkInterval;
        float flameVar_streamDripsChunkMinFrac;
        float flameVar_streamDripsChunkRandFrac;
        float flameVar_streamSmokeChunkInterval;
        float flameVar_streamSmokeChunkMinFrac;
        float flameVar_streamSmokeChunkRandFrac;
        float flameVar_streamChunkCullDistSizeFrac;
        float flameVar_streamChunkCullMinLife;
        float flameVar_streamChunkCullMaxLife;
        float flameVar_streamFuelSizeStart;
        float flameVar_streamFuelSizeEnd;
        float flameVar_streamFuelLength;
        float flameVar_streamFuelNumSegments;
        float flameVar_streamFuelAnimLoopTime;
        float flameVar_streamFlameSizeStart;
        float flameVar_streamFlameSizeEnd;
        float flameVar_streamFlameLength;
        float flameVar_streamFlameNumSegments;
        float flameVar_streamFlameAnimLoopTime;
        float flameVar_streamPrimaryLightRadius;
        float flameVar_streamPrimaryLightRadiusFlutter;
        float flameVar_streamPrimaryLightR;
        float flameVar_streamPrimaryLightG;
        float flameVar_streamPrimaryLightB;
        float flameVar_streamPrimaryLightFlutterR;
        float flameVar_streamPrimaryLightFlutterG;
        float flameVar_streamPrimaryLightFlutterB;
        float flameVar_fireLife;
        float flameVar_fireLifeRand;
        float flameVar_fireSpeedScale;
        float flameVar_fireSpeedScaleRand;
        float flameVar_fireVelocityAddZ;
        float flameVar_fireVelocityAddZRand;
        float flameVar_fireVelocityAddSideways;
        float flameVar_fireGravity;
        float flameVar_fireGravityEnd;
        float flameVar_fireMaxRotVel;
        float flameVar_fireFriction;
        float flameVar_fireEndSizeAdd;
        float flameVar_fireStartSizeScale;
        float flameVar_fireEndSizeScale;
        float flameVar_dripsLife;
        float flameVar_dripsLifeRand;
        float flameVar_dripsSpeedScale;
        float flameVar_dripsSpeedScaleRand;
        float flameVar_dripsVelocityAddZ;
        float flameVar_dripsVelocityAddZRand;
        float flameVar_dripsVelocityAddSideways;
        float flameVar_dripsGravity;
        float flameVar_dripsGravityEnd;
        float flameVar_dripsMaxRotVel;
        float flameVar_dripsFriction;
        float flameVar_dripsEndSizeAdd;
        float flameVar_dripsStartSizeScale;
        float flameVar_dripsEndSizeScale;
        float flameVar_smokeLife;
        float flameVar_smokeLifeRand;
        float flameVar_smokeSpeedScale;
        float flameVar_smokeVelocityAddZ;
        float flameVar_smokeGravity;
        float flameVar_smokeGravityEnd;
        float flameVar_smokeMaxRotation;
        float flameVar_smokeMaxRotVel;
        float flameVar_smokeFriction;
        float flameVar_smokeEndSizeAdd;
        float flameVar_smokeStartSizeAdd;
        float flameVar_smokeOriginSizeOfsZScale;
        float flameVar_smokeOriginOfsZ;
        float flameVar_smokeFadein;
        float flameVar_smokeFadeout;
        float flameVar_smokeMaxAlpha;
        float flameVar_smokeBrightness;
        float flameVar_smokeOriginOffset;
        float flameVar_collisionSpeedScale;
        float flameVar_collisionVolumeScale;
        const char* name;
        Material* fire;
        Material* smoke;
        Material* heat;
        Material* drips;
        Material* streamFuel;
        Material* streamFuel2;
        Material* streamFlame;
        Material* streamFlame2;
        const char* flameOffLoopSound;
        const char* flameIgniteSound;
        const char* flameOnLoopSound;
        const char* flameCooldownSound;
    };

    struct WeaponDef
    {
        const char* szOverlayName;
        XModel** gunXModel;
        XModel* handXModel;
        const char* szModeName;
        uint16_t* notetrackSoundMapKeys;
        uint16_t* notetrackSoundMapValues;
        int playerAnimType;
        int weapType;
        int weapClass;
        int penetrateType;
        int impactType;
        int inventoryType;
        int fireType;
        int clipType;
        int itemIndex;
        const char* parentWeaponName;
        int iJamFireTime;
        int tracerFrequency;
        float tracerWidth;
        float tracerLength;
        int overheatWeapon;
        float overheatRate;
        float cooldownRate;
        float overheatEndVal;
        bool coolWhileFiring;
        bool fuelTankWeapon;
        int iTankLifeTime;
        int offhandClass;
        int offhandSlot;
        int stance;
        FxEffectDef* viewFlashEffect;
        FxEffectDef* worldFlashEffect;
        const char* pickupSound;
        const char* pickupSoundPlayer;
        const char* ammoPickupSound;
        const char* ammoPickupSoundPlayer;
        const char* projectileSound;
        const char* pullbackSound;
        const char* pullbackSoundPlayer;
        const char* fireSound;
        const char* fireSoundPlayer;
        const char* fireLoopSound;
        const char* fireLoopSoundPlayer;
        const char* fireLoopEndSound;
        const char* fireLoopEndSoundPlayer;
        const char* fireStopSound;
        const char* fireStopSoundPlayer;
        const char* fireLastSound;
        const char* fireLastSoundPlayer;
        const char* emptyFireSound;
        const char* emptyFireSoundPlayer;
        const char* crackSound;
        const char* whizbySound;
        const char* meleeSwipeSound;
        const char* meleeSwipeSoundPlayer;
        const char* meleeHitSound;
        const char* meleeMissSound;
        const char* rechamberSound;
        const char* rechamberSoundPlayer;
        const char* reloadSound;
        const char* reloadSoundPlayer;
        const char* reloadEmptySound;
        const char* reloadEmptySoundPlayer;
        const char* reloadStartSound;
        const char* reloadStartSoundPlayer;
        const char* reloadEndSound;
        const char* reloadEndSoundPlayer;
        const char* rotateLoopSound;
        const char* rotateLoopSoundPlayer;
        const char* deploySound;
        const char* deploySoundPlayer;
        const char* finishDeploySound;
        const char* finishDeploySoundPlayer;
        const char* breakdownSound;
        const char* breakdownSoundPlayer;
        const char* finishBreakdownSound;
        const char* finishBreakdownSoundPlayer;
        const char* detonateSound;
        const char* detonateSoundPlayer;
        const char* nightVisionWearSound;
        const char* nightVisionWearSoundPlayer;
        const char* nightVisionRemoveSound;
        const char* nightVisionRemoveSoundPlayer;
        const char* altSwitchSound;
        const char* altSwitchSoundPlayer;
        const char* raiseSound;
        const char* raiseSoundPlayer;
        const char* firstRaiseSound;
        const char* firstRaiseSoundPlayer;
        const char* putawaySound;
        const char* putawaySoundPlayer;
        const char* overheatSound;
        const char* overheatSoundPlayer;
        const char* adsZoomSound;
        const char** bounceSound;
        const char* standMountedWeapdef;
        const char* crouchMountedWeapdef;
        const char* proneMountedWeapdef;
        int standMountedIndex;
        int crouchMountedIndex;
        int proneMountedIndex;
        FxEffectDef* viewShellEjectEffect;
        FxEffectDef* worldShellEjectEffect;
        FxEffectDef* viewLastShotEjectEffect;
        FxEffectDef* worldLastShotEjectEffect;
        Material* reticleCenter;
        Material* reticleSide;
        int iReticleCenterSize;
        int iReticleSideSize;
        int iReticleMinOfs;
        int activeReticleType;
        float vStandMove[3];
        float vStandRot[3];
        float vDuckedOfs[3];
        float vDuckedMove[3];
        float vDuckedSprintOfs[3];
        float vDuckedSprintRot[3];
        float vDuckedSprintBob[2];
        float fDuckedSprintCycleScale;
        float vSprintOfs[3];
        float vSprintRot[3];
        float vSprintBob[2];
        float fSprintCycleScale;
        float vLowReadyOfs[3];
        float vLowReadyRot[3];
        float vDtpOfs[3];
        float vDtpRot[3];
        float vDtpBob[2];
        float fDtpCycleScale;
        float vMantleOfs[3];
        float vMantleRot[3];
        float vSlideOfs[3];
        float vSlideRot[3];
        float vDuckedRot[3];
        float vProneOfs[3];
        float vProneMove[3];
        float vProneRot[3];
        float vStrafeMove[3];
        float vStrafeRot[3];
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
        XModel** worldModel;
        XModel* worldClipModel;
        XModel* rocketModel;
        XModel* mountedModel;
        XModel* additionalMeleeModel;
        Material* hudIcon;
        int hudIconRatio;
        Material* indicatorIcon;
        int indicatorIconRatio;
        Material* ammoCounterIcon;
        int ammoCounterIconRatio;
        int ammoCounterClip;
        int iStartAmmo;
        int iHeatIndex;
        int iMaxAmmo;
        int shotCount;
        const char* szSharedAmmoCapName;
        int iSharedAmmoCapIndex;
        int iSharedAmmoCap;
        bool unlimitedAmmo;
        bool ammoCountClipRelative;
        int damage;
        float damageDuration;
        float damageInterval;
        int playerDamage;
        int iMeleeDamage;
        int iDamageType;
        uint16_t explosionTag;
        int iFireDelay;
        int iMeleeDelay;
        int meleeChargeDelay;
        int iDetonateDelay;
        int iSpinUpTime;
        int iSpinDownTime;
        float spinRate;
        const char* spinLoopSound;
        const char* spinLoopSoundPlayer;
        const char* startSpinSound;
        const char* startSpinSoundPlayer;
        const char* stopSpinSound;
        const char* stopSpinSoundPlayer;
        int iFireTime;
        int iLastFireTime;
        int iRechamberTime;
        int iRechamberBoltTime;
        int iHoldFireTime;
        int iDetonateTime;
        int iMeleeTime;
        int meleeChargeTime;
        int iReloadTimeRight;
        int iReloadTimeLeft;
        int reloadShowRocketTime;
        int iReloadEmptyTimeLeft;
        int iReloadAddTime;
        int iReloadEmptyAddTime;
        int iReloadQuickAddTime;
        int iReloadQuickEmptyAddTime;
        int iReloadStartTime;
        int iReloadStartAddTime;
        int iReloadEndTime;
        int iDropTime;
        int iRaiseTime;
        int iAltDropTime;
        int quickDropTime;
        int quickRaiseTime;
        int iFirstRaiseTime;
        int iEmptyRaiseTime;
        int iEmptyDropTime;
        int sprintInTime;
        int sprintLoopTime;
        int sprintOutTime;
        int lowReadyInTime;
        int lowReadyLoopTime;
        int lowReadyOutTime;
        int contFireInTime;
        int contFireLoopTime;
        int contFireOutTime;
        int dtpInTime;
        int dtpLoopTime;
        int dtpOutTime;
        int slideInTime;
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
        int lockOnRadius;
        int lockOnSpeed;
        bool requireLockonToFire;
        bool noAdsWhenMagEmpty;
        bool avoidDropCleanup;
        unsigned int stackFire;
        float stackFireSpread;
        float stackFireAccuracyDecay;
        const char* stackSound;
        float autoAimRange;
        float aimAssistRange;
        bool mountableWeapon;
        float aimPadding;
        float enemyCrosshairRange;
        bool crosshairColorChange;
        float moveSpeedScale;
        float adsMoveSpeedScale;
        float sprintDurationScale;
        int overlayReticle;
        int overlayInterface;
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
        bool sharedAmmo;
        bool bRifleBullet;
        bool armorPiercing;
        bool bBoltAction;
        bool bUseAltTagFlash;
        bool bUseAntiLagRewind;
        bool bIsCarriedKillstreakWeapon;
        bool aimDownSight;
        bool bRechamberWhileAds;
        bool bReloadWhileAds;
        float adsViewErrorMin;
        float adsViewErrorMax;
        bool bCookOffHold;
        bool bClipOnly;
        bool bCanUseInVehicle;
        bool bNoDropsOrRaises;
        bool adsFireOnly;
        bool cancelAutoHolsterWhenEmpty;
        bool suppressAmmoReserveDisplay;
        bool laserSightDuringNightvision;
        bool bHideThirdPerson;
        bool bHasBayonet;
        bool bDualWield;
        bool bExplodeOnGround;
        bool bThrowBack;
        bool bRetrievable;
        bool bDieOnRespawn;
        bool bNoThirdPersonDropsOrRaises;
        bool bContinuousFire;
        bool bNoPing;
        bool bForceBounce;
        bool bUseDroppedModelAsStowed;
        bool bNoQuickDropWhenEmpty;
        bool bKeepCrosshairWhenADS;
        bool bUseOnlyAltWeaoponHideTagsInAltMode;
        Material* killIcon;
        int killIconRatio;
        bool flipKillIcon;
        bool bNoPartialReload;
        bool bSegmentedReload;
        bool bNoADSAutoReload;
        int iReloadAmmoAdd;
        int iReloadStartAdd;
        const char* szSpawnedGrenadeWeaponName;
        const char* szDualWieldWeaponName;
        unsigned int dualWieldWeaponIndex;
        int iDropAmmoMin;
        int iDropAmmoMax;
        int iDropClipAmmoMin;
        int iDropClipAmmoMax;
        bool blocksProne;
        bool bShowIndicator;
        int isRollingGrenade;
        int iExplosionRadius;
        int iExplosionRadiusMin;
        int iIndicatorRadius;
        int iExplosionInnerDamage;
        int iExplosionOuterDamage;
        float damageConeAngle;
        int iProjectileSpeed;
        int iProjectileSpeedUp;
        int iProjectileSpeedRelativeUp;
        int iProjectileSpeedForward;
        int iProjectileActivateDist;
        float projLifetime;
        float timeToAccelerate;
        float projectileCurvature;
        XModel* projectileModel;
        int projExplosion;
        FxEffectDef* projExplosionEffect;
        bool projExplosionEffectForceNormalUp;
        FxEffectDef* projExplosionEffect2;
        bool projExplosionEffect2ForceNormalUp;
        FxEffectDef* projExplosionEffect3;
        bool projExplosionEffect3ForceNormalUp;
        FxEffectDef* projExplosionEffect4;
        bool projExplosionEffect4ForceNormalUp;
        FxEffectDef* projExplosionEffect5;
        bool projExplosionEffect5ForceNormalUp;
        FxEffectDef* projDudEffect;
        const char* projExplosionSound;
        const char* projDudSound;
        const char* mortarShellSound;
        const char* tankShellSound;
        bool bProjImpactExplode;
        bool bBulletImpactExplode;
        int stickiness;
        int rotateType;
        bool plantable;
        bool hasDetonator;
        bool timedDetonation;
        bool bNoCrumpleMissile;
        bool rotate;
        bool bKeepRolling;
        bool holdButtonToThrow;
        bool offhandHoldIsCancelable;
        bool freezeMovementWhenFiring;
        float lowAmmoWarningThreshold;
        float meleeChargeRange;
        bool bUseAsMelee;
        bool isCameraSensor;
        bool isAcousticSensor;
        float* parallelBounce;
        float* perpendicularBounce;
        FxEffectDef* projTrailEffect;
        float vProjectileColor[3];
        int guidedMissileType;
        float maxSteeringAccel;
        int projIgnitionDelay;
        FxEffectDef* projIgnitionEffect;
        const char* projIgnitionSound;
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
        float fHipViewScatterMin;
        float fHipViewScatterMax;
        float fightDist;
        float maxDist;
        const char* aiVsAiAccuracyGraphName;
        const char* aiVsPlayerAccuracyGraphName;
        vec2_t* aiVsAiAccuracyGraphKnots;
        vec2_t* aiVsPlayerAccuracyGraphKnots;
        vec2_t* originalAiVsAiAccuracyGraphKnots;
        vec2_t* originalAiVsPlayerAccuracyGraphKnots;
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
        int minDamage;
        int minPlayerDamage;
        float fMaxDamageRange;
        float fMinDamageRange;
        float destabilizationRateTime;
        float destabilizationCurvatureMax;
        int destabilizeDistance;
        float* locationDamageMultipliers;
        const char* fireRumble;
        const char* meleeImpactRumble;
        const char* reloadRumble;
        float adsDofStart;
        float adsDofEnd;
        float hipDofStart;
        float hipDofEnd;
        float scanSpeed;
        float scanAccel;
        int scanPauseTime;
        const char* flameTableFirstPerson;
        const char* flameTableThirdPerson;
        flameTable* flameTableFirstPersonPtr;
        flameTable* flameTableThirdPersonPtr;
        FxEffectDef* tagFx_preparationEffect;
        FxEffectDef* tagFlash_preparationEffect;
        bool doGibbing;
        float maxGibDistance;
    };

    struct WeaponVariantDef
    {
        const char* szInternalName;
        int iVariantCount;
        WeaponDef* weapDef;
        const char* szDisplayName;
        const char** szXAnims;
        const char* szAltWeaponName;
        uint16_t* hideTags;
        unsigned int altWeaponIndex;
        int iClipSize;
        int iReloadTime;
        int iReloadEmptyTime;
        int iReloadQuickTime;
        int iReloadQuickEmptyTime;
        int iAdsTransInTime;
        int iAdsTransOutTime;
        int iAltRaiseTime;
        const char* szAmmoName;
        int iAmmoIndex;
        const char* szClipName;
        int iClipIndex;
        float fAimAssistRangeAds;
        float fAdsSwayHorizScale;
        float fAdsSwayVertScale;
        float fAdsViewKickCenterSpeed;
        float fHipViewKickCenterSpeed;
        float fAdsZoomFov1;
        float fAdsZoomFov2;
        float fAdsZoomFov3;
        float fAdsZoomInFrac;
        float fAdsZoomOutFrac;
        float fOverlayAlphaScale;
        float fOOPosAnimLength[2];
        bool bSilenced;
        bool bDualMag;
        bool bFullMetalJacket;
        bool bHollowPoint;
        bool bRapidFire;
        Material* overlayMaterial;
        Material* overlayMaterialLowRes;
        Material* dpadIcon;
        int dpadIconRatio;
        float ikLeftHandOffset[3];
        float ikLeftHandRotation[3];
        float ikLeftHandProneOffset[3];
        float ikLeftHandProneRotation[3];
        float ikLeftHandUiViewerOffset[3];
        float ikLeftHandUiViewerRotation[3];
    };

    struct RawFile
    {
        const char* name;
        int len;
        const char* buffer;
    };

    struct LocalizeEntry
    {
        const char* value;
        const char* name;
    };

    struct StringTableCell
    {
        const char* string;
        int hash;
    };

    struct StringTable
    {
        const char* name;
        int columnCount;
        int rowCount;
        StringTableCell* values;
        short* cellIndex;
    };

    struct PackIndex
    {
        const char* name;
        int packIndex;
        int packType;
    };

    struct XGlobals
    {
        const char* name;
        int xanimStreamBufferSize;
        int cinematicMaxWidth;
        int cinematicMaxHeight;
        int extracamResolution;
        int gumpReserve;
        float screenClearColor[4];
    };

    struct TextureListElement
    {
        unsigned int texture;
    };

    struct TextureList
    {
        int count;
        TextureListElement* elements;
    };

    struct ddlMemberDef_t
    {
        const char* name;
        int size;
        int offset;
        int type;
        int externalIndex;
        unsigned int min;
        unsigned int max;
        unsigned int serverDelta;
        unsigned int clientDelta;
        int arraySize;
        int enumIndex;
        int permission;
    };

    struct ddlStructDef_t
    {
        const char* name;
        int size;
        int memberCount;
        ddlMemberDef_t* members;
    };

    struct ddlEnumDef_t
    {
        const char* name;
        int memberCount;
        const char** members;
    };

    struct ddlDef_t
    {
        int version;
        int size;
        ddlStructDef_t* structList;
        int structCount;
        ddlEnumDef_t* enumList;
        int enumCount;
        ddlDef_t* next;
    };

    struct ddlRoot_t
    {
        const char* name;
        ddlDef_t* ddlDef;
    };

    struct GlassDef
    {
        const char* name;
        int maxHealth;
        float thickness;
        float minShardSize;
        float maxShardSize;
        float shardLifeProbablility;
        int maxShards;
        Material* pristineMaterial;
        Material* crackedMaterial;
        Material* shardMaterial;
        const char* crackSound;
        const char* shatterShound;
        const char* autoShatterShound;
        FxEffectDef* crackEffect;
        FxEffectDef* shatterEffect;
    };

    struct Glass
    {
        GlassDef* glassDef;
        unsigned int index;
        unsigned int brushModel;
        vec3_t origin;
        vec3_t angles;
        vec3_t absmin;
        vec3_t absmax;
        bool isPlanar;
        unsigned char numOutlineVerts;
        vec2_t* outline;
        vec3_t outlineAxis[3];
        vec3_t outlineOrigin;
        float uvScale;
        float thickness;
    };

    struct Glasses
    {
        const char* name;
        unsigned int numGlasses;
        Glass* glasses;
        unsigned char* workMemory;
        unsigned int workMemorySize;
        unsigned int smallAllocatorBlocks;
        unsigned int maxGroups;
        unsigned int maxShards;
        unsigned int maxPhysics;
        unsigned int shardMemorySize;
        unsigned int maxFreeCmd;
        unsigned int numSlots;
        unsigned int numVerts;
        unsigned int numIndices;
    };

    struct EmblemLayer
    {
        int cost;
        int unlockLevel;
        int unlockPLevel;
    };

    struct EmblemCategory
    {
        const char* name;
        const char* description;
    };

    struct EmblemIcon
    {
        GfxImage* image;
        const char* description;
        float outlineSize;
        int defaultColor;
        int cost;
        int unlockLevel;
        int unlockPLevel;
        int unclassifyAt;
        int sortKey;
        unsigned int category;
    };

    struct EmblemBackground
    {
        Material* material;
        const char* description;
        int cost;
        int unlockLevel;
        int unlockPLevel;
        int unclassifyAt;
    };

    struct EmblemSet
    {
        int colorCount;
        int layerCount;
        EmblemLayer* layers;
        int categoryCount;
        EmblemCategory* categories;
        int iconCount;
        EmblemIcon* icons;
        int backgroundCount;
        EmblemBackground* backgrounds;
        int backgroundLookupCount;
        int16_t* backgroundLookup;
    };

    typedef tdef_align32(128) unsigned int raw_uint128;
    typedef tdef_align32(4) char aligned_byte_pointer;

    union GfxTexture
    {
        unsigned char data[52];
    };

    union GfxColor
    {
        unsigned int packed;
        char array[4];
    };

    union PackedUnitVec
    {
        unsigned int packed;
        char array[4];
    };

    union GfxDrawSurf
    {
        uint64_t packed;
        unsigned int words[2];
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

    struct GfxWorldStreamInfo
    {
        int aabbTreeCount;
        GfxStreamingAabbTree* aabbTrees;
        int leafRefCount;
        int* leafRefs;
    };

    struct GfxWorldXModelStreamInfo
    {
        int aabbTreeCount;
        GfxStreamingAabbTree* aabbTrees;
        int smodelIndexCount;
        uint16_t* smodelIndices;
        int streamBufferSize;
    };

    struct GfxWorldSunColor
    {
        unsigned int control;
        float angles[3];
        float ambientColor[4];
        float sunDiffuseColor[4];
        float sunSpecularColor[4];
        float skyColor[4];
        float groundColor[4];
        float exposure;
        float sunShadowSampleSizeNear;
        float skyboxHDRScale;
    };

    struct SunLightParseParams
    {
        char name[64];
        float treeScatterIntensity;
        float treeScatterAmount;
        GfxWorldSunColor sunSettings[1];
    };

    struct type_align(16) float44
    {
        float m[4][4];
    };

    struct type_align32(16) GfxLight
    {
        char type;
        char canUseShadowMap;
        int16_t cullDist;
        float color[3];
        float dir[3];
        float origin[3];
        float radius;
        float cosHalfFovOuter;
        float cosHalfFovInner;
        int exponent;
        unsigned int spotShadowIndex;
        float angles[3];
        float spotShadowHiDistance;
        float diffuseColor[4];
        float specularColor[4];
        float shadowColor[4];
        float falloff[4];
        float attenuation[4];
        float aAbB[4];
        float cookieControl0[4];
        float cookieControl1[4];
        float cookieControl2[4];
        float44 viewMatrix;
        float44 projMatrix;
        GfxLightDef* def;
    };

    struct GfxLightCorona
    {
        float origin[3];
        float radius;
        float color[3];
        float intensity;
    };

    struct GfxShadowMapVolume
    {
        unsigned int control;
        unsigned int padding1;
        unsigned int padding2;
        unsigned int padding3;
    };

    struct GfxVolumePlane
    {
        float plane[4];
    };

    struct GfxExposureVolume
    {
        unsigned int control;
        float exposure;
        float luminanceIncreaseScale;
        float luminanceDecreaseScale;
        float featherRange;
        float featherAdjust;
    };

    struct GfxSkyDynamicIntensity
    {
        float angle0;
        float angle1;
        float factor0;
        float factor1;
    };

    struct GfxWorldDpvsPlanes
    {
        int cellCount;
        cplane_s* planes;
        uint16_t* nodes;
        unsigned int* sceneEntCellBits;
    };

    struct GfxCell;
    struct GfxPortal;

    struct GfxPortalWritable
    {
        bool isQueued;
        bool isAncestor;
        char recursionDepth;
        char hullPointCount;
        float (*hullPoints)[2];
        GfxPortal* queuedParent;
    };

    struct DpvsPlane
    {
        float coeffs[4];
        char side[3];
        char pad;
    };

    struct GfxPortal
    {
        GfxPortalWritable writable;
        DpvsPlane plane;
        GfxCell* cell;
        float (*vertices)[3];
        char vertexCount;
        float hullAxis[2][3];
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
        char reflectionProbeCount;
        char* reflectionProbes;
    };

    struct GfxReflectionProbeVolumeData
    {
        float volumePlanes[6][4];
    };

    struct GfxReflectionProbe
    {
        float origin[3];
        GfxImage* reflectionImage;
        GfxReflectionProbeVolumeData* probeVolumes;
        unsigned int probeVolumeCount;
    };

    struct GfxLightmapArray
    {
        GfxImage* primary;
        GfxImage* secondary;
        GfxImage* secondaryB;
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

    struct GfxWorldVertexData
    {
        GfxWorldVertex* vertices;
        D3DVertexBuffer worldVb;
    };

    struct GfxWorldVertexLayerData
    {
        char* data;
        D3DVertexBuffer layerVb;
    };

    struct GfxWorldDraw
    {
        unsigned int reflectionProbeCount;
        GfxReflectionProbe* reflectionProbes;
        GfxTexture* reflectionProbeTextures;
        int lightmapCount;
        GfxLightmapArray* lightmaps;
        GfxTexture* lightmapPrimaryTextures;
        GfxTexture* lightmapSecondaryTextures;
        GfxTexture* lightmapSecondaryTexturesB;
        GfxImage* terrainScorchImages[31];
        unsigned int vertexCount;
        GfxWorldVertexData vd;
        unsigned int vertexLayerDataSize;
        GfxWorldVertexLayerData vld;
        unsigned int vertexStream2DataSize;
        int indexCount;
        uint16_t* indices;
        D3DIndexBuffer indexBuffer;
    };

    struct type_align(4) GfxLightGridEntry
    {
        uint16_t colorsIndex;
        char primaryLightIndex;
        char needsTrace;
    };

    struct type_align(4) GfxCompressedLightGridColors
    {
        char rgb[56][3];
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
        aligned_byte_pointer* rawRowData;
        unsigned int entryCount;
        GfxLightGridEntry* entries;
        unsigned int colorCount;
        GfxCompressedLightGridColors* colors;
    };

    struct GfxBrushModelWritable
    {
        float mins[3];
        float maxs[3];
        float mip1radiusSq;
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

    typedef tdef_align32(4) GfxSceneDynModel GfxSceneDynModel4;

    struct BModelDrawInfo
    {
        uint16_t surfId;
    };

    struct type_align(4) GfxSceneDynBrush
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
        float lightingOrigin[3];
        GfxColor groundLighting;
    };

    struct srfTriangles_t
    {
        float mins[3];
        int vertexLayerData;
        float maxs[3];
        int firstVertex;
        uint16_t vertexCount;
        uint16_t triCount;
        int baseIndex;
        float himipRadiusSq;
        int stream2ByteOffset;
    };

    struct type_align32(16) GfxSurface
    {
        srfTriangles_t tris;
        Material* material;
        char lightmapIndex;
        char reflectionProbeIndex;
        char primaryLightIndex;
        char flags;
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

    struct GfxStaticModelDrawInst
    {
        float cullDist;
        GfxPackedPlacement placement;
        XModel* model;
        int flags;
        uint16_t lightingHandle;
        unsigned char reflectionProbeIndex;
        unsigned char primaryLightIndex;
    };

    struct GfxWorldDpvsStatic
    {
        unsigned int smodelCount;
        unsigned int dynamicSModelCount;
        unsigned int staticSurfaceCount;
        unsigned int litSurfsBegin;
        unsigned int litSurfsEnd;
        unsigned int decalSurfsBegin;
        unsigned int decalSurfsEnd;
        unsigned int emissiveSurfsBegin;
        unsigned int emissiveSurfsEnd;
        unsigned int smodelVisDataCount;
        unsigned int surfaceVisDataCount;
        raw_uint128* smodelVisData[3];
        raw_uint128* surfaceVisData[3];
        raw_uint128* smodelVisDataCameraSaved;
        raw_uint128* surfaceVisDataCameraSaved;
        raw_uint128* lodData;
        uint16_t* sortedSurfIndex;
        GfxStaticModelInst* smodelInsts;
        GfxSurface* surfaces;
        GfxCullGroup* cullGroups;
        GfxStaticModelDrawInst* smodelDrawInsts;
        GfxDrawSurf* surfaceMaterials;
        raw_uint128* surfaceCastsSunShadow;
        volatile int usageCount;
    };

    struct GfxWorldDpvsDynamic
    {
        unsigned int dynEntClientWordCount[2];
        unsigned int dynEntClientCount[2];
        unsigned int* dynEntCellBits[2];
        char* dynEntVisData[2][3];
    };

    struct GfxWorldLodChain
    {
        float origin[3];
        float lastDist;
        unsigned int firstLodInfo;
        uint16_t lodInfoCount;
    };

    struct GfxWorldLodInfo
    {
        float dist;
        unsigned int firstSurf;
        uint16_t surfCount;
    };

    struct GfxWaterBuffer
    {
        unsigned int bufferSize;
        float (*buffer)[4];
    };

    struct Occluder
    {
        unsigned int flags;
        char name[16];
        float points[4][3];
    };

    struct GfxOutdoorBounds
    {
        float bounds[2][3];
    };

    struct GfxHeroLight
    {
        char type;
        char unused[3];
        float color[3];
        float dir[3];
        float origin[3];
        float radius;
        float cosHalfFovOuter;
        float cosHalfFovInner;
        int exponent;
    };

    struct GfxHeroLightTree
    {
        float mins[3];
        float maxs[3];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        int planeCount;
        int nodeCount;
        int surfaceCount;
        GfxWorldStreamInfo streamInfo;
        GfxWorldXModelStreamInfo xmodelStreamInfo;
        int skySurfCount;
        int* skyStartSurfs;
        GfxImage* skyImage;
        char skySamplerState;
        const char* skyBoxModel;
        SunLightParseParams sunParse;
        GfxLight* sunLight;
        float sunColorFromBsp[3];
        unsigned int sunPrimaryLightIndex;
        unsigned int primaryLightCount;
        int cullGroupCount;
        unsigned int coronaCount;
        GfxLightCorona* coronas;
        unsigned int shadowMapVolumeCount;
        GfxShadowMapVolume* shadowMapVolumes;
        unsigned int shadowMapVolumePlaneCount;
        GfxVolumePlane* shadowMapVolumePlanes;
        unsigned int exposureVolumeCount;
        GfxExposureVolume* exposureVolumes;
        unsigned int exposureVolumePlaneCount;
        GfxVolumePlane* exposureVolumePlanes;
        GfxSkyDynamicIntensity skyDynIntensity;
        GfxWorldDpvsPlanes dpvsPlanes;
        int cellBitsCount;
        GfxCell* cells;
        GfxWorldDraw draw;
        GfxLightGrid lightGrid;
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
        char* nonSunPrimaryLightForModelDynEnt;
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
        Material* coronaMaterial;
        Material* ropeMaterial;
        unsigned int numOccluders;
        Occluder* occluders;
        unsigned int numOutdoorBounds;
        GfxOutdoorBounds* outdoorBounds;
        unsigned int heroLightCount;
        unsigned int heroLightTreeCount;
        GfxHeroLight* heroLights;
        GfxHeroLightTree* heroLightTree;
    };

    union XAssetHeader
    {
        PhysPreset* physPreset;
        PhysConstraints* physConstraints;
        DestructibleDef* destructibleDef;
        XAnimParts* parts;
        XModel* model;
        Material* material;
        MaterialPixelShader* pixelShader;
        MaterialTechniqueSet* techniqueSet;
        GfxImage* image;
        SndBank* sound;
        SndPatch* soundPatch;
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
        WeaponVariantDef* weapon;
        SndDriverGlobals* sndDriverGlobals;
        FxEffectDef* fx;
        FxImpactTable* impactFx;
        RawFile* rawfile;
        StringTable* stringTable;
        PackIndex* packIndex;
        XGlobals* xGlobals;
        TextureList* textureList;
        ddlRoot_t* ddlRoot;
        Glasses* glasses;
        EmblemSet* emblemSet;
        void* data;
    };

#ifndef __zonecodegenerator
} // namespace T5Xenon
#endif

#endif
