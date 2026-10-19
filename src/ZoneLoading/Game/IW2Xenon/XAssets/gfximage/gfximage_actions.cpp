#include "gfximage_actions.h"

using namespace IW2Xenon;

Actions_GfxImage::Actions_GfxImage(Zone& zone)
    : AssetLoadingActions(zone)
{
}

void Actions_GfxImage::LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const
{
    if (!loadDef || !image)
        return;

    // Load_Texture rebuilds these fields from the load definition.
    image->mapType = (loadDef->flags & IMG_FLAG_CUBEMAP) ? MAPTYPE_CUBE : (loadDef->flags & IMG_FLAG_VOLMAP) ? MAPTYPE_3D : MAPTYPE_2D;
    image->width = static_cast<unsigned short>(loadDef->dimensions[0]);
    image->height = static_cast<unsigned short>(loadDef->dimensions[1]);
    image->depth = static_cast<unsigned short>(loadDef->dimensions[2]);
}
