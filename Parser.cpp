#include "Parser.h"
#include "Log.h"
#include "IdentifierExpression.h"
#include "Token.h"
#include "Statement.h"

#include <iostream>
#include <format>

#include "BinaryExpression.h"
#include "FunctionCallExpression.h"
#include "IntegerLiteralExpression.h"

#define DISCARD (void) 

namespace
{
	enum
	{
		TL_PROGRAM,
		TL_FUNCTION,
		TL_STATEMENT,
	};

	void ast_indent(int times)
	{
		for (auto i = 0; i < 2 * times; i++)
		{
			std::cout << ' ';
		}
	}

	void ast_print(const std::string& text, int indentation)
	{
		std::cout << "[AST] ";
		::ast_indent(indentation);
		std::cout << text;
	}

	void ast_nl()
	{
		std::cout << std::endl;
	}

	bool is_binary_operator(hz::TokenType type)
	{
		return type == hz::TokenType::PLUS || type == hz::TokenType::STAR;
	}

	auto find(std::string_view name, std::vector<hz::Symbol*>& symbols)
	{
		return std::find_if(symbols.begin(), symbols.end(), [&](auto symbol)
		{
			return symbol->name == name;
		});
	}
}

namespace hz
{
	void Parser::add_symbol(Symbol::Type type, std::string_view name)
	{
		if (query_symbol(name))
		{
			Log::error(std::format("symbol {} is multiply defined", name));
		}

		using enum Symbol::Type;
		switch (type)
		{
			case FUNCTION: symbol_table.emplace_back(new FunctionSymbol{ name }); break;
			case ARGUMENT: symbol_table.emplace_back(new ArgumentSymbol{ name }); break;
			case VARIABLE: symbol_table.emplace_back(new VariableSymbol{ name }); break;
		}
	}

	bool Parser::query_symbol(std::string_view name)
	{
		return ::find(name, symbol_table) != std::end(symbol_table);
	}

	Symbol* Parser::reference_symbol(Symbol::Type type, std::string_view name)
	{
		auto symbol = ::find(name, symbol_table);
		if (symbol == std::end(symbol_table))
		{
			//TODO: ensure to output undefined symbol type in the ternary if more enum variants are added
			Log::error(std::format("{} {} is undefined", type == Symbol::Type::VARIABLE ? "variable" : "function", name));
		}

		if ((*symbol)->ytype() == type)
		{
			return *symbol;
		}

		Log::error(std::format("the specified symbol {} does not match the expected type", name));
	}

	const Token& Parser::peek() const
	{
		return tokens[cursor];
	}

	const Token& Parser::next() const
	{
		if (cursor < tokens.size())
		{
			return tokens[cursor + 1];
		}

		Log::error(std::format("({}) unexpectedly reached the end of file", peek().line));
	}

