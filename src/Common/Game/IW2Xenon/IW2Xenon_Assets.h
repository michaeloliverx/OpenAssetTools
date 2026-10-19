#pragma once

#include "../../Utils/TypeAlignment.h"

#ifndef __zonecodegenerator
namespace IW2Xenon
{
#endif
    enum XFileBlock
    {
        XFILE_BLOCK_TEMP,
        XFILE_BLOCK_VIRTUAL,
        XFILE_BLOCK_PHYSICAL,
        MAX_XFILE_COUNT
    };

    enum XAssetType
    {
        ASSET_TYPE_XANIMPARTS,
        ASSET_TYPE_XMODEL,
        ASSET_TYPE_MATERIAL,
        ASSET_TYPE_IMAGE,
        ASSET_TYPE_SOUND,
        ASSET_TYPE_SND_CURVE,
        ASSET_TYPE_CLIPMAP_SP,
        ASSET_TYPE_CLIPMAP_MP,
        ASSET_TYPE_GFXWORLD,
        ASSET_TYPE_LIGHT_DEF,
        ASSET_TYPE_UI_MAP,
        ASSET_TYPE_FONT,
        ASSET_TYPE_MENUFILE,
        ASSET_TYPE_MENU,
        ASSET_TYPE_LOCALIZE_ENTRY,
        ASSET_TYPE_WEAPON,
        ASSET_TYPE_SNDDRIVER_GLOBALS,
        ASSET_TYPE_FX,
        ASSET_TYPE_IMPACT_FX,
        ASSET_TYPE_AITYPE,
        ASSET_TYPE_MPTYPE,
        ASSET_TYPE_CHARACTER,
        ASSET_TYPE_XMODELALIAS,
        ASSET_TYPE_RAWFILE,
        ASSET_TYPE_COUNT
    };

    enum TextureSemantic
    {
        TS_2D = 0,
        TS_FUNCTION = 1
    };

    enum ImageFlags
    {
        IMG_FLAG_NOPICMIP = 1,
        IMG_FLAG_NOMIPMAPS = 2,
        IMG_FLAG_CUBEMAP = 4,
        IMG_FLAG_VOLMAP = 8,
        IMG_FLAG_CLAMP_U = 0x40,
        IMG_FLAG_CLAMP_V = 0x80
    };

