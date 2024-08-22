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
		auto variable_symbol = AS_VARIABLE_SYMBOL(_parser->reference_symbol(Symbol::Type::VARIABLE, name));
		allocation->copy(variable_symbol->allocation);
	}

	Expression* IdentifierExpression::optimize()
	{
		// No optimizations possible for an identifier expression
		return nullptr;
	}

	Node* IdentifierExpression::evaluate(Context* context) const
	{
		if (auto it = context->variables().find(name);
			it != std::end(context->variables()))
		{
			auto value = it->second;
			return unharvest(value);
		}

		Log::error(std::format("identifier {} is undefined and cannot be evaluated", name));
	}
}
