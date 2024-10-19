import std;

#include "IntegerLiteral.h"
#include "CommonErrors.h"

// Haze IntegerLiteral.h
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	template<typename T>
	T copy(T* input)
	{
		return T{ *input };
	}
}

namespace hz
{
	IntegerLiteralType UnsignedByteIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UBYTE;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_UNSIGNED_BYTE(rhs).value;
		return *this;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedByteIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_UNSIGNED_BYTE(rhs).value;
		return copy;
	}


	IntegerLiteralType SignedByteIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SBYTE;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_SIGNED_BYTE(rhs).value;
		return *this;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_SIGNED_BYTE(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedByteIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_SIGNED_BYTE(rhs).value;
		return copy;
	}



	IntegerLiteralType UnsignedWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UWORD;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_UNSIGNED_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_UNSIGNED_WORD(rhs).value;
		return copy;
	}



	IntegerLiteralType SignedWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SWORD;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_SIGNED_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_SIGNED_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_SIGNED_WORD(rhs).value;
		return copy;
	}



	IntegerLiteralType UnsignedDoubleWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UDWORD;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedDoubleWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_UNSIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}



	IntegerLiteralType SignedDoubleWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SDWORD;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_SIGNED_DOUBLE_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedDoubleWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_SIGNED_DOUBLE_WORD(rhs).value;
		return copy;
	}



	IntegerLiteralType UnsignedQuadWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::UQWORD;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_UNSIGNED_QUAD_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& UnsignedQuadWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_UNSIGNED_QUAD_WORD(rhs).value;
		return copy;
	}



	IntegerLiteralType SignedQuadWordIntegerLiteral::itype() const
	{
		return IntegerLiteralType::SQWORD;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator+(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value += AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator-(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value -= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator*(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value *= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator&(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value &= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator|(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value |= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator^(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value ^= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator>>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value >>= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator<<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value <<= AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator>(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value > AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator<(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value < AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator=(IntegerLiteral& rhs)
	{
		value = AS_SIGNED_QUAD_WORD(rhs).value;
		return *this;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator==(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value == AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}

	IntegerLiteral& SignedQuadWordIntegerLiteral::operator!=(IntegerLiteral& rhs)
	{
		auto copy = ::copy(this);
		copy.value = copy.value != AS_SIGNED_QUAD_WORD(rhs).value;
		return copy;
	}
}
