#ifndef HAZE_VERBOSITYKIND_H
#define HAZE_VERBOSITYKIND_H

#include <utility/AutoEnum.h>

// Haze VerbosityKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VERBOSITY_KINDS(X) \
	X(SILENT,  silent) \
	X(QUIET,   quiet) \
	X(NORMAL,  normal) \
	X(VERBOSE, verbose)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, VerbosityKind)

	DEFINE_ENUM(VERBOSITY_KINDS, VerbosityKind, verbosity kind)

#undef AUTOENUM_ROUTER
}

#endif
