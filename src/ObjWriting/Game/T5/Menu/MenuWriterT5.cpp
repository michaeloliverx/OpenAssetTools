#include "MenuWriterT5.h"

#include "Game/T5/MenuConstantsT5.h"
#include "Menu/AbstractMenuWriter.h"
#include "ObjWriting.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace T5;

namespace
{
    enum class ExpressionNodeType
    {
        OPERAND,
        UNARY_OPERATOR,
        BINARY_OPERATOR,
        FUNCTION,
    };

    struct ExpressionNode
    {
        ExpressionNodeType m_type;
        Operand m_operand{};
        int m_command_index{};
        std::shared_ptr<ExpressionNode> m_left;
        std::shared_ptr<ExpressionNode> m_right;
    };

    using ExpressionNodePtr = std::shared_ptr<ExpressionNode>;

    bool IsUnaryOperator(const int commandIndex)
    {
        return commandIndex == OP_NEGATE || commandIndex == OP_NOT || commandIndex == OP_BITWISENOT;
    }

    bool IsBinaryOperator(const int commandIndex)
    {
        return commandIndex >= OP_MULTIPLY && commandIndex <= OP_SUBTRACT || commandIndex >= OP_LESSTHAN && commandIndex <= OP_BITWISEOR
               || commandIndex == OP_BITSHIFTLEFT || commandIndex == OP_BITSHIFTRIGHT;
    }

    bool IsFunctionWithoutArguments(const std::string_view functionName)
    {
        return std::ranges::find(g_expFunctionsWithoutArguments, functionName) != g_expFunctionsWithoutArguments.end();
    }

    bool IsFunctionWithOptionalArguments(const std::string_view functionName)
    {
        return std::ranges::find(g_expFunctionsWithOptionalArguments, functionName) != g_expFunctionsWithOptionalArguments.end();
    }

    ExpressionNodePtr MakeOperandNode(const Operand& operand)
    {
        auto node = std::make_shared<ExpressionNode>();
        node->m_type = ExpressionNodeType::OPERAND;
        node->m_operand = operand;
        return node;
    }

    ExpressionNodePtr MakeUnaryNode(const int commandIndex, ExpressionNodePtr operand)
    {
        auto node = std::make_shared<ExpressionNode>();
        node->m_type = ExpressionNodeType::UNARY_OPERATOR;
        node->m_command_index = commandIndex;
        node->m_left = std::move(operand);
        return node;
    }

    ExpressionNodePtr MakeBinaryNode(const int commandIndex, ExpressionNodePtr left, ExpressionNodePtr right)
    {
        auto node = std::make_shared<ExpressionNode>();
        node->m_type = ExpressionNodeType::BINARY_OPERATOR;
        node->m_command_index = commandIndex;
        node->m_left = std::move(left);
        node->m_right = std::move(right);
        return node;
    }

    ExpressionNodePtr MakeFunctionNode(const int commandIndex, ExpressionNodePtr arguments = nullptr)
    {
        auto node = std::make_shared<ExpressionNode>();
        node->m_type = ExpressionNodeType::FUNCTION;
        node->m_command_index = commandIndex;
        node->m_left = std::move(arguments);
        return node;
    }

    ExpressionNodePtr BuildExpression(const ExpressionStatement& statement)
    {
        if (statement.numRpn <= 0 || !statement.rpn)
            return nullptr;

        // Unlike IW4, T5 has no VAL_FUNCTION or ExpressionSupportingData. It stores
        // functions as postfix indices into expressions/functions.txt. Most functions
        // consume one stack entry, with comma expressions representing multiple arguments.
        // A small number accept either zero or one argument, so retain both possible stacks.
        std::vector<std::vector<ExpressionNodePtr>> candidates(1);

        for (auto entryIndex = 0; entryIndex < statement.numRpn; entryIndex++)
        {
            const auto& entry = statement.rpn[entryIndex];
            if (entry.type == RPN_END)
                break;

            std::vector<std::vector<ExpressionNodePtr>> nextCandidates;
            for (const auto& candidate : candidates)
            {
                if (entry.type == RPN_CONSTANT)
                {
                    auto next = candidate;
                    next.emplace_back(MakeOperandNode(entry.data.constant));
                    nextCandidates.emplace_back(std::move(next));
                    continue;
                }

                if (entry.type != RPN_CMD_IDX)
                    continue;

                const auto commandIndex = entry.data.cmdIdx;
                if (commandIndex < 0 || static_cast<size_t>(commandIndex) >= std::size(g_expFunctionNames))
                    continue;

                if (IsUnaryOperator(commandIndex))
                {
                    if (candidate.empty())
                        continue;

                    auto next = candidate;
                    auto operand = std::move(next.back());
                    next.back() = MakeUnaryNode(commandIndex, std::move(operand));
                    nextCandidates.emplace_back(std::move(next));
                }
                else if (IsBinaryOperator(commandIndex))
                {
                    if (candidate.size() < 2)
                        continue;

                    auto next = candidate;
                    auto right = std::move(next.back());
                    next.pop_back();
                    auto left = std::move(next.back());
                    next.back() = MakeBinaryNode(commandIndex, std::move(left), std::move(right));
                    nextCandidates.emplace_back(std::move(next));
                }
                else if (commandIndex >= NUM_EXPRESSION_OPERATORS)
                {
                    const std::string_view functionName(g_expFunctionNames[commandIndex]);
                    const auto withoutArguments = IsFunctionWithoutArguments(functionName);
                    const auto optionalArguments = IsFunctionWithOptionalArguments(functionName);

                    if (withoutArguments || optionalArguments)
                    {
                        auto next = candidate;
                        next.emplace_back(MakeFunctionNode(commandIndex));
                        nextCandidates.emplace_back(std::move(next));
                    }

                    if ((!withoutArguments || optionalArguments) && !candidate.empty())
                    {
                        auto next = candidate;
                        auto arguments = std::move(next.back());
                        next.back() = MakeFunctionNode(commandIndex, std::move(arguments));
                        nextCandidates.emplace_back(std::move(next));
                    }
                }
            }

            if (nextCandidates.empty())
                return nullptr;

            // Stack depth is all that affects subsequent parsing. Keeping one candidate
            // per depth prevents optional-argument functions from growing exponentially.
            std::map<size_t, std::vector<ExpressionNodePtr>> candidatesByDepth;
            for (auto& candidate : nextCandidates)
                candidatesByDepth.try_emplace(candidate.size(), std::move(candidate));

            candidates.clear();
            candidates.reserve(candidatesByDepth.size());
            for (auto& [depth, candidate] : candidatesByDepth)
                candidates.emplace_back(std::move(candidate));
        }

        const auto result = std::ranges::find_if(candidates,
                                                 [](const auto& candidate)
                                                 {
                                                     return candidate.size() == 1;
                                                 });
        return result == candidates.end() ? nullptr : result->front();
    }

