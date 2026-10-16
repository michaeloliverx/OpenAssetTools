#include "Game/QOSXenon/QOSXenon.h"
#include "OatTestPaths.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>

#ifdef ARCH_x86
namespace
{
    std::unique_ptr<Zone> RoundTripZone(const Zone& zone)
    {
        const auto path = oat::paths::GetTempDirectory("QOSXenonShaderAliases") / (zone.m_name + ".ff");
        {
            std::ofstream output(path, std::ios::binary);
            REQUIRE(output.is_open());
            REQUIRE(ZoneWriting::WriteZone(output, zone));
        }

        std::ifstream input(path, std::ios::binary);
        char header[4]{};
        input.read(header, sizeof(header));
        REQUIRE(header[0] == 0);
        REQUIRE(header[1] == 0);
        REQUIRE(static_cast<unsigned char>(header[2]) == 1);
        REQUIRE(static_cast<unsigned char>(header[3]) == 0xd6);

        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        REQUIRE((*loaded)->m_game_id == GameId::QOSXenon);
        REQUIRE((*loaded)->m_platform == GamePlatform::XBOX);
        return std::move(*loaded);
    }

    TEST_CASE("QoS Xenon technique passes resolve shared pixel shaders through asset aliases", "[qosxenon][system][zone][pixelshader]")
    {
        Zone zone("shader_aliases", 0, GameId::QOSXenon, GamePlatform::XBOX);
        QOSXenon::MaterialPixelShader shader{};
        shader.name = "shared_pixel_shader";
        zone.m_pools.AddAsset(QOSXenon::ASSET_TYPE_PIXELSHADER, shader.name, &shader, {}, {}, {});
        QOSXenon::MaterialTechnique first{};
        first.name = "first";
        first.passCount = 1;
        first.passArray[0].pixelShader = &shader;
        auto second = first;
        second.name = "second";
        QOSXenon::MaterialTechniqueSet techset{};
        techset.name = "shared_shaders";
        techset.techniques[0] = &first;
        techset.techniques[1] = &second;
        zone.m_pools.AddAsset(QOSXenon::ASSET_TYPE_TECHNIQUE_SET, techset.name, &techset, {}, {}, {});

        const auto loaded = RoundTripZone(zone);
        const auto* loadedSet = loaded->m_pools.GetAsset<QOSXenon::AssetTechniqueSet>(techset.name);
        const auto* loadedShader = loaded->m_pools.GetAsset<QOSXenon::AssetPixelShader>(shader.name);
        REQUIRE(loadedSet);
        REQUIRE(loadedShader);
        REQUIRE(loadedSet->Asset()->techniques[0]->passArray[0].pixelShader == loadedShader->Asset());
        REQUIRE(loadedSet->Asset()->techniques[1]->passArray[0].pixelShader == loadedShader->Asset());
        REQUIRE(std::string(loadedShader->Asset()->name) == shader.name);
    }

} // namespace
#endif
