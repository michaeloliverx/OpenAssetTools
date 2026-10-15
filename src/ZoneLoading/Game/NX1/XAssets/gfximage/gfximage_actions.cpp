#include "gfximage_actions.h"

#include "Pool/XAssetInfo.h"

using namespace NX1;

Actions_GfxImage::Actions_GfxImage(Zone& zone)
    : AssetLoadingActions(zone)
{
}

void Actions_GfxImage::StoreImageStreamFiles(GfxImage* image)
{
    if (image == nullptr || !image->cached)
        return;

    XAssetStreamFilesForImage streamFiles{};

    for (auto streamIndex = 0uz; streamIndex < streamFiles.size(); streamIndex++)
    {
        if (m_zone.m_xfile_image_stream_read_index < m_zone.m_xfile_image_stream_files.size())
        {
            streamFiles[streamIndex] = m_zone.m_xfile_image_stream_files[m_zone.m_xfile_image_stream_read_index++];
        }
    }

    const auto imageName = XAssetInfoGeneric::NormalizeAssetName(AssetName<AssetImage>(*image));
    m_zone.m_xfile_image_stream_files_by_name[imageName].emplace_back(streamFiles);
}
