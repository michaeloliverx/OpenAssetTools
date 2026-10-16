#include "Game/QOS/QOS.h"
#include "Game/QOSXenon/QOSXenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "Zone/Definition/ZoneDefWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>
#include <string>

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

    TEST_CASE("QoS Xenon text assets use PC disk formats through registered loaders and dumpers", "[qosxenon][textassets][system]")
    {
        RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(QOSXenon::ASSET_TYPE_COUNT, true);

        constexpr auto ZONE_NAME = "textassets";
        constexpr auto RAW_NAME = "scripts/test.bin";
        constexpr auto MAP_NAME = "maps/mp/test_map.d3dbsp";
        constexpr auto TABLE_NAME = "mp/test.csv";
        constexpr auto LOCALIZE_NAME = "TEXT_MESSAGE";
        const char rawBytes[]{'a', '\0', 'b', '\xff'};
        char entities[] = "{\n\"classname\" \"worldspawn\"\n}\n";
        const char* cells[]{"plain", "comma, value", "quote \"value\"", ""};
        constexpr auto LOCALIZE_VALUE = "Line one\nLine \"two\"";

        MockSearchPath emptySearchPath;
        MockOutputPath pcOutput;
        MockOutputPath xenonOutput;
        {
            Zone pc(ZONE_NAME, 0, GameId::QOS, GamePlatform::PC);
            Zone xenon(ZONE_NAME, 0, GameId::QOSXenon, GamePlatform::XBOX);
            pc.m_language = xenon.m_language = GameLanguage::LANGUAGE_ENGLISH;

            QOS::RawFile pcRaw{RAW_NAME, sizeof(rawBytes), rawBytes};
            QOSXenon::RawFile xenonRaw{RAW_NAME, sizeof(rawBytes), rawBytes};
            QOS::MapEnts pcMap{MAP_NAME, entities, sizeof(entities)};
            QOSXenon::MapEnts xenonMap{MAP_NAME, entities, sizeof(entities)};
            QOS::StringTable pcTable{TABLE_NAME, 2, 2, cells};
            QOSXenon::StringTable xenonTable{TABLE_NAME, 2, 2, cells};
            QOS::LocalizeEntry pcLocalize{LOCALIZE_VALUE, LOCALIZE_NAME};
            QOSXenon::LocalizeEntry xenonLocalize{LOCALIZE_VALUE, LOCALIZE_NAME};

            pc.m_pools.AddAsset(QOS::ASSET_TYPE_RAWFILE, RAW_NAME, &pcRaw, {}, {}, {});
            pc.m_pools.AddAsset(QOS::ASSET_TYPE_MAP_ENTS, MAP_NAME, &pcMap, {}, {}, {});
            pc.m_pools.AddAsset(QOS::ASSET_TYPE_STRINGTABLE, TABLE_NAME, &pcTable, {}, {}, {});
            pc.m_pools.AddAsset(QOS::ASSET_TYPE_LOCALIZE_ENTRY, LOCALIZE_NAME, &pcLocalize, {}, {}, {});
            xenon.m_pools.AddAsset(QOSXenon::ASSET_TYPE_RAWFILE, RAW_NAME, &xenonRaw, {}, {}, {});
            xenon.m_pools.AddAsset(QOSXenon::ASSET_TYPE_MAP_ENTS, MAP_NAME, &xenonMap, {}, {}, {});
            xenon.m_pools.AddAsset(QOSXenon::ASSET_TYPE_STRINGTABLE, TABLE_NAME, &xenonTable, {}, {}, {});
            xenon.m_pools.AddAsset(QOSXenon::ASSET_TYPE_LOCALIZE_ENTRY, LOCALIZE_NAME, &xenonLocalize, {}, {}, {});

            AssetDumpingContext pcContext(pc, "", pcOutput, emptySearchPath, std::nullopt);
            AssetDumpingContext xenonContext(xenon, "", xenonOutput, emptySearchPath, std::nullopt);
            REQUIRE(IObjWriter::GetObjWriterForGame(GameId::QOS)->DumpZone(pcContext));
            REQUIRE(IObjWriter::GetObjWriterForGame(GameId::QOSXenon)->DumpZone(xenonContext));
        }

        REQUIRE(pcOutput.GetMockedFileList().size() == 4);
        REQUIRE(xenonOutput.GetMockedFileList().size() == 4);
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

        Zone target(ZONE_NAME, 0, GameId::QOSXenon, GamePlatform::XBOX);
        target.m_language = GameLanguage::LANGUAGE_ENGLISH;
        AssetCreatorCollection creators(target);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::QOSXenon)->ConfigureCreatorCollection(creators, target, dumpedSearchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(target, &creators, &ignored);
        REQUIRE(context.LoadDependency<QOSXenon::AssetMapEnts>(MAP_NAME));
        REQUIRE(context.LoadDependency<QOSXenon::AssetStringTable>(TABLE_NAME));
        REQUIRE(context.LoadDependency<QOSXenon::AssetLocalize>(ZONE_NAME));
        const auto* rawInfo = context.LoadDependency<QOSXenon::AssetRawFile>(RAW_NAME);
        REQUIRE(rawInfo);
        CHECK(rawInfo->m_zone == &target);
        REQUIRE(target.m_pools.GetTotalAssetCount() == 4);

        const auto checkAssets = [&](const Zone& zone)
        {
            const auto* raw = zone.m_pools.GetAsset<QOSXenon::AssetRawFile>(RAW_NAME);
            REQUIRE(raw);
            CHECK(raw->Asset()->len == sizeof(rawBytes));
            CHECK(std::string(raw->Asset()->buffer, raw->Asset()->len) == std::string(rawBytes, sizeof(rawBytes)));

            const auto* map = zone.m_pools.GetAsset<QOSXenon::AssetMapEnts>(MAP_NAME);
            REQUIRE(map);
            CHECK(map->Asset()->numEntityChars == sizeof(entities));
            CHECK(std::string(map->Asset()->entityString, map->Asset()->numEntityChars) == std::string(entities, sizeof(entities)));

            const auto* table = zone.m_pools.GetAsset<QOSXenon::AssetStringTable>(TABLE_NAME);
            REQUIRE(table);
            CHECK(table->Asset()->columnCount == 2);
            CHECK(table->Asset()->rowCount == 2);
            for (auto index = 0u; index < 4; ++index)
                CHECK(std::string(table->Asset()->values[index]) == cells[index]);

            const auto* localize = zone.m_pools.GetAsset<QOSXenon::AssetLocalize>(LOCALIZE_NAME);
            REQUIRE(localize);
            CHECK(std::string(localize->Asset()->value) == LOCALIZE_VALUE);
        };
        checkAssets(target);

        std::ostringstream definition;
        IZoneDefWriter::GetZoneDefWriterForGame(target.m_game_id)->WriteZoneDef(definition, target, false, false);
        CHECK(definition.str().find(">game,QOS\n>platform,xbox360\n") != std::string::npos);
        CHECK(definition.str().find("localize,textassets\n") != std::string::npos);

#ifdef ARCH_x86
        const auto path = oat::paths::GetTempDirectory("QOSXenonTextAssets") / "textassets.ff";
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, target));
        }
        const auto reloaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(reloaded);
        REQUIRE((*reloaded)->m_pools.GetTotalAssetCount() == 4);
        checkAssets(**reloaded);
