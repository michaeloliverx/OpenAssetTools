#pragma once

#include "Game/NX1/NX1.h"
#include "Loading/AssetLoadingActions.h"
#include "Zone/Zone.h"

namespace NX1
{
    class Actions_GfxImage final : public AssetLoadingActions
    {
    public:
        explicit Actions_GfxImage(Zone& zone);

        void StoreImageStreamFiles(GfxImage* image);
    };
} // namespace NX1