    enum MapType
    {
        MAPTYPE_NONE = 0,
        MAPTYPE_INVALID1 = 1,
        MAPTYPE_INVALID2 = 2,
        MAPTYPE_2D = 3,
        MAPTYPE_3D = 4,
        MAPTYPE_CUBE = 5
    };
    struct RawFile;
    struct LocalizeEntry;
    struct SndCurve;
    struct D3DTexture;
    union GfxTexture;
    struct GfxImageLoadDef;
    union GfxTextureLoad;
    struct GfxImage;
    struct MaterialInfo;
    struct MaterialStateMapRule;
    struct MaterialStateMapRuleSet;
    struct MaterialStateMap;
    struct MaterialStreamRouting;
    struct D3DVertexDeclaration;
    struct MaterialVertexDeclaration;
    struct MaterialShader;
    struct MaterialArgumentCodeConst;
    union MaterialArgumentDef;
    struct MaterialShaderArgument;
    struct MaterialPassDx9;
    struct MaterialTechnique;
    struct MaterialTechniqueSet;
    struct complex_s;
    struct water_t;
    struct MaterialWaterDef;
    union MaterialTextureDefInfo;
    struct MaterialTextureDef;
    struct MaterialConstantDef;
    struct Material;
    struct GfxLightImage;
    struct GfxLightDef;
    struct Glyph;
    struct Font_s;
    struct XaReverbSettings;
    struct SndDriverGlobals;
    struct cplane_t;
    struct dmaterial_t;
    struct cLeaf_t;
    struct CollisionVertex;
    struct CollisionEdge;
    struct CollisionTriangle;
    struct CollisionBorder;
    struct CollisionAabbTree;
    struct cmodel_t;
    struct pathlink_t;
    struct cStaticModel_t;
    struct cbrushside_t;
    struct cNode_t;
    struct cLeafBrushNodeLeaf_t;
    struct cLeafBrushNodeChildren_t;
    union cLeafBrushNodeData_t;
    struct cLeafBrushNode_s;
    struct CollisionPartition;
    struct cbrush_t;
    struct pathnode_constant_t;
    struct pathnode_t;
    struct pathnode_tree_nodes_t;
    union pathnode_tree_info_t;
    struct pathnode_tree_t;
    struct clipMap_t;
    struct mnode_node_t;
    struct mnode_leaf_t;
    union mnode_info_t;
    struct mnode_t;
    struct srfTriangles_t;
    struct GfxSurface;
    struct GfxCullGroup;
    struct GfxBrushModel;
    struct GfxWorldVertex;
    struct GfxLightGridPoint;
    struct GfxLightGridColor;
    struct DpvsPlane;
    struct vec3_t;
    struct GfxWorldVertexData;
    struct GfxLight;
    struct GfxStaticModelInstance;
    struct GfxAabbTree;
    struct GfxPortal;
    struct GfxOccluderEdge;
    struct GfxOccluder;
    struct GfxCell;
    struct LightmapArray;
    struct GfxLightGrid;
    struct MaterialMemory;
    struct sunflare_t;
    struct GfxWorld;
    struct DObjAnimMat;
    struct XBlendInfo;
    struct GfxVertex;
    struct XBoneInfo;
    struct XModelCollTri;
    struct DSkel;
    struct XBoneHierarchy;
    struct XModelParts;
    struct XVertexBuffer;
    struct XRigidSurface;
    struct XSurface;
    struct XModelSurfs;
    struct XModelLodInfo;
    struct XModelCollSurf;
    struct trXSkin_t;
    struct XModel;
    struct XQuat;
    struct XQuat2;
    union XAnimDynamicIndicesPartTrans;
    union XAnimDynamicIndicesPartQuat;
    union XAnimDynamicIndicesDeltaPartTrans;
    union XAnimDynamicIndicesDeltaPartQuat;
    struct XAnimPartTransFrames;
    union XAnimPartTransData;
    struct XAnimPartTrans;
    union XAnimQuatFrames;
    struct XAnimPartQuatFrames;
    union XAnimPartQuatData;
    struct XAnimPartQuat;
    struct XAnimDeltaPartTransFrames;
    union XAnimDeltaPartTransData;
    struct XAnimDeltaPartTrans;
    struct XAnimDeltaPartQuatFrames;
    union XAnimDeltaPartQuatData;
    struct XAnimDeltaPartQuat;
    struct XAnimPart;
    struct XAnimDeltaPart;
    struct XAnimNotifyInfo;
    struct XAnimParts;
    struct XaSeekTable;
    struct XAUDIOPACKET_ALIGNED;
    struct XaSound;
    struct XaPrimedStream;
    union XaSoundInfo;
    struct SoundFile;
    struct XAUDIOCHANNELMAPENTRY;
    struct XAUDIOCHANNELMAP;
    struct SpeakerMap;
    struct snd_alias_t;
    struct snd_alias_list_t;
    union TMediaElement;
    struct TMediaList;
    struct FxList;
    struct FxCurve;
    struct FxChannel;
    struct PrimitiveTemplate;
    struct EffectTemplate;
    struct FxImpactEntry;
    struct FxImpactTable;
    struct windowDef_t;
    struct ItemKeyHandler;
    struct listBoxDef_t;
    struct editFieldDef_t;
    struct multiDef_t;
    union itemDefData_t;
    struct itemDef_t;
    struct menuDef_t;
    struct MenuList;
    struct vec2_t;
    struct WeaponDef;

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

    struct SndCurve
    {
        const char* name;
        int knotCount;
        float knots[8][2];
    };

    struct D3DTexture
    {
        unsigned char unknown0[16];
        unsigned int Format[6];
    };

    union GfxTexture
    {
        D3DTexture* map;
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

    struct GfxImage
    {
        MapType mapType;
        GfxTextureLoad texture;
        unsigned char semantic;
        unsigned char unknown9[3];
        int dataSize;
        unsigned short width;
        unsigned short height;
        unsigned short depth;
        unsigned char unknown22[2];
        unsigned char* pixels;
        unsigned int baseSize;
        const char* name;
    };

    struct MaterialInfo
    {
        const char* name;
        const char* unknownString4;
        unsigned char unknown8[12];
    };

    struct type_align(4) MaterialStateMapRule
    {
        unsigned char unknown0[32];
    };

    struct MaterialStateMapRuleSet
    {
        int ruleCount;
        MaterialStateMapRule rules[1];
    };

