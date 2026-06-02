#ifndef HAZE_SYMBOL_DATABASE_H
#define HAZE_SYMBOL_DATABASE_H

#include <ast/expression/Expression.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolExporter.h>
#include <utility/Sum.h>

// Haze SymbolDatabase.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;
	class FunctionSymbol;
	class ArgumentSymbol;
	class VariableSymbol;
	class DefineSymbol;
	class LabelSymbol;
	class StructSymbol;
	class Expression;
	class ArgumentExpression;

	struct Token;

	enum class SymbolKind;

	FORWARD_DECLARE_SUM(Expression);


	class SymbolDatabase
		: public SingletonTag<SymbolDatabase>
		, public InjectSingleton<ErrorReporter, SymbolExporter>
	{
	private:
		std::unordered_map<std::string, Symbol*> table;

	public:
		Symbol* add_symbol(SymbolKind, const std::string&, const Token&);
		FunctionSymbol* add_function(const std::string&, const Token&, TypeHandle, const std::vector<ArgumentExpression*>& arguments);
		ArgumentSymbol* add_argument(const std::string&, const Token&, TypeHandle);
		VariableSymbol* add_variable(const std::string&, const Token&);
		DefineSymbol* add_define(const std::string&, const Token&);
		LabelSymbol* add_label(const std::string&, const Token&);
		StructSymbol* add_struct(const std::string&, const Token&);

	public:
		SymbolKind query_symbol_type(const std::string&, const Token&);

	private:
		Symbol* internal_reference_symbol(bool, SymbolKind, const std::string&, const Token&, bool = false);

	public:
		Symbol* try_reference_symbol(SymbolKind, const std::string&, const Token&, bool = false);
		Symbol* reference_symbol(SymbolKind, const std::string&, const Token&, bool = false);
		FunctionSymbol* reference_function(const std::string&, const Token&, bool = false);
		ArgumentSymbol* reference_argument(const std::string&, const Token&, bool = false);
		VariableSymbol* reference_variable(const std::string&, const Token&, bool = false);
		DefineSymbol* reference_define(const std::string&, const Token&, bool = false);
		LabelSymbol* reference_label(const std::string&, const Token&, bool = false);
		StructSymbol* reference_struct(const std::string&, const Token&, bool = false);

	public:
		bool has_symbol(const std::string&);
		bool is_mapped_identifier(ExpressionHandle);

	public:
		SymbolDatabase();
	};
}

#endif
