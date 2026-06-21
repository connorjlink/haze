#ifndef HAZE_INTKIND_H
#define HAZE_INTKIND_H

#include <utility/AutoEnum.h>

// Haze IntKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define INT_KINDS(X) \
	X(CHAR,      char) \
	X(SHORT,     short) \
	X(INT,       int) \
	X(LONG,      long) \
	X(LONG_LONG, long long)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, IntKind)

	DEFINE_ENUM(INT_KINDS, IntKind, int kind)

#undef AUTOENUM_ROUTER
}

#endif
