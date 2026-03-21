import std;

#include <x86/X86Builder.h>
#include <x86/X86Register.h>

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





	byterange X86Builder::sal_imm(std::uint8_t destination, std::uint8_t immediate)
	{
		byterange out{};

		// C1 /4 ib --> SAL r/m32, imm8
		PUT(range8(0xC1));
		PUT(range8(X86Builder::modrm(0b11, 0b100, destination)));
		PUT(range8(immediate));

		return out;
	}

	byterange X86Builder::sar_imm(std::uint8_t destination, std::uint8_t immediate)
	{
		byterange out{};

		// C1 /7 ib --> SAR r/m32, imm8
		PUT(range8(0xC1));
		PUT(range8(X86Builder::modrm(0b11, 0b111, destination)));
		PUT(range8(immediate));

		return out;
	}

	byterange X86Builder::cmp_ri(std::uint8_t lhs, std::int32_t immediate)
	{
		byterange out{};

		if (lhs == EAX)
		{
			// 3D id --> CMP EAX, imm32
			PUT(range8(0x3D));
			PUT(range32(immediate));
		}

		else
		{
			if (immediate >= -0x80 && immediate <= 0x7F)
			{
				// 83 /7 ib --> CMP r/m32, imm8
				PUT(range8(0x83));
				PUT(range8(X86Builder::modrm_rr(lhs, 0b111)));
				PUT(range8(immediate));
			}

			else
			{
				// 81 /7 id --> CMP r/m32, imm32
				PUT(range8(0x81));
				PUT(range8(X86Builder::modrm_rr(lhs, 0b111)));
				PUT(range32(immediate));
			}
		}
		
		return out;
	}


	byterange X86Builder::call_absolute(std::uint32_t address)
	{
		byterange out{};

		// FF /2 --> CALL r/m32
		PUT(range8(0xFF));
		PUT(range8(0x15));
		PUT(range32(address));

		return out;
	}

	byterange X86Builder::call_relative(std::int32_t displacement)
	{
		byterange out{};

		// E8 cd --> CALL rel32
		PUT(range8(0xE8));
		PUT(range32(displacement));

		return out;
	}

	byterange X86Builder::jmp_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			//EB cb --> JMP rel8
			PUT(range8(0xEB));
			PUT(range8(displacement));
		}

		else
		{
			// E9 cd --> JMP rel32
			PUT(range8(0xE9));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::je_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 74 cb --> JE rel8
			PUT(range8(0x74));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 84 cw/cd --> JE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x84));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jne_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 75 cb --> JNE rel8
			PUT(range8(0x75));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 85 cw/cd --> JNE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x85));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jl_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7C cb --> JL rel8
			PUT(range8(0x7C));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 8C cw/cd --> JL rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x8C));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jle_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7E cb --> JLE rel8
			PUT(range8(0x7E));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 8E cw/cd --> JLE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x8E));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jg_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7F cb --> JG rel8
			PUT(range8(0x7F));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 8F cw/cd --> JG rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x8F));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jge_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 7D cb --> JGE rel8
			PUT(range8(0x7D));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 8D cw/cd --> JGE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x8D));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::ja_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 77 cb --> JA rel8
			PUT(range8(0x77));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 87 cw/cd --> JA rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x87));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jae_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 73 cb --> JAE rel8
			PUT(range8(0x73));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 83 cw/cd --> JAE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x83));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jb_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 72 cb --> JB rel8
			PUT(range8(0x72));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 82 cw/cd	JB rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x82));
			PUT(range32(displacement));
		}

		return out;
	}

	byterange X86Builder::jbe_relative(std::int32_t displacement)
	{
		byterange out{};

		if (displacement >= -0x80 && displacement <= 0x7F)
		{
			// 76 cb --> JBE rel8
			PUT(range8(0x76));
			PUT(range8(displacement));
		}

		else
		{
			// 0F 86 cw/cd --> JBE rel16/32
			PUT(range8(0x0F));
			PUT(range8(0x86));
			PUT(range32(displacement));
		}

		return out;
	}


	byterange X86Builder::sete_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 94 --> SETE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x94));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setne_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 95 --> SETNE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x95));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setl_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9C --> SETL r/m8
		PUT(range8(0x0F));
		PUT(range8(0x9C));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setle_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9E --> SETLE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x9E));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setg_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9F --> SETG r/m8
		PUT(range8(0x0F));
		PUT(range8(0x9F));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setge_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 9D --> SETGE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x9D));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::seta_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 97 --> SETA r/m8
		PUT(range8(0x0F));
		PUT(range8(0x97));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setae_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 93 --> SETAE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x93));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setb_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 92 --> SETB r/m8
		PUT(range8(0x0F));
		PUT(range8(0x92));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::setbe_r(std::uint8_t destination)
	{
		byterange out{};

		// 0F 96 --> SETBE r/m8
		PUT(range8(0x0F));
		PUT(range8(0x96));
		PUT(range8(X86Builder::modrm_rr(destination, destination)));

		return out;
	}

	byterange X86Builder::movzx_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 0F B6 /r --> MOVZX r32, r/m8
		PUT(range8(0x0F));
		PUT(range8(0xB6));
		PUT(range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::ret()
	{
		byterange out{};

		// C3 --> RET
		PUT(range8(0xC3));

		return out;
	}

	byterange X86Builder::ret(std::uint16_t immediate)
	{
		byterange out{};

		// C2 iw --> RET imm16
		PUT(range8(0xC2));
		PUT(range16(immediate));

		return out;
	}

	byterange X86Builder::leave()
	{
		byterange out{};

		// C9 --> LEAVE
		PUT(range8(0xC9));

		return out;
	}
}