    class MenuWriter final : public menu::AbstractBaseWriter, public menu::IWriterT5
    {
    public:
        explicit MenuWriter(std::ostream& stream)
            : AbstractBaseWriter(stream)
        {
        }

        void WriteMenu(const menuDef_t& menu) override
        {
            StartMenuDefScope();
            WriteMenuData(menu);
            EndScope();
        }

        void Start() override
        {
            AbstractBaseWriter::Start();
        }

        void End() override
        {
            AbstractBaseWriter::End();
        }

        void IncludeMenu(const std::string& menuPath) const override
        {
            AbstractBaseWriter::IncludeMenu(menuPath);
        }

    private:
        static bool HasStatement(const ExpressionStatement& statement)
        {
            return statement.numRpn > 0 && statement.rpn;
        }

        void WriteExpressionNode(const ExpressionNode& node) const
        {
            switch (node.m_type)
            {
            case ExpressionNodeType::OPERAND:
                switch (node.m_operand.dataType)
                {
                case VAL_FLOAT:
                    m_stream << node.m_operand.internals.floatVal;
                    break;

                case VAL_INT:
                    m_stream << node.m_operand.internals.intVal;
                    break;

                case VAL_STRING:
                    WriteEscapedString(node.m_operand.internals.stringVal ? node.m_operand.internals.stringVal : "");
                    break;
                }
                break;

            case ExpressionNodeType::UNARY_OPERATOR:
                m_stream << g_expFunctionNames[node.m_command_index] << "(";
                WriteExpressionNode(*node.m_left);
                m_stream << ")";
                break;

            case ExpressionNodeType::BINARY_OPERATOR:
                if (node.m_command_index == OP_COMMA)
                {
                    WriteExpressionNode(*node.m_left);
                    m_stream << ", ";
                    WriteExpressionNode(*node.m_right);
                }
                else
                {
                    m_stream << "(";
                    WriteExpressionNode(*node.m_left);
                    m_stream << " " << g_expFunctionNames[node.m_command_index] << " ";
                    WriteExpressionNode(*node.m_right);
                    m_stream << ")";
                }
                break;

            case ExpressionNodeType::FUNCTION:
                m_stream << g_expFunctionNames[node.m_command_index] << "(";
                if (node.m_left)
                    WriteExpressionNode(*node.m_left);
                m_stream << ")";
                break;
            }
        }

        void WriteStatementProperty(const std::string& propertyKey, const ExpressionStatement& statement, const bool isBooleanStatement) const
        {
            const auto expression = BuildExpression(statement);
            if (!expression)
                return;

            Indent();
            WriteKey(propertyKey);

            if (isBooleanStatement)
                m_stream << "when(";

            const auto wrapCommaExpression = expression->m_type == ExpressionNodeType::BINARY_OPERATOR && expression->m_command_index == OP_COMMA;
            if (wrapCommaExpression)
                m_stream << "(";
            WriteExpressionNode(*expression);
            if (wrapCommaExpression)
                m_stream << ")";

            if (isBooleanStatement)
                m_stream << ")";
            m_stream << ";\n";
        }

        void WriteUnconditionalScript(const char* script) const
        {
            const auto tokenList = CreateScriptTokenList(script, true);

            auto isNewStatement = true;
            for (const auto& token : tokenList)
            {
                if (isNewStatement)
                {
                    if (token == ";")
                        continue;
                    Indent();
                }

                if (token == ";")
                {
                    m_stream << ";\n";
                    isNewStatement = true;
                    continue;
                }

                if (!isNewStatement)
                    m_stream << " ";
                else
                    isNewStatement = false;

                if (DoesTokenNeedQuotationMarks(token))
                    WriteEscapedString(token);
                else
                    m_stream << token;
            }

            if (!isNewStatement)
                m_stream << ";\n";
        }

