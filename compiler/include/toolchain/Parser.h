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
#include <utility/Sum.h>

// Haze Parser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct ParserType;

	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Symbol)


	struct Parser
		: public ServiceTag<Parser>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, SymbolExporter, CommandLineOptions>
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;
		const ExpressionStorage& storage;

	protected:
		// explicitly mutable!
		Token& lookbehind();
		Token& peek();
		Token& lookahead();

	protected:
		Token consume(TokenKind);
		std::vector<Token> fetch_until(TokenKind);

	protected:
		ExpressionReference<IdentifierExpression> parse_identifier_expression();
		ExpressionReference<IntegerLiteralExpression> parse_integerliteral_expression();
		ExpressionReference<StringLiteralExpression> parse_string_literal_expression();
		ExpressionReference<CharacterLiteralExpression> parse_character_literal_expression();
		ExpressionReference<FunctionCallExpression> parse_functioncall_expression();
		ExpressionHandle parse_parenthesis_expression();

	protected:
		ExpressionHandle parse_expression();
		ExpressionHandle parse_expression_optimized();

	private:
		ExpressionHandle parse_infix_expression(ExpressionHandle, Precedence);
		ExpressionHandle parse_generic_expression();


		template<typename Self>
		std::vector<typename Self::NodeType> parse_generic_expression(this Self&& self)
		{
#pragma message("TODO: implement generic expression parsing")
		}

	public:
		template<typename Self>
		std::vector<typename Self::NodeType> parse(this Self&& self)
		{
			return std::forward<Self>(self).parse_implementation();
		}

	public:
		Parser(std::vector<Token>, const ExpressionStorage&);
		~Parser();
	};
}

#endif
