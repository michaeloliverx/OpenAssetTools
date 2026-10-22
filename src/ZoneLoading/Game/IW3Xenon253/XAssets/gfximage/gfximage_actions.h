#pragma once

#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Loading/AssetLoadingActions.h"

namespace IW3Xenon253
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace IW3Xenon253
