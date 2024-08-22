#include "InterpreterParser.h"
#include "Log.h"

#include "ColorIntrinsic.h"

namespace hz
{
	Intrinsic* InterpreterParser::parse_intrinsic()
	{
		DISCARD consume(TokenType::INTRINSIC);

		auto identifier = parse_identifier_expression();

		DISCARD consume(TokenType::EQUALS);

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

				auto percent = [&](std::uint16_t value)
				{
					return static_cast<float>(value * 100);
				};

				intrinsic = new ColorIntrinsic{ std::move(identifier->name), percent(r->value), percent(g->value), percent(b->value) };
			} break;
			
			default:
			{
				Log::error(std::format("({}) unrecognized intrinsic declaration", peek().line));
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
			default: Log::error(std::format("({}) unrecognized script declarator", peek().line));
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