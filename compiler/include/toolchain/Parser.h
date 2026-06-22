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
	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Symbol)


	struct Parser
		: public ServiceTag<Parser>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, SymbolExporter, CommandLineOptions>
	{
	protected:
		std::size_t cursor = 0;
		std::vector<Token> tokens;
		ExpressionStorage& expression_storage;

	protected:
		template<typename Self>
		decltype(auto) lookbehind(this Self&& self)
		{
			if (cursor <= 0)
			{
				USE_SAFE(ErrorReporter)->post_uncorrectable(
					"invalid token backtrack", self.peek());
			}

			return self.tokens[self.cursor - 1];
		}

		template<typename Self>
		decltype(auto) peek(this Self&& self)
		{
			return self.tokens[self.cursor];
		}

		template<typename Self>
		decltype(auto) lookahead(this Self&& self)
		{
			if (self.cursor >= self.tokens.size() - 1)
			{
				USE_SAFE(ErrorReporter)->post_uncorrectable(
					"unexpectedly reached the end of file", self.peek());
			}

			return self.tokens[self.cursor + 1];
		}

	protected:
		const Token& consume(TokenKind);
		std::vector<Token> fetch_until(TokenKind);

	protected:
		ExpressionReference<IdentifierExpression> parse_identifier_expression();
		ExpressionReference<IntegerLiteralExpression> parse_integer_literal_expression();
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
		std::vector<typename Self::NodeHandle> parse_generic_expression(this Self&& self)
		{
#pragma message("TODO: implement generic expression parsing")
		}

	public:
		template<typename Self>
		std::vector<typename Self::NodeHandle> parse(this Self&& self)
		{
			return std::forward<Self>(self).parse_implementation();
		}

	public:
		void reload(std::vector<Token>);

	public:
		Parser(const std::filesystem::path&, ExpressionStorage&);
		~Parser();
	};
}

#endif
