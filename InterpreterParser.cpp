#include "InterpreterParser.h"
#include "Log.h"

namespace hz
{
	InstrinsicDeclarator* InterpreterParser::parse_intrinsic()
	{
		DISCARD consume(TokenType::INTRINSIC);
	}

	Node* InterpreterParser::parse_declarator()
	{
		switch (peek().type)
		{
			case TokenType::INTRINSIC: return parse_intrinsic();
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