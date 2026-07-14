#include "MaterialRawDumperIW3.h"

#include "Game/IW3/CommonIW3.h"
#include "Game/IW3/Material/MaterialConstantZoneStateIW3.h"
#include "Game/IW3/Techset/TechsetConstantsIW3.h"
#include "Techset/StateMap/StateMapHandler.h"
#include "Techset/StateMap/StateMapReader.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace IW3;

namespace
{
    constexpr auto MATERIAL_TYPE_COUNT = 34u;

#pragma pack(push, 1)

    struct MaterialInfoRaw
    {
        uint32_t nameOffset;
        uint32_t refImageNameOffset;
        uint8_t gameFlags;
        uint8_t sortKey;
        uint8_t textureAtlasRowCount;
        uint8_t textureAtlasColumnCount;
        float maxDeformMove;
        uint8_t deformFlags;
        uint8_t usage;
        uint16_t toolFlags;
        uint32_t locale;
        uint16_t autoTexScaleWidth;
        uint16_t autoTexScaleHeight;
        float tessSize;
        int32_t surfaceFlags;
        int32_t contents;
    };

    struct MaterialRaw
    {
        MaterialInfoRaw info;
        uint32_t refStateBits[2];
        uint16_t textureCount;
        uint16_t constantCount;
        uint32_t techSetNameOffset;
        uint32_t textureTableOffset;
        uint32_t constantTableOffset;
    };

    struct MaterialTextureDefRaw
    {
        uint32_t nameOffset;
        uint8_t samplerState;
        uint8_t semantic;
        uint16_t padding;
        uint32_t imageNameOffset;
    };

    struct MaterialConstantDefRaw
    {
        uint32_t nameOffset;
        float literal[4];
    };

    struct MaterialWaterDefRaw
    {
        int32_t textureWidth;
        float horizontalWorldLength;
        float verticalWorldLength;
        float amplitude;
        float windSpeed;
        float windDirection[2];
        uint32_t map;
    };

#pragma pack(pop)

    static_assert(sizeof(MaterialInfoRaw) == 0x28);
    static_assert(sizeof(MaterialRaw) == 0x40);
    static_assert(sizeof(MaterialTextureDefRaw) == 0xC);
    static_assert(sizeof(MaterialConstantDefRaw) == 0x14);
    static_assert(sizeof(MaterialWaterDefRaw) == 0x20);
    static_assert(offsetof(MaterialRaw, refStateBits) == 0x28);
    static_assert(offsetof(MaterialRaw, textureTableOffset) == 0x38);

    struct StateCandidate
    {
        uint32_t stateBits[2];
        uint16_t toolFlags;
    };

    using TechniqueNames = std::array<std::string, MATERIAL_TYPE_COUNT>;
    using StateBits = std::array<uint32_t, 2>;

    std::string_view StripReferencePrefix(const std::string_view value)
    {
        return !value.empty() && value.front() == ',' ? value.substr(1) : value;
    }

    std::string_view StripMaterialTypePrefix(const std::string_view name)
    {
        constexpr std::array<std::string_view, 4> prefixes{"mc/", "wc/", "m/", "w/"};
        for (const auto prefix : prefixes)
        {
            if (name.starts_with(prefix))
                return name.substr(prefix.size());
        }

        return name;
    }

    std::string_view TechniqueSetPrefixForMaterial(const std::string_view name)
    {
        if (name.starts_with("mc/"))
            return "mc_";
        if (name.starts_with("wc/"))
            return "wc_";
        if (name.starts_with("m/"))
            return "m_";
        if (name.starts_with("w/"))
            return "w_";
        return {};
    }

    std::string GetRawTechniqueSetName(const std::string_view materialName, const std::string_view techniqueSetName)
    {
        auto normalizedName = StripReferencePrefix(techniqueSetName);
        const auto prefix = TechniqueSetPrefixForMaterial(materialName);
        if (!prefix.empty() && normalizedName.starts_with(prefix))
            normalizedName.remove_prefix(prefix.size());
        return std::string(normalizedName);
    }

    void RemoveCarriageReturn(std::string& value)
    {
        if (!value.empty() && value.back() == '\r')
            value.pop_back();
    }

