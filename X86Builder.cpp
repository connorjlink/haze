#include "X86Builder.h"

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
}