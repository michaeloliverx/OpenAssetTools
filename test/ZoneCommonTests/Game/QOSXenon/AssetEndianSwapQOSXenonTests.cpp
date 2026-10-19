#include "Game/QOS/QOS.h"
#include "Game/QOSXenon/AssetEndianSwapQOSXenon.h"

#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace test::zone::game::qosxenon::asset_endian_swap
{
    using QOSXenon::EndianOperation;

    TEST_CASE("QOS recovered fields retain their retail offsets", "[qos][qosxenon][layout]")
    {
        CHECK(sizeof(QOS::GfxPlacement) == 28);
        CHECK(sizeof(QOSXenon::GfxPlacement) == 28);
        CHECK(sizeof(QOS::DynEntityPhysConstraint) == 72);
        CHECK(sizeof(QOSXenon::DynEntityPhysConstraint) == 72);
        CHECK(offsetof(QOS::DynEntityPhysConstraint, entityIds) == 4);
        CHECK(offsetof(QOSXenon::DynEntityPhysConstraint, entityIds) == 4);
        CHECK(offsetof(QOS::DynEntityPhysConstraint, enabled) == 8);
        CHECK(offsetof(QOSXenon::DynEntityPhysConstraint, enabled) == 8);
        CHECK(offsetof(QOS::DynEntityPhysConstraint, entityCategories) == 10);
        CHECK(offsetof(QOSXenon::DynEntityPhysConstraint, entityCategories) == 10);
        CHECK(sizeof(QOS::SunLightParseParams) == 128);
        CHECK(sizeof(QOSXenon::SunLightParseParams) == 128);
        CHECK(offsetof(QOS::SunLightParseParams, diffuseColorHasBeenSet) == 112);
        CHECK(offsetof(QOSXenon::SunLightParseParams, diffuseColorHasBeenSet) == 112);
        CHECK(offsetof(QOS::SunLightParseParams, angles) == 116);
        CHECK(offsetof(QOSXenon::SunLightParseParams, angles) == 116);

        if constexpr (sizeof(void*) == 4)
        {
            CHECK(sizeof(QOS::DynEntityDef) == 88);
            CHECK(sizeof(QOSXenon::DynEntityDef) == 88);
            CHECK(offsetof(QOS::DynEntityDef, brushModel) == 36);
            CHECK(offsetof(QOSXenon::DynEntityDef, brushModel) == 36);
            CHECK(sizeof(QOS::clipMap_t) == 324);
            CHECK(sizeof(QOSXenon::clipMap_t) == 324);
            CHECK(offsetof(QOS::clipMap_t, dynEntFirstRuntimeModel) == 260);
            CHECK(offsetof(QOSXenon::clipMap_t, dynEntFirstRuntimeModel) == 260);
            CHECK(sizeof(QOS::GfxStaticModelDrawInst) == 64);
            CHECK(sizeof(QOSXenon::GfxStaticModelDrawInst) == 40);
            CHECK(offsetof(QOS::GfxStaticModelDrawInst, model) == 56);
            CHECK(offsetof(QOSXenon::GfxStaticModelDrawInst, model) == 32);
            CHECK(sizeof(QOS::GfxAabbTree) == 48);
            CHECK(sizeof(QOSXenon::GfxAabbTree) == 48);
            CHECK(offsetof(QOS::GfxAabbTree, surfaceCount) == 24);
            CHECK(offsetof(QOSXenon::GfxAabbTree, startSurfIndex) == 28);
            CHECK(sizeof(QOS::GfxWorld) == 728);
            CHECK(sizeof(QOSXenon::GfxWorld) == 828);
            CHECK(offsetof(QOS::GfxWorld, sunParse) == 104);
            CHECK(offsetof(QOSXenon::GfxWorld, sunParse) == 204);
            CHECK(offsetof(QOS::MaterialTechniqueSet, remappedTechniqueSet) == 8);
            CHECK(offsetof(QOS::MaterialTechniqueSet, techniques) == 12);
            CHECK(sizeof(QOS::SoundFile) == 200);
            CHECK(offsetof(QOS::SoundFile, packIndices) == 16);
            CHECK(offsetof(QOS::SoundFile, packLengths) == 76);
            CHECK(offsetof(QOS::SoundFile, packOffsets) == 136);
            CHECK(offsetof(QOS::SoundFile, type) == 196);
            CHECK(sizeof(QOS::AILSOUNDINFO) == 40);
            CHECK(offsetof(QOS::AILSOUNDINFO, channel_mask) == 24);
            CHECK(offsetof(QOS::AILSOUNDINFO, initial_ptr) == 36);
            CHECK(offsetof(QOS::QosMssSound, data) == 40);
        }
    }

    TEST_CASE("QOSXenon dynamic entities decode placement brush indices and spawn base", "[qosxenon][endian][collision]")
    {
        if constexpr (sizeof(void*) == 4)
        {
            unsigned char wire[88]{};
            wire[3] = 1;
            const unsigned char placement[]{
                0x3f,
                0x80,
                0,
                0,
                0x40,
                0,
                0,
                0,
                0x40,
                0x40,
                0,
                0,
                0x40,
                0x80,
                0,
                0,
            };
            std::memcpy(wire + 16, placement, sizeof(placement));
            wire[36] = 0x01;
            wire[37] = 0x23;
            wire[38] = 0x04;
            wire[39] = 0x56;
            wire[52] = 0xab;
            QOSXenon::DynEntityDef entity;
            std::memcpy(&entity, wire, sizeof(entity));
            EndianSwap(entity, EndianOperation::Decode);
            CHECK(entity.type == 1);
            CHECK(entity.pose.quat[3] == 1.0f);
            CHECK(entity.pose.origin[0] == 2.0f);
            CHECK(entity.pose.origin[1] == 3.0f);
            CHECK(entity.pose.origin[2] == 4.0f);
            CHECK(entity.brushModel == 0x123);
            CHECK(entity.physicsBrushModel == 0x456);
            CHECK(entity.unknown2[0] == 0xab);
            EndianSwap(entity, EndianOperation::Encode);
            CHECK(std::memcmp(&entity, wire, sizeof(entity)) == 0);

            unsigned char clipWire[sizeof(QOSXenon::clipMap_t)]{};
            clipWire[259] = 0xab;
            clipWire[260] = 0x01;
            clipWire[261] = 0x23;
            QOSXenon::clipMap_t clip;
            std::memcpy(&clip, clipWire, sizeof(clip));
            EndianSwap(clip, EndianOperation::Decode);
            CHECK(clip.dynEntFirstRuntimeModel == 0x123);
            CHECK(clip.unknown1[71] == 0xab);
            EndianSwap(clip, EndianOperation::Encode);
            CHECK(std::memcmp(&clip, clipWire, sizeof(clip)) == 0);
        }
    }

    TEST_CASE("QOSXenon constraint decoding preserves entity categories", "[qosxenon][endian][collision]")
    {
        const unsigned char wire[72]{0, 0, 0, 2, 0x01, 0x23, 0x03, 0xff, 0, 1, 1, 2, 0x12, 0x34, 0x56, 0x78};
        QOSXenon::DynEntityPhysConstraint constraint;
        std::memcpy(&constraint, wire, sizeof(constraint));
        EndianSwap(constraint, EndianOperation::Decode);
        CHECK(constraint.type == 2);
        CHECK(constraint.entityIds[0] == 0x123);
        CHECK(constraint.entityIds[1] == 1023);
        CHECK(constraint.enabled == 1);
        CHECK(constraint.entityCategories[0] == 1);
        CHECK(constraint.entityCategories[1] == 2);
        CHECK(std::memcmp(constraint.unknown, wire + 12, sizeof(constraint.unknown)) == 0);
        EndianSwap(constraint, EndianOperation::Encode);
        CHECK(std::memcmp(&constraint, wire, sizeof(constraint)) == 0);
    }

    TEST_CASE("QOSXenon sunlight decoding leaves its name flag and padding intact", "[qosxenon][endian][world]")
    {
        unsigned char wire[128]{};
        std::memcpy(wire, "sun", 4);
        const unsigned char scalar[]{0x3f, 0x80, 0, 0};
        std::memcpy(wire + 64, scalar, sizeof(scalar));
        std::memcpy(wire + 116, scalar, sizeof(scalar));
        wire[112] = 1;
        wire[113] = 0x12;
        wire[114] = 0x34;
        wire[115] = 0x56;
        QOSXenon::SunLightParseParams sun;
        std::memcpy(&sun, wire, sizeof(sun));
        EndianSwap(sun, EndianOperation::Decode);
        CHECK(std::strcmp(sun.name, "sun") == 0);
        CHECK(sun.ambientScale == 1.0f);
        CHECK(sun.angles[0] == 1.0f);
        CHECK(sun.diffuseColorHasBeenSet);
        CHECK(std::memcmp(sun.pad, wire + 113, sizeof(sun.pad)) == 0);
        EndianSwap(sun, EndianOperation::Encode);
        CHECK(std::memcmp(&sun, wire, sizeof(sun)) == 0);
    }

    TEST_CASE("QOSXenon static placements decode their packed axes as words", "[qosxenon][endian][world]")
    {
        const unsigned char wire[]{
            0x3f, 0x80, 0,    0,    0x40, 0,    0,    0,    0x40, 0x40, 0,    0,    0x40, 0x80, 0, 0,
            0x12, 0x34, 0x56, 0x78, 0x23, 0x45, 0x67, 0x89, 0x34, 0x56, 0x78, 0x9a, 0x40, 0xa0, 0, 0,
        };
        QOSXenon::GfxStaticModelDrawInst instance{};
        std::memcpy(&instance, wire, sizeof(wire));
        EndianSwap(instance, EndianOperation::Decode);
        CHECK(instance.cullDist == 1.0f);
        CHECK(instance.placement.origin[0] == 2.0f);
        CHECK(instance.placement.origin[1] == 3.0f);
        CHECK(instance.placement.origin[2] == 4.0f);
        CHECK(instance.placement.axis[0] == 0x12345678u);
        CHECK(instance.placement.axis[1] == 0x23456789u);
        CHECK(instance.placement.axis[2] == 0x3456789au);
        CHECK(instance.placement.scale == 5.0f);
        EndianSwap(instance, EndianOperation::Encode);
        CHECK(std::memcmp(&instance, wire, sizeof(wire)) == 0);
    }

    TEST_CASE("QOS collision AABB nodes keep bounds before their child count", "[qos][qosxenon][endian][collision]")
    {
        CHECK(sizeof(QOS::CollisionAabbTree) == 32);
        CHECK(offsetof(QOS::CollisionAabbTree, halfSize) == 12);
        CHECK(offsetof(QOS::CollisionAabbTree, materialIndex) == 24);
        CHECK(offsetof(QOS::CollisionAabbTree, childCount) == 26);
        CHECK(offsetof(QOS::CollisionAabbTree, u) == 28);
        CHECK(sizeof(QOSXenon::CollisionAabbTree) == 32);
        CHECK(offsetof(QOSXenon::CollisionAabbTree, halfSize) == 12);
        CHECK(offsetof(QOSXenon::CollisionAabbTree, materialIndex) == 24);
        CHECK(offsetof(QOSXenon::CollisionAabbTree, childCount) == 26);
        CHECK(offsetof(QOSXenon::CollisionAabbTree, u) == 28);

        const unsigned char wire[]{
            0x3f, 0x80, 0, 0, 0x40, 0, 0, 0, 0x40, 0x40, 0, 0, 0x40, 0x80, 0, 0, 0x40, 0xa0, 0, 0, 0x40, 0xc0, 0, 0, 0x01, 0x23, 0, 2, 0, 0, 0x04, 0x56,
        };
        QOSXenon::CollisionAabbTree node;
        std::memcpy(&node, wire, sizeof(node));
        EndianSwap(node, EndianOperation::Decode);
        CHECK(node.origin[0] == 1.0f);
        CHECK(node.origin[1] == 2.0f);
        CHECK(node.origin[2] == 3.0f);
        CHECK(node.halfSize[0] == 4.0f);
        CHECK(node.halfSize[1] == 5.0f);
        CHECK(node.halfSize[2] == 6.0f);
        CHECK(node.materialIndex == 0x123);
        CHECK(node.childCount == 2);
        CHECK(node.u.firstChildIndex == 0x456);
        EndianSwap(node, EndianOperation::Encode);
        CHECK(std::memcmp(&node, wire, sizeof(node)) == 0);
    }

    TEST_CASE("QOSXenon endian swap: Material techniques swap only their static prefix", "[qosxenon][endian]")
    {
        QOSXenon::MaterialTechnique technique{};
        technique.flags = 0x1234;
        technique.passCount = 0x5678;
        technique.passArray[0].perPrimArgCount = 3;

        EndianSwapPartial(technique, offsetof(QOSXenon::MaterialTechnique, passArray), EndianOperation::Encode);

        REQUIRE(technique.flags == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(technique.passCount == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
        REQUIRE(technique.passArray[0].perPrimArgCount == 3);

        EndianSwapPartial(technique, offsetof(QOSXenon::MaterialTechnique, passArray), EndianOperation::Decode);

        REQUIRE(technique.flags == 0x1234);
        REQUIRE(technique.passCount == 0x5678);
        REQUIRE(technique.passArray[0].perPrimArgCount == 3);
    }

    TEST_CASE("QOSXenon endian swap: Material code constants preserve byte fields", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_CODE_VERTEX_CONST, QOSXenon::MTL_ARG_CODE_PIXEL_CONST);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        argument.dest = 0x1234;
        argument.u.codeConst.index = 0x5678;
        argument.u.codeConst.firstRow = 3;
        argument.u.codeConst.rowCount = 4;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        REQUIRE(argument.dest == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(argument.u.codeConst.index == endianness::ToBigEndian(static_cast<uint16_t>(0x5678)));
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        REQUIRE(argument.dest == 0x1234);
        REQUIRE(argument.u.codeConst.index == 0x5678);
        REQUIRE(argument.u.codeConst.firstRow == 3);
        REQUIRE(argument.u.codeConst.rowCount == 4);
    }

    TEST_CASE("QOSXenon endian swap: Material hashes and samplers use their full word", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_MATERIAL_VERTEX_CONST,
                                   QOSXenon::MTL_ARG_MATERIAL_PIXEL_CONST,
                                   QOSXenon::MTL_ARG_MATERIAL_PIXEL_SAMPLER,
                                   QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            argument.u.codeSampler = 0x12345678u;
        else
            argument.u.nameHash = 0x12345678u;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            REQUIRE(argument.u.codeSampler == endianness::ToBigEndian(0x12345678u));
        else
            REQUIRE(argument.u.nameHash == endianness::ToBigEndian(0x12345678u));

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        if (type == QOSXenon::MTL_ARG_CODE_PIXEL_SAMPLER)
            REQUIRE(argument.u.codeSampler == 0x12345678u);
        else
            REQUIRE(argument.u.nameHash == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: Unknown shader argument types are not inferred from swapped tags", "[qosxenon][endian]")
    {
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = 0x0300;
        argument.dest = 0x1234;
        argument.u.nameHash = 0x12345678u;

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(0x0300)));
        REQUIRE(argument.dest == endianness::ToBigEndian(static_cast<uint16_t>(0x1234)));
        REQUIRE(argument.u.nameHash == 0x12345678u);

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == 0x0300);
        REQUIRE(argument.dest == 0x1234);
        REQUIRE(argument.u.nameHash == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: Integer operands use the requested operation", "[qosxenon][endian]")
    {
        QOSXenon::Operand operand{};
        operand.dataType = QOSXenon::VAL_INT;
        operand.internals.intVal = 0x12345678;

        EndianSwap(operand, EndianOperation::Encode);

        REQUIRE(operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_INT))));
        REQUIRE(operand.internals.intVal == endianness::ToBigEndian(0x12345678));

        EndianSwap(operand, EndianOperation::Decode);

        REQUIRE(operand.dataType == QOSXenon::VAL_INT);
        REQUIRE(operand.internals.intVal == 0x12345678);
    }

    TEST_CASE("QOSXenon endian swap: Expressions forward the operation to their operands", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERAND;
        entry.data.operand.dataType = QOSXenon::VAL_FLOAT;
        entry.data.operand.internals.floatVal = 123.5f;

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERAND)));
        REQUIRE(entry.data.operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_FLOAT))));
        REQUIRE(std::bit_cast<uint32_t>(entry.data.operand.internals.floatVal) == endianness::ToBigEndian(std::bit_cast<uint32_t>(123.5f)));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERAND);
        REQUIRE(entry.data.operand.dataType == QOSXenon::VAL_FLOAT);
        REQUIRE(entry.data.operand.internals.floatVal == 123.5f);
    }

    TEST_CASE("QOSXenon endian swap: Operator expressions swap their enum", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERATOR;
        entry.data.op = QOSXenon::OP_ADD;

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERATOR)));
        REQUIRE(entry.data.op == static_cast<QOSXenon::operationEnum>(endianness::ToBigEndian(static_cast<int>(QOSXenon::OP_ADD))));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERATOR);
        REQUIRE(entry.data.op == QOSXenon::OP_ADD);
    }