        void WriteScriptProperty(const std::string& propertyKey, const char* script)
        {
            if (!script || !script[0])
                return;

            Indent();
            m_stream << propertyKey << "\n";
            Indent();
            m_stream << "{\n";
            IncIndent();
            WriteUnconditionalScript(script);
            DecIndent();
            Indent();
            m_stream << "}\n";
        }

        using ConditionKey = std::pair<int, int>;

        static std::map<ConditionKey, const ExpressionStatement*> CreateConditionMap(const GenericEventScript* eventScript)
        {
            std::map<ConditionKey, const ExpressionStatement*> result;
            for (auto* current = eventScript; current; current = current->next)
            {
                if (HasStatement(current->condition))
                    result.emplace(ConditionKey(current->constructID, current->blockID), &current->condition);
            }
            return result;
        }

        bool WriteEventCondition(const GenericEventScript& eventScript, const std::map<ConditionKey, const ExpressionStatement*>& conditionMap) const
        {
            auto firstCondition = true;
            for (const auto* prerequisite = eventScript.prerequisites; prerequisite; prerequisite = prerequisite->next)
            {
                const auto condition = conditionMap.find(ConditionKey(prerequisite->constructID, prerequisite->blockID));
                if (condition == conditionMap.end())
                    return false;

                const auto expression = BuildExpression(*condition->second);
                if (!expression)
                    return false;

                if (!firstCondition)
                    m_stream << " && ";
                if (!prerequisite->fireOnTrue)
                    m_stream << "!(";
                WriteExpressionNode(*expression);
                if (!prerequisite->fireOnTrue)
                    m_stream << ")";
                firstCondition = false;
            }

            if (eventScript.type == EVENT_IF || eventScript.type == EVENT_ELSE_IF)
            {
                const auto expression = BuildExpression(eventScript.condition);
                if (!expression)
                    return false;

                if (!firstCondition)
                    m_stream << " && ";
                if (!eventScript.fireOnTrue)
                    m_stream << "!(";
                WriteExpressionNode(*expression);
                if (!eventScript.fireOnTrue)
                    m_stream << ")";
                firstCondition = false;
            }

            return !firstCondition;
        }

        void WriteEventScript(const GenericEventScript* eventScript)
        {
            if (!eventScript)
                return;

            const auto conditionMap = CreateConditionMap(eventScript);
            for (auto* current = eventScript; current; current = current->next)
            {
                if (!current->action || !current->action[0])
                    continue;

                if (current->type == EVENT_UNCONDITIONAL && !current->prerequisites)
                {
                    WriteUnconditionalScript(current->action);
                    continue;
                }

                Indent();
                m_stream << "if (";
                if (!WriteEventCondition(*current, conditionMap))
                {
                    m_stream << "0)\n";
                }
                else
                {
                    m_stream << ")\n";
                }
                Indent();
                m_stream << "{\n";
                IncIndent();
                WriteUnconditionalScript(current->action);
                DecIndent();
                Indent();
                m_stream << "}\n";
            }
        }

        void WriteEventProperty(const std::string& propertyKey, const GenericEventScript* eventScript)
        {
            if (!eventScript)
                return;

            Indent();
            m_stream << propertyKey << "\n";
            Indent();
            m_stream << "{\n";
            IncIndent();
            WriteEventScript(eventScript);
            DecIndent();
            Indent();
            m_stream << "}\n";
        }

        static std::string_view GetEventPropertyName(const std::string_view name, const bool isMenu)
        {
            constexpr std::array menuEventProperties{
                std::string_view("onOpen"),
                std::string_view("onFocus"),
                std::string_view("leaveFocus"),
                std::string_view("onClose"),
                std::string_view("onESC"),
            };
            if (std::ranges::find(menuEventProperties, name) != menuEventProperties.end())
                return name;

            if (isMenu)
                return {};

            constexpr std::array itemEventProperties{
                std::string_view("onFocus"),
                std::string_view("leaveFocus"),
                std::string_view("action"),
                std::string_view("focusSound"),
                std::string_view("onListboxSelectionChange"),
                std::string_view("onEnter"),
            };
            if (std::ranges::find(itemEventProperties, name) != itemEventProperties.end())
                return name;

            if (name == "onAccept")
                return "accept";
            if (name == "doubleClick")
                return "doubleclick";
            if (name == "RightClick")
                return "rightclick";

            return {};
        }

        void WriteEventHandlers(const GenericEventHandler* handler, const bool isMenu)
        {
            std::vector<const GenericEventHandler*> handlers;
            for (auto* current = handler; current; current = current->next)
                handlers.emplace_back(current);

            for (auto current = handlers.rbegin(); current != handlers.rend(); ++current)
            {
                const auto* eventHandler = *current;
                if (!eventHandler->name || !eventHandler->eventScript)
                    continue;

                const std::string_view name(eventHandler->name);
                const auto propertyName = GetEventPropertyName(name, isMenu);
                if (!propertyName.empty())
                {
                    WriteEventProperty(std::string(propertyName), eventHandler->eventScript);
                }
                else if (!isMenu)
                {
                    Indent();
                    WriteKey("onEvent");
                    m_stream << eventHandler->name << "\n";
                    Indent();
                    m_stream << "{\n";
                    IncIndent();
                    WriteEventScript(eventHandler->eventScript);
                    DecIndent();
                    Indent();
                    m_stream << "}\n";
                }
            }
        }

