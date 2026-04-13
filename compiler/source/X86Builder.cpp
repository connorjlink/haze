import std;

#include <x86/X86Builder.h>
#include <x86/X86Register.h>

// Haze X86Builder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Register X86Builder::modrm(Register mod, Register reg, Register rm)
	{
		return ((mod & 0b11) << 6) | ((reg & 0b111) << 3) | ((rm & 0b111) << 0);
	}

	Register X86Builder::modrm_rr(Register destination, Register source)
	{
		const auto mod = 0b11;
		const auto reg = source;
		const auto rm = destination;

		return modrm(mod, reg, rm);
	}

	Register X86Builder::sib(Register scale, Register index, Register base)
	{
		return X86Builder::modrm(scale, index, base);
	}
}
