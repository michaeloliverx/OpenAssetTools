#include "Loading/ContentLoaderBase.h"
#include "Loading/Exception/InvalidXStringIndexException.h"
#include "Loading/LoadingFileStream.h"
#include "Utils/Endianness.h"
#include "Writing/ContentWriterBase.h"
#include "Writing/WritingException.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstring>
#include <sstream>

#ifdef ARCH_x86

namespace
{
    class StringWriter final : public ContentWriterBase
    {
    public:
        StringWriter(const Zone& zone, ZoneOutputStream& stream)
            : ContentWriterBase(zone, stream)
        {
        }

        void Write(const char* value)
        {
            varXString = &value;
            WriteXString(true);
        }
    };

    class StringReader final : public ContentLoaderBase
    {
    public:
        StringReader(Zone& zone, ZoneInputStream& stream)
            : ContentLoaderBase(zone, stream)
        {
        }

        const char* Read()
        {
            const auto value = m_stream.Alloc<const char*>(1);
            varXString = value;
            LoadXString(true);
            return *value;
        }

        void ReadArray(const char** values, const size_t count)
        {
            const auto loaded = m_stream.Alloc<const char*>(1);
            varXString = loaded;
            LoadXStringArray(true, count);
            for (size_t i = 0; i < count; ++i)
                values[i] = loaded[i];
        }
    };

    std::string GetPayload(const InMemoryZoneData& data)
    {
        std::string payload;
        for (const auto& buffer : data.m_buffers)
            payload.append(buffer.m_data.get(), buffer.m_size);
        return payload;
    }
} // namespace

TEST_CASE("Indexed XStrings round trip null, empty and repeated strings", "[zone][indexedstrings][system]")
{
    const auto bigEndian = GENERATE(false, true);
    const auto array = GENERATE(false, true);
    Zone zone("strings", 0, bigEndian ? GameId::IW3Xenon : GameId::IW3, bigEndian ? GamePlatform::XBOX : GamePlatform::PC);
    XBlock block("virtual", 0, XBlockType::BLOCK_TYPE_NORMAL);
    std::vector<XBlock*> blocks{&block};
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks, 0, data, delay, bigEndian ? GameEndianness::BE : GameEndianness::LE);
    const char* strings[]{"first", "", "last"};
    const std::string repeated = "first";
    const char* values[]{nullptr, "last", "first", "", repeated.c_str()};
    output->SetIndexedXStrings(strings);
    output->PushBlock(0);
    StringWriter writer(zone, *output);
    for (const auto* value : values)
        writer.Write(value);
    output->PopBlock();

    const auto payload = GetPayload(data);
    REQUIRE(payload.size() == sizeof(uint32_t) * std::size(values));
    const uint32_t expected[]{0, 3, 1, 2, 1};
    for (size_t i = 0; i < std::size(expected); ++i)
    {
        uint32_t index;
        std::memcpy(&index, payload.data() + sizeof(index) * i, sizeof(index));
        REQUIRE((bigEndian ? endianness::FromBigEndian(index) : index) == expected[i]);
    }

    block.Alloc(payload.size());
    std::istringstream bytes(payload, std::ios::binary);
    LoadingFileStream source(bytes);
    auto input = ZoneInputStream::Create(32, 2, blocks, 0, source, zone.Memory(), std::nullopt);
    input->SetIndexedXStrings(strings);
    input->PushBlock(0);
    StringReader reader(zone, *input);
    const char* actual[std::size(values)]{};
    if (array)
        reader.ReadArray(actual, std::size(actual));
    else
        for (auto& value : actual)
            value = reader.Read();
    input->PopBlock();
    for (size_t i = 0; i < std::size(actual); ++i)
        REQUIRE(actual[i] == (expected[i] ? strings[expected[i] - 1] : nullptr));
    REQUIRE(source.Pos() == payload.size());
}

TEST_CASE("Indexed XStrings reject missing strings and invalid indices", "[zone][indexedstrings][system]")
{
    Zone zone("strings", 0, GameId::IW3, GamePlatform::PC);
    XBlock block("virtual", 0, XBlockType::BLOCK_TYPE_NORMAL);
    std::vector<XBlock*> blocks{&block};
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks, 0, data, delay, GameEndianness::LE);
    const char* strings[]{"known"};
    output->SetIndexedXStrings(strings);
    output->PushBlock(0);
    REQUIRE_THROWS_AS(StringWriter(zone, *output).Write("missing"), WritingException);
    output->PopBlock();

    const auto index = GENERATE(2u, UINT32_MAX);
    const auto empty = GENERATE(false, true);
    block.Alloc(sizeof(index));
    std::istringstream bytes(std::string(reinterpret_cast<const char*>(&index), sizeof(index)), std::ios::binary);
    LoadingFileStream source(bytes);
    auto input = ZoneInputStream::Create(32, 2, blocks, 0, source, zone.Memory(), std::nullopt);
    input->SetIndexedXStrings(empty ? std::span<const char* const>() : std::span<const char* const>(strings));
    input->PushBlock(0);
    REQUIRE_THROWS_AS(StringReader(zone, *input).Read(), InvalidXStringIndexException);
    input->PopBlock();
}

TEST_CASE("Unconfigured XStrings retain inline string storage and reuse", "[zone][indexedstrings][system]")
{
    Zone zone("strings", 0, GameId::IW3, GamePlatform::PC);
    XBlock block("virtual", 0, XBlockType::BLOCK_TYPE_NORMAL);
    std::vector<XBlock*> blocks{&block};
    InMemoryZoneData data, delay;
    auto output = ZoneOutputStream::Create(32, 2, blocks, 0, data, delay, GameEndianness::LE);
    output->PushBlock(0);
    StringWriter writer(zone, *output);
    const char* text = "inline";
    writer.Write(text);
    writer.Write(text);
    output->PopBlock();
    const auto payload = GetPayload(data);
    REQUIRE(payload.size() == sizeof(uint32_t) * 2 + std::strlen(text) + 1);

    block.Alloc(payload.size());
    std::istringstream bytes(payload, std::ios::binary);
    LoadingFileStream source(bytes);
    auto input = ZoneInputStream::Create(32, 2, blocks, 0, source, zone.Memory(), std::nullopt);
    input->PushBlock(0);
    StringReader reader(zone, *input);
    const auto* first = reader.Read();
    REQUIRE(std::string(first) == text);
    REQUIRE(reader.Read() == first);
    input->PopBlock();
    REQUIRE(source.Pos() == payload.size());
}

#endif
