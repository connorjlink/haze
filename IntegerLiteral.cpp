import std;

#include "IntegerLiteral.h"
#include "CommonErrors.h"

// Haze IntegerLiteral.h
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	template<typename T>
	T* copy(T* input)
	{
		return new T{ *input };
	}
}

namespace hz
{
	IntegerLiteralType UnsignedByteIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UBYTE;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedByteIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_UNSIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType SignedByteIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SBYTE;
	}

	IntegerLiteral* SignedByteIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* SignedByteIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedByteIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_SIGNED_BYTE_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType UnsignedWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UWORD;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_UNSIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType SignedWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SWORD;
	}

	IntegerLiteral* SignedWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* SignedWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_SIGNED_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType UnsignedDoubleWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UDWORD;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedDoubleWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType SignedDoubleWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SDWORD;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedDoubleWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType UnsignedQuadWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UQWORD;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* UnsignedQuadWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}



	IntegerLiteralType SignedQuadWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SQWORD;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::plus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value += AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::minus(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value -= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::times(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value *= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::bitwiseor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value |= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::bitwisexor(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value ^= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::bitwiseand(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value &= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::bitwisershift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value >>= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::bitwiselshift(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value <<= AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::greater(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value > AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::less(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value < AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::assign(IntegerLiteral* rhs)
	{
		value = AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return this;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::equals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value == AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}

	IntegerLiteral* SignedQuadWordIntegerLiteral::notequals(IntegerLiteral* rhs)
	{
		auto copy = ::copy(this);
		copy->value = copy->value != AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(rhs)->value;
		return copy;
	}
}
