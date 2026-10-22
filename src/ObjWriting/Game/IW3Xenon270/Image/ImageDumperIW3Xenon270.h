#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon270/IW3Xenon270.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW3Xenon270 final : public AbstractAssetDumper<IW3Xenon270::AssetImage>
    {
    public:
        DumperIW3Xenon270();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon270::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
