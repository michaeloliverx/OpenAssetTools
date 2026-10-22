#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "OatTestPaths.h"
#include "Zone/Definition/ZoneDefWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>

#ifdef ARCH_x86

TEST_CASE("IW4 482 retains sound files and nested model surfaces", "[iw4xenon482][zone][system]")
{
    using namespace IW4Xenon482;
    Zone zone("nested_assets", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
    SoundFile packed{};
    packed.type = SAT_STREAMED;
    packed.exists = true;
    packed.u.streamSnd.filename.fileIndex = 3;
    packed.u.streamSnd.filename.info.packed = {0x12345678, 0x1234};
    SoundFile raw{};
    raw.type = SAT_STREAMED;
    raw.exists = true;
    raw.u.streamSnd.filename.info.raw = {"sound/test", "tone.wav"};
    snd_alias_t aliases[2]{};
    aliases[0].aliasName = "packed";
    aliases[0].soundFile = &packed;
    aliases[1].aliasName = "raw";
    aliases[1].soundFile = &raw;
    snd_alias_list_t sounds[]{
        {"packed", &aliases[0], 1},
        {"raw",    &aliases[1], 1}
    };
    for (auto& sound : sounds)
        zone.m_pools.AddAsset(ASSET_TYPE_SOUND, sound.aliasName, &sound, {}, {}, {});

    XSurface surface{};
    surface.partBits.m_array[4] = 0x12345678;
    XModelSurfs surfaces{};
    surfaces.name = "model_surfs";
    surfaces.numsurfs = 1;
    surfaces.surfs = &surface;
    Material* materials[]{nullptr};
    XModel model{};
    model.name = "model";
    model.numsurfs = 1;
    model.numLods = 1;
    model.scale = 1.0f;
    model.materialHandles = materials;
    model.lodInfo[0].numsurfs = 1;
    model.lodInfo[0].modelSurfs = &surfaces;
    zone.m_pools.AddAsset(ASSET_TYPE_XMODEL, model.name, &model, {}, {}, {});

    const auto path = oat::paths::GetTempDirectory("IW4Xenon482RoundTrip") / "nested_assets.ffm";
    {
        std::ofstream output(path, std::ios::binary);
        REQUIRE(ZoneWriting::WriteZone(output, zone));
    }
    auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
    REQUIRE(loaded);
    REQUIRE((*loaded)->m_game_id == GameId::IW4Xenon482);
    REQUIRE((*loaded)->m_pools.GetTotalAssetCount() == 3);
    const auto* loadedPacked = (*loaded)->m_pools.GetAsset<AssetSound>("packed");
    const auto* loadedRaw = (*loaded)->m_pools.GetAsset<AssetSound>("raw");
    REQUIRE(loadedPacked);
    REQUIRE(loadedRaw);
    REQUIRE(loadedPacked->Asset()->head->soundFile->type == SAT_STREAMED);
    REQUIRE(loadedPacked->Asset()->head->soundFile->u.streamSnd.filename.fileIndex == 3);
    REQUIRE(loadedPacked->Asset()->head->soundFile->u.streamSnd.filename.info.packed.offset == 0x12345678);
    REQUIRE(loadedPacked->Asset()->head->soundFile->u.streamSnd.filename.info.packed.length == 0x1234);
    REQUIRE(loadedRaw->Asset()->head->soundFile->type == SAT_STREAMED);
    REQUIRE(std::string(loadedRaw->Asset()->head->soundFile->u.streamSnd.filename.info.raw.dir) == "sound/test");
    REQUIRE(std::string(loadedRaw->Asset()->head->soundFile->u.streamSnd.filename.info.raw.name) == "tone.wav");
    const auto* loadedModel = (*loaded)->m_pools.GetAsset<AssetXModel>(model.name);
    REQUIRE(loadedModel);
    REQUIRE(loadedModel->Asset()->lodInfo[0].modelSurfs);
    REQUIRE(loadedModel->Asset()->lodInfo[0].modelSurfs->numsurfs == 1);
    REQUIRE(loadedModel->Asset()->lodInfo[0].modelSurfs->surfs->partBits.m_array[4] == 0x12345678);
    std::ostringstream definition;
    IZoneDefWriter::GetZoneDefWriterForGame(GameId::IW4Xenon482)->WriteZoneDef(definition, **loaded, false, false);
    REQUIRE(definition.str().find(">game,IW4Xenon482\n>platform,xbox360\n") != std::string::npos);
    REQUIRE(definition.str().find("xmodel,model\n") != std::string::npos);
    REQUIRE(definition.str().find("xmodelsurfs,") == std::string::npos);
}

TEST_CASE("IW4 482 rejects standalone model surfaces", "[iw4xenon482][zone][system]")
{
    using namespace IW4Xenon482;
    Zone zone("standalone_surfaces", 0, GameId::IW4Xenon482, GamePlatform::XBOX);
    XModelSurfs surfaces{};
    surfaces.name = "standalone_surfaces";
    zone.m_pools.AddAsset(ASSET_TYPE_XMODEL_SURFS, surfaces.name, &surfaces, {}, {}, {});
    std::ostringstream output;
    REQUIRE_FALSE(ZoneWriting::WriteZone(output, zone));
}

#endif
