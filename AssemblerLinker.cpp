#include "AssemblerLinker.h"
#include "DotOrgCommand.h"
#include "LabelCommand.h"

#include "Parser.h"

#include "Utility.h"
#include "Constants.h"

namespace hz
{
	Linker::Type AssemblerLinker::ltype() const
	{
		return Linker::Type::ASSEMBLER;
	}

	bool AssemblerLinker::optimize()
	{
		return false;
	}

	std::vector<std::uint8_t> AssemblerLinker::link(std::uint16_t base_pointer)
	{
		std::vector<std::uint8_t> executable(HALF_DWORD_MAX);

		auto address_tracker = 0;


		commands.insert(commands.begin(), new DotOrgCommand{ base_pointer });

		for (auto node : commands)
		{
			auto command = AS_COMMAND(node);

			command->offset = address_tracker;

			switch (command->ctype())
			{
				case Command::Type::INSTRUCTION:
				{
					address_tracker += 3;
				} break;

				case Command::Type::DOTORG:
				{
					address_tracker = AS_DOTORG_COMMAND(command)->address;
				} break;

				case Command::Type::LABEL:
				{
					auto label_command = AS_LABEL_COMMAND(command);

					label_command->offset = address_tracker; // TODO: this assignment might be unnecessary
					AS_LABEL_SYMBOL(assembler_parser->reference_symbol(Symbol::Type::LABEL, label_command->identifier))->address = address_tracker;
				} break;
			}
		}

		for (auto node : commands)
		{
			auto command = AS_COMMAND(node);

			switch (command->ctype())
			{
				case Command::Type::INSTRUCTION:
				{
					auto instruction_command = AS_INSTRUCTION_COMMAND(command);

					if (!instruction_command->marked_for_deletion)
					{
						if (instruction_command->opcode == CALL ||
							instruction_command->opcode == BREZ)
						{
							auto label_symbol = AS_LABEL_SYMBOL(assembler_parser->reference_symbol(Symbol::Type::LABEL, instruction_command->branch_target));

							//const auto branch_target = base_pointer + label_symbol->address;
							const auto branch_target = label_symbol->address;

							instruction_command->mem = branch_target;
						}

						const auto bytes = extract(instruction_command->bytes());
						const auto base = instruction_command->offset;

						executable[base + 0] = bytes[0];
						executable[base + 1] = bytes[1];
						executable[base + 2] = bytes[2];
					}
				} break;
				
				// TODO: do we need to worry about .org directives here as well??
			}
		}

		return executable;
	}
}