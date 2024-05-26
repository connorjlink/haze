#include "IntegerLiteralExpression.h"
#include "Allocation.h"

#include <format>

namespace hz
{
    Expression::Type IntegerLiteralExpression::etype() const
    {
        return Expression::Type::INTEGER_LITERAL;
    }

    std::string IntegerLiteralExpression::string() const
    {
        return std::format("integer literal ({})", value);
    }

    IntegerLiteralExpression* IntegerLiteralExpression::copy() const
    {
        return new IntegerLiteralExpression{ value };
    }

    void IntegerLiteralExpression::generate(Allocation* allocation)
    {
        allocation->write(value);
    }

    Expression* IntegerLiteralExpression::optimize()
    {
        return nullptr;
    }
}
