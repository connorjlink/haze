#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/new/AST.h>
#include <runtime/Context.h>
#include <type/Type.h>
#include <toolchain/Generator.h>

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
    class FloatLiteralExpression;
    class StringLiteralExpression;
    class StructOrUnionLiteralExpression;
    class UnaryExpression;
    class BinaryExpression;
    class TernaryExpression;
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
        FloatLiteralExpression,
        StringLiteralExpression,
        StructOrUnionLiteralExpression,
        UnaryExpression,
        BinaryExpression,
        TernaryExpression,
        MemberAccessExpression,
        ArrayAccessExpression
    >;

    using ExpressionSum = MakeSum<ASTMethods, ExpressionTypes>::Type;
    using ExpressionSumBase = SumMemberBase<ExpressionSum>;


    class ExpressionBase 
        : public ExpressionSumBase
        , public InjectService<Generator>
    {
    public:
        template<typename Self>
        bool check_types(this Self&& self, const Storage& ast) const
        {
            return self.get_type().is_valid();
        }
    };


    class AdjustExpression : public ExpressionBase
    {
    public:
        enum class Type
        {
            PRE_INCREMENT,
            POST_INCREMENT,
            PRE_DECREMENT,
            POST_DECREMENT,
        };

    public:
        Type adjust_type;
        SumHandle target;

    public:
        std::string format(void) const;
        void generate(const Storage&) const;
        SumHandle evaluate(const Storage&, Context&) const;
        SumHandle optimize(const Storage&) const;
        SumHandle get_type(const Storage&) const;
    };
#define MAKE_ADJUST_EXPRESSION(target, adjust_type) AdjustExpression{ adjust_type, MAKE_HANDLE(ast, target) }

    class ArgumentExpression : public ExpressionBase
    {
    public:
        Type type;
        IdentifierExpression identifier;

    public:
        std::string format(void) const;
        void generate(const Storage&) const;
        SumHandle evaluate(const Storage&, Context&) const;
        SumHandle optimize(const Storage&) const;
        SumHandle get_type(const Storage&) const;
    };
#define MAKE_ARGUMENT_EXPRESSION(type, identifier) ArgumentExpression{ type, identifier }

    class FunctionArgumentExpression : public ExpressionBase
    {
    public:
        Type type;
        SumReference<IdentifierExpression> identifier;
        SumHandle value;

    public:
        std::string format(void) const;
        void generate(const Storage&) const;
        SumHandle evaluate(const Storage&, Context&) const;
        SumHandle optimize(const Storage&) const;
        bool check_types(const Storage&) const;
    };
#define MAKE_FUNCTION_ARGUMENT_EXPRESSION(type, identifier, value) FunctionArgumentExpression{ type, MAKE_REFERENCE(ast, identifier), MAKE_HANDLE(ast, value) }

    class TernaryExpression : public ExpressionBase
    {
    public:
        SumHandle condition;
        SumHandle true_branch;
        SumHandle false_branch;

    public:
        std::string format(void) const;
        void generate(const Storage&) const;
        SumHandle evaluate(const Storage&, Context&) const;
        SumHandle optimize(const Storage&) const;
        bool check_types(const Storage&) const;
    };
#define MAKE_TERNARY_EXPRESSION(condition, true_expression, false_expression) TernaryExpression{ MAKE_HANDLE(ast, condition), MAKE_HANDLE(ast, true_expression), MAKE_HANDLE(ast, false_expression) }
}

#endif
