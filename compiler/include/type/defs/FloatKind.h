#ifndef HAZE_FLOATKIND_H
#define HAZE_FLOATKIND_H

#include <utility/AutoEnum.h>

// Haze FloatKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define FLOAT_KINDS(X) \
	X(FLOAT,       float) \
	X(DOUBLE,      double) \
	X(LONG_DOUBLE, long double)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, FloatKind)

	DEFINE_ENUM(FLOAT_KINDS, FloatKind, float kind)

#undef AUTOENUM_ROUTER
}

#endif
