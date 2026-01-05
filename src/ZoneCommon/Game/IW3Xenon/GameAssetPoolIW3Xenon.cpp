#include "GameAssetPoolIW3Xenon.h"
#include "Pool/AssetPoolDynamic.h"

#include <cassert>
#include <type_traits>

using namespace IW3Xenon;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[ASSET_TYPE_COUNT]{
        "xmodelpieces", "physpreset",  "xanim",      "xmodel",     "material",    "pixelshader",      "techset",  "image",    "sound",    "sndcurve",
        "loaded_sound", "col_map_sp",  "col_map_mp", "com_map",    "game_map_sp", "game_map_mp",      "map_ents", "gfx_map",  "lightdef", "ui_map",
        "font",         "menufile",    "menu",       "localize",   "weapon",      "snddriverglobals", "fx",       "impactfx", "aitype",   "mptype",
        "character",    "xmodelalias", "rawfile",    "stringtable"};
}

GameAssetPoolIW3Xenon::GameAssetPoolIW3Xenon(Zone* zone, const zone_priority_t priority)
    : ZoneAssetPools(zone),
      m_priority(priority)
{
    static_assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);

#define INIT_POOL(poolName) (poolName) = std::make_unique<AssetPoolDynamic<decltype(poolName)::element_type::type>>(m_priority)

    INIT_POOL(m_phys_preset);
    INIT_POOL(m_xanim_parts);
    INIT_POOL(m_xmodel);
    INIT_POOL(m_material);
    INIT_POOL(m_pixel_shader);
    INIT_POOL(m_technique_set);
    INIT_POOL(m_image);
    INIT_POOL(m_sound);
    INIT_POOL(m_sound_curve);
    INIT_POOL(m_loaded_sound);
    INIT_POOL(m_clip_map);
    INIT_POOL(m_com_world);
    INIT_POOL(m_game_world_sp);
    INIT_POOL(m_game_world_mp);
    INIT_POOL(m_map_ents);
    INIT_POOL(m_gfx_world);
    INIT_POOL(m_gfx_light_def);
    INIT_POOL(m_font);
    INIT_POOL(m_menu_list);
    INIT_POOL(m_menu_def);
    INIT_POOL(m_localize);
    INIT_POOL(m_weapon);
    INIT_POOL(m_sound_driver_globals);
    INIT_POOL(m_fx);
    INIT_POOL(m_fx_impact_table);
    INIT_POOL(m_raw_file);
    INIT_POOL(m_string_table);

#undef INIT_POOL
}

