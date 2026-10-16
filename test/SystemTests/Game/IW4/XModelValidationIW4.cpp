#include "Game/IW4/IW4.h"
#include "Game/IW4/XModel/XModelDumperIW4.h"
#include "Game/IW4/XModel/XModelToCommonConverterIW4.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"

#include <catch2/catch_test_macros.hpp>

using namespace IW4;

namespace
{
    TEST_CASE("IW4 model dumping skips incomplete assets without emitting model metadata", "[iw4][system][xmodel]")
    {
        Zone zone("incomplete_models", 0, GameId::IW4, GamePlatform::PC);
        Material material{};
        material.info.name = "material";
        Material* materials[]{&material};
        XSurface surface{};
        XModelSurfs surfs{};
        surfs.numsurfs = 1;
        surfs.surfs = &surface;
        XModel model{};
        model.name = "incomplete";
        model.numLods = 1;
        model.numsurfs = 1;
        model.materialHandles = materials;
        model.lodInfo[0].numsurfs = 1;
        model.lodInfo[0].modelSurfs = &surfs;
        model.lodInfo[0].surfs = &surface;

        SECTION("missing material array")
        {
            model.materialHandles = nullptr;
        }
        SECTION("missing material")
        {
            materials[0] = nullptr;
        }
        SECTION("reference-only surfaces")
        {
            surfs.surfs = nullptr;
            model.lodInfo[0].surfs = nullptr;
        }
        SECTION("missing vertex data")
        {
            surface.vertCount = 3;
        }
        SECTION("missing triangles")
        {
            surface.triCount = 1;
        }
        SECTION("missing bone data")
        {
            model.numBones = 1;
        }
        SECTION("invalid LOD count")
        {
            model.numLods = 5;
        }

        zone.m_pools.AddAsset(std::make_unique<XAssetInfo<XModel>>(ASSET_TYPE_XMODEL, model.name, &model));
        const auto* asset = zone.m_pools.GetAsset<AssetXModel>(model.name);
        REQUIRE(asset);
        CHECK_FALSE(xmodel::ToCommonConverterIW4().Convert(*asset, 0));
        MockSearchPath searchPath;
        MockOutputPath output;
        AssetDumpingContext context(zone, "", output, searchPath, std::nullopt);
        xmodel::DumperIW4().Dump(context);
        CHECK_FALSE(output.GetMockedFile("xmodel/incomplete.json"));
    }
} // namespace
