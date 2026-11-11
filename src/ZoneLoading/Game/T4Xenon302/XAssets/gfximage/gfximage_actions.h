#pragma once

#include "Game/T4Xenon302/T4Xenon302.h"
#include "Loading/AssetLoadingActions.h"

namespace T4Xenon302
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace T4Xenon302
