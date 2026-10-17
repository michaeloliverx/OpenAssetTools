#include "Utils/Endianness.h"
#include "Zone/Stream/ZoneOutputStream.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE("Temp asset aliases can reuse the original pointer slot", "[zonewriting][aliases][system]")
{
    const auto originalPointerSlots = GENERATE(false, true);
    XBlock temp("temp", 0, XBlockType::BLOCK_TYPE_TEMP);
    XBlock virt("virtual", 1, XBlockType::BLOCK_TYPE_NORMAL);
    std::vector<XBlock*> blocks{&temp, &virt};
    InMemoryZoneData data, delay;
    auto stream = ZoneOutputStream::Create(32, 2, blocks, 1, data, delay, GameEndianness::BE);
    if (originalPointerSlots)
        stream->UseOriginalPointerSlots();

    stream->PushBlock(1);
    uint32_t slots[2]{};
    const auto written = stream->Write(slots, 2);
    uint32_t value = 0;
    stream->PushBlock(0);
    REQUIRE(stream->ReusableShouldWrite(&value, written));
    stream->ReusableAddOffset(&value);
    stream->Write(&value);
    stream->MarkFollowing(written);
    stream->PopBlock();
    stream->PushBlock(0);
    REQUIRE_FALSE(stream->ReusableShouldWrite(&value, written.AtOffset(4)));
    stream->PopBlock();
    stream->PopBlock();

    const auto* result = static_cast<const uint32_t*>(written.Offset());
    REQUIRE(endianness::FromBigEndian(result[0]) == (originalPointerSlots ? 0xffffffffu : 0xfffffffeu));
    REQUIRE(endianness::FromBigEndian(result[1]) == (originalPointerSlots ? 0x40000001u : 0x40000009u));
    REQUIRE(virt.m_buffer_size == (originalPointerSlots ? 8 : 12));
    REQUIRE(temp.m_buffer_size == 4);
}

TEST_CASE("Non-temp reusable aliases retain offsets through fill accessors", "[zonewriting][aliases][system]")
{
    XBlock virt("virtual", 0, XBlockType::BLOCK_TYPE_NORMAL);
    std::vector<XBlock*> blocks{&virt};
    InMemoryZoneData data, delay;
    auto stream = ZoneOutputStream::Create(32, 2, blocks, 0, data, delay, GameEndianness::BE);
    stream->PushBlock(0);
    const auto fill = stream->WriteWithFill(12);
    const auto first = fill.AtOffset(4).Offset();
    const auto second = fill.Offset().AtOffset(8);
    uint32_t value = 0;
    REQUIRE(stream->ReusableShouldWrite(&value, first));
    stream->ReusableAddOffset(&value, first);
    stream->Write(&value);
    stream->MarkFollowing(first);
    REQUIRE_FALSE(stream->ReusableShouldWrite(&value, second));
    stream->PopBlock();
    REQUIRE(endianness::FromBigEndian(*static_cast<uint32_t*>(first.Offset())) == 0xffffffffu);
    REQUIRE(endianness::FromBigEndian(*static_cast<uint32_t*>(second.Offset())) == 5);
}
