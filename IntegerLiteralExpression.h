#ifndef HAZE_INTEGERLITERALEXPRESSION_H
#define HAZE_INTEGERLITERALEXPRESSION_H

#include "Expression.h"

// Haze IntegerLiteralExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class IntegerLiteralExpression : public Expression
    {
    public:
        std::uint32_t value;

    public:
        IntegerLiteralExpression(std::uint32_t value, Token token)
            : Expression{ token }, value{ value }
        {
        }

    public:
        virtual ExpressionType etype() const final override;
        virtual IntegerLiteralExpression* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Expression* optimize() final override;
        virtual Node* evaluate(Context* context) const final override;
    };
}

#endif
