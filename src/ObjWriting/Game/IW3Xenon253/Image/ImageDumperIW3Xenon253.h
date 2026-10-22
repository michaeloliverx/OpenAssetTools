#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon253/IW3Xenon253.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW3Xenon253 final : public AbstractAssetDumper<IW3Xenon253::AssetImage>
    {
    public:
        DumperIW3Xenon253();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon253::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
