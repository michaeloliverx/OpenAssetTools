#include "Game/IW4Xenon/IW4Xenon.h"
#include "Game/IW4Xenon/LightDef/LightDefDumperIW4Xenon.h"
#include "Game/IW4Xenon/LightDef/LightDefLoaderIW4Xenon.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_test_macros.hpp>

using namespace IW4Xenon;

namespace
{
    TEST_CASE("IW4 Xenon light definitions preserve sampler state and account for streamed image widths", "[iw4xenon][system][lightdef]")
    {
        Zone zone("lights", 0, GameId::IW4Xenon, GamePlatform::XBOX);
        GfxImage image{};
        image.name = "attenuation";
        image.cached = 1;
        image.width = 1;
        image.streams[0].width = 64;
        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<GfxImage>>(ASSET_TYPE_IMAGE, image.name, &image));

        MockSearchPath searchPath;
        const std::string data("\x35"
                               "attenuation\0",
                               13);
        searchPath.AddFileData("lights/first", data);
        searchPath.AddFileData("lights/second", data);
        AssetCreatorCollection creators(zone);
        creators.AddAssetCreator(light_def::CreateLoaderIW4Xenon(zone.Memory(), searchPath));
        IgnoredAssetLookup ignored;
        AssetCreationContext context(zone, &creators, &ignored);
        REQUIRE(creators.CreateAsset(ASSET_TYPE_LIGHT_DEF, "first", context).HasBeenSuccessful());
        REQUIRE(creators.CreateAsset(ASSET_TYPE_LIGHT_DEF, "second", context).HasBeenSuccessful());

        const auto* first = zone.m_pools.GetAsset<AssetLightDef>("first");
        const auto* second = zone.m_pools.GetAsset<AssetLightDef>("second");
        REQUIRE(first);
        REQUIRE(second);
        CHECK(first->Asset()->attenuation.samplerState == 0x35);
        CHECK(first->Asset()->attenuation.image == &image);
        CHECK(first->Asset()->lmapLookupStart == 1);
        CHECK(second->Asset()->lmapLookupStart == 67);

        MockOutputPath output;
        AssetDumpingContext dumpingContext(zone, "", output, searchPath, std::nullopt);
        light_def::DumperIW4Xenon().Dump(dumpingContext);
        const auto* file = output.GetMockedFile("lights/first");
        REQUIRE(file);
        CHECK(file->AsString() == data);
    }
} // namespace
