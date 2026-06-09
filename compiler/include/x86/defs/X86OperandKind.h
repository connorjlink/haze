#ifndef HAZE_X86OPERANDKIND_H
#define HAZE_X86OPERANDKIND_H

#include <utility/AutoEnum.h>

// Haze X86OperandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define X86_OPERAND_KINDS(X) \
	X(IMMEDIATE,          immediate) \
	X(INDIRECT,           indirect) \
	X(REGISTER,           register) \
	X(REGISTER_INDIRECT,  register-indirect) \
	X(REGISTER_DISPLACED, register-displaced)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, X86OperandKind)

	DEFINE_ENUM(X86_OPERAND_KINDS, X86OperandKind, x86 operand kind)

#undef AUTOENUM_ROUTER
}

#endif
