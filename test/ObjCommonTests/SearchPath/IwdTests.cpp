#include "OatTestPaths.h"
#include "SearchPath/IWD.h"
#include "Utils/FileToZlibWrapper.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>
#include <zip.h>

namespace fs = std::filesystem;

namespace
{
    void WriteIwd(std::ostream& stream, const std::string& entryName, const std::string_view contents)
    {
        auto zlibFunctions = FileToZlibWrapper::CreateFunctions32ForFile(&stream);
        const auto zipHandle = zipOpen2("foo.zip", 0, nullptr, &zlibFunctions);
        zipOpenNewFileInZip2(zipHandle, entryName.c_str(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0);
        zipWriteInFileInZip(zipHandle, contents.data(), static_cast<unsigned>(contents.size()));
        zipCloseFileInZip(zipHandle);
        zipClose(zipHandle, nullptr);
    }

    TEST_CASE("Iwd: Reads a peeked entry through end of stream", "[searchpath][iwd]")
    {
        constexpr auto ENTRY_NAME = "test.txt";
        constexpr auto CONTENTS = "IWD stream test";

        const auto tempDirectory = oat::paths::GetTempDirectory("iwd");
        fs::create_directories(tempDirectory);
        const auto iwdPath = tempDirectory / "test.iwd";

        {
            std::ofstream stream(iwdPath, std::ios::out | std::ios::binary);
            WriteIwd(stream, ENTRY_NAME, CONTENTS);
            stream.close();
        }

        const auto searchPath = iwd::LoadFromFile(iwdPath.string());
        REQUIRE(searchPath);

        const auto file = searchPath->Open(ENTRY_NAME);
        REQUIRE(file.IsOpen());

        auto& stream = *file.m_stream;
        REQUIRE(stream.peek() == CONTENTS[0]);

        std::array<char, std::string_view{CONTENTS}.size()> buffer{};
        stream.read(buffer.data(), buffer.size());
        REQUIRE(stream.gcount() == static_cast<std::streamsize>(buffer.size()));
        REQUIRE(std::string_view{buffer.data(), buffer.size()} == CONTENTS);
        REQUIRE(stream.get() == std::char_traits<char>::eof());
    }

    TEST_CASE("Iwd: Opens entries regardless of path case or separators", "[searchpath][iwd]")
    {
        const auto* entryName =
            GENERATE("sound/sfx/levels/test.wav", "sound/SFX/Levels/test.wav", "sound\\sfx\\levels\\test.wav", "sound\\SFX\\Levels\\test.wav");
        const auto* requestedName =
            GENERATE("sound/sfx/levels/test.wav", "SOUND/SFX/LEVELS/TEST.WAV", "sound\\sfx\\levels\\test.wav", "SOUND\\SFX\\LEVELS\\TEST.WAV");
        constexpr auto CONTENTS = "IWD path test";
        CAPTURE(entryName, requestedName);

        const auto tempDirectory = oat::paths::GetTempDirectory("iwd-paths");
        fs::create_directories(tempDirectory);
        const auto iwdPath = tempDirectory / "test.iwd";
        {
            std::ofstream stream(iwdPath, std::ios::out | std::ios::binary);
            WriteIwd(stream, entryName, CONTENTS);
        }

        const auto searchPath = iwd::LoadFromFile(iwdPath.string());
        REQUIRE(searchPath);
        const auto file = searchPath->Open(requestedName);
        REQUIRE(file.IsOpen());
        const std::string contents{std::istreambuf_iterator<char>(*file.m_stream), std::istreambuf_iterator<char>()};
        REQUIRE(contents == CONTENTS);
    }

    TEST_CASE("Iwd: Find preserves entry case and returns an openable path", "[searchpath][iwd]")
    {
        const auto* entryName = GENERATE("sound/SFX/Levels/test.wav", "sound\\SFX\\Levels\\test.wav");
        constexpr auto CONTENTS = "IWD find test";
        const auto tempDirectory = oat::paths::GetTempDirectory("iwd-find");
        fs::create_directories(tempDirectory);
        const auto iwdPath = tempDirectory / "test.iwd";
        {
            std::ofstream stream(iwdPath, std::ios::out | std::ios::binary);
            WriteIwd(stream, entryName, CONTENTS);
        }

        const auto searchPath = iwd::LoadFromFile(iwdPath.string());
        REQUIRE(searchPath);
        std::vector<std::string> found;
        searchPath->Find(SearchPathSearchOptions().IncludeSubdirectories(true),
                         [&found](const std::string& name)
                         {
                             found.emplace_back(name);
                         });
        REQUIRE(found == std::vector<std::string>{"sound/SFX/Levels/test.wav"});
        const auto file = searchPath->Open(found.front());
        REQUIRE(file.IsOpen());
        const std::string contents{std::istreambuf_iterator<char>(*file.m_stream), std::istreambuf_iterator<char>()};
        REQUIRE(contents == CONTENTS);
    }
} // namespace
