#pragma once

#include "Game/NX1/NX1.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace NX1
{
    enum class EndianOperation
    {
        Decode,
        Encode
    };

    // ---- Endian Swap Helpers

    static inline void EndianSwap(uint8_t&) {}

    static inline void EndianSwap(int8_t&) {}

    static inline void EndianSwap(char&) {}

    static inline void EndianSwap(bool&) {}

    static inline void EndianSwap(uint16_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(int16_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(uint32_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(int32_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(uint64_t& v)
    {
        v = endianness::FromBigEndian(v);
    }

    static inline void EndianSwap(float& v)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(v));
    }

    static inline void EndianSwap(const float& v)
    {
        EndianSwap(const_cast<float&>(v));
    }

    template<typename T> static inline void EndianSwap(T*& p)
    {
        EndianSwap(reinterpret_cast<uint32_t&>(p));
    }

    template<typename T>
        requires std::is_enum_v<T>
    static inline void EndianSwap(T& v)
    {
        EndianSwap(reinterpret_cast<std::underlying_type_t<T>&>(v));
    }

    template<typename T, size_t N> static inline void EndianSwap(T (&v)[N])
    {
        for (size_t i = 0; i < N; i++)
            EndianSwap(v[i]);
    }

    template<typename T> static inline T EndianSwapDiscriminator(T& value, const EndianOperation operation)
    {
        if (operation == EndianOperation::Decode)
        {
            EndianSwap(value);
            return value;
        }

        const auto hostValue = value;
        EndianSwap(value);
        return hostValue;
    }

    template<typename T> static inline void EndianSwap32Range(T& value, const size_t begin, const size_t end)
    {
        auto* bytes = reinterpret_cast<uint8_t*>(&value);
        for (size_t offset = begin; offset < end; offset += sizeof(uint32_t))
            EndianSwap(*reinterpret_cast<uint32_t*>(bytes + offset));
    }

    static inline void EndianSwap(NX1::vec2_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(NX1::vec3_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(NX1::vec4_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(NX1::columnInfo_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ConditionalScript& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::editFieldDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::entryInternalData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::EventData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::expressionEntry& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ExpressionString& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ExpressionSupportingData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::itemDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::itemDefData_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::itemDefDataByte_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ItemFloatExpression& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ItemKeyHandler& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::listBoxDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::animParamsDef_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::AudioVolumeSetting& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::menuData_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::menuDef_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::MenuEventHandler& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::MenuEventHandlerSet& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::MenuList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::menuTransition& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::multiDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::newsTickerDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::Operand& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::operandInternalDataUnion& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::PathData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::rectDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::SetLocalVarData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::Statement_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::StaticDvar& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::StaticDvarList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::StringList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::ShellShockParams& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::SurfaceFxEntry& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::SurfaceFxTable& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::textScrollDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::UIFunctionList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::VehicleTrack& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(NX1::windowDef_t& v, [[maybe_unused]] const EndianOperation operation);

    static inline void EndianSwap(NX1::GfxVertexShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cachedPart);
        EndianSwap(v.physicalPart);
        EndianSwap(v.cachedPartSize);
        EndianSwap(v.physicalPartSize);
    }

    static inline void EndianSwap(NX1::GfxPixelShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cachedPart);
        EndianSwap(v.physicalPart);
        EndianSwap(v.cachedPartSize);
        EndianSwap(v.physicalPartSize);
    }

    static inline void EndianSwap(NX1::MaterialVertexShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadDef, operation);
    }

    static inline void EndianSwap(NX1::MaterialPixelShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadDef, operation);
    }

    static inline void EndianSwap(NX1::MaterialVertexShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.prog, operation);
    }

    static inline void EndianSwap(NX1::MaterialPixelShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.prog, operation);
    }

    static inline void EndianSwap(NX1::MaterialVertexDeclaration& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::MaterialArgumentCodeConst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.index);
    }

    static inline void EndianSwap(NX1::MaterialArgumentDef& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::MaterialShaderArgument& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        EndianSwap(v.dest);

        switch (type)
        {
        case NX1::MTL_ARG_LITERAL_VERTEX_CONST:
        case NX1::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(v.u.literalConst);
            break;

        case NX1::MTL_ARG_CODE_VERTEX_CONST:
        case NX1::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(v.u.codeConst, operation);
            break;

        case NX1::MTL_ARG_CODE_PIXEL_SAMPLER:
            EndianSwap(v.u.codeSampler);
            break;

        case NX1::MTL_ARG_MATERIAL_VERTEX_CONST:
        case NX1::MTL_ARG_MATERIAL_PIXEL_CONST:
        case NX1::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
            EndianSwap(v.u.nameHash);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(NX1::MaterialPass& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertexDecl);
        EndianSwap(v.vertexShaderArray);
        EndianSwap(v.vertexShader);
        EndianSwap(v.pixelShader);
        EndianSwap(v.args);
    }

    static inline void EndianSwap(NX1::MaterialTechnique& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.flags);
        EndianSwap(v.passCount);
    }

    static inline void EndianSwapPartial(NX1::MaterialTechnique& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v, operation);
    }

    static inline void EndianSwap(NX1::MaterialTechniqueSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.remappedTechniqueSet);
        EndianSwap(v.techniques);
    }

    static inline void EndianSwap(NX1::GfxDrawSurf& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::GfxStateBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadBits);
    }

    static inline void EndianSwap(NX1::MaterialInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.gameFlags);
        EndianSwap(v.drawSurf, operation);
        EndianSwap(v.surfaceTypeBits);
    }

    static inline void EndianSwap(NX1::MaterialTextureDefInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.image);
    }

    static inline void EndianSwap(NX1::MaterialTextureDefOverride& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::MaterialTextureDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.u, operation);
        EndianSwap(v.override.packed);
    }

    static inline void EndianSwap(NX1::MaterialConstantDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.literal);
    }

    static inline void EndianSwap(NX1::water_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable.floatTime);
        EndianSwap(v.H0X);
        EndianSwap(v.H0Y);
        EndianSwap(v.wTerm);
        EndianSwap(v.M);
        EndianSwap(v.N);
        EndianSwap(v.Lx);
        EndianSwap(v.Lz);
        EndianSwap(v.gravity);
        EndianSwap(v.windvel);
        EndianSwap(v.winddir);
        EndianSwap(v.amplitude);
        EndianSwap(v.codeConstant);
        EndianSwap(v.image);
    }

    static inline void EndianSwap(NX1::Material& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.techniqueSet);
        EndianSwap(v.textureTable);
        EndianSwap(v.constantTable);
        EndianSwap(v.stateBitsTable);
        EndianSwap(v.subMaterials);
    }

    static inline void EndianSwap(NX1::GfxTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        // Xenon streams the runtime D3D texture object bytes inline. There are no
        // zone pointers inside this blob for the loader to byte-swap.
    }

    static inline void EndianSwap(NX1::CardMemory& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.platform);
    }

    static inline void EndianSwap(NX1::GfxImageStreamData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.width);
        EndianSwap(v.height);
        EndianSwap(v.pixelSize);
    }

    static inline void EndianSwap(NX1::GfxImage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.texture, operation);
        EndianSwap(v.format);
        EndianSwap(v.cardMemory, operation);
        EndianSwap(v.width);
        EndianSwap(v.height);
        EndianSwap(v.depth);
        EndianSwap(v.pixels);
        for (auto& stream : v.streams)
            EndianSwap(stream, operation);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(NX1::RawFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.compressedLen);
        EndianSwap(v.len);
        EndianSwap(v.buffer);
    }

    static inline void EndianSwap(NX1::StringTableCell& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
        EndianSwap(v.hash);
    }

    static inline void EndianSwap(NX1::StringTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.columnCount);
        EndianSwap(v.rowCount);
        EndianSwap(v.values);
    }

    static inline void EndianSwap(NX1::Bounds& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.midPoint.v);
        EndianSwap(v.halfSize.v);
    }

    static inline void EndianSwap(NX1::FxIntRange& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(NX1::FxFloatRange& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(NX1::FxSpawnDefLooping& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.intervalMsec);
        EndianSwap(v.count);
    }

    static inline void EndianSwap(NX1::FxSpawnDefOneShot& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count, operation);
    }

    static inline void EndianSwap(NX1::FxSpawnDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.looping, operation);
    }

    static inline void EndianSwap(NX1::FxElemAtlas& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fps);
        EndianSwap(v.entryCount);
    }

    static inline void EndianSwap(NX1::FxElemVec3Range& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(NX1::FxElemVelStateInFrame& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.velocity, operation);
        EndianSwap(v.totalDelta, operation);
    }

    static inline void EndianSwap(NX1::FxElemVelStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.local, operation);
        EndianSwap(v.world, operation);
    }

    static inline void EndianSwap(NX1::FxElemVisualState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.rotationDelta);
        EndianSwap(v.rotationTotal);
        EndianSwap(v.size);
        EndianSwap(v.scale);
    }

    static inline void EndianSwap(NX1::FxElemVisStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base, operation);
        EndianSwap(v.amplitude, operation);
    }

    static inline void EndianSwap(NX1::FxEffectDefRef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(NX1::FxElemVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.anonymous);
    }

    static inline void EndianSwap(NX1::FxElemMarkVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.materials);
    }

    static inline void EndianSwap(NX1::FxElemDefVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.markArray);
    }

    static inline void EndianSwap(NX1::FxTrailVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pos);
        EndianSwap(v.normal);
        EndianSwap(v.texCoord);
    }

    static inline void EndianSwap(NX1::FxTrailDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.scrollTimeMsec);
        EndianSwap(v.repeatDist);
        EndianSwap(v.invSplitDist);
        EndianSwap(v.invSplitArcDist);
        EndianSwap(v.invSplitTime);
        EndianSwap(v.vertCount);
        EndianSwap(v.verts);
        EndianSwap(v.indCount);
        EndianSwap(v.inds);
    }

    static inline void EndianSwap(NX1::FxSparkFountainDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.gravity);
        EndianSwap(v.bounceFrac);
        EndianSwap(v.bounceRand);
        EndianSwap(v.sparkSpacing);
        EndianSwap(v.sparkLength);
        EndianSwap(v.sparkCount);
        EndianSwap(v.loopTime);
        EndianSwap(v.velMin);
        EndianSwap(v.velMax);
        EndianSwap(v.velConeFrac);
        EndianSwap(v.restSpeed);
        EndianSwap(v.boostTime);
        EndianSwap(v.boostFactor);
    }

    static inline void EndianSwap(NX1::FxElemExtendedDefPtr& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.unknownDef);
    }

    static inline void EndianSwap(NX1::FxElemDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.flags);
        EndianSwap(v.spawn, operation);
        EndianSwap(v.spawnRange, operation);
        EndianSwap(v.fadeInRange, operation);
        EndianSwap(v.fadeOutRange, operation);
        EndianSwap(v.spawnFrustumCullRadius);
        EndianSwap(v.spawnDelayMsec, operation);
        EndianSwap(v.lifeSpanMsec, operation);
        for (auto& range : v.spawnOrigin)
            EndianSwap(range, operation);
        EndianSwap(v.spawnOffsetRadius, operation);
        EndianSwap(v.spawnOffsetHeight, operation);
        for (auto& range : v.spawnAngles)
            EndianSwap(range, operation);
        for (auto& range : v.angularVelocity)
            EndianSwap(range, operation);
        EndianSwap(v.initialRotation, operation);
        EndianSwap(v.gravity, operation);
        EndianSwap(v.reflectionFactor, operation);
        EndianSwap(v.atlas, operation);
        EndianSwap(v.runnerPreSeed);
        EndianSwap(v.velSamples);
        EndianSwap(v.visSamples);
        EndianSwap(v.visuals, operation);
        EndianSwap(v.collBounds, operation);
        EndianSwap(v.effectOnImpact, operation);
        EndianSwap(v.effectOnDeath, operation);
        EndianSwap(v.effectEmitted, operation);
        EndianSwap(v.emitDist, operation);
        EndianSwap(v.emitDistVariance, operation);
        EndianSwap(v.extended, operation);
        EndianSwap(v.randomSeed);
        EndianSwap(v.m_facing);
        EndianSwap(v.m_laser_shape);
        EndianSwap(v.m_laser_width);
        EndianSwap(v.m_laser_segment);
        EndianSwap(v.m_laser_texture_animation);
        EndianSwap(v.m_tracer_draw_interval);
        EndianSwap(v.m_tracer_length);
        EndianSwap(v.m_tracer_width);
        EndianSwap(v.m_tracer_speed);
        EndianSwap(v.m_tracer_screw_distance);
        EndianSwap(v.m_tracer_screw_radius);
        EndianSwap(v.m_tracer_screw_radius_expansion_start_a);
        EndianSwap(v.m_tracer_screw_radius_expansion_start_b);
        EndianSwap(v.m_tracer_screw_radius_expansion_overtime_a);
        EndianSwap(v.m_tracer_screw_radius_expansion_overtime_b);
        EndianSwap(v.m_tracer_fadeout_begin_a);
        EndianSwap(v.m_tracer_fadeout_begin_b);
        EndianSwap(v.m_tracer_fadeout_duration_a);
        EndianSwap(v.m_tracer_fadeout_duration_b);
    }

    static inline void EndianSwap(NX1::FxEffectDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.flags);
        EndianSwap(v.totalSize);
        EndianSwap(v.msecLoopingLife);
        EndianSwap(v.elemDefCountLooping);
        EndianSwap(v.elemDefCountOneShot);
        EndianSwap(v.elemDefCountEmission);
        EndianSwap(v.elemDefs);
    }

    static inline void EndianSwap(NX1::cplane_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.normal);
        EndianSwap(v.dist);
    }

    static inline void EndianSwap(NX1::cbrushside_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.materialNum);
    }

    static inline void EndianSwap(NX1::cbrushWrapper_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numsides);
        EndianSwap(v.glassPieceIndex);
        EndianSwap(v.sides);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.axialMaterialNum);
    }

    static inline void EndianSwap(NX1::BrushWrapper& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.brush, operation);
        EndianSwap(v.totalEdgeCount);
        EndianSwap(v.planes);
    }

    static inline void EndianSwap(NX1::sndPhysicsParams& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.soundPitchLow);
        EndianSwap(v.soundPitchHigh);
        EndianSwap(v.soundVolLow);
        EndianSwap(v.soundVolHigh);
        EndianSwap(v.soundImpactRangeLow);
        EndianSwap(v.soundImpactRangeHigh);
    }

    static inline void EndianSwap(NX1::PhysPreset& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type);
        EndianSwap(v.mass);
        EndianSwap(v.bounce);
        EndianSwap(v.friction);
        EndianSwap(v.bulletForceScale);
        EndianSwap(v.explosiveForceScale);
        EndianSwap(v.sndAliasPrefix);
        EndianSwap(v.sndParams, operation);
        EndianSwap(v.piecesSpreadFraction);
        EndianSwap(v.piecesUpwardVelocity);
    }

    static inline void EndianSwap(NX1::PhysMass& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.centerOfMass);
        EndianSwap(v.momentsOfInertia);
        EndianSwap(v.productsOfInertia);
    }

    static inline void EndianSwap(NX1::PhysGeomInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.brushWrapper);
        EndianSwap(v.type);
        EndianSwap(v.orientation);
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(NX1::PhysCollmap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.count);
        EndianSwap(v.geoms);
        EndianSwap(v.mass, operation);
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(NX1::GfxColor& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::PackedTexCoords& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::PackedUnitVec& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::GfxPackedVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz.v);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord, operation);
        EndianSwap(v.normal, operation);
        EndianSwap(v.tangent, operation);
    }

    static inline void EndianSwap(NX1::XSurfaceCollisionAabb& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
    }

    static inline void EndianSwap(NX1::XSurfaceCollisionNode& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabb, operation);
        EndianSwap(v.childBeginIndex);
        EndianSwap(v.childCount);
    }

    static inline void EndianSwap(NX1::XSurfaceCollisionLeaf& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.triangleBeginIndex);
    }

    static inline void EndianSwap(NX1::XSurfaceCollisionTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.trans);
        EndianSwap(v.nodeCount);
        EndianSwap(v.scale);
        EndianSwap(v.nodes);
        EndianSwap(v.invScale);
        EndianSwap(v.leafCount);
        EndianSwap(v.leafs);
        EndianSwap(v.pad);
    }

    static inline void EndianSwap(NX1::XRigidVertList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.boneOffset);
        EndianSwap(v.vertCount);
        EndianSwap(v.triOffset);
        EndianSwap(v.triCount);
        EndianSwap(v.collisionTree);
    }

    static inline void EndianSwap(NX1::XSurfaceVertexInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.vertsBlend);
        EndianSwap(v.tensionData);
    }

    static inline void EndianSwap(NX1::D3DVertexBuffer& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::D3DIndexBuffer& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::XPartBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.m_array);
    }

    static inline void EndianSwap(NX1::XSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.triCount);
        EndianSwap(v.triIndices);
        EndianSwap(v.vertInfo, operation);
        EndianSwap(v.m_flags);
        EndianSwap(v.verts0);
        EndianSwap(v.vb0, operation);
        EndianSwap(v.vertListCount);
        EndianSwap(v.vertList);
        EndianSwap(v.indexBuffer, operation);
        EndianSwap(v.partBits, operation);
    }

    static inline void EndianSwap(NX1::XModelSurfs& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.surfs);
        EndianSwap(v.numsurfs);
        EndianSwap(v.partBits, operation);
    }

    static inline void EndianSwap(NX1::XModelLodInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dist);
        EndianSwap(v.numsurfs);
        EndianSwap(v.surfIndex);
        EndianSwap(v.modelSurfs);
        EndianSwap(v.partBits, operation);
        EndianSwap(v.surfs);
    }

    static inline void EndianSwap(NX1::XModelCollSurf_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.boneIdx);
        EndianSwap(v.contents);
        EndianSwap(v.surfFlags);
    }

    static inline void EndianSwap(NX1::XBoneInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radiusSquared);
    }

    static inline void EndianSwap(NX1::DObjAnimMat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat.v);
        EndianSwap(v.trans.v);
        EndianSwap(v.transWeight);
    }

    static inline void EndianSwap(NX1::XModelQuat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(NX1::XModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.scale);
        EndianSwap(v.noScalePartBits);
        EndianSwap(v.boneNames);
        EndianSwap(v.parentList);
        EndianSwap(v.quats);
        EndianSwap(v.trans);
        EndianSwap(v.partClassification);
        EndianSwap(v.baseMat);
        EndianSwap(v.materialHandles);
        for (auto& lodInfo : v.lodInfo)
            EndianSwap(lodInfo, operation);
        EndianSwap(v.collSurfs);
        EndianSwap(v.numCollSurfs);
        EndianSwap(v.contents);
        EndianSwap(v.boneInfo);
        EndianSwap(v.radius);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.invHighMipRadius);
        EndianSwap(v.memUsage);
        EndianSwap(v.physPreset);
        EndianSwap(v.physCollmap);
    }

    static inline void EndianSwap(NX1::cbrush_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numsides);
        EndianSwap(v.glassPieceIndex);
        EndianSwap(v.sides);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.axialMaterialNum);
    }

    static inline void EndianSwap(NX1::cLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstCollAabbIndex);
        EndianSwap(v.collAabbCount);
        EndianSwap(v.brushContents);
        EndianSwap(v.terrainContents);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.leafBrushNode);
    }

    static inline void EndianSwap(NX1::cLeafBrushNodeLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.brushes);
    }

    static inline void EndianSwap(NX1::cLeafBrushNodeChildren_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dist);
        EndianSwap(v.range);
        EndianSwap(v.childOffset);
    }

    static inline void EndianSwap(NX1::cLeafBrushNodeData_t& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::cLeafBrushNode_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto leafBrushCount = EndianSwapDiscriminator(v.leafBrushCount, operation);
        EndianSwap(v.contents);
        if (leafBrushCount > 0)
            EndianSwap(v.data.leaf, operation);
        else
            EndianSwap(v.data.children, operation);
    }

    static inline void EndianSwap(NX1::ClipMaterial& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.surfaceFlags);
        EndianSwap(v.contents);
    }

    static inline void EndianSwap(NX1::cmodel_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radius);
        EndianSwap(v.leaf, operation);
    }

    static inline void EndianSwap(NX1::cNode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.children);
    }

    static inline void EndianSwap(NX1::clipMap_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.isInUse);
        EndianSwap(v.planeCount);
        EndianSwap(v.planes);
        EndianSwap(v.numStaticModels);
        EndianSwap(v.staticModelList);
        EndianSwap(v.numMaterials);
        EndianSwap(v.materials);
        EndianSwap(v.numBrushSides);
        EndianSwap(v.brushsides);
        EndianSwap(v.numBrushEdges);
        EndianSwap(v.brushEdges);
        EndianSwap(v.numNodes);
        EndianSwap(v.nodes);
        EndianSwap(v.numLeafs);
        EndianSwap(v.leafs);
        EndianSwap(v.leafbrushNodesCount);
        EndianSwap(v.leafbrushNodes);
        EndianSwap(v.numLeafBrushes);
        EndianSwap(v.leafbrushes);
        EndianSwap(v.numLeafSurfaces);
        EndianSwap(v.leafsurfaces);
        EndianSwap(v.vertCount);
        EndianSwap(v.verts);
        EndianSwap(v.triCount);
        EndianSwap(v.triIndices);
        EndianSwap(v.triEdgeIsWalkable);
        EndianSwap(v.borderCount);
        EndianSwap(v.borders);
        EndianSwap(v.partitionCount);
        EndianSwap(v.partitions);
        EndianSwap(v.aabbTreeCount);
        EndianSwap(v.aabbTrees);
        EndianSwap(v.numSubModels);
        EndianSwap(v.cmodels);
        EndianSwap(v.numBrushes);
        EndianSwap(v.brushes);
        EndianSwap(v.brushBounds);
        EndianSwap(v.brushContents);
        EndianSwap(v.mapEnts);
        EndianSwap(v.smodelNodeCount);
        EndianSwap(v.smodelNodes);
        EndianSwap(v.originalDynEntCount);
        EndianSwap(v.dynEntCount);
        EndianSwap(v.dynEntDefList);
        EndianSwap(v.dynEntPoseList);
        EndianSwap(v.dynEntClientList);
        EndianSwap(v.dynEntCollList);
        EndianSwap(v.checksum);
    }

    static inline void EndianSwap(NX1::CollisionAabbTreeIndex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstChildIndex);
    }

    static inline void EndianSwap(NX1::CollisionAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.midPoint);
        EndianSwap(v.materialIndex);
        EndianSwap(v.childCount);
        EndianSwap(v.halfSize);
        EndianSwap(v.u, operation);
    }

    static inline void EndianSwap(NX1::CollisionBorder& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.distEq);
        EndianSwap(v.zBase);
        EndianSwap(v.zSlope);
        EndianSwap(v.start);
        EndianSwap(v.length);
    }

    static inline void EndianSwap(NX1::CollisionPartition& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstTri);
        EndianSwap(v.borders);
    }

    static inline void EndianSwap(NX1::columnInfo_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pos);
        EndianSwap(v.width);
        EndianSwap(v.maxChars);
        EndianSwap(v.alignment);
    }

    static inline void EndianSwap(NX1::ComPrimaryLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.cosHalfFovExpanded);
        EndianSwap(v.rotationLimit);
        EndianSwap(v.translationLimit);
        EndianSwap(v.defName);
    }

    static inline void EndianSwap(NX1::ComWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.isInUse);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.primaryLights);
    }

    static inline void EndianSwap(NX1::ConditionalScript& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventHandlerSet);
        EndianSwap(v.eventExpression);
    }

    static inline void EndianSwap(NX1::cStaticModel_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xmodel);
        EndianSwap(v.origin);
        EndianSwap(v.invScaledAxis);
        EndianSwap(v.absBounds, operation);
    }

    static inline void EndianSwap(NX1::GfxPlacement& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat);
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(NX1::DynEntityClient& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.physObjId);
        EndianSwap(v.flags);
        EndianSwap(v.lightingHandle);
        EndianSwap(v.health);
        EndianSwap(v.hinge);
    }

    static inline void EndianSwap(NX1::DynEntityColl& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.sector);
        EndianSwap(v.nextEntInSector);
        EndianSwap(v.linkMins);
        EndianSwap(v.linkMaxs);
    }

    static inline void EndianSwap(NX1::DynEntityHingeDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.axisOrigin);
        EndianSwap(v.axisDir);
        EndianSwap(v.angleMin);
        EndianSwap(v.angleMax);
        EndianSwap(v.momentOfInertia);
        EndianSwap(v.friction);
    }

    static inline void EndianSwap(NX1::DynEntityDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.type);
        EndianSwap(v.pose, operation);
        EndianSwap(v.xModel);
        EndianSwap(v.brushModel);
        EndianSwap(v.physicsBrushModel);
        EndianSwap(v.destroyFx);
        EndianSwap(v.physPreset);
        EndianSwap(v.health);
        EndianSwap(v.hinge);
        EndianSwap(v.mass, operation);
        EndianSwap(v.contents);
    }

    static inline void EndianSwap(NX1::DynEntityPose& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pose, operation);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(NX1::editFieldDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.minVal);
        EndianSwap(v.maxVal);
        EndianSwap(v.defVal);
        EndianSwap(v.range);
        EndianSwap(v.maxChars);
        EndianSwap(v.maxCharsGotoNext);
        EndianSwap(v.maxPaintChars);
        EndianSwap(v.paintOffset);
    }

    static inline void EndianSwap(NX1::entryInternalData& v, [[maybe_unused]] const EndianOperation operation)
    {
        // This union is swapped by expressionEntry once its discriminator is known.
        (void)v;
    }

    static inline void EndianSwap(NX1::EventData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.unconditionalScript);
    }

    static inline void EndianSwap(NX1::expressionEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        if (type == NX1::EET_OPERAND)
            EndianSwap(v.data.operand, operation);
        else
            EndianSwap(v.data.op);
    }

    static inline void EndianSwap(NX1::ExpressionString& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
    }

    static inline void EndianSwap(NX1::ExpressionSupportingData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uifunctions, operation);
        EndianSwap(v.staticDvarList, operation);
        EndianSwap(v.uiStrings, operation);
    }

    static inline void EndianSwap(NX1::Font_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fontName);
        EndianSwap(v.pixelHeight);
        EndianSwap(v.glyphCount);
        EndianSwap(v.material);
        EndianSwap(v.glowMaterial);
        EndianSwap(v.glyphs);
    }

    static inline void EndianSwap(NX1::FxGlassDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.halfThickness);
        EndianSwap(v.texVecs);
        EndianSwap(v.color, operation);
        EndianSwap(v.material);
        EndianSwap(v.materialShattered);
        EndianSwap(v.physPreset);
        EndianSwap(v.invHighMipRadius);
        EndianSwap(v.shatteredInvHighMipRadius);
    }

    static inline void EndianSwap(NX1::FxSpatialFrame& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat);
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(NX1::FxGlassPiecePlace& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frame, operation);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(NX1::FxGlassPieceState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.texCoordOrigin);
        EndianSwap(v.supportMask);
        EndianSwap(v.initIndex);
        EndianSwap(v.geoDataStart);
        EndianSwap(v.flags);
        EndianSwap(v.areaX2);
    }

    static inline void EndianSwap(NX1::FxGlassPieceDynamics& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fallTime);
        EndianSwap(v.physObjId);
        EndianSwap(v.physJointId);
        EndianSwap(v.vel);
        EndianSwap(v.avel);
    }

    static inline void EndianSwap(NX1::FxGlassVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.x);
        EndianSwap(v.y);
    }

    static inline void EndianSwap(NX1::FxGlassHoleHeader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uniqueVertCount);
    }

    static inline void EndianSwap(NX1::FxGlassCrackHeader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uniqueVertCount);
    }

    static inline void EndianSwap(NX1::FxGlassGeometryData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.anonymous);
    }

    static inline void EndianSwap(NX1::FxGlassInitPieceState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frame, operation);
        EndianSwap(v.radius);
        EndianSwap(v.texCoordOrigin);
        EndianSwap(v.supportMask);
        EndianSwap(v.areaX2);
    }

    static inline void EndianSwap(NX1::FxGlassSystem& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.time);
        EndianSwap(v.prevTime);
        EndianSwap(v.defCount);
        EndianSwap(v.pieceLimit);
        EndianSwap(v.pieceWordCount);
        EndianSwap(v.initPieceCount);
        EndianSwap(v.cellCount);
        EndianSwap(v.activePieceCount);
        EndianSwap(v.firstFreePiece);
        EndianSwap(v.geoDataLimit);
        EndianSwap(v.geoDataCount);
        EndianSwap(v.initGeoDataCount);
        EndianSwap(v.defs);
        EndianSwap(v.piecePlaces);
        EndianSwap(v.pieceStates);
        EndianSwap(v.pieceDynamics);
        EndianSwap(v.geoData);
        EndianSwap(v.isInUse);
        EndianSwap(v.cellBits);
        EndianSwap(v.visData);
        EndianSwap(v.linkOrg);
        EndianSwap(v.halfThickness);
        EndianSwap(v.lightingHandles);
        EndianSwap(v.initPieceStates);
        EndianSwap(v.initGeoData);
        EndianSwap(v.effectChanceAccum);
        EndianSwap(v.lastPieceDeletionTime);
    }

    static inline void EndianSwap(NX1::FxImpactEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nonflesh);
        EndianSwap(v.flesh);
    }

    static inline void EndianSwap(NX1::FxImpactTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.table);
    }

    static inline void EndianSwap(NX1::SurfaceFxEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfaceEffect);
    }

    static inline void EndianSwap(NX1::SurfaceFxTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.table);
    }

    static inline void EndianSwap(NX1::ShellShockParams& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_name);
        EndianSwap(v.screenBlendBlurredFadeTime);
        EndianSwap(v.screenBlendBlurredEffectTime);
        EndianSwap(v.screenBlendFlashWhiteFadeTime);
        EndianSwap(v.screenBlendFlashShotFadeTime);
        EndianSwap(v.screenBlendType);
        EndianSwap(v.viewFadeTime);
        EndianSwap(v.viewKickRate);
        EndianSwap(v.viewKickRadius);
        EndianSwap(v.lookControlAffect);
        EndianSwap(v.lookControlFadeTime);
        EndianSwap(v.lookControlMouseSensitivity);
        EndianSwap(v.lookControlMaxPitchSpeed);
        EndianSwap(v.lookControlMaxYawSpeed);
        EndianSwap(v.movementAffect);
        EndianSwap(v.volumeSettingName);
        EndianSwap(v.soundAffect);
        EndianSwap(v.soundLoop);
        EndianSwap(v.soundEnd);
        EndianSwap(v.soundEndAbort);
        EndianSwap(v.soundFadeInTime);
        EndianSwap(v.soundFadeOutTime);
        EndianSwap(v.soundDrylevel);
        EndianSwap(v.soundWetlevel);
        EndianSwap(v.soundRoomtype);
        EndianSwap(v.soundModEndDelay);
        EndianSwap(v.soundLoopFadeTime);
        EndianSwap(v.soundLoopEndDelay);
    }

    static inline void EndianSwap(NX1::AudioVolumeSetting& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_name);
        EndianSwap(v.soundChannelVolumes);
    }

    static inline void EndianSwap(NX1::RadVerbPreset& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_name);
        EndianSwap(v.earlyTime);
        EndianSwap(v.lateTime);
        EndianSwap(v.earlyGain);
        EndianSwap(v.lateGain);
        EndianSwap(v.lateGainProx);
        EndianSwap(v.returnGain);
        EndianSwap(v.earlyLpf);
        EndianSwap(v.lateLpf);
        EndianSwap(v.inputLpf);
        EndianSwap(v.dampLpf);
        EndianSwap(v.wallReflect);
        EndianSwap(v.dryGain);
        EndianSwap(v.earlySize);
        EndianSwap(v.lateSize);
        EndianSwap(v.diffusion);
        EndianSwap(v.rearLevel);
    }

    static inline void EndianSwap(NX1::FogDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mp_name);
        EndianSwap(v.m_start_dist);
        EndianSwap(v.m_halfway_dist);
        EndianSwap(v.m_red);
        EndianSwap(v.m_green);
        EndianSwap(v.m_blue);
        EndianSwap(v.m_max_opacity);
        EndianSwap(v.m_transition_time);
        EndianSwap(v.m_multiplied_blend);
        EndianSwap(v.m_multiplied_near_red);
        EndianSwap(v.m_multiplied_near_green);
        EndianSwap(v.m_multiplied_near_blue);
        EndianSwap(v.m_multiplied_far_red);
        EndianSwap(v.m_multiplied_far_green);
        EndianSwap(v.m_multiplied_far_blue);
        EndianSwap(v.m_sun_red);
        EndianSwap(v.m_sun_green);
        EndianSwap(v.m_sun_blue);
        EndianSwap(v.m_sun_dir);
        EndianSwap(v.m_sun_begin_fade_angle);
        EndianSwap(v.m_sun_end_fade_angle);
        EndianSwap(v.m_sun_fog_scale);
    }

    static inline void EndianSwap(NX1::FxWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.glassSys, operation);
    }

    static inline void EndianSwap(NX1::G_GlassPiece& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.damageTaken);
        EndianSwap(v.collapseTime);
        EndianSwap(v.lastStateChangeTime);
    }

    static inline void EndianSwap(NX1::G_GlassName& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameStr);
        EndianSwap(v.name);
        EndianSwap(v.pieceCount);
        EndianSwap(v.pieceIndices);
    }

    static inline void EndianSwap(NX1::G_GlassData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.glassPieces);
        EndianSwap(v.pieceCount);
        EndianSwap(v.damageToWeaken);
        EndianSwap(v.damageToDestroy);
        EndianSwap(v.glassNameCount);
        EndianSwap(v.glassNames);
    }

    static inline void EndianSwap(NX1::GameWorldMp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.g_glassData);
    }

    static inline void EndianSwap(NX1::GameWorldSp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.path, operation);
        EndianSwap(v.vehicleTrack, operation);
        EndianSwap(v.g_glassData);
    }

    static inline void EndianSwap(NX1::GfxSky& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.skySurfCount);
        EndianSwap(v.skyStartSurfs);
        EndianSwap(v.skyImage);
    }

    static inline void EndianSwap(NX1::GfxWorldDpvsPlanes& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cellCount);
        EndianSwap(v.planes);
        EndianSwap(v.nodes);
        EndianSwap(v.sceneEntCellBits);
    }

    static inline void EndianSwap(NX1::GfxCellTreeCount& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTreeCount);
    }

    static inline void EndianSwap(NX1::GfxAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.childCount);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
        EndianSwap(v.smodelIndexCount);
        EndianSwap(v.smodelIndexes);
        EndianSwap(v.childrenOffset);
    }

    static inline void EndianSwap(NX1::GfxCellTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTree);
    }

    static inline void EndianSwap(NX1::GfxPortalWritable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hullPoints);
        EndianSwap(v.queuedParent);
    }

    static inline void EndianSwap(NX1::DpvsPlane& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.coeffs);
    }

    static inline void EndianSwap(NX1::GfxPortal& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable, operation);
        EndianSwap(v.plane, operation);
        EndianSwap(v.vertices);
        EndianSwap(v.cellIndex);
        EndianSwap(v.hullAxis);
    }

    static inline void EndianSwap(NX1::GfxCell& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.portalCount);
        EndianSwap(v.portals);
        EndianSwap(v.reflectionProbes);
    }

    static inline void EndianSwap(NX1::GfxReflectionProbe& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(NX1::GfxLightmapArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.primary);
        EndianSwap(v.secondary);
    }

    static inline void EndianSwap(NX1::GfxWorldVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord);
        EndianSwap(v.lmapCoord);
        EndianSwap(v.normal, operation);
        EndianSwap(v.tangent, operation);
    }

    static inline void EndianSwap(NX1::GfxWorldVertexData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertices);
        EndianSwap(v.worldVb, operation);
    }

    static inline void EndianSwap(NX1::GfxWorldVertexLayerData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
        EndianSwap(v.layerVb, operation);
    }

    static inline void EndianSwap(NX1::GfxWorldDraw& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reflectionProbeCount);
        EndianSwap(v.reflectionProbes);
        EndianSwap(v.reflectionProbeOrigins);
        EndianSwap(v.reflectionProbeTextures);
        EndianSwap(v.lightmapCount);
        EndianSwap(v.lightmaps);
        EndianSwap(v.lightmapPrimaryTextures);
        EndianSwap(v.lightmapSecondaryTextures);
        EndianSwap(v.lightmapOverridePrimary);
        EndianSwap(v.lightmapOverrideSecondary);
        EndianSwap(v.mp_light_lut);
        EndianSwap(v.m_num_light_lut);
        EndianSwap(v.m_light_lut_grids_x);
        EndianSwap(v.m_light_lut_grids_y);
        EndianSwap(v.vertexCount);
        EndianSwap(v.vd, operation);
        EndianSwap(v.vertexLayerDataSize);
        EndianSwap(v.vld, operation);
        EndianSwap(v.indexCount);
        EndianSwap(v.indices);
        EndianSwap(v.indexBuffer, operation);
    }

    static inline void EndianSwap(NX1::GfxLightGridEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.colorsIndex);
    }

    static inline void EndianSwap(NX1::GfxLightGridColors& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::GfxLightGridVolumeNode& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reference);
        EndianSwap(v.childMask);
    }

    static inline void EndianSwap(NX1::DiskGfxLightGridSH& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.m_coeff);
    }

    static inline void EndianSwap(NX1::GfxLightGrid& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.lastSunPrimaryLightIndex);
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
        EndianSwap(v.rowAxis);
        EndianSwap(v.colAxis);
        EndianSwap(v.rowDataStart);
        EndianSwap(v.rawRowDataSize);
        EndianSwap(v.rawRowData);
        EndianSwap(v.entryCount);
        EndianSwap(v.entries);
        EndianSwap(v.colorCount);
        EndianSwap(v.colors);
        EndianSwap(v.lightGridMins);
        EndianSwap(v.lightGridMaxs);
        EndianSwap(v.lightBlockMins);
        EndianSwap(v.lightBlockMaxs);
        EndianSwap(v.nodeTableVersion);
        EndianSwap(v.leafTableVersion);
        EndianSwap(v.lightGridVolumeRootNodeCount);
        EndianSwap(v.mp_rootNodeTable);
        EndianSwap(v.lightGridVolumeNodeCount);
        EndianSwap(v.mp_nodeTable);
        EndianSwap(v.lightGridVolumeLeafSize);
        EndianSwap(v.mp_leafTable);
        EndianSwap(v.m_spherical_harmonics_count);
        EndianSwap(v.mp_spherical_harmonics);
    }

    static inline void EndianSwap(NX1::GfxBrushModelWritable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(NX1::GfxBrushModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable, operation);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radius);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
    }

    static inline void EndianSwap(NX1::MaterialMemory& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.material);
        EndianSwap(v.memory);
    }

    static inline void EndianSwap(NX1::sunflare_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.spriteMaterial);
        EndianSwap(v.flareMaterial);
        EndianSwap(v.spriteSize);
        EndianSwap(v.flareMinSize);
        EndianSwap(v.flareMinDot);
        EndianSwap(v.flareMaxSize);
        EndianSwap(v.flareMaxDot);
        EndianSwap(v.flareMaxAlpha);
        EndianSwap(v.flareFadeInTime);
        EndianSwap(v.flareFadeOutTime);
        EndianSwap(v.blindMinDot);
        EndianSwap(v.blindMaxDot);
        EndianSwap(v.blindMaxDarken);
        EndianSwap(v.blindFadeInTime);
        EndianSwap(v.blindFadeOutTime);
        EndianSwap(v.glareMinDot);
        EndianSwap(v.glareMaxDot);
        EndianSwap(v.glareMaxLighten);
        EndianSwap(v.glareFadeInTime);
        EndianSwap(v.glareFadeOutTime);
        EndianSwap(v.sunFxPosition);
    }

    static inline void EndianSwap(NX1::XModelDrawInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfId);
    }

    static inline void EndianSwap(NX1::GfxSceneDynModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(NX1::BModelDrawInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfId);
    }

    static inline void EndianSwap(NX1::GfxSceneDynBrush& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(NX1::GfxShadowGeometry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfaceCount);
        EndianSwap(v.smodelCount);
        EndianSwap(v.sortedSurfIndex);
        EndianSwap(v.smodelIndex);
    }

    static inline void EndianSwap(NX1::GfxLightRegionAxis& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.midPoint);
        EndianSwap(v.halfSize);
    }

    static inline void EndianSwap(NX1::GfxLightRegionHull& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.kdopMidPoint);
        EndianSwap(v.kdopHalfSize);
        EndianSwap(v.axisCount);
        EndianSwap(v.axis);
    }

    static inline void EndianSwap(NX1::GfxLightRegion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hullCount);
        EndianSwap(v.hulls);
    }

    static inline void EndianSwap(NX1::GfxPackedPlacement& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        for (auto& axis : v.axis)
            EndianSwap(axis, operation);
        EndianSwap(v.scale);
    }

    static inline void EndianSwap(NX1::GfxMatrix& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.m);
    }

    static inline void EndianSwap(NX1::SCamoPaletteObjectData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.m_world_transform, operation);
        EndianSwap(v.m_origin);
        EndianSwap(v.m_half_lengths);
        EndianSwap(v.m_hsv_vec);
        EndianSwap(v.m_palette_colors);
        EndianSwap(v.mp_texture);
        EndianSwap(v.m_flags);
        EndianSwap(v.m_intensity);
        EndianSwap(v.pad);
    }

    static inline void EndianSwap(NX1::GfxStaticModelDrawInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.placement, operation);
        EndianSwap(v.model);
        EndianSwap(v.cullDist);
        EndianSwap(v.lightingHandle);
        EndianSwap(v.groundLighting, operation);
        EndianSwap(v.mp_camo_palette_data);
    }

    static inline void EndianSwap(NX1::GfxStaticModelInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.lightingOrigin);
    }

    static inline void EndianSwap(NX1::srfTriangles_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertexLayerData);
        EndianSwap(v.firstVertex);
        EndianSwap(v.vertexCount);
        EndianSwap(v.triCount);
        EndianSwap(v.baseIndex);
    }

    static inline void EndianSwap(NX1::GfxSurfaceLightingAndFlagsFields& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::GfxSurfaceLightingAndFlags& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(NX1::GfxSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.tris, operation);
        EndianSwap(v.material);
        EndianSwap(v.laf, operation);
    }

    static inline void EndianSwap(NX1::GfxSurfaceBounds& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.mipRadius);
    }

    static inline void EndianSwap(NX1::GfxCasterBitsLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.primaryLightIndex);
        EndianSwap(v.casterBitsIndex);
    }

    static inline void EndianSwap(NX1::GfxCasterBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bits);
        EndianSwap(v.sortedBits);
    }

    static inline void EndianSwap(NX1::GfxWorldDpvsStatic& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.smodelCount);
        EndianSwap(v.staticSurfaceCount);
        EndianSwap(v.litOpaqueSurfsBegin);
        EndianSwap(v.litOpaqueSurfsEnd);
        EndianSwap(v.litTransSurfsBegin);
        EndianSwap(v.litTransSurfsEnd);
        EndianSwap(v.shadowCasterSurfsBegin);
        EndianSwap(v.shadowCasterSurfsEnd);
        EndianSwap(v.emissiveSurfsBegin);
        EndianSwap(v.emissiveSurfsEnd);
        EndianSwap(v.smodelVisDataCount);
        EndianSwap(v.surfaceVisDataCount);
        EndianSwap(v.smodelVisData);
        EndianSwap(v.surfaceVisData);
        EndianSwap(v.sortedSurfIndex);
        EndianSwap(v.smodelInsts);
        EndianSwap(v.surfaces);
        EndianSwap(v.surfacesBounds);
        EndianSwap(v.smodelDrawInsts);
        EndianSwap(v.surfaceMaterials);
        EndianSwap(v.surfaceCastsSunShadow);
        EndianSwap(v.usageCount);
        EndianSwap(v.casterBitsLightsCount);
        EndianSwap(v.casterBitsLights);
        EndianSwap(v.casterBitsElemCount);
        EndianSwap(v.casterBitsCount);
        EndianSwap(v.casterBits);
    }

    static inline void EndianSwap(NX1::GfxWorldDpvsDynamic& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dynEntClientWordCount);
        EndianSwap(v.dynEntClientCount);
        EndianSwap(v.dynEntCellBits);
        EndianSwap(v.dynEntVisData);
    }

    static inline void EndianSwap(NX1::GfxHeroOnlyLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.exponent);
    }

    static inline void EndianSwap(NX1::GfxWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.baseName);
        EndianSwap(v.planeCount);
        EndianSwap(v.nodeCount);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.skyCount);
        EndianSwap(v.skies);
        EndianSwap(v.skyBoxModel);
        EndianSwap(v.lastSunPrimaryLightIndex);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.sortKeyLitDecal);
        EndianSwap(v.sortKeyEffectDecal);
        EndianSwap(v.sortKeyEffectAuto);
        EndianSwap(v.sortKeyDistortion);
        EndianSwap(v.dpvsPlanes, operation);
        EndianSwap(v.aabbTreeCounts);
        EndianSwap(v.aabbTrees);
        EndianSwap(v.cells);
        EndianSwap(v.draw, operation);
        EndianSwap(v.lightGrid, operation);
        EndianSwap(v.modelCount);
        EndianSwap(v.models);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.checksum);
        EndianSwap(v.materialMemoryCount);
        EndianSwap(v.materialMemory);
        EndianSwap(v.sun, operation);
        EndianSwap(v.outdoorLookupMatrix);
        EndianSwap(v.outdoorImage);
        EndianSwap(v.cellCasterBits);
        EndianSwap(v.cellHasSunLitSurfsBits);
        EndianSwap(v.sceneDynModel);
        EndianSwap(v.sceneDynBrush);
        EndianSwap(v.primaryLightEntityShadowVis);
        EndianSwap(v.primaryLightDynEntShadowVis);
        EndianSwap(v.nonSunPrimaryLightForModelDynEnt);
        EndianSwap(v.shadowGeom);
        EndianSwap(v.lightRegion);
        EndianSwap(v.dpvs, operation);
        EndianSwap(v.dpvsDyn, operation);
        EndianSwap(v.mapVtxChecksum);
        EndianSwap(v.heroOnlyLightCount);
        EndianSwap(v.heroOnlyLights);
    }

    static inline void EndianSwap(NX1::GfxLightImage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.image);
    }

    static inline void EndianSwap(NX1::GfxLightDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.attenuation, operation);
        EndianSwap(v.lmapLookupStart);
    }

    static inline void EndianSwap(NX1::Glyph& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.letter);
        EndianSwap(v.x0);
        EndianSwap(v.y0);
        EndianSwap(v.dx);
        EndianSwap(v.pixelWidth);
        EndianSwap(v.pixelHeight);
        EndianSwap(v.s0);
        EndianSwap(v.t0);
        EndianSwap(v.s1);
        EndianSwap(v.t1);
    }

    static inline void EndianSwap(NX1::itemDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.window, operation);
        for (auto& textRect : v.textRect)
            EndianSwap(textRect, operation);
        EndianSwap(v.type);
        EndianSwap(v.dataType);
        EndianSwap(v.alignment);
        EndianSwap(v.fontEnum);
        EndianSwap(v.textAlignMode);
        EndianSwap(v.textalignx);
        EndianSwap(v.textaligny);
        EndianSwap(v.textscale);
        EndianSwap(v.textStyle);
        EndianSwap(v.gameMsgWindowIndex);
        EndianSwap(v.gameMsgWindowMode);
        EndianSwap(v.text);
        EndianSwap(v.itemFlags);
        EndianSwap(v.parent);
        EndianSwap(v.mouseEnterText);
        EndianSwap(v.mouseExitText);
        EndianSwap(v.mouseEnter);
        EndianSwap(v.mouseExit);
        EndianSwap(v.action);
        EndianSwap(v.accept);
        EndianSwap(v.onFocus);
        EndianSwap(v.leaveFocus);
        EndianSwap(v.onEnter);
        EndianSwap(v.dvar);
        EndianSwap(v.dvarTest);
        EndianSwap(v.onKey);
        EndianSwap(v.enableDvar);
        EndianSwap(v.localVar);
        EndianSwap(v.dvarFlags);
        EndianSwap(v.focusSound);
        EndianSwap(v.special);
        EndianSwap(v.cursorPos);
        EndianSwap(v.typeData, operation);
        EndianSwap(v.floatExpressionCount);
        EndianSwap(v.floatExpressions);
        EndianSwap(v.visibleExp);
        EndianSwap(v.disabledExp);
        EndianSwap(v.textExp);
        EndianSwap(v.materialExp);
        EndianSwap(v.screenLayer);
        EndianSwap(v.glowColor);
        EndianSwap(v.fxBirthTime);
        EndianSwap(v.fxLetterTime);
        EndianSwap(v.fxDecayStartTime);
        EndianSwap(v.fxDecayDuration);
        EndianSwap(v.lastSoundPlayedTime);
        EndianSwap(v.animStateCount);
        EndianSwap(v.animStates);
        EndianSwap(v.currentAnimState, operation);
        EndianSwap(v.nextAnimState);
        EndianSwap(v.animStartTime);
        EndianSwap(v.animDuration);
    }

    static inline void EndianSwap(NX1::itemDefData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(NX1::itemDefDataByte_t& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::ItemFloatExpression& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.target);
        EndianSwap(v.expression);
    }

    static inline void EndianSwap(NX1::ItemKeyHandler& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.key);
        EndianSwap(v.action);
        EndianSwap(v.next);
    }

    static inline void EndianSwap(NX1::LbColumnDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.id);
        EndianSwap(v.propertyId);
        EndianSwap(v.hidden);
        EndianSwap(v.statName);
        EndianSwap(v.type);
        EndianSwap(v.precision);
        EndianSwap(v.agg);
    }

    static inline void EndianSwap(NX1::LeaderboardDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.id);
        EndianSwap(v.columnCount);
        EndianSwap(v.xpColId);
        EndianSwap(v.prestigeColId);
        EndianSwap(v.columns);
    }

    static inline void EndianSwap(NX1::listBoxDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.startPos);
        EndianSwap(v.endPos);
        EndianSwap(v.drawPadding);
        EndianSwap(v.elementWidth);
        EndianSwap(v.elementHeight);
        EndianSwap(v.elementStyle);
        EndianSwap(v.numColumns);
        for (auto& column : v.columnInfo)
            EndianSwap(column, operation);
        EndianSwap(v.onDoubleClick);
        EndianSwap(v.notselectable);
        EndianSwap(v.noScrollBars);
        EndianSwap(v.usePaging);
        EndianSwap(v.selectBorder);
        EndianSwap(v.selectIcon);
    }

    static inline void EndianSwap(NX1::tWAVEFORMATEX& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.wFormatTag);
        EndianSwap(v.nChannels);
        EndianSwap(v.nSamplesPerSec);
        EndianSwap(v.nAvgBytesPerSec);
        EndianSwap(v.nBlockAlign);
        EndianSwap(v.wBitsPerSample);
        EndianSwap(v.cbSize);
    }

    static inline void EndianSwap(NX1::XMA2WAVEFORMATEX& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.wfx, operation);
        EndianSwap(v.NumStreams);
        EndianSwap(v.ChannelMask);
        EndianSwap(v.SamplesEncoded);
        EndianSwap(v.BytesPerBlock);
        EndianSwap(v.PlayBegin);
        EndianSwap(v.PlayLength);
        EndianSwap(v.LoopBegin);
        EndianSwap(v.LoopLength);
        EndianSwap(v.LoopCount);
        EndianSwap(v.EncoderVersion);
        EndianSwap(v.BlockCount);
    }

    static inline void EndianSwap(NX1::XMA2DataAligned& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_audioData);
        EndianSwap(v.audioBytes);
    }

    static inline void EndianSwap(NX1::XA2SeekTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_seekData);
        EndianSwap(v.seekBytes);
    }

    static inline void EndianSwap(NX1::PlatSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data, operation);
        EndianSwap(v.format, operation);
        EndianSwap(v.seekTable, operation);
    }

    static inline void EndianSwap(NX1::PrimedInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.crc);
        EndianSwap(v.dataSizeLoaded);
        EndianSwap(v.dataSizeStreamed);
        EndianSwap(v.format);
    }

    static inline void EndianSwap(NX1::LocalizeEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(NX1::TriggerModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.contents);
        EndianSwap(v.hullCount);
        EndianSwap(v.firstHull);
    }

    static inline void EndianSwap(NX1::TriggerHull& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.contents);
        EndianSwap(v.slabCount);
        EndianSwap(v.firstSlab);
    }

    static inline void EndianSwap(NX1::TriggerSlab& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.midPoint);
        EndianSwap(v.halfSize);
    }

    static inline void EndianSwap(NX1::MapTriggers& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count);
        EndianSwap(v.models);
        EndianSwap(v.hullCount);
        EndianSwap(v.hulls);
        EndianSwap(v.slabCount);
        EndianSwap(v.slabs);
    }

    static inline void EndianSwap(NX1::Stage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.origin);
        EndianSwap(v.triggerIndex);
    }

    static inline void EndianSwap(NX1::MapEnts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.entityString);
        EndianSwap(v.numEntityChars);
        EndianSwap(v.trigger, operation);
        EndianSwap(v.stages);
    }

    static inline void EndianSwap(NX1::AddonMapEnts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.entityString);
        EndianSwap(v.numEntityChars);
        EndianSwap(v.trigger, operation);
    }

    static inline void EndianSwap(NX1::menuTransition& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.transitionType);
        EndianSwap(v.startTime);
        EndianSwap(v.startVal);
        EndianSwap(v.endVal);
        EndianSwap(v.time);
        EndianSwap(v.endTriggerType);
    }

    static inline void EndianSwap(NX1::menuData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.menuFlags);
        EndianSwap(v.fadeCycle);
        EndianSwap(v.fadeClamp);
        EndianSwap(v.fadeAmount);
        EndianSwap(v.fadeInAmount);
        EndianSwap(v.blurRadius);
        EndianSwap(v.screenLayer);
        EndianSwap(v.onOpen);
        EndianSwap(v.onCloseRequest);
        EndianSwap(v.onClose);
        EndianSwap(v.onESC);
        EndianSwap(v.onKey);
        EndianSwap(v.visibleExp);
        EndianSwap(v.allowedBinding);
        EndianSwap(v.soundName);
        EndianSwap(v.focusColor);
        EndianSwap(v.rectXExp);
        EndianSwap(v.rectYExp);
        EndianSwap(v.rectWExp);
        EndianSwap(v.rectHExp);
        EndianSwap(v.openSoundExp);
        EndianSwap(v.closeSoundExp);
        EndianSwap(v.cursorItem);
        for (auto& transition : v.scaleTransition)
            EndianSwap(transition, operation);
        for (auto& transition : v.alphaTransition)
            EndianSwap(transition, operation);
        for (auto& transition : v.xTransition)
            EndianSwap(transition, operation);
        for (auto& transition : v.yTransition)
            EndianSwap(transition, operation);
        EndianSwap(v.expressionData);
    }

    static inline void EndianSwap(NX1::menuDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
        EndianSwap(v.window, operation);
        EndianSwap(v.itemCount);
        EndianSwap(v.items);
    }

    static inline void EndianSwap(NX1::animParamsDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.rectClient, operation);
        EndianSwap(v.borderSize);
        EndianSwap(v.foreColor);
        EndianSwap(v.backColor);
        EndianSwap(v.borderColor);
        EndianSwap(v.outlineColor);
        EndianSwap(v.textscale);
        EndianSwap(v.glowColor);
        EndianSwap(v.onEnter);
    }

    static inline void EndianSwap(NX1::MenuEventHandler& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventData, operation);
    }

    static inline void EndianSwap(NX1::MenuEventHandlerSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventHandlerCount);
        EndianSwap(v.eventHandlers);
    }

    static inline void EndianSwap(NX1::MenuList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.menuCount);
        EndianSwap(v.menus);
    }

    static inline void EndianSwap(NX1::multiDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dvarList);
        EndianSwap(v.dvarStr);
        EndianSwap(v.dvarValue);
        EndianSwap(v.count);
        EndianSwap(v.strDef);
    }

    static inline void EndianSwap(NX1::newsTickerDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.feedId);
        EndianSwap(v.speed);
        EndianSwap(v.spacing);
        EndianSwap(v.lastTime);
        EndianSwap(v.start);
        EndianSwap(v.end);
        EndianSwap(v.x);
    }

    static inline void EndianSwap(NX1::operandInternalDataUnion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.intVal);
    }

    static inline void EndianSwap(NX1::Operand& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dataType);
        EndianSwap(v.internals, operation);
    }

    static inline void EndianSwap(NX1::pathbasenode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vOrigin);
        EndianSwap(v.type);
    }

    static inline void EndianSwap(NX1::PathData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
        EndianSwap(v.basenodes);
        EndianSwap(v.chainNodeCount);
        EndianSwap(v.chainNodeForNode);
        EndianSwap(v.nodeForChainNode);
        EndianSwap(v.visBytes);
        EndianSwap(v.pathVis);
        EndianSwap(v.nodeTreeCount);
        EndianSwap(v.nodeTree);
    }

    static inline void EndianSwap(NX1::pathlink_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fDist);
        EndianSwap(v.nodeNum);
    }

    static inline void EndianSwap(NX1::pathnode_constant_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.type);
        EndianSwap(v.spawnflags);
        EndianSwap(v.targetname);
        EndianSwap(v.script_linkName);
        EndianSwap(v.script_noteworthy);
        EndianSwap(v.target);
        EndianSwap(v.animscript);
        EndianSwap(v.animscriptfunc);
        EndianSwap(v.vOrigin);
        EndianSwap(v.fAngle);
        EndianSwap(v.forward);
        EndianSwap(v.fRadius);
        EndianSwap(v.minUseDistSq);
        EndianSwap(v.wOverlapNode);
        EndianSwap(v.totalLinkCount);
        EndianSwap(v.Links);
    }

    static inline void EndianSwap(NX1::pathnode_dynamic_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pOwner);
        EndianSwap(v.iFreeTime);
        EndianSwap(v.iValidTime);
        EndianSwap(v.dangerousNodeTime);
        EndianSwap(v.inPlayerLOSTime);
        EndianSwap(v.wLinkCount);
        EndianSwap(v.wOverlapCount);
        EndianSwap(v.turretEntNumber);
    }

    static inline void EndianSwap(NX1::pathnode_transient_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.iSearchFrame);
        EndianSwap(v.pNextOpen);
        EndianSwap(v.pPrevOpen);
        EndianSwap(v.pParent);
        EndianSwap(v.fCost);
        EndianSwap(v.fHeuristic);
        EndianSwap(v.nodeCost);
    }

    static inline void EndianSwap(NX1::pathnode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.constant, operation);
        EndianSwap(v.dynamic, operation);
        EndianSwap(v.transient, operation);
    }

    static inline void EndianSwap(NX1::pathnode_tree_nodes_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
    }

    static inline void EndianSwap(NX1::pathnode_tree_info_t& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::pathnode_tree_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto axis = EndianSwapDiscriminator(v.axis, operation);
        EndianSwap(v.dist);

        if (axis >= 0)
        {
            EndianSwap(v.u.child[0]);
            EndianSwap(v.u.child[1]);
        }
        else
        {
            EndianSwap(v.u.s, operation);
        }
    }

    static inline void EndianSwap(NX1::rectDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.x);
        EndianSwap(v.y);
        EndianSwap(v.w);
        EndianSwap(v.h);
    }

    static inline void EndianSwap(NX1::SetLocalVarData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.localVarName);
        EndianSwap(v.expression);
    }

    static inline void EndianSwap(NX1::SModelAabbNode& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.firstChild);
        EndianSwap(v.childCount);
    }

    static inline void EndianSwap(NX1::StreamFileNameRaw& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(NX1::StreamFileInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.raw, operation);
    }

    static inline void EndianSwap(NX1::StreamFileName& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fileIndex);
        EndianSwap(v.info, operation);
    }

    static inline void EndianSwap(NX1::LoadedSoundStreamFileInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.raw, operation);
    }

    static inline void EndianSwap(NX1::LoadedSoundStreamFileName& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fileIndex);
        EndianSwap(v.info, operation);
    }

    static inline void EndianSwap(NX1::StreamedSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename, operation);
        EndianSwap(v.totalMsec);
        EndianSwap(v.compressed);
    }

    static inline void EndianSwap(NX1::LoadedSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_name);
        EndianSwap(v.sound, operation);
        EndianSwap(v.info, operation);
        EndianSwap(v.streamFilename, operation);
    }

    static inline void EndianSwap(NX1::SoundFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        if (v.type == NX1::SAT_LOADED || v.type == NX1::SAT_VOICED)
        {
            EndianSwap(v.ref.loadSnd);
        }
        else if (v.type == NX1::SAT_STREAMED)
        {
            EndianSwap(v.ref.streamSnd, operation);
        }
    }

    static inline void EndianSwap(NX1::SoundFileRef&, [[maybe_unused]] const EndianOperation operation)
    {
        // SoundFile swaps this union using its type discriminator.
        assert(false);
    }

    static inline void EndianSwap(NX1::SpeakerLevels& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.speaker);
        EndianSwap(v.numLevels);
        EndianSwap(v.levels);
    }

    static inline void EndianSwap(NX1::ChannelMap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.speakerCount);
        for (auto& speaker : v.speakers)
            EndianSwap(speaker, operation);
    }

    static inline void EndianSwap(NX1::SpeakerMap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.isDefault);
        EndianSwap(v.name);
        for (auto& channelMapRow : v.channelMaps)
        {
            for (auto& channelMap : channelMapRow)
                EndianSwap(channelMap, operation);
        }
    }

    static inline void EndianSwap(NX1::SndDuckMap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.duckAmount);
        EndianSwap(v.attack);
        EndianSwap(v.sustain);
        EndianSwap(v.release);
        EndianSwap(v.entChannelMask);
    }

    static inline void EndianSwap(NX1::snd_alias_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aliasName);
        EndianSwap(v.subtitle);
        EndianSwap(v.secondaryAliasName);
        EndianSwap(v.chainAliasName);
        EndianSwap(v.mixerGroup);
        EndianSwap(v.soundFile);
        EndianSwap(v.sequence);
        EndianSwap(v.volMin);
        EndianSwap(v.volMax);
        EndianSwap(v.pitchMin);
        EndianSwap(v.pitchMax);
        EndianSwap(v.distMin);
        EndianSwap(v.distMax);
        EndianSwap(v.velocityMin);
        EndianSwap(v.flags);
        EndianSwap(v.slavePercentage);
        EndianSwap(v.probability);
        EndianSwap(v.lfePercentage);
        EndianSwap(v.centerPercentage);
        EndianSwap(v.startDelay);
        EndianSwap(v.volumeFalloffCurve);
        EndianSwap(v.lpfCurve);
        EndianSwap(v.envelopMin);
        EndianSwap(v.envelopMax);
        EndianSwap(v.envelopPercentage);
        EndianSwap(v.speakerMap);
        EndianSwap(v.adsrSettingIndex);
        EndianSwap(v.instanceLimit);
        EndianSwap(v.reverbSend);
        EndianSwap(v.duckMap);
    }

    static inline void EndianSwap(NX1::snd_alias_list_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aliasName);
        EndianSwap(v.head);
        EndianSwap(v.count);
    }

    static inline void EndianSwap(NX1::SndCurve& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.knotCount);
        EndianSwap(v.knots);
    }

    static inline void EndianSwap(NX1::SndVfCurve& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename);
        EndianSwap(v.sndCurve, operation);
    }

    static inline void EndianSwap(NX1::SndLpfCurve& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename);
        EndianSwap(v.sndCurve, operation);
    }

    // ---- XAnimParts

    static inline void EndianSwap(NX1::XAnimNotifyInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.time);
    }

    static inline void EndianSwap(NX1::XAnimDeltaPart& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.trans);
        EndianSwap(v.quat2);
        EndianSwap(v.quat);
    }

    static inline void EndianSwapPartial(NX1::XAnimPartTrans& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void
        EndianSwapPartial(NX1::XAnimPartTransFrames& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.size);
        EndianSwap(v.frames._1);
    }

    static inline void EndianSwap(NX1::XAnimDynamicFrames& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v._1);
    }

    static inline void
        EndianSwapPartial(NX1::XAnimDeltaPartQuat2& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void
        EndianSwapPartial(NX1::XAnimDeltaPartQuatDataFrames2& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frames);
    }

    static inline void EndianSwap(NX1::XQuat2& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
    }

    static inline void EndianSwapPartial(NX1::XAnimDeltaPartQuat& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void
        EndianSwapPartial(NX1::XAnimDeltaPartQuatDataFrames& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frames);
    }

    static inline void EndianSwap(NX1::XQuat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
    }

    static inline void EndianSwap(NX1::XAnimIndices& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(NX1::XAnimParts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.dataByteCount);
        EndianSwap(v.dataShortCount);
        EndianSwap(v.dataIntCount);
        EndianSwap(v.randomDataByteCount);
        EndianSwap(v.randomDataIntCount);
        EndianSwap(v.numframes);
        EndianSwap(v.randomDataShortCount);
        EndianSwap(v.indexCount);
        EndianSwap(v.framerate);
        EndianSwap(v.frequency);
        EndianSwap(v.names);
        EndianSwap(v.dataByte);
        EndianSwap(v.dataShort);
        EndianSwap(v.dataInt);
        EndianSwap(v.randomDataShort);
        EndianSwap(v.randomDataByte);
        EndianSwap(v.randomDataInt);
        EndianSwap(v.indices.data);
        EndianSwap(v.notify);
        EndianSwap(v.deltaPart);
    }

    static inline void EndianSwap(NX1::snd_alias_list_name& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.soundName);
    }

    static inline void EndianSwap(NX1::SndAliasCustom& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(NX1::WeaponDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap32Range(v, 0, offsetof(NX1::WeaponDef, useRemoteTurretReticle));
        EndianSwap32Range(v, offsetof(NX1::WeaponDef, vStandMove), offsetof(NX1::WeaponDef, originalAiVsAiAccuracyGraphKnotCount));
        EndianSwap(v.originalAiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.originalAiVsPlayerAccuracyGraphKnotCount);
        EndianSwap32Range(v, offsetof(NX1::WeaponDef, iPositionReloadTransTime), offsetof(NX1::WeaponDef, preferManualTarget));
        EndianSwap32Range(v, offsetof(NX1::WeaponDef, aiSpread), offsetof(NX1::WeaponDef, turretBarrelSpinDownAfterEveryShot));
        EndianSwap32Range(v, offsetof(NX1::WeaponDef, turretBarrelSpinMaxSnd), offsetof(NX1::WeaponDef, sharedAmmo));
        EndianSwap(v.airBurstGrenadeHUD);
        EndianSwap(v.dtpInTime);
        EndianSwap(v.dtpLoopTime);
        EndianSwap(v.dtpOutTime);
        EndianSwap(v.xrayConeAngle);
        EndianSwap(v.xrayConeLength);
        EndianSwap(v.maxGibDistance);
    }

    static inline void EndianSwap(NX1::WeaponCompleteDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.szInternalName);
        EndianSwap(v.weapDef);
        EndianSwap(v.szDisplayName);
        EndianSwap(v.hideTags);
        EndianSwap(v.szXAnims);
        EndianSwap(v.fAdsZoomFov);
        EndianSwap(v.iAdsTransInTime);
        EndianSwap(v.iAdsTransOutTime);
        EndianSwap(v.iClipSize);
        EndianSwap(v.impactType);
        EndianSwap(v.iFireTime);
        EndianSwap(v.shotsPerSecondAi);
        EndianSwap(v.dpadIconRatio);
        EndianSwap(v.penetrateMultiplier);
        EndianSwap(v.fAdsViewKickCenterSpeed);
        EndianSwap(v.fHipViewKickCenterSpeed);
        EndianSwap(v.szAltWeaponName);
        EndianSwap(v.altWeaponIndex);
        EndianSwap(v.iAltRaiseTime);
        EndianSwap(v.killIcon);
        EndianSwap(v.dpadIcon);
        EndianSwap(v.fireAnimLength);
        EndianSwap(v.iFirstRaiseTime);
        EndianSwap(v.ammoDropStockMax);
        EndianSwap(v.adsDofStart);
        EndianSwap(v.adsDofEnd);
        EndianSwap(v.aiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnotCount);
        EndianSwap(v.aiVsAiAccuracyGraphKnots);
        EndianSwap(v.aiVsPlayerAccuracyGraphKnots);
    }

    // ---- SndDriverGlobals

    static inline void EndianSwap(NX1::XAUDIO2FX_REVERB_I3DL2_PARAMETERS& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.WetDryMix);
        EndianSwap(v.Room);
        EndianSwap(v.RoomHF);
        EndianSwap(v.RoomRolloffFactor);
        EndianSwap(v.DecayTime);
        EndianSwap(v.DecayHFRatio);
        EndianSwap(v.Reflections);
        EndianSwap(v.ReflectionsDelay);
        EndianSwap(v.Reverb);
        EndianSwap(v.ReverbDelay);
        EndianSwap(v.Diffusion);
        EndianSwap(v.Density);
        EndianSwap(v.HFReference);
    }

    static inline void EndianSwap(NX1::ReverbPreset& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reverbSettings, operation);
    }

    static inline void EndianSwap(NX1::ReverbPresetList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numReverbPresets);
        EndianSwap(v.p_reverbPresets);
        EndianSwap(v.pp_reverbPresetNames);
    }

    static inline void EndianSwap(NX1::SndPlatformGlobals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reverbPresets, operation);
    }

    static inline void EndianSwap(NX1::ADSRSetting& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hashName);
        EndianSwap(v.attackLength);
        EndianSwap(v.decayLength);
        EndianSwap(v.releaseLength);
        EndianSwap(v.sustainLevel);
        EndianSwap(v.sustainLength);
    }

    static inline void EndianSwap(NX1::ADSRSettingList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numSettings);
        EndianSwap(v.p_adsrSettings);
    }

    static inline void EndianSwap(NX1::WeaponAreaMappingEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plrAdsrIndex);
        EndianSwap(v.npcAdsrIndex);
        EndianSwap(v.npcVerbIndex);
    }

    static inline void EndianSwap(NX1::AreaMappingEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reverbWet);
        EndianSwap(v.reverbDry);
        EndianSwap(v.reverbFadeTime);
    }

    static inline void EndianSwap(NX1::WeaponMappingEntry& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::ReverbNameEntry& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(NX1::WeaponAreaMappingList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numWeapons);
        EndianSwap(v.numAreas);
        EndianSwap(v.numReverbNames);
        EndianSwap(v.p_weaponAreaMappings);
        EndianSwap(v.p_areaMappingNames);
        EndianSwap(v.p_weaponMappingNames);
        EndianSwap(v.p_reverbNames);
    }

    static inline void EndianSwap(NX1::SOcclusionTableEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.occlusionValue);
        EndianSwap(v.freq);
    }

    static inline void EndianSwap(NX1::SOcclusionTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hashName);
        EndianSwap(v.numEntries);
        EndianSwap(v.p_entries);
    }

    static inline void EndianSwap(NX1::OcclusionTableList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numOcclusionTables);
        EndianSwap(v.p_occlusionTables);
    }

    static inline void EndianSwap(NX1::snd_entchannel_info_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.priority);
        EndianSwap(v.type3d);
        EndianSwap(v.maxVoices);
        EndianSwap(v.voiceCount);
        EndianSwap(v.volume);
        EndianSwap(v.flags);
    }

    static inline void EndianSwap(NX1::SndEntChannelInfoList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numEntChannels);
        EndianSwap(v.p_entChannelList);
    }

    static inline void EndianSwap(NX1::SndDriverGlobals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.p_name);
        EndianSwap(v.platformGlobals, operation);
        EndianSwap(v.adsrSettingsList, operation);
        EndianSwap(v.weaponAreaMappingList, operation);
        EndianSwap(v.occlusionTableList, operation);
        EndianSwap(v.entChannelInfoList, operation);
    }

    static inline void EndianSwap(NX1::Statement_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numEntries);
        EndianSwap(v.entries);
        EndianSwap(v.supportingData);
        EndianSwap(v.lastExecuteTime);
        EndianSwap(v.lastResult, operation);
    }

    static inline void EndianSwap(NX1::StaticDvar& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dvar);
        EndianSwap(v.dvarName);
    }

    static inline void EndianSwap(NX1::StaticDvarList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numStaticDvars);
        EndianSwap(v.staticDvars);
    }

    static inline void EndianSwap(NX1::StringList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.totalStrings);
        EndianSwap(v.strings);
    }

    static inline void EndianSwap(NX1::StructuredDataEnumEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
        EndianSwap(v.index);
    }

    static inline void EndianSwap(NX1::StructuredDataEnum& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.entryCount);
        EndianSwap(v.reservedEntryCount);
        EndianSwap(v.entries);
    }

    static inline void EndianSwap(NX1::StructuredDataTypeUnion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.stringDataLength);
    }

    static inline void EndianSwap(NX1::StructuredDataType& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);

        switch (type)
        {
        case NX1::DATA_STRING:
        case NX1::DATA_ENUM:
        case NX1::DATA_STRUCT:
        case NX1::DATA_INDEXED_ARRAY:
        case NX1::DATA_ENUM_ARRAY:
            EndianSwap(v.u, operation);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(NX1::StructuredDataStructProperty& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type, operation);
        EndianSwap(v.offset);
    }

    static inline void EndianSwap(NX1::StructuredDataStruct& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.propertyCount);
        EndianSwap(v.properties);
        EndianSwap(v.size);
        EndianSwap(v.bitOffset);
    }

    static inline void EndianSwap(NX1::StructuredDataIndexedArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.arraySize);
        EndianSwap(v.elementType, operation);
        EndianSwap(v.elementSize);
    }

    static inline void EndianSwap(NX1::StructuredDataEnumedArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.enumIndex);
        EndianSwap(v.elementType, operation);
        EndianSwap(v.elementSize);
    }

    static inline void EndianSwap(NX1::StructuredDataDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.version);
        EndianSwap(v.formatChecksum);
        EndianSwap(v.enumCount);
        EndianSwap(v.enums);
        EndianSwap(v.structCount);
        EndianSwap(v.structs);
        EndianSwap(v.indexedArrayCount);
        EndianSwap(v.indexedArrays);
        EndianSwap(v.enumedArrayCount);
        EndianSwap(v.enumedArrays);
        EndianSwap(v.rootType, operation);
        EndianSwap(v.size);
    }

    static inline void EndianSwap(NX1::StructuredDataDefSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.defCount);
        EndianSwap(v.defs);
    }

    static inline void EndianSwap(NX1::textScrollDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.startTime);
    }

    static inline void EndianSwap(NX1::TracerDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.material);
        EndianSwap(v.drawInterval);
        EndianSwap(v.speed);
        EndianSwap(v.beamLength);
        EndianSwap(v.beamWidth);
        EndianSwap(v.screwRadius);
        EndianSwap(v.screwDist);
        EndianSwap(v.m_screwRadius_top_begin_expanding_overtime);
        EndianSwap(v.m_screwRadius_bottom_begin_expanding_overtime);
        EndianSwap(v.m_screwRadius_top_expanding_overtime);
        EndianSwap(v.m_screwRadius_bottom_expanding_overtime);
        EndianSwap(v.m_fadeout_top_begin);
        EndianSwap(v.m_fadeout_top_duration);
        EndianSwap(v.m_fadeout_bottom_begin);
        EndianSwap(v.m_fadeout_bottom_duration);
        EndianSwap(v.colors);
    }

    static inline void EndianSwap(NX1::LaserDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mp_name);
        EndianSwap(v.m_material);
        EndianSwap(v.m_src_width);
        EndianSwap(v.m_segment);
        EndianSwap(v.m_texture_animation);
        EndianSwap(v.m_src_red);
        EndianSwap(v.m_src_green);
        EndianSwap(v.m_src_blue);
        EndianSwap(v.m_src_alpha);
        EndianSwap(v.m_dst_red);
        EndianSwap(v.m_dst_green);
        EndianSwap(v.m_dst_blue);
        EndianSwap(v.m_dst_alpha);
        EndianSwap(v.m_uv);
        EndianSwap(v.m_shape);
    }

    static inline void EndianSwap(NX1::UIFunctionList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.totalFunctions);
        EndianSwap(v.functions);
    }

    static inline void EndianSwap(NX1::VehiclePhysDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.physicsEnabled);
        EndianSwap(v.physPresetName);
        EndianSwap(v.physPreset);
        EndianSwap(v.accelGraphName);
        EndianSwap(v.steeringAxle);
        EndianSwap(v.powerAxle);
        EndianSwap(v.brakingAxle);
        EndianSwap(v.topSpeed);
        EndianSwap(v.reverseSpeed);
        EndianSwap(v.maxVelocity);
        EndianSwap(v.maxPitch);
        EndianSwap(v.maxRoll);
        EndianSwap(v.suspensionTravelFront);
        EndianSwap(v.suspensionTravelRear);
        EndianSwap(v.suspensionStrengthFront);
        EndianSwap(v.suspensionDampingFront);
        EndianSwap(v.suspensionStrengthRear);
        EndianSwap(v.suspensionDampingRear);
        EndianSwap(v.frictionBraking);
        EndianSwap(v.frictionCoasting);
        EndianSwap(v.frictionTopSpeed);
        EndianSwap(v.frictionSide);
        EndianSwap(v.frictionSideRear);
        EndianSwap(v.velocityDependentSlip);
        EndianSwap(v.rollStability);
        EndianSwap(v.rollResistance);
        EndianSwap(v.pitchResistance);
        EndianSwap(v.yawResistance);
        EndianSwap(v.uprightStrengthPitch);
        EndianSwap(v.uprightStrengthRoll);
        EndianSwap(v.targetAirPitch);
        EndianSwap(v.airYawTorque);
        EndianSwap(v.airPitchTorque);
        EndianSwap(v.minimumMomentumForCollision);
        EndianSwap(v.collisionLaunchForceScale);
        EndianSwap(v.wreckedMassScale);
        EndianSwap(v.wreckedBodyFriction);
        EndianSwap(v.minimumJoltForNotify);
        EndianSwap(v.slipThresholdFront);
        EndianSwap(v.slipThresholdRear);
        EndianSwap(v.slipFricScaleFront);
        EndianSwap(v.slipFricScaleRear);
        EndianSwap(v.slipFricRateFront);
        EndianSwap(v.slipFricRateRear);
        EndianSwap(v.slipYawTorque);
    }

    static inline void EndianSwap(NX1::VehicleTrackObstacle& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(NX1::VehicleTrackSector& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.startEdgeDir);
        EndianSwap(v.startEdgeDist);
        EndianSwap(v.leftEdgeDir);
        EndianSwap(v.leftEdgeDist);
        EndianSwap(v.rightEdgeDir);
        EndianSwap(v.rightEdgeDist);
        EndianSwap(v.sectorLength);
        EndianSwap(v.sectorWidth);
        EndianSwap(v.totalPriorLength);
        EndianSwap(v.totalFollowingLength);
        EndianSwap(v.obstacles);
        EndianSwap(v.obstacleCount);
    }

    static inline void EndianSwap(NX1::VehicleTrackSegment& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.targetName);
        EndianSwap(v.sectors);
        EndianSwap(v.sectorCount);
        EndianSwap(v.nextBranches);
        EndianSwap(v.nextBranchesCount);
        EndianSwap(v.prevBranches);
        EndianSwap(v.prevBranchesCount);
        EndianSwap(v.endEdgeDir);
        EndianSwap(v.endEdgeDist);
        EndianSwap(v.totalLength);
    }

    static inline void EndianSwap(NX1::VehicleTrack& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.segments);
        EndianSwap(v.segmentCount);
    }

    static inline void EndianSwap(NX1::VehicleDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type);
        EndianSwap(v.useHintString);
        EndianSwap(v.health);
        EndianSwap(v.quadBarrel);
        EndianSwap(v.texScrollScale);
        EndianSwap(v.topSpeed);
        EndianSwap(v.accel);
        EndianSwap(v.rotRate);
        EndianSwap(v.rotAccel);
        EndianSwap(v.maxBodyPitch);
        EndianSwap(v.maxBodyRoll);
        EndianSwap(v.collisionRadius);
        EndianSwap(v.enableReticleColoring);
        EndianSwap(v.fakeBodyAccelPitch);
        EndianSwap(v.fakeBodyAccelRoll);
        EndianSwap(v.fakeBodyVelPitch);
        EndianSwap(v.fakeBodyVelRoll);
        EndianSwap(v.fakeBodySideVelPitch);
        EndianSwap(v.fakeBodyPitchStrength);
        EndianSwap(v.fakeBodyRollStrength);
        EndianSwap(v.fakeBodyPitchDampening);
        EndianSwap(v.fakeBodyRollDampening);
        EndianSwap(v.fakeBodyBoatRockingAmplitude);
        EndianSwap(v.fakeBodyBoatRockingPeriod);
        EndianSwap(v.fakeBodyBoatRockingRotationPeriod);
        EndianSwap(v.fakeBodyBoatRockingFadeoutSpeed);
        EndianSwap(v.boatBouncingMinForce);
        EndianSwap(v.boatBouncingMaxForce);
        EndianSwap(v.boatBouncingRate);
        EndianSwap(v.boatBouncingFadeinSpeed);
        EndianSwap(v.boatBouncingFadeoutSteeringAngle);
        EndianSwap(v.collisionDamage);
        EndianSwap(v.collisionSpeed);
        EndianSwap(v.killcamOffset);
        EndianSwap(v.playerProtected);
        EndianSwap(v.bulletDamage);
        EndianSwap(v.armorPiercingDamage);
        EndianSwap(v.grenadeDamage);
        EndianSwap(v.projectileDamage);
        EndianSwap(v.projectileSplashDamage);
        EndianSwap(v.heavyExplosiveDamage);
        EndianSwap(v.hasSplitShrouds);
        EndianSwap(v.shroudTravelUp);
        EndianSwap(v.shroudTravelDown);
        EndianSwap(v.vehPhysDef, operation);
        EndianSwap(v.boostDuration);
        EndianSwap(v.boostRechargeTime);
        EndianSwap(v.boostAcceleration);
        EndianSwap(v.suspensionTravel);
        EndianSwap(v.maxSteeringAngle);
        EndianSwap(v.steeringLerp);
        EndianSwap(v.minSteeringScale);
        EndianSwap(v.minSteeringSpeed);
        EndianSwap(v.camLookEnabled);
        EndianSwap(v.camLerp);
        EndianSwap(v.camPitchInfluence);
        EndianSwap(v.camRollInfluence);
        EndianSwap(v.camFovIncrease);
        EndianSwap(v.camFovOffset);
        EndianSwap(v.camFovSpeed);
        EndianSwap(v.turretWeaponName);
        EndianSwap(v.turretWeapon);
        EndianSwap(v.turretHorizSpanLeft);
        EndianSwap(v.turretHorizSpanRight);
        EndianSwap(v.turretVertSpanUp);
        EndianSwap(v.turretVertSpanDown);
        EndianSwap(v.turretRotRate);
        EndianSwap(v.turretSpinSnd, operation);
        EndianSwap(v.turretStopSnd, operation);
        EndianSwap(v.trophyEnabled);
        EndianSwap(v.trophyIsTurret);
        EndianSwap(v.trophyRadius);
        EndianSwap(v.trophyInactiveRadius);
        EndianSwap(v.trophyAmmoCount);
        EndianSwap(v.trophyReloadTime);
        EndianSwap(v.trophyMaximumAngle);
        EndianSwap(v.trophyTags);
        EndianSwap(v.compassFriendlyIcon);
        EndianSwap(v.compassEnemyIcon);
        EndianSwap(v.compassIconWidth);
        EndianSwap(v.compassIconHeight);
        EndianSwap(v.nx_compassFriendlyIcon);
        EndianSwap(v.nx_compassEnemyIcon);
        EndianSwap(v.nx_compassIconWidth);
        EndianSwap(v.nx_compassIconHeight);
        EndianSwap(v.idleLowSnd, operation);
        EndianSwap(v.idleHighSnd, operation);
        EndianSwap(v.engineLowSnd, operation);
        EndianSwap(v.engineHighSnd, operation);
        EndianSwap(v.engineSndSpeed);
        EndianSwap(v.engineStartUpSnd, operation);
        EndianSwap(v.engineStartUpLength);
        EndianSwap(v.engineShutdownSnd, operation);
        EndianSwap(v.engineIdleSnd, operation);
        EndianSwap(v.engineSustainSnd, operation);
        EndianSwap(v.engineRampUpSnd, operation);
        EndianSwap(v.engineRampUpLength);
        EndianSwap(v.engineRampDownSnd, operation);
        EndianSwap(v.engineRampDownLength);
        EndianSwap(v.suspensionSoftSnd, operation);
        EndianSwap(v.suspensionSoftCompression);
        EndianSwap(v.suspensionHardSnd, operation);
        EndianSwap(v.suspensionHardCompression);
        EndianSwap(v.collisionSnd, operation);
        EndianSwap(v.collisionBlendSpeed);
        EndianSwap(v.speedSnd, operation);
        EndianSwap(v.speedSndBlendSpeed);
        EndianSwap(v.surfaceSndPrefix);
        for (auto& surfaceSnd : v.surfaceSnds)
            EndianSwap(surfaceSnd, operation);
        EndianSwap(v.surfaceSndBlendSpeed);
        EndianSwap(v.speedOfSound);
        EndianSwap(v.useDoppler);
        EndianSwap(v.slideVolume);
        EndianSwap(v.slideBlendSpeed);
        EndianSwap(v.inAirPitch);
    }

    static inline void EndianSwap(NX1::windowDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.rect, operation);
        EndianSwap(v.rectClient, operation);
        EndianSwap(v.group);
        EndianSwap(v.style);
        EndianSwap(v.border);
        EndianSwap(v.ownerDraw);
        EndianSwap(v.ownerDrawFlags);
        EndianSwap(v.borderSize);
        EndianSwap(v.staticFlags);
        EndianSwap(v.dynamicFlags);
        EndianSwap(v.nextTime);
        EndianSwap(v.foreColor);
        EndianSwap(v.backColor);
        EndianSwap(v.borderColor);
        EndianSwap(v.outlineColor);
        EndianSwap(v.disableColor);
        EndianSwap(v.background);
    }

} // namespace NX1
