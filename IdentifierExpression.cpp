#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"

#include <format>

namespace hz
{
	Expression::Type IdentifierExpression::etype() const
	{
		return Expression::Type::IDENTIFIER;
	}

	std::string IdentifierExpression::string() const
	{
		return std::format("identifier ({})", name);
	}

	Expression* IdentifierExpression::copy() const
	{
		return new IdentifierExpression{ *this };
	}

	void IdentifierExpression::generate(Allocation* allocation)
	{
		auto variable_symbol = AS_VARIABLE_SYMBOL(parser->reference_symbol(Symbol::Type::VARIABLE, name));
		allocation->copy(variable_symbol->allocation);
	}

	Expression* IdentifierExpression::optimize()
	{
		return nullptr;
	}
}
