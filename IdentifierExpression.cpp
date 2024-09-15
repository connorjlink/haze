#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "ErrorReporter.h"
#include "Symbol.h"

import std;

// Haze IdentifierExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType IdentifierExpression::etype() const
	{
		return ExpressionType::IDENTIFIER;
	}

	IdentifierExpression* IdentifierExpression::copy() const
	{
		return new IdentifierExpression{ *this };
	}

	void IdentifierExpression::generate(Allocation* allocation)
	{
		auto type = _parser->query_symbol_type(name, _token);

		using enum SymbolType;
		switch (type)
		{
			case SymbolType::VARIABLE:
			{
				auto variable_symbol = AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, _token));
				allocation->copy(variable_symbol->allocation);
			} break;

			case SymbolType::ARGUMENT:
			{
				auto argument_symbol = AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT, name, _token));
				allocation->copy(argument_symbol->allocation);
			} break;

			default:
			{
				_error_reporter->post_error("invalid identifier symbol type", _token);
			} break;
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

		_error_reporter->post_error(std::format("identifier `{}` is undefined", name), _token);
		return nullptr;
	}
}
