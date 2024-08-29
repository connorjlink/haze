#include "X86Emitter.h"
#include "X86Builder.h"
#include "PEBuilder.h"
#include "BinaryConstants.h"
#include "ErrorReporter.h"
#include "Log.h"

#include <format>

// Haze X86Emitter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	

	constexpr auto x86_reg_reg(Register destination, Register source)
	{
		const auto mod = 0b11;
		const auto reg = source;
		const auto rm = destination;

		return X86Builder::modrm(mod, reg, rm);
	}
	
	
}


namespace hz
{
	EmitterType X86Emitter::etype() const
	{
		return EmitterType::X86;
	}


	byterange X86Emitter::build_stop(std::uint8_t exit_code)
	{
		byterange out{};

		// push {exit_code}
		PUT(BinaryUtilities::range8(0x68));
		PUT(BinaryUtilities::range32(exit_code));

		// call [0x402068] (ExitProcessA)
		PUT(BinaryUtilities::range8(0xFF));
		PUT(BinaryUtilities::range8(0x15));
		PUT(BinaryUtilities::range32(EXIT_PROCESS_VA));

		return out;
	}


	// done
	byterange X86Emitter::emit_move(Register destination, Register source)
	{
		// mov r/m32, r32
		return { 0x89, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_load(Register destination, std::uint32_t address)
	{
		byterange out{};

		// mov r32, r/m32 
		PUT(BinaryUtilities::range8(0x8B));
		// mod == 0, so displacement only
		// r/m == 101, so 32-bit displacement?
		PUT(BinaryUtilities::range8(BYTE(X86Builder::modrm(0b00, destination, 0b101)))); 
		PUT(BinaryUtilities::range32(address));

		return out;
	}

	// done
	byterange X86Emitter::emit_copy(Register destination, std::uint8_t immediate)
	{
		// mov r/m32, imm32
		byterange out{};

		PUT(BinaryUtilities::range8(0xB8 | destination));
		PUT(BinaryUtilities::range32(immediate));

		return out;
	}

	// done
	byterange X86Emitter::emit_save(std::uint32_t address, Register source)
	{
		byterange out{};

		//special case the print statements
		if (address == 0x1111)
		{
			PUT(BinaryUtilities::range8(BYTE(0x50 | source)));
			PUT(BinaryUtilities::range8(0xFF));
			PUT(BinaryUtilities::range8(0x15));
			PUT(BinaryUtilities::range32(EXIT_PROCESS_VA));
		}

		else
		{
			// mov r/m32, r32
			PUT(BinaryUtilities::range8(0x89));
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b00, source, 0b101)));
			PUT(BinaryUtilities::range32(address));
		}
		
		return out;
	}

	// done
	byterange X86Emitter::emit_iadd(Register destination, Register source)
	{
		// add r/m32, r32
		return { 0x01, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_isub(Register destination, Register source)
	{
		// sub r/m32, r32
		return { 0x29, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_band(Register destination, Register source)
	{
		// and r/m32, r32
		return { 0x21, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_bior(Register destination, Register source)
	{
		// or r/m32, r32
		return { 0x09, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_bxor(Register destination, Register source)
	{
		// xor r/m32, r32
		return { 0x31, BYTE(::x86_reg_reg(destination, source)) };
	}

	// done
	byterange X86Emitter::emit_call(std::uint32_t address)
	{
		byterange out{};

		// TODO: replace with `sub esp, 4`
		// push imm32 (save some space on the stack for the return value)
		PUT(BinaryUtilities::range8(0x68));
		PUT(BinaryUtilities::range32(0xAAAAAAAA)); // temp value

		// call NEAR
		PUT(BinaryUtilities::range8(0xE8));
		PUT(BinaryUtilities::range32(address));

		return out;
	}

	// done
	byterange X86Emitter::emit_exit()
	{
		// ret NEAR
		return { BYTE(0xC3) };
	}

	// done
	byterange X86Emitter::emit_push(Register source)
	{
		// NOTE: old format--clobbers the stack pointer register so the `ret` goes bad :(
		// push r32
		//return { BYTE(0x50 | source) };

#pragma message("TODO: have to figure out how to pass both return values and parameters")

		byterange out{};

		// mov DWORD PTR [esp+0x4], r32
		PUT(BinaryUtilities::range8(0x89));
		PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, source, 0b100)));
		PUT(BinaryUtilities::range8(0x24));
		PUT(BinaryUtilities::range8(0x04));

		return out;
	}

	// done
	byterange X86Emitter::emit_pull(Register destination)
	{
		// NOTE: even if putting data at [esp+0x4], this is still fine to do
		// Since we first would pop the return value and increment, 
		// another pop will point to the desired return value.
		
		// pop r32
		return { BYTE(0x58 | destination) };

		//PEBuilder::bytes_t out{};

		// mov r32, DWORD PTR [esp-0x4]
		//PUT(PEBuilder::make8(0x8b));
		//PUT(PEBuilder::make8(::x86_modrm(0b01, destination, 0b100)));
		//PUT(PEBuilder::make8(0x24));
		//PUT(PEBuilder::make8(0xFC));

		//return out;
	}

	// done
	byterange X86Emitter::emit_brnz(std::uint32_t address, Register source)
	{
		// NOTE: old format before using relative jump addresses
		
		// test r/m32, r32
		// je skip
		// jmp abs32
		//skip:

		//PUT(PEBuilder::make8(0x85));
		//PUT(PEBuilder::make8(BYTE(::x86_reg_reg(source, source))));
		//PUT(PEBuilder::make8(0x74));
		//PUT(PEBuilder::make8(0x06)); // 6 since the length of the next instruction is 6 bytes
		//PUT(PEBuilder::make8(0xFF));
		//PUT(PEBuilder::make8(0x25));
		//PUT(PEBuilder::make32(address));
		
		byterange out{};

		// test r/m32, r32
		PUT(BinaryUtilities::range8(0x85));
		PUT(BinaryUtilities::range8(BYTE(::x86_reg_reg(source, source))));
#pragma message("TODO: optimizations for rel8/rel16 jumps which have shorter encoding!")
		// jne rel32
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x85));
		PUT(BinaryUtilities::range32(address));


		return out;
	}

	// done
	byterange X86Emitter::emit_bool(Register source)
	{
		byterange out{};

		// test r/m32, r32
		PUT(BinaryUtilities::range8(0x85));
		PUT(BinaryUtilities::range8(BYTE(::x86_reg_reg(source, source))));
		// sete r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x94));
		PUT(BinaryUtilities::range8(BYTE(::x86_reg_reg(source, source))));

		return out;
	}

	
	byterange X86Emitter::emit_stop()
	{
		// push 0
		// call [0x402068] (ExitProcessA)

		return build_stop(0);
	}



	byterange X86Emitter::emit()
	{
		byterange result{};

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