#include "SoundDumperIW3Xenon.h"

#include "Utils/Logging/Log.h"
#include "Utils/StringUtils.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <format>
#include <limits>
#include <string>

using namespace IW3Xenon;

namespace
{
    constexpr unsigned int RAW_FILE_INDEX = 0u;
    constexpr unsigned int LOOSE_FILE_INDEX = std::numeric_limits<unsigned int>::max();
    constexpr auto COPY_BUFFER_SIZE = 64uz * 1024uz;

    std::string GetPackfileName(const unsigned int fileIndex)
    {
        return std::format("packfile{}.pak", fileIndex);
    }

    std::string GetFileNameForStreamedSound(const std::string& assetName, const size_t aliasIndex)
    {
        return std::format("sound/{}.{}.xma", assetName, aliasIndex);
    }

    void CopyStreamedSound(AssetDumpingContext& context, const StreamFileName& filename, const std::string& assetName, const size_t aliasIndex)
    {
        std::string sourceName;
        int64_t offset = 0;
        int64_t length = 0;
        // These indices select StreamFileNameRaw in the zone format.
        const auto isRaw = filename.fileIndex == RAW_FILE_INDEX || filename.fileIndex == LOOSE_FILE_INDEX;
        if (isRaw)
        {
            const auto& raw = filename.info.raw;
            if (!raw.name || !*raw.name)
                return;
            if ((!raw.dir || !*raw.dir) && utils::StringEqualsIgnoreCase(raw.name, "null"))
                return;
            auto path = std::filesystem::path(raw.dir ? raw.dir : "") / raw.name;
            path.replace_extension(".xma");
            sourceName = path.generic_string();
        }
        else
        {
            sourceName = GetPackfileName(filename.fileIndex);
            offset = filename.info.packed.offset;
            length = filename.info.packed.length;
        }
        const auto source = context.m_obj_search_path.Open(sourceName);
        if (!source.IsOpen())
        {
            con::warn("Could not find streamed sound data for \"{}\" in \"{}\"", assetName, sourceName);
            return;
        }
        if (isRaw)
            length = source.m_length;
        if (source.m_length < 0 || offset > source.m_length || length <= 0 || length > source.m_length - offset)
        {
            con::warn("Streamed sound data for \"{}\" is outside \"{}\"", assetName, sourceName);
            return;
        }
        source.m_stream->seekg(offset);
        if (!*source.m_stream)
        {
            con::warn("Failed to seek streamed sound data for \"{}\" in \"{}\"", assetName, sourceName);
            return;
        }
        const auto assetFile = context.OpenAssetFile(GetFileNameForStreamedSound(assetName, aliasIndex));
        if (!assetFile)
            return;
        auto bytesRemaining = static_cast<uint64_t>(length);
        std::array<char, COPY_BUFFER_SIZE> copyBuffer;
        while (bytesRemaining > 0)
        {
            const auto bytesToCopy = std::min(bytesRemaining, static_cast<uint64_t>(copyBuffer.size()));
            source.m_stream->read(copyBuffer.data(), static_cast<std::streamsize>(bytesToCopy));
            const auto bytesRead = source.m_stream->gcount();
            if (bytesRead != static_cast<std::streamsize>(bytesToCopy))
            {
                con::warn("Failed to read streamed sound data for \"{}\" from \"{}\"", assetName, sourceName);
                return;
            }

            assetFile->write(copyBuffer.data(), bytesRead);
            bytesRemaining -= bytesToCopy;
        }
    }
} // namespace

namespace sound
{
    void SoundDumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<snd_alias_list_t>& asset)
    {
        const auto* sound = asset.Asset();
        if (!sound->head || sound->count <= 0)
            return;

        for (auto aliasIndex = 0uz; aliasIndex < static_cast<size_t>(sound->count); ++aliasIndex)
        {
            const auto& alias = sound->head[aliasIndex];
            if (!alias.soundFile || alias.soundFile->type != SAT_STREAMED)
                continue;

            CopyStreamedSound(context, alias.soundFile->u.streamSnd.filename, asset.m_name, aliasIndex);
        }
    }
} // namespace sound
