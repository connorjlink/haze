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

	std::uint8_t X86Builder::modrm_rr(Register destination, Register source)
	{
		const auto mod = 0b11;
		const auto reg = destination;
		const auto rm = source;

		return modrm(mod, reg, rm);
	}


	std::uint8_t X86Builder::sib(std::uint8_t scale, std::uint8_t index, std::uint8_t base)
	{
		return X86Builder::modrm(scale, index, base);
	}


	byterange X86Builder::push_r(std::uint8_t source)
	{
		const auto reg = source & 0b111;

		return { static_cast<std::uint8_t>(0x50 | reg) };
	}

	byterange X86Builder::push_i(std::uint8_t source)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x6A));
		PUT(BinaryUtilities::range8(source));

		return out;
	}

	// non-dereferencing
	byterange X86Builder::push_m(std::uint32_t address)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x68)); 
		PUT(BinaryUtilities::range32(address));

		return out;
	}





	byterange X86Builder::pop_r(std::uint8_t destination)
	{
		const auto reg = destination & 0b111;

		return { static_cast<std::uint8_t>(0x58 | reg) };
	}
}