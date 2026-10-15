#pragma once

#include "Game/NX1/NX1.h"
#include "Loading/AssetLoadingActions.h"

namespace NX1
{
    class Actions_XModel final : public AssetLoadingActions
    {
    public:
        explicit Actions_XModel(Zone& zone);

        void SetModelSurfs(XModelLodInfo* lodInfo, XModelSurfs* modelSurfs) const;
    };
} // namespace NX1
