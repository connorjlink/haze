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
		using NodeHandle = CommandHandle;
		template<typename T>
		using NodeReference = CommandReference<T>;

	protected:
		Address base;
		CommandStorage command_storage;

	private:
		NodeReference<DotOrgCommand> parse_dotorg_command();
		NodeReference<DotByteCommand> parse_dotbyte_command();
		NodeReference<DotGlobalCommand> parse_dotglobal_command();
		NodeReference<LabelCommand> parse_label_command();
		NodeHandle parse_command();

	private:
		template<typename Self>
		NodeReference<InstructionCommand> parse_instruction_command(this Self&& self)
		{
			return self.parse_instruction_command_implementation();
		}

		template<typename Self>
		Register parse_register(this Self&& self)
		{
			return self.parse_register_implementation();
		}

	public:
		std::vector<NodeHandle> parse_implementation();

	public:
		AssemblerParser(const std::filesystem::path& filepath, const ExpressionStorage& storage, Address base)
			: Parser{ filepath, storage }, base{ base }
		{
		}
	};
}

#endif