	std::string_view Parser::consume(const TokenType& token)
	{
		const auto& current = peek();
		if (current.type == token)
		{
			cursor++;
			return std::string_view{ current.value.value_or("") };
		}

		Log::error(std::format("({}) expected token '{}' but got '{}'", current.line, debug_tokens.at(token),
			((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.value.value() : debug_tokens.at(current.type))));
	}

	Expression* Parser::parse_identifier_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);
		return new IdentifierExpression{ name };
	}

	Expression* Parser::parse_intlit_expression()
	{
		//TODO: optimize the C++ conversion to integer? (maybe dont require a copy)
		const auto value = std::stoi(std::string{ consume(TokenType::INT) });
		return new IntegerLiteralExpression{ value };
	}

	Expression* Parser::parse_paren_expression()
	{
		DISCARD consume(TokenType::LPAREN);
		const auto expression = new_parse_expression();
		DISCARD consume(TokenType::RPAREN);
		return expression;
	}

	std::vector<Expression*> Parser::parse_function_call_arguments()
	{
		std::vector<Expression*> arguments;

		while (peek().type != TokenType::RPAREN)
		{
			arguments.emplace_back(new_parse_expression());

			if (peek().type != TokenType::RPAREN)
			{
				DISCARD consume(TokenType::COMMA);

				if (peek().type == TokenType::RPAREN)
				{
					Log::error(std::format("({}) expected another argument", peek().line));
				}
			}
		}

		return arguments;
	}

	Expression* Parser::parse_function_call_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);

		DISCARD consume(TokenType::LPAREN);
		const auto arguments = parse_function_call_arguments();
		DISCARD consume(TokenType::RPAREN);

		return new FunctionCallExpression{ name, arguments };
	}

	Expression* Parser::parse_expression()
	{
		Expression* expression;

		using enum TokenType;
		switch (peek().type)
		{
			case LPAREN:
			{
				expression = parse_paren_expression();
			} break;

			case INT:
			{
				expression = parse_intlit_expression();
			} break;

			case IDENTIFIER:
			{
				if (next().type == LPAREN)
				{
					expression = parse_function_call_expression();
					break;
				}

				expression = parse_identifier_expression();
			} break;

			default:
			{
				Log::error(std::format("({}) unexpected expression type", peek().line));
			} break;
		}

		return expression;
	}

	Expression* Parser::new_parse_primary()
	{
		return parse_expression();
	}

	Expression* Parser::new_parse_expression()
	{
		return new_parse_expression1(new_parse_primary(), PRECEDENCE_MIN);
	}

	Expression* Parser::new_parse_expression1(Expression* left, Precedence min_precedence)
	{
		static const std::unordered_map<TokenType, Precedence> precedences =
		{
			{ TokenType::PLUS, PRECEDENCE_TERM },
			{ TokenType::STAR, PRECEDENCE_FACTOR },
		};

		static const std::unordered_map<TokenType, BinaryExpression::Type> binary_expression_types =
		{
			{ TokenType::PLUS, BinaryExpression::Type::PLUS },
			{ TokenType::MINUS, BinaryExpression::Type::MINUS },
			{ TokenType::STAR, BinaryExpression::Type::TIMES },
		};

		do
		{
			const auto& next = peek();

			if (!is_binary_operator(next.type) || precedences.at(next.type) < min_precedence)
			{
				break;
			}

			consume(next.type);

			auto right = new_parse_primary();

			do
			{
				const auto& lookahead = peek();

				if (!is_binary_operator(lookahead.type) || precedences.at(lookahead.type) <= precedences.at(next.type))
				{
					break;
				}

				//TODO: maybe add precedence+1 for the next call of the recursive function
				right = new_parse_expression1(right, precedences.at(lookahead.type));

			} while(true);

			switch (next.type)
			{
				case TokenType::PLUS: left = new PlusBinaryExpression{ left, right }; break;
				case TokenType::MINUS: left = new MinusBinaryExpression{ left, right }; break;
				case TokenType::STAR: left = new TimesBinaryExpression{ left, right }; break;
			}
		} while (true);

		return left;
	}



	Statement* Parser::parse_null_statement()
	{
		DISCARD consume(TokenType::SEMICOLON);
		return nullptr;
	}

	Statement* Parser::parse_vardecl_statement()
	{
		//TODO: add other type specifiers
		DISCARD consume(TokenType::BYTE);

		const auto name = consume(TokenType::IDENTIFIER);
		symbol_table.emplace_back(new VariableSymbol{ name });

		if (peek().type == TokenType::EQUALS)
		{
			DISCARD consume(TokenType::EQUALS);
			const auto value = new_parse_expression();
			DISCARD consume(TokenType::SEMICOLON);

			return new VariableStatement{ name, value, nullptr };
		}

		DISCARD consume(TokenType::SEMICOLON);

		return new VariableStatement{ name, nullptr, nullptr };
	}

	Statement* Parser::parse_compound_statement()
	{
		DISCARD consume(TokenType::LBRACE);
		const auto statements = parse_statements();
		DISCARD consume(TokenType::RBRACE);

		return new CompoundStatement{ statements };
	}

	Statement* Parser::parse_return_statement()
	{
		DISCARD consume(TokenType::RETURN);
		const auto expression = new_parse_expression();
		DISCARD consume(TokenType::SEMICOLON);

		return new ReturnStatement{ expression, nullptr };
	}

	Statement* Parser::parse_statement()
	{
		using enum TokenType;
		switch (peek().type)
		{
			case LBRACE: return parse_compound_statement();
			case BYTE: return parse_vardecl_statement();
			case SEMICOLON: return parse_null_statement();
			case RETURN: return parse_return_statement();

			default:
			{
				Log::error(std::format("({}) unexpected statement type", peek().line));
			} break;
		}
	}

	std::vector<Statement*> Parser::parse_statements()
	{
		std::vector<Statement*> statements;

		while (peek().type != TokenType::RBRACE)
		{
			statements.emplace_back(parse_statement());
		}

		return statements;
	}

	Expression* Parser::parse_argument()
	{
		//TODO: add other type specifiers for arguments?
		DISCARD consume(TokenType::BYTE);
		const auto name = consume(TokenType::IDENTIFIER);
		add_symbol(Symbol::Type::ARGUMENT, name);

		return new IdentifierExpression{ name };
	}

	std::vector<Expression*> Parser::parse_arguments()
	{
		std::vector<Expression*> arguments;

		while (peek().type != TokenType::RPAREN)
		{
			arguments.emplace_back(parse_argument());

			if (peek().type != TokenType::RPAREN)
			{
				DISCARD consume(TokenType::COMMA);

				if (peek().type == TokenType::RPAREN)
				{
					Log::error(std::format("({}) expected another argument but got ')'", peek().line));
				}
			}
		}

		return arguments;
	}

	Function* Parser::parse_function()
	{
		DISCARD consume(TokenType::FUNCTION);
		//TODO: support return types other than byte (particularly void functions)
		DISCARD consume(TokenType::BYTE);

		const auto name = consume(TokenType::IDENTIFIER);
		add_symbol(Symbol::Type::FUNCTION, name);

		DISCARD consume(TokenType::EQUALS);

		DISCARD consume(TokenType::LPAREN);
		const auto arguments = parse_arguments();
		DISCARD consume(TokenType::RPAREN);

		const auto body = parse_compound_statement();

		return new Function{ name, arguments, body };
	}

	std::vector<Function*> Parser::parse_functions()
	{
		std::vector<Function*> functions;

		while (peek().type != TokenType::END)
		{
			functions.emplace_back(parse_function());
		}

		return functions;
	}

	void Parser::parse_program()
	{
		program = parse_functions();

		if (std::find_if(program.begin(), program.end(), [&](auto function)
			{
				return (function->name == "main");
			}) == std::end(program))
		{
			Log::error("no main() function was defined");
		}
	}

	void Parser::parse()
	{
		parse_program();
	}


	/*void Parser::print_statement(const Statement const* statement, int indent)
	{
		::ast_print("Statement: ", indent);

		using enum Statement::Type;
		switch (statement->stype())
		{
			case COMPOUND:
			{
				const auto compound_statement = AS_COMPOUND_STATEMENT(statement);

				std::cout << "compound:";
				::ast_nl();

				for (const auto statement : compound_statement->substatements)
				{
					print_statement(statement, indent + 2);
				}
			} break;

			case VARIABLE:
			{
				const auto variable_declaration = AS_VARIABLE_DECLARATION_STATEMENT(statement);

				std::cout << std::format("variable declaration ({} = {})",
					variable_declaration->name, variable_declaration->expression->string());

				::ast_nl();
			} break;

			case RETURN:
			{
				const auto return_statement = AS_RETURN_STATEMENT(statement);

				std::cout << std::format("return ({})", return_statement->expression->string());

				::ast_nl();
			}
		}
	}*/

	/*void Parser::print_function(Function* function)
	{
		::ast_print("Function: ", TL_FUNCTION);

		std::cout << std::format(" ({})\n");

		print_statement(function->get_body(), TL_STATEMENT);
	}*/

	/*void Parser::print_program()
	{
		if (!program.empty())
		{
			::ast_print("Program: ", TL_PROGRAM);
			::ast_nl();

			for (const auto function : program)
			{
				print_function(function);
			}
		}
	}*/

	/*void Parser::print_ast()
	{
		print_program();
	}*/
}