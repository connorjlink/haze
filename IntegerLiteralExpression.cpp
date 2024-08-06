#include "IntegerLiteralExpression.h"

#include "Allocation.h"

#include "Constants.h"
#include "Log.h"

#include <format>

namespace hz
{
    ExpressionType IntegerLiteralExpression::etype() const
    {
        return ExpressionType::INTEGER_LITERAL;
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
        if (value > WORD_MAX - 1)
        {
            Log::error(std::format("integer literal {} is too large", value));
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
