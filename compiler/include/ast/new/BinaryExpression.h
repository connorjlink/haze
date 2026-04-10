#ifndef HAZE_BINARYEXPRESSION_H
#define HAZE_BINARYEXPRESSION_H

#include <ast/new/AST.h>

// Haze BinaryExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    struct BinaryExpression
    {
    private:
        static constexpr auto tag = ExpressionIndex<BinaryExpression>::value;

    public:
        Register left_index;   // index of left expression in the ExpressionAST storage
        Register right_index;  // index of right expression
        char op;                   // '+', '-', '*', '/' etc.

        void print(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right(ast, right_index, tag);
            left.print();
            std::cout << " " << op << " ";
            right.print();
        }

        BinaryExpression evaluate(const ExpressionStorage& ast) const
        {
            const auto left_handle = MAKE_HANDLE(ast, left_index);
            const auto right_handle = MAKE_HANDLE(ast, right_index);

            const auto left_val = left_handle.evaluate();
            const auto right_val = right_handle.evaluate();

            // TODO: perform the operation based upon 'op'

            return *this;
        }

        BinaryExpression optimize(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left_handle(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right_handle(ast, right_index, tag);
            auto left_opt = left_handle.optimize();
            auto right_opt = right_handle.optimize();
            return *this;
        }

        bool check_types(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left_handle(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right_handle(ast, right_index, tag);
            return left_handle.check_types() && right_handle.check_types();
        }
    };
}

#endif
