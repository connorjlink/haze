#ifndef HAZE_UNARYINTERMEDIATEKIND_H
#define HAZE_UNARYINTERMEDIATEKIND_H

#include <utility/AutoEnum.h>

// Haze UnaryIntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define UNARY_INTERMEDIATE_KINDS(X) \
	X(NEGATE,     -,    negate) \
	X(BOOL,       bool, bool) \
	X(BITWISE_NOT, ~,   bitwise not)


#define ENUM_MEMBER(enumerator, operator, name) enumerator,
#define SWITCH_CASE(enumerator, operator, name) case UnaryIntermediateKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, operator, name) Mapping{ #name, UnaryIntermediateKind::enumerator },
#define FORWARD_DECLARATION(enumerator, operator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, UNARY_INTERMEDIATE_KINDS, UnaryIntermediateKind, unary intermediate kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
