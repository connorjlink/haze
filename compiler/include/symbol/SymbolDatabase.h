#ifndef HAZE_SYMBOL_DATABASE_H
#define HAZE_SYMBOL_DATABASE_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/models/Symbol.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/models/Environment.h>
#include <utility/Sum.h>

// Haze SymbolDatabase.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct SymbolKind;

	struct Token;

#pragma message("TODO: move symboldatabase information over to the Generator class and hook up environment push/pop logic with function enter/exit scope")

	struct SymbolDatabase
		: public SingletonTag<SymbolDatabase>
		, public InjectSingleton<ErrorReporter, SymbolExporter>
	{
	private:
		Environment<Symbol> environment;

	public:
		Symbol add_symbol(SymbolKind, std::string_view, const Token&);
		FunctionHandle add_function_symbol(std::string_view, const Token&, TypeHandle, std::vector<ExpressionHandle>);
		DeclarationHandle add_declaration(std::string_view, const Token&, TypeHandle);
		StatementHandle add_statement(std::string_view, const Token&);
		ExpressionHandle add_expression(std::string_view, const Token&, TypeHandle);
		TypeHandle add_type(std::string_view, const Token&, TypeHandle);
		
	public:
		Symbol try_reference_symbol(std::string_view, const Token&, bool = false);
		Symbol reference_symbol(std::string_view, const Token&, bool = false);
		FunctionHandle try_reference_function(std::string_view, const Token&, bool = false);
		DeclarationHandle try_reference_declaration(std::string_view, const Token&, bool = false);
		StatementHandle try_reference_statement(std::string_view, const Token&, bool = false);
		ExpressionHandle try_reference_expression(std::string_view, const Token&, bool = false);
		TypeHandle try_reference_type(std::string_view, const Token&, bool = false);

	public:
		SymbolKind query_symbol_type(std::string_view, const Token&);

	private:
		Symbol internal_reference_symbol(bool, SymbolKind, std::string_view, const Token&, bool = false);

	public:
		bool has_symbol(std::string_view);
		bool is_mapped_identifier(ExpressionHandle);

	public:
		SymbolDatabase();
	};
}

#endif
