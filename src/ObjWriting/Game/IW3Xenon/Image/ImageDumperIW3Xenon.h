#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/IW3Xenon/IW3Xenon.h"
#include "Image/IImageWriter.h"

#include <memory>

namespace image
{
    class DumperIW3Xenon final : public AbstractAssetDumper<IW3Xenon::AssetImage>
    {
    public:
        explicit DumperIW3Xenon(const AssetPool<IW3Xenon::AssetImage::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<IW3Xenon::AssetImage::Type>& asset) override;

    private:
        std::unique_ptr<IImageWriter> m_writer;
    };
} // namespace image
