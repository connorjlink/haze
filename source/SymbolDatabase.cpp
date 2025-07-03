import std;

#include "SymbolDatabase.h"
#include "SymbolExporter.h"
#include "Symbol.h"
#include "ErrorReporter.h"

// Haze SymbolDatabase.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Symbol* SymbolDatabase::add_symbol(SymbolType type, const std::string& name, const Token& location)
	{
		// does the symbol already exist in the registry?
		if (_table.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("symbol `{}` was already defined as a {}",
				name, _symbol_type_map.at(_table.at(name)->ytype())), location);
			return nullptr;
		}

		Symbol* new_symbol = nullptr;

		using enum SymbolType;
		switch (type)
		{
			case FUNCTION: new_symbol = _table[name] = new FunctionSymbol{ name, nullptr }; break;
			case ARGUMENT: new_symbol = _table[name] = new ArgumentSymbol{ name, nullptr }; break;
			case VARIABLE: new_symbol = _table[name] = new VariableSymbol{ name, nullptr, nullptr }; break;
			case DEFINE: new_symbol = _table[name] = new DefineSymbol{ name, nullptr, ExtendedInteger{} }; break;
			case LABEL: new_symbol = _table[name] = new LabelSymbol{ name, 0 }; break;

			default:
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid symbol type `{}`", location.text), location);
			} break;
		}

		if (new_symbol != nullptr)
		{
			USE_SAFE(SymbolExporter)->enqueue(new_symbol, location);
		}

		return new_symbol;
	}

	FunctionSymbol* SymbolDatabase::add_function(const std::string& name, const Token& location, Type* return_type)
	{
		return AS_FUNCTION_SYMBOL(add_symbol(SymbolType::FUNCTION, name, location));
	}

	ArgumentSymbol* SymbolDatabase::add_argument(const std::string& name, const Token& location)
	{
		return AS_ARGUMENT_SYMBOL(add_symbol(SymbolType::ARGUMENT, name, location));
	}

	VariableSymbol* SymbolDatabase::add_variable(const std::string& name, const Token& location)
	{
		return AS_VARIABLE_SYMBOL(add_symbol(SymbolType::VARIABLE, name, location));
	}

	DefineSymbol* SymbolDatabase::add_define(const std::string& name, const Token& location)
	{
		return AS_DEFINE_SYMBOL(add_symbol(SymbolType::DEFINE, name, location));
	}

	LabelSymbol* SymbolDatabase::add_label(const std::string& name, const Token& location)
	{
		return AS_LABEL_SYMBOL(add_symbol(SymbolType::LABEL, name, location));
	}

	StructSymbol* SymbolDatabase::add_struct(const std::string& name, const Token& location)
	{
		return AS_STRUCT_SYMBOL(add_symbol(SymbolType::STRUCT, name, location));
	}


	SymbolType SymbolDatabase::query_symbol_type(const std::string& name, const Token& location)
	{
		if (!_table.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"symbol `{}` is undefined", name), location);
			return SymbolType::VARIABLE;
		}

		return _table.at(name)->ytype();
	}

	Symbol* SymbolDatabase::internal_reference_symbol(bool log_errors, SymbolType type, const std::string& name, const Token& location, bool mark_visited)
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

		if (!_table.contains(name))
		{
			if (log_errors)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"symbol `{}` is undefined", name), location);
			}
			return nullptr;
		}

		auto symbol = _table.at(name);

		if (symbol->ytype() != type)
		{
			if (log_errors)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format("symbol `{}` was defined as a {} but referenced as a {}",
					name, _symbol_type_map.at(symbol->ytype()), _symbol_type_map.at(type)), location);
			}
			return nullptr;
		}

		if (mark_visited)
		{
			symbol->was_referenced = true;
		}

		return symbol;
	}

	Symbol* SymbolDatabase::try_reference_symbol(SymbolType type, const std::string& name, const Token& location, bool mark_visited)
	{
		return internal_reference_symbol(false, type, name, location, mark_visited);
	}

	Symbol* SymbolDatabase::reference_symbol(SymbolType type, const std::string& name, const Token& location, bool mark_visited)
	{
		return internal_reference_symbol(true, type, name, location, mark_visited);
	}

	FunctionSymbol* SymbolDatabase::reference_function(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::FUNCTION, name, location, mark_visited);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		return function_symbol;
	}

	ArgumentSymbol* SymbolDatabase::reference_argument(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::ARGUMENT, name, location, mark_visited);
		const auto argument_symbol = AS_ARGUMENT_SYMBOL(symbol);

		return argument_symbol;
	}

	VariableSymbol* SymbolDatabase::reference_variable(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::VARIABLE, name, location, mark_visited);
		const auto variable_symbol = AS_VARIABLE_SYMBOL(symbol);

		return variable_symbol;
	}

	DefineSymbol* SymbolDatabase::reference_define(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::DEFINE, name, location, mark_visited);
		const auto define_symbol = AS_DEFINE_SYMBOL(symbol);

		return define_symbol;
	}

	LabelSymbol* SymbolDatabase::reference_label(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::LABEL, name, location, mark_visited);
		const auto label_symbol = AS_LABEL_SYMBOL(symbol);

		return label_symbol;
	}

	StructSymbol* SymbolDatabase::reference_struct(const std::string& name, const Token& location, bool mark_visited)
	{
		const auto symbol = reference_symbol(SymbolType::STRUCT, name, location, mark_visited);
		const auto struct_symbol = AS_STRUCT_SYMBOL(symbol);

		return struct_symbol;
	}

	bool SymbolDatabase::has_symbol(const std::string& name)
	{
		return _table.contains(name);
	}

	SymbolDatabase::SymbolDatabase()
	{
		_table = {};
	}
}