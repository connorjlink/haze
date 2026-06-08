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
	enum class SymbolKind;

	class FunctionSymbol;
	class ArgumentSymbol;
	class VariableSymbol;
	class DefineSymbol;
	class LabelSymbol;
	class StructOrUnionSymbol;

	struct Token;

	FORWARD_DECLARE_SUM(Symbol);
	FORWARD_DECLARE_SUM(Type);
	FORWARD_DECLARE_SUM(Expression);


#pragma message("TODO: redo symbol database to use direct AST reference instead of individual symbol types")

	class SymbolDatabase
		: public SingletonTag<SymbolDatabase>
		, public InjectSingleton<ErrorReporter, SymbolExporter>
	{
	private:
		std::unordered_map<std::string, SymbolHandle> table;

	public:
		SymbolHandle add_symbol(SymbolKind, const std::string&, const Token&);
		SymbolReference<FunctionSymbol> add_function(const std::string&, const Token&, TypeHandle, const std::vector<ExpressionReference<ArgumentExpression>>& arguments);
		SymbolReference<ArgumentSymbol> add_argument(const std::string&, const Token&, TypeHandle);
		SymbolReference<VariableSymbol> add_variable(const std::string&, const Token&);
		SymbolReference<DefineSymbol> add_define(const std::string&, const Token&);
		SymbolReference<LabelSymbol> add_label(const std::string&, const Token&);
		SymbolReference<StructOrUnionSymbol> add_struct(const std::string&, const Token&);

	public:
		SymbolKind query_symbol_type(const std::string&, const Token&);

	private:
		SymbolHandle internal_reference_symbol(bool, SymbolKind, const std::string&, const Token&, bool = false);

	public:
		SymbolHandle try_reference_symbol(SymbolKind, const std::string&, const Token&, bool = false);
		SymbolHandle reference_symbol(SymbolKind, const std::string&, const Token&, bool = false);
		SymbolReference<FunctionSymbol> reference_function(const std::string&, const Token&, bool = false);
		SymbolReference<ArgumentSymbol> reference_argument(const std::string&, const Token&, bool = false);
		SymbolReference<VariableSymbol> reference_variable(const std::string&, const Token&, bool = false);
		SymbolReference<DefineSymbol> reference_define(const std::string&, const Token&, bool = false);
		SymbolReference<LabelSymbol> reference_label(const std::string&, const Token&, bool = false);
		SymbolReference<StructOrUnionSymbol> reference_struct(const std::string&, const Token&, bool = false);

	public:
		bool has_symbol(const std::string&);
		bool is_mapped_identifier(ExpressionHandle);

	public:
		SymbolDatabase();
	};
}

#endif
