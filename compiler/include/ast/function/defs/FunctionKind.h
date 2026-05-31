#ifndef HAZE_FUNCTIONKIND_H
#define HAZE_FUNCTIONKIND_H

#include <utility/AutoEnum.h>

// Haze FunctionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define FUNCTION_KINDS(X) \
	X(FUNCTION, Function, function)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case FunctionKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, FUNCTION_KINDS, FunctionKind, function kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
