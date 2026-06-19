#ifndef HAZE_RISCVASSEMBLERPARSER_H
#define HAZE_RISCVASSEMBLERPARSER_H

#include <cli/defs/ArchitectureKind.h>
#include <toolchain/AssemblerParser.h>

// Haze RISCVAssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Command;

	struct RISCVAssemblerParser : public AssemblerParser
	{
	public:
		CommandReference<InstructionCommand> parse_instruction_command() const;
		Register parse_register() const;

	public:
		using AssemblerParser::AssemblerParser;
	};
}

#endif
