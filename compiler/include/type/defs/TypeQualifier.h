#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>
#include <utility/AutoBitfield.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

// weird Windows.h stuff... again...
#undef CONST

namespace hz
{
#define TYPE_QUALIFIERS(X) \
	X(NONE,     none,          0) \
	X(CONST,    const,    1 << 0) \
	X(VOLATILE, volatile, 1 << 1) \
	X(RESTRICT, restrict, 1 << 2)


#define AUTOBITFIELD_ROUTER(X, enumerator, name, value) X(enumerator, name, value)

	DEFINE_BITFIELD(TYPE_QUALIFIERS, TypeQualifier, type qualifier)

#undef AUTOBITFIELD_ROUTER
}

#endif
