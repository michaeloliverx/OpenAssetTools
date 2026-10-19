#include "Game/IW2Xenon/IW2Xenon.h"
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

TEST_CASE("IW2 text assets survive disk loading, dumping and fastfile linking", "[iw2xenon][textassets][system]")
{
    using namespace IW2Xenon;

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
        Zone source("textassets", 0, GameId::IW2Xenon, GamePlatform::XBOX);
        source.m_language = GameLanguage::LANGUAGE_ENGLISH;
        RawFile raw{"scripts/test.bin", sizeof(bytes), bytes};
        LocalizeEntry localize{"Line one\nLine \"two\"", "TEXT_MESSAGE"};
        LocalizeEntry reference{nullptr, ",TEXT_REFERENCE"};
        SndCurve curve{
            "test", 2, {{0, 1}, {1, 0}}
        };
        source.m_pools.AddAsset(ASSET_TYPE_RAWFILE, raw.name, &raw, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_LOCALIZE_ENTRY, localize.name, &localize, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_LOCALIZE_ENTRY, reference.name, &reference, {}, {}, {});
        source.m_pools.AddAsset(ASSET_TYPE_SND_CURVE, curve.name, &curve, {}, {}, {});
        AssetDumpingContext context(source, "", dumped, emptySearch, std::nullopt);
        REQUIRE(IObjWriter::GetObjWriterForGame(source.m_game_id)->DumpZone(context));
    }
    REQUIRE(dumped.GetMockedFileList().size() == 3);
    MockSearchPath search;
    for (const auto& file : dumped.GetMockedFileList())
    {
        REQUIRE(file.AsString().find("TEXT_REFERENCE") == std::string::npos);
        search.AddFileData(file.m_name, file.AsString());
    }

    Zone target("textassets", 0, GameId::IW2Xenon, GamePlatform::XBOX);
    target.m_language = GameLanguage::LANGUAGE_ENGLISH;
    AssetCreatorCollection creators(target);
    GdtLookup gdt;
    IObjLoader::GetObjLoaderForGame(target.m_game_id)->ConfigureCreatorCollection(creators, target, search, gdt);
    IgnoredAssetLookup ignored;
    AssetCreationContext context(target, &creators, &ignored);
    REQUIRE(context.LoadDependency<AssetRawFile>("scripts/test.bin"));
    REQUIRE(context.LoadDependency<AssetLocalize>("textassets"));
    REQUIRE(context.LoadDependency<AssetSoundCurve>("test"));
    const auto check = [&](const Zone& zone)
    {
        const auto* raw = zone.m_pools.GetAsset<AssetRawFile>("scripts/test.bin");
        REQUIRE(raw);
        REQUIRE(raw->Asset()->len == sizeof(bytes));
        REQUIRE(std::string(raw->Asset()->buffer, raw->Asset()->len) == std::string(bytes, sizeof(bytes)));
        const auto* localize = zone.m_pools.GetAsset<AssetLocalize>("TEXT_MESSAGE");
        REQUIRE(localize);
        REQUIRE(std::string(localize->Asset()->value) == "Line one\nLine \"two\"");
        const auto* curve = zone.m_pools.GetAsset<AssetSoundCurve>("test");
        REQUIRE(curve);
        REQUIRE(curve->Asset()->knotCount == 2);
        REQUIRE(curve->Asset()->knots[0][1] == 1);
        REQUIRE(curve->Asset()->knots[1][0] == 1);
    };
    check(target);
    std::ostringstream definition;
    IZoneDefWriter::GetZoneDefWriterForGame(target.m_game_id)->WriteZoneDef(definition, target, false, false);
    REQUIRE(definition.str().find(">game,IW2\n>platform,xbox360\n") != std::string::npos);
    REQUIRE(definition.str().find("localize,textassets\n") != std::string::npos);
#ifdef ARCH_x86
    const auto path = oat::paths::GetTempDirectory("IW2XenonTextAssets") / "textassets.ff";
    {
        std::ofstream output(path, std::ios::binary);
        REQUIRE(ZoneWriting::WriteZone(output, target));
    }
    const auto reloaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
    REQUIRE(reloaded);
    check(**reloaded);
#endif
}
