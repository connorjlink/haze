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
			case PIPE:                 [[fallthrough]];
			case PIPEEQUALS:           [[fallthrough]];
			case PIPEPIPE:             [[fallthrough]];
			case PLUS:                 [[fallthrough]];
			case PLUSEQUALS:           [[fallthrough]];
			case MINUS:                [[fallthrough]];
			case MINUSEQUALS:          [[fallthrough]];
			case STAR:                 [[fallthrough]];
			case STAREQUALS:           [[fallthrough]];
			case SLASH:                [[fallthrough]];
			case SLASHEQUALS:          [[fallthrough]];
			case PERCENT:              [[fallthrough]];
			case PERCENTEQUALS:        [[fallthrough]];
			case CARET:                [[fallthrough]];
			case CARETEQUALS:          [[fallthrough]];
			case EQUALSEQUALS:         [[fallthrough]];
			case EXCLAMATIONEQUALS:    [[fallthrough]];
			case GREATER:              [[fallthrough]];
			case GREATEREQUALS:        [[fallthrough]];
			case GREATERGREATER:       [[fallthrough]];
			case GREATERGREATEREQUALS: [[fallthrough]];
			case LESS:                 [[fallthrough]];
			case LESSEQUALS:           [[fallthrough]];
			case LESSLESS:             [[fallthrough]];
			case LESSLESSEQUALS:
				return true;
		};

		return false;
	}
}

namespace hz
{
	Token Parser::consume(TokenKind kind)
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
		const auto name_token = consume(TokenKind::IDENTIFIER);
		
		return MAKE_IDENTIFIER_EXPRESSION(IdentifierExpression::Kind::UNKNOWN, name_token.text);
	}

	ExpressionReference<IntegerLiteralExpression> Parser::parse_integerliteral_expression()
	{
		const auto integer_literal_token = consume(TokenKind::INT);
		const auto integer_string = integer_literal_token.text;

		BigInteger integer_value{};

		const auto [pointer, error] = std::from_chars(integer_string.data(), integer_string.data() + integer_string.size(), integer_value);
		if (error != std::errc{})
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"unparseable integer literal `{}`", integer_string), integer_literal_token);
			return nullptr;
		}

		const auto& specifier = peek();
		consume(specifier.kind);

		return new IntegerLiteralExpression{ integer_value, integer_literal_token };
	}

	ExpressionReference<StringLiteralExpression> Parser::parse_string_literal_expression()
	{
		consume(TokenKind::DOUBLEQUOTE);
		const auto message_token = fetch_until(TokenKind::DOUBLEQUOTE);
		consume(TokenKind::DOUBLEQUOTE);

		return MAKE_STRING_LITERAL_EXPRESSION(message_token);
	}

	ExpressionHandle Parser::parse_parenthesis_expression()
	{
		consume(TokenKind::LPAREN);
		const auto expression = parse_expression();
		consume(TokenKind::RPAREN);

		return expression;
	}

	AdjustExpression* Parser::parse_increment_expression()
	{
		consume(TokenKind::TILDE);
		
		if (peek().kind == TokenKind::IDENTIFIER)
		{
			const auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ true, identifier_expression, identifier_expression->_token };
		}

		else if (peek().kind == TokenKind::INT)
		{
			const auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ true, integer_literal_expression, integer_literal_expression->_token };
		}

		USE_SAFE(ErrorReporter)->post_error("increment target must evaluate to a modifiable l-value", peek());
		return nullptr;
	}

	AdjustExpression* Parser::parse_decrement_expression()
	{
		consume(TokenKind::EXCLAMATION);

		if (peek().kind == TokenKind::IDENTIFIER)
		{
			const auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ false, identifier_expression, identifier_expression->_token };
		}

		else if (peek().kind == TokenKind::INT)
		{
			const auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ false, integer_literal_expression, integer_literal_expression->_token };
		}

		USE_SAFE(ErrorReporter)->post_error("decrement target must evaluate to a modifiable l-value", peek());
		return nullptr;
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

			case INT:
			{
				return parse_integerliteral_expression();
			} break;
			
			case DOUBLEQUOTE:
			{
				return parse_string_expression();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().kind == LPAREN)
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
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"expected an expression but got `{}`", peek().text), peek());
		return MAKE_INVALID_HANDLE(storage, Expression);
	}

	ExpressionHandle Parser::parse_expression_optimized()
	{
		auto expression = parse_expression();

		while (auto expression_optimized = expression.optimize(storage))
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
			return MAKE_INVALID_HANDLE(storage, Expression);
		}

		return infix_expression;
	}

	ExpressionHandle Parser::parse_infix_expression(ExpressionHandle left, Precedence minimum_precedence)
	{
		static const std::unordered_map<TokenKind, Precedence> precedences
		{
			{ TokenKind::EQUALS,            Precedence::ASSIGN },
			{ TokenKind::EQUALSEQUALS,      Precedence::EQUALITY },
			{ TokenKind::EXCLAMATIONEQUALS, Precedence::EQUALITY },
			{ TokenKind::GREATER,           Precedence::COMPARE },
			{ TokenKind::LESS,              Precedence::COMPARE },
			{ TokenKind::PLUS,              Precedence::TERM },
			{ TokenKind::MINUS,             Precedence::TERM },
			{ TokenKind::STAR,              Precedence::FACTOR },
		};

		do
		{
			const auto& next = peek();

			if (!is_binary_operator(next.kind) || precedences.at(next.kind) < minimum_precedence)
			{
				break;
			}

			consume(next.kind);

			auto right = parse_expression();

			do
			{
				const auto& lookahead = peek();

				if (!is_binary_operator(lookahead.kind) || precedences.at(lookahead.kind) <= precedences.at(next.kind))
				{
					break;
				}

#pragma message("TODO: maybe add precedence+1 for the next call of the recursive function")
				right = parse_infix_expression(right, precedences.at(lookahead.kind));

			} while(true);

			switch (next.kind)
			{
				case TokenKind::PLUS: left = new PlusBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::MINUS: left = new MinusBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::STAR: left = new TimesBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::EQUALS: left = new AssignBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::EQUALSEQUALS: left = new EqualityBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::EXCLAMATIONEQUALS: left = new InequalityBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::GREATER: left = new GreaterBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::LESS: left = new LessBinaryExpression{ left, right, left->_token }; break;
			}
		} while (true);

		return left;
	}


	void Parser::reload(std::vector<Token> tokens)
	{
		this->cursor = 0;
		this->tokens = std::move(tokens);
	}

	Parser::Parser(const std::filesystem::path& filepath, const ExpressionStorage& storage)
		: storage{ storage }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "parsing");
	}

	Parser::~Parser()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}
}
