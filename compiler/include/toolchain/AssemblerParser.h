#ifndef HAZE_ASSEMBLERPARSER_H
#define HAZE_ASSEMBLERPARSER_H

#include <toolchain/Parser.h>
#include <toolchain/AssemblerParserType.h>
#include <utility/Constants.h>

// Haze AssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerParser : public Parser
	{
	private:
		Expression* parse_literal();

	private:
		Node* parse_dotorg_command();
		Node* parse_label_command();
		Node* parse_command();

	public:
		virtual ParserType ptype() const final override;
		virtual std::vector<Node*> parse() final override;

	public:
		virtual AssemblerParserType atype() const = 0;

	protected:
		virtual Node* parse_instruction_command() = 0;
		virtual register_t parse_register() = 0;

	public:
		using Parser::Parser;
	};
}

#endif
