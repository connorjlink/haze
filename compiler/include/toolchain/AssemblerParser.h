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
		: public Parser<AssemblerParser>
		, public ServiceTag<AssemblerParser>
#pragma message("TODO: figure out how to get virtual dependencies sorted through the services (i.e., register the Parser and polymorphically initialize with AssemblerParser")
	{
	private:
		using NodeType = CommandHandle;

	private:
		Address base;

	public:
		virtual ArchitectureKind ctype() const = 0;;
		virtual CommandReference<InstructionCommand> parse_instruction_command() const = 0;
		virtual Register parse_register() const = 0;

		CommandHandle parse_dotdefine_command() const;

		std::vector<NodeType> parse() const
		{

		}

	public:
		AssemblerParser(Address base, std::filesystem::path filepath)
			: Parser{ std::format("{} <inline assembly>", std::move(filepath)) }, base{ base }
		{
		}
	};
}

#endif
