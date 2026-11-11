#pragma once

#include "Game/IW3Xenon253/Maps/D3DBspWriterIW3Xenon253.h"
#include "Game/T4Xenon204/T4Xenon204.h"

#include <nlohmann/json.hpp>

namespace T4Xenon204::d3dbsp
{
    struct Recovery
    {
        IW3Xenon253::d3dbsp::Recovery bsp;
        nlohmann::ordered_json source;
    };

    // Reconstruct a CoD4 PC IBSP v22, retaining T4 version 204-specific recovery data.
    std::expected<void, std::string> Write(std::ostream& stream, const GfxWorld& world, const clipMap_t& clipMap, ISearchPath& searchPath, Recovery& recovery);
} // namespace T4Xenon204::d3dbsp
