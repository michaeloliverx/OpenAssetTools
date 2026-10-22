#include "RawFileDumperIW3Xenon270.h"

using namespace IW3Xenon270;

namespace raw_file
{
    void DumperIW3Xenon270::DumpAsset(AssetDumpingContext& context, const XAssetInfo<RawFile>& asset)
    {
        const auto* rawFile = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(rawFile->buffer, rawFile->len);
    }
} // namespace raw_file
