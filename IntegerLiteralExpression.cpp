import std;

#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Constants.h"
#include "CommandLineOptions.h"
#include "Generator.h"
#include "ErrorReporter.h"

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
        _generator->make_immediate(allocation->read(), value);
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
