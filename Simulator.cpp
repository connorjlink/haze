#include "Simulator.h"
#include "InstructionCommand.h"
#include "Disassembler.h"
#include "ErrorReporter.h"
#include "Log.h"
#include "Token.h"

#include <iostream>
#include <format>


namespace hz
{
	bool Simulator::step()
	{
		instruction_pointer &= 0xFFFF;

		const auto fetch1 = rom[instruction_pointer - HALF_DWORD_MAX + 0],
				   fetch2 = rom[instruction_pointer - HALF_DWORD_MAX + 1],
				   fetch3 = rom[instruction_pointer - HALF_DWORD_MAX + 2];

		opcode = (fetch1 & 0xF0) >> 4;

		operand1 = (fetch1 & 0x0C) >> 2;
		operand2 = (fetch1 & 0x03) >> 0;

		immediate = fetch2;
		memory = (fetch2 << 8) | fetch3;

		//TODO: fix one or more of these instructions: call, exit, 

		switch (opcode & 0x0F)
		{
			case MOVE:
			{
				//move reg, reg
				register_file[operand1] = register_file[operand2];
			} break;

			case LOAD:
			{
				//load reg, mem
				register_file[operand1] = ram[memory];
			} break;

			case COPY:
			{
				//copy reg, imm
				register_file[operand1] = static_cast<decltype(register_file)::value_type>(immediate);
			} break;

			case SAVE:
			{
				//save mem, reg
				ram[memory] = register_file[operand2];
				if (memory == 0x1111)
				{
					_error_reporter->post_information(std::format("program output character `{}`", ram[memory]), NULL_TOKEN);
				}
			} break;

			case IADD:
			{
				//iadd reg, reg
				register_file[operand1] += register_file[operand2];
			} break;

			case ISUB:
			{
				//isub reg, reg
				register_file[operand1] -= register_file[operand2];
			} break;

			case BAND:
			{
				//band reg, reg
				register_file[operand1] &= register_file[operand2];
			} break;

			case BIOR:
			{
				//bior reg, reg
				register_file[operand1] |= register_file[operand2];
			} break;

			case BXOR:
			{
				//bxor reg, reg
				register_file[operand1] ^= register_file[operand2];
			} break;

			case CALL:
			{
				//call mem
				ram[CODE_STACK - code_stack_pointer] = (instruction_pointer & 0xFF00) >> 8;
				code_stack_pointer++;
				ram[CODE_STACK - code_stack_pointer] = (instruction_pointer & 0x00FF) >> 0;
				code_stack_pointer++;
				instruction_pointer = static_cast<decltype(instruction_pointer)>(memory);
				return true;
			} break;

			case EXIT:
			{
				//exit
				code_stack_pointer--;
				const auto lower = ram[CODE_STACK - code_stack_pointer];
				code_stack_pointer--;
				const auto upper = ram[CODE_STACK - code_stack_pointer];
				instruction_pointer = (upper << 8) | lower;
			} break;

			case PUSH:
			{
				//push reg
				if (data_stack_pointer <= DATA_STACK)
				{
					ram[DATA_STACK - data_stack_pointer] = register_file[operand2];
					data_stack_pointer++;
				}
			} break;

			case PULL:
			{
				//pull reg
				if (data_stack_pointer <= DATA_STACK)
				{
					data_stack_pointer--;
					register_file[operand1] = ram[DATA_STACK - data_stack_pointer];
				}
			} break;

			case BRNZ:
			{
				//brnz mem, reg
				if (register_file[operand2] != 0)
				{
					instruction_pointer = static_cast<decltype(instruction_pointer)>(memory);
					return true;
				}
			} break;

			case BOOL:
			{
				//bool reg
				register_file[operand2] = !(static_cast<bool>(register_file[operand2]));
			} break;

			default:
			{
				Log::output(std::format("break instruction encountered at address ${:04X}", instruction_pointer));
				running = false;
				return false;
			} break;
		}

		instruction_pointer += 3;
		return true;
	}

	void Simulator::reset()
	{
		register_file = { 0 };
		instruction_pointer = HALF_DWORD_MAX;
		data_stack_pointer = 0;
		code_stack_pointer = 0;

		ram = { 0 };
	}

	void Simulator::run()
	{
		Log::info("Simulation loading...");
		Log::info("Simulation ready!");

		Log::output("Press 'return' to start the simulation:");

		std::cin.get();

		Log::info("Simulation starting...");
		Log::info("Input 's' to step forward one instruction or 'd' to display the disassembly");

		bool freerunning = false;
		for (char option = '\0'; running; )
		{
			if (!freerunning)
			{
				std::cin >> option;
			}

			switch (option)
			{
				case 'd':
				{
					Log::info("Instruction listing:");
					const auto offset = instruction_pointer - HALF_DWORD_MAX;
					//Log::raw(Disassembler::disassemble_program(rom, offset - std::min(3, offset), rom.size()));
					Log::raw(Disassembler::disassemble_program(rom, offset - std::min(3u, offset), rom.size() / 4));
				} break;

				case 's':
				{
					freerunning = true;
					if (step())
					{
						Log::info(std::format("CPU State: registers=[{}, {}, {}, {}] ip=${:X} ds={:0X} ({}) cs={:0X} ({}) committed={{opcode:{:04B} op1:{:02B} op2:{:02B} imm:(${:02X}) mem:${:04X}}}",
							register_file[0], register_file[1], register_file[2], register_file[3], instruction_pointer, data_stack_pointer, DATA_STACK - data_stack_pointer, code_stack_pointer, CODE_STACK - code_stack_pointer, opcode, operand1, operand2, immediate, memory));
					}
				} break;

				case 'q':
				{
					Log::info("Simulation ending...");
					return;
				} break;

				//Special case to handle the first iteration cleanly
				case '\0':
					break;

				default:
				{
					Log::info("Unrecognized simulation command");
				} break;
			}
		}
	}
}
