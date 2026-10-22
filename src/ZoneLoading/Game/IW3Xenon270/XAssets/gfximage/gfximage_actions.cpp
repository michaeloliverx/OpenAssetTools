#include "gfximage_actions.h"

#include <cstring>

using namespace IW3Xenon270;

Actions_GfxImage::Actions_GfxImage(Zone& zone)
    : AssetLoadingActions(zone)
{
}

void Actions_GfxImage::LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const
{
    if (!loadDef || !image)
        return;

    // Retain the headers after the temp block is reused.
    auto* newLoadDef = static_cast<GfxImageLoadDef*>(m_zone.Memory().AllocRaw(sizeof(GfxImageLoadDef)));
    std::memcpy(newLoadDef, loadDef, sizeof(GfxImageLoadDef));

    if (loadDef->texture.map)
    {
        auto* newTexture = static_cast<D3DTexture*>(m_zone.Memory().AllocRaw(sizeof(D3DTexture)));
        std::memcpy(newTexture, loadDef->texture.map, sizeof(D3DTexture));
        newLoadDef->texture.map = newTexture;
    }

    image->texture.loadDef = newLoadDef;
}
