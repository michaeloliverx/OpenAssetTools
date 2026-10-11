#include "LoadedSoundDumperIW3Xenon.h"

#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>

using namespace IW3Xenon;

namespace fs = std::filesystem;

namespace
{
    constexpr auto RIFF_CHUNK_HEADER_SIZE = 8uz;
    constexpr auto XMA_WAVE_FORMAT_HEADER_SIZE = 12uz;
    constexpr auto XMA_STREAM_FORMAT_SIZE = 20uz;
    // Names and values from the Xbox 360 SDK's xma2defs.h.
    constexpr auto XMA_OUTPUT_SAMPLE_BITS = std::uint16_t{16};
    constexpr auto WAVE_FORMAT_XMA = std::uint16_t{0x0165};
    constexpr auto XMA_FORMAT_VERSION = std::uint8_t{2};
    constexpr auto XAUDIO_SAMPLE_TYPE_XMA = std::uint8_t{3}; // Retail IW3's XAudio source format.
    // XAudioCreateSourceEffect selects CXMASourceEffect for SampleType 4 in the IW3 debug executable.
    constexpr auto XAUDIO_SAMPLE_TYPE_XMA_DEBUG = std::uint8_t{4};

    std::string GetFileNameForLoadedSound(const std::string& assetName)
    {
        auto fileName = fs::path(assetName);
        fileName.replace_extension(".xma");
        return std::format("sound/{}", fileName.generic_string());
    }

    template<std::integral T> void WriteLittleEndian(std::ostream& stream, const T value)
    {
        const auto littleEndian = endianness::ToLittleEndian(value);
        stream.write(reinterpret_cast<const char*>(&littleEndian), sizeof(littleEndian));
    }

    void WriteFourCc(std::ostream& stream, const std::string_view fourCc)
    {
        stream.write(fourCc.data(), static_cast<std::streamsize>(fourCc.size()));
    }

    void WriteChunkHeader(std::ostream& stream, const std::string_view id, const std::uint32_t size)
    {
        WriteFourCc(stream, id);
        WriteLittleEndian(stream, size);
    }

    void WriteChunkPadding(std::ostream& stream, const std::uint32_t size)
    {
        if ((size & 1u) != 0u)
            stream.put('\0');
    }

    [[nodiscard]] std::uint32_t GetPseudoBytesPerSecond(const XAUDIOPACKET_ALIGNED& packet, const int totalMsec, const std::uint16_t streamCount)
    {
        if (totalMsec <= 0 || streamCount == 0)
            return 0u;

        const auto numerator = static_cast<std::uint64_t>(packet.BufferSize) * 1000u;
        const auto denominator = static_cast<std::uint64_t>(totalMsec) * streamCount;
        return static_cast<std::uint32_t>(std::min(numerator / denominator, static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max())));
    }

    [[nodiscard]] bool CanWriteRiffXma(const LoadedSound& loadedSound, const std::string& assetName)
    {
        const auto& sound = loadedSound.sound;
        const auto streamCount = sound.format.___u1.NumStreams;

        if (sound.format.SampleType != XAUDIO_SAMPLE_TYPE_XMA && sound.format.SampleType != XAUDIO_SAMPLE_TYPE_XMA_DEBUG)
        {
            con::warn("Loaded sound \"{}\" has unsupported sample type {}", assetName, sound.format.SampleType);
            return false;
        }

        if (streamCount == 0u || streamCount > std::size(sound.format.___u2.Stream))
        {
            con::warn("Loaded sound \"{}\" has invalid XMA stream count {}", assetName, streamCount);
            return false;
        }

        for (auto streamIndex = 0u; streamIndex < streamCount; ++streamIndex)
        {
            const auto& source = sound.format.___u2.Stream[streamIndex];
            if (source.ChannelCount < 1u || source.ChannelCount > 2u || endianness::FromBigEndian(source.SampleRate) == 0u)
            {
                con::warn("Loaded sound \"{}\" has invalid XMA stream format", assetName);
                return false;
            }
        }

        if (sound.seekTable.size < 0 || (sound.seekTable.size > 0 && !sound.seekTable.data))
        {
            con::warn("Loaded sound \"{}\" has invalid XMA seek data", assetName);
            return false;
        }

        return true;
    }

    void WriteXmaFormat(std::ostream& stream, const XaSound& sound, const std::uint16_t streamCount)
    {
        // This is the legacy XMAWAVEFORMAT defined by the Xbox 360 SDK. IW3 stores the packet, loops,
        // source format, duration, and seek table separately in the fastfile, so reconstruct it here.
        WriteLittleEndian(stream, WAVE_FORMAT_XMA);
        WriteLittleEndian(stream, XMA_OUTPUT_SAMPLE_BITS);
        WriteLittleEndian(stream, std::uint16_t{0}); // EncodeOptions is not retained by XaSound.
        WriteLittleEndian(stream, std::uint16_t{0}); // LargestSkip is not retained by XaSound.
        WriteLittleEndian(stream, streamCount);
        stream.put(static_cast<char>(sound.packet.LoopCount));
        stream.put(static_cast<char>(XMA_FORMAT_VERSION));

        const auto pseudoBytesPerSecond = GetPseudoBytesPerSecond(sound.packet, sound.xaIwXmaDataInfo.totalMsec, streamCount);
        for (auto streamIndex = 0u; streamIndex < streamCount; ++streamIndex)
        {
            const auto& sourceStream = sound.format.___u2.Stream[streamIndex];
            const auto& loop = sound.packet.XMALoop[streamIndex];

            WriteLittleEndian(stream, pseudoBytesPerSecond);
            // The zone loader leaves the XAUDIOSOURCEFORMAT union in Xenon byte order.
            WriteLittleEndian(stream, endianness::FromBigEndian(sourceStream.SampleRate));
            WriteLittleEndian(stream, loop.LoopStart);
            WriteLittleEndian(stream, loop.LoopEnd);
            stream.put(static_cast<char>((loop.LoopSubframeEnd << 4u) | loop.LoopSubframeSkip));
            stream.put(static_cast<char>(sourceStream.ChannelCount));
            WriteLittleEndian(stream, std::uint16_t{0}); // Channel assignments are not retained; stock streamed XMA also uses zero.
        }
    }

    void WriteSeekData(std::ostream& stream, const XaSeekTable& seekTable)
    {
        for (auto seekIndex = 0; seekIndex < seekTable.size; ++seekIndex)
            WriteLittleEndian(stream, seekTable.data[seekIndex]);
    }
} // namespace

