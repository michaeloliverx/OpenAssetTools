#include "D3DBspWriterT4Xenon204.h"

#include "Image/XenonImageToCommonConverter.h"
#include "Utils/Endianness.h"
#include "Utils/MemoryManager.h"

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
    namespace src = T4Xenon204;
    namespace view = IW3Xenon253;

    void Require(bool condition, const char* message)
    {
        if (!condition)
            throw std::runtime_error(message);
    }

    template<class T> size_t Index(const T* base, size_t count, const T* pointer)
    {
        const auto start = reinterpret_cast<uintptr_t>(base), address = reinterpret_cast<uintptr_t>(pointer);
        Require(base && pointer && address >= start && (address - start) % sizeof(T) == 0 && (address - start) / sizeof(T) < count,
                "T4 FF204 array reference is out of range");
        return (address - start) / sizeof(T);
    }

#include "LightGridConversionT4Xenon204.inl"

    unsigned CountSourceModels204(const src::clipMap_t& clip)
    {
        if (!clip.entityString || clip.numEntityChars <= 0)
            return 0;
        std::istringstream input(std::string(clip.entityString, clip.numEntityChars));
        bool inEntity = false;
        unsigned count = 0;
        for (;;)
        {
            input >> std::ws;
            const auto next = input.peek();
            if (next == std::char_traits<char>::eof() || next == 0)
                break;
            if (next == '{' || next == '}')
            {
                Require(inEntity == (next == '}'), "Invalid T4 FF204 entity braces");
                inEntity = next == '{';
                input.get();
                continue;
            }
            std::string key, value;
            Require(inEntity && bool(input >> std::quoted(key) >> std::quoted(value)), "Invalid T4 FF204 entity key/value");
            if (key == "classname" && value == "misc_model")
                ++count;
        }
        Require(!inEntity, "Unclosed T4 FF204 entity");
        return count;
    }

    class Adapter
    {
        MemoryManager memory;
        ISearchPath& searchPath;
        src::d3dbsp::Recovery& recovery;
        std::unordered_map<const src::XModel*, view::XModel*> models;
        std::unordered_map<const src::cplane_s*, size_t> planeIndices;
        std::unordered_map<const view::GfxImage*, std::pair<const src::GfxImage*, const src::GfxImage*>> splitLightmaps;
        std::vector<view::cplane_s> planes;
        std::vector<unsigned short> partition;
        ConvertedGrid204 grid;
        bool sourceModelsPresent = false;

        void AddPlane(const src::cplane_s* source)
        {
            Require(source, "Missing T4 FF204 plane");
            if (planeIndices.contains(source))
                return;
            for (const auto component : source->normal)
                Require(std::isfinite(component), "Non-finite T4 FF204 plane");
            Require(std::isfinite(source->dist), "Non-finite T4 FF204 plane distance");
            planeIndices.emplace(source, planes.size());
            view::cplane_s plane{};
            std::memcpy(&plane, source, sizeof(plane));
            planes.push_back(plane);
            for (auto& component : plane.normal)
                component = -component;
            plane.dist = -plane.dist;
            plane.signbits = 0;
            for (unsigned axis = 0; axis < 3; ++axis)
                if (plane.normal[axis] < 0)
                    plane.signbits |= 1u << axis;
            planes.push_back(plane);
        }

        void Planes(const src::GfxWorld& w, const src::clipMap_t& c)
        {
            for (unsigned i = 0; i < c.numBrushSides; ++i)
                AddPlane(c.brushsides[i].plane);
            for (unsigned i = 0; i < c.numNodes; ++i)
                AddPlane(c.nodes[i].plane);
            for (int i = 0; i < w.nodeCount; ++i)
                if (w.nodes[i].unknownNodeType == -1)
                    AddPlane(w.nodes[i].data.branch.plane);
        }

        void Partition(const src::GfxWorld& w)
        {
            std::unordered_set<const src::GfxBspNode*> visited;
            const auto visit = [&](auto&& self, const src::GfxBspNode* node, size_t depth) -> void
            {
                Index(w.nodes, w.nodeCount, node);
                Require(depth < 1024 && visited.insert(node).second, "Invalid T4 FF204 BSP node graph");
                if (node->unknownNodeType != -1)
                {
                    Require(node->cellIndex >= -1 && node->cellIndex < w.cellCount, "Invalid T4 FF204 BSP leaf cell");
                    partition.push_back(static_cast<unsigned short>(node->cellIndex + 1));
                    return;
                }
                const auto code = planeIndices.at(node->data.branch.plane) + w.cellCount + 1;
                Require(code <= UINT16_MAX, "T4 FF204 partition plane exceeds compact format");
                const auto start = partition.size();
                partition.push_back(static_cast<unsigned short>(code));
                partition.push_back(0);
                self(self, node->data.branch.children[0], depth + 1);
                const auto offset = partition.size() - start;
                Require(offset <= UINT16_MAX, "T4 FF204 partition child offset exceeds compact format");
                partition[start + 1] = static_cast<unsigned short>(offset);
                self(self, node->data.branch.children[1], depth + 1);
            };
            if (w.nodeCount)
                visit(visit, w.nodes, 0);
            world.nodes = partition.data();
            world.nodeCount = static_cast<int>(partition.size());
        }

        std::unordered_map<const view::GfxImage*, const src::GfxImage*> images;

        template<class D, class S> D* Plain(const S* source, size_t count)
        {
            static_assert(sizeof(D) == sizeof(S));
            static_assert(std::is_trivially_copyable_v<D> && std::is_trivially_copyable_v<S>);
            Require(count <= INT_MAX && (!count || source), "Invalid T4 FF204 input array");
            if (!count)
                return nullptr;
            auto* result = memory.Alloc<D>(count);
            std::memcpy(result, source, count * sizeof(D));
            return result;
        }

        template<class D, class S, class F> D* Convert(const S* source, size_t count, F convert)
        {
            Require(count <= INT_MAX && (!count || source), "Invalid T4 FF204 input array");
            if (!count)
                return nullptr;
            auto* result = memory.Alloc<D>(count);
            for (size_t i = 0; i < count; ++i)
                convert(result[i], source[i]);
            return result;
        }

        template<class D, class S> D* Named(const S* source)
        {
            if (!source)
                return nullptr;
            auto* result = memory.Alloc<D>();
            result->name = source->name;
            return result;
        }

        view::XModel* Model(const src::XModel* source)
        {
            if (!source)
                return nullptr;
            if (const auto found = models.find(source); found != models.end())
                return found->second;
            auto* result = Named<view::XModel>(source);
            models[source] = result;
            return result;
        }

        view::GfxImage* Image(const src::GfxImage* source)
        {
            Require(source, "Missing T4 FF204 lighting image");
            auto* result = Named<view::GfxImage>(source);
            images.emplace(result, source);
            return result;
        }

        void Tree(view::GfxAabbTree& target, const src::GfxAabbTree& source, std::unordered_set<const src::GfxAabbTree*>& visited, size_t depth)
        {
            Require(depth < 1024 && visited.insert(&source).second, "Cyclic or repeated T4 FF204 cell tree node");
            Require(source.surfaceCount >= 0 && source.smodelIndexCount >= 0 && source.childCount >= 0, "Negative T4 FF204 cell tree count");
            std::memcpy(target.mins, source.mins, sizeof(target.mins));
            std::memcpy(target.maxs, source.maxs, sizeof(target.maxs));
            target.startSurfIndex = source.surfaceCount ? source.startSurfIndex : 0;
            target.surfaceCount = source.surfaceCount;
            target.smodelIndexCount = source.smodelIndexCount;
            target.smodelIndexes = Plain<int>(source.smodelIndexes, source.smodelIndexCount);
            target.childCount = source.childCount;
            Require(!source.childCount || source.children, "Missing T4 FF204 cell tree children");
            if (source.childCount)
            {
                target.children = memory.Alloc<view::GfxAabbTree>(source.childCount);
                for (int i = 0; i < source.childCount; ++i)
                    Tree(target.children[i], source.children[i], visited, depth + 1);
            }
        }

        void Clip(const src::clipMap_t& s)
        {
            clip.name = s.name;
            recovery.source["dynamicEntityRuntimeCounts"] = s.dynEntCount;
#define COUNT_COPY(field, count, type)                                                                                                                         \
    clip.count = s.count;                                                                                                                                      \
    clip.field = Plain<view::type>(s.field, s.count)
            clip.planeCount = static_cast<int>(planes.size());
            clip.planes = Plain<view::cplane_s>(planes.data(), planes.size());
            COUNT_COPY(materials, numMaterials, dmaterial_t);
            COUNT_COPY(leafs, numLeafs, cLeaf_t);
            COUNT_COPY(leafbrushNodes, leafbrushNodesCount, cLeafBrushNode_s);
            COUNT_COPY(borders, borderCount, CollisionBorder);
            COUNT_COPY(aabbTrees, aabbTreeCount, CollisionAabbTree);
            COUNT_COPY(cmodels, numSubModels, cmodel_t);
#undef COUNT_COPY
            for (unsigned i = 0; i < s.numMaterials; ++i)
                recovery.source["materials"].push_back({s.materials[i].material, s.materials[i].surfaceFlags, s.materials[i].contentFlags});
            clip.numBrushEdges = s.numBrushEdges;
            clip.brushEdges = s.brushEdges;
            clip.numLeafBrushes = s.numLeafBrushes;
            clip.leafbrushes = s.leafbrushes;
            clip.numLeafSurfaces = s.numLeafSurfaces;
            clip.leafsurfaces = s.leafsurfaces;
            clip.vertCount = s.vertCount;
            clip.verts = s.verts;
            clip.triCount = s.triCount;
            clip.triIndices = s.triIndices;
            clip.triEdgeIsWalkable = s.triEdgeIsWalkable;
            clip.numClusters = s.numClusters;
            clip.clusterBytes = s.clusterBytes;
            clip.visibility = s.visibility;
            clip.numBrushSides = s.numBrushSides;
            clip.brushsides = Convert<view::cbrushside_t>(s.brushsides,
                                                          s.numBrushSides,
                                                          [&](auto& a, const auto& b)
                                                          {
                                                              a.plane = clip.planes + planeIndices.at(b.plane);
                                                              a.materialNum = b.materialNum;
                                                              a.firstAdjacentSideOffset = b.firstAdjacentSideOffset;
                                                              a.edgeCount = b.edgeCount;
                                                          });
            clip.numNodes = s.numNodes;
            clip.nodes = Convert<view::cNode_t>(s.nodes,
                                                s.numNodes,
                                                [&](auto& a, const auto& b)
                                                {
                                                    a.plane = clip.planes + planeIndices.at(b.plane);
                                                    std::memcpy(a.children, b.children, sizeof(a.children));
                                                });
            clip.partitionCount = s.partitionCount;
            clip.partitions = Convert<view::CollisionPartition>(s.partitions,
                                                                s.partitionCount,
                                                                [&](auto& a, const auto& b)
                                                                {
                                                                    a.triCount = b.triCount;
                                                                    a.borderCount = b.borderCount;
                                                                    a.firstTri = b.firstTri;
                                                                    a.borders =
                                                                        b.borderCount ? clip.borders + Index(s.borders, s.borderCount, b.borders) : nullptr;
                                                                });
            clip.numBrushes = s.numBrushes;
            clip.brushes = Convert<view::cbrush_t>(s.brushes,
                                                   s.numBrushes,
                                                   [&](auto& a, const auto& b)
                                                   {
                                                       std::memcpy(a.mins, b.mins, sizeof(a.mins));
                                                       std::memcpy(a.maxs, b.maxs, sizeof(a.maxs));
                                                       a.contents = b.contents;
                                                       a.numsides = b.numsides;
                                                       a.sides = b.numsides ? clip.brushsides + Index(s.brushsides, s.numBrushSides, b.sides) : nullptr;
                                                       std::memcpy(a.axialMaterialNum, b.axialMaterialNum, sizeof(a.axialMaterialNum));
                                                       a.baseAdjacentSide = b.baseAdjacentSide;
                                                       std::memcpy(a.firstAdjacentSideOffsets, b.firstAdjacentSideOffsets, sizeof(a.firstAdjacentSideOffsets));
                                                       std::memcpy(a.edgeCount, b.edgeCount, sizeof(a.edgeCount));
                                                   });
            clip.numStaticModels = s.numStaticModels;
            clip.staticModelList = Convert<view::cStaticModel_s>(s.staticModelList,
                                                                 s.numStaticModels,
                                                                 [&](auto& a, const auto& b)
                                                                 {
                                                                     a.xmodel = Model(b.xmodel);
                                                                     std::memcpy(a.origin, b.origin, sizeof(a.origin));
                                                                     std::memcpy(a.invScaledAxis, b.invScaledAxis, sizeof(a.invScaledAxis));
                                                                     std::memcpy(a.absmin, b.absmin, sizeof(a.absmin));
                                                                     std::memcpy(a.absmax, b.absmax, sizeof(a.absmax));
                                                                 });
            for (unsigned category = 0; category < 2; ++category)
            {
                clip.dynEntCount[category] = s.dynEntCount[category];
                clip.dynEntDefList[category] = Convert<view::DynEntityDef>(s.dynEntDefList[category],
                                                                           s.dynEntCount[category],
                                                                           [&](auto& a, const auto& b)
                                                                           {
                                                                               a.type = static_cast<view::DynEntityType>(b.type);
                                                                               std::memcpy(&a.pose, &b.pose, sizeof(a.pose));
                                                                               a.xModel = Model(b.xModel);
                                                                               a.brushModel = b.brushModel;
                                                                               a.physicsBrushModel = b.physicsBrushModel;
                                                                               a.destroyFx = Named<view::FxEffectDef>(b.destroyFx);
                                                                               a.destroyPieces = Named<view::XModelPieces>(b.destroyPieces);
                                                                               a.physPreset = Named<view::PhysPreset>(b.physPreset);
                                                                               a.health = b.health;
                                                                               std::memcpy(&a.mass, &b.mass, sizeof(a.mass));
                                                                           });
            }
        }

        void World(const src::GfxWorld& s)
        {
            world.name = s.name;
            world.baseName = s.baseName;
            world.planeCount = clip.planeCount;
            world.planes = clip.planes;
            Partition(s);
            world.indexCount = s.indexCount;
            world.indices = s.indices;
            world.vertexCount = s.vertexCount;
            world.vd.vertices = Plain<view::GfxWorldVertex>(s.vd.vertices, s.vertexCount);
            Require(s.vertexLayerDataSize == 0, "T4 FF204 vertex-layer data is unresolved");
            world.vertexLayerDataSize = s.vertexLayerDataSize;
            world.vld.data = s.vld.data;
            world.primaryLightCount = 2;
            world.sunPrimaryLightIndex = 1;
            world.surfaceCount = s.surfaceCount;
            world.surfaces = Convert<view::GfxSurface>(
                s.surfaces,
                s.surfaceCount,
                [&](auto& a, const auto& b)
                {
                    a.tris.vertexLayerData = -1;
                    a.tris.firstVertex = b.tris.firstVertex;
                    Require(b.tris.indexCount && b.tris.indexCount % 3 == 0, "Invalid T4 FF204 surface triangle count");
                    const auto firstIndex = Index(s.indices, s.indexCount, b.tris.indices);
                    Require(b.tris.indexCount <= size_t(s.indexCount) - firstIndex, "T4 FF204 triangle range exceeds its array");
                    const unsigned vertexCount = 1u + *std::max_element(b.tris.indices, b.tris.indices + b.tris.indexCount);
                    Require(vertexCount <= UINT16_MAX && b.tris.firstVertex >= 0 && uint64_t(b.tris.firstVertex) + vertexCount <= s.vertexCount,
                            "T4 FF204 surface vertex range exceeds its array");
                    a.tris.vertexCount = static_cast<unsigned short>(vertexCount);
                    a.tris.indexCount = b.tris.indexCount;
                    a.tris.baseIndex = static_cast<int>(firstIndex);
                    Require(b.material && b.material->info.name, "Missing T4 FF204 surface material");
                    a.material = memory.Alloc<view::Material>();
                    a.material->info.name = b.material->info.name;
                    a.lightmapIndex = b.lightmapIndex;
                    Require(b.reflectionProbeIndex <= s.reflectionProbeCount, "Invalid T4 FF204 surface probe");
                    a.reflectionProbeIndex = b.reflectionProbeIndex < s.reflectionProbeCount ? static_cast<unsigned char>(b.reflectionProbeIndex + 1) : 0;
                    a.primaryLightIndex = b.primaryLightIndex;
                    Require(b.primaryLightIndex == 0, "T4 FF204 nonzero surface/model primary light is unresolved");
                    a.castsSunShadow = b.castsSunShadow;
                    std::memcpy(a.bounds, b.bounds, sizeof(a.bounds));
                });
            world.sortedSurfIndex = s.sortedSurfIndex;
            world.cullGroupCount = s.cullGroupCount;
            world.cullGroups = Plain<view::GfxCullGroup>(s.cullGroups, s.cullGroupCount);
            world.modelCount = s.modelCount;
            world.models = Convert<view::GfxBrushModel>(s.models,
                                                        s.modelCount,
                                                        [](auto& a, const auto& b)
                                                        {
                                                            std::memcpy(a.bounds, b.bounds, sizeof(a.bounds));
                                                            a.surfaceCount = b.surfaceCount;
                                                            a.startSurfIndex = b.startSurfIndex;
                                                        });
            std::memcpy(world.mins, s.mins, sizeof(world.mins));
            std::memcpy(world.maxs, s.maxs, sizeof(world.maxs));
            Require(s.smodelCount >= 0, "Negative T4 FF204 static model count");
            world.smodelCount = s.smodelCount;
            world.smodelDrawInsts = Convert<view::GfxStaticModelDrawInst>(
                s.smodelDrawInsts,
                s.smodelCount,
                [&](auto& a, const auto& b)
                {
                    a.cullDist = b.cullDist;
                    std::memcpy(&a.placement, &b.placement, sizeof(a.placement));
                    a.model = Model(b.model);
                    Require(b.reflectionProbeIndex <= UINT8_MAX, "T4 FF204 static-model probe exceeds PC limit");
                    Require(b.reflectionProbeIndex <= s.reflectionProbeCount, "Invalid T4 FF204 static-model probe");
                    a.reflectionProbeIndex = b.reflectionProbeIndex < s.reflectionProbeCount ? static_cast<unsigned char>(b.reflectionProbeIndex + 1) : 0;
                    recovery.source["staticModelUnknownFlags"].push_back(b.unknown_0x4b);
                });
            world.smodelInsts =
                Convert<view::GfxStaticModelInst>(s.smodelDrawInsts,
                                                  s.smodelCount,
                                                  [&](auto& a, const auto& b)
                                                  {
                                                      std::memcpy(a.mins, b.mins, sizeof(a.mins));
                                                      std::memcpy(a.maxs, b.maxs, sizeof(a.maxs));
                                                      a.groundLighting.packed = 0;
                                                      recovery.source["staticModelUnknownLightingCoordinates"].push_back(b.unknown_0x3c);
                                                      a.primaryLightIndex = b.primaryLightIndex;
                                                      Require(b.primaryLightIndex == 0, "T4 FF204 nonzero surface/model primary light is unresolved");
                                                  });
            world.cellCount = s.cellCount;
            Require(world.cellCount >= 0 && (!world.cellCount || s.cells), "Missing T4 FF204 cells");
            world.cells = memory.Alloc<view::GfxCell>(world.cellCount);
            for (int i = 0; i < world.cellCount; ++i)
            {
                const auto& b = s.cells[i];
                auto& a = world.cells[i];
                std::memcpy(a.mins, b.mins, sizeof(a.mins));
                std::memcpy(a.maxs, b.maxs, sizeof(a.maxs));
                Require(b.aabbTree, "Missing T4 FF204 cell root");
                a.aabbTree = memory.Alloc<view::GfxAabbTree>();
                std::unordered_set<const src::GfxAabbTree*> visited;
                Tree(*a.aabbTree, *b.aabbTree, visited, 0);
                a.cullGroupCount = b.cullGroupCount;
                a.cullGroups = b.cullGroups;
                a.portalCount = b.portalCount;
                a.portals = Convert<view::GfxPortal>(b.portals,
                                                     b.portalCount,
                                                     [&](auto& out, const auto& in)
                                                     {
                                                         out.cell = world.cells + Index(s.cells, world.cellCount, in.cell);
                                                         out.vertexCount = in.vertexCount;
                                                         out.vertices = in.vertices;
                                                         std::memcpy(&out.plane, &in.plane, sizeof(out.plane));
                                                     });
            }
            // FF204 omits the implicit black probe from its source array;
            // references index this array from zero, with count as the no-probe sentinel.
            Require(s.reflectionProbeCount < UINT8_MAX, "T4 FF204 probe count exceeds PC byte index");
            world.reflectionProbeCount = s.reflectionProbeCount + 1;
            Require(!s.reflectionProbeCount || s.reflectionProbes, "Missing T4 FF204 reflection probes");
            world.reflectionProbes = memory.Alloc<view::GfxReflectionProbe>(world.reflectionProbeCount);
            for (unsigned i = 0; i < s.reflectionProbeCount; ++i)
            {
                auto& a = world.reflectionProbes[i + 1];
                const auto& b = s.reflectionProbes[i];
                std::memcpy(a.origin, b.origin, sizeof(a.origin));
                a.reflectionImage = Image(b.reflectionImage);
            }
            recovery.source["reflectionProbeIndexing"] =
                "source array excludes implicit black probe; zero-based source references become one-based, count sentinel becomes black";
            world.lightmapCount = s.lightmapCount;
            world.lightmaps = Convert<view::GfxLightmapArray>(s.lightmaps,
                                                              s.lightmapCount,
                                                              [&](auto& a, const auto& b)
                                                              {
                                                                  // This build keeps the two RGBA halves in separate images.
                                                                  // Join them vertically for the checked PC lightmap view.
                                                                  a.primary = Image(b.unknownImage);
                                                                  a.secondary = Image(b.primary);
                                                                  splitLightmaps.emplace(a.secondary, std::make_pair(b.primary, b.secondary));
                                                              });
            grid = ConvertLightGrid204(s.lightGrid, recovery.source["lightGrid"]);
            world.lightGrid.sunPrimaryLightIndex = 1;
            std::copy(grid.mins.begin(), grid.mins.end(), world.lightGrid.mins);
            std::copy(grid.maxs.begin(), grid.maxs.end(), world.lightGrid.maxs);
            world.lightGrid.rowAxis = 0;
            world.lightGrid.colAxis = 1;
            world.lightGrid.rowDataStart = grid.rows.data();
            world.lightGrid.rawRowDataSize = static_cast<unsigned>(grid.rawRows.size());
            world.lightGrid.rawRowData = grid.rawRows.data();
            world.lightGrid.entryCount = static_cast<unsigned>(grid.entries.size());
            world.lightGrid.entries = grid.entries.data();
            world.lightGrid.colorCount = static_cast<unsigned>(grid.colors.size());
            world.lightGrid.colors = grid.colors.data();
            recovery.source["lightGrid"]["unknownHeaderBytes"] = s.lightGrid.unknown_0x04;
            recovery.source["lightmapRoles"] = "source image 2 is A8 sun visibility; vertically join RGBA images 0 and 1 for the PC secondary atlas";
            // This build predates authored cell probe lists and light regions.
            // The shared writer infers probe candidates from retained geometry.
            recovery.source["hasLightRegions"] = false;
            recovery.source["cellProbeCandidates"] = "inferred from surface/static-model assignments";
            recovery.source["sourceCounts"] = {
                {"surfaces",         s.surfaceCount        },
                {"vertices",         s.vertexCount         },
                {"indices",          s.indexCount          },
                {"layerBytes",       s.vertexLayerDataSize },
                {"cells",            world.cellCount       },
                {"staticModels",     s.smodelCount         },
                {"lightmaps",        s.lightmapCount       },
                {"reflectionProbes", s.reflectionProbeCount}
            };
        }

    public:
        view::GfxWorld world{};
        view::clipMap_t clip{};
        view::ComWorld lights{};
        view::MapEnts entities{};
        view::GameWorldSp paths{};

        Adapter(const src::GfxWorld& w, const src::clipMap_t& c, ISearchPath& search, src::d3dbsp::Recovery& r)
            : searchPath(search),
              recovery(r)
        {
            Planes(w, c);
            Clip(c);
            World(w);
            lights.name = w.name;
            // CoD4/Radiant requires both reserved slots even with no local lights.
            lights.primaryLightCount = 2;
            lights.primaryLights = memory.Alloc<view::ComPrimaryLight>(2);
            auto& sun = lights.primaryLights[1];
            sun.type = 1; // PC directional light; slot zero remains GFX_LIGHT_TYPE_NONE.
            sun.dir[2] = 1;
            if (w.sunLight)
            {
                const auto sourceFloat = [&](size_t offset)
                {
                    uint32_t bits;
                    std::memcpy(&bits, w.sunLight->unknown + offset, sizeof(bits));
                    bits = endianness::FromBigEndian(bits);
                    float result;
                    std::memcpy(&result, &bits, sizeof(result));
                    Require(std::isfinite(result), "Non-finite T4 FF204 sun field");
                    return result;
                };
                float lengthSquared = 0;
                for (unsigned axis = 0; axis < 3; ++axis)
                {
                    // The retained BE runtime record has direction at 4 and
                    // direct-light RGB at 20. Its ambient RGB at 32 is separate.
                    sun.dir[axis] = sourceFloat(4 + axis * 4);
                    sun.color[axis] = sourceFloat(20 + axis * 4);
                    Require(sun.color[axis] >= 0, "Negative T4 FF204 sun color");
                    lengthSquared += sun.dir[axis] * sun.dir[axis];
                }
                Require(std::abs(lengthSquared - 1.0f) < 0.01f, "Invalid T4 FF204 sun direction");
            }
            recovery.source["primaryLights"] = {
                {"count",          2                                                                                                                         },
                {"nullLightIndex", 0                                                                                                                         },
                {"sunLightIndex",  1                                                                                                                         },
                {"direction",      sun.dir                                                                                                                   },
                {"color",          sun.color                                                                                                                 },
                {"source",
                 w.sunLight ? "retained BE sun record: float3 direction at 4, direct RGB at 20" : "compatibility sun slot, zero energy; source record absent"},
                {"unavailable",    "shadow-map flags, radius, cone and motion limits exported as zero"                                                       }
            };
            entities.name = c.name;
            entities.entityString = c.entityString;
            entities.numEntityChars = c.numEntityChars;
            const auto sourceModelCount = CountSourceModels204(c);
            Require(!sourceModelCount || sourceModelCount == static_cast<unsigned>(w.smodelCount),
                    "T4 FF204 retained model-entity count differs from render instances");
            sourceModelsPresent = sourceModelCount > 0;
            recovery.source["modelEntities"] = {
                {"retained",      sourceModelCount                       },
                {"reconstructed", sourceModelsPresent ? 0 : w.smodelCount}
            };
            paths.name = c.name;
            paths.path.nodeCount = c.path.nodeCount;
            Require(c.path.nodeCount <= UINT16_MAX, "T4 FF204 path node count exceeds PC v8 limit");
            paths.path.nodes = Convert<view::pathnode_t>(c.path.nodes,
                                                         c.path.nodeCount,
                                                         [&](auto& a, const auto& b)
                                                         {
                                                             a.constant.totalLinkCount = b.constant.totalLinkCount;
                                                             a.constant.Links = Convert<view::pathlink_s>(b.constant.Links,
                                                                                                          b.constant.totalLinkCount,
                                                                                                          [&](auto& link, const auto& from)
                                                                                                          {
                                                                                                              Require(from.nodeNum < c.path.nodeCount,
                                                                                                                      "T4 FF204 path link exceeds node count");
                                                                                                              link.nodeNum = from.nodeNum;
                                                                                                              link.fDist = from.fDist;
                                                                                                          });
                                                         });
            paths.path.visBytes = c.path.visBytes;
            paths.path.pathVis = c.path.pathVis;
        }

        view::d3dbsp::SourceOverrides Overrides()
        {
            view::d3dbsp::SourceOverrides options;
            options.reflectionCurvePrefix = "oat_t4_204";
            options.omitLightRegions = true;
            options.staticModelEntitiesAlreadyPresent = sourceModelsPresent;
            options.bigEndianLightGridRows = false;
            options.loadImage = [this](const auto* image, auto type)
            {
                if (const auto found = splitLightmaps.find(image); found != splitLightmaps.end())
                {
                    auto first = image::xenon::LoadImage(found->second.first, type, searchPath);
                    auto second = image::xenon::LoadImage(found->second.second, type, searchPath);
                    Require(first && second && type == image::TextureType::T_2D, "Cannot decode T4 FF204 split lightmaps");
                    Require(first->GetFormat()->GetId() == image::ImageFormatId::B8_G8_R8_A8
                                && second->GetFormat()->GetId() == image::ImageFormatId::B8_G8_R8_A8 && first->GetWidth() == second->GetWidth()
                                && first->GetHeight() == second->GetHeight(),
                            "T4 FF204 split lightmap formats or dimensions disagree");
                    auto combined = std::make_unique<image::Texture2D>(first->GetFormat(), first->GetWidth(), first->GetHeight() * 2);
                    combined->Allocate();
                    const auto bytes = first->GetSizeOfMipLevel(0);
                    std::memcpy(combined->GetBufferForMipLevel(0, 0), first->GetBufferForMipLevel(0), bytes);
                    std::memcpy(combined->GetBufferForMipLevel(0, 0) + bytes, second->GetBufferForMipLevel(0), bytes);
                    return std::unique_ptr<image::Texture>(std::move(combined));
                }
                return image::xenon::LoadImage(images.at(image), type, searchPath);
            };
            return options;
        }
    };
} // namespace

std::expected<void, std::string>
    T4Xenon204::d3dbsp::Write(std::ostream& stream, const GfxWorld& world, const clipMap_t& clipMap, ISearchPath& searchPath, Recovery& recovery)
{
    try
    {
        recovery = {};
        Adapter adapter(world, clipMap, searchPath, recovery);
        return IW3Xenon253::d3dbsp::Write(
            stream, adapter.world, adapter.clip, adapter.lights, adapter.entities, &adapter.paths, searchPath, &recovery.bsp, adapter.Overrides());
    }
    catch (const std::exception& error)
    {
        return std::unexpected(error.what());
    }
}
