#pragma once

#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace QOSXenon
{
#endif
    enum XFileBlock
    {
        XFILE_BLOCK_TEMP = 0x0,
        XFILE_BLOCK_RUNTIME = 0x1,
        XFILE_BLOCK_VIRTUAL = 0x2,
        XFILE_BLOCK_LARGE = 0x3,
        XFILE_BLOCK_PHYSICAL = 0x4,

        MAX_XFILE_COUNT
    };

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
        ASSET_TYPE_SND_CURVE = 0xB,
        ASSET_TYPE_CLIPMAP_SP = 0xC,
        ASSET_TYPE_CLIPMAP_MP = 0xD,
        ASSET_TYPE_COMWORLD = 0xE,
        ASSET_TYPE_GAMEWORLD_SP = 0xF,
        ASSET_TYPE_GAMEWORLD_MP = 0x10,
        ASSET_TYPE_MAP_ENTS = 0x11,
        ASSET_TYPE_GFXWORLD = 0x12,
        ASSET_TYPE_LIGHT_DEF = 0x13,
        ASSET_TYPE_UI_MAP = 0x14,
        ASSET_TYPE_FONT = 0x15,
        ASSET_TYPE_MENUFILE = 0x16,
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
        ASSET_TYPE_XMLTREE = 0x23,
        ASSET_TYPE_SCENE_ANIMATION = 0x24,
        ASSET_TYPE_CUTSCENE = 0x25,
        ASSET_TYPE_CUSTOM_CAMERA = 0x26,

        ASSET_TYPE_COUNT = 0x27,
        ASSET_TYPE_STRING = 0x27,
        ASSET_TYPE_ASSETLIST = 0x28,
    };

    typedef unsigned short ScriptString;

    struct XModelPieces;
    struct PhysPreset;
    struct PhysConstraints;
    struct DestructibleDef;
    struct XAnimParts;
    struct XModel;
    struct Material;
    struct MaterialPixelShader;
    struct MaterialTechniqueSet;
    struct GfxImage;
    struct snd_alias_list_t;
    struct SndCurve;
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
    struct AiType;
    struct MpType;
    struct Character;
    struct XModelAlias;
    struct RawFile;
    struct StringTable;
    struct XmlTree;
    struct SceneAnimation;
    struct Cutscene;
    struct CustomCamera;

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
        MaterialTechniqueSet* techniqueSet;
        GfxImage* image;
        snd_alias_list_t* sound;
        SndCurve* sndCurve;
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
        AiType* aiType;
        MpType* mpType;
        Character* character;
        XModelAlias* xmodelAlias;
        RawFile* rawfile;
        StringTable* stringTable;
        XmlTree* xmlTree;
        SceneAnimation* sceneAnimation;
        Cutscene* cutscene;
        CustomCamera* customCamera;
        void* data;
    };

    union XAnimIndices
    {
        unsigned char* _1;
        unsigned short* _2;
        void* data;
    };

    struct XAnimNotifyInfo
    {
        ScriptString name;
        float time;
    };

    typedef unsigned char ByteVec[3];
    typedef tdef_align32(4) unsigned short UShortVec[3];

    union XAnimDynamicIndicesTrans
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    union XAnimDynamicFrames
    {
        ByteVec* _1;
        UShortVec* _2;
    };

    struct XAnimPartTransFrames
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

    struct type_align(4) XQuat2
    {
        short value[2];
    };

    union XAnimDynamicIndicesQuat
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    struct XAnimDeltaPartQuatDataFrames
    {
        XQuat2* frames;
        XAnimDynamicIndicesQuat indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatDataFrames frames;
        XQuat2 frame0;
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

    enum XAnimPartType
    {
        PART_TYPE_NO_QUAT = 0x0,
        PART_TYPE_HALF_QUAT = 0x1,
        PART_TYPE_FULL_QUAT = 0x2,
        PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
        PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
        PART_TYPE_SMALL_TRANS = 0x5,
        PART_TYPE_TRANS = 0x6,
        PART_TYPE_TRANS_NO_SIZE = 0x7,
        PART_TYPE_NO_TRANS = 0x8,
        PART_TYPE_ALL = 0x9,

        PART_TYPE_COUNT
    };

    struct XAnimParts
    {
        const char* name;
        const char* unknownString;
        unsigned short dataByteCount;
        unsigned short dataShortCount;
        unsigned short dataIntCount;
        unsigned short randomDataByteCount;
        unsigned short randomDataIntCount;
        unsigned short numframes;
        bool bLoop;
        bool bDelta;
        unsigned char boneCount[PART_TYPE_COUNT];
        unsigned char notifyCount;
        unsigned char assetType;
        bool isDefault;
        unsigned char pad;
        unsigned int randomDataShortCount;
        unsigned int indexCount;
        float framerate;
        float frequency;
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

    struct PhysPreset
    {
        const char* name;
        unsigned char unknown4[24];
        const char* unknownString;
        unsigned char unknown32[16];
    };

    struct PhysConstraint
    {
        unsigned char unknown0[4];
        const char* target_bone1;
        const char* target_bone2;
        unsigned char unknown12[56];
    };

    struct PhysConstraints
    {
        const char* name;
        unsigned int count;
        PhysConstraint data[16];
    };

    struct XModelQuat
    {
        short v[4];
    };

    struct DObjAnimMat
    {
        float quat[4];
        float trans[3];
        float transWeight;
    };

    struct GfxPackedVertex16
    {
        unsigned char data[16];
    };

    struct D3DVertexBuffer
    {
        unsigned char data[32];
    };

    struct D3DIndexBuffer
    {
        unsigned char data[32];
    };

    struct XSurfaceVertexInfo
    {
        short vertCount[4];
        unsigned short* vertsBlend;
        float* tensionData;
    };

    struct XRigidVertList
    {
        unsigned short boneOffset;
        unsigned short vertCount;
        unsigned short triOffset;
        unsigned short triCount;
    };

    struct XSurface
    {
        unsigned char tileMode;
        bool deformed;
        unsigned short vertCount;
        unsigned short triCount;
        unsigned short unknown6;
        unsigned short* triIndices;
        XSurfaceVertexInfo vertInfo;
        GfxPackedVertex16* verts0;
        D3DVertexBuffer vb0;
        GfxPackedVertex16* verts1;
        D3DVertexBuffer vb1;
        GfxPackedVertex16* verts2;
        D3DVertexBuffer vb2;
        unsigned int vertListCount;
        XRigidVertList* vertList;
        D3DIndexBuffer indexBuffer;
        int partBits[6];
    };

    struct XModelLodInfo
    {
        float dist;
        unsigned short numsurfs;
        unsigned short surfIndex;
        int partBits[5];
        unsigned char lod;
        unsigned char smcIndexPlusOne;
        unsigned char smcAllocBits;
        unsigned char unused;
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

    struct XModelStreamInfo
    {
        XModelHighMipBounds* highMipBounds;
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
        unsigned int materialNum;
        short firstAdjacentSideOffset;
        unsigned char edgeCount;
        unsigned char pad;
    };

    struct BrushWrapper
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        short axialMaterialNum[2][3];
        unsigned char* baseAdjacentSide;
        short firstAdjacentSideOffsets[2][3];
        unsigned char edgeCount[2][3];
        unsigned short pad70;
        unsigned int numverts;
        float (*verts)[3];
        int totalEdgeCount;
        cplane_s* planes;
        unsigned char unknown88[8];
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
        float mass[3];
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
        unsigned char flags;
        bool bad;
        unsigned char pad226[2];
        PhysPreset* physPreset;
        PhysGeomList* physGeoms;
        PhysConstraints* physConstraints;
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

    struct SndCurve
    {
        const char* name;
        int knotCount;
        float knots[8][2];
    };

    enum QosSoundFileType
    {
        QOS_SOUND_FILE_TYPE_PRIMED = 3,
    };

    struct QosMssSoundSeekTable
    {
        int size;
        unsigned int* data;
    };

    struct QosMssSound
    {
        unsigned char* data;
        unsigned int dataSize;
        unsigned char unknown0[76];
        unsigned char* unknownData;
        unsigned char unknown1[60];
        QosMssSoundSeekTable seekTable;
    };

    struct QosPrimedSound
    {
        const char* name;
        unsigned char* buffer;
        unsigned int size;
    };

    union QosSoundFileRef
    {
        QosMssSound* mssSound;
        QosPrimedSound* primeSnd;
    };

    struct SoundFile
    {
        const char* unknownString0;
        const char* unknownString1;
        QosSoundFileRef u;
        int unknown0;
        QosSoundFileType type;
    };

    struct XAUDIOCHANNELMAPENTRY
    {
        unsigned char inputChannel;
        unsigned char outputChannel;
        float volume;
    };

    struct XAUDIOCHANNELMAP
    {
        unsigned char entryCount;
        XAUDIOCHANNELMAPENTRY* entries;
    };

    struct SpeakerMap
    {
        bool isDefault;
        const char* name;
        XAUDIOCHANNELMAP channelMaps[3][2];
    };

    struct snd_alias_t
    {
        const char* aliasName;
        const char* subtitle;
        const char* secondaryAliasName;
        const char* chainAliasName;
        SoundFile* soundFile;
        unsigned char unknown0[56];
        SndCurve* volumeFalloffCurve;
        unsigned char unknown1[12];
        SpeakerMap* speakerMap;
    };

    struct snd_alias_list_t
    {
        const char* name;
        snd_alias_t* head;
        int count;
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

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int numEntityChars;
    };

    struct cStaticModel_t
    {
        unsigned char unknown0[4];
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
        short children[2];
    };

    struct cLeaf_t
    {
        unsigned short firstCollAabbIndex;
        unsigned short collAabbCount;
        int brushContents;
        int terrainContents;
        float mins[3];
        float maxs[3];
        int leafBrushNode;
        short cluster;
    };

    struct cLeafBrushNodeLeaf_t
    {
        unsigned short* brushes;
    };

    struct cLeafBrushNodeChildren_t
    {
        float dist;
        float range;
        unsigned short childOffset[2];
    };

    union cLeafBrushNodeData_t
    {
        cLeafBrushNodeLeaf_t leaf;
        cLeafBrushNodeChildren_t children;
    };

    struct cLeafBrushNode_s
    {
        char axis;
        short leafBrushCount;
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
        unsigned char triCount;
        unsigned char borderCount;
        unsigned char unknown0[2];
        int firstTri;
        int unknown1;
        int unknown2;
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
        unsigned short materialIndex;
        unsigned short childCount;
        float halfSize[3];
        CollisionAabbTreeIndex u;
    };

    struct cmodel_t
    {
        float mins[3];
        float maxs[3];
        float radius;
        cLeaf_t leaf;
    };

    struct type_align32(16) cbrush_t
    {
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t* sides;
        short axialMaterialNum[2][3];
        unsigned char* baseAdjacentSide;
        short firstAdjacentSideOffsets[2][3];
        unsigned char edgeCount[2][3];
        unsigned char unknown0[2];
        unsigned int unknown1;
        float (*verts)[3];
    };

    struct DynEntityDef
    {
        unsigned char unknown0[32];
        XModel* xModel;
        unsigned char unknown1[4];
        FxEffectDef* destroyFx;
        XModelPieces* destroyPieces;
        PhysPreset* physPreset;
        unsigned char unknown2[36];
    };

    struct DynEntityClient
    {
        unsigned char unknown[52];
    };

    struct DynEntityServer
    {
        unsigned char unknown[36];
    };

    struct DynEntityColl
    {
        unsigned char unknown[32];
    };

    struct DynEntityPhysConstraint
    {
        unsigned char unknown[72];
    };

    struct clipMap_t
    {
        const char* name;
        int isInUse;
        int planeCount;
        cplane_s* planes;
        unsigned int numStaticModels;
        cStaticModel_t* staticModelList;
        unsigned int numMaterials;
        dmaterial_t* materials;
        unsigned int numBrushSides;
        cbrushside_t* brushsides;
        unsigned int numBrushEdges;
        unsigned char* brushEdges;
        unsigned int numNodes;
        cNode_t* nodes;
        unsigned int numLeafs;
        cLeaf_t* leafs;
        unsigned int leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        unsigned int numLeafBrushes;
        unsigned short* leafbrushes;
        unsigned int numLeafSurfaces;
        unsigned int* leafsurfaces;
        unsigned int vertCount;
        float (*verts)[3];
        unsigned int numBrushVerts;
        float (*brushVerts)[3];
        unsigned int nuinds;
        unsigned short* uinds;
        int triCount;
        unsigned short* triIndices;
        unsigned char* triEdgeIsWalkable;
        int borderCount;
        CollisionBorder* borders;
        int partitionCount;
        CollisionPartition* partitions;
        int aabbTreeCount;
        CollisionAabbTree* aabbTrees;
        unsigned int numSubModels;
        cmodel_t* cmodels;
        unsigned short numBrushes;
        unsigned short unknown0;
        cbrush_t* brushes;
        int numClusters;
        int clusterBytes;
        unsigned char* visibility;
        int unknown2;
        MapEnts* mapEnts;
        cbrush_t* box_brush;
        unsigned char unknown1[74];
        unsigned short dynEntCount[4];
        DynEntityDef* dynEntDefList[2];
        DynEntityClient* dynEntClientList[2];
        DynEntityServer* dynEntServerList[2];
        DynEntityColl* dynEntCollList[4];
        int num_constraints;
        DynEntityPhysConstraint* constraints;
        int unknown3;
    };

    struct ComPrimaryLight
    {
        unsigned char type;
        unsigned char canUseShadowMap;
        unsigned char exponent;
        unsigned char unused;
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

    struct ComWorldUnknownCell
    {
        unsigned char unknown[32];
        unsigned char* data;
    };

    struct ComWorld
    {
        const char* name;
        int isInUse;
        int unknown0;
        unsigned int primaryLightCount;
        ComPrimaryLight* primaryLights;
        unsigned char unknown1[16];
        unsigned int unknownCellCount;
        ComWorldUnknownCell* unknownCells;
    };

    struct pathnode_t;
    struct pathnode_tree_t;

    struct pathnode_tree_nodes_t
    {
        int nodeCount;
        pathnode_t** nodes;
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

    struct type_align(16) pathbasenode_t
    {
        float vOrigin[3];
        unsigned int type;
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
        int unknown0;
    };

    struct pathnode_dynamic_t
    {
        void* pOwner;
        int iFreeTime;
        int iValidTime[3];
        int inPlayerLOSTime;
        short wLinkCount;
        short wOverlapCount;
        short turretEntNumber;
        short userCount;
    };

    struct pathlink_s
    {
        float fDist;
        unsigned short nodeNum;
        unsigned char disconnectCount;
        unsigned char negotiationLink;
        unsigned char ubBadPlaceCount[4];
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
        unsigned short spawnflags;
        ScriptString targetname;
        ScriptString script_linkName;
        ScriptString script_noteworthy;
        ScriptString target;
        ScriptString animscript;
        int animscriptfunc;
        float vOrigin[3];
        float fAngle;
        float forward[2];
        float fRadius;
        float minUseDistSq;
        short wOverlapNode[2];
        short wChainId;
        short wChainDepth;
        short wChainParent;
        unsigned short totalLinkCount;
        pathlink_s* Links;
    };

    struct pathnode_t
    {
        pathnode_constant_t constant;
        pathnode_dynamic_t dynamic;
        pathnode_transient_t transient;
    };

    struct PathData
    {
        unsigned int nodeCount;
        pathnode_t* nodes;
        pathbasenode_t* basenodes;
        unsigned short* chainNodeForNode;
        unsigned short* nodeForChainNode;
        int visBytes;
        unsigned char* pathVis;
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

    union GfxColor
    {
        unsigned int packed;
        unsigned char array[4];
    };

    union PackedUnitVec
    {
        unsigned int packed;
        unsigned char array[4];
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

    struct GfxStreamingAabbTree
    {
        unsigned short firstItem;
        unsigned short itemCount;
        unsigned short firstChild;
        unsigned short childCount;
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

    struct GfxWorldVertexData
    {
        GfxWorldVertex* vertices;
        D3DVertexBuffer worldVb;
    };

    struct GfxWorldVertexLayerData
    {
        unsigned char* data;
        D3DVertexBuffer layerVb;
    };

    struct GfxLight
    {
        unsigned char type;
        unsigned char canUseShadowMap;
        unsigned char unused[2];
        float color[3];
        float dir[3];
        float origin[3];
        float radius;
        float cosHalfFovOuter;
        float cosHalfFovInner;
        int exponent;
        unsigned int spotShadowIndex;
        unsigned int unknown60;
        GfxLightDef* def;
    };

    struct GfxReflectionProbe
    {
        float origin[3];
        GfxImage* reflectionImage;
    };

    struct srfTriangles_t
    {
        int vertexLayerData;
        int firstVertex;
        unsigned short vertexCount;
        unsigned short triCount;
        int baseIndex;
        float topMipMins[3];
        float topMipMaxs[3];
    };

    struct GfxSurface
    {
        srfTriangles_t tris;
        Material* material;
        unsigned char lightmapIndex;
        unsigned char reflectionProbeIndex;
        unsigned char primaryLightIndex;
        unsigned char flags;
        float bounds[2][3];
    };

    struct GfxCullGroup
    {
        float mins[3];
        float maxs[3];
        int surfaceCount;
        int startSurfIndex;
    };

    struct type_align(4) GfxStaticModelDrawInst
    {
        unsigned char unknown0[32];
        XModel* model;
        unsigned int unknown36;
    };

    struct GfxStaticModelInst
    {
        float mins[3];
        float maxs[3];
        GfxColor groundLighting;
        unsigned int unknown28;
    };

    struct GfxAabbTreeChildren;

    struct GfxAabbTree
    {
        unsigned char unknown0[32];
        unsigned int smodelIndexCount;
        unsigned int* smodelIndexes;
        unsigned int childCount;
        GfxAabbTreeChildren* children;
    };

    struct GfxAabbTreeChildren
    {
        GfxAabbTree child;
    };

    struct GfxCell;

    struct GfxPortal
    {
        unsigned char unknown0[32];
        GfxCell* cell;
        float (*vertices)[3];
        unsigned char vertexCount;
        unsigned char unknown41[27];
    };

    struct GfxCell
    {
        float mins[3];
        float maxs[3];
        GfxAabbTree* aabbTree;
        int portalCount;
        GfxPortal* portals;
        int cullGroupCount;
        int* cullGroups;
        unsigned char reflectionProbeCount;
        unsigned char pad45[3];
        unsigned char* reflectionProbes;
    };

    struct GfxLightmapArray
    {
        GfxImage* primary;
        GfxImage* secondary;
    };

    struct GfxLightGridEntry
    {
        unsigned short colorsIndex;
        unsigned char primaryLightIndex;
        unsigned char needsTrace;
    };

    struct GfxLightGridColors
    {
        unsigned char rgb[56][3];
    };

    struct GfxLightGrid
    {
        unsigned char hasLightRegions;
        unsigned char pad1[3];
        unsigned short mins[3];
        unsigned short maxs[3];
        unsigned int rowAxis;
        unsigned int colAxis;
        unsigned short* rowDataStart;
        unsigned int rawRowDataSize;
        unsigned char* rawRowData;
        unsigned int entryCount;
        GfxLightGridEntry* entries;
        unsigned int colorCount;
        GfxLightGridColors* colors;
    };

    struct GfxBrushModel
    {
        unsigned char unknown0[48];
        unsigned int surfaceCount;
        unsigned int startSurfIndex;
        unsigned int unknown56;
    };

    struct MaterialMemory
    {
        Material* material;
        int memory;
    };

    struct sunflare_t
    {
        unsigned char hasValidData;
        unsigned char pad1[3];
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

    struct GfxDrawSurf
    {
        unsigned int packed[2];
    };

    struct GfxSceneDynModel
    {
        unsigned char unknown0[8];
    };

    struct GfxSceneDynBrush
    {
        unsigned char unknown0[8];
    };

    struct GfxDynEntCellRef
    {
        unsigned short dynEntIndex;
        unsigned short cellIndex;
        unsigned short nextIndex;
    };

    struct GfxShadowGeometry
    {
        unsigned short surfaceCount;
        unsigned short smodelCount;
        unsigned short* sortedSurfIndex;
        unsigned short* smodelIndex;
    };

    struct GfxLightRegion
    {
        unsigned char unknown0[24];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        unsigned int planeCount;
        cplane_s* planes;
        unsigned int nodeCount;
        unsigned short* nodes;
        unsigned int indexCount;
        unsigned short* indices;
        D3DIndexBuffer indexBuffer;
        unsigned int surfaceCount;
        GfxSurface* surfaces;
        GfxWorldStreamInfo streamInfo;
        unsigned char unknown88[16];
        unsigned int skySurfCount;
        int* skyStartSurfs;
        GfxImage* skyImage;
        unsigned char skySamplerState;
        unsigned char pad117[3];
        const char* skyBoxModel;
        unsigned int vertexCount;
        GfxWorldVertexData vd;
        unsigned int vertexLayerDataSize;
        GfxWorldVertexLayerData vld;
        unsigned char unknown204[128];
        GfxLight* sunLight;
        float sunColorFromBsp[3];
        unsigned int sunPrimaryLightIndex;
        unsigned int primaryLightCount;
        unsigned int unknown356;
        unsigned int cullGroupCount;
        unsigned int reflectionProbeCount;
        GfxReflectionProbe* reflectionProbes;
        GfxCullGroup* cullGroups;
        unsigned int smodelCount;
        GfxStaticModelDrawInst* smodelDrawInsts;
        GfxStaticModelInst* smodelInsts;
        unsigned int cellCount;
        unsigned int cellBitsCount;
        GfxCell* cells;
        unsigned int lightmapCount;
        GfxLightmapArray* lightmaps;
        GfxLightGrid lightGrid;
        unsigned int modelCount;
        GfxBrushModel* models;
        unsigned char unknown468[28];
        unsigned int materialMemoryCount;
        MaterialMemory* materialMemory;
        sunflare_t sun;
        float outdoorLookupMatrix[4][4];
        GfxImage* outdoorImage;
        unsigned int dynEntDataSize0;
        unsigned int dynEntDataSize1;
        unsigned int sceneDynModelCount;
        unsigned int sceneDynBrushCount;
        unsigned char* smodelVisData0;
        unsigned char* smodelVisData1;
        unsigned char* smodelVisData2;
        unsigned char* smodelVisData3;
        unsigned char* surfaceVisData0;
        unsigned char* surfaceVisData1;
        unsigned char* surfaceVisData2;
        unsigned char* surfaceVisData3;
        GfxDrawSurf* surfaceMaterials;
        unsigned int* surfaceCastsSunShadow;
        unsigned int* cellCasterBits;
        unsigned char* cellHasSunLitSurfsBits;
        unsigned char* dynEntVisData0_0;
        unsigned char* dynEntVisData0_1;
        unsigned char* dynEntVisData0_2;
        unsigned char* dynEntVisData0_3;
        unsigned char* dynEntVisData1_0;
        unsigned char* dynEntVisData1_1;
        unsigned char* dynEntVisData1_2;
        unsigned char* dynEntVisData1_3;
        unsigned char* dynEntCellBits0;
        unsigned char* dynEntCellBits1;
        unsigned short* dynEntSortList0;
        unsigned short* dynEntSortList1;
        GfxSceneDynModel* sceneDynModel;
        GfxSceneDynBrush* sceneDynBrush;
        GfxDynEntCellRef* dynEntCellRefs;
        unsigned int* dynEntUnknown;
        unsigned short* sortedSurfIndex;
        unsigned int* primaryLightEntityShadowVis;
        unsigned int* primaryLightDynEntShadowVis0;
        unsigned int* primaryLightDynEntShadowVis1;
        GfxShadowGeometry* shadowGeom;
        unsigned int lightRegionCount;
        GfxLightRegion* lightRegion;
        Material* unknownMaterial;
    };

    struct type_align(4) GfxLightImage
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

    struct Glyph
    {
        unsigned short letter;
        char x0;
        char y0;
        unsigned char dx;
        unsigned char pixelWidth;
        unsigned char pixelHeight;
        unsigned char unknown0;
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

    struct ItemKeyHandler
    {
        int key;
        const char* action;
        ItemKeyHandler* next;
    };

    union operandInternalDataUnion
    {
        int intVal;
        float floatVal;
        const char* string;
    };

    enum expDataType
    {
        VAL_INT = 0x0,
        VAL_FLOAT = 0x1,
        VAL_STRING = 0x2,
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
        OP_TOINT = 0x40,
        OP_TOSTRING = 0x41,
        OP_TOFLOAT = 0x42,
        OP_GAMETYPENAME = 0x43,
        OP_GAMETYPE = 0x44,
        OP_GAMETYPEDESCRIPTION = 0x45,
        OP_SCORE = 0x46,
        OP_FRIENDSONLINE = 0x47,
        OP_FOLLOWING = 0x48,
        OP_STATRANGEBITSSET = 0x49,
        NUM_OPERATORS = 0x4A,
    };

    struct Operand
    {
        expDataType dataType;
        operandInternalDataUnion internals;
    };

    union entryInternalData
    {
        Operand operand;
        operationEnum op;
    };

    enum expressionEntryType
    {
        EET_OPERATOR = 0x0,
        EET_OPERAND = 0x1,
    };

    struct expressionEntry
    {
        expressionEntryType type;
        entryInternalData data;
    };

    struct statement_s
    {
        int numEntries;
        expressionEntry** entries;
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
        int mousePos;
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
        ITEM_TYPE_GAME_MESSAGE_WINDOW = 0x13,
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
        int textSavegameInfo;
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
        int feeder;
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
        statement_s unknownStatement0;
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

    struct WeaponDef
    {
        const char* name;
        const char* displayName;
        const char* overlayName;
        XModel* unknownModel12;
        XModel* unknownModel16;
        const char* xanimNames[35];
        const char* modeName;
        unsigned char unknownData164[32];
        const char* unknownString196;
        unsigned char unknownData200[4];
        FxEffectDef* unknownFx204;
        FxEffectDef* unknownFx208;
        SndAliasCustom unknownSoundAliases[53];
        SndAliasCustom* bounceSound;
        FxEffectDef* unknownFx428;
        FxEffectDef* unknownFx432;
        FxEffectDef* unknownFx436;
        FxEffectDef* unknownFx440;
        Material* unknownMaterial444;
        Material* unknownMaterial448;
        unsigned char unknownData452[152];
        XModel* unknownModels604[3];
        const char* unknownString616;
        const char* unknownString620;
        XModel* unknownModel624;
        Material* unknownMaterial628;
        unsigned char unknownData632[4];
        Material* unknownMaterial636;
        unsigned char unknownData640[12];
        const char* unknownString652;
        unsigned char unknownData656[4];
        const char* unknownString660;
        unsigned char unknownData664[16];
        const char* unknownString680;
        unsigned char unknownData684[280];
        Material* unknownMaterial964;
        unsigned char unknownData968[24];
        const char* unknownString992;
        unsigned char unknownData996[224];
        const char* unknownString1220;
        unsigned char unknownData1224[40];
        Material* unknownMaterial1264;
        unsigned char unknownData1268[8];
        Material* unknownMaterial1276;
        unsigned char unknownData1280[20];
        const char* unknownString1300;
        unsigned char unknownData1304[4];
        const char* unknownString1308;
        unsigned char unknownData1312[4];
        XModel* unknownModels1316[16];
        const char* unknownStrings1380[16];
        unsigned char unknownData1444[80];
        XModel* unknownModel1524;
        unsigned char unknownData1528[4];
        FxEffectDef* unknownFx1532;
        unsigned char unknownData1536[4];
        FxEffectDef* unknownFx1540;
        SndAliasCustom unknownSound1544;
        SndAliasCustom unknownSound1548;
        unsigned char unknownData1552[32];
        FxEffectDef* unknownFx1584;
        FxEffectDef* unknownFx1588;
        SndAliasCustom unknownSound1592;
        unsigned char unknownData1596[240];
        FxEffectDef* unknownFx1836;
        unsigned char unknownData1840[24];
        FxEffectDef* unknownFx1864;
        SndAliasCustom unknownSound1868;
        unsigned char unknownData1872[208];
        const char* aiVsAiAccuracyGraphName;
        const char* aiVsPlayerAccuracyGraphName;
        float (*aiVsAiAccuracyGraphKnots)[2];
        float (*aiVsPlayerAccuracyGraphKnots)[2];
        float (*originalAiVsAiAccuracyGraphKnots)[2];
        float (*originalAiVsPlayerAccuracyGraphKnots)[2];
        int aiVsAiAccuracyGraphKnotCount;
        int aiVsPlayerAccuracyGraphKnotCount;
        unsigned char unknownData2112[80];
        const char* unknownString2192;
        const char* unknownString2196;
        unsigned char unknownData2200[16];
        const char* unknownString2216;
        unsigned char unknownData2220[112];
        const char* unknownString2332;
        const char* unknownString2336;
        unsigned char unknownData2340[424];
    };

    struct XAUDIOREVERBSETTINGS
    {
        unsigned int ReflectionsDelay;
        unsigned char ReverbDelay;
        unsigned char RearDelay;
        unsigned char PositionLeft;
        unsigned char PositionRight;
        unsigned char PositionMatrixLeft;
        unsigned char PositionMatrixRight;
        unsigned char EarlyDiffusion;
        unsigned char LateDiffusion;
        unsigned char LowEQGain;
        unsigned char LowEQCutoff;
        unsigned char HighEQGain;
        unsigned char HighEQCutoff;
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

    struct FxSpawnDefOneShot
    {
        FxIntRange count;
    };

    struct FxSpawnDefLooping
    {
        int intervalMsec;
        int count;
    };

    union FxSpawnDef
    {
        FxSpawnDefLooping looping;
        FxSpawnDefOneShot oneShot;
    };

    struct FxElemAtlas
    {
        unsigned char behavior;
        unsigned char index;
        unsigned char fps;
        unsigned char loopCount;
        unsigned char colIndexBits;
        unsigned char rowIndexBits;
        short entryCount;
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
        unsigned char color[4];
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

    union FxEffectDefRef
    {
        FxEffectDef* handle;
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

    union FxElemVisuals
    {
        const void* anonymous;
        Material* material;
        XModel* model;
        FxEffectDefRef effectDef;
        const char* soundName;
    };

    struct FxElemMarkVisuals
    {
        Material* materials[2];
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
        unsigned short* inds;
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
        unsigned char elemType;
        unsigned char visualCount;
        unsigned char velIntervalCount;
        unsigned char visStateIntervalCount;
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
        unsigned char sortOrder;
        unsigned char lightingFrac;
        unsigned char useItemClip;
        unsigned char unused;
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

    struct DestructibleModel
    {
        XModel* model;
        float breakHealth;
        FxEffectDef* breakEffect;
        SndAliasCustom breakSound;
        const char* breakNotify;
        XModel* spawnModel[3];
    };

    struct DestructiblePiece
    {
        DestructibleModel dmodels[5];
        const char* attachBone;
        ScriptString attachBoneHash;
        unsigned char parentPiece;
        unsigned char unused;
        float parentDamagePercent;
        float bulletDamageScale;
        float explosiveDamageScale;
        PhysConstraints* physConstraints;
        int health;
        FxEffectDef* burnEffect;
        SndAliasCustom burnSound;
    };

    struct DestructibleDef
    {
        const char* name;
        int unknown0;
        int numPieces;
        DestructiblePiece* pieces;
    };

    struct FxImpactEntry
    {
        FxEffectDef* nonflesh[30];
        FxEffectDef* flesh[4];
    };

    struct FxImpactTable
    {
        const char* name;
        FxImpactEntry* table;
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

    struct complex_s
    {
        float real;
        float imag;
    };

    struct WaterWritable
    {
        float floatTime;
    };

    struct water_t
    {
        WaterWritable writable;
        complex_s* H0;
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

    struct MaterialWaterDef
    {
        unsigned char unknown[28];
        water_t* water;
    };

    union MaterialTextureDefInfo
    {
        GfxImage* image;
        MaterialWaterDef* water;
    };

    struct MaterialTextureDef
    {
        unsigned int nameHash;
        unsigned char nameStart;
        unsigned char nameEnd;
        unsigned char samplerState;
        unsigned char semantic;
        MaterialTextureDefInfo u;
    };

    struct MaterialConstantDef
    {
        unsigned int unknown[8];
    };

    struct GfxStateBits
    {
        unsigned int loadBits[2];
    };

    struct MaterialInfo
    {
        const char* name;
        unsigned char unknown[20];
    };

    struct Material
    {
        MaterialInfo info;
        unsigned char unknown24[36];
        unsigned char textureCount;
        unsigned char constantCount;
        unsigned char stateBitsCount;
        unsigned char unknown63[13];
        MaterialTechniqueSet* techniqueSet;
        MaterialTextureDef* textureTable;
        MaterialConstantDef* constantTable;
        GfxStateBits* stateBitsTable;
        unsigned char unknown92[4];
    };

    struct type_align(4) D3DBaseTexture
    {
        uint32_t Common;
        uint32_t ReferenceCount;
        uint32_t Fence;
        uint32_t ReadFence;
        uint32_t Identifier;
        uint32_t BaseFlush;
        uint32_t MipFlush;
        uint32_t Format[6];
    };

    typedef D3DBaseTexture D3DTexture;
    typedef D3DBaseTexture D3DVolumeTexture;
    typedef D3DBaseTexture D3DCubeTexture;

    union GfxTexture
    {
        D3DTexture* map;
        D3DVolumeTexture* volmap;
        D3DCubeTexture* cubemap;
    };

    struct GfxImageLoadDef
    {
        unsigned char levelCount;
        unsigned char flags;
        short dimensions[3];
        int format;
        GfxTexture texture;
    };

    union GfxTextureLoad
    {
        GfxImageLoadDef* loadDef;
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

    struct CardMemory
    {
        int platform[1];
    };

    struct GfxImage
    {
        MapType mapType;
        GfxTextureLoad texture;
        unsigned char semantic;
        CardMemory cardMemory;
        unsigned short width;
        unsigned short height;
        unsigned short depth;
        unsigned char category;
        unsigned char* pixels;
        unsigned int baseSize;
        unsigned short streamSlot;
        bool streaming;
        const char* name;
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

    struct xmlAttribute
    {
        const char* name;
        const char* value;
        int unknown0;
    };

    struct xmlNode;

    struct xmlNodeList
    {
        int nodeCount;
        xmlNode** nodes;
    };

    struct xmlAttribList
    {
        int unknown0;
        xmlAttribute* attributes[8];
    };

    struct xmlNodeComplex
    {
        const char* name;
        xmlAttribList attribList;
        xmlNodeList childList;
    };

    struct xmlNode
    {
        const char* name;
        int unknown0;
        xmlNodeComplex* complex;
    };

    struct XmlTree
    {
        xmlNodeList nodeList;
        const char* name;
    };

    struct AnimReference
    {
        const char* unknownString0;
        ScriptString unknownScriptString0;
        unsigned char unknown0[6];
        XAnimParts* animation;
    };

    struct SceneAnimation
    {
        const char* name;
        unsigned int animReferenceCount;
        unsigned int unknown0;
        AnimReference* animReferences;
    };

    struct CutsceneModelAttachment
    {
        const char* unknownString0;
        const char* unknownString1;
    };

    struct CutsceneEntityDef
    {
        const char* unknownString0;
        unsigned char unknown0[8];
        const char* unknownString1;
        unsigned char unknown1[2];
        ScriptString unknownScriptString0;
        unsigned int modelAttachmentCount;
        CutsceneModelAttachment* modelAttachments;
    };

    struct CutsceneSectionDef
    {
        unsigned int entityCount;
        CutsceneEntityDef* entities;
        unsigned int sceneAnimationCount;
        SceneAnimation** sceneAnimations;
    };

    struct Cutscene
    {
        const char* name;
        unsigned char unknown0[12];
        unsigned int entityCount;
        CutsceneEntityDef* entities;
        unsigned int sectionCount;
        CutsceneSectionDef* sections;
    };

    struct CustomCameraDef
    {
        const char* name;
        int unknown4;
        const char* unknownString0;
        const char* unknownString1;
        unsigned char unknown16[100];
    };

    struct CustomCamera
    {
        const char* name;
        int cameraCount;
        CustomCameraDef* cameras;
    };

    struct type_align(4) MaterialVertexDeclaration
    {
        unsigned char unknown[80];
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

    enum MaterialShaderArgumentType
    {
        MTL_ARG_MATERIAL_VERTEX_CONST = 0x0,
        MTL_ARG_LITERAL_VERTEX_CONST = 0x1,
        MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x2,
        MTL_ARG_CODE_VERTEX_CONST = 0x3,
        MTL_ARG_CODE_PIXEL_SAMPLER = 0x4,
        MTL_ARG_CODE_PIXEL_CONST = 0x5,
        MTL_ARG_MATERIAL_PIXEL_CONST = 0x6,
        MTL_ARG_LITERAL_PIXEL_CONST = 0x7,
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

    struct MaterialShaderArgument
    {
        unsigned short type;
        unsigned short dest;
        MaterialArgumentDef u;
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShaderArray[19];
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        unsigned char perPrimArgCount;
        unsigned char perObjArgCount;
        unsigned char stableArgCount;
        unsigned char customSamplerFlags;
        unsigned char precompiledIndex;
        unsigned char unknown93[3];
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
        unsigned char unknown4[8];
        MaterialTechnique* techniques[36];
    };
#ifndef __zonecodegenerator
} // namespace QOSXenon
#endif
