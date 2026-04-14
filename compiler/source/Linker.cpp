import std;

#include <toolchain/Linker.h>

// Haze Linker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Linker::Linker()
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "linking");
	}

	Linker::~Linker()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}

	Linker::reload(const std::vector<Linkable&>& linkables)
	{
		_linkables = linkables;
	}

//	AssemblerLinker::AssemblerLinker(cosnt std::vector<Node*>& commands, AssemblerParser* assembler_parser, const std::string& filepath)
//		: Linker{ filepath }, commands{ commands }, assembler_parser{ assembler_parser }
//	{
//		commands.erase(std::remove_if(commands.begin(), commands.end(), [&](auto&& val)
//			{
//				if (val->ntype() != NodeType::COMMAND)
//				{
//					if (!_node_type_map.contains(val->ntype()))
//					{
//						USE_SAFE(ErrorReporter)->post_error("unrecognized node type", NULL_TOKEN);
//					}
//					else
//					{
//						USE_SAFE(ErrorReporter)->post_error(std::format("invalid node type `{}`", _node_type_map.at(val->ntype())), NULL_TOKEN);
//					}
//
//					return true;
//				}
//
//				return false;
//			}));
//	}
//
//	LinkerType AssemblerLinker::ltype() const
//	{
//		return LinkerType::ASSEMBLER;
//	}
//
//	bool AssemblerLinker::optimize()
//	{
//		// choose not to do any optimizations for the assembler
//		// if the user wants to do very unoptimized things, assembler is the place to let him/her do that :)
//		return false;
//	}
//
//	std::vector<InstructionCommand*> AssemblerLinker::link(Address base_pointer, Address size)
//	{
//		std::vector<InstructionCommand*> executable(size);
//
//		auto address_tracker = 0;
//
//		commands.insert(commands.begin(), new DotOrgCommand{ base_pointer, NULL_TOKEN });
//
//		std::erase_if(commands, [&](auto& command)
//			{
//				return command == nullptr;
//			});
//
//
//		for (auto node : commands)
//		{
//			auto command = AS_COMMAND(node);
//
//			command->offset = address_tracker;
//
//			switch (command->ctype())
//			{
//			case CommandType::INSTRUCTION:
//			{
//				address_tracker++;
//			} break;
//
//			case CommandType::DOTORG:
//			{
//				address_tracker = AS_DOTORG_COMMAND(command)->address;
//			} break;
//
//			case CommandType::LABEL:
//			{
//				auto label_command = AS_LABEL_COMMAND(command);
//
//				// TODO: this assignment might be unnecessary
//				label_command->offset = address_tracker;
//				USE_SAFE(SymbolDatabase)->reference_label(label_command->identifier, NULL_TOKEN)->address = address_tracker;
//			} break;
//			}
//		}
//
//		for (auto node : commands)
//		{
//			auto command = AS_COMMAND(node);
//
//			switch (command->ctype())
//			{
//			case CommandType::INSTRUCTION:
//			{
//				auto instruction_command = AS_INSTRUCTION_COMMAND(command);
//
//				if (!instruction_command->marked_for_deletion)
//				{
//					if (instruction_command->opcode == Opcode::CALL ||
//						instruction_command->opcode == Opcode::BRNZ)
//					{
//						auto label_symbol = USE_SAFE(SymbolDatabase)->reference_label(instruction_command->branch_target, NULL_TOKEN);
//
//						//const auto branch_target = base_pointer + label_symbol->address;
//						const auto branch_target = label_symbol->address;
//
//#pragma message("TODO: does this need to be adjusted for instruction length?")
//						instruction_command->relative = branch_target - instruction_command->offset;
//					}
//
//					executable[instruction_command->offset] = instruction_command;
//				}
//			} break;
//			}
//		}
//
//		for (auto i = Address{ 0 }; i < size / 2; i++)
//		{
//			if (executable[i] != 0)
//			{
//				USE_SAFE(ErrorReporter)->post_warning("possible data loss for code imaged prior to program entrypoint", NULL_TOKEN);
//			}
//		}
//
//#pragma message("TODO: figure out if this is the correct logic for pagination of the output binary. might not involve the base pointer here at all")
//		return std::vector(executable.begin() + base_pointer, executable.end());
//	}


