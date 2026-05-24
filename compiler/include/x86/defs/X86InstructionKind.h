#ifndef HAZE_X86INSTRUCTIONKIND_H
#define HAZE_X86INSTRUCTIONKIND_H

#include <utility/Formatter.h>

// Haze X86InstructionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86InstructionKind
	{
#define X(enumerator, name) enumerator,
#include <x86/defs/X86InstructionKind.x>
#undef X
	};

	constexpr std::string_view to_string(X86InstructionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case X86InstructionKind::enumerator: return name;
#include <x86/defs/X86InstructionKind.x>
#undef X
		}

		return "<unknown x86 instruction kind>";
	}
}

#endif
