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
			auto result = std::vector<NodeType>{};

#pragma message("TODO: implement assembler parsing")

			return result;
		}

	public:
		AssemblerParser(std::vector<Token> tokens, const ExpressionStorage& storage, Address base)
			: Parser{ std::move(tokens), storage }, base{ base }
		{
		}
	};
}

#endif
