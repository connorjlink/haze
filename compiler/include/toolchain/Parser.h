#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include <ast/AST.h>
#include <ast/expression/Expression.h>
#include <cli/CommandLineOptions.h>
#include <command/Command.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/models/Token.h>

// Haze Parser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Symbol;
	struct FunctionSymbol;
	struct ArgumentSymbol;
	struct VariableSymbol;
	struct StructOrUnionSymbol;
	struct DefineSymbol;
	struct LabelSymbol;
	struct AdjustExpression;

	enum struct ParserType;

	template<typename T>
	struct Parser
		: public ServiceTag<Parser<T>>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, SymbolExporter, CommandLineOptions>
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	protected:
		std::filesystem::path filepath;

	protected:
		// explicitly mutable!
		Token& lookbehind();
		Token& peek();
		Token& lookahead();

	protected:
		Token consume(TokenKind);
		std::vector<Token> fetch_until(TokenKind);

	protected:
#pragma message("TODO: replace this with primary/postfix/binary/unary operator precedence")
		enum struct Precedence
		{
			MINIMUM,
			ASSIGN,
			EQUALITY,
			COMPARE,
			TERM, // + (or - later)
			FACTOR, // * (or / later maybe)
		};

	protected:

	protected:
		ExpressionReference<IdentifierExpression> parse_identifier_expression();
		ExpressionReference<IntegerLiteralExpression> parse_integerliteral_expression();
		ExpressionReference<StringLiteralExpression> parse_string_expression();
		ExpressionReference<FunctionCallExpression> parse_functioncall_expression();
		ExpressionHandle parse_parenthesis_expression();

	protected:
		ExpressionReference<AdjustExpression> parse_increment_expression();
		ExpressionReference<AdjustExpression> parse_decrement_expression();

	protected:
		ExpressionHandle parse_expression_optimized();
		ExpressionHandle parse_expression();

	private:
		ExpressionHandle parse_generic_expression();
		ExpressionHandle parse_infix_expression(ExpressionHandle, Precedence);

	public:
		template<typename Self>
			requires std::derived_from<Self, Parser<T>>
		std::vector<typename T::NodeType> parse(this Self&& self)
		{
			return self.parse();
		}

	public:
		void reload(const std::vector<Token>&, const std::string&);

	public:
		Parser(const std::string&);
		virtual ~Parser();
	};
}

#endif
