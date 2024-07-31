#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "Symbol.h"

#include "Expression.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "FunctionCallExpression.h"

#include "Statement.h"

#include "Node.h"
#include "Function.h"
#include "InstructionCommand.h"

#define AS_COMPILER_PARSER(x) static_cast<CompilerParser*>(x)
#define AS_ASSEMBLER_PARSER(x) static_cast<AssemblerParser*>(x)

#define AS_FUNCTION(x) static_cast<Function*>(x)
#define AS_INSTRUCTION(x) static_cast<Instruction*>(x)

namespace hz
{
	class Parser
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	private:
		std::vector<Symbol*> symbol_table;

	protected:
		void add_symbol(Symbol::Type, std::string);

	public:
		bool query_symbol(std::string);
		Symbol* reference_symbol(Symbol::Type, std::string, bool = false);

	protected:
		const Token& peek() const;
		const Token& lookahead() const;
		std::string consume(TokenType);
		std::vector<Token> fetchUntil(TokenType);

	protected:
		enum class Precedence
		{
			MINIMUM,
			TERM, // + (or - later)
			FACTOR, // * (or / later maybe)
		};

	protected:
		IdentifierExpression* parse_identifier_expression();
		IntegerLiteralExpression* parse_integerliteral_expression();
		FunctionCallExpression* parse_functioncall_expression();
		Expression* parse_parenthesis_expression();

	protected:
		Expression* parse_expression_optimized();
		Expression* parse_expression();

	private:
		Expression* parse_generic_expression();
		Expression* parse_infix_expression(Expression*, Precedence);

	public:
		virtual std::vector<Node*> parse() = 0;

	public:
		explicit Parser(std::vector<Token>&& tokens)
			: cursor{ 0 }, tokens{ std::move(tokens) }
		{
		}
	};

	extern Parser* _parser;
}

#endif //HAZE_PARSER_H
