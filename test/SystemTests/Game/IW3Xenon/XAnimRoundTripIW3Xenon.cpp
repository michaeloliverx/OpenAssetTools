#include "Game/IW3/XAnim/XAnimDumperIW3.h"
#include "Game/IW3Xenon/XAnim/XAnimDumperIW3Xenon.h"
#include "Gdt/GdtLookup.h"
#include "IObjLoader.h"
#include "OatTestPaths.h"
#include "SearchPath/MockOutputPath.h"
#include "SearchPath/MockSearchPath.h"
#include "XAnim/CompiledXAnimLoader.h"
#include "XAnim/CompiledXAnimWriter.h"
#include "ZoneLoading.h"
#include "ZoneWriting.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <fstream>
#include <sstream>
#include <unordered_map>

#ifdef ARCH_x86

namespace
{
    std::unique_ptr<Zone> LoadDiskAnimation(const GameId game, const std::string& data)
    {
        auto zone = std::make_unique<Zone>("animation", 0, game, game == GameId::IW3 ? GamePlatform::PC : GamePlatform::XBOX);
        MockSearchPath searchPath;
        searchPath.AddFileData("xanim/test_anim", data);
        AssetCreatorCollection creators(*zone);
        GdtLookup gdt;
        IObjLoader::GetObjLoaderForGame(game)->ConfigureCreatorCollection(creators, *zone, searchPath, gdt);
        IgnoredAssetLookup ignored;
        AssetCreationContext context(*zone, &creators, &ignored);
        const auto type = game == GameId::IW3 ? static_cast<asset_type_t>(IW3::ASSET_TYPE_XANIMPARTS) : IW3Xenon::ASSET_TYPE_XANIMPARTS;
        REQUIRE(creators.CreateAsset(type, "test_anim", context).HasBeenSuccessful());
        return zone;
    }

    std::string DumpAnimation(const Zone& zone)
    {
        MockSearchPath searchPath;
        MockOutputPath output;
        const std::string basePath;
        AssetDumpingContext context(zone, basePath, output, searchPath, std::nullopt);
        if (zone.m_game_id == GameId::IW3)
            xanim::DumperIW3().Dump(context);
        else
            xanim::DumperIW3Xenon().Dump(context);
        const auto* file = output.GetMockedFile("xanim/test_anim");
        REQUIRE(file);
        return file->AsString();
    }

    std::unique_ptr<xanim::CommonXAnimParts> ReadAnimation(const std::string& data)
    {
        std::istringstream stream(data, std::ios::binary);
        auto parsed = xanim::LoadCompiledXAnim(stream);
        REQUIRE(parsed.has_value());
        return std::move(parsed).value();
    }

    void CheckRotation(const xanim::CommonXQuat& actual, const xanim::CommonXQuat& expected)
    {
        auto dot = 0.0;
        auto actualLength = 0.0;
        auto expectedLength = 0.0;
        for (auto axis = 0u; axis < 4u; ++axis)
        {
            dot += static_cast<double>(actual.value[axis]) * expected.value[axis];
            actualLength += static_cast<double>(actual.value[axis]) * actual.value[axis];
            expectedLength += static_cast<double>(expected.value[axis]) * expected.value[axis];
        }
        REQUIRE(actualLength > 0.0);
        REQUIRE(expectedLength > 0.0);
        // Version 17 reconstructs w from three quantized components; rotations near 180 degrees lose the most precision.
        constexpr auto HALF_DEGREE_DOT = 0.9999904807;
        CHECK(std::abs(dot) / std::sqrt(actualLength * expectedLength) > HALF_DEGREE_DOT);
    }

    xanim::CommonXQuat GetRotation(const xanim::QuatTrack& track, const size_t index)
    {
        if (track.m_type == xanim::QuatType::HALF_QUAT || track.m_type == xanim::QuatType::HALF_QUAT_NO_SIZE)
            return {0, 0, track.m_frames2[index].value[0], track.m_frames2[index].value[1]};
        return track.m_frames[index];
    }

