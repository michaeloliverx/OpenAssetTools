#include "XModelHighMipBoundsIW3Xenon.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>

using namespace IW3Xenon;

XModelHighMipBounds* xmodel::GenerateHighMipBoundsIW3Xenon(const XModel& model, MemoryManager& memory)
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

        std::fill(std::begin(surfaceBounds.mins.v), std::end(surfaceBounds.mins.v), std::numeric_limits<float>::max());
        std::fill(std::begin(surfaceBounds.maxs.v), std::end(surfaceBounds.maxs.v), std::numeric_limits<float>::lowest());
        for (auto vertexIndex = 0u; vertexIndex < surface.vertCount; ++vertexIndex)
        {
            const auto& position = surface.verts0[vertexIndex].xyz;
            for (auto axis = 0u; axis < 3u; ++axis)
            {
                surfaceBounds.mins.v[axis] = std::min(surfaceBounds.mins.v[axis], position.v[axis]);
                surfaceBounds.maxs.v[axis] = std::max(surfaceBounds.maxs.v[axis], position.v[axis]);
            }
        }
    }

    return bounds;
}
