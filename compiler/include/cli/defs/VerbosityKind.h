#ifndef HAZE_VERBOSITYKIND_H
#define HAZE_VERBOSITYKIND_H

#include <utility/AutoEnum.h>

// Haze VerbosityKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VERBOSITY_KINDS(X) \
	X(SILENT, silent) \
	X(QUIET, quiet) \
	X(NORMAL, normal) \
	X(VERBOSE, verbose)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case VerbosityKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, VerbosityKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, VERBOSITY_KINDS, VerbosityKind, verbosity kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