    std::vector<std::string> Split(const std::string_view value, const char delimiter)
    {
        std::vector<std::string> result;
        auto start = 0uz;
        while (start <= value.size())
        {
            const auto end = value.find(delimiter, start);
            if (end == std::string_view::npos)
            {
                result.emplace_back(value.substr(start));
                break;
            }

            result.emplace_back(value.substr(start, end - start));
            start = end + 1;
        }
        return result;
    }

    bool ParseHex(const std::string_view value, uint32_t& result)
    {
        const auto* first = value.data();
        const auto* last = first + value.size();
        const auto conversion = std::from_chars(first, last, result, 16);
        return conversion.ec == std::errc() && conversion.ptr == last;
    }

    class MaterialRawDumperStateIW3 final : public IZoneAssetDumperState
    {
    public:
        bool EnsureInitialized(ISearchPath& searchPath)
        {
            if (m_initialized)
                return m_valid;

            m_initialized = true;
            m_search_path = &searchPath;
            m_valid = LoadTechniqueStateMaps() && LoadTechniqueSets() && LoadStateCandidates();
            if (m_valid)
            {
                con::info("Loaded IW3 raw material metadata: {} techniques, {} techsets, {} state candidates",
                          m_technique_state_maps.size(),
                          m_technique_sets.size(),
                          m_state_candidates.size());
            }
            return m_valid;
        }

        bool FindStateCandidate(const Material& material, StateCandidate& result)
        {
            if (!m_valid || !material.techniqueSet || !material.techniqueSet->name)
                return false;

            TechniqueNames techniqueNames{};
            if (!GetTechniqueNames(material, techniqueNames))
                return false;

            auto foundCandidate = false;
            auto bestScore = std::numeric_limits<unsigned>::max();
            auto closestMismatchScore = std::numeric_limits<unsigned>::max();
            StateCandidate closestCandidate{};
            std::array<uint8_t, MATERIAL_TYPE_COUNT> closestEntries{};
            std::vector<StateBits> closestTable;
            for (const auto& candidate : m_state_candidates)
            {
                std::array<uint8_t, MATERIAL_TYPE_COUNT> entries{};
                std::vector<StateBits> table;
                if (!BuildStateBitsTable(techniqueNames, candidate, entries, table))
                    return false;

                const auto mismatchScore = StateTableMismatchScore(material, entries, table);
                if (mismatchScore < closestMismatchScore)
                {
                    closestMismatchScore = mismatchScore;
                    closestCandidate = candidate;
                    closestEntries = entries;
                    closestTable = table;
                }

                if (MatchesMaterial(material, entries, table))
                {
                    const auto score = SourceStateDistance(material, candidate);
                    if (!foundCandidate || score < bestScore)
                    {
                        foundCandidate = true;
                        bestScore = score;
                        result = candidate;
                    }
                }
            }

            if (!foundCandidate)
                LogFailedInversion(material, closestCandidate, closestEntries, closestTable);
            return foundCandidate;
        }

    private:
        bool OpenCsv(const std::string& fileName, SearchPathOpenFile& file) const
        {
            file = m_search_path->Open(fileName);
            if (file.IsOpen())
                return true;

            con::error("Could not find bundled IW3 raw material metadata file \"{}\"", fileName);
            return false;
        }

        bool LoadTechniqueStateMaps()
        {
            SearchPathOpenFile file;
            if (!OpenCsv("material_technique_state_maps.csv", file))
                return false;

            std::string line;
            std::getline(*file.m_stream, line);
            while (std::getline(*file.m_stream, line))
            {
                RemoveCarriageReturn(line);
                const auto separator = line.find(',');
                if (separator == std::string::npos || separator == 0u || separator + 1u >= line.size())
                    return false;

                m_technique_state_maps.emplace(line.substr(0, separator), Split(std::string_view(line).substr(separator + 1u), ';'));
            }

            return !m_technique_state_maps.empty();
        }

        bool LoadTechniqueSets()
        {
            SearchPathOpenFile file;
            if (!OpenCsv("material_techsets.csv", file))
                return false;

            std::string line;
            std::getline(*file.m_stream, line);
            while (std::getline(*file.m_stream, line))
            {
                RemoveCarriageReturn(line);
                const auto separator = line.find(',');
                if (separator == std::string::npos || separator == 0u)
                    return false;

                const auto values = Split(std::string_view(line).substr(separator + 1u), ';');
                if (values.size() != MATERIAL_TYPE_COUNT)
                    return false;

                TechniqueNames techniques{};
                std::copy(values.begin(), values.end(), techniques.begin());
                m_technique_sets.emplace(line.substr(0, separator), std::move(techniques));
            }

            return !m_technique_sets.empty();
        }

