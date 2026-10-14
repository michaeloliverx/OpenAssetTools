#include "InfoStringLoaderPhysPresetT4Xenon.h"

#include "Game/T4/InfoString/InfoStringToStructConverter.h"
#include "Game/T4Xenon/ObjConstantsT4Xenon.h"
#include "Game/T4Xenon/PhysPreset/PhysPresetFieldsT4Xenon.h"
#include "Game/T4Xenon/T4Xenon.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <cassert>
#include <type_traits>

using namespace T4Xenon;
using T4::cspField_t;
using T4::InfoStringToStructConverter;

namespace
{
    class InfoStringToPhysPresetConverter final : public InfoStringToStructConverter
    {
    protected:
        bool ConvertExtensionField(const cspField_t& field, const std::string& value) override
        {
            assert(false);
            return false;
        }

    public:
        InfoStringToPhysPresetConverter(const InfoString& infoString,
                                        PhysPresetInfo& physPreset,
                                        ZoneScriptStrings& zoneScriptStrings,
                                        MemoryManager& memory,
                                        AssetCreationContext& context,
                                        AssetRegistration<AssetPhysPreset>& registration,
                                        const cspField_t* fields,
                                        const size_t fieldCount)
            : InfoStringToStructConverter(infoString, &physPreset, zoneScriptStrings, memory, context, registration, fields, fieldCount)
        {
        }
    };

    void CopyFromPhysPresetInfo(const PhysPresetInfo& physPresetInfo, PhysPreset& physPreset)
    {
        physPreset.mass = std::clamp(physPresetInfo.mass, 1.0f, 2000.0f) * 0.001f;
        physPreset.bounce = physPresetInfo.bounce;

        if (physPresetInfo.isFrictionInfinity != 0)
            physPreset.friction = PHYS_PRESET_MAX_FRICTION;
        else
            physPreset.friction = physPresetInfo.friction;

        physPreset.bulletForceScale = physPresetInfo.bulletForceScale;
        physPreset.explosiveForceScale = physPresetInfo.explosiveForceScale;
        physPreset.sndAliasPrefix = physPresetInfo.sndAliasPrefix;
        physPreset.piecesSpreadFraction = physPresetInfo.piecesSpreadFraction;
        physPreset.piecesUpwardVelocity = physPresetInfo.piecesUpwardVelocity;
        physPreset.canFloat = physPresetInfo.canFloat;
        physPreset.tempDefaultToCylinder = physPresetInfo.tempDefaultToCylinder != 0;
    }
} // namespace

namespace phys_preset
{
    InfoStringLoaderT4Xenon::InfoStringLoaderT4Xenon(MemoryManager& memory, Zone& zone)
        : m_memory(memory),
          m_zone(zone)
    {
    }

    AssetCreationResult InfoStringLoaderT4Xenon::CreateAsset(const std::string& assetName, const InfoString& infoString, AssetCreationContext& context)
    {
        auto* physPreset = m_memory.Alloc<PhysPreset>();
        physPreset->name = m_memory.Dup(assetName.c_str());

        AssetRegistration<AssetPhysPreset> registration(assetName, physPreset);

        PhysPresetInfo physPresetInfo{};
        InfoStringToPhysPresetConverter converter(infoString,
                                                  physPresetInfo,
                                                  m_zone.m_script_strings,
                                                  m_memory,
                                                  context,
                                                  registration,
                                                  phys_preset_fields,
                                                  std::extent_v<decltype(phys_preset_fields)>);
        if (!converter.Convert())
        {
            con::error("Failed to parse phys preset: \"{}\"", assetName);
            return AssetCreationResult::Failure();
        }

        CopyFromPhysPresetInfo(physPresetInfo, *physPreset);

        return AssetCreationResult::Success(context.AddAsset(std::move(registration)));
    }
} // namespace phys_preset
