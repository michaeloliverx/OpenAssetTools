#include "CommonAssetIW2Xenon.h"

#include "IW2Xenon.h"

#include <cassert>
using namespace IW2Xenon;
CommonAssetTypeMapper::CommonAssetTypeMapper() = default;

CommonAssetType CommonAssetTypeMapper::GameToCommonAssetType(const asset_type_t type) const
{
    static constexpr CommonAssetType types[]{CommonAssetType::XANIM,
                                             CommonAssetType::XMODEL,
                                             CommonAssetType::MATERIAL,
                                             CommonAssetType::IMAGE,
                                             CommonAssetType::SOUND,
                                             CommonAssetType::SOUND_CURVE,
                                             CommonAssetType::CLIP_MAP,
                                             CommonAssetType::CLIP_MAP,
                                             CommonAssetType::GFX_WORLD,
                                             CommonAssetType::LIGHT_DEF,
                                             CommonAssetType::UI_MAP,
                                             CommonAssetType::FONT,
                                             CommonAssetType::MENU_LIST,
                                             CommonAssetType::MENU,
                                             CommonAssetType::LOCALIZE_ENTRY,
                                             CommonAssetType::WEAPON,
                                             CommonAssetType::SOUND_DRIVER_GLOBALS,
                                             CommonAssetType::FX,
                                             CommonAssetType::IMPACT_FX,
                                             CommonAssetType::AI_TYPE,
                                             CommonAssetType::MP_TYPE,
                                             CommonAssetType::CHARACTER,
                                             CommonAssetType::XMODEL_ALIAS,
                                             CommonAssetType::RAW_FILE};
    static_assert(std::extent_v<decltype(types)> == ASSET_TYPE_COUNT);
    assert(type < ASSET_TYPE_COUNT);
    return types[type];
}

std::optional<asset_type_t> CommonAssetTypeMapper::CommonToGameAssetType(const CommonAssetType type) const
{
    for (asset_type_t i = 0; i < ASSET_TYPE_COUNT; ++i)
        if (GameToCommonAssetType(i) == type && i != ASSET_TYPE_CLIPMAP_SP)
            return i;
    return std::nullopt;
}
