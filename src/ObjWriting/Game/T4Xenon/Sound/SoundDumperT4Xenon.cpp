#include "SoundDumperT4Xenon.h"

#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <format>
#include <string>
#include <string_view>

using namespace T4Xenon;

namespace
{
    constexpr auto COPY_BUFFER_SIZE = 64uz * 1024uz;

    std::uint32_t HashSoundPath(const std::string_view path)
    {
        auto hash = 5381u;
        for (const auto character : path)
        {
            const auto lower = character >= 'A' && character <= 'Z' ? character + ('a' - 'A') : character;
            hash = hash * 65599u + static_cast<unsigned char>(lower);
        }
        return hash ? hash : 1u;
    }

    bool CheckPackHeader(std::istream& stream, const PackIndexHeader& header, const std::string& packName)
    {
        const std::array expected{header.magic, header.timestamp, header.count, header.alignment, header.data_start};
        std::array<std::uint32_t, 5> actual{};
        stream.read(reinterpret_cast<char*>(actual.data()), sizeof(actual));
        if (stream.gcount() == sizeof(actual))
        {
            for (auto i = 0uz; i < actual.size(); ++i)
                actual[i] = endianness::FromLittleEndian(actual[i]);
            if (actual == expected)
                return true;
        }

        con::warn("Packfile \"{}\" does not match its PackIndex", packName);
        return false;
    }

    SearchPathOpenFile OpenStreamedSound(AssetDumpingContext& context, const std::string& soundPath, std::uint64_t& offset, std::uint64_t& length)
    {
        auto source = context.m_obj_search_path.Open(soundPath + ".xma");
        if (source.IsOpen())
        {
            offset = 0u;
            length = source.m_length > 0 ? static_cast<std::uint64_t>(source.m_length) : 0u;
            return source;
        }

        // T4 tries loose files first, then hashes the path without its .xma extension.
        const auto hash = HashSoundPath(soundPath);
        for (const auto* asset : context.m_zone.m_pools.PoolAssets<AssetPackIndex>())
        {
            if (asset->IsReference())
                continue;
            const auto& index = *asset->Asset();
            if (!index.name || !index.entries || !index.header.count || !index.header.alignment)
                continue;

            const auto* begin = index.entries;
            const auto* end = begin + index.header.count;
            const auto* entry = std::lower_bound(begin,
                                                 end,
                                                 hash,
                                                 [](const PackIndexEntry& entry, const std::uint32_t value)
                                                 {
                                                     return entry.hash < value;
                                                 });
            if (entry == end || entry->hash != hash)
                continue;

            source = context.m_obj_search_path.Open(index.name);
            if (!source.IsOpen())
                continue;
            if (!CheckPackHeader(*source.m_stream, index.header, index.name))
                return {};

            // Pack offsets are alignment units relative to data_start, not byte offsets.
            offset = static_cast<std::uint64_t>(entry->offset) * index.header.alignment + index.header.data_start;
            length = entry->size;
            return source;
        }
        return {};
    }

    void CopyStreamedSound(AssetDumpingContext& context, const StreamFileName& filename, const std::string& assetName, const size_t aliasIndex)
    {
        if (!filename.name || !*filename.name)
            return;

        auto path = std::filesystem::path(filename.dir ? filename.dir : "") / filename.name;
        auto soundPath = path.generic_string();
        std::ranges::replace(soundPath, '\\', '/');
        std::uint64_t offset = 0u;
        std::uint64_t length = 0u;
        const auto source = OpenStreamedSound(context, soundPath, offset, length);
        if (!source.IsOpen())
        {
            con::warn("Could not find streamed sound data for \"{}\" at \"{}\"", assetName, soundPath);
            return;
        }
        if (source.m_length < 0 || offset > static_cast<std::uint64_t>(source.m_length) || length == 0u
            || length > static_cast<std::uint64_t>(source.m_length) - offset)
        {
            con::warn("Streamed sound data for \"{}\" is outside its source file", assetName);
            return;
        }
        source.m_stream->seekg(static_cast<std::streamoff>(offset));
        if (!*source.m_stream)
        {
            con::warn("Failed to seek streamed sound data for \"{}\"", assetName);
            return;
        }
        const auto assetFile = context.OpenAssetFile(std::format("sound/{}.{}.xma", assetName, aliasIndex));
        if (!assetFile)
            return;

        auto bytesRemaining = length;
        std::array<char, COPY_BUFFER_SIZE> copyBuffer;
        while (bytesRemaining > 0)
        {
            const auto bytesToCopy = std::min(bytesRemaining, static_cast<std::uint64_t>(copyBuffer.size()));
            source.m_stream->read(copyBuffer.data(), static_cast<std::streamsize>(bytesToCopy));
            const auto bytesRead = source.m_stream->gcount();
            if (bytesRead != static_cast<std::streamsize>(bytesToCopy))
            {
                con::warn("Failed to read streamed sound data for \"{}\"", assetName);
                return;
            }

            assetFile->write(copyBuffer.data(), bytesRead);
            bytesRemaining -= bytesToCopy;
        }
    }
} // namespace

namespace sound
{
    void SoundDumperT4Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<snd_alias_list_t>& asset)
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
