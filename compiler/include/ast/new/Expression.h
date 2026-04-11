#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/new/AST.h>
#include <runtime/Context.h>
#include <type/Type.h>

// Haze ExpressionAST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class AdjustExpression;
    class ArgumentExpression;
    class FunctionArgumentExpression;
    class FunctionCallExpression;
    class CastExpression;
    class ConditionalExpression;
    class IdentifierExpression;
    class IntegerLiteralExpression;
    class StringLiteralExpression;
    class StructLiteralExpression;
    class UnaryExpression;
    class BinaryExpression;
    class MemberAccessExpression;
    class ArrayAccessExpression;

    using ExpressionTypes = SumTypeList
    <
        AdjustExpression,
        ArgumentExpression,
        FunctionArgumentExpression,
        FunctionCallExpression,
        CastExpression,
        ConditionalExpression,
        IdentifierExpression,
        IntegerLiteralExpression,
        StringLiteralExpression,
        StructLiteralExpression,
        UnaryExpression,
        BinaryExpression,
        MemberAccessExpression,
        ArrayAccessExpression
    >;

    using ExpressionSum = MakeSum<ASTMethods, ExpressionTypes>::Type;
    using ExpressionBase = SumMemberBase<ExpressionSum>;

    class AdjustExpression : public ExpressionBase
    {
    public:
        enum class AdjustType
        {
            Increment,
            Decrement
        };

    public:
        AdjustType adjust_type;
        SumHandle target;

    public:
        std::string format(void) const;
        SumHandle evaluate(const Storage&, Context&) const;
        AdjustExpression optimize(const Storage&) const;
        bool check_types(const Storage&) const;
    };

    class ArgumentExpression : public ExpressionBase
    {
    public:
        Type type;
        IdentifierExpression identifier;

    public:
        std::string format(void) const;
        ArgumentExpression evaluate(const Storage&, Context&) const;
        ArgumentExpression optimize(const Storage&) const;
        bool check_types(const Storage&) const;
    };

    class FunctionArgumentExpression : public ExpressionBase
    {
    public:
        Type type;
        SumReference<IdentifierExpression> identifier;
        SumHandle value;

    public:
        std::string format(void) const;
        FunctionArgumentExpression evaluate(const Storage&, Context&) const;
        FunctionArgumentExpression optimize(const Storage&) const;
        bool check_types(const Storage&) const;
    };

}

#endif
