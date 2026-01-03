#pragma once

// clang-format off: Order of includes matters here

#include "Game/IAsset.h"

#include "IW3Xenon_Assets.h"

// clang-format on

namespace IW3Xenon
{
    struct ScriptStringList
    {
        int count;
        const char** strings;
    };

    struct XAsset
    {
        XAssetType type;
        XAssetHeader header;
    };

    struct XAssetList
    {
        ScriptStringList stringList;
        int assetCount;
        XAsset* assets;
    };

    using AssetMaterial = Asset<ASSET_TYPE_MATERIAL, Material>;
    using AssetTechniqueSet = Asset<ASSET_TYPE_TECHNIQUE_SET, MaterialTechniqueSet>;
    using AssetImage = Asset<ASSET_TYPE_IMAGE, GfxImage>;
    using AssetLocalize = Asset<ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry>;
    using AssetRawFile = Asset<ASSET_TYPE_RAWFILE, RawFile>;
    using AssetStringTable = Asset<ASSET_TYPE_STRINGTABLE, StringTable>;
} // namespace IW3Xenon

DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetMaterial, info.name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetTechniqueSet, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetImage, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetLocalize, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetRawFile, name);
DEFINE_ASSET_NAME_ACCESSOR(IW3Xenon::AssetStringTable, name);
