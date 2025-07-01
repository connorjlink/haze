import std;

#include "CompilerLinker.h"
#include "Command.h"
#include "InstructionCommand.h"
#include "LabelCommand.h"
#include "Emitter.h"
#include "CommandLineOptions.h"
#include "Symbol.h"
#include "ErrorReporter.h"
#include "DependencyInjector.h"

// Haze CompilerLinker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace 
{
	using namespace hz;

	std::vector<Command*> gather(const std::vector<Command*>& function)
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

	void internal_linker_error()
	{
		USE_UNSAFE(ErrorReporter)->post_error("internal error caused by unrecognized command type", NULL_TOKEN);
	}


	std::uint32_t resolve_instruction_length(InstructionCommand* instruction)
	{
		std::size_t length = 0;
		
		using enum Opcode;
		switch (instruction->opcode)
		{
			case MOVE: length = REQUIRE_UNSAFE(Emitter)->emit_move(instruction->destination, instruction->source).size(); break;
			case LOAD: length = REQUIRE_UNSAFE(Emitter)->emit_load(instruction->destination, instruction->address).size(); break;
			case COPY: length = REQUIRE_UNSAFE(Emitter)->emit_copy(instruction->destination, instruction->immediate).size(); break;
			case SAVE: length = REQUIRE_UNSAFE(Emitter)->emit_save(instruction->address, instruction->source).size(); break;
			case IADD: length = REQUIRE_UNSAFE(Emitter)->emit_iadd(instruction->destination, instruction->source).size(); break;
			case ISUB: length = REQUIRE_UNSAFE(Emitter)->emit_isub(instruction->destination, instruction->source).size(); break;
			case BAND: length = REQUIRE_UNSAFE(Emitter)->emit_band(instruction->destination, instruction->source).size(); break;
			case BIOR: length = REQUIRE_UNSAFE(Emitter)->emit_bior(instruction->destination, instruction->source).size(); break;
			case BXOR: length = REQUIRE_UNSAFE(Emitter)->emit_bior(instruction->destination, instruction->source).size(); break;
			case CALL: length = REQUIRE_UNSAFE(Emitter)->emit_call(instruction->address).size(); break;
			case EXIT: length = REQUIRE_UNSAFE(Emitter)->emit_exit().size(); break;
			case PUSH: length = REQUIRE_UNSAFE(Emitter)->emit_push(instruction->source).size(); break;
			case PULL: length = REQUIRE_UNSAFE(Emitter)->emit_pull(instruction->destination).size(); break;
			case BRNZ: length = REQUIRE_UNSAFE(Emitter)->emit_brnz(instruction->address, instruction->source).size(); break;
			case BOOL: length = REQUIRE_UNSAFE(Emitter)->emit_bool(instruction->source).size(); break;
			case STOP: length = REQUIRE_UNSAFE(Emitter)->emit_stop().size(); break;
		}

		// explicit narrowing conversion to avoid nuisance warning
		return static_cast<std::uint32_t>(length);
	}



	std::size_t _global_pass = 0;
}

namespace hz
{
	LinkerType CompilerLinker::ltype() const
	{
		return LinkerType::COMPILER;
	}

