#ifndef HAZE_ASSEMBLERPARSER_H
#define HAZE_ASSEMBLERPARSER_H

#include <cli/defs/ArchitectureKind.h>
#include <command/Command.h>
#include <toolchain/Parser.h>

// Haze AssemblerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct AssemblerParser 
		: public Parser
		, public ServiceTag<AssemblerParser>
#pragma message("TODO: figure out how to get virtual dependencies sorted through the services (i.e., register the Parser and polymorphically initialize with AssemblerParser")
	{
	private:
		Address base;

	public:
		virtual ArchitectureKind ctype() const = 0;;
		virtual CommandReference<InstructionCommand> parse_instruction_command() const = 0;
		virtual Register parse_register() const = 0;

	public:
		AssemblerParser(Address base, const std::string& filepath)
			: Parser{ std::format("{} <inline assembly>", filepath) }, base{base}
		{
		}
	};
}

#endif
