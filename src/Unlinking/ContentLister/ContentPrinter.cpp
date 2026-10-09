#include "ContentPrinter.h"

#include "Game/GameTarget.h"
#include "Utils/Logging/Log.h"

#include <format>
#include <iostream>

ContentPrinter::ContentPrinter(const Zone& zone)
    : m_zone(zone)
{
}

void ContentPrinter::PrintContent() const
{
    const auto& pools = m_zone.m_pools;
    const auto* game = IGame::GetGameById(m_zone.m_game_id);
    con::info("Zone '{}' ({})", m_zone.m_name, game_target::GetDisplayName(m_zone.m_game_id, m_zone.m_platform));
    con::info("Content:");

    for (const auto& asset : pools)
        con::info("{}, {}", *game->GetAssetTypeName(asset->m_type), asset->m_name);

    con::info("");
}
