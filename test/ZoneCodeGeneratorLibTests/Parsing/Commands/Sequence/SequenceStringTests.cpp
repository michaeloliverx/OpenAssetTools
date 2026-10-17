#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Parsing/Commands/Sequence/SequenceString.h"
#include "Parsing/Mock/MockLexer.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE("String members can use indexed storage", "[parsing][sequence][indexedstrings]")
{
    const auto indexed = GENERATE(false, true);
    InMemoryRepository repository;
    auto definition = std::make_unique<StructDefinition>("", "Entry", 4);
    auto pointer = std::make_unique<TypeDeclaration>(BaseTypeDefinition::CHAR);
    pointer->m_declaration_modifiers.emplace_back(std::make_unique<PointerDeclarationModifier>());
    definition->m_members.emplace_back(std::make_shared<Variable>("name", std::move(pointer)));
    const auto* rawDefinition = definition.get();
    repository.Add(std::move(definition));
    REQUIRE(CreateStructureInformationPostProcessor().PostProcess(&repository));
    REQUIRE(CreateMemberInformationPostProcessor().PostProcess(&repository));
    CommandsParserState state(&repository);
    const TokenPos pos;
    MockLexer<CommandsParserValue> lexer(
        {
            CommandsParserValue::Identifier(pos, new std::string("set")),
            CommandsParserValue::Identifier(pos, new std::string("string")),
            CommandsParserValue::Identifier(pos, new std::string("Entry")),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("name")),
            indexed ? CommandsParserValue::Identifier(pos, new std::string("indexed")) : CommandsParserValue::Character(pos, ';'),
            indexed ? CommandsParserValue::Character(pos, ';') : CommandsParserValue::EndOfFile(pos),
        },
        CommandsParserValue::EndOfFile(pos));
    unsigned consumed = 0;
    REQUIRE(SequenceString().MatchSequence(&lexer, &state, consumed));
    REQUIRE(consumed == (indexed ? 8 : 7));
    const auto* info = repository.GetInformationFor(rawDefinition);
    REQUIRE(info->m_ordered_members.front()->m_is_string);
    REQUIRE(info->m_ordered_members.front()->m_is_indexed_string == indexed);
}
