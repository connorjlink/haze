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
	X(EXECUTION, execution) \
	X(OPTIMIZATION, optimization) \
	X(OUTPUT, output) \
	X(OUTFILE, outfile)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case OptionKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, OPTION_KINDS, OptionKind, option kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif 
