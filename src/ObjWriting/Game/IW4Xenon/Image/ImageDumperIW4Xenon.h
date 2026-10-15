#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW4Xenon/IW4Xenon.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW4Xenon final : public AbstractAssetDumper<IW4Xenon::AssetImage>
    {
    public:
        DumperIW4Xenon();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW4Xenon::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
