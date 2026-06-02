#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include <ast/AST.h>
#include <ast/expression/Expression.h>
#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/models/Token.h>

// Haze Parser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;
	class FunctionSymbol;
	class ArgumentSymbol;
	class VariableSymbol;
	class StructSymbol;
	class DefineSymbol;
	class LabelSymbol;
	class AdjustExpression;

	enum class ParserType;

	class Parser
		: public ServiceTag<Parser>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, SymbolExporter, CommandLineOptions>
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	protected:
		std::string filepath;

	protected:
		// explicitly mutable!
		Token& lookbehind();
		Token& peek();
		Token& lookahead();

	protected:
		Token consume(TokenKind);
		std::vector<Token> fetch_until(TokenKind);

	protected:
		enum class Precedence
		{
			MINIMUM,
			ASSIGN,
			EQUALITY,
			COMPARE,
			TERM, // + (or - later)
			FACTOR, // * (or / later maybe)
		};

	protected:
		Node* parse_dotdefine_command();

	protected:
		ExpressionReference<IdentifierExpression> parse_identifier_expression();
		ExpressionReference<IntegerLiteralExpression> parse_integerliteral_expression();
		ExpressionReference<StringLiteralExpression> parse_string_expression();
		ExpressionReference<FunctionCallExpression> parse_functioncall_expression();
		ExpressionHandle parse_parenthesis_expression();

	protected:
		AdjustExpression* parse_increment_expression();
		AdjustExpression* parse_decrement_expression();

	protected:
		ExpressionHandle parse_expression_optimized();
		ExpressionHandle parse_expression();

	private:
		ExpressionHandle parse_generic_expression();
		ExpressionHandle parse_infix_expression(ExpressionHandle, Precedence);

	public:
		virtual ParserType ptype() const = 0;
		virtual std::vector<Node*> parse() = 0;

	public:
		void reload(const std::vector<Token>&, const std::string&);

	public:
		Parser(const std::string&);
		virtual ~Parser();
	};
}

#endif
