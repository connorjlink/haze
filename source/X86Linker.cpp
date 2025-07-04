import std;

#include <command/IntermediateCommand.h>
#include <symbol/Symbol.h>
#include <toolchain/X86Linker.h>

// Haze X86Linker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	byterange X86Linker::link()
	{
		std::vector<IntermediateCommand*> ir_code{};
		std::vector<byterange> object_code{};
		byterange final_image{};

		// map of (label name -> address)
		std::unordered_map<std::string, std::int32_t> branches;

		// the first function starts at address 0
		std::int32_t address_tracker = 0;

		for (auto& linkable : _linkables)
		{
			linkable.offset = address_tracker;

			for (auto command : linkable.ir)
			{
				using enum IntermediateType;
				switch (command->itype())
				{
					case BRANCH_LABEL:
					{
						auto branch_label_command = AS_BRANCH_LABEL_COMMAND(command);
						branches[branch_label_command->label] = address_tracker;
					} break;

					default:
					{
						// NOTE: branches have not yet been resolved
						// so, just generate the instruction with dummy placeholders

						// The binary is wrong, but is at least of the correct 
						// length for future label/target address resolution
						const auto code = command->emit();
						const auto size = static_cast<std::int32_t>(code.size());

						command->offset = address_tracker;
						command->size = size;
						address_tracker += size;

						ir_code.emplace_back(command);
					} break;
				}
			}
		}

		for (auto& command : ir_code)
		{
#pragma message("TODO: support branch commands that go directly to an index instead of only by label!")

			if (command->itype() == IntermediateType::BRANCH)
			{
				auto branch_command = AS_BRANCH_COMMAND(command);

				auto absolute_target_offset = branch_command->target_offset;

				// does the address need to be resolved by label?
				if (!absolute_target_offset.has_value())
				{
					// at this point it is unknown if the target label is a function or instruction label
					
					const auto target_linkable = std::find_if(_linkables.begin(), _linkables.end(), [&](auto&& linkable)
					{
						return linkable.symbol->name == branch_command->label;
					});

					// if the label was found, it is a function
					if (target_linkable != _linkables.end())
					{
						absolute_target_offset = target_linkable->offset;
					}

					// otherwise, it must be a specific instruction-level label
					else
					{
						absolute_target_offset = branches[branch_command->label];
					}
				}

				const auto current_position = command->offset;
				const auto current_size = command->size;

				const auto next_instruction_offset = current_position + current_size;

				const auto relative_difference = absolute_target_offset.value() - next_instruction_offset;
				branch_command->target_offset = relative_difference;
			}

			const auto code = command->emit();
			object_code.emplace_back(code);
		}

		for (auto& instruction : object_code)
		{
			final_image.append_range(instruction);
		}

		return final_image;
	}
}
