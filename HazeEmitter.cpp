#include "HazeEmitter.h"
#include "InstructionCommand.h"
#include "ErrorReporter.h"

#include <format>

// Haze HazeEmitter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	std::vector<std::uint8_t> haze_instruction(HazeOpcode opcode, Register destination, Register source, std::uint8_t imm, std::uint32_t mem)
	{
		std::vector<std::uint8_t> result;

		const auto o = opcode & 0xF;
		const auto d = destination & 0b11;
		const auto s = source & 0b11;
		const auto i = imm & 0xFF;
		const auto m = mem & 0xFFFF;

		result.emplace_back((o << 4) | (d << 2) | (s << 0));
		result.emplace_back(i | (m >> 8));
		result.emplace_back(m & 0xFF);

		return result;
	}

	Register convert_register(register_t reg)
	{
		switch (reg)
		{
			case 0b00: return R0;
			case 0b01: return R1;
			case 0b10: return R2;
			case 0b11: return R3;

			default:
				_error_reporter->post_error(std::format("invalid register `{}` for architecture `haze`", reg), NULL_TOKEN);
				return DC;
		}
	}
	
}

namespace hz
{
	EmitterType HazeEmitter::etype() const
	{
		return EmitterType::HAZE;
	}

	byterange HazeEmitter::emit_move(Register destination, Register source)
	{
		return haze_instruction(MOVE, destination, source, 0x00, 0x000);
	}

	byterange HazeEmitter::emit_load(Register destination, std::uint32_t address)
	{
		return haze_instruction(LOAD, destination, DC, 0x00, address);
	}

	byterange HazeEmitter::emit_copy(Register destination, std::uint8_t immediate)
	{
		return haze_instruction(COPY, destination, DC, immediate, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_save(std::uint32_t address, Register source)
	{
		return haze_instruction(SAVE, DC, source, 0x00, address);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_iadd(Register destination, Register source)
	{
		return haze_instruction(IADD, destination, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_isub(Register destination, Register source)
	{
		return haze_instruction(ISUB, destination, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_band(Register destination, Register source)
	{
		return haze_instruction(BAND, destination, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_bior(Register destination, Register source)
	{
		return haze_instruction(BIOR, destination, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_bxor(Register destination, Register source)
	{
		return haze_instruction(BXOR, destination, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_call(std::uint32_t address)
	{
		return haze_instruction(CALL, DC, DC, 0x00, address);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_exit()
	{
		return haze_instruction(EXIT, DC, DC, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_push(Register source)
	{
		return haze_instruction(PUSH, DC, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_pull(Register destination)
	{
		return haze_instruction(PULL, destination, DC, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_brnz(std::uint32_t address, Register source)
	{
		return haze_instruction(BRNZ, DC, source, 0x00, address);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_bool(Register source)
	{
		return haze_instruction(BOOL, DC, source, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_stop()
	{
		return haze_instruction(STOP, DC, DC, 0x00, 0x0000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit()
	{
		std::vector<std::uint8_t> result{};

		for (auto instruction_command : image)
		{
			if (instruction_command != nullptr)
			{
				switch (instruction_command->opcode)
				{
					case Opcode::MOVE: result.append_range(emit_move(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::LOAD: result.append_range(emit_load(convert_register(instruction_command->dst), convert_register(instruction_command->mem))); break;
					case Opcode::COPY: result.append_range(emit_copy(convert_register(instruction_command->dst), convert_register(instruction_command->imm))); break;
					case Opcode::SAVE: result.append_range(emit_save(convert_register(instruction_command->mem), convert_register(instruction_command->src))); break;
					case Opcode::IADD: result.append_range(emit_iadd(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::ISUB: result.append_range(emit_isub(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::BAND: result.append_range(emit_band(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::BIOR: result.append_range(emit_bior(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::BXOR: result.append_range(emit_bxor(convert_register(instruction_command->dst), convert_register(instruction_command->src))); break;
					case Opcode::CALL: result.append_range(emit_call(convert_register(instruction_command->mem))); break;
					case Opcode::EXIT: result.append_range(emit_exit()); break;
					case Opcode::PUSH: result.append_range(emit_push(convert_register(instruction_command->src))); break;
					case Opcode::PULL: result.append_range(emit_pull(convert_register(instruction_command->dst))); break;
					case Opcode::MAKE: result.append_range(emit_make(convert_register(instruction_command->src))); break;
					case Opcode::TAKE: result.append_range(emit_take(convert_register(instruction_command->dst))); break;
					case Opcode::BRNZ: result.append_range(emit_brnz(convert_register(instruction_command->mem), convert_register(instruction_command->src))); break;
					case Opcode::BOOL: result.append_range(emit_bool(convert_register(instruction_command->src))); break;
					case Opcode::STOP: result.append_range(emit_stop()); break;

					default: 
					{
						_error_reporter->post_error(std::format("unrecognized instruction {:02x}", static_cast<std::uint8_t>(instruction_command->opcode)), NULL_TOKEN);
					} break;
				}
			}
		}

		return result;
	}
}