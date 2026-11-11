#include "D3DBspWriterT4Xenon302.h"

#include "Image/XenonImageToCommonConverter.h"
#include "Utils/MemoryManager.h"

#include <algorithm>
#include <climits>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace
{
    namespace src = T4Xenon302;
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
                "T4 302 array reference is out of range");
        return (address - start) / sizeof(T);
    }

    class Adapter
    {
        MemoryManager memory;
        ISearchPath& searchPath;
        src::d3dbsp::Recovery& recovery;
        std::unordered_map<const src::XModel*, view::XModel*> models;
        std::unordered_map<const view::GfxImage*, const src::GfxImage*> images;

        template<class D, class S> D* Plain(const S* source, size_t count)
        {
            static_assert(sizeof(D) == sizeof(S));
            static_assert(std::is_trivially_copyable_v<D> && std::is_trivially_copyable_v<S>);
            Require(count <= INT_MAX && (!count || source), "Invalid T4 302 input array");
            if (!count)
                return nullptr;
            auto* result = memory.Alloc<D>(count);
            std::memcpy(result, source, count * sizeof(D));
            return result;
        }

        template<class D, class S, class F> D* Convert(const S* source, size_t count, F convert)
        {
            Require(count <= INT_MAX && (!count || source), "Invalid T4 302 input array");
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
            result->physPreset = Named<view::PhysPreset>(source->physPreset);
            models[source] = result;
            return result;
        }

        view::GfxImage* Image(const src::GfxImage* source)
        {
            Require(source, "Missing T4 302 lighting image");
            auto* result = Named<view::GfxImage>(source);
            images.emplace(result, source);
            return result;
        }

        void Tree(view::GfxAabbTree& target, const src::GfxAabbTree& source, std::unordered_set<const src::GfxAabbTree*>& visited, size_t depth)
        {
            Require(depth < 1024 && visited.insert(&source).second, "Cyclic or repeated T4 302 cell tree node");
            Require(source.surfaceCount >= 0 && source.smodelIndexCount >= 0 && source.childCount >= 0, "Negative T4 302 cell tree count");
            std::memcpy(target.mins, source.mins, sizeof(target.mins));
            std::memcpy(target.maxs, source.maxs, sizeof(target.maxs));
            target.startSurfIndex = source.surfaceCount ? source.startSurfIndex : 0;
            target.surfaceCount = source.surfaceCount;
            target.smodelIndexCount = source.smodelIndexCount;
            target.smodelIndexes = Plain<int>(source.smodelIndexes, source.smodelIndexCount);
            target.childCount = source.childCount;
            Require(!source.childCount || source.children, "Missing T4 302 cell tree children");
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
            COUNT_COPY(planes, planeCount, cplane_s);
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
                                                              a.plane = clip.planes + Index(s.planes, s.planeCount, b.plane);
                                                              a.materialNum = b.materialNum;
                                                              a.firstAdjacentSideOffset = b.firstAdjacentSideOffset;
                                                              a.edgeCount = b.edgeCount;
                                                          });
            clip.numNodes = s.numNodes;
            clip.nodes = Convert<view::cNode_t>(s.nodes,
                                                s.numNodes,
                                                [&](auto& a, const auto& b)
                                                {
                                                    a.plane = clip.planes + Index(s.planes, s.planeCount, b.plane);
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
            world.planeCount = s.planeCount;
            world.planes = Plain<view::cplane_s>(s.planes, s.planeCount);
            world.nodeCount = s.nodeCount;
            world.nodes = s.nodes;
            world.indexCount = s.indexCount;
            world.indices = s.indices;
            world.vertexCount = s.vertexCount;
            world.vd.vertices = Plain<view::GfxWorldVertex>(s.vd.vertices, s.vertexCount);
            world.vertexLayerDataSize = s.vertexLayerDataSize;
            world.vld.data = s.vld.data;
            world.primaryLightCount = s.primaryLightCount;
            world.sunPrimaryLightIndex = s.sunPrimaryLightIndex;
            world.surfaceCount = s.surfaceCount;
            world.surfaces = Convert<view::GfxSurface>(s.surfaces,
                                                       s.surfaceCount,
                                                       [&](auto& a, const auto& b)
                                                       {
                                                           a.tris.vertexLayerData = b.tris.vertexLayerData;
                                                           a.tris.firstVertex = b.tris.firstVertex;
                                                           a.tris.vertexCount = b.tris.vertexCount;
                                                           a.tris.indexCount = b.tris.indexCount;
                                                           a.tris.baseIndex = b.tris.baseIndex;
                                                           Require(b.material && b.material->info.name, "Missing T4 302 surface material");
                                                           a.material = memory.Alloc<view::Material>();
                                                           a.material->info.name = b.material->info.name;
                                                           a.lightmapIndex = b.lightmapIndex;
                                                           a.reflectionProbeIndex = b.reflectionProbeIndex;
                                                           a.primaryLightIndex = b.primaryLightIndex;
                                                           a.castsSunShadow = b.castsSunShadow;
                                                           std::memcpy(a.bounds, b.bounds, sizeof(a.bounds));
                                                       });
            world.sortedSurfIndex = s.sortedSurfIndex;
            world.cullGroupCount = s.cullGroupCount;
            world.cullGroups = Plain<view::GfxCullGroup>(s.cullGroups, s.cullGroupCount);
            world.modelCount = s.modelCount;
            world.models = Plain<view::GfxBrushModel>(s.models, s.modelCount);
            std::memcpy(world.mins, s.mins, sizeof(world.mins));
            std::memcpy(world.maxs, s.maxs, sizeof(world.maxs));
            Require(s.smodelCount >= 0, "Negative T4 302 static model count");
            world.smodelCount = s.smodelCount;
            world.smodelDrawInsts =
                Convert<view::GfxStaticModelDrawInst>(s.smodelDrawInsts,
                                                      s.smodelCount,
                                                      [&](auto& a, const auto& b)
                                                      {
                                                          a.cullDist = b.cullDist;
                                                          std::memcpy(&a.placement, &b.placement, sizeof(a.placement));
                                                          a.model = Model(b.model);
                                                          Require(b.reflectionProbeIndex <= UINT8_MAX, "T4 302 static-model probe exceeds PC limit");
                                                          a.reflectionProbeIndex = static_cast<unsigned char>(b.reflectionProbeIndex);
                                                          recovery.source["staticModelUnknownFlags"].push_back(b.unknown_0x4b);
                                                      });
            world.smodelInsts = Convert<view::GfxStaticModelInst>(s.smodelDrawInsts,
                                                                  s.smodelCount,
                                                                  [&](auto& a, const auto& b)
                                                                  {
                                                                      std::memcpy(a.mins, b.mins, sizeof(a.mins));
                                                                      std::memcpy(a.maxs, b.maxs, sizeof(a.maxs));
                                                                      a.groundLighting.packed = b.groundLighting.packed;
                                                                      a.primaryLightIndex = b.primaryLightIndex;
                                                                  });
            world.cellCount = s.cellCount;
            Require(world.cellCount >= 0 && (!world.cellCount || s.cells), "Missing T4 302 cells");
            world.cells = memory.Alloc<view::GfxCell>(world.cellCount);
            for (int i = 0; i < world.cellCount; ++i)
            {
                const auto& b = s.cells[i];
                auto& a = world.cells[i];
                std::memcpy(a.mins, b.mins, sizeof(a.mins));
                std::memcpy(a.maxs, b.maxs, sizeof(a.maxs));
                Require(b.aabbTree, "Missing T4 302 cell root");
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
            world.reflectionProbeCount = s.reflectionProbeCount;
            world.reflectionProbes = Convert<view::GfxReflectionProbe>(s.reflectionProbes,
                                                                       s.reflectionProbeCount,
                                                                       [&](auto& a, const auto& b)
                                                                       {
                                                                           std::memcpy(a.origin, b.origin, sizeof(a.origin));
                                                                           a.reflectionImage = Image(b.reflectionImage);
                                                                       });
            world.lightmapCount = s.lightmapCount;
            world.lightmaps = Convert<view::GfxLightmapArray>(s.lightmaps,
                                                              s.lightmapCount,
                                                              [&](auto& a, const auto& b)
                                                              {
                                                                  a.primary = Image(b.primary);
                                                                  a.secondary = Image(b.secondary);
                                                              });
            const auto& g = s.lightGrid;
            auto& grid = world.lightGrid;
            grid.sunPrimaryLightIndex = g.sunPrimaryLightIndex;
            std::memcpy(grid.mins, g.mins, sizeof(grid.mins));
            std::memcpy(grid.maxs, g.maxs, sizeof(grid.maxs));
            grid.rowAxis = g.rowAxis;
            grid.colAxis = g.colAxis;
            grid.rowDataStart = g.rowDataStart;
            grid.rawRowDataSize = g.rawRowDataSize;
            grid.rawRowData = g.rawRowData;
            grid.entryCount = g.entryCount;
            grid.entries = Plain<view::GfxLightGridEntry>(g.entries, g.entryCount);
            grid.colorCount = g.colorCount;
            grid.colors = Plain<view::GfxLightGridColors>(g.colors, g.colorCount);
            // This T4 build predates authored cell probe lists and light regions.
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

        Adapter(const src::GfxWorld& w, const src::clipMap_t& c, const src::ComWorld& l, ISearchPath& search, src::d3dbsp::Recovery& r)
            : searchPath(search),
              recovery(r)
        {
            Clip(c);
            World(w);
            recovery.source["sourcePrimaryLightCounts"] = {
                {"gfxWorld", w.primaryLightCount},
                {"comWorld", l.primaryLightCount}
            };
            lights.name = l.name;
            lights.primaryLightCount = l.primaryLightCount;
            lights.primaryLights = Convert<view::ComPrimaryLight>(l.primaryLights,
                                                                  l.primaryLightCount,
                                                                  [](auto& a, const auto& b)
                                                                  {
                                                                      Require(b.type <= UINT8_MAX, "Invalid T4 302 common light type");
                                                                      a.type = static_cast<unsigned char>(b.type);
                                                                      std::memcpy(a.color, b.color, sizeof(a.color));
                                                                      std::memcpy(a.dir, b.dir, sizeof(a.dir));
                                                                      std::memcpy(a.origin, b.origin, sizeof(a.origin));
                                                                      a.radius = b.radius;
                                                                      a.cosHalfFovOuter = b.cosHalfFovOuter;
                                                                      a.cosHalfFovInner = b.cosHalfFovInner;
                                                                      a.exponent = b.exponent;
                                                                      a.rotationLimit = b.rotationLimit;
                                                                      a.translationLimit = b.translationLimit;
                                                                      a.defName = b.defName;
                                                                  });
            // Four supplied maps have an empty ComWorld but retain the primary
            // lights used by their surfaces/grid in GfxWorld. Recover those
            // records rather than emitting dangling primary-light indices.
            if (!l.primaryLightCount && w.primaryLightCount)
            {
                lights.primaryLightCount = w.primaryLightCount;
                lights.primaryLights = Convert<view::ComPrimaryLight>(w.primaryLights,
                                                                      w.primaryLightCount,
                                                                      [](auto& a, const auto& b)
                                                                      {
                                                                          Require(b.type <= UINT8_MAX, "Invalid T4 302 light type");
                                                                          a.type = static_cast<unsigned char>(b.type);
                                                                          std::memcpy(a.color, b.color, sizeof(a.color));
                                                                          std::memcpy(a.dir, b.dir, sizeof(a.dir));
                                                                          std::memcpy(a.origin, b.origin, sizeof(a.origin));
                                                                          a.radius = b.radius;
                                                                          a.cosHalfFovOuter = b.cosHalfFovOuter;
                                                                          a.cosHalfFovInner = b.cosHalfFovInner;
                                                                          a.exponent = b.exponent;
                                                                          a.defName = b.def ? b.def->name : "";
                                                                      });
                recovery.source["primaryLightFallback"] = "GfxWorld lights; shadow-map flag and motion limits unavailable, exported as zero";
            }
            entities.name = c.name;
            entities.entityString = c.entityString;
            entities.numEntityChars = c.numEntityChars;
            paths.name = c.name;
            paths.path.nodeCount = c.path.nodeCount;
            Require(c.path.nodeCount <= UINT16_MAX, "T4 302 path node count exceeds PC v8 limit");
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
                                                                                                                      "T4 302 path link exceeds node count");
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
            options.reflectionCurvePrefix = "oat_t4_302";
            options.omitLightRegions = true;
            options.loadImage = [this](const auto* image, auto type)
            {
                return image::xenon::LoadImage(images.at(image), type, searchPath);
            };
            return options;
        }
    };
} // namespace

std::expected<void, std::string> T4Xenon302::d3dbsp::Write(
    std::ostream& stream, const GfxWorld& world, const clipMap_t& clipMap, const ComWorld& comWorld, ISearchPath& searchPath, Recovery& recovery)
{
    try
    {
        recovery = {};
        Adapter adapter(world, clipMap, comWorld, searchPath, recovery);
        return IW3Xenon253::d3dbsp::Write(
            stream, adapter.world, adapter.clip, adapter.lights, adapter.entities, &adapter.paths, searchPath, &recovery.bsp, adapter.Overrides());
    }
    catch (const std::exception& error)
    {
        return std::unexpected(error.what());
    }
}