    struct MaterialStateMap
    {
        const char* name;
        MaterialStateMapRuleSet* ruleSets[11];
    };

    struct type_align(2) MaterialStreamRouting
    {
        unsigned char unknown0[2];
    };

    struct type_align(1) D3DVertexDeclaration
    {
        unsigned char unknown0[1];
    };

    struct MaterialVertexDeclaration
    {
        MaterialStreamRouting* routing;
        int streamCount;
        D3DVertexDeclaration* declarations[4];
    };

    struct MaterialShader
    {
        const char* name;
        unsigned char* program;
        unsigned short programSize;
        unsigned char unknown10[2];
        unsigned int runtimeShader;
    };

    struct type_align(4) MaterialArgumentCodeConst
    {
        unsigned short index;
        unsigned char firstRow;
        unsigned char rowCount;
    };

    union MaterialArgumentDef
    {
        float* literalConst;
        MaterialArgumentCodeConst codeConst;
        int codeSampler;
        const char* name;
    };

    struct type_align(4) MaterialShaderArgument
    {
        unsigned short type;
        unsigned short dest;
        MaterialArgumentDef u;
    };

    struct MaterialPassDx9
    {
        MaterialStateMap* stateMap;
        MaterialVertexDeclaration* vertexDecl;
        MaterialShader* vertexShader;
        MaterialShader* pixelShader;
        unsigned short vertexArgCount;
        unsigned short pixelArgCount;
        MaterialShaderArgument* vertexArgs;
        MaterialShaderArgument* pixelArgs;
    };

    struct MaterialTechnique
    {
        const char* name;
        unsigned short flags;
        unsigned short passCount;
        MaterialPassDx9 passArray[1];
    };

    struct MaterialTechniqueSet
    {
        const char* name;
        MaterialTechnique* techniques[34];
    };

    struct type_align(4) complex_s
    {
        unsigned char unknown0[8];
    };

    struct water_t
    {
        unsigned char unknown0[4];
        complex_s* H0;
        float* wTerm;
        int M;
        int N;
        unsigned char unknown20[44];
        GfxImage* image;
    };

    struct MaterialWaterDef
    {
        unsigned char unknown0[28];
        water_t* water;
    };

    union MaterialTextureDefInfo
    {
        MaterialWaterDef* water;
        GfxImage* image;
    };

    struct MaterialTextureDef
    {
        const char* name;
        unsigned char unknown4[1];
        unsigned char semantic;
        unsigned char unknown6[2];
        MaterialTextureDefInfo u;
    };

    struct MaterialConstantDef
    {
        const char* name;
        unsigned char unknown4[28];
    };

    struct Material
    {
        MaterialInfo info;
        unsigned char unknown20[8];
        unsigned short textureCount;
        unsigned short constantCount;
        MaterialTechniqueSet* techniqueSet;
        MaterialTextureDef* textureTable;
        MaterialConstantDef* constantTable;
    };

    struct GfxLightImage
    {
        GfxImage* image;
        unsigned char unknown4[4];
    };

    struct GfxLightDef
    {
        unsigned char unknown0[4];
        GfxLightImage attenuation;
        GfxLightImage unknownImage12;
        const char* name;
    };

    struct type_align(4) Glyph
    {
        unsigned char unknown0[24];
    };

    struct Font_s
    {
        const char* fontName;
        int pixelHeight;
        int glyphCount;
        Material* material;
        Glyph* glyphs;
    };

    struct type_align(4) XaReverbSettings
    {
        unsigned char unknown0[1352];
    };

    struct SndDriverGlobals
    {
        XaReverbSettings* reverbSettings;
        const char* name;
    };

    struct type_align(4) cplane_t
    {
        unsigned char unknown0[20];
    };

    struct type_align(4) dmaterial_t
    {
        unsigned char unknown0[72];
    };

    struct type_align(4) cLeaf_t
    {
        unsigned char unknown0[44];
    };

    struct type_align(4) CollisionVertex
    {
        unsigned char unknown0[12];
    };

    struct type_align(4) CollisionEdge
    {
        unsigned char unknown0[48];
    };

    struct type_align(4) CollisionTriangle
    {
        unsigned char unknown0[72];
    };

    struct type_align(4) CollisionBorder
    {
        unsigned char unknown0[28];
    };

