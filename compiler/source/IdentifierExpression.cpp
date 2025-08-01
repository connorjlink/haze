import std;

#include <allocator/Allocation.h>
#include <ast/IdentifierExpression.h>
#include <runtime/Context.h>
#include <runtime/Evaluator.h>
#include <error/ErrorReporter.h>

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

		REQUIRE_SAFE(RuntimeAllocator)->read_local(allocation->read(), name);
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

			if (value != nullptr)
			{
				return variable_to_node(value);
			}

			USE_SAFE(ErrorReporter)->post_error(std::format("identifier `{}` is unassigned", name), _token);
			return nullptr;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format("identifier `{}` is undefined", name), _token);
		return nullptr;
	}
}
