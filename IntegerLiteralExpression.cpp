#include "IntegerLiteralExpression.h"

#include <fmt/format.h>

namespace hz
{
    Expression::Type IntegerLiteralExpression::etype() const
    {
        return Expression::Type::INTEGER_LITERAL;
    }

    Expression* IntegerLiteralExpression::copy() const
    {
        return new IntegerLiteralExpression{ value };
    }

    std::string IntegerLiteralExpression::string() const
    {
        return fmt::format("integer literal ({})", value);
    }

    void IntegerLiteralExpression::generate(Allocation* allocation)
    {
        allocation->write(value);
    }
}
