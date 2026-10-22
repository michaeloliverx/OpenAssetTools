#include "Game/IW4/CommonIW4.h"
#include "Game/IW4/IW4.h"
#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <string>
#include <zlib.h>

namespace
{
    struct RestoreObjWritingConfiguration
    {
        ObjWriting::Configuration_t original{ObjWriting::Configuration};

        ~RestoreObjWritingConfiguration()
        {
            ObjWriting::Configuration = original;
        }
    };

    TEST_CASE("IW4 Xenon 482 text assets use PC disk formats through registered loaders and dumpers", "[iw4xenon482][textassets][system]")
    {
        RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(IW4Xenon482::ASSET_TYPE_COUNT, true);

        constexpr auto ZONE_NAME = "textassets";
        constexpr auto RAW_NAME = "scripts/test.bin";
        constexpr auto MAP_NAME = "maps/mp/test_map.d3dbsp";
        constexpr auto TABLE_NAME = "mp/test.csv";
        constexpr auto LOCALIZE_NAME = "TEXT_MESSAGE";
        const auto rawBytes = std::string("a\0b\xff", 4) + std::string(0x3000, 'x');
        constexpr auto EMPTY_RAW_NAME = "scripts/empty.gsc";
        constexpr auto CURVE_NAME = "test_curve";
        char entities[] = "{\n\"classname\" \"worldspawn\"\n}\n";
        const char* cells[]{"TEST", "comma, value", "quote \"value\"", ""};
        constexpr auto LOCALIZE_VALUE = "Line one\nLine \"two\"";

        MockSearchPath emptySearchPath;
        MockOutputPath pcOutput;
        MockOutputPath xenonOutput;
        {
            Zone pc(ZONE_NAME, 0, GameId::IW4, GamePlatform::PC);
            Zone xenon(ZONE_NAME, 0, GameId::IW4Xenon482, GamePlatform::XBOX);
            pc.m_language = xenon.m_language = GameLanguage::LANGUAGE_ENGLISH;

            IW4::RawFile pcRaw{RAW_NAME, 0, static_cast<int>(rawBytes.size()), {.buffer = rawBytes.c_str()}};
            IW4::RawFile pcEmptyRaw{EMPTY_RAW_NAME, 0, 0, {.buffer = ""}};
            IW4Xenon482::RawFile xenonRaw{RAW_NAME, 0, static_cast<int>(rawBytes.size()), {.buffer = rawBytes.c_str()}};
            IW4Xenon482::RawFile xenonEmptyRaw{EMPTY_RAW_NAME, 0, 0, {.buffer = ""}};
            IW4::MapEnts pcMap{MAP_NAME, entities, sizeof(entities)};
            IW4Xenon482::MapEnts xenonMap{MAP_NAME, entities, sizeof(entities)};
            IW4::StringTableCell pcCells[4]{};
            IW4Xenon482::StringTableCell xenonCells[4]{};
            for (auto index = 0u; index < 4; ++index)
            {
                pcCells[index] = {cells[index], IW4::Common::StringTable_HashString(cells[index])};
                xenonCells[index] = {pcCells[index].string, pcCells[index].hash};
            }
            IW4::StringTable pcTable{TABLE_NAME, 2, 2, pcCells};
            IW4Xenon482::StringTable xenonTable{TABLE_NAME, 2, 2, xenonCells};
            IW4::SndCurve pcCurve{
                CURVE_NAME, 2, {{0.0f, 1.0f}, {1.0f, 0.25f}}
            };
            IW4Xenon482::SndCurve xenonCurve{
                CURVE_NAME, 2, {{0.0f, 1.0f}, {1.0f, 0.25f}}
            };
            IW4::LocalizeEntry pcLocalize{LOCALIZE_VALUE, LOCALIZE_NAME};
            IW4Xenon482::LocalizeEntry xenonLocalize{LOCALIZE_VALUE, LOCALIZE_NAME};

            pc.m_pools.AddAsset(IW4::ASSET_TYPE_RAWFILE, RAW_NAME, &pcRaw, {}, {}, {});
            pc.m_pools.AddAsset(IW4::ASSET_TYPE_RAWFILE, EMPTY_RAW_NAME, &pcEmptyRaw, {}, {}, {});
            pc.m_pools.AddAsset(IW4::ASSET_TYPE_SOUND_CURVE, CURVE_NAME, &pcCurve, {}, {}, {});
            pc.m_pools.AddAsset(IW4::ASSET_TYPE_MAP_ENTS, MAP_NAME, &pcMap, {}, {}, {});
            pc.m_pools.AddAsset(IW4::ASSET_TYPE_STRINGTABLE, TABLE_NAME, &pcTable, {}, {}, {});
            pc.m_pools.AddAsset(IW4::ASSET_TYPE_LOCALIZE_ENTRY, LOCALIZE_NAME, &pcLocalize, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_RAWFILE, RAW_NAME, &xenonRaw, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_RAWFILE, EMPTY_RAW_NAME, &xenonEmptyRaw, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_SOUND_CURVE, CURVE_NAME, &xenonCurve, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_MAP_ENTS, MAP_NAME, &xenonMap, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_STRINGTABLE, TABLE_NAME, &xenonTable, {}, {}, {});
            xenon.m_pools.AddAsset(IW4Xenon482::ASSET_TYPE_LOCALIZE_ENTRY, LOCALIZE_NAME, &xenonLocalize, {}, {}, {});

            AssetDumpingContext pcContext(pc, "", pcOutput, emptySearchPath, std::nullopt);
            AssetDumpingContext xenonContext(xenon, "", xenonOutput, emptySearchPath, std::nullopt);
            REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4)->DumpZone(pcContext));
            REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4Xenon482)->DumpZone(xenonContext));
        }

        REQUIRE(pcOutput.GetMockedFileList().size() == 6);
        REQUIRE(xenonOutput.GetMockedFileList().size() == 6);
        MockSearchPath dumpedSearchPath;
        for (const auto& pcFile : pcOutput.GetMockedFileList())
        {
            const auto* xenonFile = xenonOutput.GetMockedFile(pcFile.m_name);
            REQUIRE(xenonFile);
            CHECK(xenonFile->m_data == pcFile.m_data);
            dumpedSearchPath.AddFileData(pcFile.m_name, pcFile.AsString());
        }

        const auto* entityFile = xenonOutput.GetMockedFile("maps/mp/test_map.d3dbsp.ents");
        REQUIRE(entityFile);
        CHECK(entityFile->m_data.size() == sizeof(entities) - 1);

        Zone target(ZONE_NAME, 0, GameId::IW4Xenon482, GamePlatform::XBOX);
        target.m_language = GameLanguage::LANGUAGE_ENGLISH;
        AssetCreatorCollection creators(target);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::IW4Xenon482)->ConfigureCreatorCollection(creators, target, dumpedSearchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        REQUIRE(context.LoadDependency<IW4Xenon482::AssetMapEnts>(MAP_NAME));
        REQUIRE(context.LoadDependency<IW4Xenon482::AssetRawFile>(EMPTY_RAW_NAME));
        REQUIRE(context.LoadDependency<IW4Xenon482::AssetSoundCurve>(CURVE_NAME));
        REQUIRE(context.LoadDependency<IW4Xenon482::AssetStringTable>(TABLE_NAME));
        REQUIRE(context.LoadDependency<IW4Xenon482::AssetLocalize>(ZONE_NAME));
        const auto* rawInfo = context.LoadDependency<IW4Xenon482::AssetRawFile>(RAW_NAME);
        REQUIRE(rawInfo);
        CHECK(rawInfo->m_zone == &target);
        REQUIRE(target.m_pools.GetTotalAssetCount() == 6);

        const auto checkAssets = [&](const Zone& zone)
        {
            const auto* raw = zone.m_pools.GetAsset<IW4Xenon482::AssetRawFile>(RAW_NAME);
            REQUIRE(raw);
            CHECK(raw->Asset()->len == rawBytes.size());
            REQUIRE(raw->Asset()->compressedLen > 0);
            std::string uncompressed(rawBytes.size(), '\0');
            uLongf length = static_cast<uLongf>(uncompressed.size());
            REQUIRE(uncompress(reinterpret_cast<Bytef*>(uncompressed.data()),
                               &length,
                               reinterpret_cast<const Bytef*>(raw->Asset()->data.compressedBuffer),
                               raw->Asset()->compressedLen)
                    == Z_OK);
            CHECK(length == rawBytes.size());
            CHECK(uncompressed == rawBytes);

            const auto* emptyRaw = zone.m_pools.GetAsset<IW4Xenon482::AssetRawFile>(EMPTY_RAW_NAME);
            REQUIRE(emptyRaw);
            CHECK(emptyRaw->Asset()->len == 0);
            CHECK(emptyRaw->Asset()->compressedLen > 0);

            const auto* map = zone.m_pools.GetAsset<IW4Xenon482::AssetMapEnts>(MAP_NAME);
            REQUIRE(map);
            CHECK(map->Asset()->numEntityChars == sizeof(entities));
            CHECK(std::string(map->Asset()->entityString, map->Asset()->numEntityChars) == std::string(entities, sizeof(entities)));

            const auto* table = zone.m_pools.GetAsset<IW4Xenon482::AssetStringTable>(TABLE_NAME);
            REQUIRE(table);
            CHECK(table->Asset()->columnCount == 2);
            CHECK(table->Asset()->rowCount == 2);
            for (auto index = 0u; index < 4; ++index)
            {
                CHECK(std::string(table->Asset()->values[index].string) == cells[index]);
                CHECK(table->Asset()->values[index].hash == IW4::Common::StringTable_HashString(cells[index]));
            }
            CHECK(table->Asset()->values[0].hash == 0x364492);

            const auto* curve = zone.m_pools.GetAsset<IW4Xenon482::AssetSoundCurve>(CURVE_NAME);
            REQUIRE(curve);
            REQUIRE(curve->Asset()->knotCount == 2);
            CHECK(curve->Asset()->knots[0][0] == 0.0f);
            CHECK(curve->Asset()->knots[0][1] == 1.0f);
            CHECK(curve->Asset()->knots[1][0] == 1.0f);
            CHECK(curve->Asset()->knots[1][1] == 0.25f);

            const auto* localize = zone.m_pools.GetAsset<IW4Xenon482::AssetLocalize>(LOCALIZE_NAME);
            REQUIRE(localize);
            CHECK(std::string(localize->Asset()->value) == LOCALIZE_VALUE);
        };
        checkAssets(target);
        const auto checkDump = [&](Zone& zone)
        {
            MockOutputPath output;
            AssetDumpingContext dumpingContext(zone, "", output, emptySearchPath, std::nullopt);
            REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW4Xenon482)->DumpZone(dumpingContext));
            REQUIRE(output.GetMockedFileList().size() == pcOutput.GetMockedFileList().size());
            for (const auto& pcFile : pcOutput.GetMockedFileList())
            {
                const auto* file = output.GetMockedFile(pcFile.m_name);
                REQUIRE(file);
                CHECK(file->m_data == pcFile.m_data);
            }
        };
        checkDump(target);

#ifdef ARCH_x86
        const auto path = oat::paths::GetTempDirectory("IW4Xenon482TextAssets") / "textassets.ff";
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, target));
        }
        const auto reloaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(reloaded);
        REQUIRE((*reloaded)->m_pools.GetTotalAssetCount() == 6);
        checkAssets(**reloaded);
        checkDump(**reloaded);
#endif
    }

} // namespace
