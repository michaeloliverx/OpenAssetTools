#include "ZoneDefinitionParser.h"

#include "Game/GameTarget.h"
#include "Sequence/SequenceZoneDefinitionAssetList.h"
#include "Sequence/SequenceZoneDefinitionBuild.h"
#include "Sequence/SequenceZoneDefinitionEntry.h"
#include "Sequence/SequenceZoneDefinitionIgnore.h"
#include "Sequence/SequenceZoneDefinitionInclude.h"
#include "Sequence/SequenceZoneDefinitionMetaData.h"

namespace
{
    GamePlatform GetDefaultPlatform(const GameId game)
    {
        switch (game)
        {
        case GameId::IW3Xenon:
        case GameId::T4Xenon:
        case GameId::IW4Xenon:
        case GameId::QOSXenon:
        case GameId::T5Xenon:
        case GameId::NX1:
            return GamePlatform::XBOX;

        default:
            return GamePlatform::PC;
        }
    }
} // namespace

ZoneDefinitionParser::ZoneDefinitionParser(
    ZoneDefinitionLexer* lexer, std::string targetName, ISearchPath& searchPath, IParserLineStream& underlyingStream, const std::optional<GameId> maybeGame)
    : AbstractParser(lexer, std::make_unique<ZoneDefinitionParserState>(std::move(targetName), searchPath, underlyingStream))
{
    if (maybeGame)
    {
        (void)m_state->SetPlatform(GetDefaultPlatform(*maybeGame));
        (void)m_state->SetGame(game_target::GetPublicGameId(*maybeGame));
    }
}

const std::vector<AbstractParser<ZoneDefinitionParserValue, ZoneDefinitionParserState>::sequence_t*>& ZoneDefinitionParser::GetTestsForState()
{
    static std::vector<sequence_t*> tests({
        new SequenceZoneDefinitionMetaData(),
        new SequenceZoneDefinitionInclude(),
        new SequenceZoneDefinitionIgnore(),
        new SequenceZoneDefinitionAssetList(),
        new SequenceZoneDefinitionBuild(),
        new SequenceZoneDefinitionEntry(),
    });

    return tests;
}

std::unique_ptr<ZoneDefinition> ZoneDefinitionParser::GetParsedValue()
{
    m_state->Finalize();
    return std::move(m_state->m_definition);
}
