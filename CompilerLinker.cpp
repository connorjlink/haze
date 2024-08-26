#include "CompilerLinker.h"
#include "Command.h"
#include "InstructionCommand.h"

#include "Utility.h"
#include "Log.h"

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

			if (command->ctype() == CommandType::INSTRUCTION)
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

	static void internal_linker_error()
	{
		Log::error("Internal linker error: unrecognized command type");
	}

	static std::size_t _global_pass = 0;
}

namespace hz
{
	LinkerType CompilerLinker::ltype() const
	{
		return LinkerType::COMPILER;
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
							auto c0 = instructions[j + 0];

							if (c0->ctype() != CommandType::INSTRUCTION)
							{
								internal_linker_error();
							}

							auto i0 = AS_INSTRUCTION_COMMAND(c0);

							if (i0->opcode == MOVE &&
								i0->src == i0->dst)
							{
								i0->marked_for_deletion = true;
								_global_pass++;
								return true;
							}

							if (j + 1 < instructions.size())
							{
								auto c1 = instructions[j + 1];

								if (c1->ctype() != CommandType::INSTRUCTION)
								{
									internal_linker_error();
								}

								auto i1 = AS_INSTRUCTION_COMMAND(c1);

								if ((i0->opcode == PUSH && i0->src == r &&  //push r
									 i1->opcode == PULL && i1->dst == r) || //pull r

									(i0->opcode == PULL && i0->dst == r &&  //pull r
									 i1->opcode == PUSH && i1->src == r))   //push r
								{
									//These instructions are entirely redundant
									i0->marked_for_deletion = true;
									i1->marked_for_deletion = true;
									_global_pass++;
									return true;
								}

								else if ((i0->opcode == SAVE && i0->src == r &&  //save &x, r
										  i1->opcode == LOAD && i1->dst == r) || //load r, &x

										 (i0->opcode == LOAD && i0->dst == r &&  //load r, &x
										  i1->opcode == SAVE && i1->src == r))   //save &x, r
								{
									if (i0->mem == i1->mem)
									{
										//These instructions are entirely redundant
										i0->marked_for_deletion = true;
										i1->marked_for_deletion = true;
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

	std::vector<InstructionCommand*> CompilerLinker::link(std::uint16_t base_pointer)
	{
		std::vector<InstructionCommand*> executable{};

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

				for (auto command : function)
				{
					if (command->ctype() != CommandType::INSTRUCTION)
					{
						internal_linker_error();
					}

					auto instruction = AS_INSTRUCTION_COMMAND(command);

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
				for (auto command : linkable.object_code)
				{
					if (command->ctype() != CommandType::INSTRUCTION)
					{
						internal_linker_error();
					}

					auto instruction = AS_INSTRUCTION_COMMAND(command);

					if (linkable.symbol->name != symbol->name &&
					   (instruction->opcode == CALL || instruction->opcode == BRNZ) &&
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
				const auto& command = function[i];

				if (command->ctype() != CommandType::INSTRUCTION)
				{
					internal_linker_error();
				}

				auto instruction = AS_INSTRUCTION_COMMAND(command);

				if (!instruction->marked_for_deletion)
				{
					const auto embedded_size = instruction->approximate_embedded_size;

					if (embedded_size != 0)
					{
						for (auto j = i + 1; j < function.size(); j++)
						{
							function[j]->offset += embedded_size;
						}

						for (auto& embedded_instruction : instruction->embedded_object_code)
						{
							// NOTE: since we don't do any safety checks on inline assembly,
							// we could (or maybe even likely) are overwriting compiler-generated code.
							// Depending on our needs going forward, this might be very undesirable for debugging purposes.

							if (embedded_instruction != nullptr)
							{
								executable.emplace_back(embedded_instruction);
							}
						}
					}

					else // (embedded_size == 0)
					{
						executable.emplace_back(instruction);
					}
				}
			}
		}

		return executable;
	}
}