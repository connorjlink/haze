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
		CommandHandle parse_dotdefine_command() const;

		std::vector<NodeType> parse() const
		{
			auto result = std::vector<NodeType>{};

#pragma message("TODO: implement assembler parsing")

			return result;
		}

	public:
		AssemblerParser(const std::filesystem::path& filepath, const ExpressionStorage& storage, Address base)
			: Parser{ filepath, storage }, base{ base }
		{
		}
	};
}

#endif