    struct type_align(4) CollisionAabbTree
    {
        unsigned char unknown0[32];
    };

    struct type_align(4) cmodel_t
    {
        unsigned char unknown0[72];
    };

    struct type_align(4) pathlink_t
    {
        unsigned char unknown0[12];
    };

    struct cStaticModel_t
    {
        unsigned char unknown0[4];
        XModel* xmodel;
        unsigned char unknown8[72];
    };

    struct cbrushside_t
    {
        cplane_t* plane;
        unsigned char unknown4[4];
    };

    struct cNode_t
    {
        cplane_t* plane;
        unsigned char unknown4[4];
    };

    struct cLeafBrushNodeLeaf_t
    {
        unsigned short* brushes;
        unsigned char unknown4[8];
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
        unsigned char axis;
        unsigned char unknown1[1];
        short leafBrushCount;
        int contents;
        cLeafBrushNodeData_t data;
    };

    struct CollisionPartition
    {
        unsigned char unknown0[4];
        CollisionTriangle* triangles;
        CollisionBorder* borders;
    };

    struct cbrush_t
    {
        unsigned char unknown0[32];
        cbrushside_t* sides;
        unsigned char unknown36[12];
    };

    struct pathnode_constant_t
    {
        unsigned char unknown0[6];
        unsigned short unknownScriptStrings6[5];
        unsigned char unknown16[42];
        short totalLinkCount;
        pathlink_t* Links;
    };

    struct type_align(4) pathnode_t
    {
        pathnode_constant_t constant;
        unsigned char unknown64[56];
    };

    struct pathnode_ptr
    {
        pathnode_t* node;
    };

    struct pathnode_tree_nodes_t
    {
        int nodeCount;
        pathnode_ptr* nodes;
    };

    union pathnode_tree_info_t
    {
        pathnode_tree_nodes_t nodes;
        pathnode_tree_t* children[2];
    };

    struct pathnode_tree_t
    {
        int axis;
        unsigned char unknown4[4];
        pathnode_tree_info_t u;
    };

    struct clipMap_t
    {
        const char* name;
        int numStaticModels;
        cStaticModel_t* staticModelList;
        int numMaterials;
        dmaterial_t* materials;
        int numBrushSides;
        cbrushside_t* brushsides;
        int numNodes;
        cNode_t* nodes;
        int numLeafs;
        cLeaf_t* leafs;
        int leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        int numLeafBrushes;
        unsigned short* leafbrushes;
        int numLeafSurfaces;
        int* leafsurfaces;
        int vertCount;
        CollisionVertex* verts;
        int edgeCount;
        CollisionEdge* edges;
        int triCount;
        CollisionTriangle* triangles;
        int borderCount;
        CollisionBorder* borders;
        int partitionCount;
        CollisionPartition* partitions;
        int aabbTreeCount;
        CollisionAabbTree* aabbTrees;
        int numSubModels;
        cmodel_t* cmodels;
        unsigned short numBrushes;
        unsigned char unknown126[2];
        cbrush_t* brushes;
        int numClusters;
        int clusterBytes;
        unsigned char* visibility;
        unsigned char unknown144[4];
        int numEntityChars;
        char* entityString;
        cbrush_t* box_brush;
        unsigned char unknown160[72];
        int pathNodeCount;
        pathnode_t* pathNodes;
        unsigned char unknown240[4];
        unsigned short* chainNodeForNode;
        unsigned short* nodeForChainNode;
        int visBytes;
        unsigned char* pathVis;
        int nodeTreeCount;
        pathnode_tree_t* nodeTree;
        unsigned char unknown268[4];
    };

    struct mnode_node_t
    {
        cplane_t* plane;
        mnode_t* children[2];
    };

    struct type_align(4) mnode_leaf_t
    {
        unsigned char unknown0[12];
    };

    union mnode_info_t
    {
        mnode_node_t node;
        mnode_leaf_t leaf;
    };

    struct mnode_t
    {
        int contents;
        mnode_t* parent;
        unsigned char unknown8[4];
        mnode_info_t u;
    };

    struct srfTriangles_t
    {
        unsigned char unknown0[34];
        unsigned short indexCount;
        unsigned short* indices;
    };

    struct GfxSurface
    {
        Material* material;
        unsigned char unknown4[4];
        srfTriangles_t* tris;
    };

    struct type_align(4) GfxCullGroup
    {
        unsigned char unknown0[32];
    };

