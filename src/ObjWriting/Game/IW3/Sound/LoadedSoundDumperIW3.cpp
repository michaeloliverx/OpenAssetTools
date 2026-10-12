#include "LoadedSoundDumperIW3.h"

#include "Sound/ImaAdpcmDecoder.h"
#include "Sound/WavTypes.h"
#include "Sound/WavWriter.h"
#include "Utils/Logging/Log.h"

#include <format>

using namespace IW3;

namespace sound
{
    void LoadedSoundDumperIW3::DumpAsset(AssetDumpingContext& context, const XAssetInfo<LoadedSound>& asset)
    {
        const auto* loadedSound = asset.Asset();
        const auto& info = loadedSound->sound.info;
        WavMetaData metaData{
            .channelCount = static_cast<unsigned>(info.channels), .samplesPerSec = info.rate, .bitsPerSample = static_cast<unsigned>(info.bits)};

        const void* data = loadedSound->sound.data;
        auto dataSize = static_cast<size_t>(info.data_len);
        std::vector<int16_t> samples;
        switch (static_cast<WavFormat>(info.format))
        {
        case WavFormat::PCM:
            break;

        case WavFormat::IMA_ADPCM:
            if (info.bits != 4 || (dataSize > 0 && !data)
                || !ima_adpcm::Decode(std::span(static_cast<const uint8_t*>(data), dataSize), metaData.channelCount, info.block_size, info.samples, samples))
            {
                con::error("Invalid IMA ADPCM data for loaded sound: {}", loadedSound->name);
                return;
            }
            metaData.bitsPerSample = 16;
            data = samples.data();
            dataSize = samples.size() * sizeof(int16_t);
            break;

        default:
            con::error("Unknown format {} for loaded sound: {}", info.format, loadedSound->name);
            return;
        }

        const auto assetFile = context.OpenAssetFile(std::format("sound/{}", asset.m_name));
        if (!assetFile)
            return;

        const WavWriter writer(*assetFile);
        writer.WritePcmHeader(metaData, dataSize);
        writer.WritePcmData(data, dataSize);
    }
} // namespace sound
