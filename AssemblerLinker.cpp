#include "AssemblerLinker.h"

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

	std::vector<std::uint8_t> AssemblerLinker::link()
	{
		std::vector<std::uint8_t> executable;

		auto address_tracker = 0;

		for (auto command : commands)
		{
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
			}
		}

		for (auto command : commands)
		{
			//TODO: machine code emission here!
		}



		return executable;
	}
}