#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string_view>

#include "Token.h"
#include "Symbol.h"
#include "Expression.h"
#include "Statement.h"
#include "Function.h"

namespace hz
{
	class Parser
	{
	private:
		const std::vector<Token>& tokens;
		std::size_t cursor = 0;
		std::vector<Function*> program;

	private:
		std::vector<Symbol*> symbol_table;
		void add_symbol(Symbol::Type, std::string_view);

	public:
		bool query_symbol(std::string_view);
		Symbol* reference_symbol(Symbol::Type, std::string_view);

	public:
		explicit Parser(const std::vector<Token>& tokens)
			: tokens(tokens)
		{
		}

	public:
		std::vector<Function*>& get_ast()
		{
			return program;
		}

	private:
		const Token& peek() const;
		const Token& next() const;
		std::string_view consume(const TokenType&);

	private:
		enum Precedence
		{
			PRECEDENCE_MIN,
			PRECEDENCE_TERM, // + (- later)
			PRECEDENCE_FACTOR, // * (/ later)
		};

	private:
		Expression* parse_identifier_expression();
		Expression* parse_intlit_expression();
		Expression* parse_paren_expression();
		std::vector<Expression*> parse_function_call_arguments();
		Expression* parse_function_call_expression();

		Expression* new_parse_primary();
		Expression* new_parse_expression();
		Expression* new_parse_expression1(Expression*, Precedence);

		Expression* parse_expression();

		Statement* parse_null_statement();
		Statement* parse_vardecl_statement();
		Statement* parse_compound_statement();
		Statement* parse_return_statement();

		Statement* parse_statement();
		std::vector<Statement*> parse_statements();

		Expression* parse_argument();
		std::vector<Expression*> parse_arguments();

		Function* parse_function();
		std::vector<Function*> parse_functions();

		void parse_program();

	private:
		//void print_expression(Expression*);
		//void print_statement(const Statement const*, int);
		//void print_function(Function*);
		//void print_program();

	public:
		//void print_ast();
		void parse();
	};

}

#endif //HAZE_PARSER_H
