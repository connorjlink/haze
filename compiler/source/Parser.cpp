import std;

#include <ast/expression/Expression.h>
#include <command/Command.h>
#include <error/CommonErrors.h>
#include <error/ErrorReporter.h>
#include <symbol/models/Symbol.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/Parser.h>
#include <toolchain/CompilerParser.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Parser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	bool is_binary_operator(TokenKind kind)
	{
		using enum TokenKind;
		switch (kind)
		{
			case AMPERSAND:            [[fallthrough]];
			case AMPERSANDEQUALS:      [[fallthrough]];
			case AMPERSANDAMPERSAND:   [[fallthrough]];
			case CARET:                [[fallthrough]];
			case CARETEQUALS:          [[fallthrough]];
			case COMMA:                [[fallthrough]];
			case EQUALS:               [[fallthrough]];
			case EQUALSEQUALS:         [[fallthrough]];
			case EXCLAMATIONEQUALS:    [[fallthrough]];
			case GREATER:              [[fallthrough]];
			case GREATEREQUALS:        [[fallthrough]];
			case GREATERGREATER:       [[fallthrough]];
			case GREATERGREATEREQUALS: [[fallthrough]];
			case LESS:                 [[fallthrough]];
			case LESSEQUALS:           [[fallthrough]];
			case LESSLESS:             [[fallthrough]];
			case LESSLESSEQUALS:       [[fallthrough]];
			case MINUS:                [[fallthrough]];
			case MINUSEQUALS:          [[fallthrough]];
			case PERCENT:              [[fallthrough]];
			case PERCENTEQUALS:        [[fallthrough]];
			case PIPE:                 [[fallthrough]];
			case PIPEEQUALS:           [[fallthrough]];
			case PIPEPIPE:             [[fallthrough]];
			case PLUS:                 [[fallthrough]];
			case PLUSEQUALS:           [[fallthrough]];
			case SLASH:                [[fallthrough]];
			case SLASHEQUALS:          [[fallthrough]];
			case STAR:                 [[fallthrough]];
			case STAREQUALS:
				return true;
		};

		return false;
	}

	std::expected<char, std::string_view> parse_char_literal(std::string_view character)
	{
		if (character.size() < 1)
		{
			return std::unexpected{ "invalid empty character literal" };
		}

		if (character[0] != '\\')
		{
			if (character.size() != 1)
			{
				return std::unexpected{ "invalid multi-character literal" };
			}
			// canonical single character literal
			return character[0];
		}

		if (character.size() == 1)
		{
			return std::unexpected{ "incomplete escape sequence" };
		}

		// classic escape sequences
		switch (character[1])
		{
			case 'n':  return '\n';
			case 't':  return '\t';
			case 'r':  return '\r';
			case '\\': return '\\';
			case '\'': return '\'';
			case '"':  return '"';
			case 'a':  return '\a';
			case 'b':  return '\b';
			case 'f':  return '\f';
			case 'v':  return '\v';
			case 'x':
			{
				// hex escape code
				if (character.size() == 2)
				{
					return std::unexpected{ "missing hex digits in \\x escape sequence" };
				}

				auto value = 0u;
				auto i = 2uz;

				while (i < character.size())
				{
					auto digit = 0u;

					// convert ASCII to hexadecimal numeric value
					     if (character[i] >= '0' && character[i] <= '9') digit = character[i] - '0';
					else if (character[i] >= 'a' && character[i] <= 'f') digit = 10 + (character[i] - 'a');
					else if (character[i] >= 'A' && character[i] <= 'F') digit = 10 + (character[i] - 'A');
					else break;

					value = (value << 4) | digit;
					++i;
				}

				if (i == 2)
				{
					return std::unexpected{ "invalid hex escape sequence" };
				}

				if (i != character.size())
				{
					return std::unexpected{ "invalid trailing characters after hex escape sequence" };
				}

				return static_cast<char>(value & 0xFF);
			} break;

			case '0': [[fallthrough]];
			case '1': [[fallthrough]];
			case '2': [[fallthrough]];
			case '3': [[fallthrough]];
			case '4': [[fallthrough]];
			case '5': [[fallthrough]];
			case '6': [[fallthrough]];
			case '7':
			{
				// octal escape sequence
				auto i = 1uz;
				auto value = 0u;
				auto count = 0;

				while (i < character.size() && count < 3)
				{
					if (character[i] < '0' || character[i] > '7')
					{
						// non-octal digits indicate the end of octal escape sequence
						break;
					}

					value = (value << 3) + (character[i] - '0');
					i++;
					count++;
				}

				if (i != character.size())
				{
					return std::unexpected{ "invalid trailing characters after octal escape sequence" };
				}

				// values beyond those representable with char are implementation defined
				return static_cast<char>(value & 0xFF);
			} break;
		}
		
		return std::unexpected{ "unsupported escape sequence" };
	}
}

