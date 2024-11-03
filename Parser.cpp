import std;

#include "Parser.h"
#include "Token.h"
#include "Allocator.h"
#include "CompilerParser.h"
#include "VariableStatement.h"
#include "ReturnStatement.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "FunctionCallExpression.h"
#include "AdjustExpression.h"
#include "StringExpression.h"
#include "BinaryExpression.h"
#include "DotDefineCommand.h"
#include "Symbol.h"
#include "ExtendedInteger.h"
#include "CommonErrors.h"
#include "ErrorReporter.h"

// Haze Parser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	bool is_binary_operator(TokenType type)
	{
		return _binary_expression_map.contains(type);
	}
}

namespace hz
{
	// GLOBALS
	Parser* _parser;
	// GLOBALS


	Parser::Parser(const std::vector<Token>& tokens, const std::string& filepath)
		: cursor{ 0 }, tokens{ tokens }, _filepath{ filepath }
	{
		_error_reporter->open_context(filepath, "parsing");
	}

	Parser::~Parser()
	{
		_error_reporter->close_context();
	}


	void Parser::add_symbol(SymbolType type, const std::string& name, const Token& location)
	{
		// does the symbol already exist in the registry?
		if (symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` was already defined as a {}",
				name, _symbol_map.at(symbol_table.at(name)->ytype())), location);
			return;
		}

		using enum SymbolType;
		switch (type)
		{
			case FUNCTION: symbol_table[name] = new FunctionSymbol{ name, nullptr }; break;
			case ARGUMENT: symbol_table[name] = new ArgumentSymbol{ name, nullptr }; break;
			case VARIABLE: symbol_table[name] = new VariableSymbol{ name, nullptr, nullptr }; break;
			case DEFINE: symbol_table[name] = new DefineSymbol{ name, nullptr, ExtendedInteger{} }; break;
			case LABEL: symbol_table[name] = new LabelSymbol{ name, 0 }; break;

			default:
			{
				_error_reporter->post_error(std::format("invalid symbol type `{}`", location.value), location);
			} break;
		}
	}

	void Parser::add_function(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::FUNCTION, name, location);
	}

	void Parser::add_argument(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::ARGUMENT, name, location);
	}

	void Parser::add_variable(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::VARIABLE, name, location);
	}

	void Parser::add_define(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::DEFINE, name, location);
	}

	void Parser::add_label(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::LABEL, name, location);
	}

	void Parser::add_struct(const std::string& name, const Token& location)
	{
		add_symbol(SymbolType::STRUCT, name, location);
	}


	SymbolType Parser::query_symbol_type(const std::string& name, const Token& location)
	{
		if (!symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` is undefined", name), location);
			return SymbolType::VARIABLE;
		}

		return symbol_table.at(name)->ytype();
	}

