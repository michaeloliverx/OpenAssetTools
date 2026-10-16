#pragma once

#include "Image/ImageToCommonConverter.h"

namespace image
{
    class ToCommonConverterT5Xenon final : public ToCommonConverter
    {
    public:
        std::unique_ptr<Texture> Convert(const XAssetInfoGeneric& assetInfo, ISearchPath& searchPath) override;
    };
} // namespace image
