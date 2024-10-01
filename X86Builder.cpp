import std;

#include "X86Builder.h"
#include "BinaryUtilities.h"

// Haze X86Builder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::uint8_t X86Builder::modrm(std::uint8_t mod, std::uint8_t reg, std::uint8_t rm)
	{
		return ((mod & 0b11) << 6) | ((reg & 0b111) << 3) | ((rm & 0b111) << 0);
	}

	std::uint8_t X86Builder::modrm_rr(std::uint8_t destination, std::uint8_t source)
	{
		const auto mod = 0b11;
		const auto reg = source;
		const auto rm = destination;

		return modrm(mod, reg, rm);
	}


	std::uint8_t X86Builder::sib(std::uint8_t scale, std::uint8_t index, std::uint8_t base)
	{
		return X86Builder::modrm(scale, index, base);
	}


	byterange X86Builder::push_r(std::uint8_t source)
	{
		// 50+rd --> PUSH r32
		const auto reg = source & 0b111;
		return { static_cast<std::uint8_t>(0x50 | reg) };
	}

	// is implicitly sign-extended to push a total of 4 bytes to the stack
	byterange X86Builder::push_i8(std::uint8_t immediate)
	{
		byterange out{};

		// 6A --> PUSH imm8
		PUT(BinaryUtilities::range8(0x6A));
		PUT(BinaryUtilities::range8(immediate));

		return out;
	}
	
	byterange X86Builder::push_i32(std::uint32_t immediate)
	{
		byterange out{};

		// 68 --> PUSH imm32
		PUT(BinaryUtilities::range8(0x68)); 
		PUT(BinaryUtilities::range32(immediate));

		return out;
	}

	// dereferencing
	byterange X86Builder::push_m(std::uint32_t address)
	{
		byterange out{};

		// FF /6 --> PUSH r/m32
		PUT(BinaryUtilities::range8(0xFF));
		PUT(BinaryUtilities::range8(0x35));
		PUT(BinaryUtilities::range32(address));

		return out;
	}


	byterange X86Builder::pop_r(std::uint8_t destination)
	{
		// 58+ rd --> POP r32
		const auto reg = destination & 0b111;
		return { static_cast<std::uint8_t>(0x58 | reg) };
	}



	byterange X86Builder::mov_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 89 /r --> MOV r/m32, r32
		PUT(BinaryUtilities::range8(0x89));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}


	byterange X86Builder::mov_rbd(std::uint8_t destination, std::uint8_t base, std::int32_t displacement)
	{
		byterange out{};

		// 8B /r --> MOV r32, r/m32
		PUT(BinaryUtilities::range8(0x8B));

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, destination, base)));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b10, destination, base)));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::mov_bdr(std::uint8_t base, std::int32_t displacement, std::uint8_t source)
	{
		byterange out{};

		// 89 /r --> MOV r/m32, r32
		PUT(BinaryUtilities::range8(0x89));

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, source, base)));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b10, source, base)));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::mov_rbo(std::uint8_t destination, std::int32_t offset)
	{
		byterange out{};

		// 8B /r --> MOV r32, r/m32
		PUT(BinaryUtilities::range8(0x8B));

		if (offset >= 0 && offset <= 0xFF)
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, destination, EBP)));
			PUT(BinaryUtilities::range8(offset));
		}

		else
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b10, destination, EBP)));
			PUT(BinaryUtilities::range32(offset));
		}

		return out;
	}

	byterange X86Builder::mov_obr(std::int32_t offset, std::uint8_t destination)
	{
		byterange out{};

		// 89 /r --> MOV r/m32, r32
		PUT(BinaryUtilities::range8(0x89));

		if (offset >= 0x00 && offset <= 0xFF)
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, destination, EBP)));
			PUT(BinaryUtilities::range8(offset));
		}

		else
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b10, destination, EBP)));
			PUT(BinaryUtilities::range32(offset));
		}

		return out;
	}


	byterange X86Builder::mov_rm(std::uint8_t destination, std::uint32_t pointer)
	{
		byterange out{};

		if (destination == EAX)
		{
			// A1 --> MOV EAX, moffs32*
			PUT(BinaryUtilities::range8(0xA1));
			PUT(BinaryUtilities::range32(pointer));
		}

		else
		{
			// 8B /r --> MOV r32, r/m32
			PUT(BinaryUtilities::range8(0x8B));
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b00, destination, 0b101)));
			PUT(BinaryUtilities::range32(pointer));
		}

		return out;
	}

	byterange X86Builder::mov_mr(std::uint32_t pointer, std::uint8_t source)
	{
		byterange out{};

		if (source == EAX)
		{
			// A3 --> MOV moffs32*, EAX
			PUT(BinaryUtilities::range8(0xA3));
			PUT(BinaryUtilities::range32(pointer));
		}
		
		else
		{
			// 89 /r --> MOV r/m32,r32
			PUT(BinaryUtilities::range8(0x89));
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b00, source, 0b101)));
			PUT(BinaryUtilities::range32(pointer));
		}


		return out;
	}


	byterange X86Builder::mov_ri(std::uint8_t destination, std::uint32_t immediate)
	{
		byterange out{};

		// NOTE: old method
		//if (immediate >= 0 && immediate <= 0xFF)
		if (false)
		{
			PUT(BinaryUtilities::range8(0xB0 | destination));
			PUT(BinaryUtilities::range8(immediate));
		}

		else
		{
			PUT(BinaryUtilities::range8(0xB8 | destination));
			PUT(BinaryUtilities::range32(immediate));
		}

		return out;
	}


	byterange X86Builder::mov_bdi(std::uint8_t base, std::int8_t displacement, std::uint32_t immediate)
	{
		byterange out{};

		// C7 /0 --> MOV r/m32, imm32
		PUT(BinaryUtilities::range8(0xC7));
		PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, 0b000, base)));
		PUT(BinaryUtilities::range8(displacement));
		PUT(BinaryUtilities::range32(immediate));

		return out;
	}


	byterange X86Builder::add_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 01 /r --> ADD r/m32, r32
		PUT(BinaryUtilities::range8(0x01));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::add_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 04 ib --> ADD AL, imm8
				PUT(BinaryUtilities::range8(0x04));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 05 id --> ADD EAX, imm32
				PUT(BinaryUtilities::range8(0x05));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /0 ib --> ADD r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b000)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /0 id --> ADD r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b000)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::sub_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 29 /r --> SUB r/m32,r32
		PUT(BinaryUtilities::range8(0x29));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::sub_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 2C ib --> SUB AL, imm8
				PUT(BinaryUtilities::range8(0x2C));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 2D id --> SUB EAX, imm32
				PUT(BinaryUtilities::range8(0x2D));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /5 ib --> SUB r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b101)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /5 id --> SUB r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b101)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::or_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 09 /r --> OR r/m32, r32
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::or_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 0C ib --> OR AL, imm8
				PUT(BinaryUtilities::range8(0x0C));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 0D id --> OR EAX, imm32
				PUT(BinaryUtilities::range8(0x0D));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /1 ib --> OR r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b001)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /1 id --> OR r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b001)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::and_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 09 /r --> OR r/m32, r32
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::and_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 24 ib --> AND AL, imm8
				PUT(BinaryUtilities::range8(0x24));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 25 id --> AND EAX, imm32
				PUT(BinaryUtilities::range8(0x25));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /4 ib --> AND r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b100)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /4 id --> AND r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b100)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::xor_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		//
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::xor_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				//34 ib --> XOR AL, imm8
				PUT(BinaryUtilities::range8(0x34));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 35 id --> XOR EAX, imm32
				PUT(BinaryUtilities::range8(0x35));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /6 ib --> XOR r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b110)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /6 id --> XOR r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, 0b110)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::inc_r(std::uint8_t source)
	{
		byterange out{};

		// 40+ rd --> INC r32
		PUT(BinaryUtilities::range8(0x40 | source));

		return out;
	}

	byterange X86Builder::dec_r(std::uint8_t source)
	{
		byterange out{};

		// 48+rd --> DEC r32
		PUT(BinaryUtilities::range8(0x48 | source));

		return out;
	}


	byterange X86Builder::test_rr(std::uint8_t lhs, std::uint8_t rhs)
	{
		byterange out{};

		// 85 /r --> TEST r/m32, r32
		PUT(BinaryUtilities::range8(0x85));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, rhs)));

		return out;
	}

	byterange X86Builder::cmp_rr(std::uint8_t lhs, std::uint8_t rhs)
	{
		byterange out{};

		// 39 /r --> CMP r/m32, r32
		PUT(BinaryUtilities::range8(0x39));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, rhs)));

		return out;
	}

	byterange X86Builder::cmp_ri(std::uint8_t lhs, std::int32_t immediate)
	{
		byterange out{};

		if (lhs == EAX)
		{
			// 3D id --> CMP EAX, imm32
			PUT(BinaryUtilities::range8(0x3D));
			PUT(BinaryUtilities::range32(immediate));
		}

		else
		{
			if (immediate >= -0x80 && immediate <= 0x7F)
			{
				// 83 /7 ib --> CMP r/m32, imm8
				PUT(BinaryUtilities::range8(0x83));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, 0b111)));
				PUT(BinaryUtilities::range8(immediate));
			}

			else
			{
				// 81 /7 id --> CMP r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, 0b111)));
				PUT(BinaryUtilities::range32(immediate));
			}
		}
		
		return out;
	}


	byterange X86Builder::call_absolute(std::uint32_t address)
	{
		byterange out{};

		// FF /2 --> CALL r/m32
		PUT(BinaryUtilities::range8(0xFF));
		PUT(BinaryUtilities::range8(0x15));
		PUT(BinaryUtilities::range32(address));

		return out;
	}

	byterange X86Builder::call_relative(std::int32_t displacement)
	{
		byterange out{};

		// E8 cd --> CALL rel32
		PUT(BinaryUtilities::range8(0xE8));
		PUT(BinaryUtilities::range32(displacement));

		return out;
	}

	byterange X86Builder::jmp_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			//EB cb --> JMP rel8
			PUT(BinaryUtilities::range8(0xEB));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// E9 cd --> JMP rel32
			PUT(BinaryUtilities::range8(0xE9));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::je_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 74 cb --> JE rel8
			PUT(BinaryUtilities::range8(0x74));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 84 cw/cd --> JE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x84));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jne_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 75 cb --> JNE rel8
			PUT(BinaryUtilities::range8(0x75));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 85 cw/cd --> JNE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x85));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jl_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7C cb --> JL rel8
			PUT(BinaryUtilities::range8(0x7C));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 8C cw/cd --> JL rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x8C));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jle_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7E cb --> JLE rel8
			PUT(BinaryUtilities::range8(0x7E));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 8E cw/cd --> JLE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x8E));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jg_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7F cb --> JG rel8
			PUT(BinaryUtilities::range8(0x7F));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 8F cw/cd --> JG rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x8F));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jge_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7D cb --> JGE rel8
			PUT(BinaryUtilities::range8(0x7D));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 8D cw/cd --> JGE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x8D));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::ja_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 77 cb --> JA rel8
			PUT(BinaryUtilities::range8(0x77));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 87 cw/cd --> JA rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x87));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jae_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 73 cb --> JAE rel8
			PUT(BinaryUtilities::range8(0x73));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 83 cw/cd --> JAE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x83));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jb_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 72 cb --> JB rel8
			PUT(BinaryUtilities::range8(0x72));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 82 cw/cd	JB rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x82));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jbe_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 76 cb --> JBE rel8
			PUT(BinaryUtilities::range8(0x76));
			PUT(BinaryUtilities::range8(displacement));
		}

		else
		{
			// 0F 86 cw/cd --> JBE rel16/32
			PUT(BinaryUtilities::range8(0x0F));
			PUT(BinaryUtilities::range8(0x86));
			PUT(BinaryUtilities::range32(displacement));
		}

		return out;
	}


	byterange X86Builder::sete_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 94 --> SETE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x94));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setne_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 95 --> SETNE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x95));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setl_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9C --> SETL r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x9C));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setle_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9E --> SETLE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x9E));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setg_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9F --> SETG r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x9F));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setge_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9D --> SETGE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x9D));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::seta_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 97 --> SETA r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x97));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setae_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 93 --> SETAE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x93));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setb_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 92 --> SETB r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x92));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setbe_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 96 --> SETBE r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0x96));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::movzx_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 0F B6 /r --> MOVZX r32, r/m8
		PUT(BinaryUtilities::range8(0x0F));
		PUT(BinaryUtilities::range8(0xB6));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::ret()
	{
		byterange out{};

		// C3 --> RET
		PUT(BinaryUtilities::range8(0xC3));

		return out;
	}

	byterange X86Builder::ret(std::uint16_t immediate)
	{
		byterange out{};

		// C2 iw --> RET imm16
		PUT(BinaryUtilities::range8(0xC2));
		PUT(BinaryUtilities::range16(immediate));

		return out;
	}

	byterange X86Builder::leave()
	{
		byterange out{};

		// C9 --> LEAVE
		PUT(BinaryUtilities::range8(0xC9));

		return out;
	}
}
