#include "ContentWriterIW2Xenon.h"

#include "Game/IW2Xenon/AssetEndianSwapIW2Xenon.h"
#include "Game/IW2Xenon/AssetMarkerIW2Xenon.h"
#include "Game/IW2Xenon/AssetWriterIW2Xenon.h"
#include "Writing/WritingException.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <string_view>
#include <unordered_set>

using namespace IW2Xenon;

namespace
{
    class StringCollector final : public AssetVisitor
    {
    public:
        void Visit_String(const char* value) override
        {
            if (value && m_seen.emplace(value).second)
                m_strings.push_back(value);
        }

        void Collect(const XAssetInfoGeneric& asset)
        {
            switch (asset.m_type)
            {
            case ASSET_TYPE_RAWFILE:
            {
                Marker_RawFile marker(*this);
                marker.Mark(static_cast<RawFile*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_LOCALIZE_ENTRY:
            {
                Marker_LocalizeEntry marker(*this);
                marker.Mark(static_cast<LocalizeEntry*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_SND_CURVE:
            {
                Marker_SndCurve marker(*this);
                marker.Mark(static_cast<SndCurve*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_IMAGE:
            {
                Marker_GfxImage marker(*this);
                marker.Mark(static_cast<GfxImage*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_MATERIAL:
            {
                Marker_Material marker(*this);
                marker.Mark(static_cast<Material*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_LIGHT_DEF:
            {
                Marker_GfxLightDef marker(*this);
                marker.Mark(static_cast<GfxLightDef*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_FONT:
            {
                Marker_Font_s marker(*this);
                marker.Mark(static_cast<Font_s*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_SNDDRIVER_GLOBALS:
            {
                Marker_SndDriverGlobals marker(*this);
                marker.Mark(static_cast<SndDriverGlobals*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_CLIPMAP_SP:
            case ASSET_TYPE_CLIPMAP_MP:
            {
                Marker_clipMap_t marker(*this);
                marker.Mark(static_cast<clipMap_t*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_GFXWORLD:
            {
                Marker_GfxWorld marker(*this);
                marker.Mark(static_cast<GfxWorld*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_XMODEL:
            {
                Marker_XModel marker(*this);
                marker.Mark(static_cast<XModel*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_XANIMPARTS:
            {
                Marker_XAnimParts marker(*this);
                marker.Mark(static_cast<XAnimParts*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_SOUND:
            {
                Marker_snd_alias_list_t marker(*this);
                marker.Mark(static_cast<snd_alias_list_t*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_FX:
            {
                Marker_EffectTemplate marker(*this);
                marker.Mark(static_cast<EffectTemplate*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_IMPACT_FX:
            {
                Marker_FxImpactTable marker(*this);
                marker.Mark(static_cast<FxImpactTable*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_MENU:
            {
                Marker_menuDef_t marker(*this);
                marker.Mark(static_cast<menuDef_t*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_MENUFILE:
            {
                Marker_MenuList marker(*this);
                marker.Mark(static_cast<MenuList*>(asset.m_ptr));
                break;
            }
            case ASSET_TYPE_WEAPON:
            {
                Marker_WeaponDef marker(*this);
                marker.Mark(static_cast<WeaponDef*>(asset.m_ptr));
                break;
            }
            default:
                throw WritingException("Unsupported IW2 asset type");
            }
        }

        std::vector<const char*> m_strings;

    private:
        std::unordered_set<std::string_view> m_seen;
    };
} // namespace

ContentWriter::ContentWriter(const Zone& zone)
    : ContentWriterBase(zone),
      varXAssetList(nullptr),
      varXAsset(nullptr),
      varXStringList(nullptr)
{
}

void ContentWriter::CreateXAssetList(XAssetList& xAssetList, MemoryManager& memory)
{
    if (!m_zone.m_script_strings.Empty())
    {
        assert(m_zone.m_script_strings.Count() <= SCR_STRING_MAX + 1);
        xAssetList.tempStrings.count = static_cast<int>(m_zone.m_script_strings.Count());
        xAssetList.tempStrings.strings = memory.Alloc<const char*>(m_zone.m_script_strings.Count());

        for (auto i = 0u; i < m_zone.m_script_strings.Count(); i++)
            xAssetList.tempStrings.strings[i] = m_zone.m_script_strings.CValue(i);
    }

    StringCollector collector;
    for (const auto* asset : m_zone.m_pools)
        collector.Collect(*asset);
    if (!collector.m_strings.empty())
    {
        xAssetList.rawStrings.count = static_cast<int>(collector.m_strings.size());
        xAssetList.rawStrings.strings = memory.Alloc<const char*>(collector.m_strings.size());
        std::copy(collector.m_strings.begin(), collector.m_strings.end(), xAssetList.rawStrings.strings);
    }

    const auto assetCount = m_zone.m_pools.GetTotalAssetCount();
    if (assetCount > 0)
    {
        xAssetList.assetCount = static_cast<int>(assetCount);
        xAssetList.assets = memory.Alloc<XAsset>(assetCount);

        auto index = 0u;
        for (auto i = m_zone.m_pools.begin(); i != m_zone.m_pools.end(); ++i)
        {
            auto& asset = xAssetList.assets[index++];
            asset.type = static_cast<XAssetType>((*i)->m_type);
            asset.header.data = (*i)->m_ptr;
        }
    }
}

void ContentWriter::WriteXStringList(const bool atStreamStart)
{
    assert(!atStreamStart);
    if (!varXStringList->strings)
        return;

    m_stream->Align(4);
    const auto pointers = m_stream->Write(varXStringList->strings, varXStringList->count);
    for (int i = 0; i < varXStringList->count; ++i)
    {
        const auto* string = varXStringList->strings[i];
        const auto offset = pointers.AtOffset(i * sizeof(const char*));
        if (string)
        {
            m_stream->WriteNullTerminated(string);
            m_stream->MarkFollowing(offset);
        }
    }
    m_stream->MarkFollowing(varXStringListWritten.AtOffset(4));
}

void ContentWriter::WriteXAsset(const bool atStreamStart)
{
#define WRITE_ASSET(type_index, typeName, headerEntry)                                                                                                         \
    case type_index:                                                                                                                                           \
    {                                                                                                                                                          \
        Writer_##typeName writer(varXAsset->header.headerEntry, m_zone, *m_stream);                                                                            \
        writer.Write(varXAsset->header.headerEntry, varXAssetWritten.AtOffset(4));                                                                             \
        break;                                                                                                                                                 \
    }
    assert(varXAsset != nullptr);

    if (atStreamStart)
    {
        varXAssetWritten = m_stream->Write(varXAsset);
        auto* writtenAsset = static_cast<XAsset*>(varXAssetWritten.Offset());
        EndianSwap(writtenAsset->type);
        EndianSwap(writtenAsset->header.data);
    }

    switch (varXAsset->type)
    {
        WRITE_ASSET(ASSET_TYPE_XANIMPARTS, XAnimParts, xAnim)
        WRITE_ASSET(ASSET_TYPE_XMODEL, XModel, xModel)
        WRITE_ASSET(ASSET_TYPE_MATERIAL, Material, material)
        WRITE_ASSET(ASSET_TYPE_IMAGE, GfxImage, image)
        WRITE_ASSET(ASSET_TYPE_SOUND, snd_alias_list_t, sound)
        WRITE_ASSET(ASSET_TYPE_SND_CURVE, SndCurve, soundCurve)
    case ASSET_TYPE_CLIPMAP_SP:
        WRITE_ASSET(ASSET_TYPE_CLIPMAP_MP, clipMap_t, clipMapMp)
        WRITE_ASSET(ASSET_TYPE_GFXWORLD, GfxWorld, gfxWorld)
        WRITE_ASSET(ASSET_TYPE_LIGHT_DEF, GfxLightDef, lightDef)
        WRITE_ASSET(ASSET_TYPE_FONT, Font_s, font)
        WRITE_ASSET(ASSET_TYPE_MENUFILE, MenuList, menuList)
        WRITE_ASSET(ASSET_TYPE_MENU, menuDef_t, menu)
        WRITE_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, LocalizeEntry, localize)
        WRITE_ASSET(ASSET_TYPE_WEAPON, WeaponDef, weapon)
        WRITE_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, SndDriverGlobals, soundDriverGlobals)
        WRITE_ASSET(ASSET_TYPE_FX, EffectTemplate, fx)
        WRITE_ASSET(ASSET_TYPE_IMPACT_FX, FxImpactTable, impactFx)
        WRITE_ASSET(ASSET_TYPE_RAWFILE, RawFile, rawFile)

    default:
        throw WritingException(std::format("Unsupported asset type: {}.", static_cast<unsigned>(varXAsset->type)));
    }

#undef WRITE_ASSET
}

void ContentWriter::WriteXAssetArray(const bool atStreamStart, const size_t count)
{
    assert(varXAsset != nullptr);
    static_assert(sizeof(XAsset) == 8u);

    if (atStreamStart)
    {
        varXAssetWritten = m_stream->Write(varXAsset, count);
        auto* writtenAssets = static_cast<XAsset*>(varXAssetWritten.Offset());
        for (size_t index = 0; index < count; index++)
        {
            EndianSwap(writtenAssets[index].type);
            EndianSwap(writtenAssets[index].header.data);
        }
    }

    for (size_t index = 0; index < count; index++)
    {
        WriteXAsset(false);
        varXAsset++;
        varXAssetWritten.Inc(sizeof(XAsset));
    }
}

void ContentWriter::WriteContent(ZoneOutputStream& stream)
{
    m_stream = &stream;
    m_stream->UseOriginalPointerSlots(XFILE_BLOCK_TEMP);

    MemoryManager memory;
    XAssetList assetList{};
    CreateXAssetList(assetList, memory);
    varXAssetList = &assetList;

    static_assert(sizeof(XAssetList) == 24u);
    m_stream->PushBlock(XFILE_BLOCK_TEMP);
    varXAssetListWritten = m_stream->Write(&assetList);
    auto* written = static_cast<XAssetList*>(varXAssetListWritten.Offset());
    EndianSwap(written->rawStrings.count);
    EndianSwap(written->tempStrings.count);
    EndianSwap(written->assetCount);

    m_stream->PushBlock(XFILE_BLOCK_VIRTUAL);
    varXStringList = &assetList.rawStrings;
    varXStringListWritten = varXAssetListWritten;
    WriteXStringList(false);
    m_stream->PopBlock();

    varXStringList = &assetList.tempStrings;
    varXStringListWritten = varXAssetListWritten.AtOffset(8);
    WriteXStringList(false);
    m_stream->SetIndexedXStrings({assetList.rawStrings.strings, static_cast<size_t>(assetList.rawStrings.count)});

    m_stream->PushBlock(XFILE_BLOCK_VIRTUAL);
    if (assetList.assets)
    {
        m_stream->Align(4);
        varXAsset = assetList.assets;
        WriteXAssetArray(true, assetList.assetCount);
        m_stream->MarkFollowing(varXAssetListWritten.AtOffset(20));
    }
    m_stream->PopBlock();
    m_stream->PopBlock();
}
