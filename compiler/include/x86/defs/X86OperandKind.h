#ifndef HAZE_X86OPERANDKIND_H
#define HAZE_X86OPERANDKIND_H

#include <utility/Formatter.h>

// Haze X86OperandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define X86_OPERAND_KINDS(X) \
	X(IMMEDIATE, immediate) \
	X(INDIRECT, indirect) \
	X(REGISTER, register) \
	X(REGISTER_INDIRECT, register-indirect) \
	X(REGISTER_DISPLACED, register-displaced)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case X86OperandKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, X86OperandKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, X86_OPERAND_KINDS, X86OperandKind, x86 operand kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
