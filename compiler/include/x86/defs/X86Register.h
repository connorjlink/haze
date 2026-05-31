#ifndef HAZE_X86REGISTER_H
#define HAZE_X86REGISTER_H

#include <utility/AutoEnum.h>
#include <utility/Constants.h>
#include <utility/Formatter.h>

// Haze X86Register.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define X86_REGISTER_KINDS(X) \
	X(EAX, eax, 0) \
	X(ECX, ecx, 1) \
	X(EDX, edx, 2) \
	X(EBX, ebx, 3) \
	X(ESP, esp, 4) \
	X(EBP, ebp, 5) \
	X(ESI, esi, 6) \
	X(EDI, edi, 7)


#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define SWITCH_CASE(enumerator, name, value) case X86Register::enumerator: return #name;

	DEFINE_ENUM_BACKED(ENUM_MEMBER, SWITCH_CASE, X86_REGISTER_KINDS, X86Register, x86 register, : Register)

#undef SWITCH_CASE
#undef ENUM_MEMBER


	Register to_register(X86Register reg)
	{
		return static_cast<Register>(reg);
	}
}

#endif
