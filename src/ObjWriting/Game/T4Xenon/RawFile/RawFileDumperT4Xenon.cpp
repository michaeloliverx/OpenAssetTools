#include "RawFileDumperT4Xenon.h"

using namespace T4Xenon;

namespace raw_file
{
    void DumperT4Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<RawFile>& asset)
    {
        const auto* rawFile = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(rawFile->buffer, rawFile->len);
    }
} // namespace raw_file
