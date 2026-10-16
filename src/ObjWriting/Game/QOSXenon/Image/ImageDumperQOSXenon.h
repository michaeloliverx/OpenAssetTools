#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperQOSXenon final : public AbstractAssetDumper<QOSXenon::AssetImage>
    {
    public:
        DumperQOSXenon();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<QOSXenon::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
