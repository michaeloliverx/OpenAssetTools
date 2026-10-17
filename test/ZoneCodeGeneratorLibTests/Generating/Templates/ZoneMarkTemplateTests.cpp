#include "Domain/Definition/PointerDeclarationModifier.h"
#include "Domain/Evaluation/OperandDynamic.h"
#include "Domain/Evaluation/OperandStatic.h"
#include "Domain/Evaluation/Operation.h"
#include "Generating/Templates/ZoneMarkTemplate.h"
#include "Parsing/PostProcessing/CreateMemberInformationPostProcessor.h"
#include "Parsing/PostProcessing/CreateStructureInformationPostProcessor.h"
#include "Persistence/InMemory/InMemoryRepository.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <sstream>

TEST_CASE("Union markers preserve scalar alternatives", "[generating][mark][union]")
{
    InMemoryRepository repository;
    repository.SetGame("Test");

    auto value = std::make_unique<UnionDefinition>("", "Value", 4);
    value->m_members.emplace_back(std::make_shared<Variable>("integer", std::make_unique<TypeDeclaration>(BaseTypeDefinition::INT)));
    value->m_members.emplace_back(std::make_shared<Variable>("number", std::make_unique<TypeDeclaration>(BaseTypeDefinition::FLOAT)));
    auto text = std::make_unique<TypeDeclaration>(BaseTypeDefinition::CHAR);
    text->m_declaration_modifiers.emplace_back(std::make_unique<PointerDeclarationModifier>());
    value->m_members.emplace_back(std::make_shared<Variable>("text", std::move(text)));
    const auto* valueDefinition = value.get();
    repository.Add(std::move(value));

    auto entry = std::make_unique<StructDefinition>("", "Entry", 4);
    entry->m_members.emplace_back(std::make_shared<Variable>("kind", std::make_unique<TypeDeclaration>(BaseTypeDefinition::INT)));
    entry->m_members.emplace_back(std::make_shared<Variable>("value", std::make_unique<TypeDeclaration>(valueDefinition)));
    const auto* entryDefinition = entry.get();
    repository.Add(std::move(entry));

    REQUIRE(CreateStructureInformationPostProcessor().PostProcess(&repository));
    REQUIRE(CreateMemberInformationPostProcessor().PostProcess(&repository));
    auto* entryInfo = repository.GetInformationFor(entryDefinition);
    auto* valueInfo = repository.GetInformationFor(valueDefinition);
    entryInfo->m_asset_name = "ASSET_TYPE_ENTRY";
    entryInfo->m_requires_marking = true;
    valueInfo->m_requires_marking = true;
    valueInfo->m_ordered_members[2]->m_is_string = true;
    valueInfo->m_ordered_members[2]->m_is_indexed_string = true;

    const auto condition = [entryInfo](const int kind)
    {
        std::vector<MemberAccessor> members;
        members.emplace_back(entryInfo->m_ordered_members[0].get());
        return std::make_unique<Operation>(
            OperationType::OPERATION_EQUALS, std::make_unique<OperandDynamic>(entryInfo, std::move(members)), std::make_unique<OperandStatic>(kind));
    };
    valueInfo->m_ordered_members[0]->m_condition = condition(0);
    valueInfo->m_ordered_members[1]->m_condition = condition(1);

    std::string expected;
    SECTION("Fallback strings are guarded by both scalar alternatives")
    {
        expected = "if(varEntry->kind==0){}elseif(varEntry->kind==1){}else{Mark_String(varValue->text);}";
    }
    SECTION("Conditional strings remain part of the same branch chain")
    {
        valueInfo->m_ordered_members[2]->m_condition = condition(2);
        expected = "if(varEntry->kind==0){}elseif(varEntry->kind==1){}elseif(varEntry->kind==2){Mark_String(varValue->text);}";
    }
    SECTION("Disabled scalar alternatives do not introduce branches")
    {
        valueInfo->m_ordered_members[0]->m_condition = std::make_unique<OperandStatic>(0);
        expected = "if(varEntry->kind==1){}else{Mark_String(varValue->text);}";
    }
    SECTION("An unguarded string needs no branch when both scalars are disabled")
    {
        valueInfo->m_ordered_members[0]->m_condition = std::make_unique<OperandStatic>(0);
        valueInfo->m_ordered_members[1]->m_condition = std::make_unique<OperandStatic>(0);
        expected = "Mark_String(varValue->text);";
    }

    const auto context = OncePerAssetRenderingContext::BuildContext(&repository, entryInfo);
    ZoneMarkTemplate codeTemplate;
    std::ostringstream output;
    for (const auto& file : codeTemplate.GetFilesToRenderOncePerAsset(*context))
    {
        if (file.m_file_name.ends_with(".cpp"))
            codeTemplate.RenderOncePerAssetFile(output, file.m_tag, *context);
    }

    auto source = output.str();
    std::erase_if(source,
                  [](const unsigned char c)
                  {
                      return std::isspace(c);
                  });
    INFO(source);
    REQUIRE(source.find("::Mark_Value(){assert(varValue!=nullptr);" + expected + "}") != std::string::npos);
}
