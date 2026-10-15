#include "StepWriteZoneImageHeaders.h"

#include "Utils/Endianness.h"

#include <utility>

StepWriteZoneImageHeaders::StepWriteZoneImageHeaders(const GameEndianness endianness,
                                                     ImageStreamFileProvider provider,
                                                     std::shared_ptr<XFileSizePatch> sizePatch)
    : m_endianness(endianness),
      m_provider(std::move(provider)),
      m_size_patch(std::move(sizePatch))
{
}

void StepWriteZoneImageHeaders::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    auto files = m_provider();
    uint32_t languageFlags = 1u;
    auto imageCount = static_cast<uint32_t>(files.size());
    if (m_endianness == GameEndianness::BE)
    {
        languageFlags = endianness::ToBigEndian(languageFlags);
        imageCount = endianness::ToBigEndian(imageCount);
        for (auto& file : files)
        {
            file.fileIndex = endianness::ToBigEndian(file.fileIndex);
            file.offset = endianness::ToBigEndian(file.offset);
            file.offsetEnd = endianness::ToBigEndian(file.offsetEnd);
        }
    }
    stream->Write(&languageFlags, sizeof(languageFlags));
    stream->Write(&imageCount, sizeof(imageCount));
    if (!files.empty())
        stream->Write(files.data(), files.size() * sizeof(XAssetStreamFile));

    m_size_patch->m_offset = stream->Pos();
    constexpr uint32_t sizes[2]{};
    stream->Write(sizes, sizeof(sizes));
}
