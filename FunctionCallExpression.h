#ifndef HAZE_FUNCTIONCALLEXPRESSION_H
#define HAZE_FUNCTIONCALLEXPRESSION_H

#include "Expression.h"

#include <string>
#include <string_view>

namespace hz
{
    class FunctionCallExpression : public Expression
    {
    public:
        std::string_view name;
        std::vector<Expression*> arguments;

    public:
        virtual Expression::Type etype() const final override;
        virtual std::string string() const final override;
        virtual void generate(Allocation*) final override;
        virtual Expression* optimize() final override;
    };
}

#endif //HAZE_FUNCTIONCALLEXPRESSION_H