#ifdef ARCH_x86
    TEST_CASE("QOSXenon endian swap: Literal arguments swap their pointer", "[qosxenon][endian]")
    {
        const auto type = GENERATE(QOSXenon::MTL_ARG_LITERAL_VERTEX_CONST, QOSXenon::MTL_ARG_LITERAL_PIXEL_CONST);
        QOSXenon::MaterialShaderArgument argument{};
        argument.type = type;
        argument.u.literalConst = reinterpret_cast<float (*)[4]>(0x12345678u);

        EndianSwap(argument, EndianOperation::Encode);

        REQUIRE(argument.type == endianness::ToBigEndian(static_cast<uint16_t>(type)));
        REQUIRE(reinterpret_cast<uintptr_t>(argument.u.literalConst) == endianness::ToBigEndian(0x12345678u));

        EndianSwap(argument, EndianOperation::Decode);

        REQUIRE(argument.type == type);
        REQUIRE(reinterpret_cast<uintptr_t>(argument.u.literalConst) == 0x12345678u);
    }

    TEST_CASE("QOSXenon endian swap: String expressions forward the operation to their pointer", "[qosxenon][endian]")
    {
        QOSXenon::expressionEntry entry{};
        entry.type = QOSXenon::EET_OPERAND;
        entry.data.operand.dataType = QOSXenon::VAL_STRING;
        entry.data.operand.internals.string = reinterpret_cast<const char*>(0x12345678u);

        EndianSwap(entry, EndianOperation::Encode);

        REQUIRE(entry.type == endianness::ToBigEndian(static_cast<int>(QOSXenon::EET_OPERAND)));
        REQUIRE(entry.data.operand.dataType == static_cast<QOSXenon::expDataType>(endianness::ToBigEndian(static_cast<int>(QOSXenon::VAL_STRING))));
        REQUIRE(reinterpret_cast<uintptr_t>(entry.data.operand.internals.string) == endianness::ToBigEndian(0x12345678u));

        EndianSwap(entry, EndianOperation::Decode);

        REQUIRE(entry.type == QOSXenon::EET_OPERAND);
        REQUIRE(entry.data.operand.dataType == QOSXenon::VAL_STRING);
        REQUIRE(reinterpret_cast<uintptr_t>(entry.data.operand.internals.string) == 0x12345678u);
    }
#endif
} // namespace test::zone::game::qosxenon::asset_endian_swap
