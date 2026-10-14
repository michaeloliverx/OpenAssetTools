#pragma once

#include "Game/T4Xenon/T4Xenon.h"
#include "Loading/AssetLoadingActions.h"

namespace T4Xenon
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace T4Xenon
