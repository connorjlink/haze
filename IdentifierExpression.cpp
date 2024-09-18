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
		const auto type = _parser->query_symbol_type(name, _token);

		// TODO: restructure this with our new setup
		//1.) arguments are passed through the stack with its own second
		//2.) local variables are stored on the stack so new logic is needed to compute offsets
		static_assert(false);

		using enum SymbolType;
		switch (type)
		{
			case SymbolType::VARIABLE:
			{
				auto variable_symbol = AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, _token));
				variable_symbol->allocation->copy_into(allocation);
			} break;

			case SymbolType::ARGUMENT:
			{
				auto argument_symbol = AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT, name, _token));
				argument_symbol->allocation->copy_into(allocation);
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
