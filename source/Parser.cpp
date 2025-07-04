import std;

#include <ast/AdjustExpression.h>
#include <ast/BinaryExpression.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/FunctionCallExpression.h>
#include <ast/StringExpression.h>
#include <command/DotDefineCommand.h>
#include <error/CommonErrors.h>
#include <error/ErrorReporter.h>
#include <symbol/Symbol.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/Parser.h>
#include <toolchain/CompilerParser.h>
#include <type/Type.h>
#include <utility/ExtendedInteger.h>

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
	Parser::Parser(const std::string& filepath)
		: cursor{ 0 }, _tokens{ _tokens }, _filepath{ filepath }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "parsing");
	}

	Parser::~Parser()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}


	Token& Parser::lookbehind()
	{
		if (cursor > 0)
		{
			return _tokens[cursor - 1];
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable("invalid token backtrack", peek());
	}

	Token& Parser::peek()
	{
		return _tokens[cursor];
	}

	Token& Parser::lookahead()
	{
		if (cursor < _tokens.size() - 1)
		{
			return _tokens[cursor + 1];
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable("unexpectedly reached the end of file", peek());
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

			USE_SAFE(ErrorReporter)->post_error("invalid token", current);
			return { "[error]" };
		};

		USE_SAFE(ErrorReporter)->post_error(std::format("expected token `{}` but got `{}`",
			convert(token), ((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.text : convert(current.type))), current);
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

#pragma message ("TODO: refactor the dotdefine to be overriden by subclasses")
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
			// default to unsigned 32 bits non-compiler workloads since we don't have the machinery for type resolution otherwise
			type = new IntType{ TypeQualifier::IMMUTABLE, TypeSignedness::UNSIGNED, IntTypeType::INT32, TypeStorage::VALUE };
		}
		
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenType::EQUALS);

		// if the optimized value is not a constant expression, it isn't a valid definition
		const auto value_expression = parse_expression_optimized();

		if (value_expression->etype() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error("definitions must result in a constant expression", value_expression->_token);
			return nullptr;
		}

		const auto& identifier = identifier_expression->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		USE_SAFE(SymbolDatabase)->add_define(identifier, peek());
		
		auto define_symbol = USE_SAFE(SymbolDatabase)->reference_define(identifier, peek());
		define_symbol->type = type;
		define_symbol->value = integer_literal_raw(value);

		// NOTE: exports the constant expression name symbol only
		USE_SAFE(SymbolExporter)->enqueue(define_symbol, identifier_expression->_token);

		return new DotDefineCommand{ identifier, value, identifier_expression->_token };
	}

	IdentifierExpression* Parser::parse_identifier_expression(IdentifierType itype)
	{
		const auto name_token = consume(TokenType::IDENTIFIER);
		return new IdentifierExpression{ name_token.text, name_token, itype };
	}

	IntegerLiteralExpression* Parser::parse_integerliteral_expression()
	{
		const auto integer_literal_token = consume(TokenType::INT);
		const auto& integer_string = integer_literal_token.text;

		std::int64_t integer_value{};
		auto [ptr, ec] = std::from_chars(integer_string.data(), integer_string.data() + integer_string.size(), integer_value);

		if (ec != std::errc())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("unparseable integer literal `{}`", integer_string), integer_literal_token);
			return nullptr;
		}

		// NOTE: hazels-server handles integer literal "symbols" instead

		IntegerLiteral* integer_literal = nullptr;

		const auto& specifier = peek();
		
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
		return new StringExpression{ std::move(message_token.text), message_token };
	}

	FunctionCallExpression* Parser::parse_functioncall_expression()
	{
		const auto name_token = consume(TokenType::IDENTIFIER);

		consume(TokenType::LPAREN);
		auto arguments = AS_COMPILER_PARSER(this)->parse_arguments(false);
		consume(TokenType::RPAREN);

		if (!USE_SAFE(SymbolDatabase)->has_symbol(name_token.text))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("function `{}` is undefined", name_token.text), name_token);
			return nullptr;
		}

		const auto function_symbol = USE_SAFE(SymbolDatabase)->reference_function(name_token.text, name_token);

		if (function_symbol->arity() != arguments.size())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format("function `{}` was defined with {} arguments but called with {}",
				name_token.text, function_symbol->arity(), arguments.size()), name_token);
			return nullptr;
		}

		// NOTE: exports the function name symbol only
		USE_SAFE(SymbolExporter)->enqueue(function_symbol, name_token);

		return new FunctionCallExpression{ name_token.text, std::move(arguments), name_token };
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

		USE_SAFE(ErrorReporter)->post_error("increment target must evaluate to a modifiable l-value", peek());
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

		USE_SAFE(ErrorReporter)->post_error("decrement target must evaluate to a modifiable l-value", peek());
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
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"expected an expression but got `{}`", peek().text), peek());
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

#pragma message("TODO: maybe add precedence+1 for the next call of the recursive function")
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

	void Parser::reload(const std::vector<Token>& tokens, const std::string& filepath)
	{
		_tokens = tokens;
		_filepath = filepath;
	}
}
