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

#include "Utility.h"
#include "Log.h"

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

	Statement* CompilerParser::parse_inline_asm_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::ASM);
		DISCARD consume(TokenType::LBRACE);

		auto assembly = fetchUntil(TokenType::RBRACE);
		auto assembler_parser = new AssemblerParser{ std::move(assembly) };
		auto commands = assembler_parser->parse();

		DISCARD consume(TokenType::RBRACE);

		//TODO: massage AsmStatement to use nodes but assert that they are indeed commands!
		return new InlineAsmStatement{ std::move(commands), assembler_parser };
	}

	Statement* CompilerParser::parse_while_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::WHILE);
		DISCARD consume(TokenType::LPAREN);
		auto condition = parse_expression();
		DISCARD consume(TokenType::RPAREN);

		auto body = parse_statement(enclosing_function);

		return new WhileStatement{ condition, body };
	}

	Statement* CompilerParser::parse_for_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::FOR);
		DISCARD consume(TokenType::LPAREN);
		auto initialization = parse_variabledeclaration_statement(enclosing_function);
		auto condition = parse_expression();
		DISCARD consume(TokenType::SEMICOLON);
		auto expression = parse_expression();
		DISCARD consume(TokenType::RPAREN);

		auto body = parse_statement(enclosing_function);

		return new ForStatement{ initialization, condition, expression, body };
	}

	Statement* CompilerParser::parse_if_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::IF);
		DISCARD consume(TokenType::LPAREN);
		auto condition = parse_expression();
		DISCARD consume(TokenType::RPAREN);

		auto if_body = parse_statement(enclosing_function);
		Statement* else_body = nullptr;

		if (peek().type == TokenType::ELSE)
		{
			DISCARD consume(TokenType::ELSE);
			else_body = parse_statement(enclosing_function);
		}
		
		return new IfStatement{ condition, if_body, else_body };
	}

	Statement* CompilerParser::parse_expression_statement(std::string enclosing_function)
	{
		auto expression = parse_expression();
		DISCARD consume(TokenType::SEMICOLON);

		return new ExpressionStatement{ expression };
	}

	Expression* CompilerParser::parse_argument()
	{
		DISCARD consume(TokenType::BYTE);
		const auto name = consume(TokenType::IDENTIFIER);
		add_symbol(Symbol::Type::ARGUMENT, name);

		return new IdentifierExpression{ name };
	}

	std::vector<Expression*> CompilerParser::parse_arguments(bool is_declaration)
	{
		std::vector<Expression*> arguments;

		while (peek().type != TokenType::RPAREN)
		{
			if (is_declaration)
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

				//TODO: require a type specifier for each argument of a function definition
				auto identifier = parse_identifier_expression();
				add_symbol(Symbol::Type::ARGUMENT, identifier->name);
				//TODO: this symbol should also be of type ARGUMENT
				arguments.emplace_back(identifier);
			}

			else
			{
				arguments.emplace_back(parse_expression());
			}


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
		auto arguments = parse_arguments(true);
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
					while (auto node_optimized = node->optimize())
					{
						node = node_optimized;
					}
				}
			}

			return program;
		}

		Log::error("no main() function was defined");
	}
}