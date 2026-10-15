#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/NX1/NX1.h"
#include "Image/ImageWriter.h"

#include <memory>

namespace image
{
    class DumperNX1 final : public AbstractAssetDumper<NX1::AssetImage>
    {
    public:
        DumperNX1();

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<NX1::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<ImageWriter> m_writer;
    };
} // namespace image
