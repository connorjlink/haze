#include "Parser.h"
#include "Log.h"
#include "Token.h"
#include "Allocator.h"
#include "Utility.h"

#include "CompoundStatement.h"
#include "VariableStatement.h"
#include "ReturnStatement.h"

#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "FunctionCallExpression.h"
#include "BinaryExpression.h"

#include <iostream>
#include <format>

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

	auto find(std::string name, std::vector<hz::Symbol*>& symbols)
	{
		return std::find_if(symbols.begin(), symbols.end(), [&](auto symbol)
		{
			return symbol->name == name;
		});
	}
}

namespace hz
{
	void Parser::add_symbol(Symbol::Type type, std::string name)
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
			case VARIABLE: symbol_table.emplace_back(new VariableSymbol{ name, nullptr }); break;
		}
	}

	bool Parser::query_symbol(std::string name)
	{
		return ::find(name, symbol_table) != std::end(symbol_table);
	}

	Symbol* Parser::reference_symbol(Symbol::Type type, std::string name, bool visit)
	{
		auto symbol = ::find(name, symbol_table);
		if (symbol == std::end(symbol_table))
		{
			Log::error(std::format("{} {} is undefined", 
				type == Symbol::Type::VARIABLE ? "variable" : \
					type == Symbol::Type::FUNCTION ? "function" : "argument", name));
		}

		if ((*symbol)->ytype() == type)
		{
			if (visit)
			{
				(*symbol)->was_referenced = true;
			}

			return *symbol;
		}

		Log::error(std::format("the specified symbol {} does not match the expected type", name));
	}

	const Token& Parser::peek() const
	{
		return tokens[cursor];
	}

	const Token& Parser::lookahead() const
	{
		if (cursor < tokens.size())
		{
			return tokens[cursor + 1];
		}

		Log::error(std::format("({}) unexpectedly reached the end of file", peek().line));
	}

	std::string Parser::consume(TokenType token)
	{
		const auto& current = peek();
		if (current.type == token)
		{
			cursor++;
			return current.value.value_or("UNDEFINED");
		}

		auto debug = [&](auto v)
		{
			for (const auto& [key, value] : lexeme_map)
			{
				if (value == v)
				{
					return key;
				}
			}

			Log::error("Invalid token cannot be indexed in the lexeme map");
		};

		Log::error(std::format("({}) expected token '{}' but got '{}'", current.line, debug(token),
			((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.value.value() : debug(current.type))));
	}

	IdentifierExpression* Parser::parse_identifier_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);
		return new IdentifierExpression{ name };
	}

	IntegerLiteralExpression* Parser::parse_integerliteral_expression()
	{
		return new IntegerLiteralExpression{ std::stoi(consume(TokenType::INT)) };
	}

	FunctionCallExpression* Parser::parse_functioncall_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);

		DISCARD consume(TokenType::LPAREN);
		auto arguments = AS_COMPILER_PARSER(this)->parse_arguments();
		DISCARD consume(TokenType::RPAREN);

		return new FunctionCallExpression{ name, std::move(arguments) };
	}

	Expression* Parser::parse_parenthesis_expression()
	{
		DISCARD consume(TokenType::LPAREN);
		const auto expression = parse_expression();
		DISCARD consume(TokenType::RPAREN);
		return expression;
	}


	Expression* Parser::parse_generic_expression()
	{
		Expression* expression;

		using enum TokenType;
		switch (peek().type)
		{
			case LPAREN:
			{
				expression = parse_parenthesis_expression();
			} break;

			case INT:
			{
				expression = parse_integerliteral_expression();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().type == LPAREN)
				{
					expression = parse_functioncall_expression();
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


	std::vector<Expression*> CompilerParser::parse_arguments()
	{
		std::vector<Expression*> arguments;

 		while (peek().type != TokenType::RPAREN)
		{
			arguments.emplace_back(parse_expression());

			if (peek().type != TokenType::RPAREN)
			{
				DISCARD consume(TokenType::COMMA);

				if (peek().type == TokenType::RPAREN)
				{
					Log::error(std::format("({}) expected another argument", peek().line));
				}
			}
		}

		//TODO: figure out how to support passing more than 4 arguments on the stack
		//realistically, this is possible if we push parameters to the stack in batches of 4 or fewer at a time
		if (arguments.size() > 4)
		{
			Log::error(std::format("({}) function defined with {} arguments but can have at most 4", peek().line, arguments.size()));
		}

		return arguments;
	}



	Expression* Parser::parse_expression()
	{
		return parse_infix_expression(parse_generic_expression(), Precedence::MINIMUM);
	}

	Expression* Parser::parse_infix_expression(Expression* left, Precedence min_precedence)
	{
		static const std::unordered_map<TokenType, Precedence> precedences =
		{
			{ TokenType::PLUS, Precedence::TERM },
			 { TokenType::MINUS, Precedence::TERM },
			{ TokenType::STAR, Precedence::FACTOR },
		};

		do
		{
			const auto& next = peek();

			if (!is_binary_operator(next.type) || precedences.at(next.type) < min_precedence)
			{
				break;
			}

			consume(next.type);

			auto right = parse_expression();

			do
			{
				const auto& lookahead = peek();

				if (!is_binary_operator(lookahead.type) || precedences.at(lookahead.type) <= precedences.at(next.type))
				{
					break;
				}

				//TODO: maybe add precedence+1 for the next call of the recursive function
				right = parse_infix_expression(right, precedences.at(lookahead.type));

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



	Statement* CompilerParser::parse_null_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::SEMICOLON);
		return nullptr;
	}

	Statement* CompilerParser::parse_variabledeclaration_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::BYTE);

		const auto identifier = consume(TokenType::IDENTIFIER);
		add_symbol(Symbol::Type::VARIABLE, identifier);

		if (peek().type == TokenType::EQUALS)
		{
			DISCARD consume(TokenType::EQUALS);
			const auto value = parse_expression();
			DISCARD consume(TokenType::SEMICOLON);

			return new VariableStatement{ identifier, value, nullptr };
		}

		DISCARD consume(TokenType::SEMICOLON);

		return new VariableStatement{ identifier, nullptr, nullptr };
	}

	Statement* CompilerParser::parse_compound_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::LBRACE);
		auto statements = parse_statements(enclosing_function);
		DISCARD consume(TokenType::RBRACE);

		return new CompoundStatement{ std::move(statements) };
	}

	Statement* CompilerParser::parse_return_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::RETURN);
		const auto expression = parse_expression();
		DISCARD consume(TokenType::SEMICOLON);

		return new ReturnStatement{ enclosing_function, expression, nullptr };
	}

	Statement* CompilerParser::parse_statement(std::string enclosing_function)
	{
		using enum TokenType;
		switch (peek().type)
		{
			case LBRACE: return parse_compound_statement(enclosing_function);
			case BYTE: return parse_variabledeclaration_statement(enclosing_function);
			case SEMICOLON: return parse_null_statement(enclosing_function);
			case RETURN: return parse_return_statement(enclosing_function);

			default:
			{
				Log::error(std::format("({}) unexpected statement type", peek().line));
			} break;
		}
	}

	std::vector<Statement*> CompilerParser::parse_statements(std::string enclosing_function)
	{
		std::vector<Statement*> statements;

		while (peek().type != TokenType::RBRACE)
		{
			statements.emplace_back(parse_statement(enclosing_function));
		}

		return statements;
	}

	Expression* CompilerParser::parse_argument()
	{
		DISCARD consume(TokenType::BYTE);
		const auto name = consume(TokenType::IDENTIFIER);
		add_symbol(Symbol::Type::ARGUMENT, name);

		return new IdentifierExpression{ name };
	}

	Node* CompilerParser::parse_function()
	{
		DISCARD consume(TokenType::FUNCTION);

		ReturnType return_type;
		if (peek().type == TokenType::BYTE)
		{
			DISCARD consume(TokenType::BYTE);
			return_type = ReturnType::BYTE;
		}

		else if (peek().type == TokenType::NVR)
		{
			DISCARD consume(TokenType::NVR);
			return_type = ReturnType::NVR;
		}

		else
		{
			Log::error("expected return type `byte` or `nvr`");
		}

		auto name = consume(TokenType::IDENTIFIER);

		//TODO: implement a more efficient way of modifying the return type than this mess
		add_symbol(Symbol::Type::FUNCTION, name);
		AS_FUNCTION_SYMBOL(reference_symbol(Symbol::Type::FUNCTION, name))->return_type = return_type;

		DISCARD consume(TokenType::EQUALS);

		DISCARD consume(TokenType::LPAREN);
		auto arguments = parse_arguments();
		DISCARD consume(TokenType::RPAREN);

		auto body = parse_compound_statement(name);

		return new Function{ name, return_type, std::move(arguments), body };
	}

	std::vector<Node*> CompilerParser::parse_functions()
	{
		std::vector<Node*> functions;

		while (peek().type != TokenType::END)
		{
			functions.emplace_back(parse_function());
		}

		return functions;
	}

	std::vector<Node*> CompilerParser::parse()
	{
		auto program = parse_functions();

		if (auto it = std::find_if(program.begin(), program.end(), [&](auto function)
			{
				return (AS_FUNCTION(function)->name == "main");
			}); it != std::end(program))
		{
			//at bare minimum, we must compile main() since it's the entrypoint
			DISCARD reference_symbol(Symbol::Type::FUNCTION, "main", true);

			if constexpr (OPTIMIZE_AST)
			{
				for (auto& node : program)
				{
					while (true)
					{
				        if (auto node_optimized = node->optimize())
				        {
				            node = node_optimized;
				        }

				        else
				        {
				            break;
				        }
					}
				}	    	
			}

			return program;
		}

		Log::error("no main() function was defined");
	}

	std::vector<Node*> AssemblerParser::parse()
	{
		return parse_instructions();
	}





	//TODO
	Node* AssemblerParser::parse_instruction()
	{
		return nullptr;
	}

	std::vector<Node*> AssemblerParser::parse_instructions()
	{
		std::vector<Node*> instructions;

		while (peek().type != TokenType::END)
		{
			instructions.emplace_back(parse_instruction());
		}

		return instructions;
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