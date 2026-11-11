#include "D3DBspWriterIW3Xenon253.h"

#include "Game/IW3/Maps/D3DBspCommonIW3.h"
#include "Image/Compression/ImageDecompressor.h"
#include "Image/TextureConverter.h"
#include "Image/XenonImageToCommonConverter.h"
#include "Utils/Pack.h"
#include "Utils/StreamUtils.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace IW3Xenon253;

namespace
{
    using enum IW3::d3dbsp::LumpType;
    using LeafBrush = uint16_t;
    using cbrushedge_t = uint8_t;
    using vec3_t = float[3];

    struct BspLump
    {
        IW3::d3dbsp::LumpType id;
        std::vector<std::byte> data;
    };

    struct IntBounds
    {
        int32_t mins[3];
        int32_t maxs[3];
    };

    constexpr auto REFLECTION_PROBE_RAW_DATA_SIZE = 0x1FFF8uz;
    constexpr auto LIGHTMAP_PRIMARY_RAW_PAGE_SIZE = 0x100000uz;
    constexpr auto LIGHTMAP_SECONDARY_RAW_PAGE_SIZE = 0x200000uz;
    constexpr auto LIGHTMAP_PRIMARY_RAW_WIDTH = 1024u;
    constexpr auto LIGHTMAP_PRIMARY_RAW_HEIGHT = 1024u;
    constexpr auto LIGHTMAP_SECONDARY_RAW_WIDTH = 512u;
    constexpr auto LIGHTMAP_SECONDARY_RAW_HEIGHT = 1024u;
    constexpr auto LIGHTMAP_SECONDARY_HALF_HEIGHT = LIGHTMAP_SECONDARY_RAW_HEIGHT / 2u;
    constexpr auto LIGHTMAP_SECONDARY_PIXEL_SIZE = 4u;
    constexpr auto SKY_LIGHTMAP_INDEX = 31u;
    constexpr auto PI = 3.14159265358979323846;
    constexpr auto INVALID_STATIC_MODEL_INDEX = std::numeric_limits<size_t>::max();

    struct LightmapPageLayout
    {
        std::vector<unsigned> firstRawPage;
        std::vector<unsigned> pageCount;
        std::vector<unsigned> wideCount;
        std::vector<unsigned> highCount;
        std::vector<std::vector<unsigned>> rawPageForPackedSlot;
        std::vector<std::vector<unsigned>> packedSlotForRawPage;
        unsigned rawPageCount;
    };

    struct SurfaceLightmapRemap
    {
        uint8_t rawLightmapIndex;
        unsigned lightmapIndex;
        unsigned packedSlot;
        unsigned wideCount;
        unsigned highCount;
    };

    struct VertexLightmapRemap
    {
        unsigned packedSlot;
        unsigned wideCount;
        unsigned highCount;
        bool valid;
    };

    [[nodiscard]] size_t PositiveCount(const int value)
    {
        return value > 0 ? static_cast<size_t>(value) : 0uz;
    }

    [[nodiscard]] size_t PositiveCount(const unsigned value)
    {
        return static_cast<size_t>(value);
    }

    [[nodiscard]] std::string_view BaseName(const char* value)
    {
        if (!value)
            return {};

        auto result = std::string_view(value);
        if (result.starts_with(','))
            result.remove_prefix(1);
        for (const auto prefix : {"wc/", "mc/", "w/", "m/"})
        {
            if (result.starts_with(prefix))
            {
                result.remove_prefix(std::strlen(prefix));
                break;
            }
        }

        return result;
    }

    [[nodiscard]] bool MaterialNameMatches(const std::string_view rawName, const std::string_view assetName)
    {
        if (rawName == assetName)
            return true;

        // Runtime default material assets use explicit names, but raw d3dbsp stores the shared "$default" material.
        return rawName == "$default" && (assetName == "$default2d" || assetName == "$default3d");
    }

    template<typename T> void Append(std::vector<std::byte>& out, const T& value)
    {
        const auto* bytes = reinterpret_cast<const std::byte*>(&value);
        out.insert(out.end(), bytes, bytes + sizeof(T));
    }

    [[nodiscard]] uint8_t Byte(const char value)
    {
        return static_cast<uint8_t>(static_cast<unsigned char>(value));
    }

    void AppendBytes(std::vector<std::byte>& out, const void* data, const size_t size)
    {
        if (!data || size == 0)
            return;

        const auto* bytes = static_cast<const std::byte*>(data);
        out.insert(out.end(), bytes, bytes + size);
    }

    [[nodiscard]] std::vector<std::byte> RawBytes(const void* data, const size_t size)
    {
        std::vector<std::byte> out;
        if (data && size > 0)
            out.reserve(size);

        AppendBytes(out, data, size);
        return out;
    }

    void WriteBytesAt(std::vector<std::byte>& out, const size_t offset, const void* data, const size_t size)
    {
        if (!data || size == 0)
            return;

        assert(offset + size <= out.size());
        const auto* bytes = static_cast<const std::byte*>(data);
        std::copy_n(bytes, size, out.data() + offset);
    }

    template<typename T> void WriteAt(std::vector<std::byte>& out, const size_t offset, const T& value)
    {
        WriteBytesAt(out, offset, &value, sizeof(T));
    }

    void MarkMaterialUsedByBrush(std::vector<uint8_t>& usedMaterials, const unsigned materialIndex)
    {
        if (materialIndex < usedMaterials.size())
            usedMaterials[materialIndex] = 1u;
    }

    void MarkMaterialUsedByBrush(std::vector<uint8_t>& usedMaterials, const int materialIndex)
    {
        if (materialIndex >= 0)
            MarkMaterialUsedByBrush(usedMaterials, static_cast<unsigned>(materialIndex));
    }

    [[nodiscard]] std::vector<uint8_t> BuildBrushMaterialUsage(const clipMap_t& clipMap)
    {
        std::vector<uint8_t> usedMaterials(PositiveCount(clipMap.numMaterials), 0u);

        for (auto brushIndex = 0uz; brushIndex < clipMap.numBrushes; brushIndex++)
        {
            const auto& brush = clipMap.brushes[brushIndex];

            for (auto axis = 0uz; axis < 3uz; axis++)
            {
                for (auto side = 0uz; side < 2uz; side++)
                    MarkMaterialUsedByBrush(usedMaterials, brush.axialMaterialNum[side][axis]);
            }

            for (auto sideIndex = 0uz; sideIndex < brush.numsides; sideIndex++)
                MarkMaterialUsedByBrush(usedMaterials, brush.sides[sideIndex].materialNum);
        }

        return usedMaterials;
    }

    [[nodiscard]] int ReconstructRawMaterialContents(const int contents, const bool usedByBrush)
    {
        if (!usedByBrush || contents == 0)
            return contents;

        // linker_pc loads BSP material contents as rawContents & 0xdffffffb.
        // That also strips a raw decal bit (0x4), but the important collision
        // bit here is the raw-only 0x20000000 brush marker. If we write the
        // already-masked runtime value back out, relinking the dumped BSP loses
        // solid/clip contents because brush contents are recomputed from this
        // material table.
        return static_cast<int>(static_cast<unsigned>(contents) | IW3::d3dbsp::RAW_BSP_BRUSH_CONTENT_MARKER);
    }

    [[nodiscard]] const char* NameOf(const XModel* value)
    {
        if (!value || !value->name)
            return nullptr;

        // A leading comma marks a fastfile reference asset. Raw map entity text
        // needs the real xmodel name, otherwise Radiant looks for ",model".
        return value->name[0] == ',' ? &value->name[1] : value->name;
    }

    [[nodiscard]] const char* NameOf(const PhysPreset* value)
    {
        if (!value || !value->name)
            return nullptr;

        return value->name[0] == ',' ? &value->name[1] : value->name;
    }

    template<typename T> [[nodiscard]] bool TryPointerIndex(const T* base, const size_t count, const T* value, size_t& index)
    {
        if (!base || !value || count == 0)
            return false;

        const auto baseAddress = reinterpret_cast<uintptr_t>(base);
        const auto valueAddress = reinterpret_cast<uintptr_t>(value);
        const auto endAddress = baseAddress + count * sizeof(T);

        if (valueAddress < baseAddress || valueAddress >= endAddress)
            return false;

        const auto offset = valueAddress - baseAddress;
        if (offset % sizeof(T) != 0)
            return false;

        index = offset / sizeof(T);
        return true;
    }

    template<typename T> [[nodiscard]] size_t PointerIndex(const T* base, const size_t count, const T* value)
    {
        auto index = 0uz;
        if (!TryPointerIndex(base, count, value, index))
            throw std::runtime_error("BSP reference is outside its owning array");
        return index;
    }

    [[nodiscard]] size_t FindLeafBrushRange(
        const LeafBrush* leafBrushes, const size_t leafBrushCount, const LeafBrush* nodeBrushes, const size_t nodeBrushCount, const size_t searchStart)
    {
        if (!leafBrushes || !nodeBrushes || nodeBrushCount == 0 || nodeBrushCount > leafBrushCount)
            return leafBrushCount;

        const auto matchAt = [leafBrushes, nodeBrushes, nodeBrushCount](const size_t index)
        {
            return std::equal(nodeBrushes, nodeBrushes + nodeBrushCount, leafBrushes + index);
        };

        for (auto i = std::min(searchStart, leafBrushCount); i + nodeBrushCount <= leafBrushCount; i++)
        {
            if (matchAt(i))
                return i;
        }

        // Fall back to a full scan for unusual cases where the raw leafbrush
        // ranges are not monotonic. The normal linker layout is monotonic, so
        // starting at the running cursor avoids picking earlier duplicate
        // one-brush ranges when OAT-built fastfiles contain copied node arrays.
        for (auto i = 0uz; i < std::min(searchStart, leafBrushCount); i++)
        {
            if (i + nodeBrushCount <= leafBrushCount && matchAt(i))
                return i;
        }

        return leafBrushCount;
    }

    struct LeafBrushRange
    {
        size_t first = std::numeric_limits<size_t>::max();
        size_t end = 0uz;
        bool valid = false;
    };

    void IncludeLeafBrushRange(LeafBrushRange& range, const size_t first, const size_t count)
    {
        if (count == 0uz)
            return;

        range.first = std::min(range.first, first);
        range.end = std::max(range.end, first + count);
        range.valid = true;
    }

    [[nodiscard]] bool CollectLeafBrushRange_r(const clipMap_t& clipMap, const size_t nodeIndex, LeafBrushRange& range, const size_t depth = 0uz)
    {
        if (!clipMap.leafbrushNodes || nodeIndex >= clipMap.leafbrushNodesCount || depth > clipMap.leafbrushNodesCount)
            return false;

        const auto& node = clipMap.leafbrushNodes[nodeIndex];
        if (node.leafBrushCount > 0)
        {
            auto brushesIndex = 0uz;
            if (!TryPointerIndex(clipMap.leafbrushes, clipMap.numLeafBrushes, node.data.leaf.brushes, brushesIndex))
                return false;

            IncludeLeafBrushRange(range, brushesIndex, static_cast<size_t>(node.leafBrushCount));
            return true;
        }

        if (node.leafBrushCount < 0 && !CollectLeafBrushRange_r(clipMap, nodeIndex + 1uz, range, depth + 1uz))
            return false;

        for (auto side = 0uz; side < 2uz; side++)
        {
            const auto childOffset = node.data.children.childOffset[side];
            if (childOffset == 0u || !CollectLeafBrushRange_r(clipMap, nodeIndex + childOffset, range, depth + 1uz))
                return false;
        }

        return true;
    }

    [[nodiscard]] bool CollectLeafBrushes_r(const clipMap_t& clipMap, const size_t nodeIndex, std::vector<LeafBrush>& brushes, const size_t depth = 0uz)
    {
        if (!clipMap.leafbrushNodes || nodeIndex >= clipMap.leafbrushNodesCount || depth > clipMap.leafbrushNodesCount)
            return false;

        const auto& node = clipMap.leafbrushNodes[nodeIndex];
        if (node.leafBrushCount > 0)
        {
            if (!node.data.leaf.brushes)
                return false;

            brushes.insert(brushes.end(), node.data.leaf.brushes, node.data.leaf.brushes + node.leafBrushCount);
            return true;
        }

        if (node.leafBrushCount < 0 && !CollectLeafBrushes_r(clipMap, nodeIndex + 1uz, brushes, depth + 1uz))
            return false;

        for (auto side = 0uz; side < 2uz; side++)
        {
            const auto childOffset = node.data.children.childOffset[side];
            if (childOffset == 0u || !CollectLeafBrushes_r(clipMap, nodeIndex + childOffset, brushes, depth + 1uz))
                return false;
        }

        return true;
    }

    [[nodiscard]] std::vector<std::byte> BuildMaterials(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(clipMap.numMaterials) * 72uz);
        const auto brushMaterialUsage = BuildBrushMaterialUsage(clipMap);

