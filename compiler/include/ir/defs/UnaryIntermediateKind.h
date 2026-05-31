#ifndef HAZE_UNARYINTERMEDIATEKIND_H
#define HAZE_UNARYINTERMEDIATEKIND_H

#include <utility/AutoEnum.h>

// Haze UnaryIntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define UNARY_INTERMEDIATE_KINDS(X) \
	X(NEGATE, -, negate) \
	X(BOOL, bool, bool) \
	X(BITWISE_NOT, ~, bitwise not)


#define ENUM_MEMBER(enumerator, operator_symbol, name) enumerator,
#define SWITCH_CASE(enumerator, operator_symbol, name) case UnaryIntermediateKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, UNARY_INTERMEDIATE_KINDS, UnaryIntermediateKind, unary intermediate kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
