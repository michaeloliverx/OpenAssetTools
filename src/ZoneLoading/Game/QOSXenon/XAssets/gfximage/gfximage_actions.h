#pragma once

#include "Game/QOSXenon/QOSXenon.h"
#include "Loading/AssetLoadingActions.h"

namespace QOSXenon
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);
        void LoadImageData(GfxImageLoadDef* loadDef, GfxImage* image) const;
    };
} // namespace QOSXenon
