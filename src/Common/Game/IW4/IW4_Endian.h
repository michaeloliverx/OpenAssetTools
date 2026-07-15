#pragma once

#include "IW4.h"
#include "Utils/Endianness.h"

namespace endianness
{
    template<> struct InPlaceBigEndianConverter<IW4::PhysPreset>
    {
        static void Convert(IW4::PhysPreset& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.type);
            FromBigEndianInPlace(value.mass);
            FromBigEndianInPlace(value.bounce);
            FromBigEndianInPlace(value.friction);
            FromBigEndianInPlace(value.bulletForceScale);
            FromBigEndianInPlace(value.explosiveForceScale);
            FromBigEndianInPlace(value.sndAliasPrefix);
            FromBigEndianInPlace(value.piecesSpreadFraction);
            FromBigEndianInPlace(value.piecesUpwardVelocity);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::Bounds>
    {
        static void Convert(IW4::Bounds& value)
        {
            FromBigEndianInPlace(value.midPoint.v);
            FromBigEndianInPlace(value.halfSize.v);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::cplane_s>
    {
        static void Convert(IW4::cplane_s& value)
        {
            FromBigEndianInPlace(value.normal);
            FromBigEndianInPlace(value.dist);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::cbrushside_t>
    {
        static void Convert(IW4::cbrushside_t& value)
        {
            FromBigEndianInPlace(value.plane);
            FromBigEndianInPlace(value.materialNum);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::cbrushWrapper_t>
    {
        static void Convert(IW4::cbrushWrapper_t& value)
        {
            FromBigEndianInPlace(value.numsides);
            FromBigEndianInPlace(value.glassPieceIndex);
            FromBigEndianInPlace(value.sides);
            FromBigEndianInPlace(value.baseAdjacentSide);
            FromBigEndianInPlace(value.axialMaterialNum);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::BrushWrapper>
    {
        static void Convert(IW4::BrushWrapper& value)
        {
            FromBigEndianInPlace(value.bounds);
            FromBigEndianInPlace(value.brush);
            FromBigEndianInPlace(value.totalEdgeCount);
            FromBigEndianInPlace(value.planes);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::PhysGeomInfo>
    {
        static void Convert(IW4::PhysGeomInfo& value)
        {
            FromBigEndianInPlace(value.brushWrapper);
            FromBigEndianInPlace(value.type);
            FromBigEndianInPlace(value.orientation);
            FromBigEndianInPlace(value.bounds);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::PhysMass>
    {
        static void Convert(IW4::PhysMass& value)
        {
            FromBigEndianInPlace(value.centerOfMass);
            FromBigEndianInPlace(value.momentsOfInertia);
            FromBigEndianInPlace(value.productsOfInertia);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::PhysCollmap>
    {
        static void Convert(IW4::PhysCollmap& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.count);
            FromBigEndianInPlace(value.geoms);
            FromBigEndianInPlace(value.mass);
            FromBigEndianInPlace(value.bounds);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::RawFileBuffer>
    {
        static void Convert(IW4::RawFileBuffer& value)
        {
            FromBigEndianInPlace(value.buffer);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::RawFile>
    {
        static void Convert(IW4::RawFile& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.compressedLen);
            FromBigEndianInPlace(value.len);
            FromBigEndianInPlace(value.data);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::StringTableCell>
    {
        static void Convert(IW4::StringTableCell& value)
        {
            FromBigEndianInPlace(value.string);
            FromBigEndianInPlace(value.hash);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::StringTable>
    {
        static void Convert(IW4::StringTable& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.columnCount);
            FromBigEndianInPlace(value.rowCount);
            FromBigEndianInPlace(value.values);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::SndCurve>
    {
        static void Convert(IW4::SndCurve& value)
        {
            FromBigEndianInPlace(value.filename);
            FromBigEndianInPlace(value.knotCount);
            FromBigEndianInPlace(value.knots);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::LocalizeEntry>
    {
        static void Convert(IW4::LocalizeEntry& value)
        {
            FromBigEndianInPlace(value.value);
            FromBigEndianInPlace(value.name);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::GfxLightImage>
    {
        static void Convert(IW4::GfxLightImage& value)
        {
            FromBigEndianInPlace(value.image);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::GfxLightDef>
    {
        static void Convert(IW4::GfxLightDef& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.attenuation);
            FromBigEndianInPlace(value.lmapLookupStart);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::LbColumnDef>
    {
        static void Convert(IW4::LbColumnDef& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.id);
            FromBigEndianInPlace(value.propertyId);
            FromBigEndianInPlace(value.statName);
            FromBigEndianInPlace(value.type);
            FromBigEndianInPlace(value.precision);
            FromBigEndianInPlace(value.agg);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::LeaderboardDef>
    {
        static void Convert(IW4::LeaderboardDef& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.id);
            FromBigEndianInPlace(value.columnCount);
            FromBigEndianInPlace(value.xpColId);
            FromBigEndianInPlace(value.prestigeColId);
            FromBigEndianInPlace(value.columns);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::TracerDef>
    {
        static void Convert(IW4::TracerDef& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.material);
            FromBigEndianInPlace(value.drawInterval);
            FromBigEndianInPlace(value.speed);
            FromBigEndianInPlace(value.beamLength);
            FromBigEndianInPlace(value.beamWidth);
            FromBigEndianInPlace(value.screwRadius);
            FromBigEndianInPlace(value.screwDist);
            FromBigEndianInPlace(value.colors);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::TriggerModel>
    {
        static void Convert(IW4::TriggerModel& value)
        {
            FromBigEndianInPlace(value.contents);
            FromBigEndianInPlace(value.hullCount);
            FromBigEndianInPlace(value.firstHull);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::TriggerHull>
    {
        static void Convert(IW4::TriggerHull& value)
        {
            FromBigEndianInPlace(value.bounds);
            FromBigEndianInPlace(value.contents);
            FromBigEndianInPlace(value.slabCount);
            FromBigEndianInPlace(value.firstSlab);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::TriggerSlab>
    {
        static void Convert(IW4::TriggerSlab& value)
        {
            FromBigEndianInPlace(value.dir);
            FromBigEndianInPlace(value.midPoint);
            FromBigEndianInPlace(value.halfSize);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::MapTriggers>
    {
        static void Convert(IW4::MapTriggers& value)
        {
            FromBigEndianInPlace(value.count);
            FromBigEndianInPlace(value.models);
            FromBigEndianInPlace(value.hullCount);
            FromBigEndianInPlace(value.hulls);
            FromBigEndianInPlace(value.slabCount);
            FromBigEndianInPlace(value.slabs);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::Stage>
    {
        static void Convert(IW4::Stage& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.origin);
            FromBigEndianInPlace(value.triggerIndex);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::MapEnts>
    {
        static void Convert(IW4::MapEnts& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.entityString);
            FromBigEndianInPlace(value.numEntityChars);
            FromBigEndianInPlace(value.trigger);
            FromBigEndianInPlace(value.stages);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::AddonMapEnts>
    {
        static void Convert(IW4::AddonMapEnts& value)
        {
            FromBigEndianInPlace(value.name);
            FromBigEndianInPlace(value.entityString);
            FromBigEndianInPlace(value.numEntityChars);
            FromBigEndianInPlace(value.trigger);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::ScriptStringList>
    {
        static void Convert(IW4::ScriptStringList& value)
        {
            FromBigEndianInPlace(value.count);
            FromBigEndianInPlace(value.strings);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::XAsset>
    {
        static void Convert(IW4::XAsset& value)
        {
            FromBigEndianInPlace(value.type);
            FromBigEndianInPlace(value.header.data);
        }
    };

    template<> struct InPlaceBigEndianConverter<IW4::XAssetList>
    {
        static void Convert(IW4::XAssetList& value)
        {
            InPlaceBigEndianConverter<IW4::ScriptStringList>::Convert(value.stringList);
            FromBigEndianInPlace(value.assetCount);
            FromBigEndianInPlace(value.assets);
        }
    };
} // namespace endianness
