#include "ContentLoaderBase.h"

#include "Utils/Endianness.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

ContentLoaderBase::ContentLoaderBase(Zone& zone, ZoneInputStream& stream)
    : varXString(nullptr),
      m_zone(zone),
      m_memory(zone.Memory()),
      m_stream(stream),

      // -1
      m_zone_ptr_following(
          reinterpret_cast<const void*>(std::numeric_limits<std::uintptr_t>::max() >> ((sizeof(std::uintptr_t) * 8u) - stream.GetPointerBitCount()))),

      // -2
      m_zone_ptr_insert(
          reinterpret_cast<const void*>((std::numeric_limits<std::uintptr_t>::max() >> ((sizeof(std::uintptr_t) * 8u) - stream.GetPointerBitCount())) - 1u))
{
}

void ContentLoaderBase::LoadXString(const bool atStreamStart) const
{
    assert(varXString != nullptr);

    if (atStreamStart)
        m_stream.Load<const char*>(varXString);

    if (*varXString != nullptr)
    {
        if (GetZonePointerType(*varXString) == ZonePointerType::FOLLOWING)
        {
            *varXString = m_stream.Alloc<const char>(1);
            m_stream.LoadNullTerminated(const_cast<char*>(*varXString));
        }
        else
        {
            *varXString = m_stream.ConvertOffsetToPointerNative<const char>(*varXString);
        }
    }
}

void ContentLoaderBase::LoadXStringArray(const bool atStreamStart, const size_t count)
{
    assert(varXString != nullptr);

    const auto swapBigEndianPtr32 = [](auto& pointer)
    {
        auto value = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(pointer));
        value = endianness::FromBigEndian(value);
        pointer = reinterpret_cast<std::remove_reference_t<decltype(pointer)>>(static_cast<std::uintptr_t>(value));
    };
    const auto isBigEndian = m_zone.m_game_id == GameId::IW3Xenon;

#ifdef ARCH_x86
    if (atStreamStart)
    {
        m_stream.Load<const char*>(varXString, count);
        if (isBigEndian)
        {
            for (size_t index = 0; index < count; index++)
                swapBigEndianPtr32(varXString[index]);
        }
    }
#else
    if (atStreamStart)
    {
        const auto fill = m_stream.LoadWithFill(4u * count);

        for (size_t index = 0; index < count; index++)
        {
            fill.FillPtr(varXString[index], 4u * index);
            if (isBigEndian)
                swapBigEndianPtr32(varXString[index]);
            m_stream.AddPointerLookup(&varXString[index], fill.BlockBuffer(4u * index));
        }
    }
#endif

    for (size_t index = 0; index < count; index++)
    {
        LoadXString(false);
        varXString++;
    }
}

ZonePointerType ContentLoaderBase::GetZonePointerType(const void* zonePtr) const
{
    if (zonePtr == m_zone_ptr_following)
        return ZonePointerType::FOLLOWING;
    if (zonePtr == m_zone_ptr_insert)
        return ZonePointerType::INSERT;

    return ZonePointerType::OFFSET;
}
