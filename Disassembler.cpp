#include "Disassembler.h"
#include "InstructionCommand.h"
#include "Constants.h"

#include <format>

namespace hz
{
	std::string Disassembler::disassemble_instruction(std::uint32_t raw)
	{
		const auto instruction = InstructionCommand{ raw };

		const auto opcode = instruction.opcode;

		const auto op1 = unmap(instruction.op1);
		const auto op2 = unmap(instruction.op2);

		const auto imm = instruction.imm;
		const auto mem = instruction.mem;

		if (opcode == MOVE && op1 == op2)
		{
			return std::format("noop           ");
		}

		switch (opcode)
		{
			case MOVE: return std::format("move {}, {}    ", op1, op2);
			case LOAD: return std::format("load {}, &${:04X}", op1, mem);
			case COPY: return std::format("copy {}, #${:02X}  ", op1, imm);
			case SAVE: return std::format("save &${:04X}, {}", mem, op2);
			case IADD: return std::format("iadd {}, {}    ", op1, op2);
			case ISUB: return std::format("isub {}, {}    ", op1, op2);
			case BAND: return std::format("band {}, {}    ", op1, op2);
			case BIOR: return std::format("bior {}, {}    ", op1, op2);
			case BXOR: return std::format("bxor {}, {}    ", op1, op2);
			case CALL: return std::format("call ${:04X}     ", mem);
			case EXIT: return std::format("exit           ");
			case PUSH: return std::format("push {}        ", op2);
			case PULL: return std::format("pull {}        ", op1);
			case BREZ: return std::format("brez &${:04X}, {}", mem, op2);
			default:   return             "ILLEGAL OPCODE";
		}
	}

	std::string Disassembler::disassemble_program(const std::array<std::uint8_t, HALF_DWORD_MAX>& program, std::size_t first, std::size_t last)
	{
		std::string result{};

		for (auto i = first; i < last - 2; i += 3)
		{
			const auto byte1 = program[i + 0],
			           byte2 = program[i + 1],
			           byte3 = program[i + 2];

			//if (byte1 == 0 && byte2 == 0 && byte3 == 0)
			//{
			//	// We want to cut off disassembly if we get too many zero bytes in a row
			//	// a meaningful program should never "move r0, r0"
			//
			//	if (i < last - 5)
			//	{
			//		const auto byte4 = program[i + 3],
			//				   byte5 = program[i + 4],
			//				   byte6 = program[i + 5];
			//
			//		if (byte4 == 0 && byte5 == 0 && byte6 == 0)
			//		{
			//			break;
			//		}
			//	}
			//
			//	else
			//	{
			//		break;
			//	}
			//}

			const auto instruction = InstructionCommand{ 
				static_cast<std::uint32_t>((byte1 << 16) | (byte2 << 8) | (byte3 << 0)) };

			result.append(std::format("${:04X}: {}\n", HALF_DWORD_MAX + i, instruction.string()));
		}

		return result;
	}
}
