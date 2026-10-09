#include "ZoneDefinitionParserState.h"

#include <algorithm>

ZoneDefinitionParserState::ZoneDefinitionParserState(std::string targetName, ISearchPath& searchPath, IParserLineStream& underlyingStream)
    : m_search_path(searchPath),
      m_underlying_stream(underlyingStream),
      m_definition(std::make_unique<ZoneDefinition>())

{
    m_inclusions.emplace(targetName);
    m_definition->m_name = std::move(targetName);
}

namespace
{
    bool UpdateResolvedGame(ZoneDefinition& definition, std::optional<IGame*>& game)
    {
        const auto resolvedGame = definition.GetResolvedGameId();
        if (!resolvedGame)
        {
            game = std::nullopt;
            return false;
        }

        game = IGame::GetGameById(*resolvedGame);
        return true;
    }
} // namespace

bool ZoneDefinitionParserState::SetGame(const GameId gameId)
{
    m_definition->m_game = gameId;
    return UpdateResolvedGame(*m_definition, m_game);
}

bool ZoneDefinitionParserState::SetPlatform(const GamePlatform platform)
{
    m_definition->m_platform = platform;
    m_explicit_platform = platform;

    if (m_definition->m_game == GameId::COUNT)
        return true;

    return UpdateResolvedGame(*m_definition, m_game);
}

std::optional<GameId> ZoneDefinitionParserState::GetResolvedGameId() const
{
    return m_definition->GetResolvedGameId();
}

namespace
{
    void AddCurrentObjContainerToDefinitionIfNecessary(ZoneDefinition& zoneDefinition, std::optional<ZoneDefinitionObjContainer>& maybeObjContainer)
    {
        if (!maybeObjContainer)
            return;

        maybeObjContainer->m_asset_end = static_cast<unsigned>(zoneDefinition.m_assets.size());
        zoneDefinition.m_obj_containers.emplace_back(std::move(*maybeObjContainer));
        maybeObjContainer = std::nullopt;
    }

    ZoneDefinitionObjContainer DefineNewObjContainer(const ZoneDefinition& zoneDefinition, std::string name, const ZoneDefinitionObjContainerType type)
    {
        return ZoneDefinitionObjContainer(std::move(name), type, static_cast<unsigned>(zoneDefinition.m_assets.size()));
    }

    void SortObjContainer(ZoneDefinition& zoneDefinition)
    {
        std::ranges::sort(zoneDefinition.m_obj_containers,
                          [](const ZoneDefinitionObjContainer& obj0, const ZoneDefinitionObjContainer& obj1)
                          {
                              return obj0.m_asset_start < obj1.m_asset_start;
                          });
    }
} // namespace

void ZoneDefinitionParserState::StartIPak(std::string ipakName)
{
    AddCurrentObjContainerToDefinitionIfNecessary(*m_definition, m_current_ipak);
    m_current_ipak = DefineNewObjContainer(*m_definition, std::move(ipakName), ZoneDefinitionObjContainerType::IPAK);
}

void ZoneDefinitionParserState::StartIwd(std::string iwdName)
{
    AddCurrentObjContainerToDefinitionIfNecessary(*m_definition, m_current_iwd);
    m_current_iwd = DefineNewObjContainer(*m_definition, std::move(iwdName), ZoneDefinitionObjContainerType::IWD);
}

void ZoneDefinitionParserState::Finalize()
{
    AddCurrentObjContainerToDefinitionIfNecessary(*m_definition, m_current_ipak);
    AddCurrentObjContainerToDefinitionIfNecessary(*m_definition, m_current_iwd);

    SortObjContainer(*m_definition);
}
