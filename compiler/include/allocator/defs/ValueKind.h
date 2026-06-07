#ifndef HAZE_VALUEKIND_H
#define HAZE_VALUEKIND_H

#include <utility/Sum.h>

// Haze ValueKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VALUE_KINDS(X) \
	X(REGISTER, RegisterValue, register) \
	X(STACK,    StackValue,    stack) \
	X(STATIC,   StaticValue,   static)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case ValueKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, type, name) Mapping{ #name, ValueKind::enumerator },

	DEFINE_ENUM_BACKED(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, VALUE_KINDS, ValueKind, value kind, : TagType)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
