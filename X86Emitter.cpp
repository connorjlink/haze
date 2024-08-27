#include "X86Emitter.h"
#include "Log.h"
#include "PEBuilder.h"

#include <format>

namespace
{
	constexpr auto x86_reg(hz::Register reg)
	{
		switch (reg)
		{
			case hz::R0: return 0b00; // eax
			case hz::R2: return 0b01; // ecx
			case hz::R3: return 0b10; // edx
			case hz::R1: return 0b11; // ebx
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

	// done
	std::vector<std::uint8_t> X86Emitter::emit_move(Register destination, Register source)
	{
		// mov r/m32, r32
		return { 0x89, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_load(Register destination, std::uint16_t address)
	{
		// mov r32, r/m32 
		const auto address_cast = static_cast<std::uint32_t>(address);

		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0x8B));
		// mod == 0, so displacement only
		// r/m == 111, so 32-bit displacement
		PUT(BYTE(::x86_modrm(0b00, ::x86_reg(destination), 0b101))); 
		PUT(PEBuilder::make32(address_cast));

		return out;
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_copy(Register destination, std::uint8_t immediate)
	{
		// mov r/m32, imm32
		const auto immediate_cast = static_cast<std::uint32_t>(immediate);

		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0xB8 | ::x86_reg(destination)));
		PUT(PEBuilder::make32(immediate_cast));

		return out;
	}

	std::vector<std::uint8_t> X86Emitter::emit_save(std::uint16_t address, Register source)
	{
		return {};
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_iadd(Register destination, Register source)
	{
		// add r/m32, r32
		return { 0x01, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_isub(Register destination, Register source)
	{
		// sub r/m32, r32
		return { 0x29, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_band(Register destination, Register source)
	{
		// and r/m32, r32
		return { 0x21, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_bior(Register destination, Register source)
	{
		// or r/m32, r32
		return { 0x09, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_bxor(Register destination, Register source)
	{
		// xor r/m32, r32
		return { 0x31, BYTE(::x86_reg_reg(destination, source)) };
	}

	
	std::vector<std::uint8_t> X86Emitter::emit_call(std::uint16_t address)
	{
		return {};
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_exit()
	{
		// ret NEAR
		return { BYTE(0xC3) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_push(Register source)
	{
		// push r32
		return { BYTE(0x50 | ::x86_reg(source)) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_pull(Register destination)
	{
		// pop r32
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