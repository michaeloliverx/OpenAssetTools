#include "ImageStreamPostProcessorIW4Xenon482.h"

#include "Game/IW4Xenon482/IW4Xenon482.h"

#include <format>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <unordered_map>

using namespace IW4Xenon482;

namespace
{
    class ImageStreamPostProcessor final : public IAssetPostProcessor
    {
    public:
        ImageStreamPostProcessor(Zone& zone, ISearchPath& searchPath)
            : m_zone(zone),
              m_search_path(searchPath)
        {
        }

        asset_type_t GetHandlingAssetType() const override
        {
            return AssetImage::EnumEntry;
        }

        void PostProcessAsset(XAssetInfoGeneric& assetInfo, AssetCreationContext& context) override
        {
            const auto* image = static_cast<const GfxImage*>(assetInfo.m_ptr);
            const auto name = XAssetInfoGeneric::NormalizeAssetName(assetInfo.m_name);
            if (!image->cached || assetInfo.IsReference() || m_zone.m_xfile_image_stream_files_by_name.contains(name))
                return;

            const auto* source = assetInfo.m_zone;
            if (!source)
                throw std::runtime_error(std::format("Image \"{}\" has no source zone", name));
            const auto found = source->m_xfile_image_stream_files_by_name.find(name);
            if (found == source->m_xfile_image_stream_files_by_name.end() || found->second.empty())
                throw std::runtime_error(std::format("Image \"{}\" has no stream records", name));

            auto records = found->second.front();
            for (auto& record : records)
            {
                if (record.fileIndex != 0u || record.offsetEnd == 0u)
                    continue;

                auto& file = m_sources[source];
                if (!file)
                {
                    if (!source->m_source_path.empty())
                        file = std::make_unique<std::ifstream>(source->m_source_path, std::ios::binary);
                    else
                    {
                        file = std::move(m_search_path.Open(std::format("{}.ffm", source->m_name)).m_stream);
                        if (!file)
                            file = std::move(m_search_path.Open(std::format("{}.ff", source->m_name)).m_stream);
                    }
                    if (!file || !*file)
                        throw std::runtime_error(std::format("Could not open source fastfile for image \"{}\"", name));
                }

                auto& data = m_zone.m_xfile_image_stream_data;
                if (record.offsetEnd <= record.offset || data.size() > std::numeric_limits<uint32_t>::max()
                    || record.offsetEnd - record.offset > std::numeric_limits<uint32_t>::max() - data.size())
                    throw std::runtime_error(std::format("Invalid stream range for image \"{}\"", name));

                const auto start = data.size();
                const auto length = record.offsetEnd - record.offset;
                data.resize(start + length);
                file->clear();
                file->seekg(record.offset);
                file->read(reinterpret_cast<char*>(data.data() + start), length);
                if (file->gcount() != length)
                    throw std::runtime_error(std::format("Could not read stream for image \"{}\"", name));
                record.offset = static_cast<uint32_t>(start);
                record.offsetEnd = static_cast<uint32_t>(data.size());
            }
            m_zone.m_xfile_image_stream_files_by_name[name].push_back(records);
        }

    private:
        Zone& m_zone;
        ISearchPath& m_search_path;
        std::unordered_map<const Zone*, std::unique_ptr<std::istream>> m_sources;
    };
} // namespace

namespace image
{
    std::unique_ptr<IAssetPostProcessor> CreateStreamPostProcessorIW4Xenon482(Zone& zone, ISearchPath& searchPath)
    {
        return std::make_unique<ImageStreamPostProcessor>(zone, searchPath);
    }
} // namespace image
