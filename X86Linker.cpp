import std;

#include "X86Linker.h"

// Haze X86Linker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	byterange X86Linker::link()
	{
		std::vector<IntermediateCommand*> ir_code{};
		std::vector<byterange> object_code{};

		// the first function starts at address 0
		auto address_tracker = 0;

		for (auto& linkable : _linkables)
		{
			linkable.offset = address_tracker;

			for (auto& command : linkable.ir)
			{
				const auto code = command->emit();

				command->offset = address_tracker;
				address_tracker += code.size();

				ir_code.emplace_back(command);
			}
		}

		for (auto& command : ir_code)
		{
			if (command->itype() == IntermediateType::CALL_FUNCTION)
			{
				auto call_function_command = AS_CALL_FUNCTION_COMMAND(command);

				call_function_command->emit();
			}
		}
	}
}
