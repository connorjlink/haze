#ifndef HAZE_INTERMEDIATEKIND_H
#define HAZE_INTERMEDIATEKIND_H

// Haze IntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define INTERMEDIATE_KINDS(X) \
	X(ALLOCATE_STACK, allocate) \
	X(DEREFERENCE,    dereference) \
	X(REFERENCE,      reference) \
	X(IMMEDIATE,      immediate) \
	X(COPY,           copy) \
	X(BINARY,         binary) \
	X(UNARY,          unary) \
	X(COMPARE,        compare) \
	X(BRANCH,         branch) \
	X(CALL,           call) \
	X(RETURN,         return) \
	X(SIGN_EXTEND,    signextend) \
	X(ZERO_EXTEND,    zeroextend) \
	X(TRUNCATE,       truncate) \
	X(REINTERPRET,    reinterpret) \
	X(CAST,           cast) \
	X(OFFSETOF,       offset-of) \
	X(LABEL,          label)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case IntermediateKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, IntermediateKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, INTERMEDIATE_KINDS, IntermediateKind, intermediate kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER

#define INTERMEDIATE_TYPE_KINDS(X) \
	X(VOID, 0, void) \
	X(BOOL, 1, bool) \
	X(INT8, 8, int8) \
	X(INT16, 16, int16) \
	X(INT32, 32, int32) \
	X(FLOAT32, 32, float32) \
	X(FLOAT64, 64, float64) \
	X(FLOAT80, 80, float80) \
	X(POINTER, (sizeof(Address)* std::numeric_limits<unsigned char>::digits), pointer)

}

#endif 
