#include "X86Emitter.h"
#include "Log.h"

#include <format>

namespace
{
	constexpr auto x86_reg(hz::Register reg)
	{
		switch (reg)
		{
			case hz::R0: return 0b00; // ax
			case hz::R2: return 0b01; // cx
			case hz::R3: return 0b10; // dx
			case hz::R1: return 0b11; // bx
		}

		hz::Log::error(std::format("Register value {} cannot be mapped an x86 register", static_cast<int>(reg)));
	}

	constexpr auto x86_modrm(std::uint8_t mod, std::uint8_t reg, std::uint8_t rm)
	{
		return ((mod & 0b11) << 6) | ((reg & 0b111) << 3) | ((rm & 0b111) << 0);
	}

	constexpr auto x86_reg_reg(hz::Register destination, hz::Register source)
	{
		const auto mod = 0b11;
		const auto reg = x86_reg(source);
		const auto rm = x86_reg(destination);

		return x86_modrm(mod, reg, rm);
	}
	
	[[maybe_unused]]
	constexpr auto x86_reg_reg_embedded(hz::Register destination, hz::Register source)
	{
		const auto upper = x86_reg(destination);
		const auto lower = x86_reg(source);

		return (upper << 2) | lower;
	}
}

#define BYTE(x) static_cast<std::uint8_t>(x)

namespace hz
{
	EmitterType X86Emitter::etype() const
	{
		return EmitterType::X86;
	}

	std::vector<std::uint8_t> X86Emitter::emit_move(Register destination, Register source)
	{
		// mov r/m8, r8
		return { 0x88, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_load(Register destination, std::uint16_t address)
	{
		// mov r8, r/m8 
		return {};
	}

	std::vector<std::uint8_t> X86Emitter::emit_copy(Register destination, std::uint8_t immediate)
	{
		// mov r/m8, imm8
		return { BYTE(0xB0 | ::x86_reg(destination)), immediate };
	}

	std::vector<std::uint8_t> X86Emitter::emit_save(std::uint16_t address, Register source)
	{
		return {};
	}

	std::vector<std::uint8_t> X86Emitter::emit_iadd(Register destination, Register source)
	{
		// add r/m8, r8
		return { 0x00, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_isub(Register destination, Register source)
	{
		// sub r/m8, r8
		return { 0x28, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_band(Register destination, Register source)
	{
		// and r/m8, r8
		return { 0x20, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_bior(Register destination, Register source)
	{
		// or r/m8, r8
		return { 0x08, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_bxor(Register destination, Register source)
	{
		// xor r/m8, r8
		return { 0x30, BYTE(::x86_reg_reg(destination, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_call(std::uint16_t address)
	{
		return {};
	}

	std::vector<std::uint8_t> X86Emitter::emit_exit()
	{
		// ret near
		return { 0xC3 };
	}

	std::vector<std::uint8_t> X86Emitter::emit_push(Register source)
	{
		// our math truncates anyway so this should be fine!
		// push r64
		return { BYTE(0x50 | ::x86_reg(source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_pull(Register destination)
	{
		// our math truncates anyway so this should be fine!
		// pop r64
		return { BYTE(0x58 | ::x86_reg(destination)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit_brnz(std::uint16_t address, Register source)
	{
		return {};
	}

	std::vector<std::uint8_t> X86Emitter::emit_bool(Register source)
	{
		// test r/m8, r8
		// sete r/m8
		return { 0x84, BYTE(::x86_reg_reg(source, source)), 0x0F, 0x94, BYTE(::x86_reg_reg(source, source)) };
	}

	std::vector<std::uint8_t> X86Emitter::emit()
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
					case BOOL: result.append_range(emit_bool(instruction_command->src));
				}
			}
		}

		return result;
	}
}