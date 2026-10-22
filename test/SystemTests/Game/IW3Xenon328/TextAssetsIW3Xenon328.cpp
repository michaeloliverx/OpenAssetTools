#include "Game/IW3Xenon328/IW3Xenon328.h"
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

TEST_CASE("IW3 328 text assets survive disk loading, dumping and fastfile linking", "[iw3xenon328][textassets][system]")
{
    using namespace IW3Xenon328;

    struct RestoreConfiguration
    {
        ObjWriting::Configuration_t original{ObjWriting::Configuration};

        ~RestoreConfiguration()
        {
            ObjWriting::Configuration = original;
        }
    } restore;

    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, true);
    const char bytes[]{'a', '\0', 'b', '\xff'};
    MockSearchPath emptySearch;
    MockOutputPath dumped;
    {
        Zone source("textassets", 0, GameId::IW3Xenon328, GamePlatform::XBOX);
        source.m_language = GameLanguage::LANGUAGE_ENGLISH;
        RawFile raw{"scripts/test.bin", sizeof(bytes), bytes};
        LocalizeEntry localize{"Line one\nLine \"two\"", "TEXT_MESSAGE"};
        SndCurve curve{
            "test", 2, {{0, 1}, {1, 0}}
        };
        char entities[] = "{\n\"classname\" \"worldspawn\"\n}\n";
        MapEnts mapEnts{"maps/mp/test.d3dbsp", entities, sizeof(entities)};
        const char* cells[]{"plain", "with,comma", "with\"quote", ""};
        StringTable table{"test.csv", 2, 2, cells};
        source.m_pools.AddAsset(ASSET_TYPE_MAP_ENTS, mapEnts.name, &mapEnts, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_STRINGTABLE, table.name, &table, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_RAWFILE, raw.name, &raw, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_LOCALIZE_ENTRY, localize.name, &localize, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_SOUND_CURVE, curve.filename, &curve, {}, {}, {});
        AssetDumpingContext context(source, "", dumped, emptySearch, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(source.m_game_id)->DumpZone(context));
    }
    REQUIRE(dumped.GetMockedFileList().size() == 5);
    MockSearchPath search;
    for (const auto& file : dumped.GetMockedFileList())
        search.AddFileData(file.m_name, file.AsString());

    Zone target("textassets", 0, GameId::IW3Xenon328, GamePlatform::XBOX);
    target.m_language = GameLanguage::LANGUAGE_ENGLISH;
    AssetCreatorCollection creators(target);
    GdtLookup gdt;
    IObjLoader::GetObjLoaderForGame(target.m_game_id)->ConfigureCreatorCollection(creators, target, search, gdt);
    IgnoredAssetLookup ignored;
    AssetCreationContext context(target, &creators, &ignored);
    REQUIRE(context.LoadDependency<AssetRawFile>("scripts/test.bin"));
    REQUIRE(context.LoadDependency<AssetLocalize>("textassets"));
    REQUIRE(context.LoadDependency<AssetSoundCurve>("test"));
    REQUIRE(context.LoadDependency<AssetMapEnts>("maps/mp/test.d3dbsp"));
    REQUIRE(context.LoadDependency<AssetStringTable>("test.csv"));
    const auto check = [&](const Zone& zone)
    {
        const auto* raw = zone.m_pools.GetAsset<AssetRawFile>("scripts/test.bin");
        REQUIRE(raw);
        REQUIRE(raw->Asset()->len == sizeof(bytes));
        REQUIRE(std::string(raw->Asset()->buffer, raw->Asset()->len) == std::string(bytes, sizeof(bytes)));
        const auto* localize = zone.m_pools.GetAsset<AssetLocalize>("TEXT_MESSAGE");
        REQUIRE(localize);
        REQUIRE(std::string(localize->Asset()->value) == "Line one\nLine \"two\"");
        const auto* ents = zone.m_pools.GetAsset<AssetMapEnts>("maps/mp/test.d3dbsp");
        REQUIRE(ents);
        REQUIRE(std::string(ents->Asset()->entityString) == "{\n\"classname\" \"worldspawn\"\n}\n");
        const auto* table = zone.m_pools.GetAsset<AssetStringTable>("test.csv");
        REQUIRE(table);
        REQUIRE(table->Asset()->columnCount == 2);
        REQUIRE(table->Asset()->rowCount == 2);
        REQUIRE(std::string(table->Asset()->values[1]) == "with,comma");
        REQUIRE(std::string(table->Asset()->values[2]) == "with\"quote");
        REQUIRE(std::string(table->Asset()->values[3]).empty());
        const auto* curve = zone.m_pools.GetAsset<AssetSoundCurve>("test");
        REQUIRE(curve);
        REQUIRE(curve->Asset()->knotCount == 2);
        REQUIRE(curve->Asset()->knots[0][1] == 1);
        REQUIRE(curve->Asset()->knots[1][0] == 1);
    };
    check(target);
    std::ostringstream definition;
    IZoneDefWriter::GetZoneDefWriterForGame(target.m_game_id)->WriteZoneDef(definition, target, false, false);
    REQUIRE(definition.str().find(">game,IW3Xenon328\n>platform,xbox360\n") != std::string::npos);
    REQUIRE(definition.str().find("localize,textassets\n") != std::string::npos);
#ifdef ARCH_x86
    const auto path = oat::paths::GetTempDirectory("IW3Xenon328TextAssets") / "textassets.ff";
    {
        std::ofstream output(path, std::ios::binary);
        REQUIRE(ZoneWriting::WriteZone(output, target));
    }
    const auto reloaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
    REQUIRE(reloaded);
    check(**reloaded);
#endif
}
