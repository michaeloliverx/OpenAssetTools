#pragma once

#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Image/Texture.h"
#include "SearchPath/ISearchPath.h"

#include <array>
#include <expected>
#include <functional>
#include <ostream>
#include <string>
#include <vector>

namespace IW3Xenon253::d3dbsp
{
    struct Recovery
    {
        std::vector<std::byte> reflectionProbeRecords;
        std::vector<std::string> reflectionCorrections;
        std::vector<std::vector<unsigned>> cellProbeCandidates;
        // Original spatial ordinal, exported surface index, inferred cell.
        std::vector<std::array<unsigned, 3>> recoveredSurfaceCells;
        size_t truncatedCellProbeLists = 0;
        size_t approximateProbePixels = 0;
        unsigned maximumProbeChannelError = 0;
        size_t canonicalizedProbePixels = 0;
        unsigned maximumProbeRgbmProductError = 0;
    };

    // Other games can supply decoded lighting, authored cell probe lists
    // and light regions while adapting geometry to this checked input view.
    struct SourceOverrides
    {
        std::function<std::unique_ptr<image::Texture>(const GfxImage*, image::TextureType)> loadImage;
        const std::vector<std::vector<unsigned>>* cellProbeCandidates = nullptr;
        std::string reflectionCurvePrefix = "oat253";
        bool canonicalizeReflectionRgbm = false;
        const GfxLightRegion* lightRegions = nullptr;
        unsigned lightRegionCount = 0;
        bool omitLightRegions = false;
        // Some early fastfiles retain the complete authored misc_model list.
        bool staticModelEntitiesAlreadyPresent = false;
        // PC packed normals use scale-based encoding; Xenon uses third-based.
        std::function<void(uint32_t, float (&)[3])> unpackUnitVector;
        bool bigEndianVertexLayers = true;
        bool bigEndianLightGridRows = true;
        std::function<void(size_t, float (&)[3][3])> staticModelAxis;
        std::function<bool(size_t)> staticModelNoCastShadow;
    };

    // The input structures have already undergone fastfile endian conversion.
    std::expected<void, std::string> Write(std::ostream& stream,
                                           const GfxWorld& world,
                                           const clipMap_t& clipMap,
                                           const ComWorld& comWorld,
                                           const MapEnts& mapEnts,
                                           const GameWorldSp* gameWorld,
                                           ISearchPath& searchPath,
                                           Recovery* recovery = nullptr,
                                           const SourceOverrides& overrides = {});
} // namespace IW3Xenon253::d3dbsp