    template<typename T> void CheckFrames(const std::vector<T>& actual, const std::vector<T>& expected)
    {
        REQUIRE(actual.size() == expected.size());
        for (auto frame = 0uz; frame < expected.size(); ++frame)
            for (auto axis = 0u; axis < 3u; ++axis)
                CHECK(actual[frame].value[axis] == expected[frame].value[axis]);
    }

    void CheckAnimation(const xanim::CommonXAnimParts& actual, const xanim::CommonXAnimParts& expected)
    {
        CHECK(actual.m_num_frames == expected.m_num_frames);
        CHECK(actual.m_looped == expected.m_looped);
        CHECK(actual.m_frame_rate == expected.m_frame_rate);
        CHECK(actual.m_asset_type == expected.m_asset_type);
        REQUIRE(actual.m_notifies.size() == expected.m_notifies.size());
        for (auto i = 0uz; i < expected.m_notifies.size(); ++i)
        {
            CHECK(actual.m_notifies[i].m_name == expected.m_notifies[i].m_name);
            CHECK(actual.m_notifies[i].m_time == expected.m_notifies[i].m_time);
        }
        std::unordered_map<std::string, const xanim::BoneTrack*> bones;
        for (const auto& bone : actual.m_bone_tracks)
            bones.emplace(bone.m_name, &bone);
        REQUIRE(bones.size() == expected.m_bone_tracks.size());
        for (const auto& expectedBone : expected.m_bone_tracks)
        {
            CAPTURE(expectedBone.m_name);
            REQUIRE(bones.contains(expectedBone.m_name));
            const auto& bone = *bones.at(expectedBone.m_name);
            CHECK(bone.m_quat.m_indices == expectedBone.m_quat.m_indices);
            if (expectedBone.m_quat.m_type == xanim::QuatType::NO_QUAT)
                CHECK(bone.m_quat.m_type == xanim::QuatType::NO_QUAT);
            else
            {
                const auto frames = expectedBone.m_quat.m_frames.size() + expectedBone.m_quat.m_frames2.size();
                REQUIRE(bone.m_quat.m_frames.size() + bone.m_quat.m_frames2.size() == frames);
                for (auto i = 0uz; i < frames; ++i)
                    CheckRotation(GetRotation(bone.m_quat, i), GetRotation(expectedBone.m_quat, i));
            }
            CHECK(bone.m_trans.m_type == expectedBone.m_trans.m_type);
            CHECK(bone.m_trans.m_indices == expectedBone.m_trans.m_indices);
            for (auto axis = 0u; axis < 3u; ++axis)
            {
                CHECK_THAT(bone.m_trans.m_mins[axis], Catch::Matchers::WithinAbs(expectedBone.m_trans.m_mins[axis], 0.0001f));
                CHECK_THAT(bone.m_trans.m_size[axis], Catch::Matchers::WithinAbs(expectedBone.m_trans.m_size[axis], 0.0001f));
                CHECK(bone.m_trans.m_constant[axis] == expectedBone.m_trans.m_constant[axis]);
            }
            CheckFrames(bone.m_trans.m_frames_u8, expectedBone.m_trans.m_frames_u8);
            CheckFrames(bone.m_trans.m_frames_u16, expectedBone.m_trans.m_frames_u16);
        }
        REQUIRE(static_cast<bool>(actual.m_delta_track) == static_cast<bool>(expected.m_delta_track));
        if (expected.m_delta_track)
        {
            REQUIRE(actual.m_delta_track->m_quat.has_value());
            REQUIRE(actual.m_delta_track->m_trans.has_value());
            const auto& quat = *actual.m_delta_track->m_quat;
            const auto& expectedQuat = *expected.m_delta_track->m_quat;
            CHECK(quat.m_indices == expectedQuat.m_indices);
            REQUIRE(quat.m_frames2.size() == expectedQuat.m_frames2.size());
            for (auto i = 0uz; i < quat.m_frames2.size(); ++i)
                for (auto axis = 0u; axis < 2u; ++axis)
                    CHECK(quat.m_frames2[i].value[axis] == expectedQuat.m_frames2[i].value[axis]);
            const auto& trans = *actual.m_delta_track->m_trans;
            const auto& expectedTrans = *expected.m_delta_track->m_trans;
            CHECK(trans.m_constant == expectedTrans.m_constant);
            CHECK(trans.m_indices == expectedTrans.m_indices);
            for (auto axis = 0u; axis < 3u; ++axis)
            {
                CHECK(trans.m_mins[axis] == expectedTrans.m_mins[axis]);
                CHECK_THAT(trans.m_size[axis], Catch::Matchers::WithinAbs(expectedTrans.m_size[axis], 0.0001f));
            }
            CheckFrames(trans.m_frames_u8, expectedTrans.m_frames_u8);
            CheckFrames(trans.m_frames_u16, expectedTrans.m_frames_u16);
        }
    }

