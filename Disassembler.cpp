#include "Disassembler.h"
#include "InstructionCommand.h"
#include "Constants.h"

#include <format>

namespace hz
{
	std::string Disassembler::disassemble_instruction(std::uint32_t raw)
	{
		const auto instruction = InstructionCommand{ NULL_TOKEN, raw };

		const auto opcode = instruction.opcode;

		const auto dst = _register_map.at(static_cast<Register>(instruction.dst));
		const auto src = _register_map.at(static_cast<Register>(instruction.src));

		const auto imm = instruction.imm;
		const auto mem = instruction.mem;

		switch (opcode)
		{
			case Opcode::MOVE: return std::format("move {}, {}    ",     dst, src);
			case Opcode::LOAD: return std::format("load {}, &${:04X}",   dst, mem);
			case Opcode::COPY: return std::format("copy {}, #${:02X}  ", dst, imm);
			case Opcode::SAVE: return std::format("save &${:04X}, {}",   mem, src);
			case Opcode::IADD: return std::format("iadd {}, {}    ",     dst, src);
			case Opcode::ISUB: return std::format("isub {}, {}    ",     dst, src);
			case Opcode::BAND: return std::format("band {}, {}    ",     dst, src);
			case Opcode::BIOR: return std::format("bior {}, {}    ",     dst, src);
			case Opcode::BXOR: return std::format("bxor {}, {}    ",     dst, src);
			case Opcode::CALL: return std::format("call ${:04X}     ",   mem);
			case Opcode::EXIT: return std::format("exit           ");
			case Opcode::PUSH: return std::format("push {}        ",     src);
			case Opcode::PULL: return std::format("pull {}        ",     dst);
			case Opcode::BRNZ: return std::format("brnz &${:04X}, {}", mem, src);
			case Opcode::BOOL: return std::format("bool {}        ", src);
			default:   return             "XXXX           ";
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

			if (byte1 == 0 && byte2 == 0 && byte3 == 0)
			{
				// We want to cut off disassembly if we get too many zero bytes in a row
				// a meaningful program should never "move r0, r0"
			
				if (i < last - 5)
				{
					const auto byte4 = program[i + 3],
							   byte5 = program[i + 4],
							   byte6 = program[i + 5];
			
					if (byte4 == 0 && byte5 == 0 && byte6 == 0)
					{
						break;
					}
				}
			
				else
				{
					break;
				}
			}

			const auto instruction = InstructionCommand
			{
				NULL_TOKEN,
				static_cast<std::uint32_t>((byte1 << 16) | (byte2 << 8) | (byte3 << 0)),
			};

#pragma message("TODO: fix instruction disassembly")
			//result.append(std::format("${:04X}: {}\n", HALF_DWORD_MAX + i, instruction.string()));
		}

		return result;
	}
}
