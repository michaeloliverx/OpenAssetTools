#pragma once

#include "XModel/XModelToCommonConverter.h"

namespace xmodel
{
    class ToCommonConverterIW3Xenon270 final : public ToCommonConverter
    {
    public:
        std::optional<XModelCommon> Convert(const XAssetInfoGeneric& assetInfo, unsigned lod) override;
    };
} // namespace xmodel