    struct type_align(4) GfxBrushModel
    {
        unsigned char unknown0[32];
    };

    struct type_align(4) GfxWorldVertex
    {
        unsigned char unknown0[44];
    };

    struct type_align(4) GfxLightGridPoint
    {
        unsigned char unknown0[8];
    };

    struct type_align(4) GfxLightGridColor
    {
        unsigned char unknown0[24];
    };

    struct type_align(4) DpvsPlane
    {
        unsigned char unknown0[20];
    };

    struct type_align(4) vec3_t
    {
        unsigned char unknown0[12];
    };

    struct GfxWorldVertexData
    {
        GfxWorldVertex* vertices;
        unsigned char unknown4[20];
    };

    struct GfxLight
    {
        GfxLightDef* def;
        unsigned char unknown4[40];
    };

    struct GfxStaticModelInstance
    {
        unsigned char unknown0[16];
        XModel* model;
        unsigned char unknown20[76];
    };

    struct GfxAabbTree
    {
        unsigned char unknown0[32];
        int staticModelCount;
        int* staticModelIndices;
        int childrenCount;
        GfxAabbTree* children;
    };

    struct GfxPortal
    {
        unsigned char unknown0[28];
        GfxCell* cell;
        float* vertices;
        unsigned char vertexCount;
        unsigned char unknown37[31];
    };

    struct GfxOccluderEdge
    {
        DpvsPlane* planes[2];
        vec3_t* vertices[2];
    };

    struct GfxOccluder
    {
        int planeCount;
        DpvsPlane* planes;
        int edgeCount;
        GfxOccluderEdge* edges;
        int vertexCount;
        float* vertices;
        unsigned char unknown24[12];
    };

    struct GfxOccluderRef
    {
        GfxOccluder* occluder;
    };

    struct GfxCell
    {
        unsigned char unknown0[28];
        GfxAabbTree* aabbTree;
        int portalCount;
        GfxPortal* portals;
        int cullGroupCount;
        int* cullGroups;
        int occluderCount;
        GfxOccluderRef* occluders;
        unsigned char unknown56[4];
    };

    struct LightmapArray
    {
        GfxImage* images[4];
    };

    struct GfxLightGrid
    {
        int pointCount;
        GfxLightGridPoint* points;
        int colorCount;
        GfxLightGridColor* colors;
    };

    struct MaterialMemory
    {
        Material* material;
        unsigned char unknown4[4];
    };

    struct sunflare_t
    {
        unsigned char unknown0[4];
        Material* spriteMaterial;
        Material* flareMaterial;
        unsigned char unknown12[84];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        int nodeCount;
        mnode_t* nodes;
        int surfaceCount;
        GfxSurface* surfaces;
        int unknownCount24;
        int* unknownIndices28;
        GfxImage* skyImage;
        unsigned char unknown36[4];
        int vertexCount;
        GfxWorldVertexData vertexData;
        unsigned char unknown68[128];
        GfxLight light;
        unsigned char unknown240[12];
        int cullGroupCount;
        GfxCullGroup* cullGroups;
        int staticModelCount;
        GfxStaticModelInstance* staticModels;
        int cellCount;
        GfxCell* cells;
        int lightmapCount;
        LightmapArray* lightmaps;
        GfxImage* unknownImage284;
        unsigned char unknown288[12];
        GfxLightGrid lightGrid;
        int brushModelCount;
        GfxBrushModel* brushModels;
        unsigned char unknown324[28];
        int materialMemoryCount;
        MaterialMemory* materialMemory;
        sunflare_t sun;
        unsigned char unknown456[64];
        GfxImage* unknownImage520;
    };

    struct type_align(4) DObjAnimMat
    {
        unsigned char unknown0[32];
    };

    struct type_align(4) XBlendInfo
    {
        unsigned char unknown0[16];
    };

    struct type_align(4) GfxVertex
    {
        unsigned char unknown0[64];
    };

    struct type_align(4) XBoneInfo
    {
        unsigned char unknown0[40];
    };

    struct type_align(4) XModelCollTri
    {
        unsigned char unknown0[48];
    };

    struct type_align(4) DSkel
    {
        unsigned char unknown0[48];
        DObjAnimMat mat[1];
    };

    struct XBoneHierarchy
    {
        unsigned short* names;
        unsigned char parentList[1];
    };

