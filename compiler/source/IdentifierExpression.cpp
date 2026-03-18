import std;

#include <allocator/Allocation.h>
#include <ast/IdentifierExpression.h>
#include <runtime/Context.h>
#include <runtime/Evaluator.h>
#include <type/Type.h>
#include <symbol/Symbol.h>
#include <error/ErrorReporter.h>

// Haze IdentifierExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType IdentifierExpression::etype() const
	{
		return ExpressionType::IDENTIFIER;
	}

	TypeType IdentifierExpression::ttype() const
	{
		const auto type = USE_SAFE(SymbolDatabase)->query_symbol_type(name, _token);
		switch (type)
		{
			case SymbolType::FUNCTION:
			{
				const auto function_symbol = USE_SAFE(SymbolDatabase)->reference_function(name, _token);
				return function_symbol->return_type->ttype();
			} break;
			case SymbolType::ARGUMENT:
			{
				const auto argument_symbol = USE_SAFE(SymbolDatabase)->reference_argument(name, _token);
				return argument_symbol->type->ttype();
			} break;
			case SymbolType::VARIABLE:
			{
				const auto variable_symbol = USE_SAFE(SymbolDatabase)->reference_variable(name, _token);
				return variable_symbol->type->ttype();
			} break;
			case SymbolType::DEFINE:
			{
				const auto define_symbol = USE_SAFE(SymbolDatabase)->reference_define(name, _token);
				return define_symbol->type->ttype();
			} break;
			case SymbolType::STRUCT:
			{
				return TypeType::STRUCT;
			} break;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid identifier symbol type `{}`", _symbol_type_map.at(type)), _token);
		return TypeType::VOID;
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
		const auto symbol = USE_SAFE(SymbolDatabase)->try_reference_symbol(SymbolType::DEFINE, name, _token);
		if (symbol)
		{

		}

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
