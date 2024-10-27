import std;

#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Symbol.h"
#include "ErrorReporter.h"

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
		// NOTE: old method
		/*const auto type = _parser->query_symbol_type(name, _token);

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
		}*/

		_generator->read_local(allocation->read(), name);
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
			return Variable*o_node(value);
		}

		_error_reporter->post_error(std::format("identifier `{}` is undefined", name), _token);
		return nullptr;
	}
}
