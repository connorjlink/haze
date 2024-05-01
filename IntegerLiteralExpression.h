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
        int value;

    public:
        virtual Expression* copy() final override;
        virtual Expression::Type etype() final override;
        virtual std::string string() final override;
        virtual Segment generate(Allocation*) final override;
    };
}

#endif //HAZE_INTEGERLITERALEXPRESSION_H
