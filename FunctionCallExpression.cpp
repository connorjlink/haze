#include "FunctionCallExpression.h"

#include <format>

namespace hz
{
    Expression::Type FunctionCallExpression::etype() const
    {
        return Expression::Type::FUNCTION_CALL;
    }

    std::string FunctionCallExpression::string() const
    {
        return std::format("function call ({})", name);
    }

    FunctionCallExpression* FunctionCallExpression::copy() const
    {
        return new FunctionCallExpression{ *this };
    }

    void FunctionCallExpression::generate(Allocation* allocation)
    {
        //TODO: finish generating the function code here
    }

    Expression* FunctionCallExpression::optimize()
    {
        //TODO: implement determination if a function is constexpr
        //in that case, we can fold it down to a single integer literal node
        //for now, we don't have that functionality so just terminate further optimization of this node
        return nullptr;
    }
}