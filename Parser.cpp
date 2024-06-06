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

	std::vector<Token> Parser::fetchUntil(TokenType type) const
	{
		std::vector<Token> tokens;

		//TODO: implement this function 

		return tokens;
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
		auto arguments = AS_COMPILER_PARSER(this)->parse_arguments(false);
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


	Expression* Parser::parse_expression_optimized()
	{
		auto expression = parse_expression();

		while (auto expression_optimized = expression->optimize())
		{
			expression = AS_EXPRESSION(expression_optimized);
		}

		return expression;
	}

	Expression* Parser::parse_expression()
	{
		return parse_infix_expression(parse_generic_expression(), Precedence::MINIMUM);
	}

	Expression* Parser::parse_infix_expression(Expression* left, Precedence min_precedence)
	{
		static const std::unordered_map<TokenType, Precedence> precedences =
		{
			{ TokenType::PLUS,  Precedence::TERM },
			{ TokenType::MINUS, Precedence::TERM },
			{ TokenType::STAR,  Precedence::FACTOR },
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

	Statement* CompilerParser::parse_asm_statement(std::string enclosing_function)
	{
		DISCARD consume(TokenType::ASM);
		DISCARD consume(TokenType::LBRACE);

		auto assembly = fetchUntil(TokenType::LBRACE);
		auto assembler = new AssemblerParser{ std::move(assembly) };
		auto generated = assembler->parse();
		
		for (auto instruction : generated)
		{
			instruction->generate();
		}

		//TODO: finish our statement here!
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
			case ASM: return parse_asm_statement(enclosing_function);

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



	Register AssemblerParser::parse_register()
	{
		static const std::unordered_map<TokenType, Register> registers
		{
			{ TokenType::R0, R0 },
			{ TokenType::R1, R1 },
			{ TokenType::R2, R2 },
			{ TokenType::R3, R3 },
		};

		if (auto it_operand = registers.find(peek().type); it_operand != std::end(registers))
		{
			DISCARD consume(peek().type);
			return it_operand->second;
		}

		Log::error(std::format("expected a register but got {}", peek().value));
	}


	
#define ASSERT_IS_INTEGER_LITERAL(x) if (x->etype() != Expression::Type::INTEGER_LITERAL) Log::error("term must evaluate to a constant expression")
#define ASSERT_IN_RANGE(x, a, b) if (x < a || x > b - 1) Log::error(std::format("value {} is outside the its range [0, {}]", x, b - 1))


	//TODO: roll this function and parse_immediate() below it into a "nice" macro to reduce code duplication
	Expression* AssemblerParser::parse_address()
	{
		DISCARD consume(TokenType::AMPERSAND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto label = peek().value;
			DISCARD consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ label.value() };
		}

		auto address_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(address_expression);
		
		auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;
		ASSERT_IN_RANGE(address, 0, DWORD_MAX);

		return new IntegerLiteralExpression{ address };
	}

	Expression* AssemblerParser::parse_immediate()
	{
		DISCARD consume(TokenType::POUND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto label = peek().value;
			DISCARD consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ label.value() };
		}

		auto immediate_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(immediate_expression);

		auto immediate = AS_INTEGER_LITERAL_EXPRESSION(immediate_expression)->value;
		ASSERT_IN_RANGE(immediate, 0, WORD_MAX);

		return new IntegerLiteralExpression{ immediate };
	}

#undef ASSERT_IN_RANGE
#undef ASSERT_IS_INTEGER_LITERAL



	Node* AssemblerParser::parse_instruction()
	{
		//TODO: this is probably useless and should later be removed but I am leaving for now in case I come up with a need for it :)
		static const std::unordered_map<TokenType, Opcode> opcodes
		{
			{ TokenType::MOVE, MOVE },
			{ TokenType::LOAD, LOAD },
			{ TokenType::COPY, COPY },
			{ TokenType::SAVE, SAVE },
			{ TokenType::IADD, IADD },
			{ TokenType::ISUB, ISUB },
			{ TokenType::BAND, BAND },
			{ TokenType::BIOR, BIOR },
			{ TokenType::BXOR, BXOR },
			{ TokenType::CALL, CALL },
			{ TokenType::EXIT, EXIT },
			{ TokenType::PUSH, PUSH },
			{ TokenType::PULL, PULL },
			{ TokenType::BREZ, BREZ },
		};

		
//TODO: patch branch_target resolution to more generally just resolve labels!
		switch (peek().type)
		{
			case TokenType::MOVE:
			{
				DISCARD consume(TokenType::MOVE);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ MOVE, operand1, operand2 };
			} break;

			case TokenType::LOAD:
			{
				DISCARD consume(TokenType::LOAD);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				DISCARD consume(TokenType::AMPERSAND);
				auto operand2 = parse_address();

				if (operand2->etype() == Expression::Type::IDENTIFIER)
				{
					return new Instruction{ LOAD, operand1, DC, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				return new Instruction{ LOAD, operand1, DC, 0, AS_INTEGER_LITERAL_EXPRESSION(operand2)->value };
			} break;

			case TokenType::COPY:
			{
				DISCARD consume(TokenType::COPY);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				DISCARD consume(TokenType::POUND);
				auto operand2 = parse_immediate();

				if (operand2->etype() == Expression::Type::IDENTIFIER)
				{
					return new Instruction{ COPY, operand1, DC, 0xCC, 0, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				return new Instruction{ COPY, operand1, DC, AS_INTEGER_LITERAL_EXPRESSION(operand2)->value };
			} break;

			case TokenType::SAVE:
			{
				DISCARD consume(TokenType::SAVE);
				DISCARD consume(TokenType::AMPERSAND);
				auto operand1 = parse_address();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new Instruction{ SAVE, DC, operand2, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new Instruction{ SAVE, DC, operand2, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			case TokenType::IADD:
			{
				DISCARD consume(TokenType::IADD);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ IADD, operand1, operand2, 0, 0 };
			} break;

			case TokenType::ISUB:
			{
				DISCARD consume(TokenType::ISUB);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ ISUB, operand1, operand2, 0, 0 };
			} break;

			case TokenType::BAND:
			{
				DISCARD consume(TokenType::BAND);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ BAND, operand1, operand2, 0, 0 };
			} break;

			case TokenType::BIOR:
			{
				DISCARD consume(TokenType::BIOR);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ BIOR, operand1, operand2, 0, 0 };
			} break;

			case TokenType::BXOR:
			{
				DISCARD consume(TokenType::BXOR);
				auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				return new Instruction{ BXOR, operand1, operand2, 0, 0 };
			} break;

			case TokenType::CALL:
			{
				DISCARD consume(TokenType::CALL);
				auto operand1 = parse_address();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new Instruction{ CALL, DC, DC, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new Instruction{ CALL, DC, DC, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			case TokenType::EXIT:
			{
				DISCARD consume(TokenType::EXIT);
			} break;


			case TokenType::PUSH:
			{
				DISCARD consume(TokenType::PUSH);
				auto operand1 = parse_register();

				return new Instruction{ PUSH, DC, operand1, 0, 0 };
			} break;

			case TokenType::PULL:
			{
				DISCARD consume(TokenType::PULL);
				auto operand1 = parse_register();

				return new Instruction{ PULL, operand1, DC, 0, 0 };
			} break;

			case TokenType::BREZ:
			{
				DISCARD consume(TokenType::BREZ);
				auto operand1 = parse_address();
				DISCARD consume(TokenType::COMMA);
				auto operand2 = parse_register();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new Instruction{ BREZ, DC, operand2, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new Instruction{ BREZ, DC, operand2, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			default: 
			{
				Log::error(std::format("unrecognized instruction mnemonic {}", peek().value));
			} break;
		}
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