#ifndef HAZE_OPTIONKIND_H
#define HAZE_OPTIONKIND_H

#include <utility/AutoEnum.h>

// Haze OptionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define OPTION_KINDS(X) \
	X(ARCHITECTURE, architecture) \
	X(VERBOSITY,    verbosity) \
	X(OPTIMIZATION, optimization) \
	X(OUTPUT,       output) \
	X(OUTFILE,      outfile)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, OptionKind)

	DEFINE_ENUM(OPTION_KINDS, OptionKind, option kind)

#undef AUTOENUM_ROUTER
}

#endif 
