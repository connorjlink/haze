#include "Disassembler.h"
#include "Instruction.h"
#include "Constants.h"

#include <format>

namespace hz
{
	std::string Disassembler::disassemble_instruction(std::uint32_t raw)
	{
		const auto instruction = Instruction{ raw };

		const auto op1 = unmap(instruction.op1);
		const auto op2 = unmap(instruction.op2);

		const auto& imm = instruction.imm;
		const auto& mem = instruction.mem;

		switch (instruction.opcode)
		{
			case MOVE: return std::format("move {}, {} ", op1, op2);
			case LOAD: return std::format("load {}, &{}", op1, mem);
			case COPY: return std::format("copy {}, #{}", op1, imm);
			case SAVE: return std::format("save &{}, {}", mem, op2);
			case IADD: return std::format("iadd {}, {} ", op1, op2);
			case ISUB: return std::format("isub {}, {} ", op1, op2);
			case BAND: return std::format("band {}, {} ", op1, op2);
			case BIOR: return std::format("bior {}, {} ", op1, op2);
			case BXOR: return std::format("bxor {}, {} ", op1, op2);
			case CALL: return std::format("call &{}    ", mem);
			case EXIT: return std::format("exit        ");
			case PUSH: return std::format("push {}     ", op2);
			case PULL: return std::format("pull {}     ", op1);
			case BREZ: return std::format("brez &{}, {}", mem, op2);
			default:   return             "ILLEGAL OPCODE";
		}
	}

	std::string Disassembler::disassemble_program(const std::array<std::uint8_t, QUARTER_DWORD_MAX>& program, std::size_t first, std::size_t last)
	{
		std::string result{};

		for (auto i = first; i < last - 2; i+= 3)
		{
			const auto byte1 = program[i + 0];
			const auto byte2 = program[i + 1];
			const auto byte3 = program[i + 2];

			const auto instruction = (program[i + 0] << 16) | (program[i + 1] << 8) | (program[i + 2] << 0);

			result.append(std::format("{} ; #{:02X} {:02X} {:02X}", disassemble_instruction(instruction), byte1, byte2, byte3));
		}

		return result;
	}
}
