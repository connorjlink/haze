import std;

#include "CompilerParser.h"
#include "AssemblerParser.h"
#include "VariableStatement.h"
#include "CompoundStatement.h"
#include "ReturnStatement.h"
#include "InlineAsmStatement.h"
#include "WhileStatement.h"
#include "ForStatement.h"
#include "IfStatement.h"
#include "ExpressionStatement.h"
#include "PrintStatement.h"
#include "HookStatement.h"
#include "NullStatement.h"
#include "ExitStatement.h"
#include "StringExpression.h"
#include "ArgumentExpression.h"
#include "FileManager.h"
#include "CommandLineOptions.h"
#include "Symbol.h"
#include "ErrorReporter.h"

// Haze CompilerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Statement* CompilerParser::parse_statement(std::string enclosing_function)
	{
		using enum TokenType;
		switch (peek().type)
		{
			case LBRACE: return parse_compound_statement(enclosing_function);
			case BYTE: return parse_variabledeclaration_statement(enclosing_function);
			case SEMICOLON: return parse_null_statement(enclosing_function);
			case RETURN: return parse_return_statement(enclosing_function);
			case ASM: return parse_inline_asm_statement(enclosing_function);
			case FOR: return parse_for_statement(enclosing_function);
			case WHILE: return parse_while_statement(enclosing_function);
			case IF: return parse_if_statement(enclosing_function);
			case PRINT: return parse_print_statement(enclosing_function);
			case DOTHOOK: return parse_hook_statement(enclosing_function);
			case DOTUNHOOK: return parse_unhook_statement(enclosing_function);

			// if nothing else worked, try to fall back on parsing any expression followed by a semicolon
			default: return parse_expression_statement(enclosing_function);
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

	Statement* CompilerParser::parse_null_statement(std::string enclosing_function)
	{
		const auto semicolon_token = consume(TokenType::SEMICOLON);
		return new NullStatement{ semicolon_token };
	}

	Statement* CompilerParser::parse_variabledeclaration_statement(std::string enclosing_function)
	{
		const auto type = parse_type_specifier();
		const auto identifier_token = consume(TokenType::IDENTIFIER);
		add_symbol(SymbolType::VARIABLE, identifier_token.value, lookbehind());

		if (peek().type == TokenType::EQUALS)
		{
			consume(TokenType::EQUALS);

			const auto value = parse_expression();
			
			consume(TokenType::SEMICOLON);

			return new VariableStatement{ type, identifier_token.value, value, nullptr, value->_token };
		}

		consume(TokenType::SEMICOLON);

		return new VariableStatement{ type, identifier_token.value, nullptr, nullptr, identifier_token };
	}

	Statement* CompilerParser::parse_compound_statement(std::string enclosing_function)
	{
		const auto lbrace_token = consume(TokenType::LBRACE);

		auto statements = parse_statements(enclosing_function);

		consume(TokenType::RBRACE);

		return new CompoundStatement{ std::move(statements), lbrace_token };
	}

	Statement* CompilerParser::parse_return_statement(std::string enclosing_function)
	{
		const auto return_token = consume(TokenType::RETURN);

		const auto expression = parse_expression();

		consume(TokenType::SEMICOLON);

		// NOTE: special case for `main()`:
		// return statements are really an exit from the entire program
		// this is because we don't internally bootstrap-wrap `main()` with `_main()`
		// like many real C implementations do (e.g., VC++)
		if (enclosing_function == "main")
		{
			return new ExitStatement{ expression, expression->_token };
		}

		return new ReturnStatement{ enclosing_function, expression, nullptr, return_token };
	}

	Statement* CompilerParser::parse_inline_asm_statement(std::string enclosing_function)
	{
		const auto asm_token = consume(TokenType::ASM);
		consume(TokenType::LBRACE);

		auto assembly = fetch_until(TokenType::RBRACE);
		assembly.emplace_back(TokenType::END, "eof", peek().line, peek().column);

		auto assembler_parser = new AssemblerParser{ std::move(assembly), _file_manager->_current_file };
		auto commands = assembler_parser->parse();

		consume(TokenType::RBRACE);

		return new InlineAsmStatement{ std::move(commands), assembler_parser, asm_token };
	}

	Statement* CompilerParser::parse_while_statement(std::string enclosing_function)
	{
		consume(TokenType::WHILE);
		consume(TokenType::LPAREN);

		auto condition = parse_expression();

		consume(TokenType::RPAREN);

		auto body = parse_statement(enclosing_function);

		return new WhileStatement{ condition, body, condition->_token };
	}

	Statement* CompilerParser::parse_for_statement(std::string enclosing_function)
	{
		consume(TokenType::FOR);
		consume(TokenType::LPAREN);

		auto initialization = parse_variabledeclaration_statement(enclosing_function);
		auto condition = parse_expression();

		consume(TokenType::SEMICOLON);

		auto expression = parse_expression();

		consume(TokenType::RPAREN);

		auto body = parse_statement(enclosing_function);

		return new ForStatement{ initialization, condition, expression, body, initialization->_token };
	}

	Statement* CompilerParser::parse_if_statement(std::string enclosing_function)
	{
		consume(TokenType::IF);
		consume(TokenType::LPAREN);

		auto condition = parse_expression();

		consume(TokenType::RPAREN);

		auto if_body = parse_statement(enclosing_function);
		Statement* else_body = nullptr;

		if (peek().type == TokenType::ELSE)
		{
			consume(TokenType::ELSE);
			else_body = parse_statement(enclosing_function);
		}
		
		return new IfStatement{ condition, if_body, else_body, condition->_token };
	}

	Statement* CompilerParser::parse_print_statement(std::string enclosing_function)
	{
		consume(TokenType::PRINT);
		consume(TokenType::LPAREN);

		auto expression = parse_expression();

		consume(TokenType::RPAREN);
		consume(TokenType::SEMICOLON);

		return new PrintStatement{ expression, expression->_token };
	}

	Statement* CompilerParser::parse_hook_statement(std::string enclosing_function)
	{
		const auto dothook_token = consume(TokenType::DOTHOOK);

		return new HookStatement{ true, dothook_token };
	}

	Statement* CompilerParser::parse_unhook_statement(std::string enclosing_function)
	{
		const auto dotunhook_token = consume(TokenType::DOTUNHOOK);

		return new HookStatement{ false, dotunhook_token };
	}

	Statement* CompilerParser::parse_expression_statement(std::string enclosing_function)
	{
		auto expression = parse_expression();
		consume(TokenType::SEMICOLON);

		if (expression != nullptr)
		{
			return new ExpressionStatement{ expression, expression->_token };
		}

		return nullptr;
	}

	// is_definition controls whether we are a function definition or call
	std::vector<Expression*> CompilerParser::parse_arguments(bool is_definition)
	{
		std::vector<Expression*> arguments;

		while (peek().type != TokenType::RPAREN)
		{

			/*
					I have some more work to do here before arguments are ready to be used. For now, arguments
					dont actually have an allocation and this means two things
						-if we try to use one as an identifier, we only are checking for variables symbols which do have an allocation and must be treated differently
						-if we try to use one as an argument, we cant perform basic allocation tasks like copy() because they don't have an allocation reserved for them.

					There are two main solutions to this going forward.

					1.) Unpack all arguments off the stack at the beginning of every function. This would work to make an allocation for each argument
					but would severely limit our ability to do anything with the registers in the body of our function (we would run out doing a simple addition with even 2 arguments)

					2.) Don't unpack the arguments, but instead allocate arguments on the heap. This would mean the stack is reserved exclusively for function values and return addresses.
					Howover, for this to work, we would first need to re-tool the existing heap allocator code since right now I dont think we have a good way to track arguments going out of scope
					at function termination and freeing the heap right now :(

				*/

			if (is_definition)
			{
				auto type_specifier = parse_type_specifier();
				auto identifier = parse_identifier_expression();

				add_symbol(SymbolType::ARGUMENT, identifier->name, lookbehind());
				AS_ARGUMENT_SYMBOL(reference_symbol(SymbolType::ARGUMENT, identifier->name, peek(), false))->type = type_specifier;

				arguments.emplace_back(new ArgumentExpression{ type_specifier, identifier, identifier->_token });
			}

			else
			{
				arguments.emplace_back(parse_expression());
			}


			if (peek().type != TokenType::RPAREN)
			{
				consume(TokenType::COMMA);

				if (peek().type == TokenType::RPAREN)
				{
					_error_reporter->post_error("expected another argument but got `)`", peek());
					break;
				}
			}
		}

		return arguments;
	}

	TypeSpecifier CompilerParser::parse_type_specifier()
	{
		const auto current_token = peek();

		if (!_type_specifier_map.contains(current_token.type))
		{
			_error_reporter->post_error(std::format("unrecognized type specifier `{}`", current_token.value), current_token);
			consume(current_token.type);
			return TypeSpecifier::NVR;
		}

		auto type_specifier = _type_specifier_map.at(peek().type);
		consume(peek().type);
		return type_specifier;
	}

	Node* CompilerParser::parse_function()
	{
		consume(TokenType::FUNCTION);

		auto return_type = parse_type_specifier();

		auto name_token = consume(TokenType::IDENTIFIER);

		//TODO: implement a more efficient way of modifying the return type than this mess
		add_symbol(SymbolType::FUNCTION, name_token.value, lookbehind());
		AS_FUNCTION_SYMBOL(reference_symbol(SymbolType::FUNCTION, name_token.value, peek()))->return_type = return_type;

		consume(TokenType::EQUALS);

		consume(TokenType::LPAREN);
		auto arguments = parse_arguments(true);
		consume(TokenType::RPAREN);


		// inform the parser of the function argument count (arity)
		AS_FUNCTION_SYMBOL(reference_symbol(SymbolType::FUNCTION, name_token.value, peek()))->arity 
			= static_cast<std::uint8_t>(arguments.size());


		auto body = parse_compound_statement(name_token.value);

		return new Function{ name_token.value, return_type, std::move(arguments), body, name_token };
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
#pragma message("TODO: unordered map for name -> functions")
		if (auto it = std::find_if(program.begin(), program.end(), [&](auto function)
			{
				return (AS_FUNCTION_NODE(function)->name == "main");
			}); it != std::end(program))
		{
			//at bare minimum, we must compile main() since it's the entrypoint
			reference_symbol(SymbolType::FUNCTION, "main", peek(), true);

			if (_options->_optimization & OptimizationType::AST)
			{
				for (auto& node : program)
				{
					while (auto node_optimized = node->optimize())
					{
						node = node_optimized;
					}
				}
			}

			return program;
		}

		_error_reporter->post_error("no main() function was defined", NULL_TOKEN);
		return {};
	}
}