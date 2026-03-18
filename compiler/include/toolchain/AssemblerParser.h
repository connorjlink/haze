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

	private:
		std::vector<Node*> parse_commands();

	public:
		virtual ParserType ptype() const final override;
		virtual std::vector<Node*> parse() final override;

	public:
		virtual AssemblerParserType atype() const = 0;

	private:
		virtual Node* parse_instruction_command() const = 0;
		virtual register_t parse_register() const = 0;

	public:
		using Parser::Parser;
	};
}

#endif
