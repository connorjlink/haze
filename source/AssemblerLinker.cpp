import std;

#include <toolchain/AssemblerLinker.h>
#include <toolchain/AssemblerParser.h>
#include <toolchain/Parser.h>
#include <command/DotOrgCommand.h>
#include <command/LabelCommand.h>
#include <symbol/SymbolDatabase.h>
#include <utility/Constants.h>

// Haze AssemblerLinker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AssemblerLinker(std::vector<Node*>&& commands, AssemblerParser* assembler_parser, const std::string& filepath)
		: Linker{ filepath }, commands{ std::move(commands) }, assembler_parser{ assembler_parser }
	{
		commands.erase(std::remove_if(commands.begin(), commands.end(), [&](auto&& val)
			{
				if (val->ntype() != NodeType::COMMAND)
				{
					if (!_node_type_map.contains(val->ntype()))
					{
						USE_SAFE(ErrorReporter)->post_error("unrecognized node type", NULL_TOKEN);
					}
					else
					{
						USE_SAFE(ErrorReporter)->post_error(std::format("invalid node type `{}`", _node_type_map.at(val->ntype())), NULL_TOKEN);
					}

					return true;
				}

				return false;
			}));
	}

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

	std::vector<InstructionCommand*> AssemblerLinker::link(native_int base_pointer, native_int)
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
					USE_SAFE(SymbolDatabase)->reference_label(label_command->identifier, NULL_TOKEN)->address = address_tracker;
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
						if (instruction_command->opcode == Opcode::CALL ||
							instruction_command->opcode == Opcode::BRNZ)
						{
							auto label_symbol = USE_SAFE(SymbolDatabase)->reference_label(instruction_command->branch_target, NULL_TOKEN);

							//const auto branch_target = base_pointer + label_symbol->address;
							const auto branch_target = label_symbol->address;

							instruction_command->address = branch_target;
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
				USE_SAFE(ErrorReporter)->post_warning("possible data loss for code imaged prior to program entrypoint", NULL_TOKEN);
			}
		}

		return std::vector(executable.begin() + HALF_DWORD_MAX, executable.end());
	}
}