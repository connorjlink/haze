#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "Symbol.h"
#include "Expression.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "FunctionCallExpression.h"
#include "AdjustExpression.h"
#include "ErrorReporter.h"
#include "Statement.h"
#include "Node.h"
#include "Function.h"
#include "InstructionCommand.h"
#include "ParserType.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>

namespace hz
{
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
		std::string consume(TokenType);
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
		Parser(const std::vector<Token>& tokens, const std::string& filepath)
			: cursor{ 0 }, tokens{ tokens }
		{
			_error_reporter->open_context(filepath, "parsing");
		}

		~Parser()
		{
			_error_reporter->close_context();
		}
	};

	extern Parser* _parser;
}

#endif //HAZE_PARSER_H
