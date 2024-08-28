#include "HazeEmitter.h"
#include "Log.h"

namespace
{
	constexpr std::vector<std::uint8_t> haze_instruction(hz::Opcode opcode, hz::Register destination, hz::Register source, std::uint8_t imm, std::uint32_t mem)
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
}

namespace hz
{
	EmitterType HazeEmitter::etype() const
	{
		return EmitterType::HAZE;
	}

	std::vector<std::uint8_t> HazeEmitter::emit_move(Register destination, Register source)
	{
		return haze_instruction(MOVE, destination, source, 0x00, 0x000);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_load(Register destination, std::uint32_t address)
	{
		return haze_instruction(LOAD, destination, DC, 0x00, address);
	}

	std::vector<std::uint8_t> HazeEmitter::emit_copy(Register destination, std::uint8_t immediate)
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
					case MOVE: result.append_range(emit_move(instruction_command->dst, instruction_command->src)); break;
					case LOAD: result.append_range(emit_load(instruction_command->dst, instruction_command->mem)); break;
					case COPY: result.append_range(emit_copy(instruction_command->dst, instruction_command->imm)); break;
					case SAVE: result.append_range(emit_save(instruction_command->mem, instruction_command->src)); break;
					case IADD: result.append_range(emit_iadd(instruction_command->dst, instruction_command->src)); break;
					case ISUB: result.append_range(emit_isub(instruction_command->dst, instruction_command->src)); break;
					case BAND: result.append_range(emit_band(instruction_command->dst, instruction_command->src)); break;
					case BIOR: result.append_range(emit_bior(instruction_command->dst, instruction_command->src)); break;
					case BXOR: result.append_range(emit_bxor(instruction_command->dst, instruction_command->src)); break;
					case CALL: result.append_range(emit_call(instruction_command->mem)); break;
					case EXIT: result.append_range(emit_exit()); break;
					case PUSH: result.append_range(emit_push(instruction_command->src)); break;
					case PULL: result.append_range(emit_pull(instruction_command->dst)); break;
					case BRNZ: result.append_range(emit_brnz(instruction_command->mem, instruction_command->src)); break;
					case BOOL: result.append_range(emit_bool(instruction_command->src)); break;
					case STOP: result.append_range(emit_stop()); break;

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