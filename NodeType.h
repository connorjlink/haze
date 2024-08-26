#ifndef HAZE_NODETYPE_H
#define HAZE_NODETYPE_H

#define AS_NODE(x) static_cast<Node*>(x)
#define AS_FUNCTION_NODE(x) static_cast<Function*>(x)
#define AS_STATEMENT_NODE(x) static_cast<Statement*>(x)
#define AS_EXPRESSION_NODE(x) static_cast<Expression*>(x)
#define AS_COMMAND_NODE(x) static_cast<Command*>(x)
#define AS_INTRINSIC_NODE(x)  static_cast<Intrinsic*>(x)

#include <string>
#include <unordered_map>

namespace hz
{
    enum class NodeType
    {
        FUNCTION,
        STATEMENT,
        EXPRESSION,
        COMMAND,
        INTRINSIC,
    };

    static const std::unordered_map<NodeType, std::string> _node_map
    {
        { NodeType::FUNCTION, "function" },
        { NodeType::STATEMENT, "statement" },
        { NodeType::EXPRESSION, "expression" },
        { NodeType::COMMAND, "command" },
        { NodeType::INTRINSIC, "intrinsic" },
    };
}

#endif 
