import std;

#include <ast/expression/Expression.h>
#include <ast/function/Function.h>
#include <ast/statement/Statement.h>
#include <cli/defs/OptimizationFlag.h>
#include <error/CommonErrors.h>
#include <symbol/models/Symbol.h>
#include <toolchain/CompilerParser.h>
#include <toolchain/AssemblerParser.h>
#include <utility/Random.h>

// Haze CompilerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ParserType CompilerParser::ptype() const
	{
		return ParserType::COMPILER;
	}

	ExpressionReference<FunctionCallExpression> Parser::parse_functioncall_expression()
	{
		const auto name_token = consume(TokenKind::IDENTIFIER);

		consume(TokenKind::LPAREN);
		const auto arguments = parse_arguments(false);
		consume(TokenKind::RPAREN);

		if (!USE_SAFE(SymbolDatabase)->has_symbol(name_token.text))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"function `{}` is undefined", name_token.text), name_token);
			return nullptr;
		}

		const auto function_symbol = USE_SAFE(SymbolDatabase)->reference_function(name_token.text, name_token);

		if (function_symbol->arity() != arguments.size())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"function `{}` was defined with {} arguments but called with {}",
				name_token.text, function_symbol->arity(), arguments.size()), name_token);
			return nullptr;
		}

		// NOTE: exports the function name symbol only
		USE_SAFE(SymbolExporter)->enqueue(function_symbol, name_token);

		return new FunctionCallExpression{ name_token.text, arguments, name_token };
	}

	Statement* CompilerParser::parse_statement(const std::string& enclosing_function)
	{
		using enum TokenKind;
		switch (peek().kind)
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

			// if nothing else worked, try to fall back on parsing any expression followed by a semicolon
			default: return parse_expression_statement(enclosing_function);
		}
	}

	std::vector<Statement*> CompilerParser::parse_statements(const std::string& enclosing_function)
	{
		std::vector<Statement*> statements;

		while (peek().kind != TokenKind::RBRACE)
		{
			statements.emplace_back(parse_statement(enclosing_function));
		}

		return statements;
	}

	Statement* CompilerParser::parse_null_statement(const std::string& enclosing_function)
	{
		const auto semicolon_token = consume(TokenKind::SEMICOLON);
		return MAKE_NULL_STATEMENT(semicolon_token);
	}

	Statement* CompilerParser::parse_variabledeclaration_statement(const std::string& enclosing_function)
	{
		consume(TokenKind::DECLARE);

		const auto kind = parse_type();
		const auto identifier_token = consume(TokenKind::IDENTIFIER);

		USE_SAFE(SymbolDatabase)->add_variable(identifier_token.text, lookbehind());

		auto function_symbol = USE_SAFE(SymbolDatabase)->reference_function(enclosing_function, peek());
		function_symbol->locals_count++;

		// exports the variable identifier symbol only
		USE_SAFE(SymbolExporter)->enqueue(function_symbol, identifier_token);

		if (peek().kind == TokenKind::EQUALS)
		{
			consume(TokenKind::EQUALS);

			const auto value = parse_expression();
	
			consume(TokenKind::SEMICOLON);

			return new VariableStatement{ kind, identifier_token.text, value, nullptr, value->_token };
		}

		consume(TokenKind::SEMICOLON);

		return new VariableStatement{ kind, identifier_token.text, nullptr, nullptr, identifier_token };
	}

	Statement* CompilerParser::parse_compound_statement(const std::string& enclosing_function)
	{
		const auto lbrace_token = consume(TokenKind::LBRACE);

		auto statements = parse_statements(enclosing_function);

		consume(TokenKind::RBRACE);

		return new CompoundStatement{ statements, lbrace_token };
	}

	StatementHandle CompilerParser::parse_return_statement(std::string_view enclosing_function)
	{
		const auto return_token = consume(TokenKind::RETURN);

		const auto expression = parse_expression();

		consume(TokenKind::SEMICOLON);

		auto symbol = USE_SAFE(SymbolDatabase)->reference_symbol(SymbolKind::FUNCTION, enclosing_function, return_token);

		// NOTE: special case for `main()`:
		// return statements are really an exit from the entire program
		// this is because we don't internally bootstrap-wrap `main()` with `_main()`
		// like many real C implementations do (e.g., VC++)
		if (enclosing_function == "main")
		{
			if (!expression)
			{
				USE_SAFE(ErrorReporter)->post_error("non-nvr function `main()` must specify a return value", return_token);
			}

			return new ExitStatement{ expression, expression->_token };
		}

		return new ReturnStatement{ enclosing_function, expression, nullptr, return_token };
	}

	Parser* CompilerParser::createassembler_parser(const std::filesystem::path& filepath)
	{
		const auto architecture = USE_UNSAFE(CommandLineOptions)->architecture;
		switch (architecture)
		{
			case ArchitectureKind::RISCV:
				return new RISCVAssemblerParser{ filepath };
		}

		USE_UNSAFE(ErrorReporter)->post_error(
			"unsupported architecture", NULL_TOKEN);
		return nullptr;
	}

	Statement* CompilerParser::parse_inline_asm_statement(const std::string& enclosing_function)
	{
		const auto asm_token = consume(TokenKind::ASM);
		consume(TokenKind::LBRACE);

		const auto assembly = fetch_until(TokenKind::RBRACE);
		assembly.emplace_back(Token{ TokenKind::END, "eof", peek().location });

		const auto assembler_parser = createassembler_parser(filepath);
		assembler_parser->reload(assembly, filepath);

		const auto commands = assembler_parser->parse()
			| TypeHandle get_type(const TypeStorage&) const;::transform([](auto node)
				{
					if (node->ntype() != NodeType::COMMAND)
					{
						CommonErrors::invalid_node_type(node->ntype(), asm_token);
						return nullptr;
					}
					return static_cast<Command*>(node);
				})
			| std::ranges::to<std::vector<Command*>>();

		consume(TokenKind::RBRACE);

		return new InlineAsmStatement{ commands, assembler_parser, filepath, asm_token };
	}

	Statement* CompilerParser::parse_while_statement(const std::string& enclosing_function)
	{
		consume(TokenKind::WHILE);
		consume(TokenKind::LPAREN);

		const auto condition = parse_expression();

		consume(TokenKind::RPAREN);

		const auto body = parse_statement(enclosing_function);

		return new WhileStatement{ condition, body, condition->_token };
	}

	Statement* CompilerParser::parse_for_statement(const std::string& enclosing_function)
	{
		consume(TokenKind::FOR);
		consume(TokenKind::LPAREN);

		const auto initialization = parse_variabledeclaration_statement(enclosing_function);
		const auto condition = parse_expression();

		consume(TokenKind::SEMICOLON);

		const auto expression = parse_expression();

		consume(TokenKind::RPAREN);

		const auto body = parse_statement(enclosing_function);

		return new ForStatement{ initialization, condition, expression, body, initialization->_token };
	}

	Statement* CompilerParser::parse_if_statement(const std::string& enclosing_function)
	{
		consume(TokenKind::IF);
		consume(TokenKind::LPAREN);

		const auto condition = parse_expression();

		consume(TokenKind::RPAREN);

		const auto if_body = parse_statement(enclosing_function);
		Statement* else_body = nullptr;

		if (peek().kind == TokenKind::ELSE)
		{
			consume(TokenKind::ELSE);
			else_body = parse_statement(enclosing_function);
		}
		
		return new IfStatement{ condition, if_body, else_body, condition->_token };
	}

	Statement* CompilerParser::parse_print_statement(const std::string& enclosing_function)
	{
		consume(TokenKind::PRINT);
		consume(TokenKind::LPAREN);

		const auto expression = parse_expression();

		consume(TokenKind::RPAREN);
		consume(TokenKind::SEMICOLON);

		return new PrintStatement{ expression, expression->_token };
	}

	Statement* CompilerParser::parse_expression_statement(const std::string& enclosing_function)
	{
		const auto expression = parse_expression();
		consume(TokenKind::SEMICOLON);

		if (expression != nullptr)
		{
			return new ExpressionStatement{ expression, expression->_token };
		}

		return nullptr;
	}

	Statement* CompilerParser::parse_exit_statement(const std::string& enclosing_function)
	{
		const auto exit_token = consume(TokenKind::EXIT);
		
		consume(TokenKind::LPAREN);
		const auto expression = parse_expression();
		consume(TokenKind::RPAREN);
		
		consume(TokenKind::SEMICOLON);

		return new ExitStatement{ expression, exit_token };
	}

	MemberDeclarationExpression* CompilerParser::parse_member_declaration_statement(const std::string& enclosing_function)
	{
		const auto kind = parse_type();
		const auto name = parse_identifier_expression();

		consume(TokenKind::COMMA);

		return new MemberDeclarationExpression{ kind, name, name->_token };
	}

	std::vector<MemberDeclarationExpression*> CompilerParser::parse_member_declaration_statements(const std::string& enclosing_function)
	{
		std::vector<MemberDeclarationExpression*> out{};

		while (peek().kind != TokenKind::RBRACE)
		{
			const auto member_declaration = parse_member_declaration_statement(enclosing_function);
			out.emplace_back(member_declaration);
		}

		return out;
	}

	Statement* CompilerParser::parse_struct_declaration_statement(const std::string& enclosing_function)
	{
		const auto declaration = consume(TokenKind::STRUCT);

		const auto identifier = parse_identifier_expression();

		consume(TokenKind::LBRACE);
		const auto member_declarations = parse_member_declaration_statements(enclosing_function);
		consume(TokenKind::RBRACE);

		const auto symbol = new StructOrUnionSymbol{ identifier->name, declaration };
		
		USE_SAFE(SymbolDatabase)->add_struct(identifier->name, identifier->_token);

		return new StructDeclarationStatement{ identifier, member_declarations, identifier->_token };
	}

	// is_definition controls whether we are a function definition or call
	std::vector<ExpressionHandle> CompilerParser::parse_arguments(bool is_definition)
	{
		std::vector<ExpressionHandle> arguments;

		while (peek().kind != TokenKind::RPAREN)
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
				const auto kind = parse_type();
				const auto identifier = parse_identifier_expression();

				USE_SAFE(SymbolDatabase)->add_argument(identifier->name, lookbehind(), kind);

				// exports the argument identifier symbol only
				USE_SAFE(SymbolExporter)->enqueue(symbol, identifier->_token);

				arguments.emplace_back(new ArgumentExpression{ kind, identifier, identifier->_token });
			}

			else
			{
				arguments.emplace_back(parse_expression());
			}


			if (peek().kind != TokenKind::RPAREN)
			{
				consume(TokenKind::COMMA);

				if (peek().kind == TokenKind::RPAREN)
				{
					USE_SAFE(ErrorReporter)->post_error("expected another argument but got `)`", peek());
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
		if (_type_qualifier_token_map.contains(current_token.kind))
		{
			qualifier = _type_qualifier_token_map.at(current_token.kind);
			consume(current_token.kind);
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
		if (_type_signedness_token_map.contains(current_token.kind))
		{
			signedness = _type_signedness_token_map.at(current_token.kind);
			consume(current_token.kind);
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
		if (_type_specifier_token_map.contains(current_token.kind))
		{
			specifier = _type_specifier_token_map.at(current_token.kind);
			consume(current_token.kind);
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
		if (_type_storage_token_map.contains(current_token.kind))
		{
			storage = _type_storage_token_map.at(current_token.kind);
			consume(current_token.kind);
		}

		else if (is_mandatory)
		{
			CommonErrors::invalid_type("storage", current_token);
		}

		return storage;
	}

	TypeHandle CompilerParser::parse_type()
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
			case DWORD:
			{
				auto int_kind = _type_specifier_int_map.at(specifier);
				return new IntType{ qualifier, signedness, int_kind, storage };
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
		consume(TokenKind::FUNCTION);

		const auto return_type = parse_type();

		const auto name_token = consume(TokenKind::IDENTIFIER);

		// 5 digits of randomness for now
		const auto uuid = hz::generate_digit_string(5);
		const auto end_function_label = std::format("end_function_{:05d}", uuid);
		function_label_map[name_token.text] = end_function_label;

		consume(TokenKind::EQUALS);

		consume(TokenKind::LPAREN);
		auto arguments = parse_arguments(true);
		consume(TokenKind::RPAREN);

		// update the symbol map
		USE_SAFE(SymbolDatabase)->add_function(name_token.text, lookbehind(), return_type, arguments);

		const auto body = parse_compound_statement(name_token.text);
		return new Function{ name_token.text, return_type, arguments, body, name_token };
	}

	std::vector<Node*> CompilerParser::parse_functions()
	{
		std::vector<Node*> functions;

		while (peek().kind != TokenKind::END)
		{
			functions.emplace_back(parse_function());
		}

		return functions;
	}

	std::vector<Node*> CompilerParser::parse()
	{
		auto program = parse_functions();

		if (function_label_map.contains("main"))
		{
			//at bare minimum, we must compile main() since it's the entrypoint
			USE_SAFE(SymbolDatabase)->reference_symbol(SymbolKind::FUNCTION, "main", peek(), true);

			if (USE_SAFE(CommandLineOptions)->optimization & OptimizationType::AST)
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

		USE_SAFE(ErrorReporter)->post_error("no main() function was defined", NULL_TOKEN);
		return {};
	}

	std::vector<NodeType> parse_implementation() const
	{
#pragma message("TODO: implement parsing of nonglobal declarations")
	}


}
