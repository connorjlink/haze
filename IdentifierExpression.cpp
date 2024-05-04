#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"

#include <fmt/format.h>

namespace hz
{
    Expression::Type IdentifierExpression::etype() const
    {
        return Expression::Type::IDENTIFIER;
    }

    std::string IdentifierExpression::string() const
    {
        return fmt::format("identifier ({})", name);
    }

    Expression* IdentifierExpression::copy() const
    {
        return new IdentifierExpression{ *this };
    }

    Segment IdentifierExpression::generate(Allocation* allocation)
    {
        auto symbol = parser->reference_symbol(Symbol::Type::VARIABLE, name);
        return Allocator::write(allocation, Allocator::read(symbol->allocation()));
    }

    Expression* IdentifierExpression::optimize()
    {
        auto variable_symbol = AS_VARIABLE(parser->reference_symbol(Symbol::Type::VARIABLE, name));
        if (variable_symbol->vtype() == VariableSymbol::Type::CONSTANT)
        {
            return new IntegerLiteralExpression{ AS_CONSTANT_VARIABLE(variable_symbol)->value };
        }

        return nullptr;
    }
}