	bool CompilerLinker::optimize()
	{
		for (auto& [symbol, function, ir, offset] : linkables)
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

							if (i0->opcode == Opcode::MOVE &&
								i0->source == i0->destination)
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

								if ((i0->opcode == Opcode::PUSH && i0->source == r &&  //push r
									 i1->opcode == Opcode::PULL && i1->destination == r) || //pull r

									(i0->opcode == Opcode::PULL && i0->destination == r &&  //pull r
									 i1->opcode == Opcode::PUSH && i1->source == r))   //push r
								{
									//These instructions are entirely redundant
									i0->marked_for_deletion = true;
									i1->marked_for_deletion = true;
									_global_pass++;
									return true;
								}

								else if ((i0->opcode == Opcode::SAVE && i0->source == r &&  //save &x, r
										  i1->opcode == Opcode::LOAD && i1->destination == r) || //load r, &x

										 (i0->opcode == Opcode::LOAD && i0->destination == r &&  //load r, &x
										  i1->opcode == Opcode::SAVE && i1->source == r))   //save &x, r
								{
									if (i0->address == i1->address)
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

	std::vector<InstructionCommand*> CompilerLinker::link(std::uint32_t base_pointer)
	{
		std::vector<InstructionCommand*> executable{};

		std::uint32_t address_tracker = 0;

		// resolve the length of each instruction to compute each label's address
		for (auto& [symbol, function, ir, offset] : linkables)
		{
			if (symbol->was_referenced)
			{
				if (USE_UNSAFE(CommandLineOptions)->_optimization & OptimizationType::LTO)
				{
					while (optimize());
				}

				offset = address_tracker;

				for (auto command : function)
				{
					using enum CommandType;
					switch (command->ctype())
					{
						case LABEL:
						{
							auto label = AS_LABEL_COMMAND(command);
							label->offset = address_tracker;
						} break;

						case INSTRUCTION:
						{
							auto instruction = AS_INSTRUCTION_COMMAND(command);

							if (!instruction->marked_for_deletion)
							{
								auto length = resolve_instruction_length(instruction);

								instruction->offset = address_tracker;

								// previously this was always 3 (since haze instructions are 24 bits)
								address_tracker += length;
							}
						} break;

						default:
						{
							internal_linker_error();
						} break;
					}
				}
			}
		}

		for (auto& [symbol, function, ir, offset] : linkables)
		{
			// reset the address tracker to be used again
			address_tracker = 0;

			for (auto& linkable : linkables)
			{
				for (auto command : linkable.commands)
				{
					using enum CommandType;
					switch (command->ctype())
					{
						case LABEL:
						{
							auto label = AS_LABEL_COMMAND(command);

							for (auto& patching_linkable : linkables)
							{
								for (auto patching_command : patching_linkable.commands)
								{
									if (patching_command->ctype() == INSTRUCTION)
									{
										auto patching_instruction = AS_INSTRUCTION_COMMAND(patching_command);

										if (!patching_instruction->marked_for_deletion)
										{
											if (patching_instruction->opcode == Opcode::BRNZ)
											{
												if (patching_instruction->branch_target == label->identifier)
												{
													// compute the absolute address of the jump
													const auto branch_target = label->offset;

													// compute the absolute address of the current instruction start
													const auto branch_start = patching_instruction->offset;

													// compute the relative address
													const auto distance = branch_target - branch_start - resolve_instruction_length(patching_instruction);

													patching_instruction->address = distance;

													// NOTE: absolute addressing only works for brnz
													//const auto branch_target = base_pointer + label->offset;
													//patching_instruction->mem = branch_target;
												}
											}
										}
									}
								}
							}
						} break;

						case INSTRUCTION:
						{
							auto instruction = AS_INSTRUCTION_COMMAND(command);

							if (!instruction->marked_for_deletion)
							{
								auto length = resolve_instruction_length(instruction);

								// NOTE: `linkable.symbol->name != symbol->name &&`
								// previously was part of this condition. Why?
								// Should be able to call the current function recursively...
								if (instruction->branch_target == symbol->name)
								{
									if (instruction->opcode == Opcode::CALL)
									{
										// compute the absolute address of the jump
										const auto branch_target = offset;

										// compute the absolute address of the current instruction start
										const auto branch_start = address_tracker;

										// compute the relative address
										const auto distance = branch_target - branch_start - length;

										// NOTE: call only works with relative addressing
										//const auto call_target = branch_target - address_tracker + length + length - base_pointer;
										instruction->address = distance;
									}

								}

								// previously this was always 3 (since haze instructions are 24 bits)
								address_tracker += length;
							}
						} break;

						default:
						{
							internal_linker_error();
						} break;
					}
				}
			}
		}

		for (auto& [symbol, function, ir, offset] : linkables)
		{
			for (auto i = 0; i < function.size(); i++)
			{
				const auto& command = function[i];

				using enum CommandType;
				switch (command->ctype())
				{
					case LABEL:
					{

					} break;

					case INSTRUCTION:
					{
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
					} break;

					default:
					{
						internal_linker_error();
					} break;
				}
			}
		}

		return executable;
	}
}