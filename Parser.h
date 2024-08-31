#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "ParserType.h"

#include <string>
#include <vector>
#include <unordered_map>

// Haze Parser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;
	class Node;
	class Expression;
	class IdentifierExpression;
	class IntegerLiteralExpression;
	class StringExpression;
	class FunctionCallExpression;
	class AdjustExpression;

	enum class SymbolType;

	class Parser
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	private:
		std::unordered_map<std::string, Symbol*> symbol_table;

	protected:
		void add_symbol(SymbolType, std::string, Token&);

	public:
		SymbolType query_symbol_type(std::string, Token&);
		Symbol* reference_symbol(SymbolType, std::string, Token&, bool = false);

	protected:
		Token& lookbehind();
		Token& peek();
		Token& lookahead();

	protected:
		Token consume(TokenType);
		std::vector<Token> fetch_until(TokenType);

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
		IdentifierExpression* parse_identifier_expression();
		IntegerLiteralExpression* parse_integerliteral_expression();
		StringExpression* parse_string_expression();
		FunctionCallExpression* parse_functioncall_expression();
		Expression* parse_parenthesis_expression();

	protected:
		AdjustExpression* parse_increment_expression();
		AdjustExpression* parse_decrement_expression();

	protected:
		Expression* parse_expression_optimized();
		Expression* parse_expression();

	private:
		Expression* parse_generic_expression();
		Expression* parse_infix_expression(Expression*, Precedence);

	public:
		virtual std::vector<Node*> parse() = 0;

	public:
		Parser(const std::vector<Token>&, const std::string&);
		virtual ~Parser();
	};

	extern Parser* _parser;
}

#endif