namespace sound
{
    void LoadedSoundDumperIW3Xenon::DumpAsset(AssetDumpingContext& context, const XAssetInfo<LoadedSound>& asset)
    {
        const auto* loadedSound = asset.Asset();
        const auto& packet = loadedSound->sound.packet;
        if (!packet.pBuffer || packet.BufferSize == 0)
        {
            con::warn("Loaded sound \"{}\" has no XMA data", asset.m_name);
            return;
        }

        if (!CanWriteRiffXma(*loadedSound, asset.m_name))
            return;

        const auto streamCount = static_cast<std::uint16_t>(loadedSound->sound.format.___u1.NumStreams);
        const auto formatSize = static_cast<std::uint32_t>(XMA_WAVE_FORMAT_HEADER_SIZE + XMA_STREAM_FORMAT_SIZE * streamCount);
        const auto seekSize = static_cast<std::uint64_t>(loadedSound->sound.seekTable.size) * sizeof(*loadedSound->sound.seekTable.data);
        const auto dataSize = static_cast<std::uint64_t>(packet.BufferSize);
        const auto riffSize = 4uz + RIFF_CHUNK_HEADER_SIZE + formatSize + RIFF_CHUNK_HEADER_SIZE + sizeof(std::uint32_t) + RIFF_CHUNK_HEADER_SIZE + seekSize
                              + (seekSize & 1u) + RIFF_CHUNK_HEADER_SIZE + dataSize + (dataSize & 1u);
        if (riffSize > std::numeric_limits<std::uint32_t>::max())
        {
            con::warn("Loaded sound \"{}\" is too large for an XMA RIFF file", asset.m_name);
            return;
        }

        const auto assetFile = context.OpenAssetFile(GetFileNameForLoadedSound(asset.m_name));
        if (!assetFile)
            return;

        WriteFourCc(*assetFile, "RIFF");
        WriteLittleEndian(*assetFile, static_cast<std::uint32_t>(riffSize));
        WriteFourCc(*assetFile, "WAVE");

        WriteChunkHeader(*assetFile, "fmt ", formatSize);
        WriteXmaFormat(*assetFile, loadedSound->sound, streamCount);

        WriteChunkHeader(*assetFile, "IWXI", sizeof(std::uint32_t));
        WriteLittleEndian(*assetFile, static_cast<std::uint32_t>(loadedSound->sound.xaIwXmaDataInfo.totalMsec));

        WriteChunkHeader(*assetFile, "seek", static_cast<std::uint32_t>(seekSize));
        WriteSeekData(*assetFile, loadedSound->sound.seekTable);
        WriteChunkPadding(*assetFile, static_cast<std::uint32_t>(seekSize));

        WriteChunkHeader(*assetFile, "data", packet.BufferSize);
        assetFile->write(packet.pBuffer, packet.BufferSize);
        WriteChunkPadding(*assetFile, packet.BufferSize);
    }
} // namespace sound