namespace hz
{
	const Token& Parser::consume(TokenKind kind)
	{
		const auto& current = peek();
		if (current.kind == kind)
		{
			cursor++;
			return current;
		}

		auto convert = [&](auto v) -> std::string_view
		{
			const auto item = token_map.at(v);
			if (item)
			{
				return *item;
			}

			USE_SAFE(ErrorReporter)->post_error("invalid token", current);
			return "<error>";
		};

		const auto text =  ((current.kind == TokenKind::IDENTIFIER || current.kind == TokenKind::INT)
			? current.text
			: convert(current.kind));

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"expected token `{}` but got `{}`", convert(kind), text), current);
		return current;
	}

	std::vector<Token> Parser::fetch_until(TokenKind kind)
	{
		auto result = std::vector<Token>{};

		while (peek().kind != kind)
		{
			result.emplace_back(peek());
			consume(peek().kind);
		}

		return result;
	}

	ExpressionReference<IdentifierExpression> Parser::parse_identifier_expression()
	{
		const auto token = consume(TokenKind::IDENTIFIER);
		const auto identifier = token.text;

		return MAKE_IDENTIFIER_EXPRESSION(token, IdentifierExpression::Kind::UNKNOWN, identifier);
	}

	ExpressionReference<IntegerLiteralExpression> Parser::parse_integer_literal_expression()
	{
		const auto token = consume(TokenKind::INTEGER_LITERAL);

		auto value = BigInteger{};

		const auto [pointer, error] = std::from_chars(token.text.data(), token.text.data() + token.text.size(), value);
		if (error != std::errc{})
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"unparseable integer literal `{}`", token.text), token);
			return MAKE_INVALID_REFERENCE(IntegerLiteralExpression, Expression, expression_storage);
		}

		const auto& specifier = peek();
		consume(specifier.kind);

		return MAKE_INTEGER_LITERAL_EXPRESSION(token, value);
	}

	ExpressionReference<StringLiteralExpression> Parser::parse_string_literal_expression()
	{
		const auto token = consume(TokenKind::STRING_LITERAL);
		// skip double quotes
		const auto string = token.text.substr(1, token.text.size() - 2);

		return MAKE_STRING_LITERAL_EXPRESSION(token, string);
	}

	ExpressionReference<CharacterLiteralExpression> Parser::parse_character_literal_expression()
	{
		const auto token = consume(TokenKind::CHARACTER_LITERAL);
		// skip single quotes
		const auto character = token.text.substr(1, token.text.size() - 2);

		if (character.size() != 1 || (character[0] == '\\' && character.size() != 2))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid character literal `{}`", token.text), token);
			return MAKE_INVALID_REFERENCE(CharacterLiteralExpression, Expression, expression_storage);
		}

		return MAKE_CHARACTER_LITERAL_EXPRESSION(token, character);
	}


	ExpressionHandle Parser::parse_parenthesis_expression()
	{
		consume(TokenKind::LPAREN);
		const auto expression = parse_expression();
		consume(TokenKind::RPAREN);

		return expression;
	}

	ExpressionHandle Parser::parse_generic_expression()
	{
		using enum TokenKind;
		switch (peek().kind)
		{
			case LPAREN:
			{
				return parse_parenthesis_expression();
			} break;

			case INTEGER_LITERAL:
			{
				return parse_integer_literal_expression().erase();
			} break;
			
			case CHARACTER_LITERAL:
			{
				return parse_character_literal_expression().erase();
			} break;

			case DOUBLEQUOTE:
			{
				return parse_string_literal_expression().erase();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().kind == LPAREN)
				{
					return parse_functioncall_expression().erase();
				}

				return parse_identifier_expression().erase();
			} break;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"expected an expression but got `{}`", peek().text), peek());
		return MAKE_INVALID_HANDLE(Expression, expression_storage);
	}

	ExpressionHandle Parser::parse_expression_optimized()
	{
		auto expression = parse_expression();

		while (auto expression_optimized = expression.optimize(expression_storage))
		{
			expression = expression_optimized;
		}
		
		return expression;
	}

	ExpressionHandle Parser::parse_expression()
	{
		// parse until the minimum precedence level is reached (so parse everything possible basically)
		const auto infix_expression = parse_infix_expression(parse_generic_expression(), std::numeric_limits<Precedence>::max());
		if (!infix_expression)
		{
			return MAKE_INVALID_HANDLE(Expression, expression_storage);
		}

		return infix_expression;
	}

	ExpressionHandle Parser::parse_infix_expression(ExpressionHandle left, Precedence minimum_precedence)
	{
		static const std::unordered_map<TokenKind, Precedence> precedences
		{
#define X(enumerator, token, associativity, precedence, type, name) { TokenKind::token, precedence },
			BINARY_EXPRESSION_KINDS(X)
#undef X
		};

		static const std::unordered_map<TokenKind, Associativity> associativities
		{
#define X(enumerator, token, associativity, precedence, type, name) { TokenKind::token, Associativity::associativity },
			BINARY_EXPRESSION_KINDS(X)
#undef X
		};

		do
		{
			const auto& next = peek();

			if (!is_binary_operator(next.kind))
			{
				break;
			}

			const auto next_precedence = precedences.at(next.kind);
			if (next_precedence > minimum_precedence)
			{
				break;
			}

			consume(next.kind);
			auto right = parse_generic_expression();

			do
			{
				const auto& lookahead = peek();

				if (!is_binary_operator(lookahead.kind))
				{
					break;
				}

				const auto lookahead_precedence = precedences.at(lookahead.kind);
				const auto lookahead_associativity = associativities.at(lookahead.kind);

				if (lookahead_precedence < next_precedence || (lookahead_precedence == next_precedence && lookahead_associativity == Associativity::RIGHT))
				{
					right = parse_infix_expression(right, lookahead_precedence);
					continue;
				}
				
				break;
			} while (true);

			switch (next.kind)
			{
#define X(enumerator, token, macro, associativity, precedence, type, name) case TokenKind::token: left = MAKE_##macro##_EXPRESSION(next, left, right).erase(); break;
				BINARY_EXPRESSION_KINDS(X)
#undef X
				case TokenKind::STAR:                 left = MAKE_MULTIPLY_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::SLASH:                left = MAKE_DIVIDE_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PERCENT:              left = MAKE_MODULO_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PLUS:                 left = MAKE_ADD_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::MINUS:                left = MAKE_SUBTRACT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::LESSLESS:             left = MAKE_LEFT_SHIFT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::GREATERGREATER:       left = MAKE_RIGHT_SHIFT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::LESS:                 left = MAKE_LESS_THAN_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::LESSEQUALS:           left = MAKE_LESS_THAN_OR_EQUAL_TO_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::GREATER:              left = MAKE_GREATER_THAN_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::GREATEREQUALS:        left = MAKE_GREATER_THAN_OR_EQUAL_TO_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::EQUALSEQUALS:         left = MAKE_EQUAL_TO_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::EXCLAMATIONEQUALS:    left = MAKE_NOT_EQUAL_TO_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::AMPERSAND:            left = MAKE_BITWISE_AND_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::CARET:                left = MAKE_BITWISE_XOR_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PIPE:                 left = MAKE_BITWISE_OR_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::AMPERSANDAMPERSAND:   left = MAKE_LOGICAL_AND_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PIPEPIPE:             left = MAKE_LOGICAL_OR_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::EQUALS:               left = MAKE_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PLUSEQUALS:           left = MAKE_ADD_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::MINUSEQUALS:          left = MAKE_SUBTRACT_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::STAREQUALS:           left = MAKE_MULTIPLY_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::SLASHEQUALS:          left = MAKE_DIVIDE_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PERCENTEQUALS:        left = MAKE_MODULO_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::LESSLESSEQUALS:       left = MAKE_LEFT_SHIFT_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::GREATERGREATEREQUALS: left = MAKE_RIGHT_SHIFT_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::AMPERSANDEQUALS:      left = MAKE_BITWISE_AND_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::CARETEQUALS:          left = MAKE_BITWISE_XOR_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::PIPEEQUALS:           left = MAKE_BITWISE_OR_ASSIGNMENT_EXPRESSION(next, left, right).erase(); break;
				case TokenKind::COMMA:                left = MAKE_COMMA_EXPRESSION(next, left, right).erase(); break;
				default: break;
			}
		} while (true);

		return left;
	}


	void Parser::reload(std::vector<Token> tokens)
	{
		this->cursor = 0;
		this->tokens = std::move(tokens);
	}

	Parser::Parser(const std::filesystem::path& filepath, ExpressionStorage& expression_storage)
		: expression_storage{ expression_storage }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "parsing");
	}

	Parser::~Parser()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}
}
