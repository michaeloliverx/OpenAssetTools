#include "MapEntsTriggerMapDumperIW4.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Utils/Logging/Log.h"

#include <format>

namespace map_ents
{
    namespace
    {
        constexpr auto TRIGGER_MATERIAL = "trigger";

        bool IsValidRange(const unsigned int first, const unsigned int count, const unsigned int total)
        {
            return first <= total && count <= total - first;
        }

        void WriteBounds(MapFileDumper& mapFileDumper, const IW4::Bounds& bounds)
        {
            const auto minX = bounds.midPoint.x - bounds.halfSize.x;
            const auto maxX = bounds.midPoint.x + bounds.halfSize.x;
            const auto minY = bounds.midPoint.y - bounds.halfSize.y;
            const auto maxY = bounds.midPoint.y + bounds.halfSize.y;
            const auto minZ = bounds.midPoint.z - bounds.halfSize.z;
            const auto maxZ = bounds.midPoint.z + bounds.halfSize.z;

            mapFileDumper.WriteBrushPlane(
                {
                    {1.0f, 0.0f, 0.0f},
                    minX
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {-1.0f, 0.0f, 0.0f},
                    -maxX
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {0.0f, 1.0f, 0.0f},
                    minY
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {0.0f, -1.0f, 0.0f},
                    -maxY
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {0.0f, 0.0f, 1.0f},
                    minZ
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {0.0f, 0.0f, -1.0f},
                    -maxZ
            },
                TRIGGER_MATERIAL);
        }

        void WriteSlab(MapFileDumper& mapFileDumper, const IW4::TriggerSlab& slab)
        {
            const auto minDistance = slab.midPoint - slab.halfSize;
            const auto maxDistance = slab.midPoint + slab.halfSize;

            mapFileDumper.WriteBrushPlane(
                {
                    {slab.dir[0], slab.dir[1], slab.dir[2]},
                    minDistance
            },
                TRIGGER_MATERIAL);
            mapFileDumper.WriteBrushPlane(
                {
                    {-slab.dir[0], -slab.dir[1], -slab.dir[2]},
                    -maxDistance
            },
                TRIGGER_MATERIAL);
        }
    } // namespace

    void DumpMapEntsTriggerMapIW4(const std::string& mapEntsName, const AssetDumpingContext& context, const IW4::MapTriggers& triggers)
    {
        if (triggers.count == 0u || !triggers.models || triggers.hullCount == 0u || !triggers.hulls)
            return;

        const auto assetFile = context.OpenAssetFile(std::format("{}.map", mapEntsName));
        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();
        mapFileDumper.BeginEntity();
        mapFileDumper.WriteKeyValue("classname", "worldspawn");

        for (auto modelIndex = 0u; modelIndex < triggers.count; modelIndex++)
        {
            const auto& model = triggers.models[modelIndex];
            if (!IsValidRange(model.firstHull, model.hullCount, triggers.hullCount))
            {
                con::warn("Cannot dump trigger model {} of map ents \"{}\": invalid hull range", modelIndex, mapEntsName);
                continue;
            }

            mapFileDumper.WriteComment(std::format("trigger model {}", modelIndex));

            for (auto modelHullIndex = 0u; modelHullIndex < model.hullCount; modelHullIndex++)
            {
                const auto hullIndex = model.firstHull + modelHullIndex;
                const auto& hull = triggers.hulls[hullIndex];
                if (hull.slabCount > 0u && (!triggers.slabs || !IsValidRange(hull.firstSlab, hull.slabCount, triggers.slabCount)))
                {
                    con::warn("Cannot dump trigger hull {} of map ents \"{}\": invalid slab range", hullIndex, mapEntsName);
                    continue;
                }

                mapFileDumper.BeginBrush();
                WriteBounds(mapFileDumper, hull.bounds);

                for (auto hullSlabIndex = 0u; hullSlabIndex < hull.slabCount; hullSlabIndex++)
                    WriteSlab(mapFileDumper, triggers.slabs[hull.firstSlab + hullSlabIndex]);

                mapFileDumper.EndBrush();
            }
        }

        mapFileDumper.EndEntity();
    }
} // namespace map_ents
