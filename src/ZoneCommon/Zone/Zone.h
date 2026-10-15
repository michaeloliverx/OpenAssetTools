#pragma once

#include "Game/GameLanguage.h"
#include "Game/IGame.h"
#include "Pool/AssetPool.h"
#include "XAssetStreamFile.h"
#include "Zone/ZoneTypes.h"
#include "ZoneMemory.h"
#include "ZoneScriptStrings.h"

#include <memory>
#include <string>
#include <unordered_map>

class Zone
{
public:
    Zone(std::string name, zone_priority_t priority, GameId gameId, GamePlatform platform);
    ~Zone();
    Zone(const Zone& other) = delete;
    Zone(Zone&& other) noexcept = delete;
    Zone& operator=(const Zone& other) = delete;
    Zone& operator=(Zone&& other) noexcept = delete;

    void Register();

    [[nodiscard]] ZoneMemory& Memory() const;

    std::string m_name;
    std::string m_source_path;
    zone_priority_t m_priority;
    GameLanguage m_language;
    GameId m_game_id;
    GamePlatform m_platform;
    ZoneScriptStrings m_script_strings;
    ZoneAssetPools m_pools;

    std::vector<XAssetStreamFile> m_xfile_image_stream_files;
    std::size_t m_xfile_image_stream_read_index = 0u;
    std::unordered_map<std::string, std::vector<XAssetStreamFilesForImage>> m_xfile_image_stream_files_by_name;
    std::vector<unsigned char> m_xfile_image_stream_data;

private:
    std::unique_ptr<ZoneMemory> m_memory;

    bool m_registered;
};