#endif
    }

    TEST_CASE("QoS Xenon sound curves preserve knots through disk and fastfile formats", "[qosxenon][textassets][system]")
    {
        RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(QOSXenon::ASSET_TYPE_COUNT, true);
        MockSearchPath searchPath;
        searchPath.AddFileData("soundaliases/test.vfcurve", "SNDCURVE\n\n2\n0 1\n1 0\n");
        Zone zone("curves", 0, GameId::QOSXenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(GameId::QOSXenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(zone, &creators, &ignored);
        const auto* curve = context.LoadDependency<QOSXenon::AssetSoundCurve>("test");
        REQUIRE(curve);
        REQUIRE(curve->Asset()->knotCount == 2);
        CHECK(curve->Asset()->knots[0][1] == 1.0f);
        CHECK(curve->Asset()->knots[1][0] == 1.0f);
        MockOutputPath output;
        AssetDumpingContext dumping(zone, "", output, searchPath, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::QOSXenon)->DumpZone(dumping));
        const auto* file = output.GetMockedFile("soundaliases/test.vfcurve");
        REQUIRE(file);
        CHECK(file->AsString().find("SNDCURVE") != std::string::npos);

#ifdef ARCH_x86
        const auto path = oat::paths::GetTempDirectory("QOSXenonCurves") / "curves.ff";
        {
            std::ofstream stream(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(stream, zone));
        }
        const auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        const auto* reloaded = (*loaded)->m_pools.GetAsset<QOSXenon::AssetSoundCurve>("test");
        REQUIRE(reloaded);
        CHECK(reloaded->Asset()->knotCount == 2);
        CHECK(reloaded->Asset()->knots[0][1] == 1.0f);
        CHECK(reloaded->Asset()->knots[1][0] == 1.0f);
#endif
    }
} // namespace
