#pragma once

#include "Game/IW3Xenon290/IW3Xenon290.h"
#include "Loading/AssetLoadingActions.h"

namespace IW3Xenon290
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace IW3Xenon290
