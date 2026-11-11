#include "D3DBspDumperIW3Xenon269.h"

#include "D3DBspWriterIW3Xenon269.h"
#include "Pool/GlobalAssetPool.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <nlohmann/json.hpp>
#include <sstream>
#include <unordered_set>

using namespace IW3Xenon269;

void map_d3dbsp::DumperIW3Xenon269::DumpAsset(AssetDumpingContext& context, const XAssetInfo<GfxWorld>& asset)
{
    const auto* clip = context.m_zone.m_pools.GetAsset<AssetClipMapPvs>(asset.m_name);
    if (!clip || clip->IsReference())
    {
        con::error("Cannot dump d3dbsp \"{}\": clipmap must be loaded", asset.m_name);
        return;
    }

    const auto& world = *asset.Asset();
    nlohmann::ordered_json audit;
    audit["name"] = asset.m_name;
    audit["countFields"] = {"surfaces", "vertices", "indices", "layerBytes", "cells", "lightmaps", "reflectionProbes", "materials"};
    audit["counts"] = {world.surfaceCount,
                       world.vertexCount,
                       world.indexCount,
                       world.vertexLayerDataSize,
                       world.cellCount,
                       world.lightmapCount,
                       world.reflectionProbeCount,
                       clip->Asset()->numMaterials};
    if (world.sortedSurfIndex && world.models && world.modelCount)
        audit["surfaceOrder"] = std::vector<uint16_t>(world.sortedSurfIndex, world.sortedSurfIndex + world.models[0].surfaceCount);
    for (int i = 0; i < world.modelCount; ++i)
        audit["modelRanges"].push_back({world.models[i].startSurfIndex, world.models[i].surfaceCount});
    for (unsigned i = 0; i < clip->Asset()->numMaterials; ++i)
        audit["materials"].push_back(clip->Asset()->materials[i].material);
    for (int i = 0; i < world.surfaceCount; ++i)
    {
        const auto& surface = world.surfaces[i];
        audit["surfaces"].push_back({
            {"material",    surface.material ? surface.material->info.name : ""},
            {"layerOffset", surface.tris.vertexLayerData                       },
            {"firstVertex", surface.tris.firstVertex                           },
            {"vertexCount", surface.tris.vertexCount                           },
            {"indexCount",  surface.tris.indexCount                            },
            {"baseIndex",   surface.tris.baseIndex                             }
        });
    }
    std::unordered_set<const Material*> materials;
    const auto addMaterial = [&](const Material* material)
    {
        if (material && material->info.name && material->info.name[0] == ',')
        {
            const auto* resolved = GameGlobalAssetPools::GetGlobalPoolsForGame(context.m_zone.m_game_id)->GetAsset<AssetMaterial>(material->info.name + 1);
            if (resolved && !resolved->IsReference())
                material = resolved->Asset();
        }
        if (!material || !materials.insert(material).second)
            return;
        nlohmann::ordered_json entry{
            {"name",         material->info.name                                       },
            {"techniqueSet", material->techniqueSet ? material->techniqueSet->name : ""},
            {"constants",    nlohmann::ordered_json::array()                           },
            {"textures",     nlohmann::ordered_json::array()                           }
        };
        for (unsigned c = 0; c < material->constantCount; ++c)
        {
            const auto& constant = material->constantTable[c];
            const auto end = std::find(std::begin(constant.name), std::end(constant.name), '\0');
            entry["constants"].push_back({
                {"name", std::string(constant.name, end)},
                {"hash", constant.nameHash},
                {"literal", constant.literal}
            });
        }
        for (unsigned j = 0; j < material->textureCount; ++j)
        {
            const auto& texture = material->textureTable[j];
            entry["textures"].push_back({
                {"hash",     texture.nameHash},
                {"semantic", texture.semantic},
                {"image",
                 texture.semantic == 11 ? "<water>"
                 : texture.u.image      ? texture.u.image->name
                                        : "" }
            });
        }
        audit["renderMaterials"].push_back(std::move(entry));
    };
    for (int i = 0; i < world.surfaceCount; ++i)
        addMaterial(world.surfaces[i].material);
    for (const auto* info : context.m_zone.m_pools.PoolAssets<AssetMaterial>())
        if (!info->IsReference())
            addMaterial(info->Asset());
    const auto imageInfo = [](const GfxImage* image) -> nlohmann::ordered_json
    {
        if (!image)
            return nullptr;
        return {
            {"name",   image->name                                                },
            {"width",  image->width                                               },
            {"height", image->height                                              },
            {"format", image->texture.loadDef ? image->texture.loadDef->format : 0},
            {"bytes",  image->cardMemory.platform[0]                              }
        };
    };
    for (int i = 0; i < world.lightmapCount; ++i)
        audit["lightmaps"].push_back({imageInfo(world.lightmaps[i].primary), imageInfo(world.lightmaps[i].secondary)});
    for (unsigned i = 0; i < world.reflectionProbeCount; ++i)
        audit["probes"].push_back(imageInfo(world.reflectionProbes[i].reflectionImage));
    if (const auto debug = context.OpenAssetFile(asset.m_name + ".debug.json"))
        *debug << audit.dump(2);

    const auto gridName = asset.m_name + ".source-lightgrid.bin";
    if (const auto grid = context.OpenAssetFile(gridName))
    {
        // Host-endian uint32 pairs and byte colors, prefixed by their counts.
        const auto& lightGrid = world.lightGrid;
        grid->write("LG269LE\0", 8);
        grid->write(reinterpret_cast<const char*>(&lightGrid.entryCount), 4);
        grid->write(reinterpret_cast<const char*>(&lightGrid.colorCount), 4);
        grid->write(reinterpret_cast<const char*>(lightGrid.entries), lightGrid.entryCount * sizeof(GfxLightGridEntry));
        grid->write(reinterpret_cast<const char*>(lightGrid.colors), lightGrid.colorCount * sizeof(GfxLightGridColors));
    }
    IW3Xenon269::d3dbsp::Recovery recovery;
    std::ostringstream output(std::ios::binary);
    const auto result = IW3Xenon269::d3dbsp::Write(output, *asset.Asset(), *clip->Asset(), context.m_obj_search_path, recovery);
    if (!result)
    {
        con::error("Cannot dump d3dbsp \"{}\": {}", asset.m_name, result.error());
        return;
    }
    const auto file = context.OpenAssetFile(asset.m_name);
    if (file)
    {
        *file << output.view();
        nlohmann::ordered_json report{
            {"format",                   "IW3 IBSP v22"                                                                         },
            {"source",                   "IW3 Xenon version 269 runtime assets"                                                 },
            {"reflectionCorrections",    recovery.bsp.reflectionCorrections                                                     },
            {"reflectionCorrectionFile", "reflections/reflections.csv"                                                          },
            {"reflectionErrorMetric",    "decoded BGRA bytes after the reconstructed SDK correction, before texture compression"},
            {"approximateProbePixels",   recovery.bsp.approximateProbePixels                                                    },
            {"maximumProbeChannelError", recovery.bsp.maximumProbeChannelError                                                  },
            {"unrecoverable",
             {"original pre-pack vertex tangent frames",
              "original lightmap page order",
              "source node bounds",
              "original reflection captures and correction names"}                                                              }
        };
        report["version269"] = recovery.source;
        report["version269"]["lightGrid"]["sourceFile"] = gridName;
        report["version269"]["sunBytes"] = world.sunLight ? nlohmann::ordered_json(world.sunLight->unknown) : nlohmann::ordered_json(nullptr);
        report["cellProbeCandidates"] = recovery.bsp.cellProbeCandidates;
        report["truncatedCellProbeLists"] = recovery.bsp.truncatedCellProbeLists;
        report["recoveredSurfaceCellFields"] = {"originalSpatialOrdinal", "exportedSurfaceIndex", "inferredCell"};
        report["recoveredSurfaceCells"] = recovery.bsp.recoveredSurfaceCells;
        if (!recovery.bsp.reflectionProbeRecords.empty())
        {
            if (const auto corrections = context.OpenAssetFile("reflections/reflections.csv"))
                *corrections << "name,black_level,white_level,gamma,saturation\n"
                                "default,0.3,0.7,1.2,0.5\n"
                                "oat_iw3_269_default,0.3,0.7,1.2,0.5\n"
                                "oat_iw3_269_unsaturated,0.3,0.7,1.2,1.0\n"
                                "oat_iw3_269_hdr,0.3,0.5,1.2,1.0\n";
            const auto name = asset.m_name + ".reflections.bin";
            if (const auto probes = context.OpenAssetFile(name))
                probes->write(reinterpret_cast<const char*>(recovery.bsp.reflectionProbeRecords.data()), recovery.bsp.reflectionProbeRecords.size());
            report["decodedRuntimeProbeRecords"] = name;
            report["probeRecordLayout"] = "float3 origin, char[64] correction label, byte[131064] BGRA RGBM pixels; six base faces then each face's mip tail";
        }
        if (const auto metadata = context.OpenAssetFile(asset.m_name + ".recovery.json"))
            *metadata << report.dump(2);
        if (recovery.bsp.approximateProbePixels)
            con::info("BSP reflection reconstruction approximated {} pixels (maximum channel error {}); original decoded probes retained in recovery sidecar",
                      recovery.bsp.approximateProbePixels,
                      recovery.bsp.maximumProbeChannelError);
        if (recovery.bsp.truncatedCellProbeLists)
            con::info("BSP has {} cells exceeding the PC limit of 64 reflection candidates; complete candidate lists retained in recovery metadata",
                      recovery.bsp.truncatedCellProbeLists);
        if (!recovery.bsp.recoveredSurfaceCells.empty())
            con::info("BSP recovered cell coverage for {} surfaces absent from the alpha cell trees; assignments retained in recovery metadata",
                      recovery.bsp.recoveredSurfaceCells.size());
    }
}
