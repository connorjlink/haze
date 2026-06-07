#ifndef HAZE_COMPAREINTERMEDIATEKIND_H
#define HAZE_COMPAREINTERMEDIATEKIND_H

#include <utility/AutoEnum.h>

// Haze CompareIntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define COMPARE_INTERMEDIATE_KINDS(X) \
	X(COMPARE_EQUAL, ==, equal) \
	X(COMPARE_NOT_EQUAL, !=, not equal) \
	X(COMPARE_LESS, <, less) \
	X(COMPARE_LESS_OR_EQUAL, <=, less than or equal) \
	X(COMPARE_GREATER, >, greater) \
	X(COMPARE_GREATER_OR_EQUAL, >=, greater than or equal) \
	X(COMPARE_UNSIGNED_LESS, u.<, unsigned less) \
	X(COMPARE_UNSIGNED_LESS_OR_EQUAL, u.<=, unsigned less than or equal) \
	X(COMPARE_UNSIGNED_GREATER, u.>, unsigned greater) \
	X(COMPARE_UNSIGNED_GREATER_OR_EQUAL, u.>=, unsigned greater than or equal)


#define ENUM_MEMBER(enumerator, operator, name) enumerator,
#define SWITCH_CASE(enumerator, operator, name) case CompareIntermediateKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, operator, name) Mapping{ #name, CompareIntermediateKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, COMPARE_INTERMEDIATE_KINDS, CompareIntermediateKind, compare intermediate kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
