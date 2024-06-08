#include "CompilerLinker.h"

#include "Utility.h"

namespace
{
	std::uint8_t make_opcode(hz::Opcode opcode, hz::Register operand1, hz::Register operand2)
	{
		return ((opcode & 0b1111) << 4) | ((operand1 & 0b11) << 2) | ((operand2 & 0b11) << 0);
	}

	std::vector<hz::Instruction*> gather(const std::vector<hz::Instruction*>& function)
	{
		std::vector<hz::Instruction*> instructions;

		for (auto i = 0; i < function.size(); i++)
		{
			const auto& instruction = function[i];

			if (!instruction->marked_for_deletion)
			{
				instructions.emplace_back(instruction);
			}
		}

		return instructions;
	}

	std::size_t _global_pass = 0;
}

namespace hz
{
	Linker::Type CompilerLinker::ltype() const
	{
		return Linker::Type::COMPILER;
	}

	bool CompilerLinker::optimize()
	{
		for (auto& [symbol, function, offset] : linkables)
		{
			for (auto i = 0; i < function.size(); i++)
			{
				for (auto r = R0; r <= R3; r = static_cast<Register>(r + 1))
				{
					//TODO: ensure none of our bytes are branch targets

					if (auto instructions = ::gather(function); !instructions.empty())
					{
						//Log::info(std::format("Instruction sequence prior to pass {}", _global_pass));
						//for (auto ii : instructions)
						//{
						//	std::cout << Disassembler::disassemble_instruction(ii.instruction->bytes()) << std::endl;
						//}
						//std::cout << "---------------------" << std::endl;

						for (auto j = 0; j < instructions.size(); j++)
						{
							auto o0 = instructions[j + 0];

							if (j + 1 < instructions.size())
							{
								auto o1 = instructions[j + 1];

								if ((o0->opcode == PUSH && o0->op2 == r &&  //push r
									 o1->opcode == PULL && o1->op1 == r) || //pull r

									(o0->opcode == PULL && o0->op1 == r &&  //pull r
									 o1->opcode == PUSH && o1->op2 == r))   //push r
								{
									//These instructions are entirely redundant
									o0->marked_for_deletion = true;
									o1->marked_for_deletion = true;
									_global_pass++;
									return true;
								}

								else if ((o0->opcode == SAVE && o0->op2 == r &&  //save &x, r
										  o1->opcode == LOAD && o1->op1 == r) || //load r, &x

										 (o0->opcode == LOAD && o0->op1 == r &&  //load r, &x
										  o1->opcode == SAVE && o1->op2 == r))   //save &x, r
								{
									if (o0->mem == o1->mem)
									{
										//These instructions are entirely redundant
										o0->marked_for_deletion = true;
										o1->marked_for_deletion = true;
										_global_pass++;
										return true;
									}
								}


								if (j + 2 < instructions.size())
								{
									auto o2 = instructions[j + 2];

									//TODO: 3-byte optimizations go here
								}
							}

							//No optimizations could be applied here
							//break;
						}
					}
				}
			}
		}

		return false;
	}

	std::vector<std::uint8_t> CompilerLinker::link()
	{
		std::vector<std::uint8_t> executable;

		auto address_tracker = 0;

		for (auto& [symbol, function, offset] : linkables)
		{
			if (symbol->was_referenced)
			{
				if constexpr (OPTIMIZE_LTO)
				{
					while (optimize());
				}

				offset = address_tracker;

				for (auto instruction : function)
				{
					if (!instruction->marked_for_deletion)
					{
						address_tracker += 3;
					}
				}
			}
		}

		for (auto& [symbol, function, offset] : linkables)
		{
			for (auto& linkable : linkables)
			{
				for (auto& instruction : linkable.object_code)
				{
					if (linkable.symbol->name != symbol->name &&
						instruction->opcode == CALL &&
						instruction->branch_target == symbol->name)
					{
						static constexpr auto BASE_ADDRESS = HALF_DWORD_MAX;
						const auto branch_target = BASE_ADDRESS + offset;

						instruction->mem = branch_target;
					}
				}
			}
		}

		for (auto& [symbol, function, offset] : linkables)
		{
			for (const auto& instruction : function)
			{
				if (!instruction->marked_for_deletion)
				{
					executable.append_range(extract(instruction->bytes()));
				}
			}
		}

		return executable;
	}
}