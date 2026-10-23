#include "FontDumperIW3Xenon328.h"

#include "Font/FontCommon.h"
#include "Game/IW3Xenon328/Font/JsonFontIW3Xenon328.h"

#include <iomanip>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace IW3Xenon328;

namespace
{
    const char* AssetName(const char* input)
    {
        if (input && input[0] == ',')
            return &input[1];

        return input;
    }

    void CreateJsonGlyph(JsonGlyph& jGlyph, const Glyph& glyph)
    {
        jGlyph.letter = glyph.letter;
        jGlyph.x0 = glyph.x0;
        jGlyph.y0 = glyph.y0;
        jGlyph.dx = glyph.dx;
        jGlyph.pixelWidth = glyph.pixelWidth;
        jGlyph.pixelHeight = glyph.pixelHeight;
        jGlyph.s0 = glyph.s0;
        jGlyph.t0 = glyph.t0;
        jGlyph.s1 = glyph.s1;
        jGlyph.t1 = glyph.t1;
    }

    void CreateCompiledJsonFont(JsonCompiledFont& jFont, const Font_s& font)
    {
        jFont.pixelHeight = font.pixelHeight;

        if (font.material && font.material->info.name)
            jFont.material = AssetName(font.material->info.name);

        if (font.glowMaterial && font.glowMaterial->info.name)
            jFont.glowMaterial = AssetName(font.glowMaterial->info.name);

        if (font.glyphs == nullptr || font.glyphCount <= 0)
            return;

        jFont.glyphs.resize(font.glyphCount);
        for (auto i = 0; i < font.glyphCount; i++)
            CreateJsonGlyph(jFont.glyphs[i], font.glyphs[i]);
    }

    class Dumper
    {
    public:
        explicit Dumper(std::ostream& stream)
            : m_stream(stream)
        {
        }

        void Dump(const Font_s& font) const
        {
            JsonFont jFont;
            jFont.font.emplace<JsonCompiledFont>();
            CreateCompiledJsonFont(std::get<JsonCompiledFont>(jFont.font), font);

            ordered_json jRoot;
            jRoot["$schema"] = "http://openassettools.dev/schema/font.v1.json";
            jRoot["_type"] = "font";
            jRoot["_version"] = 1;
            jRoot["_game"] = "iw3";

            jRoot.merge_patch(jFont);

            m_stream << std::setw(4) << jRoot << "\n";
        }

    private:
        std::ostream& m_stream;
    };
} // namespace

namespace font
{
    void JsonDumperIW3Xenon328::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetFont::Type>& asset)
    {
        const auto assetFile = context.OpenAssetFile(GetJsonFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        Dumper dumper(*assetFile);
        dumper.Dump(*asset.Asset());
    }
} // namespace font