        void WriteRectProperty(const std::string& propertyKey, const rectDef_s& rect) const
        {
            Indent();
            WriteKey(propertyKey);
            m_stream << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << " " << rect.horzAlign << " " << rect.vertAlign << "\n";
        }

        void WriteMaterialProperty(const std::string& propertyKey, const Material* material) const
        {
            if (!material || !material->info.name)
                return;

            const auto* materialName = material->info.name;
            if (materialName[0] == ',')
                materialName++;
            WriteStringProperty(propertyKey, materialName);
        }

        void WriteItemKeyHandlers(const ItemKeyHandler* handler)
        {
            for (const auto* current = handler; current; current = current->next)
            {
                std::string key;
                if (current->key >= '!' && current->key <= '~' && current->key != '"')
                    key = std::format("execKey \"{}\"", static_cast<char>(current->key));
                else
                    key = std::format("execKeyInt {}", current->key);

                WriteEventProperty(key, current->keyScript);
            }
        }

        void WriteMultiTokenStringProperty(const std::string& propertyKey, const char* value) const
        {
            if (!value)
                return;

            Indent();
            WriteKey(propertyKey);
            m_stream << "{ ";

            const auto tokenList = CreateScriptTokenList(value, true);
            auto firstToken = true;
            for (const auto& token : tokenList)
            {
                if (firstToken)
                    firstToken = false;
                else
                    m_stream << ";";

                WriteEscapedString(token);
            }
            if (!firstToken)
                m_stream << " ";
            m_stream << "}\n";
        }

        void WriteVisibilityBitsProperty(const uint64_t showBits, const uint64_t hideBits) const
        {
            if (!showBits && hideBits == std::numeric_limits<uint64_t>::max())
                return;

            Indent();
            WriteKey("visibilityBits");
            m_stream << "bits( ";
            for (auto bit = 0u; bit < 64u; bit++)
            {
                const auto mask = uint64_t{1} << bit;
                if (showBits & mask)
                    m_stream << bit << " ";
                if (!(hideBits & mask))
                    m_stream << "!" << bit << " ";
            }
            m_stream << ")\n";
        }

        void WriteFrameProperty(const windowDef_t& window) const
        {
            if (window.frameSides == 0 && window.frameSize == 0.0f && window.frameTexSize == 0.0f)
                return;

            Indent();
            WriteKey("frame");
            m_stream << window.frameTexSize << " " << window.frameSize << " " << static_cast<int>(window.frameSides) << "\n";
        }

        static const textDef_s* GetTextDef(const itemDef_s& item)
        {
            switch (item.type)
            {
            case ITEM_TYPE_TEXT:
            case ITEM_TYPE_BUTTON:
            case ITEM_TYPE_LISTBOX:
            case ITEM_TYPE_EDITFIELD:
            case ITEM_TYPE_NUMERICFIELD:
            case ITEM_TYPE_SLIDER:
            case ITEM_TYPE_YESNO:
            case ITEM_TYPE_MULTI:
            case ITEM_TYPE_DVARENUM:
            case ITEM_TYPE_BIND:
            case ITEM_TYPE_VALIDFILEFIELD:
            case ITEM_TYPE_UPREDITFIELD:
            case ITEM_TYPE_GAME_MESSAGE_WINDOW:
            case ITEM_TYPE_BIND2:
            case ITEM_TYPE_OWNERDRAW_TEXT:
            case ITEM_TYPE_OD_TEXT_BUTTON:
            case ITEM_TYPE_ALPHANUMERICFIELD:
            case ITEM_TYPE_DECIMALFIELD:
                return item.typeData.textDef;

            default:
                return nullptr;
            }
        }

        static const focusItemDef_s* GetFocusItemDef(const itemDef_s& item)
        {
            if (item.type == ITEM_TYPE_OD_BUTTON || item.type == ITEM_TYPE_BUTTON_NO_TEXT)
                return item.typeData.blankButtonDef;

            const auto* textDef = GetTextDef(item);
            if (!textDef)
                return nullptr;

            switch (item.type)
            {
            case ITEM_TYPE_BUTTON:
            case ITEM_TYPE_LISTBOX:
            case ITEM_TYPE_EDITFIELD:
            case ITEM_TYPE_NUMERICFIELD:
            case ITEM_TYPE_SLIDER:
            case ITEM_TYPE_YESNO:
            case ITEM_TYPE_MULTI:
            case ITEM_TYPE_DVARENUM:
            case ITEM_TYPE_BIND:
            case ITEM_TYPE_VALIDFILEFIELD:
            case ITEM_TYPE_UPREDITFIELD:
            case ITEM_TYPE_BIND2:
            case ITEM_TYPE_OD_TEXT_BUTTON:
            case ITEM_TYPE_ALPHANUMERICFIELD:
            case ITEM_TYPE_DECIMALFIELD:
                return textDef->textTypeData.focusItemDef;

            default:
                return nullptr;
            }
        }

        static const listBoxDef_s* GetListBoxDef(const itemDef_s& item)
        {
            const auto* focusDef = GetFocusItemDef(item);
            return item.type == ITEM_TYPE_LISTBOX && focusDef ? focusDef->focusTypeData.listBox : nullptr;
        }

