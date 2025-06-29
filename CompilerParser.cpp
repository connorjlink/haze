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
#include "MemberDeclarationExpression.h"
#include "StructDeclarationStatement.h"
#include "FileManager.h"
#include "CommandLineOptions.h"
#include "Symbol.h"
#include "Random.h"
#include "Type.h"
#include "HeapAllocator.h"
#include "ErrorReporter.h"
#include "CommonErrors.h"
#include "SymbolDatabase.h"
#include "SymbolExporter.h"

// Haze CompilerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ParserType CompilerParser::ptype() const
	{
		return ParserType::COMPILER;
	}

	Statement* CompilerParser::parse_statement(const std::string& enclosing_function)
	{
		using enum TokenType;
		switch (peek().type)
		{
			case LBRACE: return parse_compound_statement(enclosing_function);
			case DECLARE: return parse_variabledeclaration_statement(enclosing_function);
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

	std::vector<Statement*> CompilerParser::parse_statements(const std::string& enclosing_function)
	{
		std::vector<Statement*> statements;

		while (peek().type != TokenType::RBRACE)
		{
			statements.emplace_back(parse_statement(enclosing_function));
		}

		return statements;
	}

	Statement* CompilerParser::parse_null_statement(const std::string& enclosing_function)
	{
		const auto semicolon_token = consume(TokenType::SEMICOLON);
		return new NullStatement{ semicolon_token };
	}

	Statement* CompilerParser::parse_variabledeclaration_statement(const std::string& enclosing_function)
	{
		consume(TokenType::DECLARE);

		const auto type = parse_type();
		const auto identifier_token = consume(TokenType::IDENTIFIER);

		_database->add_variable(identifier_token.text, lookbehind());

		auto function_symbol = _database->reference_function(enclosing_function, peek());
		function_symbol->locals_count++;

		// exports the variable identifier symbol only
		_exporter->enqueue(function_symbol, identifier_token);

		if (peek().type == TokenType::EQUALS)
		{
			consume(TokenType::EQUALS);

			const auto value = parse_expression();
	
			consume(TokenType::SEMICOLON);

			return new VariableStatement{ type, identifier_token.text, value, nullptr, value->_token };
		}

		consume(TokenType::SEMICOLON);

		return new VariableStatement{ type, identifier_token.text, nullptr, nullptr, identifier_token };
	}

	Statement* CompilerParser::parse_compound_statement(const std::string& enclosing_function)
	{
		const auto lbrace_token = consume(TokenType::LBRACE);

		auto statements = parse_statements(enclosing_function);

		consume(TokenType::RBRACE);

		return new CompoundStatement{ std::move(statements), lbrace_token };
	}

	Statement* CompilerParser::parse_return_statement(const std::string& enclosing_function)
	{
		const auto return_token = consume(TokenType::RETURN);

		const auto expression = parse_expression();

		consume(TokenType::SEMICOLON);

		auto symbol = _database->reference_symbol(SymbolType::FUNCTION, enclosing_function, return_token);


		// NOTE: special case for `main()`:
		// return statements are really an exit from the entire program
		// this is because we don't internally bootstrap-wrap `main()` with `_main()`
		// like many real C implementations do (e.g., VC++)
		if (enclosing_function == "main")
		{
			if (expression == nullptr)
			{
				USE_SAFE(ErrorReporter).post_error("non-nvr function `main()` must specify a return value", return_token);
			}

			return new ExitStatement{ expression, expression->_token };
		}

		return new ReturnStatement{ enclosing_function, expression, nullptr, return_token };
	}

	Statement* CompilerParser::parse_inline_asm_statement(const std::string& enclosing_function)
	{
		const auto asm_token = consume(TokenType::ASM);
		consume(TokenType::LBRACE);

		auto assembly = fetch_until(TokenType::RBRACE);
		assembly.emplace_back(Token{ TokenType::END, "eof", peek().location });

		auto assembler_parser = new AssemblerParser{ std::move(assembly), _filepath };
		auto commands = assembler_parser->parse();

		consume(TokenType::RBRACE);

		return new InlineAsmStatement{ std::move(commands), assembler_parser, _filepath, asm_token };
	}

	Statement* CompilerParser::parse_while_statement(const std::string& enclosing_function)
	{
		consume(TokenType::WHILE);
		consume(TokenType::LPAREN);

		auto condition = parse_expression();

		consume(TokenType::RPAREN);

		auto body = parse_statement(enclosing_function);

		return new WhileStatement{ condition, body, condition->_token };
	}

	Statement* CompilerParser::parse_for_statement(const std::string& enclosing_function)
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

	Statement* CompilerParser::parse_if_statement(const std::string& enclosing_function)
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

	Statement* CompilerParser::parse_print_statement(const std::string& enclosing_function)
	{
		consume(TokenType::PRINT);
		consume(TokenType::LPAREN);

		auto expression = parse_expression();

		consume(TokenType::RPAREN);
		consume(TokenType::SEMICOLON);

		return new PrintStatement{ expression, expression->_token };
	}

	Statement* CompilerParser::parse_hook_statement(const std::string& enclosing_function)
	{
		const auto dothook_token = consume(TokenType::DOTHOOK);

		return new HookStatement{ true, dothook_token };
	}

	Statement* CompilerParser::parse_unhook_statement(const std::string& enclosing_function)
	{
		const auto dotunhook_token = consume(TokenType::DOTUNHOOK);

		return new HookStatement{ false, dotunhook_token };
	}

	Statement* CompilerParser::parse_expression_statement(const std::string& enclosing_function)
	{
		auto expression = parse_expression();
		consume(TokenType::SEMICOLON);

		if (expression != nullptr)
		{
			return new ExpressionStatement{ expression, expression->_token };
		}

		return nullptr;
	}

	MemberDeclarationExpression* CompilerParser::parse_member_declaration_statement(const std::string& enclosing_function)
	{
		const auto type = parse_type();
		const auto name = parse_identifier_expression();

		consume(TokenType::COMMA);

		return new MemberDeclarationExpression{ type, name, name->_token };
	}

	std::vector<MemberDeclarationExpression*> CompilerParser::parse_member_declaration_statements(const std::string& enclosing_function)
	{
		std::vector<MemberDeclarationExpression*> out{};

		while (peek().type != TokenType::RBRACE)
		{
			const auto member_declaration = parse_member_declaration_statement(enclosing_function);
			out.emplace_back(member_declaration);
		}

		return out;
	}

	Statement* CompilerParser::parse_struct_declaration_statement(const std::string& enclosing_function)
	{
		consume(TokenType::STRUCT);

		const auto identifier = parse_identifier_expression();

		consume(TokenType::LBRACE);
		const auto member_declarations = parse_member_declaration_statements(enclosing_function);
		consume(TokenType::RBRACE);

		const auto symbol = new StructSymbol{ identifier->name };
		
		_database->add_struct(identifier->name, identifier->_token);
		

		return new StructDeclarationStatement{ identifier, member_declarations, identifier->_token };
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
				const auto type = parse_type();
				const auto identifier = parse_identifier_expression();

				_database->add_argument(identifier->name, lookbehind());
				auto symbol = _database->reference_argument(identifier->name, peek());
				symbol->type = type;

				// exports the argument identifier symbol only
				_exporter->enqueue(symbol, identifier->_token);

				arguments.emplace_back(new ArgumentExpression{ type, identifier, identifier->_token });
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
					USE_SAFE(ErrorReporter).post_error("expected another argument but got `)`", peek());
					break;
				}
			}
		}

		return arguments;
	}

	TypeQualifier CompilerParser::parse_type_qualifier(bool is_mandatory)
	{
		const auto& current_token = peek();

		auto qualifier = TypeQualifier::IMMUTABLE;
		if (_type_qualifier_token_map.contains(current_token.type))
		{
			qualifier = _type_qualifier_token_map.at(current_token.type);
			consume(current_token.type);
		}

		else if (is_mandatory)
		{
			CommonErrors::invalid_type("qualifier", current_token);
		}

		return qualifier;
	}

	TypeSignedness CompilerParser::parse_type_signedness(bool is_mandatory)
	{
		const auto& current_token = peek();

		auto signedness = TypeSignedness::UNSIGNED;
		if (_type_signedness_token_map.contains(current_token.type))
		{
			signedness = _type_signedness_token_map.at(current_token.type);
			consume(current_token.type);
		}

		else if (is_mandatory)
		{
			CommonErrors::invalid_type("signedness", current_token);
		}

		return signedness;
	}

	TypeSpecifier CompilerParser::parse_type_specifier(bool is_mandatory)
	{
		const auto& current_token = peek();

		auto specifier = TypeSpecifier::NVR;
		if (_type_specifier_token_map.contains(current_token.type))
		{
			specifier = _type_specifier_token_map.at(current_token.type);
			consume(current_token.type);
		}

		else if (is_mandatory)
		{
			CommonErrors::invalid_type("specifier", current_token);
		}

		return specifier;
	}

	TypeStorage CompilerParser::parse_type_storage(bool is_mandatory)
	{
		const auto& current_token = peek();

		auto storage = TypeStorage::VALUE;
		if (_type_storage_token_map.contains(current_token.type))
		{
			storage = _type_storage_token_map.at(current_token.type);
			consume(current_token.type);
		}

		else if (is_mandatory)
		{
			CommonErrors::invalid_type("storage", current_token);
		}

		return storage;
	}

	Type* CompilerParser::parse_type()
	{
		// default is `const` but can be overriden
		const auto qualifier = parse_type_qualifier();

		// default is `unsigned` if integer by can be overriden
		const auto signedness = parse_type_signedness();

		// type specifier is mandatory
		const auto specifier = parse_type_specifier(true);

		// default is `value` but can be overriden
		const auto storage = parse_type_storage();

	
		using enum TypeSpecifier;
		switch (specifier)
		{
			case BYTE: [[fallthrough]];
			case WORD: [[fallthrough]];
			case DWORD: [[fallthrough]];
			case QWORD:
			{
				auto int_type = _type_specifier_int_map.at(specifier);
				return new IntType{ qualifier, signedness, int_type, storage };
			} break;

			case STRUCT:
			{
				const auto tag = parse_identifier_expression();
				return new StructType{ qualifier, tag->name, storage };
			} break;

			case STRING:
			{
				return new StringType{ qualifier, storage };
			} break;

			default:
			{
				return nullptr;
			} break;
		}
	}

	Node* CompilerParser::parse_function()
	{
		consume(TokenType::FUNCTION);

		auto return_type = parse_type();

		const auto name_token = consume(TokenType::IDENTIFIER);

		// 5 digits of randomness for now
		const auto uuid = hz::generate(5);
		const auto end_function_label = std::format("end_function_{:05d}", uuid);
		_function_label_map[name_token.text] = end_function_label;

		#pragma message("TODO: implement a more efficient way of modifying the return type than this mess")

		_database->add_function(name_token.text, lookbehind(), return_type);

		consume(TokenType::EQUALS);

		consume(TokenType::LPAREN);
		auto arguments = parse_arguments(true);
		consume(TokenType::RPAREN);

		// inform the parser of the function arguments
		// creates a local copy of them for future reference
		auto function_symbol = _database->reference_function(name_token.text, peek());
		function_symbol->arguments = arguments;

		const auto body = parse_compound_statement(name_token.text);
		return new Function{ name_token.text, return_type, std::move(arguments), body, name_token };
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

		if (_function_label_map.contains("main"))
		{
			//at bare minimum, we must compile main() since it's the entrypoint
			_database->reference_symbol(SymbolType::FUNCTION, "main", peek(), true);

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

		USE_SAFE(ErrorReporter).post_error("no main() function was defined", NULL_TOKEN);
		return {};
	}
}
