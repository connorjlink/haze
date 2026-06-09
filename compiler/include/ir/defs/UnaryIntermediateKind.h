#ifndef HAZE_UNARYINTERMEDIATEKIND_H
#define HAZE_UNARYINTERMEDIATEKIND_H

#include <utility/AutoEnum.h>

// Haze UnaryIntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define UNARY_INTERMEDIATE_KINDS(X) \
	X(NEGATE,     -,    negate) \
	X(BOOL,       bool, bool) \
	X(BITWISE_NOT, ~,   bitwise not)


#define AUTOENUM_ROUTER(X, enumerator, operator, name) X(enumerator, FakeType, operator, UnaryIntermediateKind)

	DEFINE_ENUM(UNARY_INTERMEDIATE_KINDS, UnaryIntermediateKind, unary intermediate kind)

#undef AUTOENUM_ROUTER
}

#endif
