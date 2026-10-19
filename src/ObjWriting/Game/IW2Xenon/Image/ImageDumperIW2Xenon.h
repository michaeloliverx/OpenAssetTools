#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW2Xenon/IW2Xenon.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW2Xenon final : public AbstractAssetDumper<IW2Xenon::AssetImage>
    {
    public:
        DumperIW2Xenon();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW2Xenon::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
