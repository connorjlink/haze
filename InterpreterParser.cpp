import std;

#include "InterpreterParser.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "ColorIntrinsic.h"
#include "ErrorReporter.h"

// Haze InterpreterParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ParserType InterpreterParser::ptype() const
	{
		return ParserType::INTERPRETER;
	}

	Intrinsic* InterpreterParser::parse_intrinsic()
	{
		const auto intrinsic_token = consume(TokenType::INTRINSIC);

		auto identifier = parse_identifier_expression();

		consume(TokenType::EQUALS);

		Intrinsic* intrinsic = nullptr;

		using enum TokenType;
		switch (peek().type)
		{
			// Color intrinsic parsing
			case LBRACKET:
			{
				// TODO: support real expressions instead of only literals
				auto r = parse_integerliteral_expression();
				auto g = parse_integerliteral_expression();
				auto b = parse_integerliteral_expression();

				auto percent = [&](std::uint32_t value)
				{
					return static_cast<float>(value * 100);
				};

				intrinsic = new ColorIntrinsic{ std::move(identifier->name), percent(r->value), percent(g->value), percent(b->value), intrinsic_token };
			} break;
			
			default:
			{
				_error_reporter->post_error(std::format("invalid intrinsic type `{}`", peek().value), peek());
			} break;
		}

		return intrinsic;
	}

	Node* InterpreterParser::parse_declarator()
	{
		switch (peek().type)
		{
			case TokenType::DOTDEFINE: return parse_dotdefine_command();
			case TokenType::INTRINSIC: return parse_intrinsic();
			case TokenType::FUNCTION: return parse_function();

			default:
			{
				_error_reporter->post_error(std::format("invalid script declarator `{}`", peek().value), peek());
				return nullptr;
			} break;
		}
	}

	std::vector<Node*> InterpreterParser::parse_declarators()
	{
		std::vector<Node*> declarators;

		while (peek().type != TokenType::END)
		{
			declarators.emplace_back(parse_declarator());
		}

		return declarators;
	}

	std::vector<Node*> InterpreterParser::parse()
	{
		return parse_declarators();
	}
}