    std::string CreateAnimation(const uint16_t numFrames, const bool looped, const bool constantDelta, const bool smallDelta)
    {
        xanim::CommonXAnimParts source;
        source.m_num_frames = numFrames;
        source.m_looped = looped;
        source.m_frame_rate = 30.0f;
        source.m_asset_type = 1u;
        source.m_notifies.emplace_back("fire", 0.5f);
        source.m_notifies.emplace_back("end", 1.0f);
        for (auto i = 0u; i < 5u; ++i)
        {
            xanim::BoneTrack bone;
            bone.m_name = "tag_" + std::to_string(i);
            bone.m_quat.m_type = static_cast<xanim::QuatType>(i);
            bone.m_trans.m_type = static_cast<xanim::TransType>(5u + i % 4u);
            if (i == 1u)
            {
                bone.m_quat.m_indices = {0u, static_cast<uint16_t>(numFrames / 2u), numFrames};
                bone.m_quat.m_frames2 = {
                    {0,      32767},
                    {16384,  28377},
                    {-16384, 28377}
                };
            }
            else if (i == 2u)
                for (auto frame = 0u; frame <= numFrames; ++frame)
                {
                    bone.m_quat.m_indices.push_back(static_cast<uint16_t>(frame));
                    bone.m_quat.m_frames.emplace_back(8192, -8192, 8192, 29536);
                }
            else if (i == 3u)
                bone.m_quat.m_frames2.emplace_back(-16384, 28377);
            else if (i == 4u)
                bone.m_quat.m_frames.emplace_back(-29536, 8192, 8192, -8192);
            if (i % 4u < 2u)
            {
                bone.m_trans.m_indices = {0u, static_cast<uint16_t>(numFrames / 2u), numFrames};
                bone.m_trans.m_mins = {-10.0f, 20.0f, 30.0f};
                bone.m_trans.m_size = {0.1f, 0.2f, 0.3f};
                if (i % 4u == 0u)
                    bone.m_trans.m_frames_u8 = {
                        {0,  10, 20},
                        {30, 40, 50},
                        {60, 70, 80}
                    };
                else
                    bone.m_trans.m_frames_u16 = {
                        {0,    1000, 2000},
                        {3000, 4000, 5000},
                        {6000, 7000, 8000}
                    };
            }
            else if (i == 2u)
                bone.m_trans.m_constant = {-1.0f, 2.0f, 3.0f};
            source.m_bone_tracks.emplace_back(std::move(bone));
        }
        source.m_delta_track = std::make_unique<xanim::CommonXAnimDeltaTrack>();
        source.m_delta_track->m_quat.emplace();
        source.m_delta_track->m_trans.emplace();
        auto& quat = *source.m_delta_track->m_quat;
        auto& trans = *source.m_delta_track->m_trans;
        if (constantDelta)
        {
            quat.m_frames2.emplace_back(0, 32767);
            trans.m_constant = {1.0f, -2.0f, 3.0f};
        }
        else
        {
            quat.m_indices = trans.m_indices = {0u, static_cast<uint16_t>(numFrames / 2u), numFrames};
            quat.m_frames2 = {
                {0,      32767},
                {16384,  28377},
                {-16384, 28377}
            };
            trans.m_small_trans = smallDelta;
            trans.m_mins = {-1.0f, 2.0f, 3.0f};
            trans.m_size = {0.1f, 0.2f, 0.3f};
            if (smallDelta)
                trans.m_frames_u8 = {
                    {0,  10, 20},
                    {30, 40, 50},
                    {60, 70, 80}
                };
            else
                trans.m_frames_u16 = {
                    {0,    1000, 2000},
                    {3000, 4000, 5000},
                    {6000, 7000, 8000}
                };
        }
        std::ostringstream output(std::ios::binary);
        xanim::WriteCompiledXAnim(output, source, xanim::CompiledXAnimVersion::VERSION_17);
        return output.str();
    }

