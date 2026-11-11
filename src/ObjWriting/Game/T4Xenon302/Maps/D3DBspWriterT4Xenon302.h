#pragma once

#include "Game/IW3Xenon253/Maps/D3DBspWriterIW3Xenon253.h"
#include "Game/T4Xenon302/T4Xenon302.h"

#include <nlohmann/json.hpp>

namespace T4Xenon302::d3dbsp
{
    struct Recovery
    {
        IW3Xenon253::d3dbsp::Recovery bsp;
        nlohmann::ordered_json source;
    };

    // Reconstruct a CoD4 PC IBSP v22, retaining T4 version 302-specific recovery data.
    std::expected<void, std::string>
        Write(std::ostream& stream, const GfxWorld& world, const clipMap_t& clipMap, const ComWorld& comWorld, ISearchPath& searchPath, Recovery& recovery);
} // namespace T4Xenon302::d3dbsp
