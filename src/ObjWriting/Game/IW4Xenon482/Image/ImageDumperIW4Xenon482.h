#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW4Xenon482 final : public AbstractAssetDumper<IW4Xenon482::AssetImage>
    {
    public:
        DumperIW4Xenon482();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon482::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
