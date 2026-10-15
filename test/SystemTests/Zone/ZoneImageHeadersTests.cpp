#include "Loading/LoadingFileStream.h"
#include "Loading/Steps/StepSkipZoneImageHeaders.h"
#include "Loading/ZoneLoader.h"
#include "Writing/Steps/StepPatchZoneImageHeaderSizes.h"
#include "Writing/Steps/StepWriteZoneImageHeaders.h"
#include "Writing/WritingFileStream.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Zone image stream headers preserve records and patch file sizes", "[zoneimagestream][system]")
{
    std::stringstream data(std::ios::in | std::ios::out | std::ios::binary);
    WritingFileStream output(data);
    const auto patch = std::make_shared<XFileSizePatch>();
    StepWriteZoneImageHeaders writeHeader(
        GameEndianness::BE,
        []
        {
            return std::vector<XAssetStreamFile>{
                {3, 0x1000, 0x2000},
                {4, 0x3000, 0x4000}
            };
        },
        patch);
    writeHeader.PerformStep(nullptr, &output);
    const uint32_t payload = 0x12345678;
    output.Write(&payload, sizeof(payload));
    StepPatchZoneImageHeaderSizes patchSizes(patch, GameEndianness::BE);
    patchSizes.PerformStep(nullptr, &output);
    REQUIRE(output.Pos() == 44);

    data.seekg(0);
    LoadingFileStream input(data);
    bool called = false;
    auto readHeader = step::CreateStepSkipZoneImageHeaders(
        [&](const uint32_t flags,
            const uint32_t languages,
            const uint32_t count,
            const std::vector<XAssetStreamFile>& files,
            const uint32_t size,
            const uint32_t loadSize)
        {
            called = true;
            REQUIRE(flags == 1);
            REQUIRE(languages == 1);
            REQUIRE(count == 2);
            REQUIRE(files.size() == 2);
            REQUIRE(files[0].fileIndex == 3);
            REQUIRE(files[0].offset == 0x1000);
            REQUIRE(files[0].offsetEnd == 0x2000);
            REQUIRE(files[1].fileIndex == 4);
            REQUIRE(size == 44);
            REQUIRE(loadSize == 44);
        });
    ZoneLoader loader(std::make_unique<Zone>("header", 0, GameId::IW4, GamePlatform::PC));
    readHeader->PerformStep(loader, input);
    REQUIRE(called);
    uint32_t actualPayload = 0;
    REQUIRE(input.Load(&actualPayload, sizeof(actualPayload)) == sizeof(actualPayload));
    REQUIRE(actualPayload == payload);
}
