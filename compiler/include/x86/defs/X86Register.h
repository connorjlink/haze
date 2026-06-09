#ifndef HAZE_X86REGISTER_H
#define HAZE_X86REGISTER_H

#include <utility/AutoEnum.h>
#include <utility/Constants.h>

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


#define AUTOENUM_ROUTER(X, enumerator, name, value) X(enumerator, FakeType, name, X86Register)

	DEFINE_ENUM_BASE(X86_REGISTER_KINDS, X86Register, x86 register, Register)

#undef AUTOENUM_ROUTER


	inline Register to_register(X86Register $register)
	{
		return static_cast<Register>($register);
	}
}

#endif
