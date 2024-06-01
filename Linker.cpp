#include "Linker.h"
#include "Utility.h"
#include "Symbol.h"

namespace
{
	std::uint8_t make_opcode(hz::Opcode opcode, hz::Register operand1, hz::Register operand2)
	{
		return ((opcode & 0b1111) << 4) | ((operand1 & 0b11) << 2) | ((operand2 & 0b11) << 0);
	}

	std::vector<hz::IndexedInstruction> gather(const std::vector<hz::IndexedInstruction>& function)
	{
		std::vector<hz::IndexedInstruction> instructions;

		for (auto i = 0; i < function.size(); i++)
		{
			const auto& ii = function[i];

			if (!ii.instruction->marked_for_deletion)
			{
				instructions.emplace_back(ii);
			}
		}

		return instructions;
	}
}

namespace hz
{
	bool Linker::optimize()
	{
		for (auto&[symbol, function, offset] : linkables)
		{
			for (auto i = 0; i < function.size(); i++)
			{
				for (auto r = R0; r <= R3; r = static_cast<Register>(r + 1))
				{
					//TODO: ensure none of our bytes are branch targets

					if (auto instructions = ::gather(function); !instructions.empty())
					{
						for (auto j = 0; j < instructions.size(); j++)
						{
							auto o0 = instructions[i + 0];

							if (i + 1 < instructions.size())
							{
								auto o1 = instructions[i + 1];

								if ((o0.instruction->opcode == PUSH && o0.instruction->op2 == r &&  //push r
									 o1.instruction->opcode == PULL && o1.instruction->op1 == r) || //pull r
									
									(o0.instruction->opcode == PULL && o0.instruction->op1 == r &&  //pull r
									 o1.instruction->opcode == PUSH && o1.instruction->op2 == r))   //push r
								{
									//These instructions are entirely redundant
									o0.instruction->marked_for_deletion = true;
									o1.instruction->marked_for_deletion = true;
									return true;
								}

								else if ((o0.instruction->opcode == SAVE && o0.instruction->op2 == r &&
										  o1.instruction->opcode == LOAD && o1.instruction->op1 == r) ||
										
										 (o0.instruction->opcode == LOAD && o0.instruction->op1 == r &&
										  o1.instruction->opcode == SAVE && o1.instruction->op2 == r))
								{
									if (o0.instruction->mem == o1.instruction->mem)
									{
										o0.instruction->marked_for_deletion = true;
										o1.instruction->marked_for_deletion = true;
										return true;
									}
								}


								if (i + 2 < instructions.size())
								{
									auto o2 = instructions[i + 2];

									//TODO: 3-byte optimizations go here
								}
							}

							//No optimizations could be applied here
							return false;	
						}
					}
				}
			}
		}

		return false;
	}

	std::vector<std::uint8_t> Linker::link()
	{
		std::vector<std::uint8_t> executable;

		auto address_tracker = 0;

		for (auto&[symbol, function, offset] : linkables)
		{
			if (symbol->was_referenced)
			{
				if constexpr (OPTIMIZE_LTO)
				{
					while (optimize());
				}

				offset = address_tracker;

				for (auto ii : function)
				{
					if (!ii.instruction->marked_for_deletion)
					{
						address_tracker += 3;
					}
				}
			}
		}

		for (auto&[symbol, function, offset] : linkables)
		{
			for (auto& linkable : linkables)
			{
				for (auto& ii : linkable.object_code)
				{
					if (linkable.symbol->name != symbol->name &&
						ii.instruction->opcode == CALL &&
						ii.instruction->branch_target == symbol->name)
					{
						static constexpr auto BASE_ADDRESS = HALF_DWORD_MAX;
						const auto branch_target = BASE_ADDRESS + offset;

						ii.instruction->mem = branch_target;
					}
				}
			}
		}

		for (auto&[symbol, function, offset] : linkables)
		{
			for (const auto& ii : function)
			{
				if (!ii.instruction->marked_for_deletion)
				{
					executable.append_range(extract(ii.instruction->bytes()));
				}
			}	
		}

		return executable;
	}
}