        static const editFieldDef_s* GetEditFieldDef(const itemDef_s& item)
        {
            switch (item.type)
            {
            case ITEM_TYPE_EDITFIELD:
            case ITEM_TYPE_NUMERICFIELD:
            case ITEM_TYPE_SLIDER:
            case ITEM_TYPE_YESNO:
            case ITEM_TYPE_BIND:
            case ITEM_TYPE_VALIDFILEFIELD:
            case ITEM_TYPE_UPREDITFIELD:
            case ITEM_TYPE_BIND2:
            case ITEM_TYPE_ALPHANUMERICFIELD:
            case ITEM_TYPE_DECIMALFIELD:
                break;

            default:
                return nullptr;
            }

            const auto* focusDef = GetFocusItemDef(item);
            return focusDef ? focusDef->focusTypeData.editField : nullptr;
        }

        static const multiDef_s* GetMultiDef(const itemDef_s& item)
        {
            const auto* focusDef = GetFocusItemDef(item);
            return item.type == ITEM_TYPE_MULTI && focusDef ? focusDef->focusTypeData.multi : nullptr;
        }

        static const enumDvarDef_s* GetEnumDvarDef(const itemDef_s& item)
        {
            const auto* focusDef = GetFocusItemDef(item);
            return item.type == ITEM_TYPE_DVARENUM && focusDef ? focusDef->focusTypeData.enumDvar : nullptr;
        }

        void WriteColumnProperty(const listBoxDef_s& listBox) const
        {
            if (listBox.numColumns <= 0)
                return;

            Indent();
            WriteKey("columns");
            m_stream << listBox.numColumns << "\n";

            const auto columnCount = std::min<size_t>(listBox.numColumns, std::size(listBox.columnInfo));
            for (size_t columnIndex = 0u; columnIndex < columnCount; columnIndex++)
            {
                const auto& column = listBox.columnInfo[columnIndex];

                Indent();
                for (auto i = 0u; i < MENU_KEY_SPACING; i++)
                    m_stream << " ";

                m_stream << column.rect.x << " " << column.rect.w << " " << column.maxChars << " " << column.rect.horzAlign << "\n";
            }
        }

        static bool IsUserArea(const listBoxDef_s& listBox)
        {
            const auto columnCount = std::min<size_t>(listBox.numColumns, std::size(listBox.columnInfo));
            for (auto columnIndex = 0uz; columnIndex < columnCount; columnIndex++)
            {
                const auto& rect = listBox.columnInfo[columnIndex].rect;
                if (rect.y != 0.0f || rect.h != listBox.elementHeight || rect.vertAlign != 0)
                    return true;
            }
            return false;
        }

        void WriteUserAreaProperty(const listBoxDef_s& listBox) const
        {
            if (listBox.numColumns <= 0)
                return;

            Indent();
            WriteKey("userarea");
            m_stream << listBox.numColumns << "\n";

            const auto columnCount = std::min<size_t>(listBox.numColumns, std::size(listBox.columnInfo));
            for (auto columnIndex = 0uz; columnIndex < columnCount; columnIndex++)
            {
                const auto& column = listBox.columnInfo[columnIndex];

                Indent();
                for (auto i = 0u; i < MENU_KEY_SPACING; i++)
                    m_stream << " ";

                m_stream << column.rect.x << " " << column.rect.y << " " << column.rect.w << " " << column.rect.h << " " << column.maxChars << " "
                         << column.rect.horzAlign << " " << column.rect.vertAlign << "\n";
            }
        }

        void WriteMenuItemsDef(const listBoxDef_s& listBox)
        {
            if (listBox.maxRows <= 0)
                return;

            Indent();
            m_stream << "menuItemsDef\n";
            Indent();
            m_stream << "{\n";
            IncIndent();
            WriteIntProperty("maxRows", listBox.maxRows, 0);

            const auto columnCount = std::min<size_t>(listBox.numColumns, std::size(listBox.columnInfo));
            for (auto columnIndex = 0uz; columnIndex < columnCount; columnIndex++)
            {
                Indent();
                m_stream << "menuColDef\n";
                Indent();
                m_stream << "{\n";
                IncIndent();

                const auto& column = listBox.columnInfo[columnIndex];
                auto elementStyle = column.elementStyle;
                if (listBox.rows && listBox.maxRows > 0 && listBox.rows[0].cells)
                    elementStyle = listBox.rows[0].cells[columnIndex].type;
                WriteIntProperty("type", elementStyle, -1);
                WriteRectProperty("rect", column.rect);
                WriteIntProperty("maxChars", column.maxChars, 0);

                DecIndent();
                Indent();
                m_stream << "}\n";
            }

            DecIndent();
            Indent();
            m_stream << "}\n";
        }