    struct XModelParts
    {
        short numBones;
        short numRootBones;
        XBoneHierarchy* hierarchy;
        short* quats;
        float* trans;
        unsigned char* partClassification;
        DSkel skel;
    };

    struct type_align(4) XVertexBuffer
    {
        unsigned char unknown0[60];
        unsigned char blendCount;
        unsigned char unknown61[3];
        XBlendInfo w[1];
    };

    struct XRigidSurface
    {
        unsigned char unknown0[20];
        GfxVertex* verts;
    };

    struct XSurface
    {
        unsigned char unknown0[2];
        short vertCount;
        short triCount;
        unsigned char unknown6[2];
        unsigned short* triIndices;
        XVertexBuffer* verts;
        XRigidSurface rigid;
        unsigned char unknown40[20];
    };

    struct XModelSurfs
    {
        XSurface** surfs;
        unsigned char unknown4[16];
    };

    struct XModelLodInfo
    {
        float dist;
        const char* filename;
        short numsurfs;
        unsigned char unknown10[2];
        unsigned short* surfNames;
        XModelSurfs* surfs;
    };

    struct XModelCollSurf
    {
        XModelCollTri* collTris;
        int numCollTris;
        unsigned char unknown8[36];
    };

    struct trXSkin_t
    {
        Material** materials;
    };

    struct XModel
    {
        XModelParts* parts;
        XModelLodInfo lodInfo[4];
        XModelCollSurf* collSurfs;
        int numCollSurfs;
        unsigned char unknown92[4];
        XBoneInfo* boneInfo;
        unsigned char unknown100[24];
        short numLods;
        short collLod;
        trXSkin_t* xskins;
        unsigned char unknown132[4];
        const char* name;
        unsigned char unknown140[4];
    };

    struct type_align(4) XQuat
    {
        unsigned char unknown0[8];
    };

    struct type_align(4) XQuat2
    {
        unsigned char unknown0[4];
    };

    union XAnimDynamicIndicesPartTrans
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    union XAnimDynamicIndicesPartQuat
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    union XAnimDynamicIndicesDeltaPartTrans
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    union XAnimDynamicIndicesDeltaPartQuat
    {
        unsigned char _1[1];
        unsigned short _2[1];
    };

    struct XAnimPartTransFrames
    {
        vec3_t* frames;
        XAnimDynamicIndicesPartTrans indices;
    };

    union XAnimPartTransData
    {
        XAnimPartTransFrames frames;
        vec3_t frame0;
    };

    struct type_align(4) XAnimPartTrans
    {
        unsigned short size;
        unsigned char unknown2[2];
        XAnimPartTransData u;
    };

    union XAnimQuatFrames
    {
        XQuat2* small;
        XQuat* normal;
    };

    struct type_align(4) XAnimPartQuatFrames
    {
        XAnimQuatFrames frames;
        XAnimDynamicIndicesPartQuat indices;
    };

    union XAnimPartQuatData
    {
        XAnimPartQuatFrames frames;
        XQuat frame0;
        XQuat2 frame02;
    };

    struct type_align(4) XAnimPartQuat
    {
        unsigned short size;
        unsigned char unknown2[2];
        XAnimPartQuatData u;
    };

    struct XAnimDeltaPartTransFrames
    {
        vec3_t* frames;
        XAnimDynamicIndicesDeltaPartTrans indices;
    };

    union XAnimDeltaPartTransData
    {
        XAnimDeltaPartTransFrames frames;
        vec3_t frame0;
    };

    struct type_align(4) XAnimDeltaPartTrans
    {
        unsigned short size;
        unsigned char unknown2[2];
        XAnimDeltaPartTransData u;
    };

    struct XAnimDeltaPartQuatFrames
    {
        XQuat2* frames;
        XAnimDynamicIndicesDeltaPartQuat indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatFrames frames;
        XQuat2 frame0;
    };

    struct type_align(4) XAnimDeltaPartQuat
    {
        unsigned short size;
        unsigned char unknown2[2];
        XAnimDeltaPartQuatData u;
    };

    struct XAnimPart
    {
        XAnimPartTrans* trans;
        XAnimPartQuat* quat;
    };

    struct XAnimDeltaPart
    {
        XAnimDeltaPartTrans* trans;
        XAnimDeltaPartQuat* quat;
    };

