#include "gfximage_actions.h"

#include <cassert>
#include <cstring>

using namespace IW3Xenon;

Actions_GfxImage::Actions_GfxImage(Zone& zone)
    : AssetLoadingActions(zone)
{
}

void Actions_GfxImage::LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const
{
    if (!loadDef || !image)
    {
        return;
    }
    // GfxImageLoadDef is loaded in the TEMP block which will get destroyed so we need to copy data

    auto* new_loadDef = static_cast<GfxImageLoadDef*>(m_zone.Memory().AllocRaw(sizeof(GfxImageLoadDef)));

    memcpy(new_loadDef, loadDef, sizeof(GfxImageLoadDef));

    // Deep copy the D3D texture object (52 bytes)
    auto* new_tex = static_cast<DummyD3DTexture*>(m_zone.Memory().AllocRaw(sizeof(DummyD3DTexture)));

    memcpy(new_tex, loadDef->texture.map, sizeof(DummyD3DTexture));

    new_loadDef->texture.map = new_tex;
    image->texture.loadDef = new_loadDef;
}
