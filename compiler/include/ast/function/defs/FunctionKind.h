#ifndef HAZE_FUNCTIONKIND_H
#define HAZE_FUNCTIONKIND_H

#include <utility/AutoEnum.h>

// Haze FunctionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define FUNCTION_KINDS(X) \
	X(FUNCTION, Function, function)


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, FunctionKind)

	DEFINE_ENUM(FUNCTION_KINDS, FunctionKind, function kind)

#undef AUTOENUM_ROUTER
}

#endif
