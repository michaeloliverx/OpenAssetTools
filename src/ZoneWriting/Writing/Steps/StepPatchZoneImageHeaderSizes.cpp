#include "StepPatchZoneImageHeaderSizes.h"

#include "Utils/Endianness.h"
#include "Utils/Logging/Log.h"
#include "Writing/WritingException.h"

#include <cstdint>
#include <limits>
#include <utility>

StepPatchZoneImageHeaderSizes::StepPatchZoneImageHeaderSizes(std::shared_ptr<XFileSizePatch> patch, const GameEndianness endianness)
    : m_patch(std::move(patch)),
      m_endianness(endianness)
{
}

void StepPatchZoneImageHeaderSizes::PerformStep(ZoneWriter* zoneWriter, IWritingStream* stream)
{
    (void)zoneWriter;

    if (!m_patch || m_patch->m_offset < 0)
        return;

    stream->Flush();
    const auto fileSize = stream->Pos();
    if (fileSize < 0 || static_cast<std::uint64_t>(fileSize) > std::numeric_limits<std::uint32_t>::max())
        throw WritingException("XFile image header size exceeds 32-bit range");

    if (!stream->SetPos(m_patch->m_offset))
        throw WritingException("Failed to seek to XFile image header size");

    auto xfileSize = static_cast<std::uint32_t>(fileSize);
    auto xfileLoadSize = xfileSize;
    con::debug("Patching generated XFile image header: xfileSize={} xfileLoadSize={}", xfileSize, xfileLoadSize);
    if (m_endianness == GameEndianness::BE)
    {
        xfileSize = endianness::ToBigEndian(xfileSize);
        xfileLoadSize = endianness::ToBigEndian(xfileLoadSize);
    }

    stream->Write(&xfileSize, sizeof(xfileSize));
    stream->Write(&xfileLoadSize, sizeof(xfileLoadSize));
    stream->Flush();

    if (!stream->SetPos(fileSize))
        throw WritingException("Failed to restore XFile output position");
}
