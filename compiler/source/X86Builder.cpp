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

		PUT(range8(0xC9));

		return out;
	}
}
