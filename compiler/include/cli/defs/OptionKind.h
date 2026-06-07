#ifndef HAZE_OPTIONKIND_H
#define HAZE_OPTIONKIND_H

#include <utility/AutoEnum.h>

// Haze OptionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define OPTION_KINDS(X) \
	X(ARCHITECTURE, architecture) \
	X(VERBOSITY, verbosity) \
	X(OPTIMIZATION, optimization) \
	X(OUTPUT, output) \
	X(OUTFILE, outfile)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case OptionKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, OptionKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, OPTION_KINDS, OptionKind, option kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif 
