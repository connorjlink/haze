#include "Linker.h"
#include "Instruction.h"
#include "Utility.h"

namespace
{
	std::uint8_t make_opcode(hz::Opcode opcode, hz::Register operand1, hz::Register operand2)
	{
		return ((opcode & 0b1111) << 4) | ((operand1 & 0b11) << 2) | ((operand2 & 0b11) << 0);
	}
}

namespace hz
{
	std::vector<std::array<std::uint8_t, 3>> Linker::optimize(const std::vector<std::array<std::uint8_t, 3>>& object_code) const
	{
		std::vector<std::array<std::uint8_t, 3>> optimized_object_code{};

		for (auto i = 0; i < object_code.size(); i++)
		{
			for (auto r = R0; r <= R3; r = static_cast<Register>(r + 1))
			{
				//Ensure that there are two more bytes after (since we must test 3 bytes in total)
				if (i + 2 < object_code.size())
				{
					//First check the opcode sequence
					if (object_code[i + 0][0] == make_opcode(COPY, r, DC) &&
						object_code[i + 1][0] == make_opcode(SAVE, DC, r) &&
						object_code[i + 2][0] == make_opcode(LOAD, r, DC))
					{
						//Then check that the memory addresses are the same
						if (object_code[i + 1][1] == object_code[i + 2][1] &&
							object_code[i + 1][2] == object_code[i + 2][2])
						{
							optimized_object_code.emplace_back(object_code[i + 0]);
							//Do we actually need to add two or three here?
							i += 2;
							goto next_iteration;
						}

						optimized_object_code.emplace_back(object_code[i]);
					}
				}

				else
				{
					optimized_object_code.emplace_back(object_code[i]);
					break;
				}
			}
		next_iteration:
			{}
		}

		return optimized_object_code;
	}

	std::vector<std::uint8_t> Linker::link() const
	{
		std::vector<std::uint8_t> executable;

		for (auto [_, object_code] : code)
		{
			//This is probably not the best way of doing this since we copy
			//But, at least it looks clean
			if constexpr (OPTIMIZE)
			{
				object_code = optimize(object_code);
			}

			for (const auto& instruction : object_code)
			{
				executable.append_range(instruction);
			}
		}

		return executable;
	}
}
