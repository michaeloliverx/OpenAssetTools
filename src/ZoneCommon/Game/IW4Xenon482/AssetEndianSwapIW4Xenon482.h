#pragma once

#include "Game/IW4Xenon482/IW4Xenon482.h"
#include "Utils/Endianness.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace IW4Xenon482
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

    static inline void EndianSwap(IW4Xenon482::vec2_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(IW4Xenon482::vec3_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(IW4Xenon482::vec4_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(IW4Xenon482::columnInfo_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::ConditionalScript& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::editFieldDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::entryInternalData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::EventData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::expressionEntry& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::ExpressionString& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::ExpressionSupportingData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::itemDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::itemDefData_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::ItemKeyHandler& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::listBoxDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::menuDef_t& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::MenuEventHandler& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::MenuEventHandlerSet& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::MenuList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::menuTransition& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::multiDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::newsTickerDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::Operand& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::operandInternalDataUnion& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::PathData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::rectDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::SetLocalVarData& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::Statement_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::StaticDvar& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::StaticDvarList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::StringList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::textScrollDef_s& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::UIFunctionList& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::VehicleTrack& v, [[maybe_unused]] const EndianOperation operation);
    static inline void EndianSwap(IW4Xenon482::windowDef_t& v, [[maybe_unused]] const EndianOperation operation);

    static inline void EndianSwap(IW4Xenon482::GfxVertexShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cachedPart);
        EndianSwap(v.physicalPart);
        EndianSwap(v.cachedPartSize);
        EndianSwap(v.physicalPartSize);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPixelShaderLoadDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cachedPart);
        EndianSwap(v.physicalPart);
        EndianSwap(v.cachedPartSize);
        EndianSwap(v.physicalPartSize);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialVertexShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadDef, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialPixelShaderProgram& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadDef, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialVertexShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.prog, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialPixelShader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.prog, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialVertexDeclaration& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::MaterialArgumentCodeConst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.index);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialArgumentDef& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::MaterialShaderArgument& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        EndianSwap(v.dest);

        switch (type)
        {
        case IW4Xenon482::MTL_ARG_LITERAL_VERTEX_CONST:
        case IW4Xenon482::MTL_ARG_LITERAL_PIXEL_CONST:
            EndianSwap(v.u.literalConst);
            break;

        case IW4Xenon482::MTL_ARG_CODE_VERTEX_CONST:
        case IW4Xenon482::MTL_ARG_CODE_PIXEL_CONST:
            EndianSwap(v.u.codeConst, operation);
            break;

        case IW4Xenon482::MTL_ARG_CODE_PIXEL_SAMPLER:
            EndianSwap(v.u.codeSampler);
            break;

        case IW4Xenon482::MTL_ARG_MATERIAL_VERTEX_CONST:
        case IW4Xenon482::MTL_ARG_MATERIAL_PIXEL_CONST:
        case IW4Xenon482::MTL_ARG_MATERIAL_PIXEL_SAMPLER:
            EndianSwap(v.u.nameHash);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(IW4Xenon482::MaterialPass& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertexDecl);
        EndianSwap(v.vertexShaderArray);
        EndianSwap(v.vertexShader);
        EndianSwap(v.pixelShader);
        EndianSwap(v.args);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialTechnique& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.flags);
        EndianSwap(v.passCount);
    }

    static inline void
        EndianSwapPartial(IW4Xenon482::MaterialTechnique& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialTechniqueSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.remappedTechniqueSet);
        EndianSwap(v.techniques);
    }

    static inline void EndianSwap(IW4Xenon482::GfxDrawSurf& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(IW4Xenon482::GfxStateBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.loadBits);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.drawSurf, operation);
        EndianSwap(v.surfaceTypeBits);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialTextureDefInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.image);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialTextureDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.u, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialConstantDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameHash);
        EndianSwap(v.literal);
    }

    static inline void EndianSwap(IW4Xenon482::water_t& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::Material& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.techniqueSet);
        EndianSwap(v.textureTable);
        EndianSwap(v.constantTable);
        EndianSwap(v.stateBitsTable);
        EndianSwap(v.subMaterials);
    }

    static inline void EndianSwap(IW4Xenon482::GfxTexture& v, [[maybe_unused]] const EndianOperation operation)
    {
        // Xenon streams the runtime D3D texture object bytes inline. There are no
        // zone pointers inside this blob for the loader to byte-swap.
    }

    static inline void EndianSwap(IW4Xenon482::CardMemory& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.platform);
    }

    static inline void EndianSwap(IW4Xenon482::GfxImageStreamData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.width);
        EndianSwap(v.height);
        EndianSwap(v.pixelSize);
    }

    static inline void EndianSwap(IW4Xenon482::GfxImage& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::RawFileBuffer& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.buffer);
    }

    static inline void EndianSwap(IW4Xenon482::RawFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.compressedLen);
        EndianSwap(v.len);
        EndianSwap(v.data, operation);
    }

    static inline void EndianSwap(IW4Xenon482::StringTableCell& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
        EndianSwap(v.hash);
    }

    static inline void EndianSwap(IW4Xenon482::StringTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.columnCount);
        EndianSwap(v.rowCount);
        EndianSwap(v.values);
    }

    static inline void EndianSwap(IW4Xenon482::Bounds& v, [[maybe_unused]] const EndianOperation operation = EndianOperation::Decode)
    {
        EndianSwap(v.midPoint.v);
        EndianSwap(v.halfSize.v);
    }

    static inline void EndianSwap(IW4Xenon482::FxIntRange& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(IW4Xenon482::FxFloatRange& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(IW4Xenon482::FxSpawnDefLooping& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.intervalMsec);
        EndianSwap(v.count);
    }

    static inline void EndianSwap(IW4Xenon482::FxSpawnDefOneShot& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count, operation);
    }

    static inline void EndianSwap(IW4Xenon482::FxSpawnDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.looping, operation);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemAtlas& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.entryCount);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVec3Range& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base);
        EndianSwap(v.amplitude);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVelStateInFrame& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.velocity, operation);
        EndianSwap(v.totalDelta, operation);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVelStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.local, operation);
        EndianSwap(v.world, operation);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVisualState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.rotationDelta);
        EndianSwap(v.rotationTotal);
        EndianSwap(v.size);
        EndianSwap(v.scale);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVisStateSample& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.base, operation);
        EndianSwap(v.amplitude, operation);
    }

    static inline void EndianSwap(IW4Xenon482::FxEffectDefRef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.anonymous);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemMarkVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.materials);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemDefVisuals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.markArray);
    }

    static inline void EndianSwap(IW4Xenon482::FxTrailVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pos);
        EndianSwap(v.normal);
        EndianSwap(v.texCoord);
    }

    static inline void EndianSwap(IW4Xenon482::FxTrailDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::FxSparkFountainDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::FxElemExtendedDefPtr& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.unknownDef);
    }

    static inline void EndianSwap(IW4Xenon482::FxElemDef& v, [[maybe_unused]] const EndianOperation operation)
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
    }

    static inline void EndianSwap(IW4Xenon482::FxEffectDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::cplane_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.normal);
        EndianSwap(v.dist);
    }

    static inline void EndianSwap(IW4Xenon482::cbrushside_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.materialNum);
    }

    static inline void EndianSwap(IW4Xenon482::cbrushWrapper_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numsides);
        EndianSwap(v.glassPieceIndex);
        EndianSwap(v.sides);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.axialMaterialNum);
    }

    static inline void EndianSwap(IW4Xenon482::BrushWrapper& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.brush, operation);
        EndianSwap(v.totalEdgeCount);
        EndianSwap(v.planes);
    }

    static inline void EndianSwap(IW4Xenon482::PhysPreset& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type);
        EndianSwap(v.mass);
        EndianSwap(v.bounce);
        EndianSwap(v.friction);
        EndianSwap(v.bulletForceScale);
        EndianSwap(v.explosiveForceScale);
        EndianSwap(v.sndAliasPrefix);
        EndianSwap(v.piecesSpreadFraction);
        EndianSwap(v.piecesUpwardVelocity);
    }

    static inline void EndianSwap(IW4Xenon482::PhysMass& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.centerOfMass);
        EndianSwap(v.momentsOfInertia);
        EndianSwap(v.productsOfInertia);
    }

    static inline void EndianSwap(IW4Xenon482::PhysGeomInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.brushWrapper);
        EndianSwap(v.type);
        EndianSwap(v.orientation);
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(IW4Xenon482::PhysCollmap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.count);
        EndianSwap(v.geoms);
        EndianSwap(v.mass, operation);
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxColor& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(IW4Xenon482::PackedTexCoords& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(IW4Xenon482::PackedUnitVec& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPackedVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz.v);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord, operation);
        EndianSwap(v.normal, operation);
        EndianSwap(v.tangent, operation);
    }

    static inline void EndianSwap(IW4Xenon482::XSurfaceCollisionAabb& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.maxs);
    }

    static inline void EndianSwap(IW4Xenon482::XSurfaceCollisionNode& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabb, operation);
        EndianSwap(v.childBeginIndex);
        EndianSwap(v.childCount);
    }

    static inline void EndianSwap(IW4Xenon482::XSurfaceCollisionLeaf& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.triangleBeginIndex);
    }

    static inline void EndianSwap(IW4Xenon482::XSurfaceCollisionTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.trans);
        EndianSwap(v.scale);
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
        EndianSwap(v.leafCount);
        EndianSwap(v.leafs);
    }

    static inline void EndianSwap(IW4Xenon482::XRigidVertList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.boneOffset);
        EndianSwap(v.vertCount);
        EndianSwap(v.triOffset);
        EndianSwap(v.triCount);
        EndianSwap(v.collisionTree);
    }

    static inline void EndianSwap(IW4Xenon482::XSurfaceVertexInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.vertsBlend);
    }

    static inline void EndianSwap(IW4Xenon482::D3DVertexBuffer& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::D3DIndexBuffer& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::XPartBits& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.m_array);
    }

    static inline void EndianSwap(IW4Xenon482::XSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertCount);
        EndianSwap(v.triCount);
        EndianSwap(v.triIndices);
        EndianSwap(v.vertInfo, operation);
        EndianSwap(v.verts0);
        EndianSwap(v.vb0, operation);
        EndianSwap(v.vertListCount);
        EndianSwap(v.vertList);
        EndianSwap(v.indexBuffer, operation);
        EndianSwap(v.partBits, operation);
    }

    static inline void EndianSwap(IW4Xenon482::XModelSurfs& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.surfs);
        EndianSwap(v.numsurfs);
        EndianSwap(v.partBits, operation);
    }

    static inline void EndianSwap(IW4Xenon482::XModelLodInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dist);
        EndianSwap(v.numsurfs);
        EndianSwap(v.surfIndex);
        EndianSwap(v.modelSurfs);
        EndianSwap(v.partBits, operation);
        EndianSwap(v.surfs);
    }

    static inline void EndianSwap(IW4Xenon482::XModelCollSurf_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.boneIdx);
        EndianSwap(v.contents);
        EndianSwap(v.surfFlags);
    }

    static inline void EndianSwap(IW4Xenon482::XBoneInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radiusSquared);
    }

    static inline void EndianSwap(IW4Xenon482::DObjAnimMat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat.v);
        EndianSwap(v.trans.v);
        EndianSwap(v.transWeight);
    }

    static inline void EndianSwap(IW4Xenon482::XModelQuat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.v);
    }

    static inline void EndianSwap(IW4Xenon482::XModel& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::cbrush_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numsides);
        EndianSwap(v.glassPieceIndex);
        EndianSwap(v.sides);
        EndianSwap(v.baseAdjacentSide);
        EndianSwap(v.axialMaterialNum);
    }

    static inline void EndianSwap(IW4Xenon482::cLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstCollAabbIndex);
        EndianSwap(v.collAabbCount);
        EndianSwap(v.brushContents);
        EndianSwap(v.terrainContents);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.leafBrushNode);
    }

    static inline void EndianSwap(IW4Xenon482::cLeafBrushNodeLeaf_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.brushes);
    }

    static inline void EndianSwap(IW4Xenon482::cLeafBrushNodeChildren_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dist);
        EndianSwap(v.range);
        EndianSwap(v.childOffset);
    }

    static inline void EndianSwap(IW4Xenon482::cLeafBrushNodeData_t& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::cLeafBrushNode_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto leafBrushCount = EndianSwapDiscriminator(v.leafBrushCount, operation);
        EndianSwap(v.contents);
        if (leafBrushCount > 0)
            EndianSwap(v.data.leaf, operation);
        else
            EndianSwap(v.data.children, operation);
    }

    static inline void EndianSwap(IW4Xenon482::ClipMaterial& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.surfaceFlags);
        EndianSwap(v.contents);
    }

    static inline void EndianSwap(IW4Xenon482::cmodel_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radius);
        EndianSwap(v.leaf, operation);
    }

    static inline void EndianSwap(IW4Xenon482::cNode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.plane);
        EndianSwap(v.children);
    }

    static inline void EndianSwap(IW4Xenon482::clipMap_t& v, [[maybe_unused]] const EndianOperation operation)
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
        EndianSwap(v.dynEntCount);
        EndianSwap(v.dynEntDefList);
        EndianSwap(v.dynEntPoseList);
        EndianSwap(v.dynEntClientList);
        EndianSwap(v.dynEntCollList);
        EndianSwap(v.checksum);
    }

    static inline void EndianSwap(IW4Xenon482::CollisionAabbTreeIndex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstChildIndex);
    }

    static inline void EndianSwap(IW4Xenon482::CollisionAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.midPoint);
        EndianSwap(v.materialIndex);
        EndianSwap(v.childCount);
        EndianSwap(v.halfSize);
        EndianSwap(v.u, operation);
    }

    static inline void EndianSwap(IW4Xenon482::CollisionBorder& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.distEq);
        EndianSwap(v.zBase);
        EndianSwap(v.zSlope);
        EndianSwap(v.start);
        EndianSwap(v.length);
    }

    static inline void EndianSwap(IW4Xenon482::CollisionPartition& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.firstTri);
        EndianSwap(v.borders);
    }

    static inline void EndianSwap(IW4Xenon482::columnInfo_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pos);
        EndianSwap(v.width);
        EndianSwap(v.maxChars);
        EndianSwap(v.alignment);
    }

    static inline void EndianSwap(IW4Xenon482::ComPrimaryLight& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::ComWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.isInUse);
        EndianSwap(v.primaryLightCount);
        EndianSwap(v.primaryLights);
    }

    static inline void EndianSwap(IW4Xenon482::ConditionalScript& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventHandlerSet);
        EndianSwap(v.eventExpression);
    }

    static inline void EndianSwap(IW4Xenon482::cStaticModel_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xmodel);
        EndianSwap(v.origin);
        EndianSwap(v.invScaledAxis);
        EndianSwap(v.absBounds, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPlacement& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat);
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(IW4Xenon482::DynEntityClient& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.physObjId);
        EndianSwap(v.flags);
        EndianSwap(v.lightingHandle);
        EndianSwap(v.health);
    }

    static inline void EndianSwap(IW4Xenon482::DynEntityColl& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.sector);
        EndianSwap(v.nextEntInSector);
        EndianSwap(v.linkMins);
        EndianSwap(v.linkMaxs);
    }

    static inline void EndianSwap(IW4Xenon482::DynEntityDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.type);
        EndianSwap(v.pose, operation);
        EndianSwap(v.xModel);
        EndianSwap(v.brushModel);
        EndianSwap(v.physicsBrushModel);
        EndianSwap(v.destroyFx);
        EndianSwap(v.physPreset);
        EndianSwap(v.health);
        EndianSwap(v.mass, operation);
        EndianSwap(v.contents);
    }

    static inline void EndianSwap(IW4Xenon482::DynEntityPose& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pose, operation);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(IW4Xenon482::editFieldDef_s& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::entryInternalData& v, [[maybe_unused]] const EndianOperation operation)
    {
        // This union is swapped by expressionEntry once its discriminator is known.
        (void)v;
    }

    static inline void EndianSwap(IW4Xenon482::EventData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.unconditionalScript);
    }

    static inline void EndianSwap(IW4Xenon482::expressionEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);
        if (type == IW4Xenon482::EET_OPERAND)
            EndianSwap(v.data.operand, operation);
        else
            EndianSwap(v.data.op);
    }

    static inline void EndianSwap(IW4Xenon482::ExpressionString& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
    }

    static inline void EndianSwap(IW4Xenon482::ExpressionSupportingData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uifunctions, operation);
        EndianSwap(v.staticDvarList, operation);
        EndianSwap(v.uiStrings, operation);
    }

    static inline void EndianSwap(IW4Xenon482::Font_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fontName);
        EndianSwap(v.pixelHeight);
        EndianSwap(v.glyphCount);
        EndianSwap(v.material);
        EndianSwap(v.glowMaterial);
        EndianSwap(v.glyphs);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::FxSpatialFrame& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.quat);
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassPiecePlace& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frame, operation);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassPieceState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.texCoordOrigin);
        EndianSwap(v.supportMask);
        EndianSwap(v.initIndex);
        EndianSwap(v.geoDataStart);
        EndianSwap(v.flags);
        EndianSwap(v.areaX2);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassPieceDynamics& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fallTime);
        EndianSwap(v.physObjId);
        EndianSwap(v.physJointId);
        EndianSwap(v.vel);
        EndianSwap(v.avel);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.x);
        EndianSwap(v.y);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassHoleHeader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uniqueVertCount);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassCrackHeader& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.uniqueVertCount);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassGeometryData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.anonymous);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassInitPieceState& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frame, operation);
        EndianSwap(v.radius);
        EndianSwap(v.texCoordOrigin);
        EndianSwap(v.supportMask);
        EndianSwap(v.areaX2);
    }

    static inline void EndianSwap(IW4Xenon482::FxGlassSystem& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::FxImpactEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nonflesh);
        EndianSwap(v.flesh);
    }

    static inline void EndianSwap(IW4Xenon482::FxImpactTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.table);
    }

    static inline void EndianSwap(IW4Xenon482::FxWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.glassSys, operation);
    }

    static inline void EndianSwap(IW4Xenon482::G_GlassPiece& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.damageTaken);
        EndianSwap(v.collapseTime);
        EndianSwap(v.lastStateChangeTime);
    }

    static inline void EndianSwap(IW4Xenon482::G_GlassName& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nameStr);
        EndianSwap(v.name);
        EndianSwap(v.pieceCount);
        EndianSwap(v.pieceIndices);
    }

    static inline void EndianSwap(IW4Xenon482::G_GlassData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.glassPieces);
        EndianSwap(v.pieceCount);
        EndianSwap(v.damageToWeaken);
        EndianSwap(v.damageToDestroy);
        EndianSwap(v.glassNameCount);
        EndianSwap(v.glassNames);
    }

    static inline void EndianSwap(IW4Xenon482::GameWorldMp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.g_glassData);
    }

    static inline void EndianSwap(IW4Xenon482::GameWorldSp& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.path, operation);
        EndianSwap(v.vehicleTrack, operation);
        EndianSwap(v.g_glassData);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSky& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.skySurfCount);
        EndianSwap(v.skyStartSurfs);
        EndianSwap(v.skyImage);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldDpvsPlanes& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.cellCount);
        EndianSwap(v.planes);
        EndianSwap(v.nodes);
        EndianSwap(v.sceneEntCellBits);
    }

    static inline void EndianSwap(IW4Xenon482::GfxCellTreeCount& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTreeCount);
    }

    static inline void EndianSwap(IW4Xenon482::GfxAabbTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.childCount);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
        EndianSwap(v.smodelIndexCount);
        EndianSwap(v.smodelIndexes);
        EndianSwap(v.childrenOffset);
    }

    static inline void EndianSwap(IW4Xenon482::GfxCellTree& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aabbTree);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPortalWritable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hullPoints);
        EndianSwap(v.queuedParent);
    }

    static inline void EndianSwap(IW4Xenon482::DpvsPlane& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.coeffs);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPortal& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable, operation);
        EndianSwap(v.plane, operation);
        EndianSwap(v.vertices);
        EndianSwap(v.cellIndex);
        EndianSwap(v.hullAxis);
    }

    static inline void EndianSwap(IW4Xenon482::GfxCell& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.portalCount);
        EndianSwap(v.portals);
        EndianSwap(v.reflectionProbes);
    }

    static inline void EndianSwap(IW4Xenon482::GfxReflectionProbe& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightmapArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.primary);
        EndianSwap(v.secondary);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldVertex& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.xyz);
        EndianSwap(v.binormalSign);
        EndianSwap(v.color, operation);
        EndianSwap(v.texCoord);
        EndianSwap(v.lmapCoord);
        EndianSwap(v.normal, operation);
        EndianSwap(v.tangent, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldVertexData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertices);
        EndianSwap(v.worldVb, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldVertexLayerData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
        EndianSwap(v.layerVb, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldDraw& v, [[maybe_unused]] const EndianOperation operation)
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
        EndianSwap(v.vertexCount);
        EndianSwap(v.vd, operation);
        EndianSwap(v.vertexLayerDataSize);
        EndianSwap(v.vld, operation);
        EndianSwap(v.indexCount);
        EndianSwap(v.indices);
        EndianSwap(v.indexBuffer, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightGridEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.colorsIndex);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightGridColors& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::GfxLightGrid& v, [[maybe_unused]] const EndianOperation operation)
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
    }

    static inline void EndianSwap(IW4Xenon482::GfxBrushModelWritable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxBrushModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.writable, operation);
        EndianSwap(v.bounds, operation);
        EndianSwap(v.radius);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.startSurfIndex);
    }

    static inline void EndianSwap(IW4Xenon482::MaterialMemory& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.material);
        EndianSwap(v.memory);
    }

    static inline void EndianSwap(IW4Xenon482::sunflare_t& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::XModelDrawInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfId);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSceneDynModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(IW4Xenon482::BModelDrawInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfId);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSceneDynBrush& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.info, operation);
        EndianSwap(v.dynEntId);
    }

    static inline void EndianSwap(IW4Xenon482::GfxShadowGeometry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.surfaceCount);
        EndianSwap(v.smodelCount);
        EndianSwap(v.sortedSurfIndex);
        EndianSwap(v.smodelIndex);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightRegionAxis& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.midPoint);
        EndianSwap(v.halfSize);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightRegionHull& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.kdopMidPoint);
        EndianSwap(v.kdopHalfSize);
        EndianSwap(v.axisCount);
        EndianSwap(v.axis);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightRegion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.hullCount);
        EndianSwap(v.hulls);
    }

    static inline void EndianSwap(IW4Xenon482::GfxPackedPlacement& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        for (auto& axis : v.axis)
            EndianSwap(axis, operation);
        EndianSwap(v.scale);
    }

    static inline void EndianSwap(IW4Xenon482::GfxStaticModelDrawInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.placement, operation);
        EndianSwap(v.model);
        EndianSwap(v.cullDist);
        EndianSwap(v.lightingHandle);
        EndianSwap(v.groundLighting, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxStaticModelInst& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.lightingOrigin);
    }

    static inline void EndianSwap(IW4Xenon482::srfTriangles_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vertexLayerData);
        EndianSwap(v.firstVertex);
        EndianSwap(v.vertexCount);
        EndianSwap(v.triCount);
        EndianSwap(v.baseIndex);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSurfaceLightingAndFlagsFields& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::GfxSurfaceLightingAndFlags& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packed);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSurface& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.tris, operation);
        EndianSwap(v.material);
        EndianSwap(v.laf, operation);
    }

    static inline void EndianSwap(IW4Xenon482::GfxSurfaceBounds& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.mipRadius);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldDpvsStatic& v, [[maybe_unused]] const EndianOperation operation)
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
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorldDpvsDynamic& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dynEntClientWordCount);
        EndianSwap(v.dynEntClientCount);
        EndianSwap(v.dynEntCellBits);
        EndianSwap(v.dynEntVisData);
    }

    static inline void EndianSwap(IW4Xenon482::GfxHeroOnlyLight& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.color);
        EndianSwap(v.dir);
        EndianSwap(v.origin);
        EndianSwap(v.radius);
        EndianSwap(v.cosHalfFovOuter);
        EndianSwap(v.cosHalfFovInner);
        EndianSwap(v.exponent);
    }

    static inline void EndianSwap(IW4Xenon482::GfxWorld& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.baseName);
        EndianSwap(v.planeCount);
        EndianSwap(v.nodeCount);
        EndianSwap(v.surfaceCount);
        EndianSwap(v.skyCount);
        EndianSwap(v.skies);
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

    static inline void EndianSwap(IW4Xenon482::GfxLightImage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.image);
    }

    static inline void EndianSwap(IW4Xenon482::GfxLightDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.attenuation, operation);
        EndianSwap(v.lmapLookupStart);
    }

    static inline void EndianSwap(IW4Xenon482::Glyph& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::itemDef_s& v, [[maybe_unused]] const EndianOperation operation)
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
        v.parent = nullptr;
        EndianSwap(v.mouseEnterText);
        EndianSwap(v.mouseExitText);
        EndianSwap(v.mouseEnter);
        EndianSwap(v.mouseExit);
        EndianSwap(v.action);
        EndianSwap(v.accept);
        EndianSwap(v.onFocus);
        EndianSwap(v.leaveFocus);
        EndianSwap(v.dvar);
        EndianSwap(v.dvarTest);
        EndianSwap(v.onKey);
        EndianSwap(v.enableDvar);
        EndianSwap(v.dvarFlags);
        EndianSwap(v.focusSound);
        EndianSwap(v.special);
        EndianSwap(v.cursorPos);
        EndianSwap(v.typeData, operation);
        EndianSwap(v.imageTrack);
        EndianSwap(v.visibleExp);
        EndianSwap(v.disabledExp);
        EndianSwap(v.textExp);
        EndianSwap(v.materialExp);
        EndianSwap(v.rectXExp);
        EndianSwap(v.rectYExp);
        EndianSwap(v.rectWExp);
        EndianSwap(v.rectHExp);
        EndianSwap(v.forecolorRExp);
        EndianSwap(v.forecolorGExp);
        EndianSwap(v.forecolorBExp);
        EndianSwap(v.forecolorAExp);
        EndianSwap(v.glowColorRExp);
        EndianSwap(v.glowColorGExp);
        EndianSwap(v.glowColorBExp);
        EndianSwap(v.glowColorAExp);
        EndianSwap(v.glowColor);
        EndianSwap(v.fxBirthTime);
        EndianSwap(v.fxLetterTime);
        EndianSwap(v.fxDecayStartTime);
        EndianSwap(v.fxDecayDuration);
        EndianSwap(v.lastSoundPlayedTime);
    }

    static inline void EndianSwap(IW4Xenon482::itemDefData_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(IW4Xenon482::ItemKeyHandler& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.key);
        EndianSwap(v.action);
        EndianSwap(v.next);
    }

    static inline void EndianSwap(IW4Xenon482::LbColumnDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::LeaderboardDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.id);
        EndianSwap(v.columnCount);
        EndianSwap(v.xpColId);
        EndianSwap(v.prestigeColId);
        EndianSwap(v.columns);
    }

    static inline void EndianSwap(IW4Xenon482::listBoxDef_s& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::XAUDIOPACKET_ALIGNED& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.pBuffer);
        EndianSwap(v.BufferSize);
        EndianSwap(v.LoopCount);
        for (auto& loop : v.XMALoop)
        {
            EndianSwap(loop.LoopStart);
            EndianSwap(loop.LoopEnd);
        }
        EndianSwap(v.pContext);
    }

    static inline void EndianSwap(IW4Xenon482::XaSeekTable& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
        EndianSwap(v.data);
    }

    static inline void EndianSwap(IW4Xenon482::XaSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.packet, operation);
        EndianSwap(v.xaIwXmaDataInfo.totalMsec);
        EndianSwap(v.seekTable, operation);
    }

    static inline void EndianSwap(IW4Xenon482::LoadedSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.sound, operation);
    }

    static inline void EndianSwap(IW4Xenon482::LocalizeEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(IW4Xenon482::TriggerModel& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.contents);
        EndianSwap(v.hullCount);
        EndianSwap(v.firstHull);
    }

    static inline void EndianSwap(IW4Xenon482::TriggerHull& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.contents);
        EndianSwap(v.slabCount);
        EndianSwap(v.firstSlab);
    }

    static inline void EndianSwap(IW4Xenon482::TriggerSlab& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.midPoint);
        EndianSwap(v.halfSize);
    }

    static inline void EndianSwap(IW4Xenon482::MapTriggers& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.count);
        EndianSwap(v.models);
        EndianSwap(v.hullCount);
        EndianSwap(v.hulls);
        EndianSwap(v.slabCount);
        EndianSwap(v.slabs);
    }

    static inline void EndianSwap(IW4Xenon482::Stage& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.origin);
        EndianSwap(v.triggerIndex);
    }

    static inline void EndianSwap(IW4Xenon482::MapEnts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.entityString);
        EndianSwap(v.numEntityChars);
        EndianSwap(v.trigger, operation);
        EndianSwap(v.stages);
    }

    static inline void EndianSwap(IW4Xenon482::AddonMapEnts& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.entityString);
        EndianSwap(v.numEntityChars);
        EndianSwap(v.trigger, operation);
    }

    static inline void EndianSwap(IW4Xenon482::menuTransition& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.transitionType);
        EndianSwap(v.targetField);
        EndianSwap(v.startTime);
        EndianSwap(v.startVal);
        EndianSwap(v.endVal);
        EndianSwap(v.time);
        EndianSwap(v.endTriggerType);
    }

    static inline void EndianSwap(IW4Xenon482::menuDef_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.window, operation);
        EndianSwap(v.font);
        EndianSwap(v.fullScreen);
        EndianSwap(v.itemCount);
        EndianSwap(v.fontIndex);
        EndianSwap(v.cursorItem);
        EndianSwap(v.fadeCycle);
        EndianSwap(v.fadeClamp);
        EndianSwap(v.fadeAmount);
        EndianSwap(v.fadeInAmount);
        EndianSwap(v.blurRadius);
        EndianSwap(v.onOpen);
        EndianSwap(v.onCloseRequest);
        EndianSwap(v.onClose);
        EndianSwap(v.onESC);
        EndianSwap(v.onKey);
        EndianSwap(v.visibleExp);
        EndianSwap(v.allowedBinding);
        EndianSwap(v.soundName);
        EndianSwap(v.imageTrack);
        EndianSwap(v.focusColor);
        EndianSwap(v.rectXExp);
        EndianSwap(v.rectYExp);
        EndianSwap(v.openSoundExp);
        EndianSwap(v.closeSoundExp);
        EndianSwap(v.items);
        EndianSwap(v.scaleTransition, operation);
        EndianSwap(v.alphaTransition, operation);
        EndianSwap(v.xTransition, operation);
        EndianSwap(v.yTransition, operation);
        EndianSwap(v.expressionData);
    }

    static inline void EndianSwap(IW4Xenon482::MenuEventHandler& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventData, operation);
    }

    static inline void EndianSwap(IW4Xenon482::MenuEventHandlerSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.eventHandlerCount);
        EndianSwap(v.eventHandlers);
    }

    static inline void EndianSwap(IW4Xenon482::MenuList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.menuCount);
        EndianSwap(v.menus);
    }

    static inline void EndianSwap(IW4Xenon482::multiDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dvarList);
        EndianSwap(v.dvarStr);
        EndianSwap(v.dvarValue);
        EndianSwap(v.count);
        EndianSwap(v.strDef);
    }

    static inline void EndianSwap(IW4Xenon482::newsTickerDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.feedId);
        EndianSwap(v.speed);
        EndianSwap(v.spacing);
        EndianSwap(v.lastTime);
        EndianSwap(v.start);
        EndianSwap(v.end);
        EndianSwap(v.x);
    }

    static inline void EndianSwap(IW4Xenon482::operandInternalDataUnion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.intVal);
    }

    static inline void EndianSwap(IW4Xenon482::Operand& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dataType);
        EndianSwap(v.internals, operation);
    }

    static inline void EndianSwap(IW4Xenon482::pathbasenode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.vOrigin);
        EndianSwap(v.type);
    }

    static inline void EndianSwap(IW4Xenon482::PathData& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::pathlink_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fDist);
        EndianSwap(v.nodeNum);
    }

    static inline void EndianSwap(IW4Xenon482::pathnode_constant_t& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::pathnode_dynamic_t& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::pathnode_transient_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.iSearchFrame);
        EndianSwap(v.pNextOpen);
        EndianSwap(v.pPrevOpen);
        EndianSwap(v.pParent);
        EndianSwap(v.fCost);
        EndianSwap(v.fHeuristic);
        EndianSwap(v.nodeCost);
    }

    static inline void EndianSwap(IW4Xenon482::pathnode_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.constant, operation);
        EndianSwap(v.dynamic, operation);
        EndianSwap(v.transient, operation);
    }

    static inline void EndianSwap(IW4Xenon482::pathnode_tree_nodes_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.nodeCount);
        EndianSwap(v.nodes);
    }

    static inline void EndianSwap(IW4Xenon482::pathnode_tree_info_t& v, [[maybe_unused]] const EndianOperation operation) {}

    static inline void EndianSwap(IW4Xenon482::pathnode_tree_t& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::rectDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.x);
        EndianSwap(v.y);
        EndianSwap(v.w);
        EndianSwap(v.h);
    }

    static inline void EndianSwap(IW4Xenon482::SetLocalVarData& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.localVarName);
        EndianSwap(v.expression);
    }

    static inline void EndianSwap(IW4Xenon482::SModelAabbNode& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.bounds, operation);
        EndianSwap(v.firstChild);
        EndianSwap(v.childCount);
    }

    static inline void EndianSwap(IW4Xenon482::XAUDIOCHANNELMAPENTRY& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.Volume);
    }

    static inline void EndianSwap(IW4Xenon482::XAUDIOCHANNELMAP& v, [[maybe_unused]] const EndianOperation operation)
    {
        (void)v;
    }

    static inline void EndianSwap(IW4Xenon482::StreamFileNameRaw& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.dir);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(IW4Xenon482::StreamFileInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.raw, operation);
    }

    static inline void EndianSwap(IW4Xenon482::StreamFileName& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.fileIndex);
        EndianSwap(v.info, operation);
    }

    static inline void EndianSwap(IW4Xenon482::StreamedSound& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename, operation);
    }

    static inline void EndianSwap(IW4Xenon482::SoundFile& v, [[maybe_unused]] const EndianOperation operation)
    {
        if (v.type == IW4Xenon482::SAT_LOADED)
        {
            EndianSwap(v.u.loadSnd);
        }
        else
        {
            EndianSwap(v.u.streamSnd, operation);
        }
    }

    static inline void EndianSwap(IW4Xenon482::SoundFileRef&, [[maybe_unused]] const EndianOperation operation)
    {
        // SoundFile swaps this union using its type discriminator.
        assert(false);
    }

    static inline void EndianSwap(IW4Xenon482::SpeakerMap& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        for (auto& channelMapRow : v.channelMaps)
        {
            for (auto& channelMap : channelMapRow)
                EndianSwap(channelMap.paEntries);
        }
    }

    static inline void EndianSwap(IW4Xenon482::snd_alias_t& v, [[maybe_unused]] const EndianOperation operation)
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
        EndianSwap(v.envelopMin);
        EndianSwap(v.envelopMax);
        EndianSwap(v.envelopPercentage);
        EndianSwap(v.speakerMap);
    }

    static inline void EndianSwap(IW4Xenon482::snd_alias_list_t& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.aliasName);
        EndianSwap(v.head);
        EndianSwap(v.count);
    }

    static inline void EndianSwap(IW4Xenon482::SndCurve& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.filename);
        EndianSwap(v.knotCount);
        EndianSwap(v.knots);
    }

    // ---- XAnimParts

    static inline void EndianSwap(IW4Xenon482::XAnimNotifyInfo& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.time);
    }

    static inline void EndianSwap(IW4Xenon482::XAnimDeltaPart& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.trans);
        EndianSwap(v.quat2);
        EndianSwap(v.quat);
    }

    static inline void
        EndianSwapPartial(IW4Xenon482::XAnimPartTrans& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void
        EndianSwapPartial(IW4Xenon482::XAnimPartTransFrames& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.mins);
        EndianSwap(v.size);
        EndianSwap(v.frames._1);
    }

    static inline void EndianSwap(IW4Xenon482::XAnimDynamicFrames& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v._1);
    }

    static inline void
        EndianSwapPartial(IW4Xenon482::XAnimDeltaPartQuat2& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void EndianSwapPartial(IW4Xenon482::XAnimDeltaPartQuatDataFrames2& v,
                                         [[maybe_unused]] const size_t loadedSize,
                                         [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frames);
    }

    static inline void EndianSwap(IW4Xenon482::XQuat2& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
    }

    static inline void
        EndianSwapPartial(IW4Xenon482::XAnimDeltaPartQuat& v, [[maybe_unused]] const size_t loadedSize, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.size);
    }

    static inline void EndianSwapPartial(IW4Xenon482::XAnimDeltaPartQuatDataFrames& v,
                                         [[maybe_unused]] const size_t loadedSize,
                                         [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.frames);
    }

    static inline void EndianSwap(IW4Xenon482::XQuat& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.value);
    }

    static inline void EndianSwap(IW4Xenon482::XAnimIndices& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.data);
    }

    static inline void EndianSwap(IW4Xenon482::XAnimParts& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::snd_alias_list_name& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.soundName);
    }

    static inline void EndianSwap(IW4Xenon482::SndAliasCustom& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
    }

    static inline void EndianSwap(IW4Xenon482::WeaponDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        // IW4 Xenon 482 WeaponDef is all 32-bit fields until the final bool pack, except
        // the two accuracy graph knot counts at 0x51C.
        EndianSwap32Range(v, 0, offsetof(IW4Xenon482::WeaponDef, originalAiVsAiAccuracyGraphKnotCount));
        EndianSwap(v.originalAiVsAiAccuracyGraphKnotCount);
        EndianSwap(v.originalAiVsPlayerAccuracyGraphKnotCount);
        EndianSwap32Range(v, offsetof(IW4Xenon482::WeaponDef, iPositionReloadTransTime), offsetof(IW4Xenon482::WeaponDef, sharedAmmo));
    }

    static inline void EndianSwap(IW4Xenon482::WeaponCompleteDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::XAUDIOREVERBSETTINGS& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.ReflectionsDelay);
        EndianSwap(v.RoomFilterFreq);
        EndianSwap(v.RoomFilterMain);
        EndianSwap(v.RoomFilterHF);
        EndianSwap(v.ReflectionsGain);
        EndianSwap(v.ReverbGain);
        EndianSwap(v.DecayTime);
        EndianSwap(v.Density);
        EndianSwap(v.RoomSize);
    }

    static inline void EndianSwap(IW4Xenon482::XaReverbSettings& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.presetOverridden);
        EndianSwap(v.reverbSettings, operation);
    }

    static inline void EndianSwap(IW4Xenon482::SndDriverGlobals& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.reverbSettings);
        EndianSwap(v.name);
    }

    static inline void EndianSwap(IW4Xenon482::Statement_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numEntries);
        EndianSwap(v.entries);
        EndianSwap(v.supportingData);
        EndianSwap(v.lastExecuteTime);
        v.lastResult = {};
    }

    static inline void EndianSwap(IW4Xenon482::StaticDvar& v, [[maybe_unused]] const EndianOperation operation)
    {
        v.dvar = nullptr;
        EndianSwap(v.dvarName);
    }

    static inline void EndianSwap(IW4Xenon482::StaticDvarList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.numStaticDvars);
        EndianSwap(v.staticDvars);
    }

    static inline void EndianSwap(IW4Xenon482::StringList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.totalStrings);
        EndianSwap(v.strings);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataEnumEntry& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.string);
        EndianSwap(v.index);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataEnum& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.entryCount);
        EndianSwap(v.reservedEntryCount);
        EndianSwap(v.entries);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataTypeUnion& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.stringDataLength);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataType& v, [[maybe_unused]] const EndianOperation operation)
    {
        const auto type = EndianSwapDiscriminator(v.type, operation);

        switch (type)
        {
        case IW4Xenon482::DATA_STRING:
        case IW4Xenon482::DATA_ENUM:
        case IW4Xenon482::DATA_STRUCT:
        case IW4Xenon482::DATA_INDEXED_ARRAY:
        case IW4Xenon482::DATA_ENUM_ARRAY:
            EndianSwap(v.u, operation);
            break;

        default:
            break;
        }
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataStructProperty& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.type, operation);
        EndianSwap(v.offset);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataStruct& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.propertyCount);
        EndianSwap(v.properties);
        EndianSwap(v.size);
        EndianSwap(v.bitOffset);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataIndexedArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.arraySize);
        EndianSwap(v.elementType, operation);
        EndianSwap(v.elementSize);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataEnumedArray& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.enumIndex);
        EndianSwap(v.elementType, operation);
        EndianSwap(v.elementSize);
    }

    static inline void EndianSwap(IW4Xenon482::StructuredDataDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::StructuredDataDefSet& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.defCount);
        EndianSwap(v.defs);
    }

    static inline void EndianSwap(IW4Xenon482::textScrollDef_s& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.startTime);
    }

    static inline void EndianSwap(IW4Xenon482::TracerDef& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.name);
        EndianSwap(v.material);
        EndianSwap(v.drawInterval);
        EndianSwap(v.speed);
        EndianSwap(v.beamLength);
        EndianSwap(v.beamWidth);
        EndianSwap(v.screwRadius);
        EndianSwap(v.screwDist);
        EndianSwap(v.colors);
    }

    static inline void EndianSwap(IW4Xenon482::UIFunctionList& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.totalFunctions);
        EndianSwap(v.functions);
    }

    static inline void EndianSwap(IW4Xenon482::VehiclePhysDef& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::VehicleTrackObstacle& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.origin);
        EndianSwap(v.radius);
    }

    static inline void EndianSwap(IW4Xenon482::VehicleTrackSector& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::VehicleTrackSegment& v, [[maybe_unused]] const EndianOperation operation)
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

    static inline void EndianSwap(IW4Xenon482::VehicleTrack& v, [[maybe_unused]] const EndianOperation operation)
    {
        EndianSwap(v.segments);
        EndianSwap(v.segmentCount);
    }

    static inline void EndianSwap(IW4Xenon482::VehicleDef& v, [[maybe_unused]] const EndianOperation operation)
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
        EndianSwap(v.trophyRadius);
        EndianSwap(v.trophyInactiveRadius);
        EndianSwap(v.trophyAmmoCount);
        EndianSwap(v.trophyReloadTime);
        EndianSwap(v.trophyTags);
        EndianSwap(v.compassFriendlyIcon);
        EndianSwap(v.compassEnemyIcon);
        EndianSwap(v.compassIconWidth);
        EndianSwap(v.compassIconHeight);
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
        EndianSwap(v.slideVolume);
        EndianSwap(v.slideBlendSpeed);
        EndianSwap(v.inAirPitch);
    }

    static inline void EndianSwap(IW4Xenon482::windowDef_t& v, [[maybe_unused]] const EndianOperation operation)
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

} // namespace IW4Xenon482