        bool LoadStateCandidates()
        {
            SearchPathOpenFile file;
            if (!OpenCsv("material_state_candidates.csv", file))
                return false;

            std::string line;
            std::getline(*file.m_stream, line);
            while (std::getline(*file.m_stream, line))
            {
                RemoveCarriageReturn(line);
                const auto values = Split(line, ',');
                if (values.size() < 3u)
                    return false;

                StateCandidate candidate{};
                uint32_t toolFlags;
                if (!ParseHex(values[0], candidate.stateBits[0]) || !ParseHex(values[1], candidate.stateBits[1]) || !ParseHex(values[2], toolFlags)
                    || toolFlags > std::numeric_limits<uint16_t>::max())
                {
                    return false;
                }

                candidate.toolFlags = static_cast<uint16_t>(toolFlags);
                m_state_candidates.emplace_back(candidate);
            }

            return !m_state_candidates.empty();
        }

        bool GetTechniqueNames(const Material& material, TechniqueNames& result) const
        {
            const auto& techniqueSet = *material.techniqueSet;
            const auto isReferenceOnly = techniqueSet.name[0] == ',';
            const auto normalizedName = std::string(StripReferencePrefix(techniqueSet.name));
            const auto foundSet = m_technique_sets.find(normalizedName);
            if (foundSet != m_technique_sets.end())
            {
                result = foundSet->second;

                // Reference-only technique sets contain no technique pointers in the
                // current zone. Their complete layout is provided by the bundled
                // Mod Tools manifest instead. The compiled entry mask identifies
                // optional technique slots that were pruned for this fastfile.
                if (isReferenceOnly)
                {
                    for (auto techniqueIndex = 0u; techniqueIndex < MATERIAL_TYPE_COUNT; techniqueIndex++)
                    {
                        if (static_cast<uint8_t>(material.stateBitsEntry[techniqueIndex]) == std::numeric_limits<uint8_t>::max())
                            result[techniqueIndex].clear();
                        else if (result[techniqueIndex].empty())
                            return false;
                    }
                    return true;
                }
            }

            for (auto techniqueIndex = 0u; techniqueIndex < MATERIAL_TYPE_COUNT; techniqueIndex++)
            {
                const auto* technique = techniqueSet.techniques[techniqueIndex];
                if (!technique)
                {
                    result[techniqueIndex].clear();
                    continue;
                }

                if (result[techniqueIndex].empty())
                {
                    if (!technique->name)
                        return false;
                    result[techniqueIndex] = StripReferencePrefix(technique->name);
                }

                const auto foundTechnique = m_technique_state_maps.find(result[techniqueIndex]);
                if (foundTechnique == m_technique_state_maps.end() || foundTechnique->second.size() != technique->passCount)
                {
                    con::debug("IW3 technique metadata mismatch for \"{}\" at type {}: metadata passes={}, compiled passes={}",
                               result[techniqueIndex],
                               techniqueIndex,
                               foundTechnique == m_technique_state_maps.end() ? 0uz : foundTechnique->second.size(),
                               technique->passCount);
                    return false;
                }
            }

            return true;
        }

        const state_map::StateMapDefinition* GetStateMap(const std::string& name)
        {
            const auto existing = m_state_maps.find(name);
            if (existing != m_state_maps.end())
                return existing->second.get();

            auto file = m_search_path->Open(std::string("statemaps/") + name + ".sm");
            if (!file.IsOpen())
            {
                con::error("Could not find IW3 state map \"{}\"", name);
                m_state_maps.emplace(name, nullptr);
                return nullptr;
            }

            state_map::StateMapReader reader(*file.m_stream, std::string("statemaps/") + name + ".sm", name, stateMapLayout);
            auto definition = reader.ReadStateMapDefinition();
            const auto* result = definition.get();
            m_state_maps.emplace(name, std::move(definition));
            return result;
        }

