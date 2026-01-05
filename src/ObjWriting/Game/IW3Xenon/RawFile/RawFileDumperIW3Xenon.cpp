#include "RawFileDumperIW3Xenon.h"

using namespace IW3Xenon;

namespace raw_file
{
    DumperIW3Xenon::DumperIW3Xenon(const AssetPool<AssetRawFile::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void DumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<RawFile>& asset)
    {
        const auto* rawFile = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(rawFile->buffer, rawFile->len);
    }
} // namespace raw_file
