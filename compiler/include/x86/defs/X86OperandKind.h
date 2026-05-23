#ifndef HAZE_X86OPERANDKIND_H
#define HAZE_X86OPERANDKIND_H

#include <utility/Formatter.h>

// Haze X86OperandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86OperandKind
	{
#define X(enumerator, name) enumerator,
#include <x86/defs/X86OperandKind.def>
#undef X
	};

	constexpr std::string_view to_string(X86OperandKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case X86OperandKind::enumerator: return name;
#include <x86/defs/X86OperandKind.def>
#undef X
		}

		return "<unknown x86 operand kind>";
	}
}

#endif
