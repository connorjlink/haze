import std;

#include <command/InstructionCommand.h>
#include <toolchain/X86Emitter.h>
#include <utility/BinaryConstants.h>
#include <utility/BinaryUtilities.h>
#include <x86/X86Builder.h>

// Haze X86Emitter.cpp
// (c) Connor J. Link. All Rights Reserved.

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
		PUT(range8(0x68));
		PUT(range32(exit_code));

		// call ExitProcessA
		PUT(range8(0xFF));
		PUT(range8(0x15));
		PUT(range32(PROCEDURE_BASE + exitprocess_iat_va));

		return out;
	}


	// done
	byterange X86Emitter::emit_move(register_t destination, register_t source)
	{
		// mov r/m32, r32
		return { 0x89, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_load(register_t destination, std::uint32_t address)
	{
		byterange out{};

		// mov r32, r/m32 
		PUT(range8(0x8B));
		// mod == 0, so displacement only
		// r/m == 101, so 32-bit displacement?
		PUT(range8(X86Builder::modrm(0b00, destination, 0b101))); 
		PUT(range32(address));

		return out;
	}

	// done
	byterange X86Emitter::emit_copy(register_t destination, std::uint8_t immediate)
	{
		// mov r/m32, imm32
		byterange out{};

		PUT(range8(0xB8 | destination));
		PUT(range32(immediate));

		return out;
	}

	/*byterange emit_print(std::uint32_t text_pointer)
	{
		return {};
	}*/

	byterange X86Emitter::emit_make(register_t source)
	{
		return {};
	}

	byterange X86Emitter::emit_take(register_t destination)
	{
		return {};
	}

	// done
	byterange X86Emitter::emit_save(std::uint32_t address, register_t source)
	{
		byterange out{};

		//special case the print statements
		if (address == 0x1111)
		{
			// push NULL
			PUT(range8(0x6A));
			PUT(range8(0x00));

			// push NULL
			PUT(range8(0x6A));
			PUT(range8(0x00));

			// push length (15 bytes)
			PUT(range8(0x6A));
			PUT(range8(16));

			// push string (output info)
			PUT(range8(0x68));
			PUT(range32(0x0040304E));

			// push esi
			PUT(range8(0x56));

			// call WriteConsole()
			PUT(range8(0xFF));
			PUT(range8(0x15));
			PUT(range32(PROCEDURE(writeconsole_iat_va)));


			// Convert our one? digit to ascii

			// and r/m32, 0xFF
			PUT(range8(0x81));
			PUT(range8(X86Builder::modrm(0b11, 0b100, source)));
			//PUT(range8(0xE0));
			PUT(range8(0xFF));
			PUT(range8(0x00));
			PUT(range8(0x00));
			PUT(range8(0x00));

			// add r/m32, imm8
			PUT(range8(0x83));
			PUT(range8(X86Builder::modrm_rr(source, source)));
			PUT(range8(0x30));



			// push NULL
			PUT(range8(0x6A));
			PUT(range8(0x00));

			// push NULL
			PUT(range8(0x6A));
			PUT(range8(0x00));

			// push length (1 byte)
			PUT(range8(0x6A));
			PUT(range8(2));

			// move [0x4013FF], r/m32
			if (source == EAX)
			{
				PUT(range8(0xA3));
				PUT(range32(0x004033FF));
			}

			else
			{
				PUT(range8(0x89));
				PUT(range8(X86Builder::modrm(0b00, source, 0b101)));
				PUT(range32(0x004033FF));
			}

			

			// push [0x4013FF]
			PUT(X86Builder::push_i32(0x004033FF));


			// push esi
			PUT(X86Builder::push_r(ESI));

			// call WriteConsole()
			PUT(range8(0xFF));
			PUT(range8(0x15));
			PUT(range32(PROCEDURE_BASE + writeconsole_iat_va));
		}

		else
		{
			// mov r/m32, r32
			PUT(range8(0x89));
			PUT(range8(X86Builder::modrm(0b00, source, 0b101)));
			PUT(range32(address));
		}
		
		return out;
	}

	// done
	byterange X86Emitter::emit_iadd(register_t destination, register_t source)
	{
		// add r/m32, r32
		return { 0x01, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_isub(register_t destination, register_t source)
	{
		// sub r/m32, r32
		return { 0x29, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_band(register_t destination, register_t source)
	{
		// and r/m32, r32
		return { 0x21, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_bior(register_t destination, register_t source)
	{
		// or r/m32, r32
		return { 0x09, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_bxor(register_t destination, register_t source)
	{
		// xor r/m32, r32
		return { 0x31, X86Builder::modrm_rr(destination, source) };
	}

	// done
	byterange X86Emitter::emit_call(std::uint32_t address)
	{
		byterange out{};

		// TODO: replace with `sub esp, 4`
		// push imm32 (save some space on the stack for the return value)
		PUT(range8(0x68));
		PUT(range32(0xAAAAAAAA)); // temp value

		// call NEAR
		PUT(range8(0xE8));
		PUT(range32(address));

		return out;
	}

	// done
	byterange X86Emitter::emit_exit()
	{
		// ret NEAR
		return { 0xC3 };
	}

	// done
	byterange X86Emitter::emit_push(register_t source)
	{
		// NOTE: old format--clobbers the stack pointer register so the `ret` goes bad :(
		// push r32
		//return { BYTE(0x50 | source) };

		// NOTE: newer format--used before switching to CodeBuilder generic codegenerator frontend
		//byterange out{};

		// mov DWORD PTR [esp+0x4], r32
		//PUT(range8(0x89));
		//PUT(range8(X86Builder::modrm(0b01, source, 0b100)));
		//PUT(range8(0x24));
		//PUT(range8(0x04));

		//return out;

		return X86Builder::push_r(source);
	}

	// done
	byterange X86Emitter::emit_pull(register_t destination)
	{
		// NOTE: newer format--used before switching to CodeBuilder generic codegenerator frontend
		//byterange out{};

		// mov r32, DWORD PTR [esp-0x4]
		//PUT(PEBuilder::make8(0x8b));
		//PUT(PEBuilder::make8(::x86_modrm(0b01, destination, 0b100)));
		//PUT(PEBuilder::make8(0x24));
		//PUT(PEBuilder::make8(0xFC));

		//return out;
		
		// NOTE: even if putting data at [esp+0x4], this is still fine to do
		// Since we first would pop the return value and increment, 
		// another pop will point to the desired return value.
		
		// pop r32
		return { static_cast<byte>(0x58 | static_cast<byte>(destination)) };
	}

	// done
	byterange X86Emitter::emit_brnz(std::uint32_t address, register_t source)
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
		PUT(range8(0x85));
		PUT(range8(X86Builder::modrm_rr(source, source)));
#pragma message("TODO: optimizations for rel8/rel16 jumps which have shorter encoding!")
		// jne rel32
		PUT(range8(0x0F));
		PUT(range8(0x85));
		PUT(range32(address));

		return out;
	}

	// done
	byterange X86Emitter::emit_bool(register_t source)
	{
		byterange out{};

		// test r/m32, r32
		PUT(range8(0x85));
		PUT(range8(X86Builder::modrm_rr(source, source)));
		// sete r/m8
		PUT(range8(0x0F));
		PUT(range8(0x94));
		PUT(range8(X86Builder::modrm_rr(source, source)));

		return out;
	}

	
	byterange X86Emitter::emit_stop()
	{
		// push 0
		// call [0x402068] (ExitProcess)
		//return build_stop(0);

		// push 0
		// push 0x403000
		// push 0x403000
		// push 0
		// call MessageBoxA

		byterange out{};

		/*PUT(range8(0x6A));
		PUT(range8(0x00));

		PUT(range8(0x68));
		PUT(range8(0x00));
		PUT(range8(0x30));
		PUT(range8(0x40));
		PUT(range8(0x00));

		PUT(range8(0x68));
		PUT(range8(0x00));
		PUT(range8(0x30));
		PUT(range8(0x40));
		PUT(range8(0x00));

		PUT(range8(0x6A));
		PUT(range8(0x00));

		PUT(range8(0xFF));
		PUT(range8(0x15));
		PUT(range32(PROCEDURE_BASE + messageboxa_iat_va));*/

		// old exit process proc address
		/*PUT(range8(0x70));
		PUT(range8(0x20));
		PUT(range8(0x40));z`
		PUT(range8(0x00));*/

		PUT(X86Builder::push_i8(0x65));
		
		/*
		0:  b8 65 00 00 00          mov    eax,0x65
		5:  a3 40 30 40 00          mov    ds:0x403040,eax
		a:  68 40 30 40 00          push   0x403040 
*/

		//PUT(range8(0xB8));
		//PUT(range32(0x00000065));

		//PUT(range8(0xA3));
		//PUT(range32(0x00403340));

		//PUT(X86Builder::push_m(0x00403340));

		// format string
		PUT(X86Builder::push_i32(0x0040305F));

		PUT(X86Builder::push_i8(0x0F));

		// output string
		PUT(X86Builder::push_i32(0x004033F0));

		PUT(range8(0xFF));
		PUT(range8(0x15));
		PUT(range32(PROCEDURE_BASE + wnsprintfa_iat_va));

		PUT(build_stop(201));

		return out;
	}


	byterange X86Emitter::emit_init()
	{
		byterange out{};

		// push STD_OUTPUT_HANDLE
		{
			PUT(range8(0x68));

			// NOTE: this is some hackery so we can avoid including <windows.h> 
			// since it is a MASSIVE header file and we really don't need anything from it.
			// See the following link for more info:
			// https://learn.microsoft.com/en-us/windows/console/getstdhandle
#define DWORD std::uint32_t
#define STD_OUTPUT_HANDLE ((DWORD)-11)
			PUT(range32(STD_OUTPUT_HANDLE));
#undef STD_OUTPUT_HANDLE
#undef DWORD
		}

		// call GetStdHandle
		PUT(X86Builder::call_absolute(PROCEDURE(getstdhandle_iat_va)));
		// NOTE: old method
		//PUT(range32(0x402080));

		// handle is stored in eax
		
		// mov [STDOUT_HANDLE], eax
		PUT(X86Builder::mov_mr(STDOUT_HANDLE, EAX));

		// mov esi, eax
		PUT(X86Builder::mov_rr(ESI, EAX));

		// push NULL
		PUT(X86Builder::push_i8(0x00));

		// push NULL
		PUT(X86Builder::push_i8(0x00));

		// push length (77 bytes)
		PUT(X86Builder::push_i8(77));

		// push string (copyright logo)
		PUT(X86Builder::push_i32(COPYRIGHT_LOGO));

		// push eax
		PUT(X86Builder::push_r(EAX));

		// call WriteConsoleA
		PUT(X86Builder::call_absolute(PROCEDURE(writeconsole_iat_va)));
		// NOTE: old method
		//PUT(range32(0x4020A0));

		return out;
	}


	byterange X86Emitter::emit()
	{
		byterange result{};

		result.append_range(emit_init());

		for (auto instruction_command : image)
		{
			if (instruction_command != nullptr)
			{
				switch (instruction_command->opcode)
				{
					case Opcode::MOVE: result.append_range(emit_move(instruction_command->destination, instruction_command->source)); break;
					case Opcode::LOAD: result.append_range(emit_load(instruction_command->destination, instruction_command->address)); break;
					case Opcode::COPY: result.append_range(emit_copy(instruction_command->destination, instruction_command->immediate)); break;
					case Opcode::SAVE: result.append_range(emit_save(instruction_command->address, instruction_command->source)); break;
					case Opcode::IADD: result.append_range(emit_iadd(instruction_command->destination, instruction_command->source)); break;
					case Opcode::ISUB: result.append_range(emit_isub(instruction_command->destination, instruction_command->source)); break;
					case Opcode::BAND: result.append_range(emit_band(instruction_command->destination, instruction_command->source)); break;
					case Opcode::BIOR: result.append_range(emit_bior(instruction_command->destination, instruction_command->source)); break;
					case Opcode::BXOR: result.append_range(emit_bxor(instruction_command->destination, instruction_command->source)); break;
					case Opcode::CALL: result.append_range(emit_call(instruction_command->address)); break;
					case Opcode::EXIT: result.append_range(emit_exit()); break;
					case Opcode::PUSH: result.append_range(emit_push(instruction_command->source)); break;
					case Opcode::PULL: result.append_range(emit_pull(instruction_command->destination)); break;
					case Opcode::BRNZ: result.append_range(emit_brnz(instruction_command->address, instruction_command->source)); break;
					case Opcode::BOOL: result.append_range(emit_bool(instruction_command->source)); break;
					case Opcode::STOP: result.append_range(emit_stop()); break;
				}
			}
		}

		return result;
	}
}