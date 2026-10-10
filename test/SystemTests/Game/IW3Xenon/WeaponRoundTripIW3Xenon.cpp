#include "Game/IW3Xenon/IW3Xenon.h"
#include "Game/IW3Xenon/Weapon/WeaponStrings.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "InfoString/InfoString.h"
#include "OatTestPaths.h"
#include "Obj/Gdt/GdtStream.h"
#include "ObjWriter.h"
#include "ObjWriting.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <fstream>
#include <sstream>

#ifdef ARCH_x86

using namespace IW3Xenon;

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

    void AddWeaponDependencies(AssetCreatorCollection& creators, AssetCreationContext& context)
    {
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_XMODEL, ",test_model", context).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_MATERIAL, ",test_material", context).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_FX, ",test_fx", context).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_SOUND, ",test_sound", context).HasBeenSuccessful());
        REQUIRE(creators.CreateDefaultAsset(ASSET_TYPE_XANIMPARTS, ",test_anim", context).HasBeenSuccessful());
    }

    void CheckWeapon(const Zone& zone)
    {
        const auto* asset = zone.m_pools.GetAsset<AssetWeapon>("defaultweapon_mp");
        REQUIRE(asset);
        const auto& weapon = *asset->Asset();
        CHECK(std::string(weapon.szDisplayName) == "WEAPON_TEST");
        CHECK(weapon.weapType == WEAPTYPE_BULLET);
        CHECK(weapon.weapClass == WEAPCLASS_SMG);
        CHECK(weapon.penetrateType == PENETRATE_TYPE_LARGE);
        CHECK(weapon.fireType == WEAPON_FIRETYPE_BURSTFIRE3);
        CHECK(weapon.overlayInterface == WEAPOVERLAYINTERFACE_TURRETSCOPE);
        CHECK(weapon.damage == 37);
        CHECK(weapon.iFireTime == 125);
        CHECK(weapon.iAdsTransInTime == 200);
        CHECK_THAT(weapon.fOOPosAnimLength[0], Catch::Matchers::WithinAbs(0.005f, 0.00001f));
        CHECK(weapon.fMaxDamageRange == 1200.0f);
        CHECK(weapon.fMinDamageRange == 2400.0f);
        REQUIRE(weapon.gunXModel[0]);
        CHECK(std::string(weapon.gunXModel[0]->name) == ",test_model");
        REQUIRE(weapon.hudIcon);
        CHECK(std::string(weapon.hudIcon->info.name) == ",test_material");
        REQUIRE(weapon.viewFlashEffect);
        CHECK(std::string(weapon.viewFlashEffect->name) == ",test_fx");
        REQUIRE(weapon.pickupSound.name);
        CHECK(std::string(weapon.pickupSound.name->soundName) == "test_sound");
        CHECK(std::string(weapon.szXAnims[WEAP_ANIM_IDLE]) == "test_anim");
        CHECK(zone.m_script_strings.CValue(weapon.hideTags[0]) == std::string("tag_flash"));
        CHECK(zone.m_script_strings.CValue(weapon.hideTags[1]) == std::string("tag_clip"));
        CHECK(zone.m_script_strings.CValue(weapon.notetrackSoundMapKeys[0]) == std::string("reload"));
        CHECK(zone.m_script_strings.CValue(weapon.notetrackSoundMapValues[0]) == std::string("test_sound"));
        REQUIRE(weapon.bounceSound);
        for (auto surface = 0u; surface < SURF_TYPE_NUM; ++surface)
        {
            REQUIRE(weapon.bounceSound[surface].name);
            CHECK(std::string(weapon.bounceSound[surface].name->soundName) == std::string("test_bounce") + bounceSoundSuffixes[surface]);
        }
        REQUIRE(weapon.aiVsAiAccuracyGraphKnotCount == 3);
        REQUIRE(weapon.originalAiVsAiAccuracyGraphKnotCount == 3);
        REQUIRE(weapon.aiVsPlayerAccuracyGraphKnotCount == 3);
        REQUIRE(weapon.originalAiVsPlayerAccuracyGraphKnotCount == 3);
        REQUIRE(weapon.aiVsAiAccuracyGraphKnots);
        REQUIRE(weapon.originalAiVsAiAccuracyGraphKnots);
        REQUIRE(weapon.aiVsPlayerAccuracyGraphKnots);
        REQUIRE(weapon.originalAiVsPlayerAccuracyGraphKnots);
        CHECK(weapon.aiVsAiAccuracyGraphKnots[1].x == 0.5f);
        CHECK(weapon.originalAiVsAiAccuracyGraphKnots[1].y == 0.75f);
        CHECK(weapon.aiVsPlayerAccuracyGraphKnots[1].y == 0.75f);
        CHECK(weapon.originalAiVsPlayerAccuracyGraphKnots[1].x == 0.5f);
    }

    TEST_CASE("IW3 Xenon weapons preserve raw and GDT fields through native fastfiles", "[iw3xenon][system][weapon]")
    {
        const auto gdtInput = GENERATE(false, true);
        const auto gdtOutput = GENERATE(false, true);
        CAPTURE(gdtInput, gdtOutput);
        const RestoreObjWritingConfiguration restoreConfiguration;
        ObjWriting::Configuration.AssetTypesToHandleBitfield.assign(ASSET_TYPE_COUNT, false);
        ObjWriting::Configuration.AssetTypesToHandleBitfield[ASSET_TYPE_WEAPON] = true;

        InfoString source;
        for (const auto& [key, value] : {
                 std::pair{"displayName",             "WEAPON_TEST"        },
                 {"weaponType",              "bullet"             },
                 {"weaponClass",             "smg"                },
                 {"penetrateType",           "large"              },
                 {"fireType",                "3-Round Burst"      },
                 {"adsOverlayInterface",     "Turret Scope"       },
                 {"damage",                  "37"                 },
                 {"fireTime",                "0.125"              },
                 {"adsTransInTime",          "0.2"                },
                 {"maxDamageRange",          "1200"               },
                 {"minDamageRange",          "2400"               },
                 {"gunModel",                "test_model"         },
                 {"hudIcon",                 "test_material"      },
                 {"viewFlashEffect",         "test_fx"            },
                 {"pickupSound",             "test_sound"         },
                 {"idleAnim",                "test_anim"          },
                 {"hideTags",                "tag_flash\ntag_clip"},
                 {"notetrackSoundMap",       "reload test_sound"  },
                 {"bounceSound",             "test_bounce"        },
                 {"aiVsAiAccuracyGraph",     "test_graph"         },
                 {"aiVsPlayerAccuracyGraph", "test_graph"         },
        })
            source.SetValueForKey(key, value);

        MockSearchPath searchPath;
        constexpr auto graph = "WEAPONACCUFILE\n\n3\n0.0 0.1\n0.5 0.75\n1.0 1.0\n";
        searchPath.AddFileData("accuracy/aivsai/test_graph", graph);
        searchPath.AddFileData("accuracy/aivsplayer/test_graph", graph);
        Gdt sourceGdt;
        GdtLookup gdt;
        if (gdtInput)
        {
            auto entry = std::make_unique<GdtEntry>("defaultweapon_mp", "weapon.gdf");
            source.ToGdtProperties("WEAPONFILE", *entry);
            sourceGdt.m_entries.emplace_back(std::move(entry));
            gdt.Initialize({&sourceGdt});
        }
        else
            searchPath.AddFileData("weapons/defaultweapon_mp", source.ToString("WEAPONFILE"));

        Zone zone("weapon_round_trip", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        AssetCreatorCollection creators(zone);
        IObjLoader::GetObjLoaderForGame(GameId::IW3Xenon)->ConfigureCreatorCollection(creators, zone, searchPath, gdt);
        IgnoredAssetLookup ignoredAssets;
        AssetCreationContext context(zone, &creators, &ignoredAssets);
        AddWeaponDependencies(creators, context);
        REQUIRE(creators.CreateAsset(ASSET_TYPE_WEAPON, "defaultweapon_mp", context).HasBeenSuccessful());
        CheckWeapon(zone);

        const auto path = oat::paths::GetTempDirectory("IW3XenonWeaponRoundTrip") / "weapon.ff";
        {
            std::ofstream fastfile(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(fastfile, zone));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        CheckWeapon(**loaded);

        MockOutputPath output;
        const std::string basePath;
        AssetDumpingContext dumpingContext(**loaded, basePath, output, searchPath, std::nullopt);
        std::ostringstream gdtStream;
        if (gdtOutput)
        {
            dumpingContext.m_gdt = std::make_unique<GdtOutputStream>(gdtStream);
            dumpingContext.m_gdt->BeginStream();
        }
        REQUIRE(IObjWriter::GetObjWriterForGame(GameId::IW3Xenon)->DumpZone(dumpingContext));
        Gdt dumpedGdt;
        GdtLookup dumpedGdtLookup;
        MockSearchPath dumpedSearchPath;
        if (gdtOutput)
        {
            dumpingContext.m_gdt->EndStream();
            std::istringstream gdtInputStream(gdtStream.str());
            REQUIRE(GdtReader(gdtInputStream).Read(dumpedGdt));
            REQUIRE(dumpedGdt.m_entries.size() == 1u);
            CHECK(dumpedGdt.m_entries[0]->m_gdf_name == "weapon.gdf");
            dumpedGdtLookup.Initialize({&dumpedGdt});
        }
        else
        {
            const auto* weaponFile = output.GetMockedFile("weapons/defaultweapon_mp");
            REQUIRE(weaponFile);
            dumpedSearchPath.AddFileData("weapons/defaultweapon_mp", weaponFile->AsString());
        }
        for (const auto* graphPath : {"accuracy/aivsai/test_graph", "accuracy/aivsplayer/test_graph"})
        {
            const auto* graphFile = output.GetMockedFile(graphPath);
            REQUIRE(graphFile);
            dumpedSearchPath.AddFileData(graphPath, graphFile->AsString());
        }
        Zone relinked("weapon_relinked", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        AssetCreatorCollection relinkCreators(relinked);
        IObjLoader::GetObjLoaderForGame(GameId::IW3Xenon)->ConfigureCreatorCollection(relinkCreators, relinked, dumpedSearchPath, dumpedGdtLookup);
        AssetCreationContext relinkContext(relinked, &relinkCreators, &ignoredAssets);
        AddWeaponDependencies(relinkCreators, relinkContext);
        REQUIRE(relinkCreators.CreateAsset(ASSET_TYPE_WEAPON, "defaultweapon_mp", relinkContext).HasBeenSuccessful());
        CheckWeapon(relinked);
    }
} // namespace

#endif
