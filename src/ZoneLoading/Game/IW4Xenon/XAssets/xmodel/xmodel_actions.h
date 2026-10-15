#pragma once

#include "Game/IW4Xenon/IW4Xenon.h"
#include "Loading/AssetLoadingActions.h"

namespace IW4Xenon
{
    class Actions_XModel final : public AssetLoadingActions
    {
    public:
        explicit Actions_XModel(Zone& zone);

        void SetModelSurfs(XModelLodInfo* lodInfo, XModelSurfs* modelSurfs) const;
    };
} // namespace IW4Xenon
