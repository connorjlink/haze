#ifndef HAZE_TYPEPRECEDENCE_H
#define HAZE_TYPEPRECEDENCE_H

#include <utility/AutoEnum.h>

// Haze TypePrecedence.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TYPE_PRECEDENCES(X) \
	X(LOWEST,   lowest) \
	X(POINTER,  pointer) \
	X(ARRAY,    array) \
	X(FUNCTION, function) \
	X(LEAF,     leaf) // base type


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, TypePrecedence)

	DEFINE_ENUM(TYPE_PRECEDENCES, TypePrecedence, type precedence)

#undef AUTOENUM_ROUTER
}

#endif