        void WriteListBoxProperties(const itemDef_s& item)
        {
            const auto* listBox = GetListBoxDef(item);
            if (!listBox)
                return;

            WriteKeywordProperty("notselectable", listBox->notselectable != 0);
            WriteKeywordProperty("noscrollbars", listBox->noScrollBars != 0);
            WriteKeywordProperty("noBlinkingHighlight", listBox->noBlinkingHighlight != 0);
            WriteKeywordProperty("usepaging", listBox->usePaging != 0);
            WriteFloatProperty("elementwidth", listBox->elementWidth, 0.0f);
            WriteFloatProperty("elementheight", listBox->elementHeight, 0.0f);
            WriteFloatProperty("feeder", listBox->special, 0.0f);

            if (listBox->numColumns > 0)
            {
                Indent();
                WriteKey("elementtype");
                m_stream << listBox->columnInfo[0].elementStyle << "\n";
            }

            if (listBox->maxRows > 0)
                WriteMenuItemsDef(*listBox);
            else if (IsUserArea(*listBox))
                WriteUserAreaProperty(*listBox);
            else
                WriteColumnProperty(*listBox);

            WriteColorProperty("selectBorder", listBox->selectBorder, COLOR_0000);
            WriteColorProperty("disableColor", listBox->disableColor, COLOR_0000);
            WriteColorProperty("focusColor", listBox->focusColor, COLOR_0000);
            WriteColorProperty("elementHighlightColor", listBox->elementHighlightColor, COLOR_0000);
            WriteColorProperty("elementBackgroundColor", listBox->elementBackgroundColor, COLOR_0000);
            WriteMaterialProperty("selectIcon", listBox->selectIcon);
            WriteMaterialProperty("backgroundItemListbox", listBox->backgroundItemListbox);
            WriteMaterialProperty("highlightTexture", listBox->highlightTexture);
        }

        void WriteDvarFloatProperty(const itemDef_s& item, const editFieldDef_s& editField) const
        {
            if (!item.dvar)
                return;

            Indent();
            WriteKey("dvarFloat");
            WriteEscapedString(item.dvar);
            m_stream << " " << editField.defVal << " " << editField.minVal << " " << editField.maxVal << "\n";
        }

        void WriteEditFieldProperties(const itemDef_s& item) const
        {
            const auto* editField = GetEditFieldDef(item);
            if (!editField)
                return;

            if (std::fabs(-1.0f - editField->defVal) >= std::numeric_limits<float>::epsilon()
                || std::fabs(-1.0f - editField->minVal) >= std::numeric_limits<float>::epsilon()
                || std::fabs(-1.0f - editField->maxVal) >= std::numeric_limits<float>::epsilon())
            {
                WriteDvarFloatProperty(item, *editField);
            }
            else
            {
                WriteStringProperty("dvar", item.dvar);
            }

            WriteIntProperty("maxChars", editField->maxChars, 0);
            WriteKeywordProperty("maxCharsGotoNext", editField->maxCharsGotoNext != 0);
            WriteIntProperty("maxPaintChars", editField->maxPaintChars, 0);
        }

        void WriteMultiValueProperty(const multiDef_s& multi) const
        {
            if (multi.count <= 0)
                return;

            Indent();
            WriteKey(multi.strDef ? "dvarStrList" : "dvarFloatList");
            m_stream << "{";
            const auto valueCount = std::min<size_t>(multi.count, std::size(multi.dvarValue));
            for (size_t valueIndex = 0u; valueIndex < valueCount; valueIndex++)
            {
                if (!multi.dvarList[valueIndex] || (multi.strDef && !multi.dvarStr[valueIndex]))
                    continue;

                m_stream << " ";
                WriteEscapedString(multi.dvarList[valueIndex]);
                m_stream << " ";
                if (multi.strDef)
                    WriteEscapedString(multi.dvarStr[valueIndex]);
                else
                    m_stream << multi.dvarValue[valueIndex];
            }
            m_stream << " }\n";
        }

        void WriteMultiProperties(const itemDef_s& item) const
        {
            const auto* multi = GetMultiDef(item);
            if (!multi)
                return;

            WriteStringProperty("dvar", item.dvar);
            WriteMultiValueProperty(*multi);
            WriteKeywordProperty("actionOnEnterPressOnly", multi->actionOnEnterPressOnly != 0);
        }

        void WriteEnumDvarProperties(const itemDef_s& item) const
        {
            const auto* enumDvar = GetEnumDvarDef(item);
            if (!enumDvar)
                return;

            WriteStringProperty("dvar", item.dvar);
            WriteStringProperty("dvarEnumList", enumDvar->enumDvarName);
        }

        void WriteItemTextProperty(const textDef_s* textDef) const
        {
            if (!textDef)
                return;

            if (textDef->itemFlags & ITEM_TEXT_FLAG_SAVE_GAME_INFO)
                WriteKeywordProperty("textsavegame", true);
            else if (textDef->itemFlags & ITEM_TEXT_FLAG_CINEMATIC_SUBTITLE)
                WriteKeywordProperty("textcinematicsubtitle", true);
            else if (textDef->text)
            {
                Indent();
                WriteKey("text");
                WriteEscapedString(textDef->text);
                m_stream << "\n";
            }
        }

        void WriteAnimationStates(const UIAnimInfo* animInfo)
        {
            if (!animInfo || animInfo->animStateCount <= 1 || !animInfo->animStates)
                return;

            for (auto stateIndex = 1; stateIndex < animInfo->animStateCount; stateIndex++)
            {
                const auto* state = animInfo->animStates[stateIndex];
                if (!state || !state->name)
                    continue;

                Indent();
                m_stream << "state\n";
                Indent();
                m_stream << "{\n";
                IncIndent();
                WriteStringProperty("name", state->name);
                WriteRectProperty("rect", state->rectClient);
                WriteFloatProperty("borderSize", state->borderSize, 0.0f);
                WriteColorProperty("backcolor", state->backColor, COLOR_0000);
                WriteColorProperty("forecolor", state->foreColor, COLOR_1111);
                WriteColorProperty("bordercolor", state->borderColor, COLOR_0000);
                WriteColorProperty("outlinecolor", state->outlineColor, COLOR_0000);
                WriteFloatProperty("textscale", state->textScale, 0.0f);
                WriteFloatProperty("rotation", state->rotation, 0.0f);
                WriteEventHandlers(state->onEvent, false);
                DecIndent();
                Indent();
                m_stream << "}\n";
            }
        }

