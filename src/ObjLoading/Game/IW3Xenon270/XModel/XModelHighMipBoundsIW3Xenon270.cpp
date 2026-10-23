#include "XModelHighMipBoundsIW3Xenon270.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>

using namespace IW3Xenon270;

XModelHighMipBounds* xmodel::GenerateHighMipBoundsIW3Xenon270(const XModel& model, MemoryManager& memory)
{
    const auto& lod = model.lodInfo[0];
    if (!lod.numsurfs)
        return nullptr;

    assert(lod.surfIndex + lod.numsurfs <= model.numsurfs);
    auto* bounds = memory.Alloc<XModelHighMipBounds>(lod.numsurfs);
    for (auto surfaceIndex = 0u; surfaceIndex < lod.numsurfs; ++surfaceIndex)
    {
        const auto& surface = model.surfs[lod.surfIndex + surfaceIndex];
        auto& surfaceBounds = bounds[surfaceIndex];
        if (!surface.vertCount)
            continue;

        std::fill(std::begin(surfaceBounds.mins), std::end(surfaceBounds.mins), std::numeric_limits<float>::max());
        std::fill(std::begin(surfaceBounds.maxs), std::end(surfaceBounds.maxs), std::numeric_limits<float>::lowest());
        for (auto vertexIndex = 0u; vertexIndex < surface.vertCount; ++vertexIndex)
        {
            const auto& position = surface.verts[vertexIndex].xyz;
            for (auto axis = 0u; axis < 3u; ++axis)
            {
                surfaceBounds.mins[axis] = std::min(surfaceBounds.mins[axis], position[axis]);
                surfaceBounds.maxs[axis] = std::max(surfaceBounds.maxs[axis], position[axis]);
            }
        }
    }

    return bounds;
}