    TEST_CASE("IW3 Xenon animations share PC disk files across native fastfile round trips", "[iw3xenon][system][xanim]")
    {
        const auto numFrames = GENERATE(uint16_t(24), uint16_t(300));
        const auto looped = GENERATE(false, true);
        const auto constantDelta = GENERATE(false, true);
        const auto smallDelta = GENERATE(false, true);
        CAPTURE(numFrames, looped, constantDelta, smallDelta);
        const auto data = CreateAnimation(numFrames, looped, constantDelta, smallDelta);
        const auto expected = ReadAnimation(data);
        const auto native = LoadDiskAnimation(GameId::IW3Xenon, data);
        const auto* asset = native->m_pools.GetAsset<IW3Xenon::AssetXAnim>("test_anim");
        REQUIRE(asset);
        CHECK(asset->Asset()->boneCount[IW3Xenon::PART_TYPE_PRECISION_QUAT] == 2u);
        CHECK(asset->Asset()->boneCount[IW3Xenon::PART_TYPE_PRECISION_QUAT_NO_SIZE] == 2u);
        CHECK(asset->Asset()->boneCount[IW3Xenon::PART_TYPE_SIMPLE_QUAT] == 0u);
        CHECK(asset->Asset()->boneCount[IW3Xenon::PART_TYPE_NORMAL_QUAT] == 0u);

        const auto path = oat::paths::GetTempDirectory("IW3XenonXAnimRoundTrip") / "animation.ff";
        {
            std::ofstream stream(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(stream, *native));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        const auto dumped = DumpAnimation(**loaded);
        REQUIRE(static_cast<uint8_t>(dumped[0]) == 17u);
        CheckAnimation(*ReadAnimation(dumped), *expected);
        const auto pc = LoadDiskAnimation(GameId::IW3, dumped);
        CHECK(DumpAnimation(*pc) == dumped);
        const auto reread = LoadDiskAnimation(GameId::IW3Xenon, dumped);
        CheckAnimation(*ReadAnimation(DumpAnimation(*reread)), *expected);
    }

    TEST_CASE("IW3 Xenon accepts the existing IW3 PC compiled animation fixture", "[iw3xenon][system][xanim]")
    {
        std::ifstream file(oat::paths::GetTestDirectory() / "SystemTests/Game/IW3/XAnim/test_anim", std::ios::binary);
        REQUIRE(file.is_open());
        const std::string data(std::istreambuf_iterator<char>(file), {});
        const auto native = LoadDiskAnimation(GameId::IW3Xenon, data);
        const auto dumped = DumpAnimation(*native);
        const auto pc = LoadDiskAnimation(GameId::IW3, dumped);
        CHECK(DumpAnimation(*pc) == dumped);
        CheckAnimation(*ReadAnimation(dumped), *ReadAnimation(data));
    }

    TEST_CASE("All Xenon quaternion encodings dump the same bytes as equivalent PC animations", "[iw3xenon][system][xanim]")
    {
        namespace native = IW3Xenon;
        Zone zone("packed_animation", 0, GameId::IW3Xenon, GamePlatform::XBOX);
        native::XAnimParts parts{};
        parts.name = "test_anim";
        parts.numframes = 24;
        parts.framerate = 30.0f;
        parts.frequency = 1.25f;
        for (auto type = native::PART_TYPE_NO_QUAT; type <= native::PART_TYPE_PRECISION_QUAT_NO_SIZE; type = static_cast<native::XAnimPartType>(type + 1))
            parts.boneCount[type] = 1u;
        parts.boneCount[native::PART_TYPE_NO_TRANS] = parts.boneCount[native::PART_TYPE_ALL] = 7u;
        std::vector<native::ScriptString> names;
        xanim::CommonXAnimParts expected;
        expected.m_num_frames = parts.numframes;
        expected.m_frame_rate = parts.framerate;
        const std::array types{xanim::QuatType::NO_QUAT,
                               xanim::QuatType::HALF_QUAT,
                               xanim::QuatType::FULL_QUAT,
                               xanim::QuatType::FULL_QUAT,
                               xanim::QuatType::HALF_QUAT_NO_SIZE,
                               xanim::QuatType::FULL_QUAT_NO_SIZE,
                               xanim::QuatType::FULL_QUAT_NO_SIZE};
        for (auto bone = 0u; bone < types.size(); ++bone)
        {
            xanim::BoneTrack track;
            track.m_name = "tag_" + std::to_string(bone);
            names.push_back(zone.m_script_strings.AddOrGetScriptString(track.m_name));
            track.m_quat.m_type = types[bone];
            if (bone > 0u && bone < 4u)
                track.m_quat.m_indices = {0u, 12u, 24u};
            if (bone == 1u)
                track.m_quat.m_frames2 = {
                    {0,     32767 },
                    {32767, 0     },
                    {0,     -32767}
                };
            else if (bone == 2u)
                track.m_quat.m_frames = {
                    {0,     0,      0, 32767},
                    {32767, 0,      0, 0    },
                    {0,     -32767, 0, 0    }
                };
            else if (bone == 3u)
                track.m_quat.m_frames = {
                    {0,      0, 0,     32767},
                    {0,      0, 32767, 0    },
                    {-32767, 0, 0,     0    }
                };
            else if (bone == 4u)
                track.m_quat.m_frames2.emplace_back(-32767, 0);
            else if (bone == 5u)
                track.m_quat.m_frames.emplace_back(-32767, 0, 0, 0);
            else if (bone == 6u)
                track.m_quat.m_frames.emplace_back(0, 0, 0, 32767);
            expected.m_bone_tracks.emplace_back(std::move(track));
        }
        parts.names = names.data();
        uint8_t bytes[]{0, 12, 24, 0, 12, 24, 0, 12, 24, 0, 1, 2, 3, 4, 5, 6};
        int16_t shorts[]{2, 2, 2, static_cast<int16_t>(0xc000u), 0, 0, 0};
        int ints[]{static_cast<int>(0xe0000000u)};
        int16_t randomShorts[]{0, 0x4000, static_cast<int16_t>(0x8000u), 0, 0, 0, 0x2000, 0, 0, static_cast<int16_t>(0xe000u), 0, 0};
        int randomInts[]{0, 0x60000000, static_cast<int>(0xc0000000u)};
        parts.dataByte = bytes;
        parts.dataByteCount = std::size(bytes);
        parts.dataShort = shorts;
        parts.dataShortCount = std::size(shorts);
        parts.dataInt = ints;
        parts.dataIntCount = std::size(ints);
        parts.randomDataShort = randomShorts;
        parts.randomDataShortCount = std::size(randomShorts);
        parts.randomDataInt = randomInts;
        parts.randomDataIntCount = std::size(randomInts);
        zone.m_pools.AddAsset(native::ASSET_TYPE_XANIMPARTS, parts.name, &parts, {}, {names.begin(), names.end()}, {});

        const auto path = oat::paths::GetTempDirectory("IW3XenonPackedXAnim") / "animation.ff";
        {
            std::ofstream stream(path, std::ios::binary);
            REQUIRE(ZoneWriting::WriteZone(stream, zone));
        }
        auto loaded = ZoneLoading::LoadZone(path.string(), std::nullopt);
        REQUIRE(loaded);
        std::ostringstream expectedStream(std::ios::binary);
        xanim::WriteCompiledXAnim(expectedStream, expected, xanim::CompiledXAnimVersion::VERSION_17);
        const auto pc = LoadDiskAnimation(GameId::IW3, expectedStream.str());
        CHECK(DumpAnimation(**loaded) == DumpAnimation(*pc));
    }
} // namespace

#endif
