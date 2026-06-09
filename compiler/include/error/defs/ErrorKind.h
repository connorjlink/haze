#ifndef HAZE_ERRORKIND_H
#define HAZE_ERRORKIND_H

#include <utility/AutoEnum.h>

// Haze ErrorKind.h
// (c) Connor J. Link. All Rights Reserved.

// weird Windows.h stuff... again...
#undef ERROR

namespace hz
{
#define ERROR_KINDS(X) \
	X(OUTPUT,        output) \
	X(INFORMATION,   information) \
	X(WARNING,       warning) \
	X(ERROR,         error) \
	X(UNCORRECTABLE, uncorrectable internal error)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, ErrorKind)

	DEFINE_ENUM(ERROR_KINDS, ErrorKind, error kind)

#undef AUTOENUM_ROUTER
}

#endif
