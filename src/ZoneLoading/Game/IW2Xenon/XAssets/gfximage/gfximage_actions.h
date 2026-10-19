#pragma once

#include "Game/IW2Xenon/IW2Xenon.h"
#include "Loading/AssetLoadingActions.h"

namespace IW2Xenon
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace IW2Xenon
