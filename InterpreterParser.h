#ifndef HAZE_INTERPRETERPARSER_H
#define HAZE_INTERPRETERPARSER_H

#include "CompilerParser.h"

#include "Intrinsic.h"

namespace hz
{
	class InterpreterParser : public CompilerParser
	{
	private:
		Intrinsic* parse_intrinsic();

	private:
		Node* parse_declarator();
		std::vector<Node*> parse_declarators();

	public:
		virtual std::vector<Node*> parse() final override;

	public:
		using CompilerParser::CompilerParser;
	};
}

#endif
