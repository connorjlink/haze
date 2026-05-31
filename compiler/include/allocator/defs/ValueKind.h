#ifndef HAZE_VALUEKIND_H
#define HAZE_VALUEKIND_H

#include <utility/Sum.h>

// Haze ValueKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VALUE_KINDS(X) \
	X(REGISTER, register) \
	X(STACK, stack) \
	X(STATIC, static)

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case ValueTag::enumerator: return #name;

	DEFINE_ENUM_BACKED(ENUM_MEMBER, SWITCH_CASE, VALUE_KINDS, ValueTag, value tag, : TagType)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
