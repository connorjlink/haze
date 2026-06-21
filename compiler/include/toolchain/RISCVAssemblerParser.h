#ifndef HAZE_RISCVASSEMBLERPARSER_H
#define HAZE_RISCVASSEMBLERPARSER_H

#include <command/Command.h>
#include <riscv/defs/RISCVInstructionKind.h>
#include <toolchain/AssemblerParser.h>

// Haze RISCVAssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Command;

	struct RISCVAssemblerParser : public AssemblerParser
	{
	private:
#define X(enumerator, name) CommandReference<InstructionCommand> parse_##enumerator##_instruction();
		RISCV_INSTRUCTION_KINDS(X)
#undef X

	public:
		CommandReference<InstructionCommand> parse_instruction_command_implementation();
		Register parse_register_implementation();

	public:
		using AssemblerParser::AssemblerParser;
	};
}

#endif
