#ifndef HAZE_INTERPRETERPARSER_H
#define HAZE_INTERPRETERPARSER_H

#include "Parser.h"

#include "IntrinsicDeclarator.h"

namespace hz
{
	class InterpreterParser : public Parser
	{
	private:
		

	private:
		InstrinsicDeclarator* parse_intrinsic();

	private:
		Node* parse_declarator();
		std::vector<Node*> parse_declarators();

	public:
		virtual std::vector<Node*> parse() final override;

	public:
		using Parser::Parser;
	};
}

#endif
