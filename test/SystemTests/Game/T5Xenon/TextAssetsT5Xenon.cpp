#include "Game/T5Xenon/T5Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("T5 Xenon text loaders and dumpers preserve text and compressed scripts", "[t5xenon][textassets][system]")
{
    MockSearchPath searchPath;
    const std::string script = "main() { println(\"test\"); }\n";
    const std::string entities = "{\n\"classname\" \"worldspawn\"\n}\n";
    searchPath.AddFileData("scripts/test.gsc", script);
    searchPath.AddFileData("scripts/test.csc", script);
    searchPath.AddFileData("maps/mp/test.d3dbsp.ents", entities);
    searchPath.AddFileData("mp/test.csv", "KEY,\"comma, value\"\nlast,cell\n");
    searchPath.AddFileData("english/localizedstrings/textassets.str",
                           "VERSION \"1\"\nCONFIG \"\"\nFILENOTES \"\"\nREFERENCE TEST\nLANG_ENGLISH \"Test message\"\nENDMARKER\n");
    Zone zone("textassets", 0, GameId::T5Xenon, GamePlatform::XBOX);
    zone.m_language = GameLanguage::LANGUAGE_ENGLISH;
    AssetCreatorCollection creators(zone);
    GdtLookup gdt;
    IObjLoader::GetObjLoaderForGame(GameId::T5Xenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
    IgnoredAssetLookup ignored;
    AssetCreationContext context(zone, &creators, &ignored);
    REQUIRE(context.LoadDependency<T5Xenon::AssetRawFile>("scripts/test.gsc"));
    REQUIRE(context.LoadDependency<T5Xenon::AssetRawFile>("scripts/test.csc"));
    REQUIRE(context.LoadDependency<T5Xenon::AssetMapEnts>("maps/mp/test.d3dbsp"));
    REQUIRE(context.LoadDependency<T5Xenon::AssetStringTable>("mp/test.csv"));
    REQUIRE(context.LoadDependency<T5Xenon::AssetLocalize>("textassets"));
    const auto* table = zone.m_pools.GetAsset<T5Xenon::AssetStringTable>("mp/test.csv");
    REQUIRE(table);
    REQUIRE(std::string(table->Asset()->values[1].string) == "comma, value");
    const auto* localize = zone.m_pools.GetAsset<T5Xenon::AssetLocalize>("TEST");
    REQUIRE(localize);
    REQUIRE(std::string(localize->Asset()->value) == "Test message");
    const auto previous = ObjWriting::Configuration;
    ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(T5Xenon::ASSET_TYPE_COUNT, true);
    MockOutputPath output;
    AssetDumpingContext dumping(zone, "", output, searchPath, std::nullopt);
    const auto dumped = IObjWriter::GetObjWriterForGame(GameId::T5Xenon)->DumpZone(dumping);
    ObjWriting::Configuration = previous;
    REQUIRE(dumped);
    REQUIRE(output.GetMockedFile("scripts/test.gsc"));
    REQUIRE(output.GetMockedFile("scripts/test.gsc")->AsString() == script);
    REQUIRE(output.GetMockedFile("scripts/test.csc")->AsString() == script);
    REQUIRE(output.GetMockedFile("maps/mp/test.d3dbsp.ents")->AsString() == entities);
}
