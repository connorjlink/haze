#include "IntegerLiteralExpression.h"
#include "Allocation.h"

#include <format>

namespace hz
{
    Expression::Type IntegerLiteralExpression::etype() const
    {
        return Expression::Type::INTEGER_LITERAL;
    }

    IntegerLiteralExpression* IntegerLiteralExpression::copy() const
    {
        return new IntegerLiteralExpression{ value };
    }

    std::string IntegerLiteralExpression::string() const
    {
        return std::format("integer literal ({})", value);
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