        for (auto i = 0uz; i < clipMap.numMaterials; i++)
        {
            const auto& material = clipMap.materials[i];
            const auto contentFlags = ReconstructRawMaterialContents(material.contentFlags, brushMaterialUsage[i] != 0u);
            AppendBytes(out, material.material, sizeof(material.material));
            Append(out, material.surfaceFlags);
            Append(out, contentFlags);
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildPlanes(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;
        out.reserve(PositiveCount(clipMap.planeCount) * 16uz);

        for (auto i = 0uz; i < PositiveCount(clipMap.planeCount); i++)
        {
            const auto& plane = clipMap.planes[i];
            AppendBytes(out, plane.normal, sizeof(plane.normal));
            Append(out, plane.dist);
        }

        return out;
    }

    [[nodiscard]] uint32_t PlaneIndex(const clipMap_t& clipMap, const cplane_s* plane)
    {
        return static_cast<uint32_t>(PointerIndex(clipMap.planes, PositiveCount(clipMap.planeCount), plane));
    }

    [[nodiscard]] std::vector<std::byte> BuildBrushSideData(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;

        for (auto brushIndex = 0uz; brushIndex < clipMap.numBrushes; brushIndex++)
        {
            const auto& brush = clipMap.brushes[brushIndex];

            for (auto axis = 0uz; axis < 3uz; axis++)
            {
                for (auto side = 0uz; side < 2uz; side++)
                {
                    const auto dist = side == 0uz ? brush.mins[axis] : brush.maxs[axis];
                    const auto materialIndex = static_cast<uint32_t>(brush.axialMaterialNum[side][axis]);
                    Append(out, dist);
                    Append(out, materialIndex);
                }
            }

            for (auto sideIndex = 0uz; sideIndex < brush.numsides; sideIndex++)
            {
                const auto& side = brush.sides[sideIndex];
                Append(out, PlaneIndex(clipMap, side.plane));
                Append(out, side.materialNum);
            }
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildBrushEdgeCounts(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;

        for (auto brushIndex = 0uz; brushIndex < clipMap.numBrushes; brushIndex++)
        {
            const auto& brush = clipMap.brushes[brushIndex];

            for (auto axis = 0uz; axis < 3uz; axis++)
            {
                for (auto side = 0uz; side < 2uz; side++)
                    Append(out, brush.edgeCount[side][axis]);
            }

            for (auto sideIndex = 0uz; sideIndex < brush.numsides; sideIndex++)
                Append(out, brush.sides[sideIndex].edgeCount);
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildBrushEdges(const clipMap_t& clipMap)
    {
        return RawBytes(clipMap.brushEdges, static_cast<size_t>(clipMap.numBrushEdges) * sizeof(cbrushedge_t));
    }

    [[nodiscard]] std::vector<std::byte> BuildBrushHeaders(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(clipMap.numBrushes) * 4uz);

        for (auto brushIndex = 0uz; brushIndex < clipMap.numBrushes; brushIndex++)
        {
            const auto& brush = clipMap.brushes[brushIndex];
            const auto sideCount = static_cast<uint16_t>(brush.numsides + 6u);
            const auto materialIndex = static_cast<uint16_t>(brush.axialMaterialNum[0][0]);
            Append(out, sideCount);
            Append(out, materialIndex);
        }

        return out;
    }

    std::vector<int32_t> RecoverLeafCells(const clipMap_t& clipMap, const GfxWorld& world)
    {
        std::vector<int32_t> result(clipMap.numLeafs, -1);
        if (!clipMap.numNodes || !world.nodes || !world.nodeCount)
        {
            if (world.cellCount > 1)
                throw std::runtime_error("Cannot recover multiple cells without the world partition tree");
            for (unsigned i = 0; i < clipMap.numLeafs; ++i)
                result[i] = clipMap.leafs[i].cluster >= 0 ? 0 : -1;
            return result;
        }
        std::vector<bool> visited(clipMap.numNodes);
        const auto visit = [&](auto&& self, int clipNode, size_t worldNode) -> void
        {
            if (worldNode >= PositiveCount(world.nodeCount))
                throw std::runtime_error("World partition node exceeds its buffer");
            const auto code = world.nodes[worldNode];
            const auto terminal = code <= world.cellCount;
            if (clipNode < 0)
            {
                const auto leaf = static_cast<unsigned>(-clipNode - 1);
                if (leaf >= result.size() || !terminal)
                    throw std::runtime_error("Collision and render partition trees disagree at a leaf");
                result[leaf] = static_cast<int32_t>(code) - 1;
                return;
            }
            if (static_cast<unsigned>(clipNode) >= clipMap.numNodes || visited[clipNode])
                throw std::runtime_error("Invalid collision partition tree");
            visited[clipNode] = true;
            const auto& node = clipMap.nodes[clipNode];
            if (terminal)
            {
                self(self, node.children[0], worldNode);
                self(self, node.children[1], worldNode);
                return;
            }
            const auto planeIndex = code - world.cellCount - 1;
            if (planeIndex < 0 || planeIndex >= world.planeCount || !world.planes || worldNode + 1 >= PositiveCount(world.nodeCount))
                throw std::runtime_error("Invalid world partition plane");
            const auto& plane = world.planes[planeIndex];
            if (!node.plane || std::memcmp(plane.normal, node.plane->normal, sizeof(plane.normal)) || plane.dist != node.plane->dist)
                throw std::runtime_error("Collision and render partition planes disagree");
            self(self, node.children[0], worldNode + 2);
            self(self, node.children[1], worldNode + world.nodes[worldNode + 1]);
        };
        visit(visit, 0, 0);
        return result;
    }

    [[nodiscard]] std::vector<std::byte> BuildClipLeafs(const clipMap_t& clipMap, const GfxWorld& world)
    {
        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(clipMap.numLeafs) * 24uz);
        const auto cells = RecoverLeafCells(clipMap, world);

        auto runningFirstLeafBrush = 0;

        for (auto leafIndex = 0uz; leafIndex < clipMap.numLeafs; leafIndex++)
        {
            const auto& leaf = clipMap.leafs[leafIndex];
            const auto* leafBrushNode = leaf.leafBrushNode >= 0 && static_cast<unsigned>(leaf.leafBrushNode) < clipMap.leafbrushNodesCount
                                            ? &clipMap.leafbrushNodes[leaf.leafBrushNode]
                                            : nullptr;
            auto leafBrushCount = 0;
            auto firstLeafBrush = runningFirstLeafBrush;
            if (leafBrushNode)
            {
                auto recoveredRange = LeafBrushRange{};

                // linker_pc partitions each raw leaf brush range into a small
                // runtime tree. The raw BSP still wants the original contiguous
                // range, so flatten terminal leaf nodes back to their min/max
                // span inside clipMap.leafbrushes.
                if (CollectLeafBrushRange_r(clipMap, static_cast<size_t>(leaf.leafBrushNode), recoveredRange) && recoveredRange.valid)
                {
                    firstLeafBrush = static_cast<int32_t>(recoveredRange.first);
                    leafBrushCount = static_cast<int32_t>(recoveredRange.end - recoveredRange.first);
                }
                else
                {
                    std::vector<LeafBrush> recoveredBrushes;
                    if (CollectLeafBrushes_r(clipMap, static_cast<size_t>(leaf.leafBrushNode), recoveredBrushes) && !recoveredBrushes.empty())
                    {
                        const auto brushesIndex = FindLeafBrushRange(clipMap.leafbrushes,
                                                                     clipMap.numLeafBrushes,
                                                                     recoveredBrushes.data(),
                                                                     recoveredBrushes.size(),
                                                                     static_cast<size_t>(runningFirstLeafBrush));

                        if (brushesIndex < clipMap.numLeafBrushes)
                        {
                            firstLeafBrush = static_cast<int32_t>(brushesIndex);
                            leafBrushCount = static_cast<int32_t>(recoveredBrushes.size());
                        }
                    }
                }
            }

            // The raw leaf record carries the cluster/cell assignment even for
            // leaves that also reference brushes. Recomputing this from brush
            // count makes valid source leaves look solid to Radiant.
            const auto cluster = static_cast<int32_t>(leaf.cluster);
            const auto cellIndex = cells[leafIndex];

            Append(out, cluster);
            Append(out, static_cast<int32_t>(leaf.firstCollAabbIndex));
            Append(out, static_cast<int32_t>(leaf.collAabbCount));
            Append(out, firstLeafBrush);
            Append(out, leafBrushCount);
            Append(out, cellIndex);

            runningFirstLeafBrush = firstLeafBrush + leafBrushCount;
        }

        return out;
    }

    [[nodiscard]] int32_t BoundsMinToInt(const float value)
    {
        return static_cast<int32_t>(std::floor(value)) - 8;
    }

    [[nodiscard]] int32_t BoundsMaxToInt(const float value)
    {
        return static_cast<int32_t>(std::ceil(value)) + 8;
    }

    [[nodiscard]] IntBounds RootClipNodeBounds(const clipMap_t& clipMap, const GfxWorld& world)
    {
        const auto* mins = world.modelCount > 0 && world.models          ? world.models[0].bounds[0]
                           : clipMap.numSubModels > 0 && clipMap.cmodels ? clipMap.cmodels[0].mins
                                                                         : nullptr;
        const auto* maxs = world.modelCount > 0 && world.models          ? world.models[0].bounds[1]
                           : clipMap.numSubModels > 0 && clipMap.cmodels ? clipMap.cmodels[0].maxs
                                                                         : nullptr;

        IntBounds result{};
        for (auto axis = 0uz; axis < 3uz; axis++)
        {
            result.mins[axis] = mins ? BoundsMinToInt(mins[axis]) : 0;
            result.maxs[axis] = maxs ? BoundsMaxToInt(maxs[axis]) : 0;
        }

        return result;
    }

    void AssignClipNodeBounds(
        const clipMap_t& clipMap, std::vector<IntBounds>& bounds, std::vector<bool>& visited, const int nodeIndex, const IntBounds& nodeBounds)
    {
        if (nodeIndex < 0 || static_cast<size_t>(nodeIndex) >= bounds.size() || visited[nodeIndex])
            return;

        visited[nodeIndex] = true;
        bounds[nodeIndex] = nodeBounds;

        const auto& node = clipMap.nodes[nodeIndex];
        const auto planeIndex = PointerIndex(clipMap.planes, PositiveCount(clipMap.planeCount), node.plane);
        if (planeIndex >= PositiveCount(clipMap.planeCount))
            return;

        const auto& plane = clipMap.planes[planeIndex];
        auto frontBounds = nodeBounds;
        auto backBounds = nodeBounds;
        if (plane.type < 3 && std::abs(plane.normal[plane.type]) == 1.0f)
        {
            const auto axis = plane.type;
            const auto dist = plane.dist / plane.normal[axis];
            if (plane.normal[axis] > 0)
            {
                frontBounds.mins[axis] = std::max(frontBounds.mins[axis], static_cast<int32_t>(std::floor(dist)));
                backBounds.maxs[axis] = std::min(backBounds.maxs[axis], static_cast<int32_t>(std::ceil(dist)));
            }
            else
            {
                frontBounds.maxs[axis] = std::min(frontBounds.maxs[axis], static_cast<int32_t>(std::ceil(dist)));
                backBounds.mins[axis] = std::max(backBounds.mins[axis], static_cast<int32_t>(std::floor(dist)));
            }
        }

        AssignClipNodeBounds(clipMap, bounds, visited, node.children[0], frontBounds);
        AssignClipNodeBounds(clipMap, bounds, visited, node.children[1], backBounds);
    }

    [[nodiscard]] std::vector<std::byte> BuildClipNodes(const clipMap_t& clipMap, const GfxWorld& world)
    {
        const auto nodeCount = PositiveCount(clipMap.numNodes);
        std::vector<IntBounds> bounds(nodeCount);
        std::vector<bool> visited(nodeCount);

        if (nodeCount > 0)
            AssignClipNodeBounds(clipMap, bounds, visited, 0, RootClipNodeBounds(clipMap, world));

        std::vector<std::byte> out;
        out.reserve(nodeCount * 36uz);

        for (auto nodeIndex = 0uz; nodeIndex < nodeCount; nodeIndex++)
        {
            const auto& node = clipMap.nodes[nodeIndex];
            const auto planeIndex = static_cast<int32_t>(PointerIndex(clipMap.planes, PositiveCount(clipMap.planeCount), node.plane));
            Append(out, planeIndex);
            Append(out, static_cast<int32_t>(node.children[0]));
            Append(out, static_cast<int32_t>(node.children[1]));
            AppendBytes(out, bounds[nodeIndex].mins, sizeof(bounds[nodeIndex].mins));
            AppendBytes(out, bounds[nodeIndex].maxs, sizeof(bounds[nodeIndex].maxs));
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildLeafBrushes(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(clipMap.numLeafBrushes) * sizeof(uint32_t));

        for (auto i = 0uz; i < clipMap.numLeafBrushes; i++)
        {
            const auto value = static_cast<uint32_t>(clipMap.leafbrushes[i]);
            Append(out, value);
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionVerts(const clipMap_t& clipMap)
    {
        return RawBytes(clipMap.verts, static_cast<size_t>(clipMap.vertCount) * sizeof(vec3_t));
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionTriIndices(const clipMap_t& clipMap)
    {
        return RawBytes(clipMap.triIndices, PositiveCount(clipMap.triCount) * 3uz * sizeof(uint16_t));
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionTriEdgeIsWalkable(const clipMap_t& clipMap)
    {
        const auto size = ((PositiveCount(clipMap.triCount) * 3uz + 31uz) / 32uz) * sizeof(uint32_t);
        return RawBytes(clipMap.triEdgeIsWalkable, size);
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionBorders(const clipMap_t& clipMap)
    {
        return RawBytes(clipMap.borders, PositiveCount(clipMap.borderCount) * sizeof(CollisionBorder));
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionPartitions(const clipMap_t& clipMap)
    {
        std::vector<std::byte> out;
        out.reserve(PositiveCount(clipMap.partitionCount) * 12uz);

        for (auto i = 0uz; i < PositiveCount(clipMap.partitionCount); i++)
        {
            const auto& partition = clipMap.partitions[i];
            const uint8_t padding[2]{};
            const auto firstTri = partition.firstTri;
            const auto borderIndex =
                partition.borderCount ? static_cast<uint32_t>(PointerIndex(clipMap.borders, PositiveCount(clipMap.borderCount), partition.borders)) : 0u;

            AppendBytes(out, padding, sizeof(padding));
            Append(out, partition.triCount);
            Append(out, partition.borderCount);
            Append(out, firstTri);
            Append(out, borderIndex);
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildCollisionAabbTrees(const clipMap_t& clipMap)
    {
        return RawBytes(clipMap.aabbTrees, PositiveCount(clipMap.aabbTreeCount) * sizeof(CollisionAabbTree));
    }

    [[nodiscard]] size_t LightGridRowCount(const GfxLightGrid& lightGrid)
    {
        if (lightGrid.rowAxis >= 3u || lightGrid.maxs[lightGrid.rowAxis] < lightGrid.mins[lightGrid.rowAxis])
            return 0uz;

        return static_cast<size_t>(lightGrid.maxs[lightGrid.rowAxis] - lightGrid.mins[lightGrid.rowAxis] + 1u);
    }

    [[nodiscard]] std::vector<std::byte> BuildLightGridHeader(const GfxWorld& world)
    {
        const auto& lightGrid = world.lightGrid;
        std::vector<std::byte> out;

        AppendBytes(out, lightGrid.mins, sizeof(lightGrid.mins));
        AppendBytes(out, lightGrid.maxs, sizeof(lightGrid.maxs));
        Append(out, lightGrid.rowAxis);
        Append(out, lightGrid.colAxis);
        AppendBytes(out, lightGrid.rowDataStart, LightGridRowCount(lightGrid) * sizeof(uint16_t));

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildLightGridEntries(const GfxWorld& world)
    {
        return RawBytes(world.lightGrid.entries, static_cast<size_t>(world.lightGrid.entryCount) * sizeof(GfxLightGridEntry));
    }

    [[nodiscard]] std::vector<std::byte> BuildLightGridColors(const GfxWorld& world)
    {
        std::vector<std::byte> out;
        auto colorCount = static_cast<size_t>(world.lightGrid.colorCount);

        AppendBytes(out, world.lightGrid.colors, colorCount * sizeof(GfxLightGridColors));
        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildLightGridRawRows(const GfxWorld& world, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        const auto& grid = world.lightGrid;
        auto out = RawBytes(grid.rawRowData, grid.rawRowDataSize);
        if (out.empty())
            return out;
        if (!grid.rowDataStart)
            throw std::runtime_error("Missing light-grid row offsets");
        std::unordered_set<size_t> rows;
        for (size_t i = 0; i < LightGridRowCount(grid); ++i)
        {
            if (grid.rowDataStart[i] == 0xffff)
                continue;
            const auto offset = size_t(grid.rowDataStart[i]) * 4;
            if (!rows.insert(offset).second)
                continue;
            if (offset + 12 > out.size())
                throw std::runtime_error("Light-grid row header exceeds its buffer");
            if (overrides.bigEndianLightGridRows)
            {
                for (size_t word = 0; word < 8; word += 2)
                    std::swap(out[offset + word], out[offset + word + 1]);
                std::reverse(out.begin() + offset + 8, out.begin() + offset + 12);
            }
        }
        return out;
    }

    [[nodiscard]] uint16_t SurfaceMaterialIndex(const clipMap_t* clipMap, const GfxSurface& surface)
    {
        const auto* materialName = surface.material && surface.material->info.name ? surface.material->info.name : nullptr;
        const auto materialBaseName = BaseName(materialName);

        if (!clipMap || !clipMap->materials)
            throw std::runtime_error("Missing BSP material table");

        for (auto i = 0uz; i < clipMap->numMaterials; i++)
        {
            const auto rawName = BaseName(clipMap->materials[i].material);
            if (MaterialNameMatches(rawName, materialBaseName))
                return static_cast<uint16_t>(i);
        }

        throw std::runtime_error(std::format("Surface material {} is missing from the BSP material table", materialBaseName));
    }

    [[nodiscard]] uint8_t RawPrimaryLightIndex(const GfxSurface& surface)
    {
        // Sky surfaces in test.d3dbsp store 0 here; the linker normalizes them to the sun primary light at runtime.
        if (static_cast<unsigned char>(surface.lightmapIndex) == SKY_LIGHTMAP_INDEX)
            return 0u;

        return static_cast<uint8_t>(surface.primaryLightIndex);
    }

    struct RawImageData
    {
        std::unique_ptr<image::Texture> texture;
        unsigned width = 0;
        unsigned height = 0;
        size_t size = 0;

        bool IsValid() const
        {
            return texture != nullptr;
        }

        const uint8_t* Data() const
        {
            return texture->GetBufferForMipLevel(0);
        }
    };

    RawImageData ImageDataForImage(const GfxImage* source,
                                   ISearchPath& searchPath,
                                   const image::ImageFormatId format,
                                   const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        if (!source)
            throw std::runtime_error("Missing lightmap image");
        std::string error;
        auto texture = overrides.loadImage ? overrides.loadImage(source, image::TextureType::T_2D)
                                           : image::xenon::LoadImage(source, image::TextureType::T_2D, searchPath, &error);
        if (!texture)
            throw std::runtime_error(std::format("Cannot decode lightmap {}: {}", source->name, error));
        // Xenon may expose the primary's single byte as luminance/red rather
        // than alpha; the BSP stores the same byte with no channel expansion.
        const auto actualFormat = texture->GetFormat()->GetId();
        if (actualFormat != format && !(format == image::ImageFormatId::A8 && actualFormat == image::ImageFormatId::R8))
            throw std::runtime_error(std::format("Unsupported lightmap pixel format for {}", source->name));
        const auto width = texture->GetWidth();
        const auto height = texture->GetHeight();
        const auto size = texture->GetSizeOfMipLevel(0);
        return {std::move(texture), width, height, size};
    }

    [[nodiscard]] unsigned LightmapRawPageCount(const RawImageData& primary, const RawImageData& secondary)
    {
        if (!primary.IsValid() || !secondary.IsValid() || primary.size % LIGHTMAP_PRIMARY_RAW_PAGE_SIZE || secondary.size % LIGHTMAP_SECONDARY_RAW_PAGE_SIZE)
            throw std::runtime_error("Invalid lightmap page storage");
        const auto primaryPages = primary.size / LIGHTMAP_PRIMARY_RAW_PAGE_SIZE;
        const auto secondaryPages = secondary.size / LIGHTMAP_SECONDARY_RAW_PAGE_SIZE;
        if (!primaryPages || primaryPages != secondaryPages)
            throw std::runtime_error("Primary and secondary lightmap page counts disagree");
        return static_cast<unsigned>(primaryPages);
    }

    [[nodiscard]] std::pair<unsigned, unsigned> LightmapAtlasGrid(const RawImageData& primary, const RawImageData& secondary)
    {
        const auto resourcePageCount = LightmapRawPageCount(primary, secondary);
        const auto wideCount = primary.width / LIGHTMAP_PRIMARY_RAW_WIDTH;
        const auto highCount = primary.height / LIGHTMAP_PRIMARY_RAW_HEIGHT;
        if (!wideCount || !highCount || primary.width % LIGHTMAP_PRIMARY_RAW_WIDTH || primary.height % LIGHTMAP_PRIMARY_RAW_HEIGHT
            || wideCount * highCount != resourcePageCount || secondary.width != wideCount * LIGHTMAP_SECONDARY_RAW_WIDTH
            || secondary.height != highCount * LIGHTMAP_SECONDARY_RAW_HEIGHT)
            throw std::runtime_error("Invalid lightmap atlas dimensions");

        return std::make_pair(wideCount, highCount);
    }

    [[nodiscard]] std::vector<unsigned> LinkerRawPageOrder(const unsigned pageCount)
    {
        std::vector<unsigned> result;
        result.reserve(pageCount);

        if (pageCount >= 2u)
        {
            // The linker's packing pass seeds each multi-page group with the selected pair in this order.
            // For a two-page runtime lightmap that means raw page 1 is packed into the first atlas slot and raw page 0 into the second.
            result.emplace_back(1u);
            result.emplace_back(0u);
        }
        else if (pageCount == 1u)
        {
            result.emplace_back(0u);
        }

        for (auto pageIndex = 2u; pageIndex < pageCount; pageIndex++)
            result.emplace_back(pageIndex);

        return result;
    }

    [[nodiscard]] std::vector<unsigned> InvertRawPageOrder(const std::vector<unsigned>& rawPageForPackedSlot)
    {
        std::vector<unsigned> result(rawPageForPackedSlot.size());

        for (auto packedSlot = 0uz; packedSlot < rawPageForPackedSlot.size(); packedSlot++)
        {
            const auto rawPage = rawPageForPackedSlot[packedSlot];
            if (rawPage < result.size())
                result[rawPage] = static_cast<unsigned>(packedSlot);
        }

        return result;
    }

    void AppendZeros(std::vector<std::byte>& out, const size_t size)
    {
        out.resize(out.size() + size, std::byte{});
    }

    void AppendPrimaryLightmapRawPage(
        std::vector<std::byte>& out, const RawImageData& primary, const unsigned wideCount, const unsigned highCount, const unsigned packedSlot)
    {
        const auto sourceWidth = primary.width;
        const auto sourceHeight = primary.height;
        const auto slotX = packedSlot % wideCount;
        const auto slotY = packedSlot / wideCount;

        if (sourceWidth < wideCount * LIGHTMAP_PRIMARY_RAW_WIDTH || sourceHeight < highCount * LIGHTMAP_PRIMARY_RAW_HEIGHT)
        {
            throw std::runtime_error("Invalid primary lightmap dimensions");
        }

        const auto sourceStride = static_cast<size_t>(sourceWidth);
        const auto sourceX = static_cast<size_t>(slotX) * LIGHTMAP_PRIMARY_RAW_WIDTH;
        const auto sourceY = static_cast<size_t>(slotY) * LIGHTMAP_PRIMARY_RAW_HEIGHT;

        for (auto row = 0u; row < LIGHTMAP_PRIMARY_RAW_HEIGHT; row++)
        {
            const auto sourceOffset = (sourceY + row) * sourceStride + sourceX;
            if (sourceOffset + LIGHTMAP_PRIMARY_RAW_WIDTH <= primary.size)
                AppendBytes(out, primary.Data() + sourceOffset, LIGHTMAP_PRIMARY_RAW_WIDTH);
            else
                AppendZeros(out, LIGHTMAP_PRIMARY_RAW_WIDTH);
        }
    }

    void AppendSecondaryLightmapRawPage(
        std::vector<std::byte>& out, const RawImageData& secondary, const unsigned wideCount, const unsigned highCount, const unsigned packedSlot)
    {
        const auto sourceWidth = secondary.width;
        const auto sourceHeight = secondary.height;
        const auto slotX = packedSlot % wideCount;
        const auto slotY = packedSlot / wideCount;

        if (sourceWidth < wideCount * LIGHTMAP_SECONDARY_RAW_WIDTH || sourceHeight < highCount * LIGHTMAP_SECONDARY_RAW_HEIGHT)
        {
            throw std::runtime_error("Invalid secondary lightmap dimensions");
        }

        const auto sourceStride = static_cast<size_t>(sourceWidth) * LIGHTMAP_SECONDARY_PIXEL_SIZE;
        const auto sourceX = static_cast<size_t>(slotX) * LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE;

        for (auto row = 0u; row < LIGHTMAP_SECONDARY_HALF_HEIGHT; row++)
        {
            const auto sourceY = static_cast<size_t>(slotY) * LIGHTMAP_SECONDARY_HALF_HEIGHT + row;
            const auto sourceOffset = sourceY * sourceStride + sourceX;
            if (sourceOffset + LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE <= secondary.size)
                AppendBytes(out, secondary.Data() + sourceOffset, LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE);
            else
                AppendZeros(out, LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE);
        }

        for (auto row = 0u; row < LIGHTMAP_SECONDARY_HALF_HEIGHT; row++)
        {
            const auto sourceY = static_cast<size_t>(highCount + slotY) * LIGHTMAP_SECONDARY_HALF_HEIGHT + row;
            const auto sourceOffset = sourceY * sourceStride + sourceX;
            if (sourceOffset + LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE <= secondary.size)
                AppendBytes(out, secondary.Data() + sourceOffset, LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE);
            else
                AppendZeros(out, LIGHTMAP_SECONDARY_RAW_WIDTH * LIGHTMAP_SECONDARY_PIXEL_SIZE);
        }
    }

    [[nodiscard]] LightmapPageLayout
        BuildLightmapPageLayout(const GfxWorld& world, ISearchPath& searchPath, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        LightmapPageLayout layout;
        layout.firstRawPage.reserve(PositiveCount(world.lightmapCount));
        layout.pageCount.reserve(PositiveCount(world.lightmapCount));
        layout.wideCount.reserve(PositiveCount(world.lightmapCount));
        layout.highCount.reserve(PositiveCount(world.lightmapCount));
        layout.rawPageForPackedSlot.reserve(PositiveCount(world.lightmapCount));
        layout.packedSlotForRawPage.reserve(PositiveCount(world.lightmapCount));
        layout.rawPageCount = 0u;

        for (auto lightmapIndex = 0uz; lightmapIndex < PositiveCount(world.lightmapCount); lightmapIndex++)
        {
            const auto primary = ImageDataForImage(world.lightmaps[lightmapIndex].primary, searchPath, image::ImageFormatId::A8, overrides);
            const auto secondary = ImageDataForImage(world.lightmaps[lightmapIndex].secondary, searchPath, image::ImageFormatId::B8_G8_R8_A8, overrides);

            const auto [wideCount, highCount] = LightmapAtlasGrid(primary, secondary);
            const auto pageCount = wideCount * highCount;
            const auto rawPageForPackedSlot = LinkerRawPageOrder(pageCount);
            layout.firstRawPage.emplace_back(layout.rawPageCount);
            layout.pageCount.emplace_back(pageCount);
            layout.wideCount.emplace_back(wideCount);
            layout.highCount.emplace_back(highCount);
            layout.rawPageForPackedSlot.emplace_back(rawPageForPackedSlot);
            layout.packedSlotForRawPage.emplace_back(InvertRawPageOrder(rawPageForPackedSlot));
            layout.rawPageCount += pageCount;
        }

        return layout;
    }

    [[nodiscard]] unsigned SurfacePackedLightmapSlot(const GfxWorld& world, const GfxSurface& surface, const unsigned wideCount, const unsigned highCount)
    {
        if (wideCount * highCount <= 1u || !world.indices || !world.vd.vertices || world.vertexCount == 0u)
            return 0u;

        const auto firstIndex = surface.tris.baseIndex;
        const auto indexCount = static_cast<int>(surface.tris.indexCount);
        if (firstIndex < 0 || indexCount <= 0 || firstIndex + indexCount > world.indexCount)
            return 0u;

        auto minU = std::numeric_limits<float>::max();
        auto maxU = std::numeric_limits<float>::lowest();
        auto minV = std::numeric_limits<float>::max();
        auto maxV = std::numeric_limits<float>::lowest();
        auto foundVertex = false;

        for (auto indexOffset = 0; indexOffset < indexCount; indexOffset++)
        {
            const auto vertexIndex = surface.tris.firstVertex + world.indices[firstIndex + indexOffset];
            if (vertexIndex < 0 || static_cast<unsigned>(vertexIndex) >= world.vertexCount)
                continue;

            const auto u = world.vd.vertices[vertexIndex].lmapCoord[0];
            const auto v = world.vd.vertices[vertexIndex].lmapCoord[1];
            minU = std::min(minU, u);
            maxU = std::max(maxU, u);
            minV = std::min(minV, v);
            maxV = std::max(maxV, v);
            foundVertex = true;
        }

        if (!foundVertex)
            return 0u;

        const auto midpointU = (static_cast<double>(minU) + static_cast<double>(maxU)) * 0.5;
        const auto midpointV = (static_cast<double>(minV) + static_cast<double>(maxV)) * 0.5;
        const auto slotX = std::clamp(static_cast<int>(std::floor(midpointU * wideCount)), 0, static_cast<int>(wideCount - 1u));
        const auto slotY = std::clamp(static_cast<int>(std::floor(midpointV * highCount)), 0, static_cast<int>(highCount - 1u));

        return static_cast<unsigned>(slotY) * wideCount + static_cast<unsigned>(slotX);
    }

    [[nodiscard]] std::vector<SurfaceLightmapRemap> BuildSurfaceLightmapRemaps(const GfxWorld& world, const LightmapPageLayout& layout)
    {
        std::vector<SurfaceLightmapRemap> remaps;
        remaps.reserve(PositiveCount(world.surfaceCount));

        for (auto surfaceIndex = 0uz; surfaceIndex < PositiveCount(world.surfaceCount); surfaceIndex++)
        {
            const auto& surface = world.surfaces[surfaceIndex];
            const auto lightmapIndex = static_cast<unsigned char>(surface.lightmapIndex);
            if (lightmapIndex == SKY_LIGHTMAP_INDEX || lightmapIndex >= layout.pageCount.size())
            {
                remaps.emplace_back(SurfaceLightmapRemap{static_cast<uint8_t>(lightmapIndex), lightmapIndex, 0u, 1u, 1u});
                continue;
            }

            const auto wideCount = layout.wideCount[lightmapIndex];
            const auto highCount = layout.highCount[lightmapIndex];
            const auto packedSlot = SurfacePackedLightmapSlot(world, surface, wideCount, highCount);
            const auto rawPage =
                packedSlot < layout.rawPageForPackedSlot[lightmapIndex].size() ? layout.rawPageForPackedSlot[lightmapIndex][packedSlot] : packedSlot;
            const auto rawLightmapIndex = layout.firstRawPage[lightmapIndex] + rawPage;
            remaps.emplace_back(SurfaceLightmapRemap{static_cast<uint8_t>(rawLightmapIndex), lightmapIndex, packedSlot, wideCount, highCount});
        }

        return remaps;
    }

    [[nodiscard]] std::vector<VertexLightmapRemap> BuildVertexLightmapRemaps(const GfxWorld& world, const std::vector<SurfaceLightmapRemap>& surfaceRemaps)
    {
        std::vector<VertexLightmapRemap> remaps(world.vertexCount, VertexLightmapRemap{0u, 1u, 1u, false});
        if (!world.indices || !world.vd.vertices || !world.surfaces)
            return remaps;

        for (auto surfaceIndex = 0uz; surfaceIndex < PositiveCount(world.surfaceCount) && surfaceIndex < surfaceRemaps.size(); surfaceIndex++)
        {
            const auto& surface = world.surfaces[surfaceIndex];
            const auto lightmapIndex = static_cast<unsigned char>(surface.lightmapIndex);
            if (lightmapIndex == SKY_LIGHTMAP_INDEX)
                continue;

            const auto firstIndex = surface.tris.baseIndex;
            const auto indexCount = static_cast<int>(surface.tris.indexCount);
            if (firstIndex < 0 || indexCount <= 0 || firstIndex + indexCount > world.indexCount)
                continue;

            const auto& surfaceRemap = surfaceRemaps[surfaceIndex];
            for (auto indexOffset = 0; indexOffset < indexCount; indexOffset++)
            {
                const auto vertexIndex = surface.tris.firstVertex + world.indices[firstIndex + indexOffset];
                if (vertexIndex < 0 || static_cast<unsigned>(vertexIndex) >= world.vertexCount)
                    continue;

                auto& remap = remaps[vertexIndex];
                if (remap.valid
                    && (remap.packedSlot != surfaceRemap.packedSlot || remap.wideCount != surfaceRemap.wideCount || remap.highCount != surfaceRemap.highCount))
                    throw std::runtime_error("Shared vertex requires conflicting lightmap remaps");
                remap = VertexLightmapRemap{surfaceRemap.packedSlot, surfaceRemap.wideCount, surfaceRemap.highCount, true};
            }
        }

        return remaps;
    }

    [[nodiscard]] std::vector<std::byte> BuildSurfaces(const clipMap_t* clipMap, const GfxWorld& world, const std::vector<SurfaceLightmapRemap>& lightmapRemaps)
    {
        std::vector<std::byte> out;
        out.reserve(PositiveCount(world.surfaceCount) * 24uz);

        // Surface order is part of the BSP's index space. Brush models and AABB
        // ranges point into this same list, so reordering here requires remapping
        // every dependent range as well.
        for (auto surfaceIndex = 0uz; surfaceIndex < PositiveCount(world.surfaceCount); surfaceIndex++)
        {
            const auto& surface = world.surfaces[surfaceIndex];
            const auto materialIndex = SurfaceMaterialIndex(clipMap, surface);
            const auto lightmapIndex =
                surfaceIndex < lightmapRemaps.size() ? lightmapRemaps[surfaceIndex].rawLightmapIndex : static_cast<uint8_t>(surface.lightmapIndex);
            const auto reflectionProbeIndex = static_cast<uint8_t>(surface.reflectionProbeIndex);
            const auto primaryLightIndex = RawPrimaryLightIndex(surface);
            const auto flags = static_cast<uint8_t>(surface.castsSunShadow);
            const uint16_t padding = 0u;
            const auto vertexLayerData = surface.tris.vertexLayerData;
            const auto firstVertex = surface.tris.firstVertex;
            const auto vertexCount = surface.tris.vertexCount;
            const auto indexCount = static_cast<uint16_t>(surface.tris.indexCount);
            const auto baseIndex = surface.tris.baseIndex;

            Append(out, materialIndex);
            Append(out, lightmapIndex);
            Append(out, reflectionProbeIndex);
            Append(out, primaryLightIndex);
            Append(out, flags);
            Append(out, padding);
            Append(out, vertexLayerData);
            Append(out, firstVertex);
            Append(out, vertexCount);
            Append(out, indexCount);
            Append(out, baseIndex);
        }

        return out;
    }

    void CrossProduct(const float (&left)[3], const float (&right)[3], float (&out)[3])
    {
        out[0] = left[1] * right[2] - left[2] * right[1];
        out[1] = left[2] * right[0] - left[0] * right[2];
        out[2] = left[0] * right[1] - left[1] * right[0];
    }

    [[nodiscard]] std::vector<std::byte>
        BuildVertices(const GfxWorld& world, const std::vector<VertexLightmapRemap>& lightmapRemaps, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(world.vertexCount) * 68uz);

        for (auto vertexIndex = 0uz; vertexIndex < world.vertexCount; vertexIndex++)
        {
            const auto& vertex = world.vd.vertices[vertexIndex];
            float normal[3]{};
            float tangent[3]{};
            float binormal[3]{};
            float lmapCoord[2]{vertex.lmapCoord[0], vertex.lmapCoord[1]};
            const auto unpack = overrides.unpackUnitVector ? overrides.unpackUnitVector : pack32::Vec3UnpackUnitVecThirdBased;
            unpack(vertex.normal.packed, normal);
            unpack(vertex.tangent.packed, tangent);
            CrossProduct(normal, tangent, binormal);

            for (auto& component : binormal)
                component *= vertex.binormalSign;

            if (vertexIndex < lightmapRemaps.size() && lightmapRemaps[vertexIndex].valid
                && lightmapRemaps[vertexIndex].wideCount * lightmapRemaps[vertexIndex].highCount > 1u)
            {
                const auto& remap = lightmapRemaps[vertexIndex];
                const auto slotX = remap.packedSlot % remap.wideCount;
                const auto slotY = remap.packedSlot / remap.wideCount;
                lmapCoord[0] = lmapCoord[0] * static_cast<float>(remap.wideCount) - static_cast<float>(slotX);
                lmapCoord[1] = lmapCoord[1] * static_cast<float>(remap.highCount) - static_cast<float>(slotY);
            }

            AppendBytes(out, vertex.xyz, sizeof(vertex.xyz));
            AppendBytes(out, normal, sizeof(normal));
            Append(out, vertex.color.packed);
            AppendBytes(out, vertex.texCoord, sizeof(vertex.texCoord));
            AppendBytes(out, lmapCoord, sizeof(lmapCoord));
            AppendBytes(out, tangent, sizeof(tangent));
            AppendBytes(out, binormal, sizeof(binormal));
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildIndices(const GfxWorld& world)
    {
        return RawBytes(world.indices, PositiveCount(world.indexCount) * sizeof(uint16_t));
    }

    [[nodiscard]] std::vector<std::byte> BuildLightmapImages(const GfxWorld& world,
                                                             const LightmapPageLayout& layout,
                                                             ISearchPath& searchPath,
                                                             const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        if (world.lightmapCount <= 0)
            return {};

        std::vector<std::byte> out;
        for (auto lightmapIndex = 0uz; lightmapIndex < PositiveCount(world.lightmapCount); lightmapIndex++)
        {
            const auto primary = ImageDataForImage(world.lightmaps[lightmapIndex].primary, searchPath, image::ImageFormatId::A8, overrides);
            const auto secondary = ImageDataForImage(world.lightmaps[lightmapIndex].secondary, searchPath, image::ImageFormatId::B8_G8_R8_A8, overrides);
            if (!primary.IsValid() || !secondary.IsValid())
                return {};

            const auto pageCount = lightmapIndex < layout.pageCount.size() ? layout.pageCount[lightmapIndex] : 1u;
            const auto wideCount = lightmapIndex < layout.wideCount.size() ? layout.wideCount[lightmapIndex] : 1u;
            const auto highCount = lightmapIndex < layout.highCount.size() ? layout.highCount[lightmapIndex] : pageCount;
            for (auto rawPage = 0u; rawPage < pageCount; rawPage++)
            {
                // Raw d3dbsp stores fixed-height lightmap pages as secondary texture data first, then primary.
                // The linker packs raw pages into a runtime atlas and applies the matching UV scale/offset when loading surfaces.
                const auto packedSlot = lightmapIndex < layout.packedSlotForRawPage.size() && rawPage < layout.packedSlotForRawPage[lightmapIndex].size()
                                            ? layout.packedSlotForRawPage[lightmapIndex][rawPage]
                                            : rawPage;
                AppendSecondaryLightmapRawPage(out, secondary, wideCount, highCount, packedSlot);
                AppendPrimaryLightmapRawPage(out, primary, wideCount, highCount, packedSlot);
            }
        }

        return out;
    }

    [[nodiscard]] std::vector<std::byte> BuildBrushModels(const clipMap_t* clipMap, const GfxWorld& world)
    {
        std::vector<std::byte> out;
        out.reserve(PositiveCount(world.modelCount) * 48uz);

        for (auto modelIndex = 0uz; modelIndex < PositiveCount(world.modelCount); modelIndex++)
        {
            const auto& model = world.models[modelIndex];
            const auto startSurfIndex = static_cast<uint16_t>(model.startSurfIndex);
            const auto surfaceCount = static_cast<uint16_t>(model.surfaceCount);
            const auto surfaceCountNoDecal = surfaceCount;
            const auto firstTriSoup = surfaceCount > 0u ? startSurfIndex : std::numeric_limits<uint16_t>::max();
            const auto firstTriSoupNoDecal = surfaceCountNoDecal > 0u ? startSurfIndex : std::numeric_limits<uint16_t>::max();
            auto firstCollAabbIndex = 0u;
            auto collAabbCount = 0u;
            auto firstBrush = 0u;
            auto brushCount = modelIndex == 0uz && clipMap ? static_cast<uint32_t>(clipMap->numBrushes) : 0u;

            if (clipMap && modelIndex > 0uz && modelIndex < clipMap->numSubModels)
            {
                const auto& cmodel = clipMap->cmodels[modelIndex];
                firstCollAabbIndex = static_cast<uint16_t>(cmodel.leaf.firstCollAabbIndex);
                collAabbCount = static_cast<uint16_t>(cmodel.leaf.collAabbCount);

                // linker_pc derives submodel collision leaves from these raw
                // brush ranges. Runtime clipMap only keeps the partitioned
                // leafbrush tree, so flatten it back to a contiguous range.
                std::vector<LeafBrush> modelBrushes;
                if (cmodel.leaf.leafBrushNode > 0 && CollectLeafBrushes_r(*clipMap, static_cast<size_t>(cmodel.leaf.leafBrushNode), modelBrushes)
                    && !modelBrushes.empty())
                {
                    const auto [minBrush, maxBrush] = std::minmax_element(modelBrushes.begin(), modelBrushes.end());
                    firstBrush = *minBrush;
                    brushCount = static_cast<uint32_t>(*maxBrush - *minBrush + 1u);
                }
            }

            AppendBytes(out, model.bounds[0], sizeof(model.bounds[0]));
            AppendBytes(out, model.bounds[1], sizeof(model.bounds[1]));
            // Raw v22 stores firstTriSoup[2] and triSoupCount[2]. R_LoadSubmodels
            // maps an empty tri-soup range to startSurfIndex 0xffff.
            Append(out, firstTriSoupNoDecal);
            Append(out, firstTriSoup);
            Append(out, surfaceCountNoDecal);
            Append(out, surfaceCount);
            // The render fields above are 16-bit, but the following collision
            // and brush ranges are 32-bit in the raw model record.
            Append(out, firstCollAabbIndex);
            Append(out, collAabbCount);
            Append(out, firstBrush);
            Append(out, brushCount);
        }

        return out;
    }

    [[nodiscard]] std::string FormatFloat(const float value)
    {
        auto result = std::format("{:.9g}", value);
        if (result == "-0")
            return "0";

        return result;
    }

    [[nodiscard]] float StaticModelScale(const cStaticModel_s& staticModel)
    {
        const auto invScale =
            std::sqrt(staticModel.invScaledAxis[0][0] * staticModel.invScaledAxis[0][0] + staticModel.invScaledAxis[0][1] * staticModel.invScaledAxis[0][1]
                      + staticModel.invScaledAxis[0][2] * staticModel.invScaledAxis[0][2]);
        if (invScale <= std::numeric_limits<float>::epsilon())
            return 1.0f;

        return 1.0f / invScale;
    }

    void StaticModelAxis(const cStaticModel_s& staticModel, float (&axis)[3][3])
    {
        const auto scale = StaticModelScale(staticModel);
        for (auto row = 0uz; row < 3uz; row++)
        {
            for (auto column = 0uz; column < 3uz; column++)
                axis[row][column] = staticModel.invScaledAxis[column][row] * scale;
        }
    }

    [[nodiscard]] std::array<float, 3> AxisToAngles(const float (&axis)[3][3])
    {
        const auto forwardLength = std::sqrt(axis[0][0] * axis[0][0] + axis[0][1] * axis[0][1]);
        return {static_cast<float>(std::atan2(-axis[0][2], forwardLength) * 180.0 / PI),
                static_cast<float>(std::atan2(axis[0][1], axis[0][0]) * 180.0 / PI),
                static_cast<float>(std::atan2(axis[1][2], axis[2][2]) * 180.0 / PI)};
    }

    [[nodiscard]] std::array<float, 3> StaticModelAngles(const cStaticModel_s& staticModel)
    {
        float axis[3][3]{};
        StaticModelAxis(staticModel, axis);

        return AxisToAngles(axis);
    }

    void QuatToAxis(const float (&quat)[4], float (&axis)[3][3])
    {
        const auto x = quat[0];
        const auto y = quat[1];
        const auto z = quat[2];
        const auto w = quat[3];
        const auto x2 = x + x;
        const auto y2 = y + y;
        const auto z2 = z + z;
        const auto xx = x * x2;
        const auto xy = x * y2;
        const auto xz = x * z2;
        const auto yy = y * y2;
        const auto yz = y * z2;
        const auto zz = z * z2;
        const auto wx = w * x2;
        const auto wy = w * y2;
        const auto wz = w * z2;

        // dyn_model entities are loaded as angles -> AnglesToAxis -> MatrixToQuat.
        // Rebuild that IW3 axis convention from the runtime quat before writing angles.
        axis[0][0] = 1.0f - yy - zz;
        axis[0][1] = xy + wz;
        axis[0][2] = xz - wy;
        axis[1][0] = xy - wz;
        axis[1][1] = 1.0f - xx - zz;
        axis[1][2] = yz + wx;
        axis[2][0] = xz + wy;
        axis[2][1] = yz - wx;
        axis[2][2] = 1.0f - xx - yy;
    }

    [[nodiscard]] std::array<float, 3> DynEntityAngles(const DynEntityDef& dynEnt)
    {
        float axis[3][3]{};
        QuatToAxis(dynEnt.pose.quat, axis);
        return AxisToAngles(axis);
    }

    [[nodiscard]] std::string_view DynEntityTypeName(const DynEntityType type)
    {
        switch (type)
        {
        case DYNENT_TYPE_CLUTTER:
            return "clutter";

        case DYNENT_TYPE_DESTRUCT:
            return "destruct";

        default:
            return {};
        }
    }

    [[nodiscard]] bool HasNonZeroVector(const float (&value)[3])
    {
        return value[0] != 0.0f || value[1] != 0.0f || value[2] != 0.0f;
    }

    void AppendFloat3Field(std::string& out, const std::string_view fieldName, const float (&value)[3])
    {
        out += std::format("\"{}\" \"{} {} {}\"\n", fieldName, FormatFloat(value[0]), FormatFloat(value[1]), FormatFloat(value[2]));
    }

    [[nodiscard]] bool AlmostEqual(const float a, const float b)
    {
        return std::abs(a - b) <= 0.001f;
    }

    [[nodiscard]] bool StaticModelOriginMatches(const cStaticModel_s& staticModel, const GfxStaticModelDrawInst& drawInst)
    {
        return AlmostEqual(staticModel.origin[0], drawInst.placement.origin[0]) && AlmostEqual(staticModel.origin[1], drawInst.placement.origin[1])
               && AlmostEqual(staticModel.origin[2], drawInst.placement.origin[2]);
    }

    [[nodiscard]] bool StaticModelNameMatches(const cStaticModel_s& staticModel, const GfxStaticModelDrawInst& drawInst)
    {
        const auto* staticModelName = NameOf(staticModel.xmodel);
        const auto* drawModelName = NameOf(drawInst.model);
        return staticModelName && drawModelName && std::strcmp(staticModelName, drawModelName) == 0;
    }

    [[nodiscard]] size_t FindStaticModelDrawIndex(const GfxWorld* world, const cStaticModel_s& staticModel, const std::vector<bool>& usedDrawInsts)
    {
        if (!world || !world->smodelDrawInsts)
            return INVALID_STATIC_MODEL_INDEX;

        for (auto drawIndex = 0uz; drawIndex < PositiveCount(world->smodelCount); drawIndex++)
        {
            if (drawIndex < usedDrawInsts.size() && usedDrawInsts[drawIndex])
                continue;

            const auto& drawInst = world->smodelDrawInsts[drawIndex];
            if (StaticModelNameMatches(staticModel, drawInst) && StaticModelOriginMatches(staticModel, drawInst))
                return drawIndex;
        }

        return INVALID_STATIC_MODEL_INDEX;
    }

    [[nodiscard]] std::string StaticModelGroundLighting(const GfxStaticModelInst& inst)
    {
        // The linker parses gndLt with "%02x%02x%02x%02x%02x" and stores the
        // first four fields into groundLighting as B, G, R, alpha. IW3Xenon253
        // stores the fifth field on GfxStaticModelInst.
        const auto& color = inst.groundLighting.array;
        return std::format("{:02x}{:02x}{:02x}{:02x}{:02x}",
                           static_cast<unsigned>(Byte(color[2])),
                           static_cast<unsigned>(Byte(color[1])),
                           static_cast<unsigned>(Byte(color[0])),
                           static_cast<unsigned>(Byte(color[3])),
                           static_cast<unsigned>(Byte(inst.primaryLightIndex)));
    }

    void AppendStaticModelGroundLighting(std::string& out, const GfxWorld* world, const size_t staticModelIndex)
    {
        if (!world || staticModelIndex == INVALID_STATIC_MODEL_INDEX || !world->smodelInsts || !world->smodelDrawInsts
            || staticModelIndex >= PositiveCount(world->smodelCount))
            return;

        const auto& inst = world->smodelInsts[staticModelIndex];
        // The stock caller only keeps parsed gndLt when the packed ground
        // lighting color is non-zero; otherwise it recomputes model lighting.
        if (inst.groundLighting.packed == 0u)
            return;

        out += std::format("\"gndLt\" \"{}\"\n", StaticModelGroundLighting(inst));
    }

    void AppendStaticModelEntity(std::string& out,
                                 const cStaticModel_s& staticModel,
                                 const GfxWorld* world,
                                 const size_t staticModelIndex,
                                 const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        const auto* modelName = NameOf(staticModel.xmodel);
        if (!modelName || !*modelName)
            return;

        const auto angles = StaticModelAngles(staticModel);
        out += "{\n";
        out += std::format("\"modelscale\" \"{}\"\n", FormatFloat(StaticModelScale(staticModel)));
        out += std::format(
            "\"origin\" \"{} {} {}\"\n", FormatFloat(staticModel.origin[0]), FormatFloat(staticModel.origin[1]), FormatFloat(staticModel.origin[2]));
        out += std::format("\"angles\" \"{} {} {}\"\n", FormatFloat(angles[0]), FormatFloat(angles[1]), FormatFloat(angles[2]));

        AppendStaticModelGroundLighting(out, world, staticModelIndex);
        if (staticModelIndex != INVALID_STATIC_MODEL_INDEX && overrides.staticModelNoCastShadow && overrides.staticModelNoCastShadow(staticModelIndex))
            out += "\"spawnflags\" \"2\"\n";
        out += std::format("\"model\" \"{}\"\n", modelName);
        out += "\"classname\" \"misc_model\"\n";
        out += "}\n";
    }

    void AppendStaticModelEntities(std::string& out, const clipMap_t* clipMap, const GfxWorld* world, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        if (!out.empty() && out.back() != '\n')
            out += '\n';

        std::vector<bool> usedDrawInsts(world ? PositiveCount(world->smodelCount) : 0uz);

        // Static models originate as misc_model entity text in raw d3dbsp, but
        // linking moves placement/collision data into clipMap.staticModelList
        // and lighting/render data into GfxWorld DPVS arrays. The order can
        // differ, so match the DPVS entry by model name and origin.
        for (auto staticModelIndex = 0uz; clipMap && staticModelIndex < clipMap->numStaticModels; staticModelIndex++)
        {
            const auto drawIndex = FindStaticModelDrawIndex(world, clipMap->staticModelList[staticModelIndex], usedDrawInsts);
            if (drawIndex != INVALID_STATIC_MODEL_INDEX && drawIndex < usedDrawInsts.size())
                usedDrawInsts[drawIndex] = true;

            AppendStaticModelEntity(out, clipMap->staticModelList[staticModelIndex], world, drawIndex, overrides);
        }
        // Models without collision have no cStaticModel_s. Recover their
        // placement from the render instances instead of dropping them.
        for (size_t i = 0; world && i < PositiveCount(world->smodelCount); ++i)
        {
            if (usedDrawInsts[i])
                continue;
            const auto& instance = world->smodelDrawInsts[i];
            if (instance.placement.scale <= 0)
                throw std::runtime_error("Invalid static-model scale");
            float axis[3][3];
            if (overrides.staticModelAxis)
                overrides.staticModelAxis(i, axis);
            else
                for (size_t row = 0; row < 3; ++row)
                    pack32::Vec3UnpackUnitVecThirdBased(instance.placement.axis[row].packed, axis[row]);
            const auto angles = AxisToAngles(axis);
            out += "{\n";
            AppendFloat3Field(out, "origin", instance.placement.origin);
            out += std::format("\"angles\" \"{} {} {}\"\n", FormatFloat(angles[0]), FormatFloat(angles[1]), FormatFloat(angles[2]));
            out += std::format("\"modelscale\" \"{}\"\n", FormatFloat(instance.placement.scale));
            AppendStaticModelGroundLighting(out, world, i);
            if (overrides.staticModelNoCastShadow && overrides.staticModelNoCastShadow(i))
                out += "\"spawnflags\" \"2\"\n";
            out += std::format("\"model\" \"{}\"\n\"classname\" \"misc_model\"\n}}\n", NameOf(instance.model));
        }
    }

    void AppendDynEntityMassFields(std::string& out, const DynEntityDef& dynEnt)
    {
        if (HasNonZeroVector(dynEnt.mass.centerOfMass))
            AppendFloat3Field(out, "centerofmass", dynEnt.mass.centerOfMass);

        if (HasNonZeroVector(dynEnt.mass.momentsOfInertia))
            AppendFloat3Field(out, "momofinertia", dynEnt.mass.momentsOfInertia);

        if (HasNonZeroVector(dynEnt.mass.productsOfInertia))
            AppendFloat3Field(out, "prodofinertia", dynEnt.mass.productsOfInertia);
    }

    void AppendDynModelPhysPreset(std::string& out, const DynEntityDef& dynEnt)
    {
        if (!dynEnt.physPreset || (dynEnt.xModel && dynEnt.xModel->physPreset == dynEnt.physPreset))
            return;

        const auto* physPresetName = NameOf(dynEnt.physPreset);
        if (physPresetName && *physPresetName)
            out += std::format("\"physPreset\" \"{}\"\n", physPresetName);
    }

    void AppendDynEntity(std::string& out, const DynEntityDef& dynEnt, const bool brush)
    {
        const auto* modelName = NameOf(dynEnt.xModel);
        const auto typeName = DynEntityTypeName(dynEnt.type);
        if ((!brush && (!modelName || !*modelName)) || typeName.empty())
            throw std::runtime_error("Invalid dynamic entity model or type");

        const auto angles = DynEntityAngles(dynEnt);
        out += "{\n";
        AppendDynModelPhysPreset(out, dynEnt);
        AppendFloat3Field(out, "origin", dynEnt.pose.origin);
        out += std::format("\"type\" \"{}\"\n", typeName);
        out += std::format("\"angles\" \"{} {} {}\"\n", FormatFloat(angles[0]), FormatFloat(angles[1]), FormatFloat(angles[2]));
        if (dynEnt.health != 0)
            out += std::format("\"health\" \"{}\"\n", dynEnt.health);
        AppendDynEntityMassFields(out, dynEnt);
        if (dynEnt.destroyFx && dynEnt.destroyFx->name)
            out += std::format("\"destroyEfx\" \"{}\"\n", dynEnt.destroyFx->name);
        if (dynEnt.destroyPieces && dynEnt.destroyPieces->name)
            out += std::format("\"destroyPieces\" \"{}\"\n", dynEnt.destroyPieces->name);
        if (dynEnt.physicsBrushModel)
            out += std::format("\"physicsmodel\" \"*{}\"\n", dynEnt.physicsBrushModel);
        if (brush)
            out += std::format("\"model\" \"*{}\"\n\"classname\" \"dyn_brushmodel\"\n", dynEnt.brushModel);
        else
            out += std::format("\"model\" \"{}\"\n\"classname\" \"dyn_model\"\n", modelName);
        out += "}\n";
    }

    void AppendDynModelEntities(std::string& out, const clipMap_t* clipMap)
    {
        if (!clipMap)
            return;

        if (!out.empty() && out.back() != '\n')
            out += '\n';

        // dyn_model source entities are consumed into clipMap dynEnt arrays by
        // linker_pc. Reconstruct the source-side entity blocks from the runtime
        // model and brush dynents, including destruction and physics metadata.
        for (size_t type = 0; type < 2; ++type)
            for (size_t i = 0; i < clipMap->dynEntCount[type]; ++i)
                AppendDynEntity(out, clipMap->dynEntDefList[type][i], type == 1);
    }

    [[nodiscard]] std::vector<std::byte>
        BuildEntities(const MapEnts& mapEnts, const clipMap_t* clipMap, const GfxWorld* world, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        auto entityCharCount = PositiveCount(mapEnts.numEntityChars);
        if (entityCharCount > 0uz && mapEnts.entityString && mapEnts.entityString[entityCharCount - 1uz] == '\0')
            entityCharCount--;

        std::string entities;
        if (mapEnts.entityString && entityCharCount > 0uz)
            entities.assign(mapEnts.entityString, entityCharCount);

        if (!overrides.staticModelEntitiesAlreadyPresent)
            AppendStaticModelEntities(entities, clipMap, world, overrides);
        AppendDynModelEntities(entities, clipMap);
        if (!entities.empty() && entities.back() != '\n')
            entities += '\n';

        std::vector<std::byte> out;
        AppendBytes(out, entities.data(), entities.size());
        out.emplace_back(std::byte{0});
        return out;
    }

    void AppendLiteralRle(std::vector<std::byte>& out, const unsigned char* data, const size_t size)
    {
        auto offset = 0uz;
        while (offset < size)
        {
            const auto chunkSize = std::min(127uz, size - offset);
            const auto marker = static_cast<uint8_t>(~chunkSize);
            Append(out, marker);
            AppendBytes(out, data + offset, chunkSize);
            offset += chunkSize;
        }
    }

    [[nodiscard]] std::vector<std::byte> BuildGameWorldSpPath(const GameWorldSp& gameWorld)
    {
        std::vector<std::byte> out;
        const uint32_t version = 8u;
        const auto nodeCount = static_cast<uint16_t>(std::min(gameWorld.path.nodeCount, static_cast<unsigned>(std::numeric_limits<uint16_t>::max())));

        Append(out, version);
        Append(out, nodeCount);

        for (auto nodeIndex = 0uz; nodeIndex < nodeCount; nodeIndex++)
        {
            const auto& node = gameWorld.path.nodes[nodeIndex];
            const auto linkCount = node.constant.totalLinkCount;
            Append(out, linkCount);

            for (auto linkIndex = 0uz; linkIndex < linkCount; linkIndex++)
            {
                const auto& link = node.constant.Links[linkIndex];
                Append(out, link.nodeNum);
                Append(out, link.fDist);
            }
        }

        AppendLiteralRle(out, gameWorld.path.pathVis, PositiveCount(gameWorld.path.visBytes));
        return out;
    }

    void AppendPrimaryLight(std::vector<std::byte>& out, const ComPrimaryLight& light)
    {
        constexpr auto TYPE_OFFSET = 0uz;
        constexpr auto CAN_USE_SHADOW_MAP_OFFSET = 1uz;
        constexpr auto UNUSED_OFFSET = 2uz;
        constexpr auto COLOR_OFFSET = 4uz;
        constexpr auto DIR_OFFSET = 16uz;
        constexpr auto ORIGIN_OFFSET = 28uz;
        constexpr auto RADIUS_OFFSET = 40uz;
        constexpr auto COS_HALF_FOV_OUTER_OFFSET = 44uz;
        constexpr auto COS_HALF_FOV_INNER_OFFSET = 48uz;
        constexpr auto EXPONENT_OFFSET = 52uz;
        constexpr auto ROTATION_LIMIT_OFFSET = 56uz;
        constexpr auto TRANSLATION_LIMIT_OFFSET = 60uz;
        constexpr auto DEF_NAME_OFFSET = 64uz;
        constexpr auto DEF_NAME_SIZE = 64uz;

        const auto baseOffset = out.size();
        out.resize(out.size() + IW3::d3dbsp::RAW_PRIMARY_LIGHT_SIZE, std::byte{});

        out[baseOffset + TYPE_OFFSET] = static_cast<std::byte>(light.type);
        out[baseOffset + CAN_USE_SHADOW_MAP_OFFSET] = static_cast<std::byte>(light.canUseShadowMap);
        out[baseOffset + UNUSED_OFFSET] = static_cast<std::byte>(light.unused[0]);
        out[baseOffset + UNUSED_OFFSET + 1uz] = static_cast<std::byte>(light.unused[1]);

        // The v22 BSP stores DiskPrimaryLight, not the runtime ComPrimaryLight
        // layout. cosHalfFovExpanded is derived by the linker from outer FOV and
        // rotationLimit, while exponent is stored as a 32-bit disk field.
        WriteBytesAt(out, baseOffset + COLOR_OFFSET, light.color, sizeof(light.color));
        WriteBytesAt(out, baseOffset + DIR_OFFSET, light.dir, sizeof(light.dir));
        WriteBytesAt(out, baseOffset + ORIGIN_OFFSET, light.origin, sizeof(light.origin));
        WriteAt(out, baseOffset + RADIUS_OFFSET, light.radius);
        WriteAt(out, baseOffset + COS_HALF_FOV_OUTER_OFFSET, light.cosHalfFovOuter);
        WriteAt(out, baseOffset + COS_HALF_FOV_INNER_OFFSET, light.cosHalfFovInner);
        const auto exponent = static_cast<int32_t>(light.exponent);
        WriteAt(out, baseOffset + EXPONENT_OFFSET, exponent);
        WriteAt(out, baseOffset + ROTATION_LIMIT_OFFSET, light.rotationLimit);
        WriteAt(out, baseOffset + TRANSLATION_LIMIT_OFFSET, light.translationLimit);
        if (light.defName)
        {
            const auto defNameLength = std::min(std::strlen(light.defName), DEF_NAME_SIZE - 1uz);
            WriteBytesAt(out, baseOffset + DEF_NAME_OFFSET, light.defName, defNameLength);
        }
    }

    [[nodiscard]] std::vector<std::byte> BuildPrimaryLights(const ComWorld& comWorld)
    {
        if (comWorld.primaryLightCount <= 0u || !comWorld.primaryLights)
            return {};

        std::vector<std::byte> out;
        out.reserve(static_cast<size_t>(comWorld.primaryLightCount) * IW3::d3dbsp::RAW_PRIMARY_LIGHT_SIZE);
        for (auto lightIndex = 0uz; lightIndex < comWorld.primaryLightCount; lightIndex++)
            AppendPrimaryLight(out, comWorld.primaryLights[lightIndex]);
        return out;
    }

    struct ReflectionProbeColorCorrection
    {
        float blackLevel, whiteLevel, gamma, saturation;
    };

    constexpr ReflectionProbeColorCorrection DEFAULT_REFLECTION_PROBE_CORRECTION{0.3f, 0.7f, 1.2f, 0.5f};

    uint8_t ClampToByte(int value)
    {
        return static_cast<uint8_t>(std::clamp(value, 0, 255));
    }

    [[nodiscard]] uint32_t PackRgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    {
        return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8u) | (static_cast<uint32_t>(b) << 16u) | (static_cast<uint32_t>(a) << 24u);
    }

    [[nodiscard]] std::array<float, 256> ReflectionGammaTable(const ReflectionProbeColorCorrection& correction)
    {
        const auto range = correction.whiteLevel - correction.blackLevel;
        std::array<float, 256> result;
        for (size_t i = 0; i < result.size(); ++i)
        {
            auto color = static_cast<float>(i / 255.0);
            color = std::max(0.0f, (color - correction.blackLevel) / range);
            result[i] = static_cast<float>(std::pow(static_cast<double>(color), static_cast<double>(correction.gamma)));
        }
        return result;
    }

    [[nodiscard]] uint32_t TransformReflectionProbeColor(
        const uint8_t r, const uint8_t g, const uint8_t b, const ReflectionProbeColorCorrection& correction, const std::array<float, 256>& gammaTable)
    {
        // Match the float stores in linker_pc's R_ColorCorrectBGRAPixel.
        // The byte order here is BGRA, including its BGR luminance weights.
        float color[3]{gammaTable[r], gammaTable[g], gammaTable[b]};
        const auto luma = 0.114f * color[0] + 0.587f * color[1] + 0.299f * color[2];
        for (auto& channel : color)
            channel = correction.saturation * channel + (1.0f - correction.saturation) * luma;
        for (auto& channel : color)
            channel = std::min(channel, 4.0f);
        const auto scale = std::max({0.1f, color[0], color[1], color[2]});
        return PackRgba(ClampToByte(static_cast<int>(color[0] / scale * 255.0)),
                        ClampToByte(static_cast<int>(color[1] / scale * 255.0)),
                        ClampToByte(static_cast<int>(color[2] / scale * 255.0)),
                        ClampToByte(static_cast<int>(255.0 * (scale * 0.25))));
    }

    [[nodiscard]] uint32_t EstimateReflectionProbeSourceColor(const uint32_t targetColor, const double scale, const ReflectionProbeColorCorrection& correction)
    {
        constexpr double LUMA_R = 0.1140000000596046;
        constexpr double LUMA_G = 0.5870000123977661;
        constexpr double LUMA_B = 0.2989999949932098;

        const auto r = static_cast<double>(targetColor & 0xFFu);
        const auto g = static_cast<double>((targetColor >> 8u) & 0xFFu);
        const auto b = static_cast<double>((targetColor >> 16u) & 0xFFu);

        const double corrected[3]{
            ((r + 0.5) / 255.0) * scale,
            ((g + 0.5) / 255.0) * scale,
            ((b + 0.5) / 255.0) * scale,
        };

        const auto luma = LUMA_R * corrected[0] + LUMA_G * corrected[1] + LUMA_B * corrected[2];
        const auto range = correction.whiteLevel - correction.blackLevel;

        uint8_t source[3]{};
        for (auto i = 0uz; i < 3uz; i++)
        {
            const auto unsaturated = std::max(0.0, (corrected[i] - (1.0 - correction.saturation) * luma) / correction.saturation);
            const auto linear = std::pow(unsaturated, 1.0 / correction.gamma) * range + correction.blackLevel;
            source[i] = ClampToByte(static_cast<int>(std::round(linear * 255.0)));
        }

        return PackRgba(source[0], source[1], source[2], 0u);
    }

    void RecoverReflectionCapture(std::vector<std::byte>& records,
                                  IW3Xenon253::d3dbsp::Recovery* recovery,
                                  const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        if (recovery)
            recovery->reflectionProbeRecords = records;
        // These are reconstruction curves, not recovered original capture
        // settings. Some alpha captures retain saturation outside the default
        // curve's gamut; select the curve that best reproduces each probe.
        constexpr ReflectionProbeColorCorrection corrections[]{
            DEFAULT_REFLECTION_PROBE_CORRECTION, {0.3f, 0.7f, 1.2f, 1.0f},
             {0.3f, 0.5f, 1.2f, 1.0f}
        };
        const auto& prefix = overrides.reflectionCurvePrefix;
        const std::string names[]{prefix + "_default", prefix + "_unsaturated", prefix + "_hdr"};
        if (names[1].size() >= 64)
            throw std::runtime_error("Reflection correction name exceeds disk capacity");
        const auto gammaTables = std::array{ReflectionGammaTable(corrections[0]), ReflectionGammaTable(corrections[1]), ReflectionGammaTable(corrections[2])};

        struct Pixel
        {
            uint32_t source, corrected;
            uint64_t error;
        };

        std::unordered_map<uint32_t, Pixel> caches[3];
        const auto recoverPixel = [&](uint32_t target, size_t preset) -> const Pixel&
        {
            auto& cache = caches[preset];
            if (const auto found = cache.find(target); found != cache.end())
                return found->second;
            const auto& correction = corrections[preset];
            const auto scale = std::max(0.1, (double(target >> 24) + 0.5) / 63.75);
            const auto estimate = EstimateReflectionProbeSourceColor(target, scale, correction);
            const int base[3]{int(estimate & 255), int((estimate >> 8) & 255), int((estimate >> 16) & 255)};
            Pixel best{estimate, 0, std::numeric_limits<uint64_t>::max()};
            for (int b = std::max(0, base[0] - 2); b <= std::min(255, base[0] + 2) && best.error; ++b)
                for (int g = std::max(0, base[1] - 2); g <= std::min(255, base[1] + 2) && best.error; ++g)
                    for (int r = std::max(0, base[2] - 2); r <= std::min(255, base[2] + 2) && best.error; ++r)
                    {
                        const auto corrected = TransformReflectionProbeColor(
                            static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r), correction, gammaTables[preset]);
                        uint64_t error = 0;
                        for (unsigned shift = 0; shift < (overrides.canonicalizeReflectionRgbm ? 24u : 32u); shift += 8)
                        {
                            const int64_t delta = overrides.canonicalizeReflectionRgbm ? int64_t((corrected >> shift) & 255) * (corrected >> 24)
                                                                                             - int64_t((target >> shift) & 255) * (target >> 24)
                                                                                       : int64_t((corrected >> shift) & 255) - int64_t((target >> shift) & 255);
                            error += static_cast<uint64_t>(delta * delta);
                        }
                        if (error < best.error)
                            best = {PackRgba(static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r), 0), corrected, error};
                    }
            return cache.emplace(target, best).first->second;
        };
        constexpr size_t RECORD_SIZE = 76 + REFLECTION_PROBE_RAW_DATA_SIZE;
        for (size_t record = 0; record < records.size(); record += RECORD_SIZE)
        {
            if (overrides.canonicalizeReflectionRgbm)
                for (size_t offset = record + 76; offset < record + RECORD_SIZE; offset += 4)
                {
                    auto* pixel = reinterpret_cast<uint8_t*>(records.data() + offset);
                    const std::array<uint8_t, 4> original{pixel[0], pixel[1], pixel[2], pixel[3]};
                    // RGBM is not a unique byte representation. QOS retains a
                    // larger multiplier floor than the IW3 capture encoder.
                    // Normalize RGB*M without treating that floor as colour.
                    const float maximumProduct = std::max(0.1f * 255.0f * 255.0f / 4.0f, float(std::max({pixel[0], pixel[1], pixel[2]})) * pixel[3]);
                    for (unsigned channel = 0; channel < 3; ++channel)
                        pixel[channel] = ClampToByte(static_cast<int>(float(original[channel]) * original[3] * 255.0f / maximumProduct));
                    pixel[3] = ClampToByte(static_cast<int>(maximumProduct / 255.0f));
                    if (recovery && !std::equal(original.begin(), original.end(), pixel))
                        ++recovery->canonicalizedProbePixels;
                }
            std::array<uint64_t, 3> errors{};
            std::array<uint64_t, 3> maximumErrors{};
            for (size_t offset = record + 76; offset < record + RECORD_SIZE; offset += 4)
            {
                uint32_t target;
                std::memcpy(&target, records.data() + offset, 4);
                const auto error = recoverPixel(target, 0).error;
                errors[0] += error;
                maximumErrors[0] = std::max(maximumErrors[0], error);
            }
            if (errors[0])
                for (size_t offset = record + 76; offset < record + RECORD_SIZE; offset += 4)
                {
                    uint32_t target;
                    std::memcpy(&target, records.data() + offset, 4);
                    for (size_t preset = 1; preset < errors.size(); ++preset)
                    {
                        const auto error = recoverPixel(target, preset).error;
                        errors[preset] += error;
                        maximumErrors[preset] = std::max(maximumErrors[preset], error);
                    }
                }
            size_t chosen = 0;
            for (size_t preset = 1; preset < errors.size() && errors[0]; ++preset)
            {
                // A rare QOS HDR highlight must not be clipped to reduce the
                // aggregate error of many dark texels. Prefer the smallest
                // worst-pixel RGBM product error, then total error. Preserve
                // the original Alpha 253 selection when normalization is off.
                const bool better = overrides.canonicalizeReflectionRgbm
                                        ? std::pair{maximumErrors[preset], errors[preset]} < std::pair{maximumErrors[chosen], errors[chosen]}
                                        : errors[preset] < errors[chosen];
                if (better)
                    chosen = preset;
            }
            std::memset(records.data() + record + 12, 0, 64);
            std::memcpy(records.data() + record + 12, names[chosen].data(), names[chosen].size());
            if (recovery)
                recovery->reflectionCorrections.emplace_back(names[chosen]);
            for (size_t offset = record + 76; offset < record + RECORD_SIZE; offset += 4)
            {
                uint32_t target;
                std::memcpy(&target, records.data() + offset, 4);
                const auto& best = recoverPixel(target, chosen);
                if (recovery)
                {
                    recovery->approximateProbePixels += best.corrected != target;
                    for (unsigned shift = 0; shift < 32; shift += 8)
                        recovery->maximumProbeChannelError =
                            std::max(recovery->maximumProbeChannelError,
                                     static_cast<unsigned>(std::abs(int((best.corrected >> shift) & 255) - int((target >> shift) & 255))));
                    const auto* original = reinterpret_cast<const uint8_t*>(recovery->reflectionProbeRecords.data() + offset);
                    for (unsigned channel = 0; channel < 3; ++channel)
                        recovery->maximumProbeRgbmProductError =
                            std::max(recovery->maximumProbeRgbmProductError,
                                     static_cast<unsigned>(std::abs(int(original[channel]) * original[3]
                                                                    - int((best.corrected >> (channel * 8)) & 255) * int(best.corrected >> 24))));
                }
                std::memcpy(records.data() + offset, &best.source, 4);
            }
        }
    }

    std::vector<std::byte> BuildReflectionProbes(const GfxWorld& world, ISearchPath& searchPath, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        std::vector<std::byte> out;
        const std::array<char, 64> correction{'d', 'e', 'f', 'a', 'u', 'l', 't'};
        // Probe zero is the runtime fallback and has no disk record.
        for (unsigned probeIndex = 1; probeIndex < world.reflectionProbeCount; ++probeIndex)
        {
            const auto& probe = world.reflectionProbes[probeIndex];
            std::string error;
            auto texture = overrides.loadImage ? overrides.loadImage(probe.reflectionImage, image::TextureType::T_CUBE)
                                               : image::xenon::LoadImage(probe.reflectionImage, image::TextureType::T_CUBE, searchPath, &error);
            if (!texture || texture->GetWidth() != 64 || texture->GetHeight() != 64 || texture->GetMipMapCount() != 7)
                throw std::runtime_error(std::format("Cannot decode reflection probe {}: {}", probeIndex, error));
            if (texture->GetFormat()->GetId() != image::ImageFormatId::B8_G8_R8_A8)
            {
                const auto* target = image::ImageFormat::GetImageFormatById(image::ImageFormatId::B8_G8_R8_A8);
                std::unique_ptr<image::Texture> converted;
                if (texture->GetFormat()->GetType() == image::ImageFormatType::BLOCK_COMPRESSED)
                {
                    const auto decompressor = image::ImageDecompressor::GetDecompressorForFormat(texture->GetFormat()->GetId());
                    if (decompressor)
                        converted = decompressor->Decompress(*texture, target);
                }
                else
                    converted = image::TextureConverter(texture.get(), target).Convert();
                if (!converted)
                    throw std::runtime_error("Unsupported reflection probe pixel format");
                texture = std::move(converted);
            }
            AppendBytes(out, probe.origin, sizeof(probe.origin));
            AppendBytes(out, correction.data(), correction.size());
            const auto appendMip = [&](int face, int mip)
            {
                const auto* pixels = texture->GetBufferForMipLevel(mip, face);
                AppendBytes(out, pixels, texture->GetSizeOfMipLevel(mip));
            };
            // R_GenerateReflectionImageFromRawData: six base faces, then each
            // face's complete mip chain. Texture uses face-major storage.
            for (int face = 0; face < 6; ++face)
                appendMip(face, 0);
            for (int face = 0; face < 6; ++face)
                for (int mip = 1; mip < 7; ++mip)
                    appendMip(face, mip);
        }
        return out;
    }

    struct LayerLayout
    {
        size_t layers = 1;
        size_t normals = 0;
    };

    LayerLayout ParseLayers(std::string_view name, const clipMap_t& clipMap)
    {
        name = BaseName(name.data());
        if (!name.starts_with('*'))
            return {};
        name.remove_prefix(1);
        LayerLayout layout{0, 0};
        while (!name.empty())
        {
            unsigned index;
            const auto parsed = std::from_chars(name.data(), name.data() + name.size(), index);
            if (parsed.ec != std::errc{} || index >= clipMap.numMaterials)
                throw std::runtime_error("Generated material has an invalid BSP material reference");
            const auto referenced = BaseName(clipMap.materials[index].material);
            if (referenced.starts_with('*'))
                throw std::runtime_error("Generated material references another generated material");
            name.remove_prefix(parsed.ptr - name.data());
            ++layout.layers;
            if (name.starts_with('n'))
            {
                ++layout.normals;
                name.remove_prefix(1);
            }
            if (name.empty())
                break;
            if (!name.starts_with('_') || name.size() == 1)
                throw std::runtime_error("Invalid generated material layer separator");
            name.remove_prefix(1);
        }
        if (layout.layers < 2 || layout.layers > 5 || layout.normals > 3)
            throw std::runtime_error("Unsupported generated material layer count");
        return layout;
    }

    std::vector<std::byte> BuildVertexLayers(const GfxWorld& world, const clipMap_t& clipMap, const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        auto out = RawBytes(world.vld.data, world.vertexLayerDataSize);
        // cod4map Tris_CopyReorderedVert: float2 per additional layer followed
        // by a four-byte tangent frame per additional normal-mapped layer.
        // Fastfile loading treats this stream as bytes, so only UV floats swap.
        std::unordered_set<size_t> swapped;
        for (int i = 0; i < world.surfaceCount; ++i)
        {
            const auto& surface = world.surfaces[i];
            const auto layout = ParseLayers(surface.material->info.name, clipMap);
            if (layout.layers == 1)
                continue;
            const auto uvBytes = (layout.layers - 1) * 8;
            const auto stride = uvBytes + (layout.normals > 0 ? layout.normals - 1 : 0) * 4;
            const auto offset = surface.tris.vertexLayerData;
            if (offset < 0 || size_t(offset) + surface.tris.vertexCount * stride > out.size())
                throw std::runtime_error(std::format("Layer data for {} exceeds the vertex-layer buffer", surface.material->info.name));
            for (size_t vertex = 0; vertex < surface.tris.vertexCount; ++vertex)
            {
                for (size_t component = 0; component < uvBytes; component += 4)
                {
                    const auto at = size_t(offset) + vertex * stride + component;
                    if (overrides.bigEndianVertexLayers && swapped.insert(at).second)
                        std::reverse(out.begin() + at, out.begin() + at + 4);
                }
            }
        }
        return out;
    }

    struct WorldTopology
    {
        std::vector<std::byte> cells;
        std::vector<std::byte> trees;
        std::vector<std::byte> portals;
        std::vector<std::byte> portalVertices;
        std::vector<std::byte> cullGroups;
        std::vector<std::byte> cullGroupIndices;
        std::vector<std::byte> planes;
    };

    WorldTopology BuildTopology(const GfxWorld& world,
                                const clipMap_t& clipMap,
                                IW3Xenon253::d3dbsp::Recovery* recovery,
                                const IW3Xenon253::d3dbsp::SourceOverrides& overrides)
    {
        WorldTopology result;
        result.planes = BuildPlanes(clipMap);
        // Disk child roots are contiguous, with their descendants following
        // the sibling group (R_FinishLoadingAabbTrees_r), not simple preorder.
        std::vector<const GfxAabbTree*> trees;
        std::unordered_set<const GfxAabbTree*> active;
        const auto appendChildren = [&](auto&& self, const GfxAabbTree& tree) -> void
        {
            if (!active.insert(&tree).second)
                throw std::runtime_error("Cyclic world AABB tree");
            if (tree.childCount < 0 || (tree.childCount && !tree.children))
                throw std::runtime_error("Invalid world AABB children");
            for (int i = 0; i < tree.childCount; ++i)
                trees.push_back(&tree.children[i]);
            for (int i = 0; i < tree.childCount; ++i)
                self(self, tree.children[i]);
            active.erase(&tree);
        };

        for (int cellIndex = 0; cellIndex < world.cellCount; ++cellIndex)
        {
            const auto& cell = world.cells[cellIndex];
            if (!cell.aabbTree || trees.size() > 0xffff)
                throw std::runtime_error("Missing or oversized cell AABB tree");
            const auto treeIndex = static_cast<uint16_t>(trees.size());
            trees.push_back(cell.aabbTree);
            appendChildren(appendChildren, *cell.aabbTree);
            AppendBytes(result.cells, cell.mins, sizeof(cell.mins));
            AppendBytes(result.cells, cell.maxs, sizeof(cell.maxs));
            Append(result.cells, treeIndex);
            Append(result.cells, treeIndex);
            Append(result.cells, static_cast<uint32_t>(result.portals.size() / 16));
            Append(result.cells, cell.portalCount);
            Append(result.cells, static_cast<uint32_t>(result.cullGroupIndices.size() / 4));
            Append(result.cells, cell.cullGroupCount);
            AppendBytes(result.cullGroupIndices, cell.cullGroups, PositiveCount(cell.cullGroupCount) * sizeof(int32_t));

            // 253 predates per-cell probe lists. Recover probes referenced by
            // this cell's surfaces and static models; the v22 array holds 64.
            std::unordered_map<unsigned, size_t> probes;
            for (size_t t = treeIndex; t < trees.size(); ++t)
            {
                const auto& tree = *trees[t];
                if (tree.surfaceCount < 0
                    || (tree.surfaceCount && (tree.startSurfIndex < 0 || size_t(tree.startSurfIndex) + tree.surfaceCount > PositiveCount(world.surfaceCount))))
                    throw std::runtime_error(
                        std::format("AABB surface range {} + {} exceeds {} surfaces", tree.startSurfIndex, tree.surfaceCount, world.surfaceCount));
                for (int i = 0; i < tree.surfaceCount; ++i)
                    ++probes[world.surfaces[tree.startSurfIndex + i].reflectionProbeIndex];
                for (int i = 0; i < tree.smodelIndexCount; ++i)
                {
                    const auto index = tree.smodelIndexes[i];
                    if (index < 0 || index >= world.smodelCount)
                        throw std::runtime_error("AABB static-model index exceeds the world");
                    ++probes[world.smodelDrawInsts[index].reflectionProbeIndex];
                }
            }
            probes.erase(0);
            std::vector<unsigned> ordered;
            for (const auto& [probe, references] : probes)
                ordered.push_back(probe);
            std::sort(ordered.begin(), ordered.end());
            if (overrides.cellProbeCandidates)
            {
                if (overrides.cellProbeCandidates->size() != PositiveCount(world.cellCount))
                    throw std::runtime_error("Cell probe override count differs from world");
                ordered = overrides.cellProbeCandidates->at(cellIndex);
                for (const auto probe : ordered)
                    if (probe >= world.reflectionProbeCount)
                        throw std::runtime_error("Invalid authored cell reflection probe");
            }
            if (recovery)
                recovery->cellProbeCandidates.push_back(ordered);
            if (ordered.size() > 64)
            {
                // The alpha can exceed the PC DiskGfxCell capacity. Keep all
                // global probes and explicit surface/model assignments, but
                // prioritize the most referenced probes for dynamic lookup.
                // The complete candidate list remains in the recovery file.
                std::stable_sort(ordered.begin(),
                                 ordered.end(),
                                 [&](unsigned a, unsigned b)
                                 {
                                     return probes[a] > probes[b];
                                 });
                ordered.resize(64);
                std::sort(ordered.begin(), ordered.end());
                if (recovery)
                    ++recovery->truncatedCellProbeLists;
            }
            Append(result.cells, static_cast<uint8_t>(ordered.size()));
            for (const auto probe : ordered)
                Append(result.cells, static_cast<uint8_t>(probe));
            result.cells.resize(result.cells.size() + 67 - ordered.size(), std::byte{});

            for (int i = 0; i < cell.portalCount; ++i)
            {
                const auto& portal = cell.portals[i];
                uint32_t planeIndex = static_cast<uint32_t>(result.planes.size() / 16);
                // Keep the original collision plane indices stable. Portal-only
                // planes can safely be appended to the shared disk plane table.
                for (size_t p = 0; p < result.planes.size() / 16; ++p)
                {
                    float plane[4];
                    std::memcpy(plane, result.planes.data() + p * 16, 16);
                    if (plane[0] == portal.plane.coeffs[0] && plane[1] == portal.plane.coeffs[1] && plane[2] == portal.plane.coeffs[2]
                        && plane[3] == -portal.plane.coeffs[3])
                    {
                        planeIndex = static_cast<uint32_t>(p);
                        break;
                    }
                }
                if (planeIndex == result.planes.size() / 16)
                {
                    AppendBytes(result.planes, portal.plane.coeffs, 12);
                    Append(result.planes, -portal.plane.coeffs[3]);
                }
                size_t targetCell;
                if (!TryPointerIndex(world.cells, PositiveCount(world.cellCount), portal.cell, targetCell))
                    throw std::runtime_error("Portal references an invalid cell");
                Append(result.portals, planeIndex);
                Append(result.portals, static_cast<uint32_t>(targetCell));
                Append(result.portals, static_cast<uint32_t>(result.portalVertices.size() / 12));
                Append(result.portals, static_cast<uint32_t>(portal.vertexCount));
                AppendBytes(result.portalVertices, portal.vertices, size_t(portal.vertexCount) * 12);
            }
        }
        for (const auto* tree : trees)
        {
            Append(result.trees, tree->surfaceCount ? static_cast<uint32_t>(tree->startSurfIndex) : 0u);
            Append(result.trees, static_cast<uint32_t>(tree->surfaceCount));
            Append(result.trees, static_cast<uint32_t>(tree->childCount));
        }
        for (int i = 0; i < world.cullGroupCount; ++i)
        {
            const auto& group = world.cullGroups[i];
            AppendBytes(result.cullGroups, group.mins, sizeof(group.mins));
            AppendBytes(result.cullGroups, group.maxs, sizeof(group.maxs));
            Append(result.cullGroups, group.startSurfIndex);
            Append(result.cullGroups, group.surfaceCount);
        }
        return result;
    }

    struct DiskSurfaceLayout
    {
        GfxWorld world;
        std::vector<GfxSurface> surfaces;
        std::vector<GfxCell> cells;
        std::vector<std::unique_ptr<GfxAabbTree[]>> trees;
        std::vector<std::unique_ptr<GfxPortal[]>> portals;
        std::vector<std::unique_ptr<int[]>> cellCullGroups;
        std::vector<GfxCullGroup> cullGroups;

        GfxAabbTree* AllocateTrees(size_t count)
        {
            trees.push_back(std::make_unique<GfxAabbTree[]>(count));
            return trees.back().get();
        }

        unsigned InferCell(const GfxWorld& source, const GfxSurface& surface)
        {
            float point[3];
            for (size_t axis = 0; axis < 3; ++axis)
                point[axis] = (surface.bounds[0][axis] + surface.bounds[1][axis]) * 0.5f;
            size_t node = 0;
            for (size_t step = 0; source.nodes && node < PositiveCount(source.nodeCount) && step < PositiveCount(source.nodeCount); ++step)
            {
                const auto code = source.nodes[node];
                if (code <= source.cellCount)
                {
                    if (code)
                        return code - 1;
                    break;
                }
                const auto planeIndex = code - source.cellCount - 1;
                if (planeIndex >= source.planeCount || !source.planes || node + 1 >= PositiveCount(source.nodeCount))
                    break;
                const auto& plane = source.planes[planeIndex];
                const auto distance = point[0] * plane.normal[0] + point[1] * plane.normal[1] + point[2] * plane.normal[2] - plane.dist;
                node += distance >= 0 ? 2 : source.nodes[node + 1];
            }
            // Surfaces on a solid partition boundary have no unique point
            // cell. Use the nearest cell bounds, with a stable index tie-break.
            unsigned bestCell = 0;
            double bestDistance = std::numeric_limits<double>::max();
            for (int cell = 0; cell < source.cellCount; ++cell)
            {
                double distance = 0;
                for (size_t axis = 0; axis < 3; ++axis)
                {
                    const auto nearest = std::clamp(point[axis], source.cells[cell].mins[axis], source.cells[cell].maxs[axis]);
                    const auto delta = double(point[axis]) - nearest;
                    distance += delta * delta;
                }
                if (distance < bestDistance)
                {
                    bestCell = cell;
                    bestDistance = distance;
                }
            }
            return bestCell;
        }

        explicit DiskSurfaceLayout(const GfxWorld& source, IW3Xenon253::d3dbsp::Recovery* recovery)
            : world(source)
        {
            const auto count = source.modelCount ? source.models[0].surfaceCount : 0u;
            if (count > PositiveCount(source.surfaceCount))
                throw std::runtime_error("World model surface range exceeds the world");
            if (source.surfaceCount)
                surfaces.assign(source.surfaces, source.surfaces + source.surfaceCount);
            // AABB and cull-group spans address the spatial order, while the
            // runtime surface array is sorted by material. The saved mapping
            // goes from the former order to the latter.
            if (source.sortedSurfIndex)
            {
                std::vector<bool> seen(count);
                for (unsigned i = 0; i < count; ++i)
                {
                    const auto index = source.sortedSurfIndex[i];
                    if (index >= count || seen[index])
                        throw std::runtime_error("Invalid runtime surface permutation");
                    seen[index] = true;
                    surfaces[i] = source.surfaces[index];
                }
            }
            world.surfaces = surfaces.data();
            if (!count)
                return;
            if (!source.cellCount)
                throw std::runtime_error("World surfaces have no cells");
            std::vector<int> owners(count, -1);
            for (int cell = 0; cell < source.cellCount; ++cell)
            {
                const auto* tree = source.cells[cell].aabbTree;
                if (!tree || tree->surfaceCount < 0
                    || (tree->surfaceCount && (tree->startSurfIndex < 0 || size_t(tree->startSurfIndex) + tree->surfaceCount > count)))
                    throw std::runtime_error("Invalid cell root surface range");
                for (int i = 0; i < tree->surfaceCount; ++i)
                    owners[tree->startSurfIndex + i] = cell;
            }
            std::vector<std::vector<unsigned>> extra(source.cellCount);
            for (unsigned i = 0; i < count; ++i)
                if (owners[i] < 0)
                    extra[InferCell(source, surfaces[i])].push_back(i);
            if (std::none_of(extra.begin(),
                             extra.end(),
                             [](const auto& list)
                             {
                                 return !list.empty();
                             }))
                return;

            // Some alpha maps retain surfaces outside all cell tree ranges.
            // PC requires every world surface to be reachable from leaf trees.
            // Restore those ranges without flattening the existing hierarchy.
            cells.assign(source.cells, source.cells + source.cellCount);
            world.cells = cells.data();
            std::vector<unsigned> remap(count, std::numeric_limits<unsigned>::max());
            std::vector<GfxSurface> reordered = surfaces;
            unsigned nextSurface = 0;
            std::unordered_set<const GfxAabbTree*> active;
            const auto clone = [&](auto&& self, GfxAabbTree& out, const GfxAabbTree& in, int delta) -> void
            {
                if (!active.insert(&in).second || in.childCount < 0 || (in.childCount && !in.children))
                    throw std::runtime_error("Invalid or cyclic AABB tree during surface recovery");
                out = in;
                out.startSurfIndex = in.surfaceCount ? in.startSurfIndex + delta : 0;
                if (in.childCount)
                {
                    out.children = AllocateTrees(in.childCount);
                    for (int i = 0; i < in.childCount; ++i)
                        self(self, out.children[i], in.children[i], delta);
                }
                active.erase(&in);
            };
            for (int cell = 0; cell < source.cellCount; ++cell)
            {
                const auto& oldRoot = *source.cells[cell].aabbTree;
                const auto first = nextSurface;
                const auto append = [&](unsigned oldIndex)
                {
                    if (oldIndex >= count || remap[oldIndex] != std::numeric_limits<unsigned>::max() || nextSurface >= count)
                        throw std::runtime_error("Overlapping cell ranges during surface recovery");
                    remap[oldIndex] = nextSurface;
                    reordered[nextSurface++] = surfaces[oldIndex];
                };
                for (int i = 0; i < oldRoot.surfaceCount; ++i)
                    append(oldRoot.startSurfIndex + i);
                for (const auto index : extra[cell])
                {
                    if (recovery)
                        recovery->recoveredSurfaceCells.push_back({index, nextSurface, static_cast<unsigned>(cell)});
                    append(index);
                    for (size_t axis = 0; axis < 3; ++axis)
                    {
                        cells[cell].mins[axis] = std::min(cells[cell].mins[axis], surfaces[index].bounds[0][axis]);
                        cells[cell].maxs[axis] = std::max(cells[cell].maxs[axis], surfaces[index].bounds[1][axis]);
                    }
                }
                auto* root = AllocateTrees(1);
                cells[cell].aabbTree = root;
                if (extra[cell].empty())
                    clone(clone, *root, oldRoot, int(first) - oldRoot.startSurfIndex);
                else
                {
                    *root = oldRoot;
                    root->startSurfIndex = first;
                    root->surfaceCount = nextSurface - first;
                    root->childCount = 2;
                    root->smodelIndexCount = 0;
                    root->smodelIndexes = nullptr;
                    root->children = AllocateTrees(2);
                    const auto originalChild = oldRoot.surfaceCount ? 0 : 1;
                    clone(clone, root->children[originalChild], oldRoot, int(first) - oldRoot.startSurfIndex);
                    auto& recovered = root->children[1 - originalChild];
                    recovered.startSurfIndex = first + oldRoot.surfaceCount;
                    recovered.surfaceCount = static_cast<int>(extra[cell].size());
                }
                if (cells[cell].portalCount)
                {
                    auto copy = std::make_unique<GfxPortal[]>(cells[cell].portalCount);
                    for (int i = 0; i < cells[cell].portalCount; ++i)
                    {
                        copy[i] = cells[cell].portals[i];
                        copy[i].cell = &cells[PointerIndex(source.cells, source.cellCount, copy[i].cell)];
                    }
                    cells[cell].portals = copy.get();
                    portals.push_back(std::move(copy));
                }
            }
            if (nextSurface != count)
                throw std::runtime_error("Incomplete world surface recovery");
            surfaces = std::move(reordered);
            world.surfaces = surfaces.data();

            // A group can become several contiguous runs after the reorder.
            // Split it and expand its cell references rather than enclosing
            // unrelated surfaces in a fabricated range.
            std::vector<std::vector<int>> groupMap(source.cullGroupCount);
            for (int group = 0; group < source.cullGroupCount; ++group)
            {
                const auto& old = source.cullGroups[group];
                std::vector<unsigned> indices;
                for (int i = 0; i < old.surfaceCount; ++i)
                {
                    const auto index = old.startSurfIndex + i;
                    if (index < 0 || static_cast<unsigned>(index) >= count)
                        throw std::runtime_error("Invalid cull group during surface recovery");
                    indices.push_back(remap[index]);
                }
                std::sort(indices.begin(), indices.end());
                for (size_t first = 0; first < indices.size();)
                {
                    auto end = first + 1;
                    while (end < indices.size() && indices[end] == indices[end - 1] + 1)
                        ++end;
                    auto copy = old;
                    copy.startSurfIndex = indices[first];
                    copy.surfaceCount = static_cast<int>(end - first);
                    groupMap[group].push_back(static_cast<int>(cullGroups.size()));
                    cullGroups.push_back(copy);
                    first = end;
                }
            }
            for (auto& cell : cells)
            {
                std::vector<int> indices;
                for (int i = 0; i < cell.cullGroupCount; ++i)
                {
                    const auto group = cell.cullGroups[i];
                    if (group < 0 || group >= source.cullGroupCount)
                        throw std::runtime_error("Invalid cell cull-group reference");
                    indices.insert(indices.end(), groupMap[group].begin(), groupMap[group].end());
                }
                auto copy = std::make_unique<int[]>(indices.size());
                std::copy(indices.begin(), indices.end(), copy.get());
                cell.cullGroupCount = static_cast<int>(indices.size());
                cell.cullGroups = copy.get();
                cellCullGroups.push_back(std::move(copy));
            }
            world.cullGroupCount = static_cast<int>(cullGroups.size());
            world.cullGroups = cullGroups.data();
        }
    };

    template<typename T, typename N> void RequireArray(const T* data, N count, const char* name)
    {
        if (count < 0 || (count && !data))
            throw std::runtime_error(std::format("Invalid {} array", name));
    }

    void Validate(const GfxWorld& world, const clipMap_t& clipMap, const ComWorld& comWorld)
    {
        if (clipMap.numMaterials > 65536)
            throw std::runtime_error("BSP material table exceeds 16-bit indices");
        RequireArray(world.surfaces, world.surfaceCount, "surfaces");
        RequireArray(world.vd.vertices, world.vertexCount, "vertices");
        RequireArray(world.indices, world.indexCount, "indices");
        RequireArray(world.vld.data, world.vertexLayerDataSize, "vertex layers");
        RequireArray(world.cells, world.cellCount, "cells");
        RequireArray(world.models, world.modelCount, "models");
        RequireArray(world.cullGroups, world.cullGroupCount, "cull groups");
        for (int i = 0; i < world.cellCount; ++i)
        {
            RequireArray(world.cells[i].portals, world.cells[i].portalCount, "portals");
            RequireArray(world.cells[i].cullGroups, world.cells[i].cullGroupCount, "cell cull groups");
        }
        RequireArray(world.lightmaps, world.lightmapCount, "lightmaps");
        RequireArray(world.reflectionProbes, world.reflectionProbeCount, "reflection probes");
        RequireArray(clipMap.materials, clipMap.numMaterials, "materials");
        RequireArray(clipMap.planes, clipMap.planeCount, "planes");
        RequireArray(clipMap.brushes, clipMap.numBrushes, "brushes");
        RequireArray(clipMap.nodes, clipMap.numNodes, "collision nodes");
        RequireArray(clipMap.leafs, clipMap.numLeafs, "collision leaves");
        RequireArray(clipMap.staticModelList, clipMap.numStaticModels, "static model collision instances");
        RequireArray(world.smodelDrawInsts, world.smodelCount, "static model render instances");
        RequireArray(clipMap.dynEntDefList[0], clipMap.dynEntCount[0], "dynamic models");
        RequireArray(clipMap.dynEntDefList[1], clipMap.dynEntCount[1], "dynamic brush models");
        RequireArray(comWorld.primaryLights, comWorld.primaryLightCount, "primary lights");
        for (int i = 0; i < world.surfaceCount; ++i)
        {
            const auto& surface = world.surfaces[i];
            const auto& tris = surface.tris;
            if (!surface.material || !surface.material->info.name || tris.baseIndex < 0 || tris.firstVertex < 0 || tris.indexCount % 3
                || size_t(tris.baseIndex) + tris.indexCount > PositiveCount(world.indexCount)
                || size_t(tris.firstVertex) + tris.vertexCount > world.vertexCount)
                throw std::runtime_error(std::format("Invalid surface {} geometry or material", i));
            for (unsigned j = 0; j < tris.indexCount; ++j)
                if (world.indices[tris.baseIndex + j] >= tris.vertexCount)
                    throw std::runtime_error(std::format("Surface {} has an invalid local vertex index", i));
        }
    }

    void AddLump(std::vector<BspLump>& lumps, const IW3::d3dbsp::LumpType id, std::vector<std::byte>&& data)
    {
        if (!data.empty())
            lumps.emplace_back(id, std::move(data));
    }

    [[nodiscard]] size_t LumpOrderIndex(const IW3::d3dbsp::LumpType id)
    {
        for (auto i = 0uz; i < IW3::d3dbsp::LUMP_WRITE_ORDER.size(); i++)
        {
            if (IW3::d3dbsp::LUMP_WRITE_ORDER[i] == id)
                return i;
        }

        return IW3::d3dbsp::LUMP_WRITE_ORDER.size();
    }

    void SortLumps(std::vector<BspLump>& lumps)
    {
        std::stable_sort(lumps.begin(),
                         lumps.end(),
                         [](const BspLump& left, const BspLump& right)
                         {
                             return LumpOrderIndex(left.id) < LumpOrderIndex(right.id);
                         });
    }

    void WriteBsp(std::ostream& stream, std::vector<BspLump> lumps)
    {
        SortLumps(lumps);

        const auto lumpCount = static_cast<uint32_t>(lumps.size());
        stream::Write(stream, IW3::d3dbsp::BSP_MAGIC.data(), IW3::d3dbsp::BSP_MAGIC.size());
        stream::WriteValue(stream, IW3::d3dbsp::BSP_VERSION);
        stream::WriteValue(stream, lumpCount);

        for (const auto& lump : lumps)
        {
            const auto id = std::to_underlying(lump.id);
            const auto size = static_cast<uint32_t>(lump.data.size());
            stream::WriteValue(stream, id);
            stream::WriteValue(stream, size);
        }

        const std::byte padding[3]{};
        for (const auto& lump : lumps)
        {
            stream::Write(stream, lump.data.data(), lump.data.size());

            const auto paddingSize = (4uz - lump.data.size() % 4uz) % 4uz;
            if (paddingSize > 0)
                stream::Write(stream, padding, paddingSize);
        }
    }
} // namespace

std::expected<void, std::string> IW3Xenon253::d3dbsp::Write(std::ostream& stream,
                                                            const GfxWorld& sourceWorld,
                                                            const clipMap_t& clipMap,
                                                            const ComWorld& comWorld,
                                                            const MapEnts& mapEnts,
                                                            const GameWorldSp* gameWorld,
                                                            ISearchPath& searchPath,
                                                            Recovery* recovery,
                                                            const SourceOverrides& overrides)
{
    try
    {
        if (recovery)
            *recovery = {};
        Validate(sourceWorld, clipMap, comWorld);
        DiskSurfaceLayout surfaceLayout(sourceWorld, recovery);
        const auto& world = surfaceLayout.world;
        const auto layout = BuildLightmapPageLayout(world, searchPath, overrides);
        if (layout.rawPageCount > SKY_LIGHTMAP_INDEX)
            throw std::runtime_error("Recovered lightmaps exceed the v22 limit of 31 pages");
        const auto surfaces = BuildSurfaceLightmapRemaps(world, layout);
        const auto vertices = BuildVertexLightmapRemaps(world, surfaces);
        auto topology = BuildTopology(world, clipMap, recovery, overrides);
        std::vector<BspLump> lumps;
        AddLump(lumps, LUMP_MATERIALS, BuildMaterials(clipMap));
        AddLump(lumps, LUMP_PLANES, std::move(topology.planes));
        AddLump(lumps, LUMP_BRUSHSIDES, BuildBrushSideData(clipMap));
        AddLump(lumps, LUMP_BRUSHSIDE_EDGE_COUNTS, BuildBrushEdgeCounts(clipMap));
        AddLump(lumps, LUMP_BRUSHEDGES, BuildBrushEdges(clipMap));
        AddLump(lumps, LUMP_BRUSHES, BuildBrushHeaders(clipMap));
        AddLump(lumps, LUMP_NODES, BuildClipNodes(clipMap, world));
        AddLump(lumps, LUMP_LEAFS, BuildClipLeafs(clipMap, world));
        AddLump(lumps, LUMP_LEAFBRUSHES, BuildLeafBrushes(clipMap));
        AddLump(lumps, LUMP_COLLISIONVERTS, BuildCollisionVerts(clipMap));
        AddLump(lumps, LUMP_COLLISIONTRIS, BuildCollisionTriIndices(clipMap));
        AddLump(lumps, LUMP_COLLISION_EDGE_WALKABLE, BuildCollisionTriEdgeIsWalkable(clipMap));
        AddLump(lumps, LUMP_COLLISIONBORDERS, BuildCollisionBorders(clipMap));
        AddLump(lumps, LUMP_COLLISIONPARTITIONS, BuildCollisionPartitions(clipMap));
        AddLump(lumps, LUMP_COLLISIONAABBS, BuildCollisionAabbTrees(clipMap));
        AddLump(lumps, LUMP_LIGHTGRID_HEADER, BuildLightGridHeader(world));
        AddLump(lumps, LUMP_LIGHTGRID_ROWS, BuildLightGridRawRows(world, overrides));
        AddLump(lumps, LUMP_LIGHTGRID_ENTRIES, BuildLightGridEntries(world));
        AddLump(lumps, LUMP_LIGHTGRID_COLORS, BuildLightGridColors(world));
        AddLump(lumps, LUMP_LIGHTMAPS, BuildLightmapImages(world, layout, searchPath, overrides));
        AddLump(lumps, LUMP_LAYERED_TRI_SOUPS, BuildSurfaces(&clipMap, world, surfaces));
        AddLump(lumps, LUMP_LAYERED_VERTS, BuildVertices(world, vertices, overrides));
        AddLump(lumps, LUMP_LAYERED_INDICES, BuildIndices(world));
        AddLump(lumps, LUMP_VERTEX_LAYER_DATA, BuildVertexLayers(world, clipMap, overrides));
        AddLump(lumps, LUMP_LAYERED_AABBTREES, std::move(topology.trees));
        AddLump(lumps, LUMP_CELLS, std::move(topology.cells));
        AddLump(lumps, LUMP_PORTALS, std::move(topology.portals));
        AddLump(lumps, LUMP_PORTALVERTS, std::move(topology.portalVertices));
        AddLump(lumps, LUMP_CULLGROUPS, std::move(topology.cullGroups));
        AddLump(lumps, LUMP_CULLGROUP_INDICES, std::move(topology.cullGroupIndices));
        AddLump(lumps, LUMP_MODELS, BuildBrushModels(&clipMap, world));
        auto probes = BuildReflectionProbes(world, searchPath, overrides);
        RecoverReflectionCapture(probes, recovery, overrides);
        AddLump(lumps, LUMP_REFLECTION_PROBES, std::move(probes));
        AddLump(lumps, LUMP_ENTITIES, BuildEntities(mapEnts, &clipMap, &world, overrides));
        AddLump(lumps, LUMP_PRIMARY_LIGHTS, BuildPrimaryLights(comWorld));
        // Build 253 predates retail light regions. Zero hull counts explicitly
        // select the PC loader's conservative fallback for each primary light.
        // R_LoadPrimaryLights derives the PC render count from ComWorld. Some
        // source games retain a different count in their compiled GfxWorld.
        if (!overrides.omitLightRegions)
        {
            std::vector<std::byte> counts(comWorld.primaryLightCount), hulls, axes;
            if (overrides.lightRegions)
            {
                if (overrides.lightRegionCount != comWorld.primaryLightCount)
                    throw std::runtime_error("Light region count differs from primary light count");
                for (unsigned i = 0; i < comWorld.primaryLightCount; ++i)
                {
                    const auto& region = overrides.lightRegions[i];
                    if (region.hullCount > UINT8_MAX || (region.hullCount && !region.hulls))
                        throw std::runtime_error("Invalid light region hull array");
                    counts[i] = static_cast<std::byte>(region.hullCount);
                    for (unsigned h = 0; h < region.hullCount; ++h)
                    {
                        const auto& hull = region.hulls[h];
                        if (hull.axisCount && !hull.axis)
                            throw std::runtime_error("Missing light region axes");
                        AppendBytes(hulls, hull.kdopMidPoint, sizeof(hull.kdopMidPoint));
                        AppendBytes(hulls, hull.kdopHalfSize, sizeof(hull.kdopHalfSize));
                        Append(hulls, hull.axisCount);
                        for (unsigned a = 0; a < hull.axisCount; ++a)
                        {
                            AppendBytes(axes, hull.axis[a].dir, sizeof(hull.axis[a].dir));
                            Append(axes, hull.axis[a].midPoint);
                            Append(axes, hull.axis[a].halfSize);
                        }
                    }
                }
            }
            else if (overrides.lightRegionCount)
                throw std::runtime_error("Missing light region array");
            AddLump(lumps, LUMP_LIGHT_REGION_COUNTS, std::move(counts));
            AddLump(lumps, LUMP_LIGHT_REGION_HULLS, std::move(hulls));
            AddLump(lumps, LUMP_LIGHT_REGION_AXES, std::move(axes));
        }
        if (gameWorld)
            AddLump(lumps, LUMP_PATHCONNECTIONS, BuildGameWorldSpPath(*gameWorld));
        WriteBsp(stream, std::move(lumps));
        if (!stream)
            return std::unexpected("Could not write BSP stream");
        return {};
    }
    catch (const std::exception& error)
    {
        return std::unexpected(error.what());
    }
}
