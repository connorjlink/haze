#include "AssemblerLinker.h"
#include "DotOrgCommand.h"
#include "LabelCommand.h"
#include "Parser.h"
#include "Constants.h"

// Haze AssemblerLinker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	LinkerType AssemblerLinker::ltype() const
	{
		return LinkerType::ASSEMBLER;
	}

	bool AssemblerLinker::optimize()
	{
		// choose not to do any optimizations for the assembler
		// if the user wants to do very unoptimized things, assembler is the place to let him/her do that :)
		return false;
	}

	std::vector<InstructionCommand*> AssemblerLinker::link(std::uint32_t base_pointer)
	{
		std::vector<InstructionCommand*> executable(DWORD_MAX);

		auto address_tracker = 0;

		commands.insert(commands.begin(), new DotOrgCommand{ base_pointer, NULL_TOKEN });

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
					AS_LABEL_SYMBOL(assembler_parser->reference_symbol(SymbolType::LABEL, label_command->identifier, NULL_TOKEN))->address = address_tracker;
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
							auto label_symbol = AS_LABEL_SYMBOL(assembler_parser->reference_symbol(SymbolType::LABEL, instruction_command->branch_target, NULL_TOKEN));

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
				_error_reporter->post_warning("possible data loss for code imaged prior to program entrypoint", NULL_TOKEN);
			}
		}


		return std::vector(executable.begin() + HALF_DWORD_MAX, executable.end());
	}
}