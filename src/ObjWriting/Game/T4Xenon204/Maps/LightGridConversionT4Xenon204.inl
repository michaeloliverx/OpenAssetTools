// Included inside the adapter's anonymous namespace.
// Contracts: linker_pc R_ModernizeLegacyLightGridColors and
// R_LoadLightGridPoints_Version15 / R_EncodeLightGrid_Version15.
struct LegacyGridPoint204
{
    std::array<uint16_t, 3> pos;
    view::GfxLightGridEntry entry;
};

struct ConvertedGrid204
{
    std::array<uint16_t, 3> mins{}, maxs{};
    std::vector<uint16_t> rows{0xffff};
    std::vector<unsigned char> rawRows;
    std::vector<view::GfxLightGridEntry> entries;
    std::vector<view::GfxLightGridColors> colors;
};

view::GfxLightGridColors ConvertGridColor204(const src::GfxLightGridColors& source)
{
    // The old 24-byte record is channel-major: R[8], G[8], B[8].
    const auto* legacy = reinterpret_cast<const unsigned char*>(&source);
    constexpr float rotation[3][3]{
        {.47140452f,  0.f,         .33333334f},
        {-.23570226f, .40824828f,  .33333334f},
        {-.23570226f, -.40824828f, .33333334f}
    };
    view::GfxLightGridColors result{};
    unsigned basis = 0;
    for (unsigned z = 0; z < 4; ++z)
        for (unsigned y = 0; y < 4; ++y)
            for (unsigned x = 0; x < 4; ++x)
            {
                if (x > 0 && x < 3 && y > 0 && y < 3 && z > 0 && z < 3)
                    continue;
                const float delta[]{x * (2.f / 3.f) - 1.f, y * (2.f / 3.f) - 1.f, z * (2.f / 3.f) - 1.f};
                float projected[3];
                for (unsigned axis = 0; axis < 3; ++axis)
                    projected[axis] = delta[0] * rotation[axis][0] + delta[1] * rotation[axis][1] + delta[2] * rotation[axis][2];
                const float length = std::max({std::abs(projected[0]), std::abs(projected[1]), std::abs(projected[2])});
                const float inverseLength = 1.f / length;
                float weight[2][3];
                for (unsigned axis = 0; axis < 3; ++axis)
                {
                    weight[0][axis] = (projected[axis] * inverseLength) * .5f + .5f;
                    weight[1][axis] = 1.f - weight[0][axis];
                }
                float cornerWeight[8];
                for (unsigned corner = 0; corner < 8; ++corner)
                    cornerWeight[corner] = weight[corner & 1][0] * weight[(corner >> 1) & 1][1] * weight[corner >> 2][2];
                for (unsigned channel = 0; channel < 3; ++channel)
                {
                    float sum = 0;
                    for (unsigned corner = 0; corner < 8; ++corner)
                        sum += legacy[channel * 8 + corner] * cornerWeight[corner];
                    result.rgb[basis][channel] = static_cast<unsigned char>(std::clamp(sum, 0.f, 255.f));
                }
                ++basis;
            }
    return result;
}

template<typename T> void AppendGrid204(std::vector<unsigned char>& bytes, const T value)
{
    for (unsigned i = 0; i < sizeof(T); ++i)
        bytes.push_back(static_cast<unsigned char>(value >> (i * 8)));
}

