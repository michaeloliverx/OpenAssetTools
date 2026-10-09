#include "Domain/Definition/ArrayDeclarationModifier.h"
#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Domain/Evaluation/OperandDynamic.h"
#include "Domain/Evaluation/OperandStatic.h"
#include "Parsing/Commands/Sequence/SequenceCount.h"
#include "Parsing/Mock/MockLexer.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <catch2/catch_test_macros.hpp>

namespace test::parsing::commands::sequence::sequence_count
{
    class CommandsSequenceTestsHelper
    {
    public:
        std::unique_ptr<IDataRepository> m_repository;
        std::unique_ptr<CommandsParserState> m_state;
        std::unique_ptr<ILexer<CommandsParserValue>> m_lexer;

        StructDefinition* m_lod_info_t_raw;
        StructureInformation* m_lod_info_t;

        StructDefinition* m_xmodel_raw;
        StructureInformation* m_xmodel;

        StructDefinition* m_xmodel_stream_info_raw;
        StructureInformation* m_xmodel_stream_info;
        MemberInformation* m_high_mip_bounds_member;

        unsigned m_consumed_token_count;

    private:
        [[nodiscard]] bool CreateInformation() const
        {
            auto createStructureInformation = std::make_unique<CreateStructureInformationPostProcessor>();
            auto createMemberInformation = std::make_unique<CreateMemberInformationPostProcessor>();

            return createStructureInformation->PostProcess(m_repository.get()) && createMemberInformation->PostProcess(m_repository.get());
        }

        void AddSampleData()
        {
            auto def = std::make_unique<StructDefinition>("", "lod_info_t", 8);
            def->m_members.emplace_back(std::make_shared<Variable>("numsurfs", std::make_unique<TypeDeclaration>(BaseTypeDefinition::INT)));
            m_lod_info_t_raw = def.get();
            m_repository->Add(std::move(def));

            def = std::make_unique<StructDefinition>("", "XModel", 8);
            auto lodInfoType = std::make_unique<TypeDeclaration>(m_lod_info_t_raw);
            lodInfoType->m_declaration_modifiers.emplace_back(std::make_unique<ArrayDeclarationModifier>(4));
            def->m_members.emplace_back(std::make_shared<Variable>("lodInfo", std::move(lodInfoType)));
            m_xmodel_raw = def.get();
            m_repository->Add(std::move(def));

            def = std::make_unique<StructDefinition>("", "XModelStreamInfo", 8);
            auto highMipBoundsType = std::make_unique<TypeDeclaration>(BaseTypeDefinition::UNSIGNED_CHAR);
            highMipBoundsType->m_declaration_modifiers.emplace_back(std::make_unique<PointerDeclarationModifier>());
            def->m_members.emplace_back(std::make_shared<Variable>("highMipBounds", std::move(highMipBoundsType)));
            m_xmodel_stream_info_raw = def.get();
            m_repository->Add(std::move(def));
        }

        void RetrieveInformationPointers()
        {
            m_lod_info_t = m_repository->GetInformationFor(m_lod_info_t_raw);
            REQUIRE(m_lod_info_t != nullptr);

            m_xmodel = m_repository->GetInformationFor(m_xmodel_raw);
            REQUIRE(m_xmodel != nullptr);

            m_xmodel_stream_info = m_repository->GetInformationFor(m_xmodel_stream_info_raw);
            REQUIRE(m_xmodel_stream_info != nullptr);
            REQUIRE(m_xmodel_stream_info->m_ordered_members.size() == 1);

            m_high_mip_bounds_member = m_xmodel_stream_info->m_ordered_members[0].get();
            REQUIRE(m_high_mip_bounds_member != nullptr);
        }

    public:
        CommandsSequenceTestsHelper()
            : m_repository(std::make_unique<InMemoryRepository>()),
              m_state(std::make_unique<CommandsParserState>(m_repository.get())),
              m_lod_info_t_raw(nullptr),
              m_lod_info_t(nullptr),
              m_xmodel_raw(nullptr),
              m_xmodel(nullptr),
              m_xmodel_stream_info_raw(nullptr),
              m_xmodel_stream_info(nullptr),
              m_high_mip_bounds_member(nullptr),
              m_consumed_token_count(0u)
        {
            AddSampleData();
            REQUIRE(CreateInformation());
            RetrieveInformationPointers();
        }

        void Tokens(std::initializer_list<Movable<CommandsParserValue>> tokens)
        {
            m_lexer = std::make_unique<MockLexer<CommandsParserValue>>(tokens, CommandsParserValue::EndOfFile(TokenPos()));
        }

        bool PerformTest()
        {
            REQUIRE(m_lexer);
            const auto sequence = std::make_unique<SequenceCount>();
            return sequence->MatchSequence(m_lexer.get(), m_state.get(), m_consumed_token_count);
        }
    };

    TEST_CASE("SequenceCount: Ensure can parse array access in middle of dynamic member chain", "[parsing][sequence]")
    {
        CommandsSequenceTestsHelper helper;
        const TokenPos pos;
        helper.m_state->SetInUse(helper.m_xmodel_stream_info);
        helper.Tokens({
            CommandsParserValue::Identifier(pos, new std::string("set")),
            CommandsParserValue::Identifier(pos, new std::string("count")),
            CommandsParserValue::Identifier(pos, new std::string("highMipBounds")),
            CommandsParserValue::Identifier(pos, new std::string("XModel")),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("lodInfo")),
            CommandsParserValue::Character(pos, '['),
            CommandsParserValue::Integer(pos, 0),
            CommandsParserValue::Character(pos, ']'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Character(pos, ':'),
            CommandsParserValue::Identifier(pos, new std::string("numsurfs")),
            CommandsParserValue::Character(pos, ';'),
            CommandsParserValue::EndOfFile(pos),
        });

        const auto result = helper.PerformTest();

        REQUIRE(result);
        REQUIRE(helper.m_consumed_token_count == 14);

        auto* pointer =
            dynamic_cast<PointerDeclarationModifier*>(helper.m_high_mip_bounds_member->m_member->m_type_declaration->m_declaration_modifiers[0].get());
        REQUIRE(pointer != nullptr);
        REQUIRE(pointer->m_count_evaluation != nullptr);
        REQUIRE(pointer->m_count_evaluation->GetType() == EvaluationType::OPERAND_DYNAMIC);

        const auto* dynamic = dynamic_cast<const OperandDynamic*>(pointer->m_count_evaluation.get());
        REQUIRE(dynamic != nullptr);
        REQUIRE(dynamic->m_structure == helper.m_xmodel);
        REQUIRE(dynamic->m_member_chain.size() == 2);

        REQUIRE(dynamic->m_member_chain[0].m_member->m_member->m_name == "lodInfo");
        REQUIRE(dynamic->m_member_chain[0].m_array_indices.size() == 1);
        REQUIRE(dynamic->m_member_chain[0].m_array_indices[0]->GetType() == EvaluationType::OPERAND_STATIC);
        REQUIRE(dynamic_cast<const OperandStatic*>(dynamic->m_member_chain[0].m_array_indices[0].get())->m_value == 0);

        REQUIRE(dynamic->m_member_chain[1].m_member->m_member->m_name == "numsurfs");
        REQUIRE(dynamic->m_member_chain[1].m_array_indices.empty());
    }
} // namespace test::parsing::commands::sequence::sequence_count
