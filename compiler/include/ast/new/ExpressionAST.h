#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/new/AST.h>

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

    using ExpressionMethods = ASTMethods<ExpressionTypes::Anchor>;

    using ExpressionSum = MakeSum<ExpressionMethods, ExpressionTypes>::Type;
}

#endif
