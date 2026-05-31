#ifndef HAZE_TYPEPRECEDENCE_H
#define HAZE_TYPEPRECEDENCE_H

#include <utility/AutoEnum.h>

// Haze $file$.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TYPE_PRECEDENCE(X) \
	X(LOWEST, lowest) \
	X(POINTER, pointer) \
	X(ARRAY, array) \
	X(FUNCTION, function) \
	X(LEAF, leaf) // base type

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case TypePrecedence::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TYPE_PRECEDENCE, TypePrecedence, type precedence)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
