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


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, IntermediateKind)

	DEFINE_ENUM(INTERMEDIATE_KINDS, IntermediateKind, intermediate kind)

#undef AUTOENUM_ROUTER
}

#endif 
