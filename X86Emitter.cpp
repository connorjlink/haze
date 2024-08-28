#include "X86Emitter.h"
#include "Log.h"
#include "PEBuilder.h"
#include "ErrorReporter.h"

#include <format>

namespace
{
	using namespace hz;

	constexpr auto x86_modrm(std::uint8_t mod, std::uint8_t reg, std::uint8_t rm)
	{
		return ((mod & 0b11) << 6) | ((reg & 0b111) << 3) | ((rm & 0b111) << 0);
	}

	constexpr auto x86_reg_reg(Register destination, Register source)
	{
		const auto mod = 0b11;
		const auto reg = source;
		const auto rm = destination;

		return x86_modrm(mod, reg, rm);
	}
	
	[[maybe_unused]]
	constexpr auto x86_reg_reg_embedded(Register destination, Register source)
	{
		const auto upper = destination;
		const auto lower = source;

		return (upper << 2) | lower;
	}

	static constexpr auto EXIT_PROCESS_VA = 0x402068;
}

#define BYTE(x) static_cast<std::uint8_t>(x)

namespace hz
{
	EmitterType X86Emitter::etype() const
	{
		return EmitterType::X86;
	}


	PEBuilder::bytes_t X86Emitter::build_stop(std::uint8_t exit_code)
	{
		// push {exit_code}
		// call [0x402068] (ExitProcessA)

		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0x68));
		PUT(PEBuilder::make32(exit_code));

		PUT(PEBuilder::make8(0xFF));
		PUT(PEBuilder::make8(0x15));
		PUT(PEBuilder::make32(EXIT_PROCESS_VA));

		return out;
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
		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0x8B));
		// mod == 0, so displacement only
		// r/m == 111, so 32-bit displacement
		PUT(PEBuilder::make8(BYTE(::x86_modrm(0b00, destination, 0b101)))); 
		PUT(PEBuilder::make32(address));

		return out;
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_copy(Register destination, std::uint8_t immediate)
	{
		// mov r/m32, imm32
		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0xB8 | destination));
		PUT(PEBuilder::make32(immediate));

		return out;
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_save(std::uint16_t address, Register source)
	{
		// mov r/m32, r32
		PEBuilder::bytes_t out{};

		if (address == 0x1111)
		{
			PUT(emit_push(source));

			PUT(PEBuilder::make8(0xFF));
			PUT(PEBuilder::make8(0x15));
			PUT(PEBuilder::make32(EXIT_PROCESS_VA));

			// temporary debugging purposes only
			//PUT(PEBuilder::make8(0xCC));
		}

		else
		{
			PUT(PEBuilder::make8(0x89));
			PUT(PEBuilder::make8(BYTE(::x86_modrm(0b00, source, 0b101))));
			PUT(PEBuilder::make32(address));
		}
		
		return out;
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

	// done
	std::vector<std::uint8_t> X86Emitter::emit_call(std::uint16_t address)
	{
		// call NEAR
		PEBuilder::bytes_t out{};
		
		PUT(PEBuilder::make8(0xFF));
		PUT(PEBuilder::make8(0x15));
		PUT(PEBuilder::make32(address));

		return out;
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
		return { BYTE(0x50 | source) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_pull(Register destination)
	{
		// pop r32
		return { BYTE(0x58 | destination) };
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_brnz(std::uint16_t address, Register source)
	{
		// jne NEAR (cant use because its relative jumping)
		// TODO: patch linker to allow for relative branches

		// test r/m32, r32
		// je skip
		// jmp abs32
		//skip:
		
		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0x85));
		PUT(PEBuilder::make8(BYTE(::x86_reg_reg(source, source))));
		PUT(PEBuilder::make8(0x74));
		PUT(PEBuilder::make8(0x06)); // 6 since the length of the next instruction is 6 bytes
		PUT(PEBuilder::make8(0xFF));
		PUT(PEBuilder::make8(0x25));
		PUT(PEBuilder::make32(address));

		return out;
	}

	// done
	std::vector<std::uint8_t> X86Emitter::emit_bool(Register source)
	{
		// test r/m32, r32
		// sete r/m8
		
		PEBuilder::bytes_t out{};

		PUT(PEBuilder::make8(0x85));
		PUT(PEBuilder::make8(BYTE(::x86_reg_reg(source, source))));
		PUT(PEBuilder::make8(0x0F));
		PUT(PEBuilder::make8(0x94));
		PUT(PEBuilder::make8(BYTE(::x86_reg_reg(source, source))));

		return out;
	}

	
	std::vector<std::uint8_t> X86Emitter::emit_stop()
	{
		// push 0
		// call [0x402068] (ExitProcessA)

		return build_stop(0);
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
					case BOOL: result.append_range(emit_bool(instruction_command->src)); break;
					case STOP: result.append_range(emit_stop()); break;
				}
			}
		}

		return result;
	}
}