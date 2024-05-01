#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"

#include <fmt/format.h>

namespace hz
{
    Expression* IdentifierExpression::copy()
    {
        return new IdentifierExpression{ *this };
    }

    Expression::Type IdentifierExpression::etype()
    {
        return Expression::Type::IDENTIFIER;
    }

    std::string IdentifierExpression::string()
    {
        return fmt::format("identifier ({})", name);
    }

    Segment IdentifierExpression::generate(Allocation* allocation)
    {
        auto symbol = parser->reference_symbol(SymbolType::VARIABLE, name);
        return Allocator::write(allocation, Allocator::read(symbol->allocation()));
    }

    Expression* IdentifierExpression::optimize()
    {
        auto variable_symbol = static_cast<VariableSymbol*>(parser->reference_symbol(SymbolType::VARIABLE, name));
        if (variable_symbol->type() == VariableSymbolType::CONSTANT)
        {
            return new IntegerLiteralExpression{ static_cast<ConstantVariableSymbol*>(variable_symbol)->value };
        }

        return nullptr;
    }
}
