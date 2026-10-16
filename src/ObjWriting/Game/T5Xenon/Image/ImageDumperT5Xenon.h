#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T5Xenon/T5Xenon.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperT5Xenon final : public AbstractAssetDumper<T5Xenon::AssetImage>
    {
    public:
        DumperT5Xenon();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T5Xenon::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