        static uint8_t AddStateBitsSequence(const std::vector<StateBits>& sequence, std::vector<StateBits>& table)
        {
            auto scan = 0uz;
            auto partialMatchCount = 0uz;
            for (; scan < table.size(); scan++)
            {
                partialMatchCount = std::min(table.size() - scan, sequence.size());
                if (std::equal(sequence.begin(), sequence.begin() + partialMatchCount, table.begin() + scan))
                    break;
            }

            if (scan == table.size())
                partialMatchCount = 0u;

            table.insert(table.end(), sequence.begin() + partialMatchCount, sequence.end());
            return static_cast<uint8_t>(scan);
        }

        bool BuildStateBitsTable(const TechniqueNames& techniqueNames,
                                 const StateCandidate& candidate,
                                 std::array<uint8_t, MATERIAL_TYPE_COUNT>& entries,
                                 std::vector<StateBits>& table)
        {
            entries.fill(std::numeric_limits<uint8_t>::max());
            for (auto techniqueIndex = 0u; techniqueIndex < MATERIAL_TYPE_COUNT; techniqueIndex++)
            {
                const auto& techniqueName = techniqueNames[techniqueIndex];
                if (techniqueName.empty())
                    continue;

                const auto foundTechnique = m_technique_state_maps.find(techniqueName);
                if (foundTechnique == m_technique_state_maps.end())
                    return false;

                std::vector<StateBits> sequence;
                sequence.reserve(foundTechnique->second.size());
                for (const auto& stateMapName : foundTechnique->second)
                {
                    const auto* stateMap = GetStateMap(stateMapName);
                    if (!stateMap)
                        return false;

                    StateBits remapped{};
                    state_map::StateMapHandler(stateMapLayout, *stateMap).ApplyStateMap(candidate.stateBits, remapped.data());
                    if ((candidate.toolFlags & 0x200u) == 0u && (remapped[1] & 0x30u) == 0x10u)
                        remapped[1] &= ~0x30u;
                    sequence.emplace_back(remapped);
                }

                entries[techniqueIndex] = AddStateBitsSequence(sequence, table);
            }

            return true;
        }

        static bool MatchesMaterial(const Material& material, const std::array<uint8_t, MATERIAL_TYPE_COUNT>& entries, const std::vector<StateBits>& table)
        {
            if (material.stateBitsCount != table.size())
                return false;

            for (auto techniqueIndex = 0u; techniqueIndex < MATERIAL_TYPE_COUNT; techniqueIndex++)
            {
                if (static_cast<uint8_t>(material.stateBitsEntry[techniqueIndex]) != entries[techniqueIndex])
                    return false;
            }

            if (!table.empty() && !material.stateBitsTable)
                return false;

            for (auto stateIndex = 0uz; stateIndex < table.size(); stateIndex++)
            {
                if (material.stateBitsTable[stateIndex].loadBits.raw[0] != table[stateIndex][0]
                    || material.stateBitsTable[stateIndex].loadBits.raw[1] != table[stateIndex][1])
                {
                    return false;
                }
            }

            return true;
        }

        static unsigned SourceStateDistance(const Material& material, const StateCandidate& candidate)
        {
            auto result = std::numeric_limits<unsigned>::max();
            for (auto stateIndex = 0u; stateIndex < material.stateBitsCount; stateIndex++)
            {
                const auto distance = static_cast<unsigned>(std::popcount(candidate.stateBits[0] ^ material.stateBitsTable[stateIndex].loadBits.raw[0])
                                                            + std::popcount(candidate.stateBits[1] ^ material.stateBitsTable[stateIndex].loadBits.raw[1]));
                result = std::min(result, distance);
            }
            return result;
        }

        static unsigned
            StateTableMismatchScore(const Material& material, const std::array<uint8_t, MATERIAL_TYPE_COUNT>& entries, const std::vector<StateBits>& table)
        {
            auto result =
                static_cast<unsigned>(std::max<size_t>(material.stateBitsCount, table.size()) - std::min<size_t>(material.stateBitsCount, table.size())) * 128u;
            for (auto techniqueIndex = 0u; techniqueIndex < MATERIAL_TYPE_COUNT; techniqueIndex++)
            {
                if (static_cast<uint8_t>(material.stateBitsEntry[techniqueIndex]) != entries[techniqueIndex])
                    result += 64u;
            }

            const auto sharedStateCount = std::min<size_t>(material.stateBitsCount, table.size());
            for (auto stateIndex = 0uz; stateIndex < sharedStateCount; stateIndex++)
            {
                result += static_cast<unsigned>(std::popcount(material.stateBitsTable[stateIndex].loadBits.raw[0] ^ table[stateIndex][0])
                                                + std::popcount(material.stateBitsTable[stateIndex].loadBits.raw[1] ^ table[stateIndex][1]));
            }
            return result;
        }

