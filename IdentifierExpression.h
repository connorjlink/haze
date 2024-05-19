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
    public:
        std::string_view name;

    public:
        IdentifierExpression(std::string_view name)
            : name(name)
        {
        }

    public:
        virtual Expression::Type etype() const final override;
        virtual std::string string() const final override;
        virtual Expression* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Expression* optimize() final override;
    };
}

#endif //HAZE_IDENTIFIEREXPRESSION_H
