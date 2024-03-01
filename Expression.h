#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include <string_view>

namespace hz
{
    struct IntegerLiteral;
    struct Identifier;
    struct FunctionCall;
    struct BinaryExpression;

    enum class ExpressionType
    {
        INTEGER_LITERAL,
        IDENTIFIER,
        FUNCTION_CALL,
        BINARY_EXPRESSION,
    };
    struct Expression
    {
        const ExpressionType type;

        union
        {
            IntegerLiteral* integer_literal;
            Identifier* identifier;
            FunctionCall* function_call;
            BinaryExpression* binary_expression;
        } const as;
    };

    struct IntegerLiteral
    {
        int value;
    };

    struct Identifier
    {
        std::string_view name;
        bool is_constexpr = false;
    };

    struct FunctionCall
    {
        std::string_view name;
        std::vector<Expression*> arguments;
    };

    enum class BinaryExpressionType
    {
        PLUS,
        MINUS,
        MULTIPLY,
    };
    struct BinaryExpression
    {
        BinaryExpressionType type;
        Expression* left;
        Expression* right;
    };
}

#endif //HAZE_EXPRESSION_H
