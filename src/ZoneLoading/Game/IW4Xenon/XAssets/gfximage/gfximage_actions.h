#pragma once

#include "Game/IW4Xenon/IW4Xenon.h"
#include "Loading/AssetLoadingActions.h"
#include "Zone/Zone.h"

namespace IW4Xenon
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);

        void StoreImageStreamFiles(GfxImage* image);
    };
} // namespace IW4Xenon
