#pragma once

#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Loading/AssetLoadingActions.h"

namespace IW4Xenon482
{
    class Actions_XModel final : public AssetLoadingActions
    {
    public:
        explicit Actions_XModel(Zone& zone);

        void SetModelSurfs(XModelLodInfo* lodInfo, XModelSurfs* modelSurfs) const;
    };
} // namespace IW4Xenon482
