#ifndef HAZE_BINARYINTERMEDIATEKIND_H
#define HAZE_BINARYINTERMEDIATEKIND_H

#include <utility/AutoEnum.h>

// Haze BinaryIntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define BINARY_INTERMEDIATE_KINDS(X) \
	X(ADD, +, add) \
	X(SUBTRACT, -, subtract) \
	X(MULTIPLY, *, multiply) \
	X(DIVIDE, /, divide) \
	X(UNSIGNED_DIVIDE, u./ , unsigned divide) \
	X(MODULO, %, modulo) \
	X(UNSIGNED_MODULO, u.%, unsigned modulo) \
	X(AND, &, bitwise and) \
	X(OR, |, bitwise or ) \
	X(XOR, ^, bitwise xor) \
	X(SHIFT_LEFT, <<, bitwise shift left) \
	X(SHIFT_RIGHT, >>, bitwise shift right) \
	X(UNSIGNED_SHIFT_RIGHT, u.>> , unsigned bitwise shift right)


#define ENUM_MEMBER(enumerator, operator, name) enumerator,
#define SWITCH_CASE(enumerator, operator, name) case BinaryIntermediateKind::enumerator: return #name;
	
	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, BINARY_INTERMEDIATE_KINDS, BinaryIntermediateKind, binary intermediate kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
