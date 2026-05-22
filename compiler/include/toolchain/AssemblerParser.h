#ifndef HAZE_ASSEMBLERPARSER_H
#define HAZE_ASSEMBLERPARSER_H

#include <toolchain/Parser.h>
#include <cli/ArchitectureKind.h>
#include <command/Command.h>
#include <utility/Constants.h>

// Haze AssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Instruction;

	class AssemblerParser : public Parser
	{
	private:
		ExpressionHandle parse_literal();

	private:
		CommandHandle parse_dotorg_command();
		CommandHandle parse_label_command();
		CommandHandle parse_command();

	public:
		virtual ParserType ptype() const final override;
		virtual std::vector<Node*> parse() final override;

	public:
		virtual ArchitectureKind ctype() const = 0;

	protected:
		virtual Instruction* parse_instruction_command() = 0;
		virtual Register parse_register() = 0;

	public:
		using Parser::Parser;
	};
}

#endif