        static std::string StateTableToString(const std::vector<StateBits>& table)
        {
            std::string result;
            for (const auto& state : table)
                result += std::format(" [{:08X},{:08X}]", state[0], state[1]);
            return result;
        }

        static std::string StateTableToString(const Material& material)
        {
            std::string result;
            for (auto stateIndex = 0u; stateIndex < material.stateBitsCount; stateIndex++)
            {
                result +=
                    std::format(" [{:08X},{:08X}]", material.stateBitsTable[stateIndex].loadBits.raw[0], material.stateBitsTable[stateIndex].loadBits.raw[1]);
            }
            return result;
        }

        static void LogFailedInversion(const Material& material,
                                       const StateCandidate& candidate,
                                       const std::array<uint8_t, MATERIAL_TYPE_COUNT>&,
                                       const std::vector<StateBits>& table)
        {
            con::debug("Failed IW3 state inversion for techset \"{}\"; closest source was [{:08X},{:08X}] toolFlags=0x{:04X}",
                       material.techniqueSet->name,
                       candidate.stateBits[0],
                       candidate.stateBits[1],
                       candidate.toolFlags);
            con::debug("Compiled state table:{}", StateTableToString(material));
            con::debug("Closest generated table:{}", StateTableToString(table));
        }

        bool m_initialized = false;
        bool m_valid = false;
        ISearchPath* m_search_path = nullptr;
        std::unordered_map<std::string, std::vector<std::string>> m_technique_state_maps;
        std::unordered_map<std::string, TechniqueNames> m_technique_sets;
        std::vector<StateCandidate> m_state_candidates;
        std::unordered_map<std::string, std::unique_ptr<state_map::StateMapDefinition>> m_state_maps;
    };

    class OutputBuffer
    {
    public:
        template<typename T> uint32_t Append(const T& value)
        {
            const auto offset = CheckedOffset();
            const auto oldSize = m_data.size();
            m_data.resize(oldSize + sizeof(T));
            std::memcpy(m_data.data() + oldSize, &value, sizeof(T));
            return offset;
        }

        template<typename T> uint32_t AppendZeroed(const size_t count)
        {
            const auto offset = CheckedOffset();
            m_data.resize(m_data.size() + sizeof(T) * count, 0u);
            return offset;
        }

        template<typename T> void Write(const uint32_t offset, const T& value)
        {
            std::memcpy(m_data.data() + offset, &value, sizeof(T));
        }

        uint32_t AddString(const std::string& value)
        {
            const auto existing = m_string_offsets.find(value);
            if (existing != m_string_offsets.end())
                return existing->second;

            const auto offset = CheckedOffset();
            m_data.insert(m_data.end(), value.begin(), value.end());
            m_data.emplace_back(0u);
            m_string_offsets.emplace(value, offset);
            return offset;
        }

        [[nodiscard]] const std::vector<uint8_t>& Data() const
        {
            return m_data;
        }

    private:
        [[nodiscard]] uint32_t CheckedOffset() const
        {
            return static_cast<uint32_t>(m_data.size());
        }

        std::vector<uint8_t> m_data;
        std::unordered_map<std::string, uint32_t> m_string_offsets;
    };

    struct TextureSource
    {
        std::string name;
        std::string imageName;
        const MaterialTextureDef* texture;
    };

    bool ResolveTextureName(const MaterialTextureDef& texture, const MaterialConstantZoneState& constantState, std::string& result)
    {
        if (!constantState.GetTextureDefName(texture.nameHash, result))
            return false;
        return Common::R_HashString(result.c_str(), 0u) == texture.nameHash;
    }

    bool ResolveConstantName(const MaterialConstantDef& constant, const MaterialConstantZoneState& constantState, std::string& result)
    {
        const auto fragmentLength = strnlen(constant.name, std::extent_v<decltype(MaterialConstantDef::name)>);
        const std::string fragment(constant.name, fragmentLength);
        if (fragmentLength < std::extent_v<decltype(MaterialConstantDef::name)> || Common::R_HashString(fragment.c_str(), 0u) == constant.nameHash)
        {
            result = fragment;
            return true;
        }

        return constantState.GetConstantName(constant.nameHash, result) && Common::R_HashString(result.c_str(), 0u) == constant.nameHash;
    }

