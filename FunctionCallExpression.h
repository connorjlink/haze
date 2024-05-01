#ifndef HAZE_FUNCTIONCALLEXPRESSION_H
#define HAZE_FUNCTIONCALLEXPRESSION_H

#include "Expression.h"

#include <string>
#include <string_view>

namespace hz
{
    class FunctionCallExpression : public Expression
    {
    private:
        std::string_view name;
        std::vector<Expression*> arguments;

    public:
        virtual Expression::Type etype() final override;
        virtual std::string string() final override;
        virtual Segment generate(Allocation*) final override;
        virtual Expression* optimize() final override;
    };
}

#endif //HAZE_FUNCTIONCALLEXPRESSION_H
