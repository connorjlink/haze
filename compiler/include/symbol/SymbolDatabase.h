#ifndef HAZE_SYMBOL_DATABASE_H
#define HAZE_SYMBOL_DATABASE_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolExporter.h>
#include <utility/Sum.h>

// Haze SymbolDatabase.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct SymbolKind;

	struct FunctionSymbol;
	struct ArgumentSymbol;
	struct VariableSymbol;
	struct DefineSymbol;
	struct LabelSymbol;
	struct StructOrUnionSymbol;
	struct Token;

	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Symbol)
	FORWARD_DECLARE_SUM(Type)


#pragma message("TODO: redo symbol database to use direct AST reference instead of individual symbol types")

	struct SymbolDatabase
		: public SingletonTag<SymbolDatabase>
		, public InjectSingleton<ErrorReporter, SymbolExporter>
	{
	private:
		std::unordered_map<std::string, SymbolHandle> table;

	public:
		SymbolHandle add_symbol(SymbolKind, std::string_view, const Token&);
		SymbolReference<FunctionSymbol> add_function(std::string_view, const Token&, TypeHandle, std::vector<ExpressionHandle> arguments);
		SymbolReference<ArgumentSymbol> add_argument(std::string_view, const Token&, TypeHandle);
		SymbolReference<VariableSymbol> add_variable(std::string_view, const Token&);
		SymbolReference<DefineSymbol> add_define(std::string_view, const Token&);
		SymbolReference<LabelSymbol> add_label(std::string_view, const Token&);
		SymbolReference<StructOrUnionSymbol> add_struct(std::string_view, const Token&);

	public:
		SymbolHandle try_reference_symbol(SymbolKind, std::string_view, const Token&, bool = false);
		SymbolHandle reference_symbol(SymbolKind, std::string_view, const Token&, bool = false);
		SymbolReference<FunctionSymbol> reference_function(std::string_view, const Token&, bool = false);
		SymbolReference<ArgumentSymbol> reference_argument(std::string_view, const Token&, bool = false);
		SymbolReference<VariableSymbol> reference_variable(std::string_view, const Token&, bool = false);
		SymbolReference<DefineSymbol> reference_define(std::string_view, const Token&, bool = false);
		SymbolReference<LabelSymbol> reference_label(std::string_view, const Token&, bool = false);
		SymbolReference<StructOrUnionSymbol> reference_struct(std::string_view, const Token&, bool = false);

	public:
		SymbolKind query_symbol_type(std::string_view, const Token&);

	private:
		SymbolHandle internal_reference_symbol(bool, SymbolKind, std::string_view, const Token&, bool = false);

	public:
		bool has_symbol(std::string_view);
		bool is_mapped_identifier(ExpressionHandle);

	public:
		SymbolDatabase();
	};
}

#endif
