#pragma once

#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Loading/AssetLoadingActions.h"
#include "Zone/Zone.h"

namespace IW4Xenon482
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);

        void StoreImageStreamFiles(GfxImage* image);
    };
} // namespace IW4Xenon482
