#ifndef HAZE_VALUEKIND_H
#define HAZE_VALUEKIND_H

#include <utility/AutoEnum.h>
#include <utility/Sum.h>

// Haze ValueKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VALUE_KINDS(X) \
	X(REGISTER, RegisterValue, register) \
	X(STACK,    StackValue,    stack) \
	X(STATIC,   StaticValue,   static)


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, ValueKind)

	DEFINE_ENUM_BASE(VALUE_KINDS, ValueKind, value kind, TagType)

#undef AUTOENUM_ROUTER
}

#endif
