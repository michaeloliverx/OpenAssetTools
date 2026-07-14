#include "Game/IW3/Techset/TechsetConstantsIW3.h"
#include "Techset/StateMap/StateMapHandler.h"
#include "Techset/StateMap/StateMapReader.h"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace
{
    std::unique_ptr<state_map::StateMapDefinition> LoadStateMap(const fs::path& filePath)
    {
        std::ifstream stream(filePath);
        REQUIRE(stream.is_open());

        const auto fileName = filePath.string();
        const state_map::StateMapReader reader(stream, fileName, filePath.stem().string(), IW3::stateMapLayout);
        return reader.ReadStateMapDefinition();
    }
} // namespace

TEST_CASE("Bundled IW3 state maps parse with the IW3 layout", "[iw3][techset][statemap]")
{
    const auto stateMapDirectory = fs::current_path() / "raw" / "iw3" / "statemaps";
    REQUIRE(fs::is_directory(stateMapDirectory));

    auto stateMapCount = 0u;
    for (const auto& entry : fs::directory_iterator(stateMapDirectory))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".sm")
            continue;

        INFO(entry.path().string());
        CHECK(LoadStateMap(entry.path()) != nullptr);
        stateMapCount++;
    }

    CHECK(stateMapCount == 21u);
}

TEST_CASE("IW3 passthrough state map preserves reference state bits", "[iw3][techset][statemap]")
{
    const auto stateMapPath = fs::current_path() / "raw" / "iw3" / "statemaps" / "passthrough.sm";
    const auto stateMap = LoadStateMap(stateMapPath);
    REQUIRE(stateMap != nullptr);

    constexpr uint32_t input[2]{0x08128965u, 0x0000000Cu};
    uint32_t output[2]{};

    const state_map::StateMapHandler handler(IW3::stateMapLayout, *stateMap);
    handler.ApplyStateMap(input, output);

    CHECK(output[0] == input[0]);
    CHECK(output[1] == input[1]);
}

TEST_CASE("IW3 state map entries may exhaustively omit a default rule", "[iw3][techset][statemap]")
{
    const auto stateMapPath = fs::current_path() / "raw" / "iw3" / "statemaps" / "build_shadowmap.sm";
    const auto stateMap = LoadStateMap(stateMapPath);
    REQUIRE(stateMap != nullptr);

    constexpr uint32_t input[2]{0x08128812u, 0x0000000Du};
    uint32_t output[2]{};

    const state_map::StateMapHandler handler(IW3::stateMapLayout, *stateMap);
    handler.ApplyStateMap(input, output);

    CHECK((input[0] & IW3::GFXS0_CULL_MASK) == IW3::GFXS0_CULL_BACK);
    CHECK((output[0] & IW3::GFXS0_CULL_MASK) == IW3::GFXS0_CULL_FRONT);

    constexpr uint32_t frontFaceInput[2]{0x0812C812u, 0x0000000Du};
    handler.ApplyStateMap(frontFaceInput, output);

    CHECK((frontFaceInput[0] & IW3::GFXS0_CULL_MASK) == IW3::GFXS0_CULL_FRONT);
    CHECK((output[0] & IW3::GFXS0_CULL_MASK) == IW3::GFXS0_CULL_BACK);
}
