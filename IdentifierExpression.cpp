#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Evaluator.h"

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

	IdentifierExpression* IdentifierExpression::copy() const
	{
		return new IdentifierExpression{ *this };
	}

	void IdentifierExpression::generate(Allocation* allocation)
	{
		// try first as a variable
		if (auto variable_symbol = AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, NULL_TOKEN)))
		{
			allocation->copy(variable_symbol->allocation);
		}

		else
		{
			auto argument_symbol = AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT, name, NULL_TOKEN));
			allocation->copy(argument_symbol->allocation);
		}
	}

	Expression* IdentifierExpression::optimize()
	{
		// No optimizations possible for an identifier expression
		return nullptr;
	}

	Node* IdentifierExpression::evaluate(Context* context) const
	{
		if (context->variables().contains(name))
		{
			const auto value = context->variables().at(name);
			return unharvest(value);
		}

		Log::error(std::format("identifier {} is undefined and cannot be evaluated", name));
	}
}