        void WriteItemData(const itemDef_s& item)
        {
            const auto* textDef = GetTextDef(item);
            const auto* focusDef = GetFocusItemDef(item);

            WriteStringProperty("name", item.window.name);
            WriteStringProperty("group", item.window.group);
            WriteRectProperty("rect", item.window.rectClient);
            WriteIntProperty("style", item.window.style, 0);
            WriteKeywordProperty("decoration", item.window.staticFlags & WINDOW_FLAG_DECORATION);
            WriteKeywordProperty("autowrapped", item.window.staticFlags & WINDOW_FLAG_AUTO_WRAPPED);
            WriteKeywordProperty("horizontalscroll", item.window.staticFlags & WINDOW_FLAG_HORIZONTAL_SCROLL);
            WriteKeywordProperty("modal", item.window.modal != 0);
            WriteFrameProperty(item.window);
            WriteIntProperty("type", item.type, ITEM_TYPE_DEFAULT);
            // T5's text parser validates the current item type, and ITEM_TYPE_DEFAULT
            // is no longer the text type. Emit the type before any text property.
            WriteItemTextProperty(textDef);
            WriteIntProperty("border", item.window.border, 0);
            WriteFloatProperty("borderSize", item.window.borderSize, 1.0f);

            if (HasStatement(item.visibleExp))
                WriteStatementProperty("visible", item.visibleExp, true);
            else if (item.window.dynamicFlags[0] & WINDOW_FLAG_VISIBLE)
                WriteIntProperty("visible", 1, 0);

            WriteVisibilityBitsProperty(item.showBits, item.hideBits);
            WriteIntProperty("ownerdraw", item.window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", item.window.ownerDrawFlags);
            if (textDef)
            {
                WriteIntProperty("align", textDef->alignment, 0);
                WriteIntProperty("textalign", textDef->textAlignMode, 0);
                WriteFloatProperty("textalignx", textDef->textalignx, 0.0f);
                WriteFloatProperty("textaligny", textDef->textaligny, 0.0f);
                WriteFloatProperty("textscale", textDef->textscale, 0.0f);
                WriteIntProperty("textstyle", textDef->textStyle, 0);
                WriteIntProperty("textfont", textDef->fontEnum, 0);
            }
            WriteColorProperty("backcolor", item.window.backColor, COLOR_0000);
            WriteColorProperty("forecolor", item.window.foreColor, COLOR_1111);
            WriteColorProperty("bordercolor", item.window.borderColor, COLOR_0000);
            WriteColorProperty("outlinecolor", item.window.outlineColor, COLOR_0000);
            WriteFloatProperty("rotation", item.window.rotation, 0.0f);
            WriteMaterialProperty("background", item.window.background);
            WriteEventHandlers(item.onEvent, false);
            if (focusDef)
            {
                WriteScriptProperty("mouseEnter", focusDef->mouseEnter);
                WriteScriptProperty("mouseExit", focusDef->mouseExit);
                WriteScriptProperty("mouseEnterText", focusDef->mouseEnterText);
                WriteScriptProperty("mouseExitText", focusDef->mouseExitText);
            }
            WriteStringProperty("dvarTest", item.dvarTest);

            if (item.dvarFlags & ITEM_DVAR_FLAG_ENABLE)
                WriteMultiTokenStringProperty("enableDvar", item.enableDvar);
            else if (item.dvarFlags & ITEM_DVAR_FLAG_DISABLE)
                WriteMultiTokenStringProperty("disableDvar", item.enableDvar);
            else if (item.dvarFlags & ITEM_DVAR_FLAG_SHOW)
                WriteMultiTokenStringProperty("showDvar", item.enableDvar);
            else if (item.dvarFlags & ITEM_DVAR_FLAG_HIDE)
                WriteMultiTokenStringProperty("hideDvar", item.enableDvar);
            else if (item.dvarFlags & ITEM_DVAR_FLAG_FOCUS)
                WriteMultiTokenStringProperty("focusDvar", item.enableDvar);

            if (focusDef)
                WriteItemKeyHandlers(focusDef->onKey);
            if (textDef && textDef->textExpData)
                WriteStatementProperty("exp text", textDef->textExpData->textExp, false);
            if (item.type == ITEM_TYPE_IMAGE && item.typeData.imageDef)
                WriteStatementProperty("exp material", item.typeData.imageDef->materialExp, false);
            if (item.rectExpData)
            {
                WriteStatementProperty("exp rect X", item.rectExpData->rectXExp, false);
                WriteStatementProperty("exp rect Y", item.rectExpData->rectYExp, false);
                WriteStatementProperty("exp rect W", item.rectExpData->rectWExp, false);
                WriteStatementProperty("exp rect H", item.rectExpData->rectHExp, false);
            }
            WriteStatementProperty("exp forecolor A", item.forecolorAExp, false);
            if (item.type == ITEM_TYPE_OWNERDRAW && item.typeData.ownerDrawDef)
                WriteStatementProperty("exp ownerdata", item.typeData.ownerDrawDef->dataExp, false);
            WriteIntProperty("ui3dWindowId", item.ui3dWindowId, -1);

            if (item.type == ITEM_TYPE_GAME_MESSAGE_WINDOW && textDef && textDef->textTypeData.gameMsgDef)
            {
                WriteIntProperty("gamemsgwindowindex", textDef->textTypeData.gameMsgDef->gameMsgWindowIndex, 0);
                WriteIntProperty("gamemsgwindowmode", textDef->textTypeData.gameMsgDef->gameMsgWindowMode, 0);
            }

            WriteListBoxProperties(item);
            WriteEditFieldProperties(item);
            WriteMultiProperties(item);
            WriteEnumDvarProperties(item);
            WriteAnimationStates(item.animInfo);
        }

        void WriteItemDefs(itemDef_s* const* items, const size_t itemCount)
        {
            if (!items || itemCount <= 0)
                return;

            for (size_t itemIndex = 0u; itemIndex < itemCount; itemIndex++)
            {
                const auto* item = items[itemIndex];
                if (!item)
                    continue;

                StartItemDefScope();
                WriteItemData(*item);
                EndScope();
            }
        }

        void WriteMenuData(const menuDef_t& menu)
        {
            WriteStringProperty("name", menu.window.name);
            WriteBoolProperty("fullscreen", menu.fullScreen != 0, false);
            WriteRectProperty("rect", menu.window.rect);
            WriteIntProperty("style", menu.window.style, 0);
            WriteIntProperty("border", menu.window.border, 0);
            WriteFloatProperty("borderSize", menu.window.borderSize, 1.0f);
            WriteFrameProperty(menu.window);
            WriteColorProperty("backcolor", menu.window.backColor, COLOR_0000);
            WriteColorProperty("forecolor", menu.window.foreColor, COLOR_1111);
            WriteColorProperty("bordercolor", menu.window.borderColor, COLOR_0000);
            WriteColorProperty("focuscolor", menu.focusColor, COLOR_0000);
            WriteColorProperty("disablecolor", menu.disableColor, COLOR_0000);
            WriteColorProperty("outlinecolor", menu.window.outlineColor, COLOR_0000);
            WriteMaterialProperty("background", menu.window.background);
            WriteIntProperty("ownerdraw", menu.window.ownerDraw, 0);
            WriteFlagsProperty("ownerdrawFlag", menu.window.ownerDrawFlags);
            WriteKeywordProperty("outOfBoundsClick", menu.window.staticFlags & WINDOW_FLAG_OUT_OF_BOUNDS_CLICK);
            WriteStringProperty("soundLoop", menu.soundName);
            WriteKeywordProperty("popup", menu.window.staticFlags & WINDOW_FLAG_POPUP);
            WriteFloatProperty("fadeClamp", menu.fadeClamp, 0.0f);
            WriteIntProperty("fadeCycle", menu.fadeCycle, 0);
            WriteFloatProperty("fadeAmount", menu.fadeAmount, 0.0f);
            WriteFloatProperty("fadeInAmount", menu.fadeInAmount, 0.0f);
            WriteFloatProperty("blurWorld", menu.blurRadius, 0.0f);
            WriteKeywordProperty("legacySplitScreenScale", menu.window.staticFlags & WINDOW_FLAG_LEGACY_SPLIT_SCREEN_SCALE);
            WriteKeywordProperty("hiddenDuringScope", menu.window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_SCOPE);
            WriteKeywordProperty("hiddenDuringFlashbang", menu.window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_FLASH_BANG);
            WriteKeywordProperty("hiddenDuringUI", menu.window.staticFlags & WINDOW_FLAG_HIDDEN_DURING_UI);
            WriteKeywordProperty("allowSignIn", menu.window.staticFlags & WINDOW_FLAG_ALLOW_SIGN_IN);
            WriteStringProperty("allowedBinding", menu.allowedBinding);
            WriteIntProperty("ui3dWindowId", menu.ui3dWindowId, -1);
            WriteIntProperty("priority", menu.priority, 2);
            WriteIntProperty("openSlideSpeed", menu.openSlideSpeed, -1);
            WriteIntProperty("closeSlideSpeed", menu.closeSlideSpeed, -1);
            WriteIntProperty("openSlideDirection", menu.openSlideDirection, -1);
            WriteIntProperty("closeSlideDirection", menu.closeSlideDirection, -1);
            WriteIntProperty("openFadingTime", menu.openFadingTime, -1);
            WriteIntProperty("closeFadingTime", menu.closeFadingTime, -1);
            WriteIntProperty("control", menu.control, 0);

            if (HasStatement(menu.visibleExp))
                WriteStatementProperty("visible", menu.visibleExp, true);
            else if (menu.window.dynamicFlags[0] & WINDOW_FLAG_VISIBLE)
                WriteIntProperty("visible", 1, 0);

            WriteVisibilityBitsProperty(menu.showBits, menu.hideBits);
            WriteStatementProperty("exp rect X", menu.rectXExp, false);
            WriteStatementProperty("exp rect Y", menu.rectYExp, false);
            WriteEventHandlers(menu.onEvent, true);
            WriteItemKeyHandlers(menu.onKey);
            WriteItemDefs(menu.items, menu.itemCount);
        }
    };
} // namespace

namespace menu
{
    std::unique_ptr<IWriterT5> CreateMenuWriterT5(std::ostream& stream)
    {
        return std::make_unique<MenuWriter>(stream);
    }
} // namespace menu
