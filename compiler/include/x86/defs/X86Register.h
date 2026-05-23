#ifndef HAZE_X86REGISTER_H
#define HAZE_X86REGISTER_H

#include <utility/Constants.h>
#include <utility/Formatter.h>

// Haze X86Register.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86Register : Register
	{
#define X(enumerator, name, value) enumerator = value,
#include <x86/defs/X86Register.def>
#undef X
	};

	constexpr std::string_view to_string(X86Register reg)
	{
		switch (reg)
		{
#define X(enumerator, name, value) case X86Register::enumerator: return name;
#include <x86/defs/X86Register.def>
#undef X
		}

		return "<unknown x86 register>";
	}
}

#endif
