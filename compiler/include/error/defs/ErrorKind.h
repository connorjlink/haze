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
	X(OUTPUT, output) \
	X(INFORMATION, information) \
	X(WARNING, warning) \
	X(ERROR, error) \
	X(UNCORRECTABLE, uncorrectable internal error)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case ErrorKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, ErrorKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, ERROR_KINDS, ErrorKind, error kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
