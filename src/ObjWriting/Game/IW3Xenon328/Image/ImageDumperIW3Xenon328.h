#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon328/IW3Xenon328.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW3Xenon328 final : public AbstractAssetDumper<IW3Xenon328::AssetImage>
    {
    public:
        DumperIW3Xenon328();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon328::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
