#ifndef HAZE_ASSEMBLERPARSER_H
#define HAZE_ASSEMBLERPARSER_H

#include <toolchain/Parser.h>
#include <cli/ArchitectureType.h>
#include <utility/Constants.h>

// Haze AssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Instruction;

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
		virtual ArchitectureType ctype() const = 0;

	protected:
		virtual Instruction* parse_instruction_command() = 0;
		virtual std::int8_t parse_register() = 0;

	public:
		using Parser::Parser;
	};
}

#endif
