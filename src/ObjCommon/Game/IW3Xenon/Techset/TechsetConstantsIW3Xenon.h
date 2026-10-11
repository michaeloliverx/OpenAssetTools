#pragma once

#include "Game/IW3/Techset/TechsetConstantsIW3.h"
#include "Game/IW3Xenon/IW3Xenon.h"

namespace IW3Xenon
{
    inline constexpr size_t PIXEL_SHADER_RUNTIME_HEADER_SIZE = 40u;
    inline constexpr size_t VERTEX_SHADER_RUNTIME_HEADER_SIZE = 872u;

    inline const char* techniqueTypeNames[]{
        "depth prepass",
        "build floatz",
        "build shadowmap depth",
        "build shadowmap color",
        "unlit",
        "emissive",
        "emissive shadow",
        "lit",
        "lit sun",
        "lit sun shadow",
        "lit spot",
        "lit spot shadow",
        "lit omni",
        "lit omni shadow",
        "light spot",
        "light omni",
        "light spot shadow",
        "fakelight normal",
        "fakelight view",
        "sunlight preview",
        "case texture",
        "solid wireframe",
        "shaded wireframe",
        "shadowcookie caster",
        "shadowcookie receiver",
        "debug bumpmap",
    };
    static_assert(std::size(techniqueTypeNames) == TECHNIQUE_COUNT);
    inline techset::CommonTechniqueTypeNames commonTechniqueTypeNames(techniqueTypeNames, std::size(techniqueTypeNames));

    inline constexpr const char* vertexDeclNames[]{
        "generic",
        "packed",
        "world",
        "world_t1n0",
        "world_t1n1",
        "world_t2n0",
        "world_t2n1",
        "world_t2n2",
        "world_t3n0",
        "world_t3n1",
        "world_t3n2",
        "world_t4n0",
        "world_t4n1",
        "world_t4n2",
        "pos_tex",
    };
    static_assert(std::size(vertexDeclNames) == VERTDECL_COUNT);

    // IW3 uses the same code constants, sampler sources and stream names on both platforms.
    static_assert(static_cast<unsigned>(CONST_SRC_CODE_WORLD_MATRIX) == IW3::CONST_SRC_CODE_WORLD_MATRIX);
    static_assert(static_cast<unsigned>(CONST_SRC_TOTAL_COUNT) == IW3::CONST_SRC_TOTAL_COUNT);
    static_assert(static_cast<unsigned>(STREAM_SRC_COUNT) == IW3::STREAM_SRC_COUNT);
    static_assert(static_cast<unsigned>(STREAM_DST_COUNT) == IW3::STREAM_DST_COUNT);
    using IW3::commonCodeSourceInfos;
    using IW3::commonRoutingInfos;
} // namespace IW3Xenon