    struct XAnimNotifyInfo
    {
        unsigned short name;
        unsigned char unknown2[2];
        float time;
    };

    struct XAnimParts
    {
        unsigned short numframes;
        unsigned char unknown2[10];
        unsigned char notifyCount;
        unsigned char unknown13[1];
        short boneCount;
        unsigned short* names;
        unsigned char* bSmallQuat;
        XAnimPart* parts;
        XAnimNotifyInfo* notify;
        XAnimDeltaPart* deltaPart;
        const char* name;
    };

    struct XaSeekTable
    {
        int count;
        unsigned int* values;
    };

    struct XAUDIOPACKET_ALIGNED
    {
        unsigned char* buffer;
        int size;
        unsigned char unknown8[76];
        unsigned char* unknownPointer84;
    };

    struct type_align(4) XaSound
    {
        XAUDIOPACKET_ALIGNED packet;
        unsigned char unknown88[60];
        XaSeekTable seekTable;
    };

    struct type_align(4) XaPrimedStream
    {
        unsigned char unknown0[60];
        XaSeekTable seekTable;
        unsigned char unknown68[8];
        unsigned short bufferSize;
        unsigned char buffer[1];
    };

    union XaSoundInfo
    {
        XaSound loaded;
        XaPrimedStream streamed;
    };

    struct SoundFile
    {
        const char* name;
        XaSoundInfo* info;
        unsigned char unknown8[4];
        int type;
    };

    struct type_align(4) XAUDIOCHANNELMAPENTRY
    {
        unsigned char unknown0[8];
    };

    struct XAUDIOCHANNELMAP
    {
        unsigned char entryCount;
        unsigned char unknown1[3];
        XAUDIOCHANNELMAPENTRY* entries;
    };

    struct SpeakerMap
    {
        unsigned char unknown0[4];
        const char* name;
        XAUDIOCHANNELMAP channelMaps[4];
    };

    struct snd_alias_t
    {
        const char* aliasName;
        const char* unknownString4;
        const char* unknownString8;
        SoundFile* soundFile;
        unsigned char unknown16[48];
        SndCurve* volumeFalloffCurve;
        SpeakerMap* speakerMap;
    };

    struct snd_alias_list_t
    {
        const char* name;
        snd_alias_t* head;
        int count;
    };

    union TMediaElement
    {
        XModel* model;
        Material* material;
    };

    struct TMediaList
    {
        TMediaElement* items;
        unsigned short count;
        unsigned char unknown6[2];
    };

    struct FxList
    {
        const char** names;
        unsigned short count;
        unsigned char unknown6[2];
    };

    struct FxCurve
    {
        int dimension;
        int keyCount;
        float keys[1];
    };

    struct FxChannel
    {
        FxCurve* curve;
        unsigned char unknown4[8];
    };

    struct PrimitiveTemplate
    {
        unsigned char unknown0[64];
        int type;
        unsigned char unknown68[36];
        TMediaList media;
        FxList effects[4];
        unsigned char unknown144[112];
        FxChannel channels[24];
        unsigned char unknown544[132];
    };

    struct EffectTemplate
    {
        const char* name;
        unsigned char unknown4[4];
        PrimitiveTemplate* primitives[24];
    };

    struct FxImpactEntry
    {
        EffectTemplate* effects[23];
    };

    struct FxImpactTable
    {
        const char* name;
        FxImpactEntry* table;
    };

    struct windowDef_t
    {
        unsigned char unknown0[192];
        const char* name;
        const char* group;
        const char* unknownString200;
        unsigned char unknown204[320];
        Material* background;
    };

    struct ItemKeyHandler
    {
        int key;
        const char* action;
        ItemKeyHandler* next;
    };

    struct listBoxDef_t
    {
        unsigned char unknown0[260];
        const char* doubleClick;
        unsigned char unknown264[8];
    };

    struct type_align(4) editFieldDef_t
    {
        unsigned char unknown0[32];
    };

    struct multiDef_t
    {
        const char* dvarList[32];
        const char* dvarStr[32];
        unsigned char unknown256[136];
    };

    union itemDefData_t
    {
        listBoxDef_t* listBox;
        editFieldDef_t* editField;
        multiDef_t* multi;
        const char* enumDvarName;
    };

