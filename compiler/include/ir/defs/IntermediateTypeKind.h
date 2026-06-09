#ifndef HAZE_INTERMEDIATETYPEKIND_H
#define HAZE_INTERMEDIATETYPEKIND_H

#include <utility/AutoEnum.h>

// Haze IntermediateTypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define INTERMEDIATE_TYPE_KINDS(X) \
	X(VOID, 0, void) \
	X(BOOL, 1, bool) \
	X(INT8, 8, int8) \
	X(INT16, 16, int16) \
	X(INT32, 32, int32) \
	X(FLOAT32, 32, float32) \
	X(FLOAT64, 64, float64) \
	X(FLOAT80, 80, float80) \
	X(POINTER, (sizeof(Address)* std::numeric_limits<unsigned char>::digits), pointer)


#define AUTOENUM_ROUTER(X, enumerator, bits, name) X(enumerator, FakeType, name, IntermediateTypeKind)
	
	DEFINE_ENUM(INTERMEDIATE_TYPE_KINDS, IntermediateTypeKind, intermediate type kind)

#undef AUTOENUM_ROUTER
}

#endif