XAssetInfoGeneric* GameAssetPoolIW3Xenon::AddAssetToPool(std::unique_ptr<XAssetInfoGeneric> xAssetInfo)
{
#define CASE_ADD_TO_POOL(assetType, poolName)                                                                                                                  \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        assert(poolName);                                                                                                                                      \
        return (poolName)->AddAsset(std::unique_ptr<XAssetInfo<decltype(poolName)::element_type::type>>(                                                       \
            static_cast<XAssetInfo<decltype(poolName)::element_type::type>*>(xAssetInfo.release())));                                                          \
    }

    switch (static_cast<XAssetType>(xAssetInfo->m_type))
    {
        CASE_ADD_TO_POOL(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_ADD_TO_POOL(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_ADD_TO_POOL(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_ADD_TO_POOL(ASSET_TYPE_MATERIAL, m_material)
        CASE_ADD_TO_POOL(ASSET_TYPE_PIXELSHADER, m_pixel_shader)
        CASE_ADD_TO_POOL(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_ADD_TO_POOL(ASSET_TYPE_IMAGE, m_image)
        CASE_ADD_TO_POOL(ASSET_TYPE_SOUND, m_sound)
        CASE_ADD_TO_POOL(ASSET_TYPE_SOUND_CURVE, m_sound_curve)
        CASE_ADD_TO_POOL(ASSET_TYPE_LOADED_SOUND, m_loaded_sound)
        CASE_ADD_TO_POOL(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_ADD_TO_POOL(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_ADD_TO_POOL(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_ADD_TO_POOL(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_ADD_TO_POOL(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_ADD_TO_POOL(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_ADD_TO_POOL(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_ADD_TO_POOL(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_ADD_TO_POOL(ASSET_TYPE_FONT, m_font)
        CASE_ADD_TO_POOL(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_ADD_TO_POOL(ASSET_TYPE_MENU, m_menu_def)
        CASE_ADD_TO_POOL(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_ADD_TO_POOL(ASSET_TYPE_WEAPON, m_weapon)
        CASE_ADD_TO_POOL(ASSET_TYPE_SNDDRIVER_GLOBALS, m_sound_driver_globals)
        CASE_ADD_TO_POOL(ASSET_TYPE_FX, m_fx)
        CASE_ADD_TO_POOL(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_ADD_TO_POOL(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_ADD_TO_POOL(ASSET_TYPE_STRINGTABLE, m_string_table)

    default:
        assert(false);
        break;
    }

    return nullptr;

#undef CASE_ADD_TO_POOL
}

XAssetInfoGeneric* GameAssetPoolIW3Xenon::GetAsset(const asset_type_t type, const std::string& name) const
{
#define CASE_GET_ASSET(assetType, poolName)                                                                                                                    \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        if (poolName)                                                                                                                                          \
            return (poolName)->GetAsset(std::move(name));                                                                                                      \
        break;                                                                                                                                                 \
    }

    switch (type)
    {
        CASE_GET_ASSET(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_GET_ASSET(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_GET_ASSET(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_GET_ASSET(ASSET_TYPE_MATERIAL, m_material)
        CASE_GET_ASSET(ASSET_TYPE_PIXELSHADER, m_pixel_shader)
        CASE_GET_ASSET(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_GET_ASSET(ASSET_TYPE_IMAGE, m_image)
        CASE_GET_ASSET(ASSET_TYPE_SOUND, m_sound)
        CASE_GET_ASSET(ASSET_TYPE_SOUND_CURVE, m_sound_curve)
        CASE_GET_ASSET(ASSET_TYPE_LOADED_SOUND, m_loaded_sound)
        CASE_GET_ASSET(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_GET_ASSET(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_GET_ASSET(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_GET_ASSET(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_GET_ASSET(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_GET_ASSET(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_GET_ASSET(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_GET_ASSET(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_GET_ASSET(ASSET_TYPE_FONT, m_font)
        CASE_GET_ASSET(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_GET_ASSET(ASSET_TYPE_MENU, m_menu_def)
        CASE_GET_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_GET_ASSET(ASSET_TYPE_WEAPON, m_weapon)
        CASE_GET_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, m_sound_driver_globals)
        CASE_GET_ASSET(ASSET_TYPE_FX, m_fx)
        CASE_GET_ASSET(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_GET_ASSET(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_GET_ASSET(ASSET_TYPE_STRINGTABLE, m_string_table)

    default:
        assert(false);
        break;
    }

    return nullptr;

#undef CASE_GET_ASSET
}

std::optional<const char*> GameAssetPoolIW3Xenon::AssetTypeNameByType(const asset_type_t assetType)
{
    if (assetType >= 0 && assetType < static_cast<int>(std::extent_v<decltype(ASSET_TYPE_NAMES)>))
        return ASSET_TYPE_NAMES[assetType];

    return std::nullopt;
}

std::optional<const char*> GameAssetPoolIW3Xenon::GetAssetTypeName(const asset_type_t assetType) const
{
    return AssetTypeNameByType(assetType);
}

asset_type_t GameAssetPoolIW3Xenon::AssetTypeCount()
{
    return ASSET_TYPE_COUNT;
}

asset_type_t GameAssetPoolIW3Xenon::GetAssetTypeCount() const
{
    return AssetTypeCount();
}
