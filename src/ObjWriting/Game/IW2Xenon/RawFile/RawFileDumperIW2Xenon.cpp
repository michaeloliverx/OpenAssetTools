#include "RawFileDumperIW2Xenon.h"

using namespace IW2Xenon;

namespace raw_file
{
    void DumperIW2Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<RawFile>& asset)
    {
        const auto* rawFile = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(rawFile->buffer, rawFile->len);
    }
} // namespace raw_file
