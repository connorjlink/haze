import std;

#include <ast/Expression.h>
#include <ast/IdentifierExpression.h>
#include <ast/ArgumentExpression.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <symbol/Symbol.h>
#include <error/ErrorReporter.h>

// Haze SymbolDatabase.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Symbol* SymbolDatabase::add_symbol(SymbolKind type, const std::string& name, const Token& token)
	{
		// does the symbol already exist in the registry?
		if (table.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("symbol `{}` was already defined as a {}",
				name, _symbol_type_map.at(table.at(name)->ytype())), token);
			return nullptr;
		}

		Symbol* new_symbol = nullptr;

		using enum SymbolKind;
		switch (type)
		{
			case FUNCTION: new_symbol = table[name] = new FunctionSymbol{ name, token, nullptr }; break;
			case ARGUMENT: new_symbol = table[name] = new ArgumentSymbol{ name, token, nullptr }; break;
			case VARIABLE: new_symbol = table[name] = new VariableSymbol{ name, token, nullptr, nullptr }; break;
			case DEFINE: new_symbol = table[name] = new DefineSymbol{ name, token, nullptr, ExtendedInteger{} }; break;
			case LABEL: new_symbol = table[name] = new LabelSymbol{ name, token, 0 }; break;

			default:
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid symbol type `{}`", token.text), token);
			} break;
		}

		if (new_symbol != nullptr)
		{
			USE_SAFE(SymbolExporter)->enqueue(new_symbol, token);
		}

		return new_symbol;
	}

	FunctionSymbol* SymbolDatabase::add_function(const std::string& name, const Token& location, TypeHandle return_type, const std::vector<ArgumentExpression*>& arguments)
	{
		auto function_symbol = AS_FUNCTION_SYMBOL(add_symbol(SymbolKind::FUNCTION, name, location));
		function_symbol->return_type = return_type;
		function_symbol->arguments = arguments;
		return function_symbol;
	}

	ArgumentSymbol* SymbolDatabase::add_argument(const std::string& name, const Token& location, TypeHandle type)
	{
		auto argument_symbol = AS_ARGUMENT_SYMBOL(add_symbol(SymbolKind::ARGUMENT, name, location));
		argument_symbol->type = type;
		return argument_symbol;
	}

	VariableSymbol* SymbolDatabase::add_variable(const std::string& name, const Token& location)
	{
		return AS_VARIABLE_SYMBOL(add_symbol(SymbolKind::VARIABLE, name, location));
	}

	DefineSymbol* SymbolDatabase::add_define(const std::string& name, const Token& location)
	{
		return AS_DEFINE_SYMBOL(add_symbol(SymbolKind::DEFINE, name, location));
	}

	LabelSymbol* SymbolDatabase::add_label(const std::string& name, const Token& location)
	{
		return AS_LABEL_SYMBOL(add_symbol(SymbolKind::LABEL, name, location));
	}

	StructSymbol* SymbolDatabase::add_struct(const std::string& name, const Token& location)
	{
		return AS_STRUCT_SYMBOL(add_symbol(SymbolKind::STRUCT, name, location));
	}


	SymbolKind SymbolDatabase::query_symbol_type(const std::string& name, const Token& location)
	{
		if (!table.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"symbol `{}` is undefined", name), location);
			return SymbolKind::VARIABLE;
		}

		return table.at(name)->ytype();
	}

	Symbol* SymbolDatabase::internal_reference_symbol(bool log_errors, SymbolKind type, const std::string& name, const Token& location, bool mark_visited)
	{
		if (!_symbol_type_map.contains(type))
		{
			if (log_errors)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid symbol type `{}`", location.text), location);
			}
			return nullptr;
		}

		if (!table.contains(name))
		{
			if (log_errors)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"symbol `{}` is undefined", name), location);
			}
			return nullptr;
		}

		auto symbol = table.at(name);

		if (symbol->ytype() != type)
		{
			if (log_errors)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format("symbol `{}` was defined as a {} but referenced as a {}",
					name, symbol->ytype(), type), location);
			}
			return nullptr;
		}

		if (mark_visited)
		{
			symbol->was_referenced = true;
		}

		return symbol;
	}

	Symbol* SymbolDatabase::try_reference_symbol(SymbolKind type, const std::string& name, const Token& location, bool mark_visited)
	{
		return internal_reference_symbol(false, type, name, location, mark_visited);
	}

	Symbol* SymbolDatabase::reference_symbol(SymbolKind type, const std::string& name, const Token& location, bool mark_visited)
	{
		return internal_reference_symbol(true, type, name, location, mark_visited);
	}

	FunctionSymbol* SymbolDatabase::reference_function(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::FUNCTION, name, location, mark_visited);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		return function_symbol;
	}

	ArgumentSymbol* SymbolDatabase::reference_argument(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::ARGUMENT, name, location, mark_visited);
		const auto argument_symbol = AS_ARGUMENT_SYMBOL(symbol);

		return argument_symbol;
	}

	VariableSymbol* SymbolDatabase::reference_variable(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::VARIABLE, name, location, mark_visited);
		const auto variable_symbol = AS_VARIABLE_SYMBOL(symbol);

		return variable_symbol;
	}

	DefineSymbol* SymbolDatabase::reference_define(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::DEFINE, name, location, mark_visited);
		const auto define_symbol = AS_DEFINE_SYMBOL(symbol);

		return define_symbol;
	}

	LabelSymbol* SymbolDatabase::reference_label(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::LABEL, name, location, mark_visited);
		const auto label_symbol = AS_LABEL_SYMBOL(symbol);

		return label_symbol;
	}

	StructSymbol* SymbolDatabase::reference_struct(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolKind::STRUCT, name, location, mark_visited);
		const auto struct_symbol = AS_STRUCT_SYMBOL(symbol);

		return struct_symbol;
	}

	bool SymbolDatabase::has_symbol(const std::string& name)
	{
		return table.contains(name);
	}

	bool SymbolDatabase::is_mapped_identifier(ExpressionHandle expression)
	{
		if (expression->etype() == ExpressionType::IDENTIFIER)
		{
			const auto identifier_expression = AS_IDENTIFIER_EXPRESSION(expression);

			if (USE_UNSAFE(SymbolDatabase)->has_symbol(identifier_expression->name))
			{
				auto symbol_type = USE_UNSAFE(SymbolDatabase)->query_symbol_type(identifier_expression->name, NULL_TOKEN);

				if (symbol_type == SymbolKind::VARIABLE)
				{
					auto symbol = USE_UNSAFE(SymbolDatabase)->reference_variable(identifier_expression->name, NULL_TOKEN);
					auto variable_symbol = AS_VARIABLE_SYMBOL(symbol);

					// does the referenced variable have a register mapping currently?
					if (variable_symbol->allocation != nullptr)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	SymbolDatabase::SymbolDatabase()
	{
		table = {};
	}
}