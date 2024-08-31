#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Constants.h"
#include "ErrorReporter.h"

#include <format>

// Haze IntegerLiteralExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    ExpressionType IntegerLiteralExpression::etype() const
    {
        return ExpressionType::INTEGER_LITERAL;
    }

    IntegerLiteralExpression* IntegerLiteralExpression::copy() const
    {
        return new IntegerLiteralExpression{ *this };
    }

    void IntegerLiteralExpression::generate(Allocation* allocation)
    {
        if (value > WORD_MAX - 1)
        {
            _error_reporter->post_warning(std::format("integer literal {} out of range", value), _token);
        }

        allocation->write(static_cast<std::uint8_t>(value));
    }

    Expression* IntegerLiteralExpression::optimize()
    {
        return nullptr;
    }

    Node* IntegerLiteralExpression::evaluate(Context* context) const
    {
        return copy();
    }
}
