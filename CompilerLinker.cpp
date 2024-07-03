#include "CompilerLinker.h"
#include "Command.h"
#include "InstructionCommand.h"

#include "Utility.h"

namespace hz
{
	static std::uint8_t make_opcode(Opcode opcode, Register operand1, Register operand2)
	{
		return ((opcode & 0b1111) << 4) | ((operand1 & 0b11) << 2) | ((operand2 & 0b11) << 0);
	}

	static std::vector<Command*> gather(const std::vector<Command*>& function)
	{
		std::vector<Command*> commands;

		for (auto i = 0; i < function.size(); i++)
		{
			const auto& command = function[i];

			if (command->ctype() == Command::Type::INSTRUCTION)
			{
				const auto instruction_command = AS_INSTRUCTION_COMMAND(command);

				if (!instruction_command->marked_for_deletion)
				{
					commands.emplace_back(instruction_command);
				}
			}	
		}

		return commands;
	}

	static std::size_t _global_pass = 0;
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

					if (auto instructions = gather(function); !instructions.empty())
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

	std::vector<std::uint8_t> CompilerLinker::link(std::uint16_t base_pointer)
	{
		std::vector<std::uint8_t> executable(HALF_DWORD_MAX);

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
						(instruction->opcode == CALL || instruction->opcode == BREZ) &&
						instruction->branch_target == symbol->name)
					{
						const auto branch_target = base_pointer + offset;

						instruction->mem = branch_target;
					}
				}
			}
		}

		for (auto& [symbol, function, offset] : linkables)
		{
			for (auto i = 0; i < function.size(); i++)
			{
				const auto& instruction = function[i];

				if (!instruction->marked_for_deletion)
				{
					const auto embedded_size = instruction->approximate_embedded_size;

					if (embedded_size != 0)
					{
						for (auto j = i + 1; j < function.size(); j++)
						{
							function[j]->offset += embedded_size;
						}

						for (auto j = 0; j < executable.size(); j++)
						{
							// NOTE: since we don't do any safety checks on inline assembly,
							// we could (or maybe even likely) are overwriting compiler-generated code.
							// Depending on our needs going forward, this might be very undesirable for debugging purposes.
							executable[j] = instruction->embedded_object_code[j];
						}
					}

					else // (embedded_size == 0)
					{
						const auto bytes = extract(instruction->bytes());
						const auto base = instruction->offset;

						executable[base + 0] = bytes[0];
						executable[base + 1] = bytes[1];
						executable[base + 2] = bytes[2];
					}
				}
			}
		}

		return executable;
	}
}