//namespace
//{
//	using namespace hz;
//
//	std::vector<Command*> gather(const std::vector<Command*>& function)
//	{
//		std::vector<Command*> commands;
//
//		for (auto i = 0; i < function.size(); i++)
//		{
//			const auto& command = function[i];
//
//			if (command->ctype() == CommandType::INSTRUCTION)
//			{
//				const auto instruction_command = AS_INSTRUCTION_COMMAND(command);
//
//				if (!instruction_command->marked_for_deletion)
//				{
//					commands.emplace_back(instruction_command);
//				}
//			}
//		}
//
//		return commands;
//	}
//
//	void internal_linker_error()
//	{
//		USE_UNSAFE(ErrorReporter)->post_error("internal error caused by unrecognized command type", NULL_TOKEN);
//	}
//
//	std::size_t _global_pass = 0;
//}
//
//namespace hz
//{
//	LinkerType CompilerLinker::ltype() const
//	{
//		return LinkerType::COMPILER;
//	}
//
//	bool CompilerLinker::optimize()
//	{
//		for (auto& [name, linkable] : _linkables)
//		{
//			auto& [symbol, function, ir, offset] = linkable;
//
//			for (auto i = 0; i < function.size(); i++)
//			{
//				const auto architecture = USE_SAFE(CommandLineOptions)->architecture;
//				const auto extrema = get_register_extrema(architecture);
//
//				for (auto r = extrema.first; r <= extrema.second; r++)
//				{
//#pragma message("TODO: ensure none of our bytes are branch targets")
//
//					if (auto instructions = gather(function); !instructions.empty())
//					{
//						for (auto j = 0; j < instructions.size(); j++)
//						{
//							auto c0 = instructions[j + 0];
//
//							if (c0->ctype() != CommandType::INSTRUCTION)
//							{
//								internal_linker_error();
//							}
//
//							auto i0 = AS_INSTRUCTION_COMMAND(c0);
//
//							if (i0->opcode == Opcode::MOVE &&
//								i0->source == i0->destination)
//							{
//								i0->marked_for_deletion = true;
//								_global_pass++;
//								return true;
//							}
//
//							if (j + 1 < instructions.size())
//							{
//								auto c1 = instructions[j + 1];
//
//								if (c1->ctype() != CommandType::INSTRUCTION)
//								{
//									internal_linker_error();
//								}
//
//								auto i1 = AS_INSTRUCTION_COMMAND(c1);
//
//								if ((i0->opcode == Opcode::PUSH && i0->source == r &&  //push r
//									i1->opcode == Opcode::PULL && i1->destination == r) || //pull r
//
//									(i0->opcode == Opcode::PULL && i0->destination == r &&  //pull r
//										i1->opcode == Opcode::PUSH && i1->source == r))   //push r
//								{
//									// these instructions are entirely redundant
//									i0->marked_for_deletion = true;
//									i1->marked_for_deletion = true;
//									_global_pass++;
//									return true;
//								}
//
//								else if ((i0->opcode == Opcode::SAVE && i0->source == r &&  //save &x, r
//									i1->opcode == Opcode::LOAD && i1->destination == r) || //load r, &x
//
//									(i0->opcode == Opcode::LOAD && i0->destination == r &&  //load r, &x
//										i1->opcode == Opcode::SAVE && i1->source == r))   //save &x, r
//								{
//									if (i0->absolute == i1->absolute)
//									{
//										// these instructions are entirely redundant
//										i0->marked_for_deletion = true;
//										i1->marked_for_deletion = true;
//										_global_pass++;
//										return true;
//									}
//								}
//
//
//								if (j + 2 < instructions.size())
//								{
//									auto o2 = instructions[j + 2];
//
//#pragma message("TODO: 3-byte optimizations go here")
//								}
//							}
//
//							// no optimizations could be applied here
//							//break;
//						}
//					}
//				}
//			}
//		}
//
//		return false;
//	}
//
//	// intentionally ignoring the size parameter since the compiler can just take as required and generate the EXE appropriately
//	std::vector<InstructionCommand*> CompilerLinker::link(Address base_pointer, Address)
//	{
//		std::vector<InstructionCommand*> executable{};
//
//		Address address_tracker = 0;
//
//		// resolve the length of each instruction to compute each label's address
//		for (auto& [name, linkable] : _linkables)
//		{
//
//			auto& [symbol, function, ir, offset] = linkable;
//
//			if (symbol->was_referenced)
//			{
//				if (USE_UNSAFE(CommandLineOptions)->optimization & OptimizationType::LTO)
//				{
//					while (optimize())
//						;
//				}
//
//				offset = address_tracker;
//
//				for (auto command : function)
//				{
//					using enum CommandType;
//					switch (command->ctype())
//					{
//					case LABEL:
//					{
//						auto label = AS_LABEL_COMMAND(command);
//						label->offset = address_tracker;
//					} break;
//
//					case INSTRUCTION:
//					{
//						auto instruction = AS_INSTRUCTION_COMMAND(command);
//
//						if (!instruction->marked_for_deletion)
//						{
//							instruction->offset = address_tracker;
//							// previously this was always 3 (since haze instructions are 24 bits)
//							address_tracker += static_cast<Address>(instruction->length());
//						}
//					} break;
//
//					default:
//					{
//						internal_linker_error();
//					} break;
//					}
//				}
//			}
//		}
//
//		for (auto& [name, linkable] : _linkables)
//		{
//			auto& [symbol, function, ir, offset] = linkable;
//
//			// reset the address tracker to be used again
//			address_tracker = 0;
//
//			for (auto& [_, linkable] : _linkables)
//			{
//				for (auto command : linkable.commands)
//				{
//					using enum CommandType;
//					switch (command->ctype())
//					{
//					case LABEL:
//					{
//						auto label = AS_LABEL_COMMAND(command);
//
//						for (auto& [_, patching_linkable] : _linkables)
//						{
//							for (auto patching_command : patching_linkable.commands)
//							{
//								if (patching_command->ctype() == INSTRUCTION)
//								{
//									auto patching_instruction = AS_INSTRUCTION_COMMAND(patching_command);
//
//									if (!patching_instruction->marked_for_deletion)
//									{
//										if (patching_instruction->opcode == Opcode::BRNZ)
//										{
//											if (patching_instruction->branch_target == label->identifier)
//											{
//												// compute the absolute address of the jump
//												const auto branch_target = label->offset;
//
//												// compute the absolute address of the current instruction start
//												const auto branch_start = patching_instruction->offset;
//
//												// compute the relative address
//												const auto distance = branch_target - branch_start - resolve_instruction_length(patching_instruction);
//
//												patching_instruction->absolute = distance;
//
//												// NOTE: absolute addressing only works for brnz
//												//const auto branch_target = base_pointer + label->offset;
//												//patching_instruction->mem = branch_target;
//											}
//										}
//									}
//								}
//							}
//						}
//					} break;
//
//					case INSTRUCTION:
//					{
//						auto instruction = AS_INSTRUCTION_COMMAND(command);
//
//						if (!instruction->marked_for_deletion)
//						{
//							auto length = resolve_instruction_length(instruction);
//
//							// NOTE: `linkable.symbol->name != symbol->name &&`
//							// previously was part of this condition. Why?
//							// Should be able to call the current function recursively...
//							if (instruction->branch_target == symbol->name)
//							{
//								if (instruction->opcode == Opcode::CALL)
//								{
//									// compute the absolute address of the jump
//									const auto branch_target = offset;
//
//									// compute the absolute address of the current instruction start
//									const auto branch_start = address_tracker;
//
//									// compute the relative address
//									const auto distance = branch_target - branch_start - length;
//
//									// NOTE: call only works with relative addressing
//									//const auto call_target = branch_target - address_tracker + length + length - base_pointer;
//									instruction->absolute = distance;
//								}
//
//							}
//
//							// previously this was always 3 (since haze instructions are 24 bits)
//							address_tracker += length;
//						}
//					} break;
//
//					default:
//					{
//						internal_linker_error();
//					} break;
//					}
//				}
//			}
//		}
//
//		for (auto& [name, linkable] : _linkables)
//		{
//			auto& [symbol, function, ir, offset] = linkable;
//
//			for (auto i = 0uz; i < function.size(); i++)
//			{
//				const auto& command = function[i];
//
//				using enum CommandType;
//				switch (command->ctype())
//				{
//				case LABEL:
//				{
//
//				} break;
//
//				case INSTRUCTION:
//				{
//					auto instruction = AS_INSTRUCTION_COMMAND(command);
//
//					if (!instruction->marked_for_deletion)
//					{
//						for (auto& embedded_instruction : instruction->object_code)
//						{
//							// NOTE: since we don't do any safety checks on inline assembly,
//							// we could (or maybe even likely) are overwriting compiler-generated code.
//							// Depending on our needs going forward, this might be very undesirable for debugging purposes.
//
//							if (embedded_instruction != nullptr)
//							{
//								executable.emplace_back(embedded_instruction);
//							}
//						}
//					}
//				} break;
//
//				default:
//				{
//					internal_linker_error();
//				} break;
//				}
//			}
//		}
//
//		return executable;
//	}
//
//	ByteRange link()
//	{
//		std::vector<IntermediateCommand*> ir_code{};
//		std::vector<ByteRange> object_code{};
//		ByteRange final_image{};
//
//		// map of (label name -> address)
//		std::unordered_map<std::string, Offset> branches;
//
//		// the first function starts at address 0
//		Offset address_tracker = 0;
//
//		for (auto& linkable : _linkables)
//		{
//			linkable.offset = address_tracker;
//
//			for (auto command : linkable.ir)
//			{
//				using enum IntermediateType;
//				switch (command->itype())
//				{
//				case BRANCH_LABEL:
//				{
//					auto branch_label_command = AS_BRANCH_LABEL_COMMAND(command);
//					branches[branch_label_command->label] = address_tracker;
//				} break;
//
//				default:
//				{
//					// NOTE: branches have not yet been resolved
//					// so, just generate the instruction with dummy placeholders
//
//					// The binary is wrong, but is at least of the correct 
//					// length for future label/target address resolution
//					const auto code = command->emit();
//					const auto size = static_cast<Offset>(code.size());
//
//					command->offset = address_tracker;
//					command->size = size;
//					address_tracker += size;
//
//					ir_code.emplace_back(command);
//				} break;
//				}
//			}
//		}
//
//		for (auto command : ir_code)
//		{
//#pragma message("TODO: support branch commands that go directly to an index instead of only by label!")
//
//			if (command->itype() == IntermediateType::BRANCH)
//			{
//				auto branch_command = AS_BRANCH_COMMAND(command);
//
//				auto absolute_target_offset = branch_command->target_offset;
//
//				// does the address need to be resolved by label?
//				if (!absolute_target_offset.has_value())
//				{
//					// at this point it is unknown if the target label is a function or instruction label
//
//					const auto target_linkable = std::find_if(_linkables.begin(), _linkables.end(), [&](auto&& linkable)
//						{
//							return linkable.symbol->name == branch_command->label;
//						});
//
//					// if the label was found, it is a function
//					if (target_linkable != _linkables.end())
//					{
//						absolute_target_offset = target_linkable->offset;
//					}
//
//					// otherwise, it must be a specific instruction-level label
//					else
//					{
//						absolute_target_offset = branches[branch_command->label];
//					}
//				}
//
//				const auto current_position = command->offset;
//				const auto current_size = command->size;
//
//				const auto next_instruction_offset = current_position + current_size;
//
//				const auto relative_difference = absolute_target_offset.value() - next_instruction_offset;
//				branch_command->target_offset = relative_difference;
//			}
//
//			const auto code = command->emit();
//			object_code.emplace_back(code);
//		}
//
//		for (auto& instruction : object_code)
//		{
//			final_image.append_range(instruction);
//		}
//
//		return final_image;
//	}
//}
}
