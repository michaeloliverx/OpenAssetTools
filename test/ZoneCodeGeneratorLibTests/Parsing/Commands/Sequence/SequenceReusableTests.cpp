#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Parsing/Commands/Sequence/SequenceReusable.h"
#include "Parsing/Mock/MockLexer.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE("Reusable members can explicitly reference pointer aliases", "[parsing][sequence][reusable]")
{
    const auto alias = GENERATE(false, true);
    InMemoryRepository repository;
    auto definition = std::make_unique<StructDefinition>("", "Pass", 4);
    auto pointer = std::make_unique<TypeDeclaration>(BaseTypeDefinition::INT);
    pointer->m_declaration_modifiers.emplace_back(std::make_unique<PointerDeclarationModifier>());
    definition->m_members.emplace_back(std::make_shared<Variable>("decl", std::move(pointer)));
    const auto* rawDefinition = definition.get();
    repository.Add(std::move(definition));
    REQUIRE(CreateStructureInformationPostProcessor().PostProcess(&repository));
    REQUIRE(CreateMemberInformationPostProcessor().PostProcess(&repository));
    CommandsParserState state(&repository);
    const TokenPos pos;
    MockLexer<CommandsParserValue> lexer(
        {
            CommandsParserValue::Identifier(pos, new std::string("set")),
            CommandsParserValue::Identifier(pos, new std::string("reusable")),
            CommandsParserValue::Identifier(pos, new std::string("Pass")),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("decl")),
            alias ? CommandsParserValue::Identifier(pos, new std::string("alias")) : CommandsParserValue::Character(pos, ';'),
            alias ? CommandsParserValue::Character(pos, ';') : CommandsParserValue::EndOfFile(pos),
        },
        CommandsParserValue::EndOfFile(pos));
    unsigned consumed = 0;
    REQUIRE(SequenceReusable().MatchSequence(&lexer, &state, consumed));
    REQUIRE(consumed == (alias ? 8 : 7));
    const auto* info = repository.GetInformationFor(rawDefinition);
    REQUIRE(info->m_ordered_members.front()->m_is_reusable);
    REQUIRE(info->m_ordered_members.front()->m_is_alias == alias);
}
