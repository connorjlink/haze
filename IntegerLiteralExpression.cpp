#include "IntegerLiteralExpression.h"

#include <fmt/format.h>

namespace hz
{
    IntegerLiteralExpression::IntegerLiteralExpression(int value)
            : value(value)
    {
    }

    Expression* IntegerLiteralExpression::copy()
    {
        return new IntegerLiteralExpression{ value };
    }

    Expression::Type IntegerLiteralExpression::etype()
    {
        return Expression::Type::INTEGER_LITERAL;
    }

    std::string IntegerLiteralExpression::string()
    {
        return fmt::format("integer literal ({})", value);
    }

    Segment IntegerLiteralExpression::generate(Allocation* allocation)
    {
        return Allocator::write(allocation, value);
    }
}