    int32_t GetSurfaceFlags(const Material& material)
    {
        uint32_t result = 0u;
        if (material.info.surfaceTypeBits)
        {
            auto surfaceIndex = 0u;
            auto bits = material.info.surfaceTypeBits;
            while ((bits & 1u) == 0u)
            {
                surfaceIndex++;
                bits >>= 1u;
            }
            result |= (surfaceIndex + 1u) << 20u;
        }

        if ((material.info.gameFlags & MTL_GAMEFLAG_CASTS_SHADOW) == 0u && material.techniqueSet
            && material.techniqueSet->techniques[TECHNIQUE_BUILD_SHADOWMAP_DEPTH] && (material.stateFlags & STATE_FLAG_DECAL) == 0u)
        {
            result |= 0x40000u;
        }
        return static_cast<int32_t>(result);
    }

    bool BuildRawMaterial(const XAssetInfo<AssetMaterial::Type>& asset,
                          const MaterialConstantZoneState& constantState,
                          MaterialRawDumperStateIW3& rawState,
                          std::vector<uint8_t>& result)
    {
        const auto& material = *asset.Asset();
        if (!material.techniqueSet || !material.techniqueSet->name)
        {
            con::warn("Cannot dump IW3 raw material \"{}\": material has no technique set", asset.m_name);
            return false;
        }

        StateCandidate candidate{};
        if (!rawState.FindStateCandidate(material, candidate))
        {
            con::warn("Cannot dump IW3 raw material \"{}\": no source state bits reproduce the compiled state table", asset.m_name);
            return false;
        }

        std::vector<TextureSource> textures;
        textures.reserve(material.textureCount);
        const GfxImage* referenceImage = nullptr;
        for (auto textureIndex = 0u; textureIndex < material.textureCount; textureIndex++)
        {
            const auto& texture = material.textureTable[textureIndex];
            TextureSource source{.texture = &texture};
            if (!ResolveTextureName(texture, constantState, source.name))
            {
                con::warn("Cannot dump IW3 raw material \"{}\": unresolved texture binding hash 0x{:08X}", asset.m_name, texture.nameHash);
                return false;
            }

            if (texture.semantic == TS_WATER_MAP)
            {
                if (!texture.u.water)
                {
                    con::warn("Cannot dump IW3 raw material \"{}\": water texture \"{}\" has no water definition", asset.m_name, source.name);
                    return false;
                }
            }
            else
            {
                if (!texture.u.image || !texture.u.image->name)
                {
                    con::warn("Cannot dump IW3 raw material \"{}\": texture \"{}\" has no image", asset.m_name, source.name);
                    return false;
                }
                source.imageName = StripReferencePrefix(texture.u.image->name);
                if (!referenceImage)
                    referenceImage = texture.u.image;
            }

            textures.emplace_back(std::move(source));
        }

        std::vector<std::string> constantNames(material.constantCount);
        for (auto constantIndex = 0u; constantIndex < material.constantCount; constantIndex++)
        {
            const auto& constant = material.constantTable[constantIndex];
            if (!ResolveConstantName(constant, constantState, constantNames[constantIndex]))
            {
                con::warn("Cannot dump IW3 raw material \"{}\": unresolved constant hash 0x{:08X}", asset.m_name, constant.nameHash);
                return false;
            }
        }

        OutputBuffer output;
        const auto headerOffset = output.Append(MaterialRaw{});
        const auto textureTableOffset = output.AppendZeroed<MaterialTextureDefRaw>(textures.size());

        std::vector<uint32_t> waterOffsets(textures.size());
        for (auto textureIndex = 0uz; textureIndex < textures.size(); textureIndex++)
        {
            if (textures[textureIndex].texture->semantic != TS_WATER_MAP)
                continue;

            const auto& water = *textures[textureIndex].texture->u.water;
            MaterialWaterDefRaw rawWater{};
            rawWater.textureWidth = water.M;
            rawWater.horizontalWorldLength = water.Lx;
            rawWater.verticalWorldLength = water.Lz;
            rawWater.amplitude = water.amplitude;
            rawWater.windSpeed = water.windvel;
            rawWater.windDirection[0] = water.winddir[0];
            rawWater.windDirection[1] = water.winddir[1];
            waterOffsets[textureIndex] = output.Append(rawWater);
        }

        const auto constantTableOffset = output.AppendZeroed<MaterialConstantDefRaw>(material.constantCount);
        const auto materialName = std::string(StripMaterialTypePrefix(asset.m_name));
        const auto techniqueSetName = GetRawTechniqueSetName(asset.m_name, material.techniqueSet->name);
        const auto referenceImageName = referenceImage ? std::string(StripReferencePrefix(referenceImage->name)) : std::string("default");

        MaterialRaw header{};
        header.techSetNameOffset = output.AddString(techniqueSetName);
        header.info.nameOffset = output.AddString(materialName);
        header.info.refImageNameOffset = output.AddString(referenceImageName);
        header.info.gameFlags = material.info.gameFlags & ~MTL_GAMEFLAG_CASTS_SHADOW;
        header.info.sortKey = material.info.sortKey;
        header.info.textureAtlasRowCount = material.info.textureAtlasRowCount;
        header.info.textureAtlasColumnCount = material.info.textureAtlasColumnCount;
        header.info.toolFlags = candidate.toolFlags;
        header.info.autoTexScaleWidth = referenceImage ? referenceImage->width : 1u;
        header.info.autoTexScaleHeight = referenceImage ? referenceImage->height : 1u;
        header.info.surfaceFlags = GetSurfaceFlags(material);
        header.info.contents = 1;
        header.refStateBits[0] = candidate.stateBits[0];
        header.refStateBits[1] = candidate.stateBits[1];
        header.textureCount = material.textureCount;
        header.constantCount = material.constantCount;
        header.textureTableOffset = textureTableOffset;
        header.constantTableOffset = constantTableOffset;

        for (auto textureIndex = 0uz; textureIndex < textures.size(); textureIndex++)
        {
            const auto& source = textures[textureIndex];
            MaterialTextureDefRaw rawTexture{};
            rawTexture.nameOffset = output.AddString(source.name);
            std::memcpy(&rawTexture.samplerState, &source.texture->samplerState, sizeof(rawTexture.samplerState));
            rawTexture.semantic = source.texture->semantic;
            rawTexture.imageNameOffset = source.texture->semantic == TS_WATER_MAP ? waterOffsets[textureIndex] : output.AddString(source.imageName);
            output.Write(textureTableOffset + static_cast<uint32_t>(textureIndex * sizeof(MaterialTextureDefRaw)), rawTexture);
        }

        for (auto constantIndex = 0u; constantIndex < material.constantCount; constantIndex++)
        {
            const auto& source = material.constantTable[constantIndex];
            MaterialConstantDefRaw rawConstant{};
            rawConstant.nameOffset = output.AddString(constantNames[constantIndex]);
            rawConstant.literal[0] = source.literal.x;
            rawConstant.literal[1] = source.literal.y;
            rawConstant.literal[2] = source.literal.z;
            rawConstant.literal[3] = source.literal.w;
            output.Write(constantTableOffset + constantIndex * sizeof(MaterialConstantDefRaw), rawConstant);
        }

        output.Write(headerOffset, header);
        result = output.Data();
        return true;
    }
} // namespace

namespace material
{
    void RawDumperIW3::Dump(AssetDumpingContext& context)
    {
        context.GetZoneAssetDumperState<MaterialConstantZoneState>()->EnsureInitialized();
        context.GetZoneAssetDumperState<MaterialRawDumperStateIW3>()->EnsureInitialized(context.m_obj_search_path);
        AbstractAssetDumper::Dump(context);
    }

    bool RawDumperIW3::ShouldDump(const XAssetInfo<AssetMaterial::Type>& asset)
    {
        return !asset.m_name.empty() && asset.m_name.front() != '*';
    }

    void RawDumperIW3::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMaterial::Type>& asset)
    {
        std::vector<uint8_t> data;
        if (!BuildRawMaterial(
                asset, *context.GetZoneAssetDumperState<MaterialConstantZoneState>(), *context.GetZoneAssetDumperState<MaterialRawDumperStateIW3>(), data))
        {
            return;
        }

        const auto assetFile = context.OpenAssetFile(std::string("materials/") + std::string(StripMaterialTypePrefix(asset.m_name)));
        if (!assetFile)
            return;

        assetFile->write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    }
} // namespace material