ConvertedGrid204 ConvertLightGrid204(const src::GfxLightGrid& source, nlohmann::ordered_json& recovery)
{
    ConvertedGrid204 out;
    if (!source.entryCount && !source.colorCount)
        return out;
    Require(source.entries && source.colors && source.entryCount && source.colorCount && source.colorCount < UINT16_MAX, "Invalid T4 FF204 light-grid arrays");
    for (unsigned i = 0; i < source.colorCount; ++i)
        out.colors.push_back(ConvertGridColor204(source.colors[i]));
    std::vector<LegacyGridPoint204> points;
    constexpr unsigned traceSwizzle[]{0, 4, 2, 6, 1, 5, 3, 7}; // PC row axis X, column axis Y.
    for (unsigned i = 0; i < source.entryCount; ++i)
    {
        const auto high = source.entries[i].unknown[0], low = source.entries[i].unknown[1];
        const auto xyz = low >> 24;
        Require(!(xyz & 2), "Unknown T4 FF204 packed light-grid position flag");
        Require((low & 0xffff) < source.colorCount, "Invalid T4 FF204 light-grid color reference");
        LegacyGridPoint204 point{};
        point.pos = {static_cast<uint16_t>(((high >> 19) & 0x1ffc) | (xyz >> 6)),
                     static_cast<uint16_t>(((high >> 8) & 0x1ffc) | ((xyz >> 4) & 3)),
                     static_cast<uint16_t>(((high << 2) & 0xffc) | ((xyz >> 2) & 3))};
        point.entry.colorsIndex = static_cast<uint16_t>(low);
        point.entry.primaryLightIndex = static_cast<unsigned char>(xyz & 1);
        const auto trace = (low >> 16) & 0xff;
        for (unsigned corner = 0; corner < 8; ++corner)
            if (trace & (1u << corner))
                point.entry.needsTrace |= static_cast<unsigned char>(1u << traceSwizzle[corner]);
        points.push_back(point);
    }
    std::stable_sort(points.begin(),
                     points.end(),
                     [](const auto& a, const auto& b)
                     {
                         return a.pos < b.pos;
                     });
    points.erase(std::unique(points.begin(),
                             points.end(),
                             [](const auto& a, const auto& b)
                             {
                                 return a.pos == b.pos;
                             }),
                 points.end());
    std::vector<unsigned> scores(out.colors.size());
    for (const auto& point : points)
        if (point.entry.primaryLightIndex == 1)
            ++scores[point.entry.colorsIndex];
    const auto fallback = static_cast<uint16_t>(std::max_element(scores.begin(), scores.end()) - scores.begin());
    std::swap(out.colors[0], out.colors[fallback]);
    for (auto& point : points)
        if (point.entry.colorsIndex == 0)
            point.entry.colorsIndex = fallback;
        else if (point.entry.colorsIndex == fallback)
            point.entry.colorsIndex = 0;
    out.mins = out.maxs = points.front().pos;
    for (const auto& point : points)
        for (unsigned axis = 0; axis < 3; ++axis)
        {
            out.mins[axis] = std::min(out.mins[axis], point.pos[axis]);
            out.maxs[axis] = std::max(out.maxs[axis], point.pos[axis]);
        }
    out.rows.assign(out.maxs[0] - out.mins[0] + 1, 0xffff);
    for (size_t begin = 0; begin < points.size();)
    {
        size_t end = begin + 1;
        while (end < points.size() && points[end].pos[0] == points[begin].pos[0])
            ++end;
        uint16_t zMin = points[begin].pos[2], zMax = zMin;
        for (size_t i = begin; i < end; ++i)
        {
            zMin = std::min(zMin, points[i].pos[2]);
            zMax = std::max(zMax, points[i].pos[2]);
        }
        Require(out.rawRows.size() % 4 == 0 && out.rawRows.size() / 4 < 0xffff, "T4 FF204 PC light-grid row offset overflow");
        out.rows[points[begin].pos[0] - out.mins[0]] = static_cast<uint16_t>(out.rawRows.size() / 4);
        AppendGrid204(out.rawRows, points[begin].pos[1]);
        AppendGrid204(out.rawRows, static_cast<uint16_t>(points[end - 1].pos[1] - points[begin].pos[1] + 1));
        AppendGrid204(out.rawRows, zMin);
        AppendGrid204(out.rawRows, static_cast<uint16_t>(zMax - zMin + 1));
        AppendGrid204(out.rawRows, static_cast<uint32_t>(out.entries.size()));
        unsigned expectedColumn = points[begin].pos[1];
        const bool wideZ = zMax - zMin + 1 > 255;
        for (size_t column = begin; column < end;)
        {
            const unsigned y = points[column].pos[1];
            while (expectedColumn < y)
            {
                const auto count = std::min(y - expectedColumn, 255u);
                AppendGrid204(out.rawRows, static_cast<uint8_t>(count));
                AppendGrid204(out.rawRows, uint8_t{0});
                expectedColumn += count;
            }
            size_t columnEnd = column + 1;
            while (columnEnd < end && points[columnEnd].pos[1] == y)
                ++columnEnd;
            const unsigned bottom = points[column].pos[2], top = points[columnEnd - 1].pos[2];
            Require(top - bottom + 1 <= 255, "T4 FF204 PC light-grid column height overflow");
            auto point = column;
            for (unsigned z = bottom; z <= top; ++z)
                if (point < columnEnd && points[point].pos[2] == z)
                    out.entries.push_back(points[point++].entry);
                else
                    // Match R_EmitDefaultLightGridEntry_Version15 for holes.
                    out.entries.push_back({0, 1, 0});
            AppendGrid204(out.rawRows, uint8_t{1});
            AppendGrid204(out.rawRows, static_cast<uint8_t>(top - bottom + 1));
            AppendGrid204(out.rawRows, static_cast<uint8_t>(bottom - zMin));
            if (wideZ)
                AppendGrid204(out.rawRows, static_cast<uint8_t>((bottom - zMin) >> 8));
            expectedColumn = y + 1;
            column = columnEnd;
        }
        while (out.rawRows.size() % 4)
            out.rawRows.push_back(0);
        begin = end;
    }
    recovery = {
        {"entryCount",         source.entryCount                                                                                                                  },
        {"colorCount",         source.colorCount                                                                                                                  },
        {"decodedPoints",      points.size()                                                                                                                      },
        {"convertedEntries",   out.entries.size()                                                                                                                 },
        {"convertedColors",    out.colors.size()                                                                                                                  },
        {"defaultSourceColor", fallback                                                                                                                           },
        {"rowAxis",            0                                                                                                                                  },
        {"colAxis",            1                                                                                                                                  },
        {"conversion",         "legacy packed XYZ/visibility/trace/color entries and channel-major eight-corner colors converted using the stock linker contracts"}
    };
    return out;
}
