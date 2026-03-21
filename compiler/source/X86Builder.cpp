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
}
