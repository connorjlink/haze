#ifndef HAZE_IDENTIFIEREXPRESSION_H
#define HAZE_IDENTIFIEREXPRESSION_H

#include "Expression.h"
#include "Segment.h"

#include <string>
#include <string_view>

namespace hz
{
    class IdentifierExpression : public Expression
    {
    private:
        std::string_view name;

    public:
        virtual Expression* copy() final override;
        virtual Expression::Type etype() final override;
        virtual std::string string() final override;
        virtual Segment generate(Allocation*) final override;
        virtual Expression* optimize() final override;
    };
}

#endif //HAZE_IDENTIFIEREXPRESSION_H