	Symbol* Parser::reference_symbol(SymbolType type, const std::string& name, const Token& location, bool mark_visited)
	{
		if (!_symbol_map.contains(type))
		{
			_error_reporter->post_error(std::format("invalid symbol type `{}`", location.value), location);
			return nullptr;
		}

		if (!symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` is undefined", name), location);
			return nullptr;
		}

		auto symbol = symbol_table.at(name);

		if (symbol->ytype() != type)
		{
			_error_reporter->post_error(std::format("symbol `{}` was defined as a {} but referenced as a {}",
				name, _symbol_map.at(symbol->ytype()), _symbol_map.at(type)), location);
			return nullptr;
		}

		if (mark_visited)
		{
			symbol->was_referenced = true;
		}

		return symbol;
	}

	FunctionSymbol* Parser::reference_function(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::FUNCTION, name, location, mark_visited);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		return function_symbol;
	}

	ArgumentSymbol* Parser::reference_argument(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::ARGUMENT, name, location, mark_visited);
		auto argument_symbol = AS_ARGUMENT_SYMBOL(symbol);

		return argument_symbol;
	}

	VariableSymbol* Parser::reference_variable(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::VARIABLE, name, location, mark_visited);
		auto variable_symbol = AS_VARIABLE_SYMBOL(symbol);

		return variable_symbol;
	}

	DefineSymbol* Parser::reference_define(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::DEFINE, name, location, mark_visited);
		auto define_symbol = AS_DEFINE_SYMBOL(symbol);

		return define_symbol;
	}

	LabelSymbol* Parser::reference_label(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::LABEL, name, location, mark_visited);
		auto label_symbol = AS_LABEL_SYMBOL(symbol);

		return label_symbol;
	}

	StructSymbol* Parser::reference_struct(const std::string& name, const Token& location, bool mark_visited)
	{
		auto symbol = reference_symbol(SymbolType::STRUCT, name, location, mark_visited);
		auto struct_symbol = AS_STRUCT_SYMBOL(symbol);

		return struct_symbol;
	}

	bool Parser::has_symbol(const std::string& name)
	{
		return symbol_table.contains(name);
	}

	Token& Parser::lookbehind()
	{
		if (cursor > 0)
		{
			return tokens[cursor - 1];
		}

		_error_reporter->post_uncorrectable("invalid token backtrack", peek());
	}

	Token& Parser::peek()
	{
		return tokens[cursor];
	}

	Token& Parser::lookahead()
	{
		if (cursor < tokens.size() - 1)
		{
			return tokens[cursor + 1];
		}

		_error_reporter->post_uncorrectable("unexpectedly reached the end of file", peek());
	}

	Token Parser::consume(TokenType token)
	{
		const auto& current = peek();
		if (current.type == token)
		{
			cursor++;
			return current;
		}

		auto convert = [&](auto v) -> std::string_view
		{
			const auto item = _token_map.at(v);

			if (item)
			{
				return *item;
			}

			_error_reporter->post_error("invalid token", current);
			return { "[error]" };
		};

		_error_reporter->post_error(std::format("expected token `{}` but got `{}`", 
			convert(token), ((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.value : convert(current.type))), current);
		return current;
	}

	std::vector<Token> Parser::fetch_until(TokenType type)
	{
		std::vector<Token> tokens;

		while (peek().type != type)
		{
			tokens.emplace_back(peek());
			consume(peek().type);
		}

		return tokens;
	}

	Node* Parser::parse_dotdefine_command()
	{
		consume(TokenType::DOTDEFINE);

		Type* type = nullptr;

		if (ptype() == ParserType::COMPILER)
		{
			auto compiler_parser = AS_COMPILER_PARSER(this);
			type = compiler_parser->parse_type();
		}

		else
		{
			// default to unsigned 16 bits non-compiler workloads
			type = new IntType{ TypeQualifier::IMMUTABLE, TypeSignedness::UNSIGNED, IntTypeType::INT16, TypeStorage::VALUE };
		}
		
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenType::EQUALS);

		const auto value_expression = parse_expression();

		if (value_expression->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("definitions must result in a constant expression", value_expression->_token);
			return nullptr;
		}

		const auto identifier = identifier_expression->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		add_define(identifier, peek());
		
		auto define_symbol = reference_define(identifier, peek());
		define_symbol->type = type;
		define_symbol->value = integer_literal_raw(value);

		return new DotDefineCommand{ identifier, value, identifier_expression->_token };
	}

	IdentifierExpression* Parser::parse_identifier_expression()
	{
		return parse_identifier();
	}

	IntegerLiteralExpression* Parser::parse_integerliteral_expression()
	{
		// NOTE: old usage
		//const auto integer_value = std::stoul(integer_literal_token.value);
		
		const auto integer_literal_token = consume(TokenType::INT);

		const auto& integer_string = integer_literal_token.value;

		std::int64_t integer_value{};
		auto [ptr, ec] = std::from_chars(integer_string.data(), integer_string.data() + integer_string.size(), integer_value);

		if (ec != std::errc())
		{
			_error_reporter->post_error(std::format("unparseable integer literal `{}`", integer_string), integer_literal_token);
			return nullptr;
		}

		IntegerLiteral* integer_literal = nullptr;

		const auto specifier = peek();
		
		using enum TokenType;
		switch (specifier.type)
		{
			case U8: integer_literal = new UnsignedByteIntegerLiteral{ static_cast<std::uint8_t>(integer_value) }; break;
			case U16: integer_literal = new UnsignedWordIntegerLiteral{ static_cast<std::uint16_t>(integer_value) }; break;
			case U32: integer_literal = new UnsignedDoubleWordIntegerLiteral{ static_cast<std::uint32_t>(integer_value) }; break;
			case U64: integer_literal = new UnsignedQuadWordIntegerLiteral{ static_cast<std::uint64_t>(integer_value) }; break;

			case S8: integer_literal = new SignedByteIntegerLiteral{ static_cast<std::int8_t>(integer_value) }; break;
			case S16: integer_literal = new SignedWordIntegerLiteral{ static_cast<std::int16_t>(integer_value) }; break;
			case S32: integer_literal = new SignedDoubleWordIntegerLiteral{ static_cast<std::int32_t>(integer_value) }; break;
			case S64: integer_literal = new SignedQuadWordIntegerLiteral{ static_cast<std::int64_t>(integer_value) }; break;

			default:
			{
				CommonErrors::invalid_token_type(specifier.type, specifier);
				integer_literal = nullptr;
			} break;
		}

		consume(specifier.type);

		return new IntegerLiteralExpression{ integer_literal, integer_literal_token };
	}

	StringExpression* Parser::parse_string_expression()
	{
		const auto message_token = consume(TokenType::STRING);
		return new StringExpression{ std::move(message_token.value), message_token };
	}

	FunctionCallExpression* Parser::parse_functioncall_expression()
	{
		const auto name_token = consume(TokenType::IDENTIFIER);

		consume(TokenType::LPAREN);
		auto arguments = AS_COMPILER_PARSER(this)->parse_arguments(false);
		consume(TokenType::RPAREN);

		if (!symbol_table.contains(name_token.value))
		{
			_error_reporter->post_error(std::format("function `{}` is undefined", name_token.value), name_token);
			return nullptr;
		}

		auto symbol = symbol_table.at(name_token.value);

		if (symbol->ytype() != SymbolType::FUNCTION)
		{
			_error_reporter->post_error(std::format("symbol `{}` is a {} but was referenced as a function", 
				name_token.value, _symbol_map.at(symbol->ytype())), name_token);
			return nullptr;
		}

		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		if (function_symbol->arity() != arguments.size())
		{
			_error_reporter->post_error(std::format("function `{}` was defined with {} arguments but called with {}",
				name_token.value, function_symbol->arity(), arguments.size()), name_token);
			return nullptr;
		}

		return new FunctionCallExpression{ name_token.value, std::move(arguments), name_token };
	}

	Expression* Parser::parse_parenthesis_expression()
	{
		consume(TokenType::LPAREN);
		const auto expression = parse_expression();
		consume(TokenType::RPAREN);

		return expression;
	}

	AdjustExpression* Parser::parse_increment_expression()
	{
		consume(TokenType::TILDE);
		
		if (peek().type == TokenType::IDENTIFIER)
		{
			auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ true, identifier_expression, identifier_expression->_token };
		}

		else if (peek().type == TokenType::INT)
		{
			auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ true, integer_literal_expression, integer_literal_expression->_token };
		}

		_error_reporter->post_error("increment target must evaluate to a modifiable l-value", peek());
		return nullptr;
	}

	AdjustExpression* Parser::parse_decrement_expression()
	{
		consume(TokenType::EXCLAMATION);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ false, identifier_expression, identifier_expression->_token };
		}

		else if (peek().type == TokenType::INT)
		{
			auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ false, integer_literal_expression, integer_literal_expression->_token };
		}

		_error_reporter->post_error("decrement target must evaluate to a modifiable l-value", peek());
		return nullptr;
	}


	Expression* Parser::parse_generic_expression()
	{
		using enum TokenType;
		switch (peek().type)
		{
			case LPAREN:
			{
				return parse_parenthesis_expression();
			} break;

			case INT:
			{
				return parse_integerliteral_expression();
			} break;
			
			case STRING:
			{
				return parse_string_expression();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().type == LPAREN)
				{
					return parse_functioncall_expression();
				}

				return parse_identifier_expression();
			} break;

			case TILDE:
			{
				return parse_increment_expression();
			} break;

			case EXCLAMATION:
			{
				return parse_decrement_expression();
			} break;

			default:
			{
				_error_reporter->post_error(std::format("expected an expression but got `{}`", peek().value), peek());
				return nullptr;
			} break;
		}
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
		// parse until the minimum precedence level is reached (so parse everything possible basically)
		auto infix_expression = parse_infix_expression(parse_generic_expression(), Precedence::MINIMUM);

		if (infix_expression != nullptr)
		{
			return AS_EXPRESSION(infix_expression->copy());
		}

		return nullptr;
	}

	Expression* Parser::parse_infix_expression(Expression* left, Precedence min_precedence)
	{
		static const std::unordered_map<TokenType, Precedence> precedences
		{
			{ TokenType::EQUALS,            Precedence::ASSIGN },
			{ TokenType::EQUALSEQUALS,      Precedence::EQUALITY },
			{ TokenType::EXCLAMATIONEQUALS, Precedence::EQUALITY },
			{ TokenType::GREATER,           Precedence::COMPARE },
			{ TokenType::LESS,              Precedence::COMPARE },
			{ TokenType::PLUS,              Precedence::TERM },
			{ TokenType::MINUS,             Precedence::TERM },
			{ TokenType::STAR,              Precedence::FACTOR },
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
				case TokenType::PLUS: left = new PlusBinaryExpression{ left, right, left->_token }; break;
				case TokenType::MINUS: left = new MinusBinaryExpression{ left, right, left->_token }; break;
				case TokenType::STAR: left = new TimesBinaryExpression{ left, right, left->_token }; break;

				case TokenType::EQUALS: left = new AssignBinaryExpression{ left, right, left->_token }; break;

				case TokenType::EQUALSEQUALS: left = new EqualityBinaryExpression{ left, right, left->_token }; break;
				case TokenType::EXCLAMATIONEQUALS: left = new InequalityBinaryExpression{ left, right, left->_token }; break;

				case TokenType::GREATER: left = new GreaterBinaryExpression{ left, right, left->_token }; break;
				case TokenType::LESS: left = new LessBinaryExpression{ left, right, left->_token }; break;
			}
		} while (true);

		return left;
	}

	IdentifierExpression* Parser::parse_identifier()
	{
		const auto name_token = consume(TokenType::IDENTIFIER);
		return new IdentifierExpression{ name_token.value, name_token };
	}
}
