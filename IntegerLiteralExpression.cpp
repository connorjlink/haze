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
        using enum ArchitectureType;
        switch (_options->_architecture)
        {
            case HAZE:
            {
                if (value > WORD_MAX - 1)
                {
                    _error_reporter->post_warning(std::format("integer literal {} out of range", value), _token);
                    return;
                }
            } break;

            case X86:
            {
                if (value > x86::DWORD_MAX)
                {
                    _error_reporter->post_warning(std::format("integer literal {} out of range", value), _token);
                    return;
                }
            } break;
        }

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
