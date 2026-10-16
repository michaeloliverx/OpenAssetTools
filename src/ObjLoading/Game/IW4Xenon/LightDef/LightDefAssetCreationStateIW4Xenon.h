#pragma once

#include "Asset/AssetCreationContext.h"
#include "Asset/IZoneAssetCreationState.h"
#include "Game/IW4Xenon/IW4Xenon.h"

namespace IW4Xenon
{
    class LightDefAssetCreationState : public IZoneAssetCreationState
    {
    public:
        LightDefAssetCreationState();

        void SetLightDefLookupStart(GfxLightDef* lightDef, AssetCreationContext& context);

    private:
        int m_lmap_pixels_used_for_falloff;
    };
} // namespace IW4Xenon
