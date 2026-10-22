#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon290/IW3Xenon290.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW3Xenon290 final : public AbstractAssetDumper<IW3Xenon290::AssetImage>
    {
    public:
        DumperIW3Xenon290();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon290::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
