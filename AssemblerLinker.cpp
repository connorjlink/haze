#include "AssemblerLinker.h"
#include "DotOrgCommand.h"
#include "LabelCommand.h"

#include "Parser.h"

#include "Utility.h"
#include "Constants.h"

namespace hz
{
	LinkerType AssemblerLinker::ltype() const
	{
		return LinkerType::ASSEMBLER;
	}

	bool AssemblerLinker::optimize()
	{
		return false;
	}

	std::vector<InstructionCommand*> AssemblerLinker::link(std::uint16_t base_pointer)
	{
		std::vector<InstructionCommand*> executable(DWORD_MAX);

		auto address_tracker = 0;

		commands.insert(commands.begin(), new DotOrgCommand{ base_pointer });

		std::erase_if(commands, [&](auto& command)
		{
			return command == nullptr;
		});


		for (auto node : commands)
		{
			auto command = AS_COMMAND(node);

			command->offset = address_tracker;

			switch (command->ctype())
			{
				case CommandType::INSTRUCTION:
				{
					address_tracker++;
				} break;

				case CommandType::DOTORG:
				{
					address_tracker = AS_DOTORG_COMMAND(command)->address;
				} break;

				case CommandType::LABEL:
				{
					auto label_command = AS_LABEL_COMMAND(command);

					label_command->offset = address_tracker; // TODO: this assignment might be unnecessary
					AS_LABEL_SYMBOL(assembler_parser->reference_symbol(SymbolType::LABEL, label_command->identifier))->address = address_tracker;
				} break;
			}
		}

		for (auto node : commands)
		{
			auto command = AS_COMMAND(node);

			switch (command->ctype())
			{
				case CommandType::INSTRUCTION:
				{
					auto instruction_command = AS_INSTRUCTION_COMMAND(command);

					if (!instruction_command->marked_for_deletion)
					{
						if (instruction_command->opcode == CALL ||
							instruction_command->opcode == BRNZ)
						{
							auto label_symbol = AS_LABEL_SYMBOL(assembler_parser->reference_symbol(SymbolType::LABEL, instruction_command->branch_target));

							//const auto branch_target = base_pointer + label_symbol->address;
							const auto branch_target = label_symbol->address;

							instruction_command->mem = branch_target;
						}

						executable[instruction_command->offset] = instruction_command;
					}
				} break;
				
				// TODO: do we need to worry about .org directives here as well??
			}
		}

		for (auto i = 0; i < HALF_DWORD_MAX; i++)
		{
			if (executable[i] != 0)
			{
				Log::error("Detected data loss for code imaged before $8000");
			}
		}


		return std::vector(executable.begin() + HALF_DWORD_MAX, executable.end());
	}
}