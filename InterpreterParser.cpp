#include "InterpreterParser.h"
#include "Log.h"

namespace hz
{
	IntrinsicDeclarator* InterpreterParser::parse_intrinsic_declarator()
	{
		DISCARD consume(TokenType::INTRINSIC);

		auto identifier = parse_identifier_expression();

		DISCARD consume(TokenType::EQUALS);

		Intrinsic* intrinsic = nullptr;

		using enum TokenType;
		switch (peek().type)
		{
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

				intrinsic = new ColorIntrinsic{ percent(r->value), percent(g->value), percent(b->value) };
			} break;
			
			default:
			{
				Log::error("Unrecognized intrinsic declaration");
			} break;
		}

		return new IntrinsicDeclarator{ identifier->name, intrinsic };
	}

	Node* InterpreterParser::parse_declarator()
	{
		switch (peek().type)
		{
			case TokenType::INTRINSIC: return parse_intrinsic_declarator();
			default: Log::error("Unrecognized script declarator");
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