    struct itemDef_t
    {
        windowDef_t window;
        unsigned char unknown528[96];
        int type;
        unsigned char unknown628[32];
        const char* unknownString660;
        unsigned char unknown664[8];
        const char* unknownString672;
        const char* unknownString676;
        const char* unknownString680;
        const char* unknownString684;
        const char* unknownString688;
        const char* unknownString692;
        const char* unknownString696;
        const char* unknownString700;
        const char* unknownString704;
        const char* unknownString708;
        ItemKeyHandler* onKey;
        const char* unknownString716;
        unsigned char unknown720[4];
        snd_alias_list_t* focusSound;
        unsigned char unknown728[20];
        itemDefData_t typeData;
        unsigned char unknown752[4];
    };

    struct menuDef_t
    {
        windowDef_t window;
        const char* font;
        unsigned char unknown532[4];
        int itemCount;
        unsigned char unknown540[40];
        const char* unknownString580;
        const char* unknownString584;
        const char* unknownString588;
        ItemKeyHandler* onKey;
        const char* unknownString596;
        unsigned char unknown600[36];
        itemDef_t** items;
    };

    struct MenuList
    {
        const char* name;
        int menuCount;
        menuDef_t** menus;
    };

    struct type_align(4) vec2_t
    {
        unsigned char unknown0[8];
    };

    struct WeaponDef
    {
        const char* name;
        const char* unknownString4;
        const char* unknownString8;
        const char* unknownString12;
        const char* unknownString16;
        const char* unknownString20;
        const char* unknownString24;
        const char* unknownString28;
        const char* unknownString32;
        const char* unknownString36;
        const char* unknownString40;
        const char* unknownString44;
        const char* unknownString48;
        const char* unknownString52;
        const char* unknownString56;
        const char* unknownString60;
        const char* unknownString64;
        const char* unknownString68;
        const char* unknownString72;
        const char* unknownString76;
        const char* unknownString80;
        const char* unknownString84;
        const char* unknownString88;
        const char* unknownString92;
        const char* unknownString96;
        const char* unknownString100;
        const char* unknownString104;
        const char* unknownString108;
        const char* unknownString112;
        unsigned char unknown116[28];
        const char* unknownString144;
        const char* unknownString148;
        const char* unknownString152;
        const char* unknownString156;
        const char* unknownString160;
        const char* unknownString164;
        const char* unknownString168;
        const char* unknownString172;
        const char* unknownString176;
        const char* unknownString180;
        const char* unknownString184;
        const char* unknownString188;
        const char* unknownString192;
        const char* unknownString196;
        const char* unknownString200;
        const char* unknownString204;
        const char* unknownString208;
        const char* unknownString212;
        const char* unknownString216;
        const char* unknownString220;
        const char* unknownString224;
        const char* unknownString228;
        const char* unknownString232;
        const char* unknownString236;
        const char* unknownString240;
        const char* unknownString244;
        const char* unknownString248;
        const char* unknownString252;
        const char* unknownString256;
        const char* unknownString260;
        const char* unknownString264;
        const char* unknownString268;
        const char* unknownString272;
        const char* unknownString276;
        const char* unknownString280;
        const char* unknownString284;
        unsigned char unknown288[148];
        const char* unknownString436;
        const char* unknownString440;
        const char* unknownString444;
        unsigned char unknown448[4];
        const char* unknownString452;
        unsigned char unknown456[4];
        const char* unknownString460;
        unsigned char unknown464[16];
        const char* unknownString480;
        unsigned char unknown484[144];
        const char* unknownString628;
        unsigned char unknown632[212];
        const char* unknownString844;
        unsigned char unknown848[24];
        const char* unknownString872;
        unsigned char unknown876[32];
        const char* unknownString908;
        unsigned char unknown912[4];
        const char* unknownString916;
        const char* unknownString920;
        unsigned char unknown924[188];
        const char* unknownString1112;
        unsigned char unknown1116[176];
        const char* unknownString1292;
        const char* unknownString1296;
        vec2_t* accuracyGraph0;
        vec2_t* accuracyGraph1;
        int accuracyGraphCount0;
        int accuracyGraphCount1;
        unsigned char unknown1316[72];
        const char* unknownString1388;
        const char* unknownString1392;
        unsigned char unknown1396[16];
        const char* unknownString1412;
        unsigned char unknown1416[116];
        const char* unknownString1532;
        const char* unknownString1536;
    };
#ifndef __zonecodegenerator
} // namespace IW2Xenon
#endif
