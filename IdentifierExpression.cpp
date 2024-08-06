#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"

#include <format>

namespace hz
{
	ExpressionType IdentifierExpression::etype() const
	{
		return ExpressionType::IDENTIFIER;
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
		auto variable_symbol = AS_VARIABLE_SYMBOL(_parser->reference_symbol(Symbol::Type::VARIABLE, name));
		allocation->copy(variable_symbol->allocation);
	}

	Expression* IdentifierExpression::optimize()
	{
		return nullptr;
	}

	Node* IdentifierExpression::evaluate(Context* context) const
	{
		for (auto& variable : context->_variables)
		{
			if (variable._name == name)
			{
				return new IntegerLiteralExpression{ variable._value };
			}
		}

		Log::error(std::format("identifier {} is undefined and cannot be evaluated", name));
	}
}
