#include "StepSkipZoneImageHeaders.h"

#include "Utils/Endianness.h"

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

namespace
{
    class StepSkipZoneImageHeaders final : public ILoadingStep
    {
    public:
        explicit StepSkipZoneImageHeaders(step::ZoneImageHeaderCallback callback)
            : m_callback(std::move(callback))
        {
        }

        void PerformStep(ZoneLoader& zoneLoader, ILoadingStream& stream) override
        {
            (void)zoneLoader;

            // Xbox fastfiles have additional header data before the auth header:
            // - 4 bytes: language flags bitmask
            // - 4 bytes: image count
            // - sizeof(XAssetStreamFile) * image count (for each language in the bitmask)
            // - 4 bytes: xfile size
            // - 4 bytes: xfile load size
            // struct XAssetStreamFile // sizeof=0xC
            // {
            //     unsigned int fileIndex;
            //     unsigned int offset;
            //     unsigned int offsetEnd;
            // };

            uint32_t languageFlags;
            stream.Load(&languageFlags, sizeof(languageFlags));
            languageFlags = endianness::FromBigEndian(languageFlags);

            uint32_t imageCount;
            stream.Load(&imageCount, sizeof(imageCount));
            imageCount = endianness::FromBigEndian(imageCount);

            // Count how many languages are set in the bitmask
            uint32_t languageCount = 0;
            for (int i = 0; i < 15; i++)
            {
                if (languageFlags & (1 << i))
                {
                    languageCount++;
                }
            }

            std::vector<XAssetStreamFile> selectedImageStreamFiles;
            selectedImageStreamFiles.reserve(imageCount);

            const auto tableLanguageCount = languageCount;
            auto selectedLanguageRead = false;
            for (auto languageIndex = 0u; languageIndex < tableLanguageCount; languageIndex++)
            {
                if (!selectedLanguageRead)
                {
                    for (auto imageIndex = 0u; imageIndex < imageCount; imageIndex++)
                    {
                        XAssetStreamFile streamFile{};
                        stream.Load(&streamFile, sizeof(streamFile));
                        streamFile.fileIndex = endianness::FromBigEndian(streamFile.fileIndex);
                        streamFile.offset = endianness::FromBigEndian(streamFile.offset);
                        streamFile.offsetEnd = endianness::FromBigEndian(streamFile.offsetEnd);
                        selectedImageStreamFiles.emplace_back(streamFile);
                    }

                    selectedLanguageRead = true;
                }
                else
                {
                    // Skip image stream file data (12 bytes per image per language)
                    const size_t imageDataSize = sizeof(XAssetStreamFile) * imageCount;
                    if (imageDataSize > 0)
                    {
                        std::vector<uint8_t> tempBuffer(std::min(imageDataSize, size_t(8192)));
                        size_t skipped = 0;
                        while (skipped < imageDataSize)
                        {
                            const size_t toSkip = std::min(imageDataSize - skipped, tempBuffer.size());
                            stream.Load(tempBuffer.data(), toSkip);
                            skipped += toSkip;
                        }
                    }
                }
            }

            uint32_t xfileSize;
            stream.Load(&xfileSize, sizeof(xfileSize));
            xfileSize = endianness::FromBigEndian(xfileSize);

            uint32_t xfileLoadSize;
            stream.Load(&xfileLoadSize, sizeof(xfileLoadSize));
            xfileLoadSize = endianness::FromBigEndian(xfileLoadSize);

            if (m_callback)
                m_callback(languageFlags, languageCount == 0 ? 1 : languageCount, imageCount, selectedImageStreamFiles, xfileSize, xfileLoadSize);
        }

    private:
        step::ZoneImageHeaderCallback m_callback;
    };
} // namespace

namespace step
{
    std::unique_ptr<ILoadingStep> CreateStepSkipZoneImageHeaders(ZoneImageHeaderCallback callback)
    {
        return std::make_unique<StepSkipZoneImageHeaders>(std::move(callback));
    }
} // namespace step
