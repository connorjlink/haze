#include "FunctionCallExpression.h"

#include <fmt/format.h>

namespace hz
{
    Expression::Type FunctionCallExpression::etype()
    {
        return Expression::Type::FUNCTION_CALL;
    }

    std::string FunctionCallExpression::string()
    {
        return fmt::format("function call ({})", name);
    }

    Segment FunctionCallExpression::generate(Allocation* allocation)
    {
        Segment result{};

        //TODO: finish generating the function code here

        return result;
    }

    Expression* FunctionCallExpression::optimize()
    {
        //TODO: implement determination if a function is constexpr
        //in that case, we can fold it down to a single integer literal node
        //for now, we don't have that functionality so just terminate further optimization of this node
        return nullptr;
    }
}