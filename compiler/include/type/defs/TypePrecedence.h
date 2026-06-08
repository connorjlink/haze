#ifndef HAZE_TYPEPRECEDENCE_H
#define HAZE_TYPEPRECEDENCE_H

#include <utility/AutoEnum.h>

// Haze $file$.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TYPE_PRECEDENCES(X) \
	X(LOWEST,   lowest) \
	X(POINTER,  pointer) \
	X(ARRAY,    array) \
	X(FUNCTION, function) \
	X(LEAF,     leaf) // base type

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case TypePrecedence::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, TypePrecedence::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, TYPE_PRECEDENCES, TypePrecedence, type precedence)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
