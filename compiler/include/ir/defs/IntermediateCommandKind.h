#ifndef HAZE_INTERMEDIATECOMMANDKIND_H
#define HAZE_INTERMEDIATECOMMANDKIND_H

// Haze IntermediateCommandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	//////////////////////////////////////////////////////
	// All Intermediate Command Kinds
	//////////////////////////////////////////////////////

#define INTERMEDIATE_COMMAND_KINDS(X) \
	X(ALLOCATE_STACK, AllocateIntermediateCommand,    allocate) \
	X(DEREFERENCE,    DereferenceIntermediateCommand, dereference) \
	X(REFERENCE,      ReferenceIntermediateCommand,   reference) \
	X(IMMEDIATE,      ImmediateIntermediateCommand,   immediate) \
	X(COPY,           CopyIntermediateCommand,        copy) \
	X(BINARY,         BinaryIntermediateCommand,      binary) \
	X(UNARY,          UnaryIntermediateCommand,       unary) \
	X(COMPARE,        CompareIntermediateCommand,     compare) \
	X(BRANCH,         BranchIntermediateCommand,      branch) \
	X(CALL,           CallIntermediateCommand,        call) \
	X(RETURN,         ReturnIntermediateCommand,      return) \
	X(SIGN_EXTEND,    SignExtendIntermediateCommand,  signextend) \
	X(ZERO_EXTEND,    ZeroExtendIntermediateCommand,  zeroextend) \
	X(TRUNCATE,       TruncateIntermediateCommand,    truncate) \
	X(REINTERPRET,    ReinterpretIntermediateCommand, reinterpret) \
	X(CAST,           CastIntermediateCommand,        cast) \
	X(OFFSETOF,       OffsetOfIntermediateCommand,    offset-of) \
	X(LABEL,          LabelIntermediateCommand,       label)


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, IntermediateCommandKind)

	DEFINE_ENUM(INTERMEDIATE_COMMAND_KINDS, IntermediateCommandKind, intermediate command kind)

#undef AUTOENUM_ROUTER


	//////////////////////////////////////////////////////
	// Unary Intermediate Command Kinds
	//////////////////////////////////////////////////////

#define UNARY_INTERMEDIATE_COMMAND_KINDS(X) \
	X(NEGATE,     -,    negate) \
	X(BOOL,       bool, bool) \
	X(BITWISE_NOT, ~,   bitwise not)


#define AUTOENUM_ROUTER(X, enumerator, operator, name) X(enumerator, FakeType, operator, UnaryIntermediateCommandKind)

	DEFINE_ENUM(UNARY_INTERMEDIATE_COMMAND_KINDS, UnaryIntermediateCommandKind, unary intermediate command kind)

#undef AUTOENUM_ROUTER


	//////////////////////////////////////////////////////
	// Binary Intermediate Command Kinds
	//////////////////////////////////////////////////////

#define BINARY_INTERMEDIATE_COMMAND_KINDS(X) \
	X(ADD,                  +,     add) \
	X(SUBTRACT,             -,     subtract) \
	X(MULTIPLY,             *,     multiply) \
	X(DIVIDE,               /,     divide) \
	X(UNSIGNED_DIVIDE,      u./,   unsigned divide) \
	X(MODULO,               %,     modulo) \
	X(UNSIGNED_MODULO,      u.%,   unsigned modulo) \
	X(AND,                  &,     bitwise and) \
	X(OR,                   |,     bitwise or ) \
	X(XOR,                  ^,     bitwise xor) \
	X(SHIFT_LEFT,           <<,    bitwise shift left) \
	X(SHIFT_RIGHT,          >>,    bitwise shift right) \
	X(UNSIGNED_SHIFT_RIGHT, u.>>, unsigned bitwise shift right)


#define AUTOENUM_ROUTER(X, enumerator, operator, name) X(enumerator, FakeType, operator, BinaryIntermediateCommandKind)

	DEFINE_ENUM(BINARY_INTERMEDIATE_COMMAND_KINDS, BinaryIntermediateCommandKind, binary intermediate command kind)

#undef AUTOENUM_ROUTER

	
	//////////////////////////////////////////////////////
	// Compare Intermediate Kinds
	//////////////////////////////////////////////////////

#define COMPARE_INTERMEDIATE_COMMAND_KINDS(X) \
	X(COMPARE_EQUAL,                     ==,   equal) \
	X(COMPARE_NOT_EQUAL,                 !=,   not equal) \
	X(COMPARE_LESS,                      <,    less) \
	X(COMPARE_LESS_EQUAL,             <=,   less than or equal) \
	X(COMPARE_GREATER,                   >,    greater) \
	X(COMPARE_GREATER_EQUAL,          >=,   greater than or equal) \
	X(COMPARE_UNSIGNED_LESS,             u.<,  unsigned less) \
	X(COMPARE_UNSIGNED_LESS_EQUAL,    u.<=, unsigned less than or equal) \
	X(COMPARE_UNSIGNED_GREATER,          u.>,  unsigned greater) \
	X(COMPARE_UNSIGNED_GREATER_EQUAL, u.>=, unsigned greater than or equal)


#define AUTOENUM_ROUTER(X, enumerator, operator, name) X(enumerator, FakeType, operator, CompareIntermediateCommandKind)

	DEFINE_ENUM(COMPARE_INTERMEDIATE_COMMAND_KINDS, CompareIntermediateCommandKind, compare intermediate command kind)

#undef AUTOENUM_ROUTER
}

#endif 
