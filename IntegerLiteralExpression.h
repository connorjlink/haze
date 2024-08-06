#ifndef HAZE_INTEGERLITERALEXPRESSION_H
#define HAZE_INTEGERLITERALEXPRESSION_H

#include "Expression.h"
#include "Segment.h"

#include <string>

namespace hz
{
    class IntegerLiteralExpression : public Expression
    {
    public:
        std::uint16_t value;

    public:
        IntegerLiteralExpression(int value)
            : value(value)
        {
        }

    public:
        virtual ExpressionType etype() const final override;
        virtual IntegerLiteralExpression* copy() const final override;
        virtual std::string string() const final override;
        virtual void generate(Allocation*) final override;
        virtual Expression* optimize() final override;
        virtual Node* evaluate(Context* context) const final override;
    };
}

#endif //HAZE_INTEGERLITERALEXPRESSION_H
