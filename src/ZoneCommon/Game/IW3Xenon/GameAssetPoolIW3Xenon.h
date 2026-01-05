#pragma once

#include "Game/IW3Xenon/IW3Xenon.h"
#include "Pool/AssetPool.h"
#include "Pool/ZoneAssetPools.h"

#include <memory>
#include <optional>

class GameAssetPoolIW3Xenon final : public ZoneAssetPools
{
public:
    std::unique_ptr<AssetPool<IW3Xenon::PhysPreset>> m_phys_preset;
    std::unique_ptr<AssetPool<IW3Xenon::XAnimParts>> m_xanim_parts;
    std::unique_ptr<AssetPool<IW3Xenon::XModel>> m_xmodel;
    std::unique_ptr<AssetPool<IW3Xenon::Material>> m_material;
    std::unique_ptr<AssetPool<IW3Xenon::MaterialTechniqueSet>> m_pixel_shader;
    std::unique_ptr<AssetPool<IW3Xenon::MaterialTechniqueSet>> m_technique_set;
    std::unique_ptr<AssetPool<IW3Xenon::GfxImage>> m_image;
    std::unique_ptr<AssetPool<IW3Xenon::snd_alias_list_t>> m_sound;
    std::unique_ptr<AssetPool<IW3Xenon::SndCurve>> m_sound_curve;
    std::unique_ptr<AssetPool<IW3Xenon::LoadedSound>> m_loaded_sound;
    std::unique_ptr<AssetPool<IW3Xenon::clipMap_t>> m_clip_map;
    std::unique_ptr<AssetPool<IW3Xenon::ComWorld>> m_com_world;
    std::unique_ptr<AssetPool<IW3Xenon::GameWorldSp>> m_game_world_sp;
    std::unique_ptr<AssetPool<IW3Xenon::GameWorldMp>> m_game_world_mp;
    std::unique_ptr<AssetPool<IW3Xenon::MapEnts>> m_map_ents;
    std::unique_ptr<AssetPool<IW3Xenon::GfxWorld>> m_gfx_world;
    std::unique_ptr<AssetPool<IW3Xenon::GfxLightDef>> m_gfx_light_def;
    std::unique_ptr<AssetPool<IW3Xenon::Font_s>> m_font;
    std::unique_ptr<AssetPool<IW3Xenon::MenuList>> m_menu_list;
    std::unique_ptr<AssetPool<IW3Xenon::menuDef_t>> m_menu_def;
    std::unique_ptr<AssetPool<IW3Xenon::LocalizeEntry>> m_localize;
    std::unique_ptr<AssetPool<IW3Xenon::WeaponDef>> m_weapon;
    std::unique_ptr<AssetPool<IW3Xenon::SndDriverGlobals>> m_sound_driver_globals;
    std::unique_ptr<AssetPool<IW3Xenon::FxEffectDef>> m_fx;
    std::unique_ptr<AssetPool<IW3Xenon::FxImpactTable>> m_fx_impact_table;
    std::unique_ptr<AssetPool<IW3Xenon::RawFile>> m_raw_file;
    std::unique_ptr<AssetPool<IW3Xenon::StringTable>> m_string_table;

    GameAssetPoolIW3Xenon(Zone* zone, zone_priority_t priority);
    ~GameAssetPoolIW3Xenon() override = default;

    [[nodiscard]] XAssetInfoGeneric* GetAsset(asset_type_t type, const std::string& name) const override;

    static std::optional<const char*> AssetTypeNameByType(asset_type_t assetType);
    [[nodiscard]] std::optional<const char*> GetAssetTypeName(asset_type_t assetType) const override;

    static asset_type_t AssetTypeCount();
    [[nodiscard]] asset_type_t GetAssetTypeCount() const override;

protected:
    XAssetInfoGeneric* AddAssetToPool(std::unique_ptr<XAssetInfoGeneric> xAssetInfo) override;

private:
    zone_priority_t m_priority;
};
